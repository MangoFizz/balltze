// SPDX-License-Identifier: GPL-3.0-only

#include <windows.h>
#include <filesystem>
#include <vector>
#include <map>
#include <deque>
#include <memory>
#include <functional>
#include <numeric>
#include <optional>

#include <balltze/engine.hpp>
#include <balltze/utils.hpp>
#include <balltze/command.hpp>
#include <balltze/event.hpp>
#include <balltze/features.hpp>
#include <balltze/memory.hpp>
#include <balltze/hook.hpp>
#include "../../config/config.hpp"
#include "../../plugins/loader.hpp"
#include "../../logger.hpp"
#include "map.hpp"

namespace Balltze::Features {
    using namespace Engine;
    using namespace Engine::TagDefinitions;

    constexpr std::size_t virtual_tag_data_buffer_size = 32 * MIB_SIZE;

    struct SecondaryMap;

    static std::string loaded_map_name;
    static std::unique_ptr<MapHeader> loaded_map_header;
    static std::unique_ptr<TagDataHeader> loaded_map_tag_data_header;
    static std::unique_ptr<std::byte[]> loaded_map_tag_data;
    static std::vector<std::shared_ptr<SecondaryMap>> secondary_maps;
    
    static std::string prepared_map_name;
    static std::unique_ptr<MapHeader> prepared_map_header;
    static std::vector<std::shared_ptr<SecondaryMap>> prepared_secondary_maps;

    static std::vector<Tag> virtual_tag_array;
    static std::unique_ptr<std::byte[]> virtual_tag_data;
    static std::size_t virtual_tag_data_cursor = 0;

    static bool tag_class_is_supported(TagClassInt tag_class) noexcept {
        static TagClassInt unsupportedTags[] = {
            TAG_CLASS_GLOBALS,
            TAG_CLASS_HUD_GLOBALS,
            TAG_CLASS_METER,
            TAG_CLASS_SCENARIO_STRUCTURE_BSP,
            TAG_CLASS_SCENARIO,
        };

        for(auto &i : unsupportedTags) {
            if(i == tag_class) {
                return false;
            }
        }
        return true;
    }

    std::byte *reserve_tag_data_space(std::size_t size) {
        auto *data = virtual_tag_data.get() + virtual_tag_data_cursor;
        virtual_tag_data_cursor += size;
        return data;
    }

    Tag &insert_tag_entry(Tag const &entry) {
        auto previous_handle = virtual_tag_array.back().handle;
        auto &new_entry = virtual_tag_array.emplace_back(entry);
        new_entry.handle.index = previous_handle.index + 1;
        new_entry.handle.id = previous_handle.id + 1;

        // Update tag data header
        auto &tag_data_header = get_tag_data_header();
        tag_data_header.tag_count++;
        tag_data_header.tag_array = virtual_tag_array.data();

        return virtual_tag_array.back();
    }

    struct SecondaryMap {
        std::string name;
        std::filesystem::path path;
        MapHeader header;
        std::unique_ptr<std::byte[]> raw_tag_data;
        TagDataHeader *tag_data_header;
        Tag *tag_array;
        std::vector<std::pair<std::string, TagClassInt>> tags_to_load;
        std::map<char *, char *> tag_path_translations;
        std::map<TagHandle, TagHandle> tag_handles_translations;
        std::map<TagHandle, std::vector<TagHandle>> tags_copies;
        bool load_all_tags = false;

        std::size_t size() {
            std::size_t count = 0;
            count += sizeof(MapHeader);
            count += header.tag_data_size;
            return count;
        }

        const Tag *get_tag(TagHandle tag_handle) {
            for(std::size_t i = 0; i < tag_data_header->tag_count; i++) {
                if(tag_array[i].handle == tag_handle) {
                    return &tag_array[i];
                }
            }
            return nullptr;
        }

        template<typename T>
        T translate_address(T address) {
            if(address != 0) {
                auto base_address_disp = reinterpret_cast<std::uint32_t>(get_tag_data_address()) - reinterpret_cast<std::uint32_t>(raw_tag_data.get());
                address = reinterpret_cast<T>(reinterpret_cast<std::uint32_t>(address) - base_address_disp);
            }
            return address;
        }

        char *translate_tag_path(char *path) noexcept {
            if(path == nullptr) {
                return nullptr;
            }

            // Check if path is already translated
            if(tag_path_translations.find(path) != tag_path_translations.end()) {
                return tag_path_translations[path];
            }

            // Copy path to tag data buffer
            auto *new_path = std::strcpy(reinterpret_cast<char *>(virtual_tag_data.get() + virtual_tag_data_cursor), path);
            virtual_tag_data_cursor += std::strlen(path) + 1;

            tag_path_translations.insert_or_assign(path, new_path);
            return new_path;
        }

        std::uint32_t get_data_base_offset() {
            std::uint32_t offset = loaded_map_header->file_size;
            for(auto &map : secondary_maps) {
                if(this == map.get()) {
                    break;
                }
                offset += map->header.file_size;
            }
            return offset;
        }

        std::uint32_t get_model_data_base_offset() {
            std::uint32_t offset = loaded_map_tag_data_header->model_data_size;
            for(auto &map : secondary_maps) {
                if(this == map.get()) {
                    break;
                }
                offset += map->tag_data_header->model_data_size;
            }
            return offset;
        }

        TagHandle load_tag(const Tag *tag, bool required) {
            // Check if current tag class is supported
            if(!tag_class_is_supported(tag->primary_class)) {
                if(required) {
                    show_error_box("Unsupported tag class %.*s", 4, reinterpret_cast<const char *>(&tag->primary_class));
                    std::exit(EXIT_FAILURE);
                }
                return TagHandle::null();
            }

            auto data_base_offset = get_data_base_offset();
            auto model_data_base_offset = get_model_data_base_offset();

            // Check if we've already loaded this tag
            if(tag_handles_translations.find(tag->handle) != tag_handles_translations.end()) {
                return tag_handles_translations.find(tag->handle)->second;
            }

            // Set up new tag entry
            auto &new_tag_entry = insert_tag_entry(*tag);
            new_tag_entry.path = translate_tag_path(translate_address(new_tag_entry.path));
            tag_handles_translations.insert_or_assign(tag->handle, new_tag_entry.handle);

            // if current tag are indexed or if tags are already fixed, we can continue
            if(tag->indexed) {
                if(new_tag_entry.primary_class == TAG_CLASS_SOUND) {
                    new_tag_entry.data = translate_address(new_tag_entry.data);
                }
                return new_tag_entry.handle;
            }

            // There's no data loaded to fix... yet
            if(tag->primary_class == TAG_CLASS_SCENARIO_STRUCTURE_BSP) {
                return new_tag_entry.handle;
            }

            new_tag_entry.fix_data_offsets(translate_address(new_tag_entry.data), [&](std::uint32_t offset) -> std::uint32_t {
                return new_tag_entry.indexed ? offset : offset + data_base_offset;
            });

            new_tag_entry.data = new_tag_entry.copy_data([](std::byte *data, std::size_t size) -> std::byte * {
                auto *new_data = reserve_tag_data_space(size);
                std::memcpy(new_data, data, size);
                return new_data;
            });

            if(!new_tag_entry.data) {
                logger.fatal("Failed to copy data for tag {} of class {}", new_tag_entry.path, tag_class_to_string(new_tag_entry.primary_class));
                std::exit(EXIT_FAILURE);
            }

            new_tag_entry.fix_dependencies([this](TagHandle tag_handle, bool self_reference) -> TagHandle {
                auto *broken_tag = this->get_tag(tag_handle);
                if(broken_tag) {
                    auto new_tag_handle = this->load_tag(broken_tag, true);
                    return new_tag_handle;
                }
                else {
                    if(tag_handle != TagHandle::null()) {
                        logger.debug("Cannot resolve tag {} in map {}", tag_handle.handle, name);
                    }
                    return tag_handle;
                }
            });

            switch(new_tag_entry.primary_class) {
                case TAG_CLASS_BITMAP: {
                    Bitmap *bitmap = reinterpret_cast<Bitmap *>(new_tag_entry.data);
                    if(bitmap->bitmap_data.count > 0) {
                        for(std::size_t j = 0; j < bitmap->bitmap_data.count; j++) {
                            bitmap->bitmap_data.offset[j].pixel_data_offset += data_base_offset; 
                        }
                    }
                    break;
                }

                case TAG_CLASS_FONT: {
                    auto *font = reinterpret_cast<Font *>(new_tag_entry.data);
                    for(std::size_t i = 0; i < font->characters.count; i++) {
                        font->characters.offset[i].pixels_offset += data_base_offset;
                    }
                    break;
                }

                case TAG_CLASS_GBXMODEL: {
                    auto *gbxmodel = reinterpret_cast<Gbxmodel *>(new_tag_entry.data);
                    for(std::size_t i = 0; i < gbxmodel->geometries.count; i++) {
                        for(std::size_t j = 0; j < gbxmodel->geometries.offset[i].parts.count; j++) {
                            gbxmodel->geometries.offset[i].parts.offset[j].vertex_offset += model_data_base_offset;
                            gbxmodel->geometries.offset[i].parts.offset[j].triangle_offset += model_data_base_offset - loaded_map_tag_data_header->vertex_size + tag_data_header->vertex_size;
                            gbxmodel->geometries.offset[i].parts.offset[j].triangle_offset_2 += model_data_base_offset - loaded_map_tag_data_header->vertex_size + tag_data_header->vertex_size;
                        }                               
                    }
                    break;
                }

                default: {
                    break;
                }
            }

            return new_tag_entry.handle;
        }

        void read_map_data() {
            logger.info("Reading tag data for secondary map {}", this->name);

            std::FILE *file = std::fopen(this->path.string().c_str(), "rb");
            std::fread(&this->header, sizeof(MapHeader), 1, file);

            if(header.engine_type == CACHE_FILE_CUSTOM_EDITION_COMPRESSED) {
                std::fclose(file);
                throw std::runtime_error("Map file is compressed");
            } 

            if(header.engine_type != CACHE_FILE_CUSTOM_EDITION) {
                std::fclose(file);
                throw std::runtime_error("Map file is not a Halo Custom Edition map");
            }

            raw_tag_data = std::make_unique<std::byte[]>(header.tag_data_size);
            std::fseek(file, header.tag_data_offset, SEEK_SET);
            std::fread(raw_tag_data.get(), 1, header.tag_data_size, file);
            std::fclose(file);

            tag_data_header = reinterpret_cast<TagDataHeader *>(raw_tag_data.get());
            tag_array = reinterpret_cast<Tag *>(raw_tag_data.get() + sizeof(TagDataHeader));
        }

        SecondaryMap(const std::string name) {
            this->name = name;

            try {
                this->path = path_for_map_local(name.c_str());
            }
            catch (std::runtime_error &e) {
                throw;
            }

            read_map_data();
        }

        SecondaryMap(std::filesystem::path path) {
            this->name = path.stem().string();
            this->path = path;

            if(!std::filesystem::exists(path)) {
                throw std::runtime_error("Map file does not exist");
            }

            read_map_data();
        }
    };

    static void load_tag_data() noexcept {
        auto &tag_data_header = get_tag_data_header();
        auto *tag_data_address = get_tag_data_address();

        // Start timestamp
        std::chrono::time_point start = std::chrono::steady_clock::now();

        // Initialize our stuff
        secondary_maps = prepared_secondary_maps;
        loaded_map_name = std::move(prepared_map_name);
        loaded_map_header = std::move(prepared_map_header);
        loaded_map_tag_data_header = std::make_unique<TagDataHeader>(tag_data_header);
        loaded_map_tag_data = std::make_unique<std::byte[]>(loaded_map_header->tag_data_size);
        std::memcpy(loaded_map_tag_data.get(), tag_data_address, loaded_map_header->tag_data_size);

        // Initialize virtual tag data
        virtual_tag_array = std::vector<Tag>(tag_data_header.tag_array, tag_data_header.tag_array + tag_data_header.tag_count);
        virtual_tag_data = std::make_unique<std::byte[]>(virtual_tag_data_buffer_size);
        virtual_tag_data_cursor = 0;

        for(auto &map : secondary_maps) {
            auto map_tag_data_header = map->tag_data_header;

            // Reserve space for tags to AVOID REALLOCATIONS during the process
            virtual_tag_array.reserve(virtual_tag_array.size() + (map->load_all_tags ? map_tag_data_header->tag_count : map->tags_to_load.size()));

            if(map->load_all_tags) {
                for(std::size_t i = 0; i < map_tag_data_header->tag_count; i++) {
                    map->load_tag(map->tag_array + i, false);
                }
            }
            else {
                for(auto &tag : map->tags_to_load) {
                    bool tag_found = false;
                    for(std::size_t i = 0; i < map_tag_data_header->tag_count; i++) {
                        std::string path = map->translate_address(map->tag_array[i].path);
                        if(path == tag.first && (map->tag_array[i].primary_class == tag.second || tag.second == TAG_CLASS_NULL)) {
                            if(map->load_tag(map->tag_array + i, false) != TagHandle::null()) {
                                tag_found = true;
                            }
                            break;
                        }
                    }
                    if(!tag_found) {
                        logger.warning("Tag {} of class {} not found in map {}", tag.first, tag_class_to_string(tag.second), map->name);
                    }
                }
            }
        }

        tag_data_header.tag_array = virtual_tag_array.data();

        // End timestamp
        std::chrono::time_point end = std::chrono::steady_clock::now();

        logger.info("Loaded {} tags in {} ms", virtual_tag_array.size() - loaded_map_tag_data_header->tag_count, std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
    }

    void import_tag_from_map(std::string map_name, std::string tag_path, TagClassInt tag_class) {
        try {
            for(auto &map : prepared_secondary_maps) {
                if(map->name == map_name) {
                    for(auto &tag : map->tags_to_load) {
                        if(tag.first == tag_path && tag.second == tag_class) {
                            return;
                        }
                    }
                    map->tags_to_load.emplace_back(tag_path, tag_class);
                    return;
                }
            }
            auto &map = prepared_secondary_maps.emplace_back(std::make_shared<SecondaryMap>(map_name));
            map->tags_to_load.emplace_back(tag_path, tag_class);
        }
        catch(std::exception &e) {
            logger.error("Failed to import tag {} from map {}: {}", tag_path, map_name, e.what());
        }
    }

    void import_tag_from_map(std::filesystem::path map_file, std::string tag_path, TagClassInt tag_class) {
        try {
            for(auto &map : prepared_secondary_maps) {
                if(map->path == map_file) {
                    for(auto &tag : map->tags_to_load) {
                        if(tag.first == tag_path && tag.second == tag_class) {
                            return;
                        }
                    }
                    map->tags_to_load.emplace_back(tag_path, tag_class);
                    return;
                }
            }
            auto &map = prepared_secondary_maps.emplace_back(std::make_shared<SecondaryMap>(map_file));
            map->tags_to_load.emplace_back(tag_path, tag_class);
        }
        catch(std::exception &e) {
            logger.debug("Failed to import tag {} from map {}: {}", tag_path, map_file.string(), e.what());
            throw;
        }
    }

    void import_tags_from_map(std::filesystem::path map_file) {
        try {
            for(auto &map : prepared_secondary_maps) {
                if(map->path == map_file) {
                    return;
                }
            }
            auto &map = prepared_secondary_maps.emplace_back(std::make_shared<SecondaryMap>(map_file));
            map->load_all_tags = true;
        }
        catch(std::runtime_error &e) {
            logger.debug("Failed to import tags from map {}: {}", map_file.string(), e.what());
            throw;
        }
    }

    void on_map_file_load(Event::MapFileLoadEvent const &event) {
        if(event.time == Event::EVENT_TIME_BEFORE) {
            logger.info("Preparing to load map {}", event.args.map_name);

            // Read next map data
            prepared_map_name = event.args.map_name;
            auto map_path = path_for_map_local(event.args.map_name.c_str());
            prepared_map_header = std::make_unique<MapHeader>();
            std::FILE *file = std::fopen(map_path.string().c_str(), "rb");
            std::fread(prepared_map_header.get(), sizeof(MapHeader), 1, file);
            std::fclose(file);

            if(prepared_map_header->head != MapHeader::HEAD_LITERAL || prepared_map_header->foot != MapHeader::FOOT_LITERAL) {
                throw std::runtime_error("Map file is corrupted");
            }

            // Clear prepared secondary maps
            prepared_secondary_maps.clear();
        }
    }

    void on_read_map_file_data(Event::MapFileDataReadEvent &event) {      
        if(secondary_maps.empty() && prepared_secondary_maps.empty()) {
            return;
        }

        const HANDLE &file_descriptor = event.args.file_handle; 
        std::byte *output = event.args.output_buffer;
        const std::size_t &size = event.args.size;
        std::size_t file_offset = event.args.overlapped->Offset;

        // Get the name of the file we're reading from
        char file_path_chars[MAX_PATH + 1] = {};
        GetFinalPathNameByHandle(file_descriptor, file_path_chars, sizeof(file_path_chars) - 1, VOLUME_NAME_NONE);
        auto file_path = std::filesystem::path(file_path_chars);
        auto file_name = file_path.filename();

        if(event.time == Event::EVENT_TIME_BEFORE) {
            if(file_name.stem().string() != loaded_map_name) {
                return;
            }

            // Look for secondary maps offset displacement
            auto offset_acc = loaded_map_header->file_size;
            if(file_offset > offset_acc) {
                for(auto &map : secondary_maps) {
                    auto map_file_size = map->header.file_size;
                    if(file_offset <= offset_acc + map_file_size) {
                        std::FILE *file = std::fopen(map->path.string().c_str(), "rb");
                        std::fseek(file, file_offset - offset_acc, SEEK_SET);
                        std::fread(output, 1, size, file);
                        std::fclose(file);
                        event.args.size = 0;
                        return;
                    }
                    offset_acc += map_file_size;
                }
                show_error_box("File offset %zu is out of bounds! Map file size: %zu", file_offset, offset_acc);
            }

            // Read model data ONLY for secondary maps
            const auto &tag_data_header = get_tag_data_header();
            if(tag_data_header.model_data_file_offset == file_offset && tag_data_header.model_data_size == size) {
                logger.info("Loading model data from secondary maps...");
                std::size_t buffer_cursor = tag_data_header.model_data_size;
                for(auto &map : secondary_maps) {
                    auto &map_tag_data_header = map->tag_data_header;
                    if(map_tag_data_header) {
                        std::FILE *file = std::fopen(map->path.string().c_str(), "rb");
                        std::fseek(file, map_tag_data_header->model_data_file_offset, SEEK_SET);
                        std::fread(output + buffer_cursor, 1, map_tag_data_header->model_data_size, file);
                        std::fclose(file);
                        
                        buffer_cursor += map_tag_data_header->model_data_size;
                    }
                    else {
                        logger.warning("Cannot load model data from map {} because it has no tag data header", map->name);
                    }
                }
            }
        }
        else {
            if(event.args.output_buffer == get_tag_data_address()) {
                // Check if we're reading from the next map
                if(file_name.stem().string() != prepared_map_name) {
                    logger.warning("Reading tag data from map {} instead of {}", file_name.stem().string(), prepared_map_name);
                    prepared_map_name = file_name.stem().string();
                }

                logger.info("Loading tag data from secondary maps...");
                load_tag_data();
            }
        }
    }

    void reload_tag_data(TagHandle tag_handle) {
        auto *tag = get_tag(tag_handle);

        // If tag is from the loaded map, create a copy of it in the virtual tag data
        auto *tag_data_address = get_tag_data_address();
        if(tag->handle.index < loaded_map_tag_data_header->tag_count) {
            auto *raw_tag_array = reinterpret_cast<Tag *>(loaded_map_tag_data.get() + sizeof(TagDataHeader));
            auto *raw_tag = raw_tag_array + tag->handle.index;

            if(tag->primary_class != raw_tag->primary_class || std::strcmp(tag->path, raw_tag->path) != 0) {
                throw std::runtime_error("Tag mismatch");
            }

            if(tag->data >= tag_data_address && tag->data < tag_data_address + loaded_map_header->tag_data_size) {
                auto translate_address = [](auto address) {
                    if(address != 0) {
                        auto base_address_disp = reinterpret_cast<std::uint32_t>(get_tag_data_address()) - reinterpret_cast<std::uint32_t>(loaded_map_tag_data.get());
                        address = reinterpret_cast<decltype(address)>(reinterpret_cast<std::uint32_t>(address) - base_address_disp);
                    }
                    return address;
                };

                raw_tag->fix_data_offsets(translate_address(raw_tag->data), std::nullopt);
                
                tag->data = raw_tag->copy_data([](std::byte *data, std::size_t size) -> std::byte * {
                    auto *new_data = reserve_tag_data_space(size);
                    std::memcpy(new_data, data, size);
                    return new_data;
                });
            }
            else {
                auto *tag_data = tag->data;
                tag->data = raw_tag->copy_data([&tag_data](std::byte *data, std::size_t size) -> std::byte * {
                    std::memcpy(tag_data, data, size);
                    tag_data += size;
                    return tag_data;
                });
            }
        }
        else {
            Tag *raw_tag = nullptr;
            for(auto &map : secondary_maps) {
                for(auto [origin_tag_handle, virtual_tag_handle] : map->tag_handles_translations) {
                    if(virtual_tag_handle == tag_handle) {
                        if(tag->primary_class == raw_tag->primary_class && std::strcmp(tag->path, raw_tag->path) == 0) {
                            raw_tag = const_cast<Tag *>(map->get_tag(origin_tag_handle));
                            break;
                        }
                        else {
                            throw std::runtime_error("Tag mismatch");
                        }
                    }
                }
                if(raw_tag) {
                    break;
                }
            }

            if(!raw_tag) {
                throw std::runtime_error("Tag not found");
            }

            auto *tag_data = tag->data;
            tag->data = raw_tag->copy_data([&tag_data](std::byte *data, std::size_t size) -> std::byte * {
                std::memcpy(tag_data, data, size);
                tag_data += size;
                return tag_data;
            });
        }
    }

    void replace_tag_dependencies(TagHandle tag_handle, TagHandle new_tag_handle) {
        for(auto &tag : virtual_tag_array) {
            tag.fix_dependencies([tag_handle, new_tag_handle](TagHandle dependency_handle, bool self_reference) -> TagHandle {
                if(dependency_handle == tag_handle && !self_reference) {
                    return new_tag_handle;
                }
                return dependency_handle;
            });
        }
    }

    extern "C" {
        void on_model_data_buffer_alloc_asm();
    
        void on_model_data_buffer_alloc(std::size_t *size) {
            logger.info("Allocating model data buffer for secondary maps...");
            for(auto &map : prepared_secondary_maps) {
                *size += map->tag_data_header->model_data_size;
            }
        }
    }

    void set_up_tag_data_importing() {
        Event::MapFileLoadEvent::subscribe_const(on_map_file_load, Event::EVENT_PRIORITY_HIGHEST);
        Event::MapFileDataReadEvent::subscribe(on_read_map_file_data);

        auto *model_data_buffer_alloc_sig = Memory::get_signature("model_data_buffer_alloc");
        auto *model_data_buffer_alloc_hook = Memory::hook_function(model_data_buffer_alloc_sig->data(), on_model_data_buffer_alloc_asm);

        register_command("imported_tag_data_info", "debug", "Prints the details of the imported tag data from secondary maps", std::nullopt, [](int arg_count, const char **args) -> bool {
            if(secondary_maps.empty()) {
                Engine::console_print("No tag data imported from secondary maps");
                return true;
            }
            std::string maps_loaded;
            std::size_t cached_data = 0;
            for(std::size_t i = 0; i < secondary_maps.size(); i++) {
                auto &map = secondary_maps[i];
                maps_loaded += map->name;
                cached_data += map->size();
                if(i < secondary_maps.size() - 1) {
                    maps_loaded += ", ";
                }
            }
            Engine::console_print("Imported tag data summary");
            Engine::console_printf("Maps loaded: %zu (%s)", secondary_maps.size(), maps_loaded.c_str());
            Engine::console_printf("Tags imported: %zu", virtual_tag_array.size() - loaded_map_tag_data_header->tag_count);
            Engine::console_printf("Imported tag data size: %.2fMiB / %.2fMiB", static_cast<float>(virtual_tag_data_cursor) / MIB_SIZE, static_cast<float>(virtual_tag_data_buffer_size) / MIB_SIZE);
            Engine::console_printf("Cached tag data size: %.2f MiB", static_cast<float>(cached_data) / MIB_SIZE);
            return true;
        }, false, 0, 0);
    }
}
