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

#include <balltze/legacy_api/engine.hpp>
#include <balltze/utils.hpp>
#include <balltze/command.hpp>
#include <balltze/legacy_api/event.hpp>
#include <balltze/features.hpp>
#include <balltze/memory.hpp>
#include <balltze/hook.hpp>
#include "../../config/config.hpp"
#include "../../plugins/loader.hpp"
#include "../../logger.hpp"
#include "map.hpp"
#include "tags_handling.hpp"

namespace Balltze::Features {
    namespace fs = std::filesystem;
    using namespace LegacyApi::Engine;
    using namespace LegacyApi::Engine::TagDefinitions;

    class VirtualTagData;
    class MapCache;
    class SecondaryMapCache;

    static fs::path map_file_path;
    static std::unique_ptr<MapCache> map_cache;
    static std::vector<std::shared_ptr<SecondaryMapCache>> secondary_maps_cache;
    static std::vector<std::shared_ptr<SecondaryMapCache>> preloaded_secondary_maps_cache;
    static std::unique_ptr<VirtualTagData> virtual_tag_data;

    constexpr std::size_t virtual_tag_data_buffer_size = 64 * MIB_SIZE;

    class VirtualTagData {
    private:
        std::vector<Tag> m_tag_array;
        std::unique_ptr<std::byte[]> m_tag_data;
        std::size_t m_tag_data_cursor = 0;
        TagHandle m_next_handle;

    public:
        VirtualTagData() {
            m_tag_data = std::make_unique<std::byte[]>(virtual_tag_data_buffer_size);
            m_next_handle.index = 0;
            m_next_handle.id = 0xE174;
        }

        VirtualTagData(VirtualTagData &&other) {
            m_tag_array = std::move(other.m_tag_array);
            m_tag_data = std::move(other.m_tag_data);
            m_tag_data_cursor = other.m_tag_data_cursor;
            m_next_handle = other.m_next_handle;

            other.m_tag_data_cursor = 0;
            other.m_next_handle.index = 0;
            other.m_next_handle.id = 0xE174;
        }

        std::size_t tag_data_size() const noexcept {
            return m_tag_data_cursor;
        }

        std::size_t tag_count() const noexcept {
            return m_tag_array.size();
        }

        std::byte *reserve_tag_data_space(std::size_t size) noexcept {
            if(m_tag_data_cursor + size > virtual_tag_data_buffer_size) {
                logger.fatal("Tag data buffer overflow");
                std::exit(EXIT_FAILURE);
            }
            auto *data = m_tag_data.get() + m_tag_data_cursor;
            m_tag_data_cursor += size;
            return data;
        }

        Tag &insert_tag_entry(Tag const &entry) noexcept {
            auto &new_entry = m_tag_array.emplace_back(entry);
            new_entry.handle.index = m_next_handle.index++;
            new_entry.handle.id = m_next_handle.id++;
            return new_entry;
        }

        void insert_tags_entries_front(Tag *tags, std::size_t count) noexcept {
            m_tag_array.insert(m_tag_array.begin(), tags, tags + count);

            auto last_handle = m_tag_array[count - 1].handle;
            auto it = m_tag_array.begin() + count;
            while(it != m_tag_array.end()) {
                it->handle.index = ++last_handle.index;
                it->handle.id = ++last_handle.id;
                it++;
            }
            m_next_handle.index = ++last_handle.index;
            m_next_handle.id = ++last_handle.id;
        }

        void reserve_tag_entries(std::size_t count) noexcept {
            m_tag_array.reserve(m_tag_array.size() + count);
        }

        void update_tag_data_header() {
            auto &tag_data_header = get_tag_data_header();
            m_tag_array.shrink_to_fit();
            tag_data_header.tag_count = m_tag_array.size();
            tag_data_header.tag_array = m_tag_array.data();
        }
    };

    class MapCache {
    protected:
        std::string m_name;
        fs::path m_path;
        MapHeader m_header;
        std::unique_ptr<std::byte[]> m_raw_tag_data;
        TagDataHeader *m_tag_data_header = nullptr;
        Tag *m_tag_array = nullptr;
        std::map<TagHandle, std::vector<TagHandle>> m_tags_copies;
        std::map<void *, void *> m_address_translations;

    public:
        void read_tag_data_from_file() {
            m_raw_tag_data = std::make_unique<std::byte[]>(m_header.tag_data_size);

            std::FILE *file = std::fopen(m_path.string().c_str(), "rb");
            std::fseek(file, m_header.tag_data_offset, SEEK_SET);
            std::fread(m_raw_tag_data.get(), 1, m_header.tag_data_size, file);
            std::fclose(file);

            m_tag_data_header = reinterpret_cast<TagDataHeader *>(m_raw_tag_data.get());
            m_tag_array = reinterpret_cast<Tag *>(m_raw_tag_data.get() + sizeof(TagDataHeader));
        }

        void read_tag_data_from_buffer(std::byte *data) noexcept {
            m_raw_tag_data = std::make_unique<std::byte[]>(m_header.tag_data_size);
            std::memcpy(m_raw_tag_data.get(), data, m_header.tag_data_size);

            m_tag_data_header = reinterpret_cast<TagDataHeader *>(m_raw_tag_data.get());
            m_tag_array = reinterpret_cast<Tag *>(m_raw_tag_data.get() + sizeof(TagDataHeader));
        }

        void read_header_from_file() {
            std::FILE *file = std::fopen(m_path.string().c_str(), "rb");
            std::fread(&m_header, sizeof(MapHeader), 1, file);
            std::fclose(file);
        }

        void read_header_from_current_map() noexcept {
            m_header = get_map_header();
        }

        MapCache(std::string map_name) : m_name(map_name) {
            try {
                m_path = path_for_map_local(m_name.c_str());
            }
            catch (std::runtime_error &e) {
                throw;
            }
        }

        MapCache(fs::path map_path) : m_path(map_path) {
            m_name = m_path.stem().string();
            if(!fs::exists(m_path)) {
                throw std::runtime_error("Map file does not exist");
            }
        }

        MapCache(MapCache &&other) {
            m_raw_tag_data = std::move(other.m_raw_tag_data);
            m_tags_copies = std::move(other.m_tags_copies);
        }

        std::string name() const noexcept {
            return m_name;
        }

        void name(std::string map_name) {
            m_name = map_name;
        }

        fs::path path() const noexcept {
            return m_path;
        }

        void path(fs::path map_path) {
            m_path = map_path;
        }

        MapHeader const &header() {
            return m_header;
        }

        TagDataHeader const &tag_data_header() const noexcept {
            return *m_tag_data_header;
        }

        std::byte *tag_data() noexcept {
            return m_raw_tag_data.get();
        }

        auto &tag_copies() noexcept {
            return m_tags_copies;
        }

        /**
         * Translate an address from the address where the tag data is supposed to be loaded to the address where the tag data is actually loaded.
         */
        template<typename T>
        T translate_address(T address) {
            if(address != 0) {
                for(auto &[original_address, translated_address] : m_address_translations) {
                    if(reinterpret_cast<void *>(address) == original_address || reinterpret_cast<void *>(address) == translated_address) {
                        return reinterpret_cast<T>(translated_address);
                    }
                }
                auto base_address_disp = reinterpret_cast<std::uint32_t>(get_tag_data_address()) - reinterpret_cast<std::uint32_t>(m_raw_tag_data.get());
                auto new_address = reinterpret_cast<T>(reinterpret_cast<std::uint32_t>(address) - base_address_disp);
                m_address_translations.insert_or_assign(reinterpret_cast<void *>(address), reinterpret_cast<void *>(new_address));
                return new_address;
            }
            return address;
        }

        /**
         * Get tag entry from the raw tag data of the map
         */
        Tag *get_raw_tag(TagHandle tag_handle) {
            for(std::size_t i = 0; i < m_tag_data_header->tag_count; i++) {
                if(m_tag_array[i].handle == tag_handle) {
                    return &m_tag_array[i];
                }
            }
            return nullptr;
        }

        /**
         * Get tag entry of the original tag for a tag copy
         */
        Tag *get_original_tag_for_copy(TagHandle tag_copy_handle) {
            auto *tag_copy = LegacyApi::Engine::get_tag(tag_copy_handle);
            for(auto &[original_tag_handle, copies] : m_tags_copies) {
                for(auto &copy : copies) {
                    if(copy == tag_copy_handle) {
                        return LegacyApi::Engine::get_tag(original_tag_handle);
                    }
                }
            }
            return nullptr;
        }

        /**
         * Get tag entry of a tag copy
         * @param original_tag_handle   Handle of the original tag
         * @param name                  Name of the tag copy
         * @return                      Tag entry of the tag copy
         */
        Tag *get_tag_copy(TagHandle original_tag_handle, std::string const &name) {
            for(auto &[original_tag_handle, copies] : m_tags_copies) {
                if(original_tag_handle == original_tag_handle) {
                    for(auto &copy : copies) {
                        auto *original_tag = LegacyApi::Engine::get_tag(original_tag_handle);
                        auto copy_path = std::string(original_tag->path) + "\\" + name;
                        auto *tag = LegacyApi::Engine::get_tag(copy);
                        if(tag->path == copy_path) {
                            return tag;
                        }
                    }
                    break;
                }
            }
            return nullptr;
        }
    };

    class SecondaryMapCache : public MapCache {
    private:
        std::vector<std::pair<std::string, TagClassInt>> m_tags_to_load;
        std::map<char *, char *> m_tag_path_translations;
        std::map<TagHandle, TagHandle> m_tag_handles_translations;
        bool m_load_all_tags = false;

        char *translate_tag_path(char *path) noexcept {
            if(path == nullptr) {
                return nullptr;
            }

            // Check if path is already translated
            if(m_tag_path_translations.find(path) != m_tag_path_translations.end()) {
                return m_tag_path_translations[path];
            }

            // Copy path to tag data buffer
            auto *buffer = virtual_tag_data->reserve_tag_data_space(std::strlen(path) + 1);
            auto *new_path = std::strcpy(reinterpret_cast<char *>(buffer), path);

            m_tag_path_translations.insert_or_assign(path, new_path);
            return new_path;
        }

        std::uint32_t get_data_base_offset() {
            std::uint32_t offset = map_cache->header().file_size;
            for(auto &map : secondary_maps_cache) {
                if(this == map.get()) {
                    break;
                }
                offset += map->header().file_size;
            }
            return offset;
        }

        std::uint32_t get_model_data_base_offset() {
            std::uint32_t offset = map_cache->tag_data_header().model_data_size;
            for(auto &map : secondary_maps_cache) {
                if(this == map.get()) {
                    break;
                }
                offset += map->tag_data_header().model_data_size;
            }
            return offset;
        }

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

        TagHandle load_tag(Tag *tag, bool required) {
            // Check if current tag class is supported
            if(!tag_class_is_supported(tag->primary_class)) {
                if(required) {
                    show_error_box("Unsupported tag class %.*s", 4, reinterpret_cast<const char *>(&tag->primary_class));
                    std::exit(EXIT_FAILURE);
                }
                return TagHandle::null();
            }

            auto tag_handle_resolver = [this](TagHandle tag_handle) -> TagHandle {
                auto *broken_tag = this->get_raw_tag(tag_handle);
                if(broken_tag) {
                    auto new_tag_handle = this->load_tag(broken_tag, true);
                    return new_tag_handle;
                }
                else {
                    if(tag_handle != TagHandle::null()) {
                        logger.debug("Cannot resolve tag {} in map {}", tag_handle.value, m_name);
                    }
                    return tag_handle;
                }
            };

            auto data_base_offset = get_data_base_offset();
            auto model_data_base_offset = get_model_data_base_offset();

            // Check if we've already loaded this tag
            if(m_tag_handles_translations.find(tag->handle) != m_tag_handles_translations.end()) {
                return m_tag_handles_translations.find(tag->handle)->second;
            }

            // Fix entry path and data pointers
            tag->path = translate_address(tag->path);
            if(!tag->indexed || tag->primary_class == TAG_CLASS_SOUND) {
                tag->data = translate_address(tag->data);
            }

            // Set up new tag entry
            auto &new_tag_entry = virtual_tag_data->insert_tag_entry(*tag);
            new_tag_entry.path = translate_tag_path(new_tag_entry.path);
            m_tag_handles_translations.insert_or_assign(tag->handle, new_tag_entry.handle);

            // if current tag are indexed or if tags are already fixed, we can continue
            if(tag->indexed) {
                if(tag->primary_class == TAG_CLASS_SOUND) {
                    auto *sound_base_struct = reinterpret_cast<Sound *>(new_tag_entry.data);
                    sound_base_struct->promotion_sound.tag_handle = tag_handle_resolver(sound_base_struct->promotion_sound.tag_handle);
                }

                return new_tag_entry.handle;
            }

            // There's no data loaded to fix... yet
            if(tag->primary_class == TAG_CLASS_SCENARIO_STRUCTURE_BSP) {
                return new_tag_entry.handle;
            }

            rebase_tag_data_offsets(&new_tag_entry, m_raw_tag_data.get(), [&](std::uint32_t offset) -> std::uint32_t {
                return new_tag_entry.indexed ? offset : offset + data_base_offset;
            });

            resolve_tag_dependencies(&new_tag_entry, tag_handle_resolver);

            switch(new_tag_entry.primary_class) {
                case TAG_CLASS_BITMAP: {
                    Bitmap *bitmap = reinterpret_cast<Bitmap *>(new_tag_entry.data);
                    if(bitmap->bitmap_data.count > 0) {
                        for(std::size_t j = 0; j < bitmap->bitmap_data.count; j++) {
                            bitmap->bitmap_data.elements[j].pixel_data_offset += data_base_offset; 
                        }
                    }
                    break;
                }

                case TAG_CLASS_GBXMODEL: {
                    auto *gbxmodel = reinterpret_cast<Gbxmodel *>(new_tag_entry.data);
                    for(std::size_t i = 0; i < gbxmodel->geometries.count; i++) {
                        for(std::size_t j = 0; j < gbxmodel->geometries.elements[i].parts.count; j++) {
                            gbxmodel->geometries.elements[i].parts.elements[j].vertex_offset += model_data_base_offset;
                            gbxmodel->geometries.elements[i].parts.elements[j].triangle_offset += model_data_base_offset - map_cache->tag_data_header().vertex_size + m_tag_data_header->vertex_size;
                            gbxmodel->geometries.elements[i].parts.elements[j].triangle_offset_2 += model_data_base_offset - map_cache->tag_data_header().vertex_size + m_tag_data_header->vertex_size;
                        }                               
                    }
                    break;
                }

                default: {
                    break;
                }
            }

            new_tag_entry.data = copy_tag_data(&new_tag_entry, [](std::byte *data, std::size_t size) -> std::byte * {
                auto *new_data = virtual_tag_data->reserve_tag_data_space(size);
                std::memcpy(new_data, data, size);
                return new_data;
            });

            if(!new_tag_entry.data) {
                logger.fatal("Failed to copy data for tag \"{}\" of class {}", new_tag_entry.path, tag_class_to_string(new_tag_entry.primary_class));
                std::exit(EXIT_FAILURE);
            }

            return new_tag_entry.handle;
        }

    public:
        SecondaryMapCache(std::string map_name) : MapCache(map_name) {
            read_header_from_file();

            if(m_header.engine_type == CACHE_FILE_CUSTOM_EDITION_COMPRESSED) {
                throw std::runtime_error("Map file is compressed");
            } 

            if(m_header.engine_type != CACHE_FILE_CUSTOM_EDITION) {
                throw std::runtime_error("Map file is not a Halo Custom Edition map");
            }

            read_tag_data_from_file();
        }

        SecondaryMapCache(fs::path map_path) : MapCache(map_path) {
            read_header_from_file();

            if(m_header.engine_type == CACHE_FILE_CUSTOM_EDITION_COMPRESSED) {
                throw std::runtime_error("Map file is compressed");
            }

            if(m_header.engine_type != CACHE_FILE_CUSTOM_EDITION) {
                throw std::runtime_error("Map file is not a Halo Custom Edition map");
            }

            read_tag_data_from_file();
        }

        void add_tag_import(std::string tag_path, TagClassInt tag_class) {
            for(auto &tag : m_tags_to_load) {
                if(tag.first == tag_path && tag.second == tag_class) {
                    return;
                }
            }
            m_tags_to_load.emplace_back(tag_path, tag_class);
        }

        void import_all_tags() {
            m_load_all_tags = true;
        }

        std::optional<TagHandle> get_origin_tag_handle(TagHandle handle) {
            auto *tag = LegacyApi::Engine::get_tag(handle);
            for(auto &[origin_handle, virtual_handle] : m_tag_handles_translations) {
                if(handle == virtual_handle) {
                    auto *origin_tag = this->get_raw_tag(origin_handle);
                    if(tag->primary_class == origin_tag->primary_class && std::strcmp(tag->path, origin_tag->path) == 0) {
                        return origin_handle;
                    }
                }
            }
            return std::nullopt;
        }

        /**
         * Get the a tag entry from the virtual tag data that belongs to the map
         * @param tag_path  Path to the tag
         * @param tag_class Tag class of the tag
         * @return          Pointer to the tag entry
         */
        Tag *get_tag(std::string const &tag_path, TagClassInt tag_class) {
            for(auto &[origin_handle, tag_handle] : m_tag_handles_translations) {
                auto *tag = LegacyApi::Engine::get_tag(tag_handle);
                if(std::strcmp(tag->path, tag_path.c_str()) == 0 && tag->primary_class == tag_class) {
                    return tag;
                }
            }
            return nullptr;
        }

        std::optional<TagHandle> translate_tag_handle(TagHandle handle) {
            if(m_tag_handles_translations.find(handle) != m_tag_handles_translations.end()) {
                return m_tag_handles_translations[handle];
            }
            return std::nullopt;
        }

        void load_tag_data() {
            // Reserve space for tags to AVOID REALLOCATIONS during the process (!!!)
            virtual_tag_data->reserve_tag_entries(m_tag_data_header->tag_count);

            if(m_load_all_tags) {
                for(std::size_t i = 0; i < m_tag_data_header->tag_count; i++) {
                    load_tag(m_tag_array + i, false);
                }
            }
            else {
                for(auto &tag : m_tags_to_load) {
                    bool tag_found = false;
                    for(std::size_t i = 0; i < m_tag_data_header->tag_count; i++) {
                        std::string path = translate_address(m_tag_array[i].path);
                        if(path == tag.first && (m_tag_array[i].primary_class == tag.second || tag.second == TAG_CLASS_NULL)) {
                            if(load_tag(m_tag_array + i, false) != TagHandle::null()) {
                                tag_found = true;
                            }
                            break;
                        }
                    }
                    if(!tag_found) {
                        logger.warning("Tag {} of class {} not found in map {}", tag.first, tag_class_to_string(tag.second), m_name);
                    }
                }
            }
        }
    };

    static bool tag_is_copy(TagHandle handle) {
        if(map_cache->get_raw_tag(handle)) {
            return false;
        }
        for(auto &map : secondary_maps_cache) {
            if(map->get_origin_tag_handle(handle)) {
                return false;
            }
        }
        return true;
    }

    static void import_tag_data() {
        auto &tag_data_header = get_tag_data_header();
        auto *tag_data_address = get_tag_data_address();

        // Reading main map data
        logger.debug("Reading tag data from loaded map...");
        map_cache = std::make_unique<MapCache>(map_file_path);
        map_cache->read_header_from_file();
        map_cache->read_tag_data_from_buffer(tag_data_address);

        // Initialize our stuff
        logger.info("Initializing virtual tag data...");
        secondary_maps_cache = preloaded_secondary_maps_cache;
        virtual_tag_data = std::make_unique<VirtualTagData>();
        virtual_tag_data->insert_tags_entries_front(tag_data_header.tag_array, tag_data_header.tag_count);
        
        logger.info("Importing tag data from other maps...");
        for(auto &map : secondary_maps_cache) {
            map->load_tag_data();
        }
        
        logger.debug("Updating tag data header...");
        virtual_tag_data->update_tag_data_header();
    }

    void on_map_file_load(LegacyApi::Event::MapFileLoadEvent const &event) {
        // Get map file path
        map_file_path = event.context.map_path; 
    }

    void prepare_to_load_map(LegacyApi::Event::MapLoadEvent const &event) {
        if(event.time == LegacyApi::Event::EVENT_TIME_BEFORE) {
            logger.info("Preparing to load map {}", map_file_path.string());
            preloaded_secondary_maps_cache.clear();
        }
    }

    static fs::path get_path_from_file_handle(HANDLE file_handle) {
        // Get the name of the file we're reading from
        char file_path_chars[MAX_PATH + 1] = {};
        GetFinalPathNameByHandle(file_handle, file_path_chars, sizeof(file_path_chars) - 1, VOLUME_NAME_NONE);
        auto file_path = fs::path(file_path_chars);
        return file_path;
    }

    void on_read_map_file_data(LegacyApi::Event::MapFileDataReadEvent &event) {      
        if(secondary_maps_cache.empty() && preloaded_secondary_maps_cache.empty()) {
            return;
        }

        const HANDLE &file_descriptor = event.context.file_handle; 
        std::byte *output = event.context.output_buffer;
        const std::size_t &size = event.context.size;
        std::size_t file_offset = event.context.overlapped->Offset;

        auto file_path = get_path_from_file_handle(file_descriptor);
        auto file_name = file_path.filename();

        if(event.time == LegacyApi::Event::EVENT_TIME_BEFORE) {
            if(!map_cache || file_name.stem().string() != map_cache->name()) {
                return;
            }

            // Look for secondary maps offset displacement
            auto offset_acc = map_cache->header().file_size;
            if(file_offset > offset_acc) {
                for(auto &map : secondary_maps_cache) {
                    auto map_file_size = map->header().file_size;
                    if(file_offset <= offset_acc + map_file_size) {
                        std::FILE *file = std::fopen(map->path().string().c_str(), "rb");
                        std::fseek(file, file_offset - offset_acc, SEEK_SET);
                        std::fread(output, 1, size, file);
                        std::fclose(file);
                        event.context.size = 0;
                        return;
                    }
                    offset_acc += map_file_size;
                }
                logger.warning("File offset {} is out of bounds! Map file size: {}", file_offset, offset_acc);
            }

            // Read model data ONLY for secondary maps
            const auto &tag_data_header = get_tag_data_header();
            if(tag_data_header.model_data_file_offset == file_offset && tag_data_header.model_data_size == size) {
                logger.info("Loading model data from secondary maps...");
                std::size_t buffer_cursor = tag_data_header.model_data_size;
                for(auto &map : secondary_maps_cache) {
                    auto &map_tag_data_header = map->tag_data_header();
                    std::FILE *file = std::fopen(map->path().string().c_str(), "rb");
                    std::fseek(file, map_tag_data_header.model_data_file_offset, SEEK_SET);
                    std::fread(output + buffer_cursor, 1, map_tag_data_header.model_data_size, file);
                    std::fclose(file);
                    
                    buffer_cursor += map_tag_data_header.model_data_size;
                }
            }
        }
    }

    void import_tag_from_map(std::string map_name, std::string tag_path, TagClassInt tag_class) {
        try {
            if(map_name == map_file_path.stem()) {
                return;
            }
            for(auto &map : preloaded_secondary_maps_cache) {
                if(map->name() == map_name) {
                    map->add_tag_import(tag_path, tag_class);
                    return;
                }
            }
            logger.debug("Creating map cache for {}", map_name);
            auto &map = preloaded_secondary_maps_cache.emplace_back(std::make_shared<SecondaryMapCache>(map_name));
            map->add_tag_import(tag_path, tag_class);
        }
        catch(std::exception &e) {
            logger.error("Failed to import tag {} from map {}: {}", tag_path, map_name, e.what());
        }
    }

    void import_tag_from_map(fs::path map_file, std::string tag_path, TagClassInt tag_class) {
        try {
            if(map_file == map_file_path.stem()) {
                return;
            }
            for(auto &map : preloaded_secondary_maps_cache) {
                if(map->path() == map_file) {
                    map->add_tag_import(tag_path, tag_class);
                    return;
                }
            }
            logger.debug("Creating map cache for {}", map_file.stem().string());
            auto &map = preloaded_secondary_maps_cache.emplace_back(std::make_shared<SecondaryMapCache>(map_file));
            map->add_tag_import(tag_path, tag_class);
        }
        catch(std::exception &e) {
            logger.error("Failed to import tag {} from map {}: {}", tag_path, map_file.stem().string(), e.what());
        }
    }

    void import_tags_from_map(fs::path map_file) {
        try {
            if(map_file == map_file_path.stem()) {
                return;
            }
            for(auto &map : preloaded_secondary_maps_cache) {
                if(map->path() == map_file) {
                    return;
                }
            }
            logger.debug("Creating map cache for {}", map_file.stem().string());
            auto &map = preloaded_secondary_maps_cache.emplace_back(std::make_shared<SecondaryMapCache>(map_file));
            map->import_all_tags();
        }
        catch(std::runtime_error &e) {
            logger.debug("Failed to import tags from map {}: {}", map_file.string(), e.what());
            throw;
        }
    }

    void reload_tag_data(TagHandle tag_handle) {
        auto *target_tag = get_tag(tag_handle);
        if(!target_tag) {
            throw std::runtime_error("Tag not found");
        }

        Tag *original_tag = nullptr;
        if(tag_is_copy(tag_handle)) {
            original_tag = map_cache->get_original_tag_for_copy(tag_handle);
            if(!original_tag) {
                for(auto &map : secondary_maps_cache) {
                    original_tag = map->get_original_tag_for_copy(tag_handle);
                    if(original_tag) {
                        break;
                    }
                }
                if(!original_tag) {
                    throw std::runtime_error("Unable to find the origin of the tag");
                }
            }
        }
        else {
            original_tag = target_tag;
        }

        // If tag is from the loaded map, create a copy of it in the virtual tag data
        if(original_tag->handle.index < map_cache->tag_data_header().tag_count) {
            auto *tag_data_address = get_tag_data_address();
            auto *raw_tag = map_cache->get_raw_tag(original_tag->handle);
            if(raw_tag->data >= tag_data_address && raw_tag->data < tag_data_address + map_cache->header().tag_data_size) {
                raw_tag->data = map_cache->translate_address(raw_tag->data);
                rebase_tag_data_offsets(raw_tag, map_cache->tag_data());
            }

            auto *tag_data = target_tag->data;
            target_tag->data = copy_tag_data(raw_tag, [&tag_data](std::byte *data, std::size_t size) -> std::byte * {
                auto *new_data = tag_data;
                std::memcpy(new_data, data, size);
                tag_data += size;
                return new_data;
            });
        }
        else {
            for(auto &map : secondary_maps_cache) {
                auto origin_handle = map->get_origin_tag_handle(original_tag->handle);
                if(origin_handle) {
                    auto *raw_tag = map->get_raw_tag(*origin_handle);
                    auto *tag_data = target_tag->data;
                    target_tag->data = copy_tag_data(raw_tag, [&tag_data](std::byte *data, std::size_t size) -> std::byte * {
                        auto *new_data = tag_data;
                        std::memcpy(new_data, data, size);
                        tag_data += size;
                        return new_data;
                    });
                    return;
                }
            }
            throw std::runtime_error("Tag not found. This should not happen, it may be a bug.");
        }
    }

    TagHandle clone_tag(TagHandle tag_handle, std::string copy_name) {
        auto *tag = get_tag(tag_handle);
        if(!tag) {
            throw std::runtime_error("Tag not found");
        }

        if(tag_is_copy(tag_handle)) {
            auto *original_tag = map_cache->get_original_tag_for_copy(tag_handle);
            if(!original_tag) {
                for(auto &map : secondary_maps_cache) {
                    original_tag = map->get_original_tag_for_copy(tag_handle);
                    if(original_tag) {
                        break;
                    }
                }
            }

            if(original_tag) {
                tag = original_tag;
            }
            else {
                throw std::runtime_error("Unable to find the origin of the tag");
            }
        }

        Tag *raw_tag = nullptr;
        MapCache *map = nullptr;

        if(tag->handle.index < map_cache->tag_data_header().tag_count) {
            auto *tag_data_address = get_tag_data_address();
            raw_tag = map_cache->get_raw_tag(tag->handle);
            if(raw_tag->data >= tag_data_address && raw_tag->data < tag_data_address + map_cache->header().tag_data_size) {
                raw_tag->data = map_cache->translate_address(raw_tag->data);
                rebase_tag_data_offsets(raw_tag, map_cache->tag_data());
            }
            map = map_cache.get();
        }
        else {
            for(auto &secondary_map : secondary_maps_cache) {
                auto origin_handle = secondary_map->get_origin_tag_handle(tag->handle);
                if(origin_handle) {
                    raw_tag = secondary_map->get_raw_tag(*origin_handle);
                    map = secondary_map.get();
                    break;
                }
            }
            if(!raw_tag) {
                logger.warning("Cannot find origin of imported tag {}", tag->path);
                throw std::runtime_error("Unable to find the origin of the imported tag");
            }
        }

        auto &new_entry = virtual_tag_data->insert_tag_entry(*raw_tag);
        new_entry.data = copy_tag_data(raw_tag, [](std::byte *data, std::size_t size) -> std::byte * {
            auto *new_data = virtual_tag_data->reserve_tag_data_space(size);
            std::memcpy(new_data, data, size);
            return new_data;
        });

        // Set virtual path for the new entry
        char *new_path = reinterpret_cast<char *>(virtual_tag_data->reserve_tag_data_space(std::strlen(new_entry.path) + copy_name.size() + 2)); // +2 for the null terminator and the backslash
        std::strcpy(new_path, new_entry.path);
        std::strcat(new_path, ("\\" + copy_name).c_str());
        new_entry.path = new_path;

        auto &map_copies = map->tag_copies();
        if(map_copies.find(tag->handle) == map_copies.end()) {
            map_copies.emplace(tag->handle, std::vector<TagHandle>());
        }
        map_copies[tag->handle].emplace_back(new_entry.handle);

        virtual_tag_data->update_tag_data_header();

        return new_entry.handle;
    }

    Tag *get_tag_copy(TagHandle handle, std::string const &name) noexcept {
        Tag *tag = nullptr;
        tag = map_cache->get_tag_copy(handle, name);
        if(!tag) {
            for(auto &map : secondary_maps_cache) {
                tag = map->get_tag_copy(handle, name);
                if(tag) {
                    break;
                }
            }
        }
        return tag;
    }

    Tag *get_imported_tag(std::string const &map_name, std::string const &tag_path, TagClassInt tag_class) noexcept {
        Tag *tag = nullptr;
        for(auto &map : secondary_maps_cache) {
            if(map->name() == map_name) {
                tag = map->get_tag(tag_path, tag_class);
                break;
            }
        }
        return tag;
    }

    void replace_tag_references(TagHandle tag_handle, TagHandle new_tag_handle) {
        auto &tag_data_header = get_tag_data_header();
        for(std::size_t i = 0; i < tag_data_header.tag_count; i++) {
            auto &current_tag = tag_data_header.tag_array[i];
            if(current_tag.data != nullptr) {
                resolve_tag_dependencies(&current_tag, [tag_handle, new_tag_handle, &current_tag](TagHandle dependency_handle) -> TagHandle {
                    if(dependency_handle == tag_handle && current_tag.handle != tag_handle) {
                        return new_tag_handle;
                    }
                    return dependency_handle;
                });
            }
        }
    }

    extern "C" {
        void on_model_data_buffer_alloc_asm();
    
        void on_model_data_buffer_alloc(std::size_t *size) {
            logger.info("Allocating model data buffer for secondary maps...");
            for(auto &map : preloaded_secondary_maps_cache) {
                *size += map->tag_data_header().model_data_size;
            }
        }
    }

    void set_up_tag_data_importing() {
        LegacyApi::Event::MapLoadEvent::subscribe_const(prepare_to_load_map, LegacyApi::Event::EVENT_PRIORITY_HIGHEST);
        LegacyApi::Event::MapFileLoadEvent::subscribe(on_map_file_load);
        LegacyApi::Event::MapFileDataReadEvent::subscribe(on_read_map_file_data);

        auto *model_data_buffer_alloc_sig = Memory::get_signature("model_data_buffer_alloc");
        auto *model_data_buffer_alloc_hook = Memory::hook_function(model_data_buffer_alloc_sig->data(), on_model_data_buffer_alloc_asm);

        auto *tag_data_read_done_sig = Memory::get_signature("tag_data_read_done");
        Memory::hook_function(tag_data_read_done_sig->data(), import_tag_data);

        register_command("imported_tag_data_info", "debug", "Prints the details of the imported tag data from secondary maps.", std::nullopt, [](int arg_count, const char **args) -> bool {
            if(secondary_maps_cache.empty()) {
                LegacyApi::Engine::console_print("No tag data imported from secondary maps");
                return true;
            }
            std::string maps_loaded;
            std::size_t cached_data = 0;
            for(std::size_t i = 0; i < secondary_maps_cache.size(); i++) {
                auto &map = secondary_maps_cache[i];
                maps_loaded += map->name();
                cached_data += map->header().tag_data_size;
                if(i < secondary_maps_cache.size() - 1) {
                    maps_loaded += ", ";
                }
            }
            LegacyApi::Engine::console_print("Imported tag data summary");
            LegacyApi::Engine::console_printf("Maps loaded: %zu (%s)", secondary_maps_cache.size(), maps_loaded.c_str());
            LegacyApi::Engine::console_printf("Tags imported: %zu", virtual_tag_data->tag_count() - map_cache->tag_data_header().tag_count);
            LegacyApi::Engine::console_printf("Imported tag data size: %.2fMiB / %.2fMiB", static_cast<float>(virtual_tag_data->tag_data_size()) / MIB_SIZE, static_cast<float>(virtual_tag_data_buffer_size) / MIB_SIZE);
            LegacyApi::Engine::console_printf("Cached tag data size: %.2f MiB", static_cast<float>(cached_data) / MIB_SIZE);
            return true;
        }, false, 0, 0);
    }
}
