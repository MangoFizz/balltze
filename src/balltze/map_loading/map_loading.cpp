// SPDX-License-Identifier: GPL-3.0-only

#include <windows.h>
#include <filesystem>
#include <vector>
#include <map>
#include <deque>
#include <memory>
#include <functional>
#include <numeric>

#include <balltze/engine/map.hpp>
#include <balltze/engine/path.hpp>
#include <balltze/engine/tag.hpp>
#include <balltze/engine/tag_definitions/definitions.hpp>
#include <balltze/engine/tag_class.hpp>
#include <balltze/map_loading/laa.hpp>
#include <balltze/memory/hook.hpp>
#include <balltze/memory/memory.hpp>
#include <balltze/output/message_box.hpp>
#include <balltze/balltze.hpp>
#include <balltze/map_loading/map.hpp>
#include <balltze/map_loading/map_loading.hpp>

namespace Balltze {
    using namespace Engine;
    using namespace Engine::TagDefinitions;

    struct LoadedMap {
        std::string name;
        std::filesystem::path path;
        MapHeader header;
        bool secondary = false;

        std::size_t tag_data_size() {
            return this->header.tag_data_size;
        }

        TagDataHeader tag_data_header() noexcept {
            std::FILE *file = std::fopen(this->path.string().c_str(), "rb");
            MapHeader header = this->header;
            TagDataHeader tag_data_header;

            // Read tag data header
            std::fseek(file, header.tag_data_offset, SEEK_SET);
            std::fread(&tag_data_header, sizeof(TagDataHeader), 1, file);

            std::fclose(file);

            return tag_data_header;
        }

        LoadedMap(const std::string name, bool secondary) noexcept {
            this->name = name;
            this->path = path_for_map_local(name.c_str());
            this->secondary = secondary;

            // Read map header
            std::FILE *file = std::fopen(this->path.string().c_str(), "rb");
            MapHeader header;
            std::fread(&header, sizeof(MapHeader), 1, file);
            std::fclose(file);
            this->header = header;
        }
    };

    static std::deque<LoadedMap> loaded_maps;
    static std::vector<Tag> tag_array;
    static std::unique_ptr<std::byte[]> tag_data;
    static std::size_t tag_data_cursor = 0;

    extern "C" {
        void map_loading_asm() noexcept;
        void on_read_map_file_data_asm() noexcept;
        void on_model_data_buffer_alloc_asm() noexcept;
    }

    static void reallocate_tag_data_buffer() noexcept {
        std::size_t buffer_size = 0;
        for(auto &map : loaded_maps) {
            buffer_size += map.header.tag_data_size;
        }
        tag_data = std::make_unique<std::byte[]>(buffer_size);
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

    void append_map(const char *map_name) {
        for(auto &map : loaded_maps) {
            if(map.name == map_name) {
                return;
            }
        }
        loaded_maps.emplace_back(map_name, true);
    }

    extern "C" void do_map_loading_handling(char *map_path, const char *map_name) {
        loaded_maps.clear();
        tag_data_cursor = 0;
        loaded_maps.emplace_front(map_name, false);
        append_map("ui");
    }

    extern "C" int on_read_map_file_data(HANDLE file_descriptor, std::byte *output, std::size_t size, LPOVERLAPPED overlapped) {        
        std::size_t file_offset = overlapped->Offset;
        
        // Get the name
        char file_path_chars[MAX_PATH + 1] = {};
        GetFinalPathNameByHandle(file_descriptor, file_path_chars, sizeof(file_path_chars) - 1, VOLUME_NAME_NONE);
        auto file_path = std::filesystem::path(file_path_chars);
        
        // If it's not a .map file, forget about it
        char file_path_extension[5] = {};
        std::snprintf(file_path_extension, sizeof(file_path_extension), "%s", file_path.extension().string().c_str());
        for(auto &fpe : file_path_extension) {
            fpe = std::tolower(fpe);
        }
        
        // Get the resource file if possible
        auto file_name = file_path.filename();
        if(file_name.stem().string() != loaded_maps[0].name) {
            return 0;
        }
        
        if(output == get_tag_data_address()) {
            auto &tag_data_header = get_tag_data_header();
            auto data_base_offset = 0;
            auto model_data_base_offset = 0;
            
            // Allocate buffer for maps tag data
            auto tag_data_buffer_size = std::transform_reduce(loaded_maps.begin(), loaded_maps.end(), 0, std::plus<>(), std::mem_fn(LoadedMap::tag_data_size));
            tag_data = std::make_unique<std::byte[]>(tag_data_buffer_size);

            for(auto &map : loaded_maps) {
                auto map_tag_data_header = map.tag_data_header();
                auto *map_tag_data = read_tag_data(map);

                if(!map.secondary) {
                    std::size_t cursor = 0;
                    
                    tag_data_header = map_tag_data_header;
                    cursor += sizeof(TagDataHeader);

                    // tag array
                    auto *tag_array_raw = reinterpret_cast<Tag *>(map_tag_data + cursor);
                    tag_array = std::vector<Tag>(tag_array_raw, tag_array_raw + tag_data_header.tag_count);
                    tag_data_header.tag_array = tag_array.data();
                    cursor += sizeof(Tag) * tag_data_header.tag_count;

                    // rest of tag data
                    std::memcpy(output + cursor, map_tag_data + cursor, size - cursor);
                }
                else {
                    auto tag_array_raw = reinterpret_cast<Tag *>(map_tag_data + sizeof(TagDataHeader));
                    auto tag_data_base_address_disp = reinterpret_cast<std::uint32_t>(get_tag_data_address()) - reinterpret_cast<std::uint32_t>(map_tag_data);
                    std::map<TagHandle, TagHandle> tag_id_map; 

                    auto is_supported_tag = [](TagClassInt tag_class) {
                        static TagClassInt supportedTags[] = {
                            TAG_CLASS_BITMAP,
                            TAG_CLASS_UNICODE_STRING_LIST,
                            TAG_CLASS_SOUND,
                            TAG_CLASS_FONT,
                            TAG_CLASS_UI_WIDGET_DEFINITION,
                            TAG_CLASS_GBXMODEL,
                            TAG_CLASS_SHADER_MODEL,
                            TAG_CLASS_SHADER_TRANSPARENT_CHICAGO,
                            TAG_CLASS_SHADER_TRANSPARENT_CHICAGO_EXTENDED,
                            TAG_CLASS_SHADER_TRANSPARENT_GENERIC,
                            TAG_CLASS_MODEL_ANIMATIONS,
                            TAG_CLASS_SCENERY,
                            TAG_CLASS_MODEL_COLLISION_GEOMETRY,
                            TAG_CLASS_PHYSICS,
                            TAG_CLASS_EFFECT,
                            TAG_CLASS_DAMAGE_EFFECT,
                            TAG_CLASS_LIGHT,
                            TAG_CLASS_LENS_FLARE,
                            TAG_CLASS_PARTICLE_SYSTEM,
                            TAG_CLASS_POINT_PHYSICS,
                            TAG_CLASS_DECAL,
                            TAG_CLASS_PARTICLE,
                            TAG_CLASS_MATERIAL_EFFECTS
                        };

                        for(auto &i : supportedTags) {
                            if(i == tag_class) {
                                return true;
                            }
                        }
                        return false;
                    };

                    auto get_tag_from_secondary_map = [&map_tag_data_header, &tag_array_raw](TagHandle tag_handle) -> Tag * {
                        for(std::size_t i = 0; i < map_tag_data_header.tag_count; i++) {
                            if(tag_array_raw[i].id == tag_handle) {
                                return &tag_array_raw[i];
                            }
                        }
                        return nullptr;
                    };

                    std::function<TagHandle *(Tag *, bool)> load_tag = [&](Tag *tag, bool required) -> TagHandle * {

                        #define TRANSLATE_ADDRESS(x) ({ \
                            decltype(x) pointer = static_cast<decltype(x)>(0); \
                            if(reinterpret_cast<void *>(x) != nullptr) { \
                                pointer = reinterpret_cast<decltype(x)>(reinterpret_cast<std::byte *>(x) - tag_data_base_address_disp); \
                            } \
                            pointer; \
                        })

                        #define TRANSLATE_DATA_OFFSET(tag_data_offset) { \
                            if(tag_data_offset.file_offset != 0) { \
                                tag_data_offset.file_offset += data_base_offset; \
                            } \
                        }

                        // Check if current tag class is supported
                        if(!is_supported_tag(tag->primary_class)) {
                            if(required) {
                                char error[2048];
                                char *tag_path = tag->path;
                                tag_path = TRANSLATE_ADDRESS(tag_path);
                                std::snprintf(error, sizeof(error), "Loading tag %s from map %s is not supported", tag_path, map.name.c_str());
                                MessageBoxA(nullptr, error, "Error", MB_OK);
                                std::exit(EXIT_FAILURE);
                            }
                            return nullptr;
                        }

                        // Check if tag already exists in main map... or maybe not
                        if(tag->indexed) {
                            char *tag_path = tag->path;
                            tag_path = TRANSLATE_ADDRESS(tag_path);
                            auto *indexed_tag = get_tag(tag_path, tag->primary_class);
                            if(indexed_tag) {
                                return &indexed_tag->id;
                            }
                        }

                        // Check if we've already loaded this tag
                        if(tag_id_map.find(tag->id) != tag_id_map.end()) {
                            return &tag_id_map.find(tag->id)->second;
                        }

                        // Set up new tag entry
                        auto &new_tag = tag_array.emplace_back(*tag);
                        auto &previous_tag = tag_array[tag_array.size() - 2];
                        new_tag.id.index.index = previous_tag.id.index.index + 1;
                        new_tag.id.index.id = previous_tag.id.index.id + 1;
                        new_tag.path = TRANSLATE_ADDRESS(new_tag.path);

                        tag_data_header.tag_count++;

                        // Map new tag
                        tag_id_map.insert_or_assign(tag->id, new_tag.id);

                        // if current tag are indexed or if tags are already fixed, we can continue
                        if(tag->indexed) {
                            return &new_tag.id; 
                        }

                        new_tag.rebase_offsets(TRANSLATE_ADDRESS(new_tag.data));

                        new_tag.fix_dependencies([&](TagDependency tag_dependency) -> TagDependency {
                            if(tag_dependency.tag_id != -1) {
                                tag_dependency.path_pointer = TRANSLATE_ADDRESS(tag_dependency.path_pointer); 
                                auto *dependency = get_tag_from_secondary_map(tag_dependency.tag_id);
                                auto *tag_id = load_tag(dependency, true); 
                                tag_dependency.tag_id = tag_id->whole_id; 
                            } 
                            return tag_dependency;
                        });

                        switch(new_tag.primary_class) {
                            case TAG_CLASS_BITMAP: {
                                Bitmap *bitmap = reinterpret_cast<Bitmap *>(new_tag.data);
                                if(bitmap->bitmap_data.count > 0) {
                                    for(std::size_t j = 0; j < bitmap->bitmap_data.count; j++) {
                                        bitmap->bitmap_data.offset[j].pixel_data_offset += data_base_offset; 
                                    }
                                }
                                break;
                            }

                            case TAG_CLASS_SOUND: {
                                auto *sound = reinterpret_cast<Sound *>(new_tag.data);
                                for(std::size_t i = 0; i < sound->pitch_ranges.count; i++) {
                                    auto &pitch_range = sound->pitch_ranges.offset[i];
                                    for(std::size_t j = 0; j < pitch_range.permutations.count; j++) {
                                        auto &permutation = pitch_range.permutations.offset[j];
                                        TRANSLATE_DATA_OFFSET(permutation.samples);
                                        TRANSLATE_DATA_OFFSET(permutation.mouth_data);
                                        TRANSLATE_DATA_OFFSET(permutation.subtitle_data);
                                    }
                                }
                                break;
                            }

                            case TAG_CLASS_FONT: {
                                auto *font = reinterpret_cast<Font *>(new_tag.data);
                                for(std::size_t i = 0; i < font->characters.count; i++) {
                                    font->characters.offset[i].pixels_offset += data_base_offset;
                                }
                                break;
                            }

                            case TAG_CLASS_GBXMODEL: {
                                auto *gbxmodel = reinterpret_cast<Gbxmodel *>(new_tag.data);
                                for(std::size_t i = 0; i < gbxmodel->geometries.count; i++) {
                                    for(std::size_t j = 0; j < gbxmodel->geometries.offset[i].parts.count; j++) {
                                        gbxmodel->geometries.offset[i].parts.offset[j].vertex_offset += model_data_base_offset;
                                        gbxmodel->geometries.offset[i].parts.offset[j].triangle_offset += model_data_base_offset - tag_data_header.vertex_size + map_tag_data_header.vertex_size;
                                        gbxmodel->geometries.offset[i].parts.offset[j].triangle_offset_2 += model_data_base_offset - tag_data_header.vertex_size + map_tag_data_header.vertex_size;
                                    }                               
                                }
                                break;
                            }

                            case TAG_CLASS_MODEL_ANIMATIONS: {
                                auto *model_animations = reinterpret_cast<ModelAnimations *>(new_tag.data);
                                for(std::size_t i = 0; i < model_animations->animations.count; i++) {
                                    TRANSLATE_DATA_OFFSET(model_animations->animations.offset[i].frame_info);
                                    TRANSLATE_DATA_OFFSET(model_animations->animations.offset[i].default_data);
                                    TRANSLATE_DATA_OFFSET(model_animations->animations.offset[i].frame_data);
                                }
                                break;
                            }

                            default: {
                                break;
                            }
                        }

                        return &new_tag.id;
                    };

                    for(std::size_t i = 0; i < map_tag_data_header.tag_count; i++) {
                        auto *tag = tag_array_raw + i;
                        switch(tag->primary_class) {
                            case TAG_CLASS_SCENARIO:
                            case TAG_CLASS_SCENARIO_STRUCTURE_BSP:
                                continue;

                            default:
                                load_tag(tag, false);
                        }
                    }
                }

                data_base_offset += std::filesystem::file_size(map.path);
                model_data_base_offset += map_tag_data_header.model_data_size;
            }

            tag_data_header.tag_array = tag_array.data();

            return 1;
        }

        auto &map = loaded_maps.front();
        auto file_size = std::filesystem::file_size(map.path);
        if(file_offset > file_size) {
            auto acc = file_size; 
            for(auto &map : loaded_maps) {
                if(!map.secondary) {
                    continue;
                }
                
                file_size = std::filesystem::file_size(map.path);
                if(file_offset <= acc + file_size) {
                    std::FILE *file = std::fopen(map.path.string().c_str(), "rb");
                    std::fseek(file, file_offset - acc, SEEK_SET);
                    std::fread(output, 1, size, file);
                    std::fclose(file);
                    return 1;
                }
                acc += file_size;
            }

            char error[256];
            std::snprintf(error, sizeof(error), "File offset %zu is out of bounds", file_offset);
            MessageBoxA(nullptr, error, "Error", MB_OK);
            std::exit(EXIT_FAILURE);
        }

        if(loaded_maps[0].tag_data_header().model_data_file_offset == file_offset && loaded_maps[0].tag_data_header().model_data_size == size) {
            std::size_t buffer_cursor = 0;
            for(auto &map : loaded_maps) {
                std::FILE *file = std::fopen(map.path.string().c_str(), "rb");
                std::fseek(file, map.tag_data_header().model_data_file_offset, SEEK_SET);
                std::fread(output + buffer_cursor, 1, map.tag_data_header().model_data_size, file);
                std::fclose(file);
                buffer_cursor += map.tag_data_header().model_data_size;
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
            *size += map.tag_data_header().model_data_size;
        }
    }

    void set_up_map_loading() {
        auto &balltze = Balltze::get();
        auto &sig_manager = balltze.signature_manager();

        // Hook loading normal maps
        static auto *load_map_path_sig = sig_manager.get("map_load_path");
        static Memory::Hook load_map_path_hook;
        load_map_path_hook.initialize(load_map_path_sig->data(), reinterpret_cast<void *>(map_loading_asm));
        load_map_path_hook.hook();

        // Handle this
        static auto *read_cache_file_data_sig = sig_manager.get("read_map_file_data");
        static Memory::Hook read_cache_file_data_hook;
        read_cache_file_data_hook.initialize(read_cache_file_data_sig->data(), reinterpret_cast<void *>(on_read_map_file_data_asm));
        read_cache_file_data_hook.hook();

        // Hook model data buffer allocation
        static auto *model_data_buffer_alloc_sig = sig_manager.get("model_data_buffer_alloc");
        static Memory::Hook model_data_buffer_alloc_hook;
        model_data_buffer_alloc_hook.initialize(model_data_buffer_alloc_sig->data(), reinterpret_cast<void *>(on_model_data_buffer_alloc_asm));
        model_data_buffer_alloc_hook.hook();
    }
}
