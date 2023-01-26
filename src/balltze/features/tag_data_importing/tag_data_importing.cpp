// SPDX-License-Identifier: GPL-3.0-only

#include <windows.h>
#include <filesystem>
#include <vector>
#include <map>
#include <deque>
#include <memory>
#include <functional>
#include <numeric>

#include <balltze/engine.hpp>
#include <balltze/output.hpp>
#include <balltze/event.hpp>
#include <balltze/feature.hpp>
#include <balltze/memory.hpp>
#include <balltze/hook.hpp>
#include "map.hpp"

namespace Balltze::Features {
    using namespace Engine;
    using namespace Engine::TagDefinitions;

    struct LoadedMap {
        std::string name;
        std::filesystem::path path;
        MapHeader header;
        TagDataHeader tag_data_header;
        bool secondary = false;
        std::vector<std::pair<std::string, Engine::TagClassInt>> tags_to_load;

        std::size_t tag_data_size() {
            return this->header.tag_data_size;
        }

        LoadedMap(const std::string name, bool secondary) noexcept {
            this->name = name;
            this->path = path_for_map_local(name.c_str());
            this->secondary = secondary;

            // Read map data
            std::FILE *file = std::fopen(this->path.string().c_str(), "rb");
            std::fread(&header, sizeof(MapHeader), 1, file);
            std::fseek(file, header.tag_data_offset, SEEK_SET);
            std::fread(&tag_data_header, sizeof(TagDataHeader), 1, file);
            std::fclose(file);
        }
    };

    static std::deque<LoadedMap> loaded_maps;
    static std::vector<Tag> tag_array;
    static std::unique_ptr<std::byte[]> tag_data;
    static std::size_t tag_data_buffer_size;
    static std::size_t tag_data_cursor = 0;

    extern "C" {
        void on_read_map_file_data_asm();
        void on_model_data_buffer_alloc_asm();
    }

    static std::byte *read_tag_data(LoadedMap &map) noexcept {
        std::FILE *file = std::fopen(map.path.string().c_str(), "rb");
        MapHeader header;
        TagDataHeader tag_data_header;

        // Read map header
        std::fread(&header, sizeof(MapHeader), 1, file);
        
        // Read tag data header
        std::fseek(file, header.tag_data_offset, SEEK_SET);
        std::fread(&tag_data_header, sizeof(TagDataHeader), 1, file);

        // Read tag data
        auto *map_tag_data = tag_data.get() + tag_data_cursor;
        std::fseek(file, header.tag_data_offset, SEEK_SET);
        tag_data_cursor += std::fread(map_tag_data, 1, header.tag_data_size, file);

        std::fclose(file);

        return map_tag_data;
    }

    static void load_tag_data() noexcept {
        auto &tag_data_header = get_tag_data_header();
        auto data_base_offset = 0;
        auto model_data_base_offset = 0;
        
        // Allocate buffer for maps tag data
        auto buffer_size = std::transform_reduce(loaded_maps.begin(), loaded_maps.end(), 0, std::plus<>(), std::mem_fn(LoadedMap::tag_data_size));
        if(buffer_size > tag_data_buffer_size) {
            tag_data = std::make_unique<std::byte[]>(buffer_size);
            tag_data_buffer_size = buffer_size;
        }
        tag_data_cursor = 0;

        for(auto &map : loaded_maps) {
            const auto &map_tag_data_header = map.tag_data_header;
            const auto *map_tag_data = read_tag_data(map);

            if(!map.secondary) {
                std::size_t cursor = 0;
                
                tag_data_header = map_tag_data_header;
                cursor += sizeof(TagDataHeader);

                // tag array
                auto *tag_array_raw = reinterpret_cast<const Tag *>(map_tag_data + cursor);
                tag_array = std::vector<Tag>(tag_array_raw, tag_array_raw + tag_data_header.tag_count);
                tag_data_header.tag_array = tag_array.data();
                cursor += sizeof(Tag) * tag_data_header.tag_count;

                // rest of tag data
                std::memcpy(get_tag_data_address() + cursor, map_tag_data + cursor, map.header.tag_data_size - cursor);
            }
            else {
                std::map<TagHandle, TagHandle> tags_directory;
                auto tag_array_raw = reinterpret_cast<const Tag *>(map_tag_data + sizeof(TagDataHeader));
                auto tag_data_base_address_disp = reinterpret_cast<std::uint32_t>(get_tag_data_address()) - reinterpret_cast<std::uint32_t>(map_tag_data);

                auto is_supported_tag = [](TagClassInt tag_class) {
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
                };

                auto get_tag_from_secondary_map = [&map_tag_data_header, &tag_array_raw](TagHandle tag_handle) -> const Tag * {
                    for(std::size_t i = 0; i < map_tag_data_header.tag_count; i++) {
                        if(tag_array_raw[i].id == tag_handle) {
                            return &tag_array_raw[i];
                        }
                    }
                    return nullptr;
                };

                auto translate_address = [&tag_data_base_address_disp](auto address) {
                    if(address != 0) {
                        address = reinterpret_cast<decltype(address)>(reinterpret_cast<std::uint32_t>(address) - tag_data_base_address_disp);
                    }
                    return address;
                };

                std::function<TagHandle (const Tag *, bool)> load_tag = [&](const Tag *tag, bool required) -> TagHandle {
                    // Check if current tag class is supported
                    if(!is_supported_tag(tag->primary_class)) {
                        if(required) {
                            show_error_box("Unsupported tag class %.*s", 4, reinterpret_cast<const char *>(&tag->primary_class));
                            std::exit(EXIT_FAILURE);
                        }
                        return TagHandle::null();
                    }

                    // Check if we've already loaded this tag
                    if(tags_directory.find(tag->id) != tags_directory.end()) {
                        return tags_directory.find(tag->id)->second;
                    }

                    // Set up new tag entry
                    auto &new_tag_entry = tag_array.emplace_back(*tag);
                    new_tag_entry.path = translate_address(new_tag_entry.path);
                    new_tag_entry.id.index.index = tag_data_header.tag_count;
                    new_tag_entry.id.index.id = tag_data_header.tags_literal + tag_data_header.tag_count;
                    tags_directory.insert_or_assign(tag->id, new_tag_entry.id);
                    tag_data_header.tag_count++;

                    // if current tag are indexed or if tags are already fixed, we can continue
                    if(tag->indexed) {
                        if(new_tag_entry.primary_class == TAG_CLASS_SOUND) {
                            new_tag_entry.data = translate_address(new_tag_entry.data);
                        }
                        return new_tag_entry.id;
                    }

                    // There's no data loaded to fix... yet
                    if(tag->primary_class == TAG_CLASS_SCENARIO_STRUCTURE_BSP) {
                        return new_tag_entry.id;
                    }

                    new_tag_entry.fix_data_offsets(translate_address(new_tag_entry.data), [&](std::uint32_t offset) -> std::uint32_t {
                        return new_tag_entry.indexed ? offset : offset + data_base_offset;
                    });

                    new_tag_entry.fix_dependencies([&](TagDependency tag_dependency) -> TagDependency {
                        if(tag_dependency.tag_id != -1) {
                            tag_dependency.path_pointer = translate_address(tag_dependency.path_pointer); 
                            auto *dependency = get_tag_from_secondary_map(tag_dependency.tag_id);
                            auto tag_handle = load_tag(dependency, true); 
                            tag_dependency.tag_id = tag_handle; 
                        }
                        return tag_dependency;
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
                                    gbxmodel->geometries.offset[i].parts.offset[j].triangle_offset += model_data_base_offset - tag_data_header.vertex_size + map_tag_data_header.vertex_size;
                                    gbxmodel->geometries.offset[i].parts.offset[j].triangle_offset_2 += model_data_base_offset - tag_data_header.vertex_size + map_tag_data_header.vertex_size;
                                }                               
                            }
                            break;
                        }

                        default: {
                            break;
                        }
                    }

                    return new_tag_entry.id;
                };

                // Reserve space for tags to AVOID REALLOCATIONS
                tag_array.reserve(tag_array.size() + map_tag_data_header.tag_count);

                // Load tags
                for(auto &tag : map.tags_to_load) {
                    for(std::size_t i = 0; i < map_tag_data_header.tag_count; i++) {
                        std::string path = translate_address(tag_array_raw[i].path);
                        if(path == tag.first && tag_array_raw[i].primary_class == tag.second) {
                            load_tag(tag_array_raw + i, true);
                        }
                    }
                }
            }

            data_base_offset += std::filesystem::file_size(map.path);
            model_data_base_offset += map_tag_data_header.model_data_size;
        }

        tag_data_header.tag_array = tag_array.data();
    }

    void import_tag_from_map(std::string map_name, std::string tag_path, Engine::TagClassInt tag_class) {
        for(auto &map : loaded_maps) {
            if(map.name == map_name) {
                for(auto &tag : map.tags_to_load) {
                    if(tag.first == tag_path && tag.second == tag_class) {
                        return;
                    }
                }
                map.tags_to_load.emplace_back(tag_path, tag_class);
                return;
            }
        }
        auto &map = loaded_maps.emplace_back(map_name, true);
        map.tags_to_load.emplace_back(tag_path, tag_class);
    }

    void on_map_file_load(Event::MapFileLoadEvent const &event) {
        loaded_maps.clear();
        loaded_maps.emplace_back(event.args.map_name, false);
    }

    extern "C" int on_read_map_file_data(HANDLE file_descriptor, std::byte *output, std::size_t size, LPOVERLAPPED overlapped) {        
        std::size_t file_offset = overlapped->Offset;
        
        // Get the name of the file we're reading from
        char file_path_chars[MAX_PATH + 1] = {};
        GetFinalPathNameByHandle(file_descriptor, file_path_chars, sizeof(file_path_chars) - 1, VOLUME_NAME_NONE);
        auto file_path = std::filesystem::path(file_path_chars);
        
        // Check if we're reading from the current loaded map
        auto &current_map = loaded_maps.front();
        auto file_name = file_path.filename();
        if(file_name.stem().string() != current_map.name) {
            return 0;
        }
        
        // Load tag data
        if(output == get_tag_data_address()) {
            load_tag_data();
            return 1;
        }

        // Look for secondary maps offset displacement
        auto offset_acc = std::filesystem::file_size(current_map.path);
        if(file_offset > offset_acc) {
            for(auto &map : loaded_maps) {
                if(!map.secondary) {
                    continue;
                }
                auto map_file_size = std::filesystem::file_size(map.path);
                if(file_offset <= offset_acc + map_file_size) {
                    std::FILE *file = std::fopen(map.path.string().c_str(), "rb");
                    std::fseek(file, file_offset - offset_acc, SEEK_SET);
                    std::fread(output, 1, size, file);
                    std::fclose(file);
                    return 1;
                }
                offset_acc += map_file_size;
            }

            show_error_box("File offset %zu is out of bounds!", file_offset);
        }

        // Read model data for all maps
        const auto &tag_data_header = current_map.tag_data_header;
        if(tag_data_header.model_data_file_offset == file_offset && tag_data_header.model_data_size == size) {
            std::size_t buffer_cursor = 0;
            for(auto &map : loaded_maps) {
                const auto map_tag_data_header = map.tag_data_header;
                std::FILE *file = std::fopen(map.path.string().c_str(), "rb");
                std::fseek(file, map_tag_data_header.model_data_file_offset, SEEK_SET);
                std::fread(output + buffer_cursor, 1, map_tag_data_header.model_data_size, file);
                std::fclose(file);
                buffer_cursor += map_tag_data_header.model_data_size;
            }
            return 1;
        }

        return 0;
    }

    extern "C" void on_model_data_buffer_alloc(std::size_t *size) {
        for(auto &map : loaded_maps) {
            if(!map.secondary) {
                continue;
            }
            *size += map.tag_data_header.model_data_size;
        }
    }

    void set_up_tag_data_importing() {
        Event::NonCancellableConstEventDelegate<Event::MapFileLoadEvent> on_map_file_load_delegate = on_map_file_load;
        Event::MapFileLoadEvent::subscribe_const(on_map_file_load_delegate);

        auto *read_cache_file_data_sig = Memory::get_signature("read_map_file_data");
        auto *read_cache_file_data_hook = Memory::hook_function(read_cache_file_data_sig->data(), on_read_map_file_data_asm);

        auto *model_data_buffer_alloc_sig = Memory::get_signature("model_data_buffer_alloc");
        auto *model_data_buffer_alloc_hook = Memory::hook_function(model_data_buffer_alloc_sig->data(), on_model_data_buffer_alloc_asm);
    }
}
