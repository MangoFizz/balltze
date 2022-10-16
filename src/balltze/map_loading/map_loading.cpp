// SPDX-License-Identifier: GPL-3.0-only

#include <windows.h>
#include <filesystem>
#include <vector>
#include <map>
#include <deque>
#include <memory>
#include <functional>

#include <balltze/engine/map.hpp>
#include <balltze/engine/path.hpp>
#include <balltze/engine/tag.hpp>
#include <balltze/engine/tag_definitions/ui_widget_definition.hpp>
#include <balltze/engine/tag_definitions/unicode_string_list.hpp>
#include <balltze/engine/tag_definitions/bitmap.hpp>
#include <balltze/engine/tag_definitions/sound.hpp>
#include <balltze/engine/tag_definitions/font.hpp>
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
        bool secondary = false;

        TagDataHeader tag_data_header() noexcept {
            std::FILE *file = std::fopen(this->path.string().c_str(), "rb");
            MapHeader header;
            TagDataHeader tag_data_header;

            // Read map header
            std::fread(&header, sizeof(MapHeader), 1, file);
            
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
        }
    };

    static std::deque<LoadedMap> loaded_maps;
    static std::vector<Tag> tag_array;
    static std::unique_ptr<std::byte[]> tag_data;
    static std::size_t tag_data_cursor = 0;

    extern "C" {
        void map_loading_asm() noexcept;
        void on_read_map_file_data_asm() noexcept;
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
            
            // Allocate buffer for maps tag data
            tag_data = std::make_unique<std::byte[]>((64 * 1024 * 1024) * loaded_maps.size());

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
                            TagClassInt::TAG_CLASS_BITMAP,
                            TagClassInt::TAG_CLASS_UNICODE_STRING_LIST,
                            TagClassInt::TAG_CLASS_SOUND,
                            TagClassInt::TAG_CLASS_FONT,
                            TagClassInt::TAG_CLASS_UI_WIDGET_DEFINITION
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

                        #define TRANSLATE_ADDRESS(x) { \
                            if(reinterpret_cast<void *>(x) != nullptr) { \
                                x = reinterpret_cast<decltype(x)>(reinterpret_cast<std::byte *>(x) - tag_data_base_address_disp); \
                            } \
                        }   

                        #define TRANSLATE_DEPENDENCY(tag_dependency) { \
                            if(tag_dependency.tag_id != -1) { \
                                TRANSLATE_ADDRESS(tag_dependency.path_pointer); \
                                auto *dependency = get_tag_from_secondary_map(tag_dependency.tag_id); \
                                auto *tag_id = load_tag(dependency, true); \
                                tag_dependency.tag_id = tag_id->whole_id; \
                            } \
                        }

                        #define TRANSLATE_DATA_OFFSET(tag_data_offset) { \
                            if(reinterpret_cast<std::byte *>(tag_data_offset.pointer) != nullptr) { \
                                TRANSLATE_ADDRESS(tag_data_offset.pointer); \
                            } \
                            else if(tag_data_offset.file_offset != 0) { \
                                tag_data_offset.file_offset += data_base_offset; \
                            } \
                        }

                        // Check if current tag class is supported
                        if(!is_supported_tag(tag->primary_class)) {
                            if(required) {
                                char error[2048];
                                char *tag_path = tag->path;
                                TRANSLATE_ADDRESS(tag_path);
                                std::snprintf(error, sizeof(error), "Loading tag %s from map %s is not supported", tag_path, map.name.c_str());
                                MessageBoxA(nullptr, error, "Error", MB_OK);
                                std::exit(EXIT_FAILURE);
                            }
                            return nullptr;
                        }

                        // Check if tag already exists in main map... or maybe not
                        if(tag->indexed) {
                            char *tag_path = tag->path;
                            TRANSLATE_ADDRESS(tag_path);
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
                        TRANSLATE_ADDRESS(new_tag.path);

                        if(!tag->indexed) {
                            // Translate data address before fix tag
                            TRANSLATE_ADDRESS(new_tag.data);
                        }

                        tag_data_header.tag_count++;

                        // Map new tag
                        tag_id_map.insert_or_assign(tag->id, new_tag.id);

                        // if current tag are indexed or if tags are already fixed, we can continue
                        if(tag->indexed) {
                            return &new_tag.id; 
                        }

                        switch(new_tag.primary_class) {
                            case TagClassInt::TAG_CLASS_BITMAP: {
                                Bitmap *bitmap = reinterpret_cast<Bitmap *>(new_tag.data);

                                if(bitmap->bitmap_group_sequence.count > 0) {
                                    TRANSLATE_ADDRESS(bitmap->bitmap_group_sequence.offset);
                                    for(std::size_t j = 0; j < bitmap->bitmap_group_sequence.count; j++) {
                                        auto *sequence = bitmap->bitmap_group_sequence.offset + j;
                                        if(sequence->sprites.count > 0) {
                                            TRANSLATE_ADDRESS(sequence->sprites.offset);
                                        }
                                    }
                                }

                                if(bitmap->bitmap_data.count > 0) {
                                    TRANSLATE_ADDRESS(bitmap->bitmap_data.offset);

                                    for(std::size_t j = 0; j < bitmap->bitmap_data.count; j++) {
                                        bitmap->bitmap_data.offset[j].pixel_data_offset += data_base_offset; 
                                    }
                                }

                                TRANSLATE_DATA_OFFSET(bitmap->compressed_color_plate_data);
                                TRANSLATE_DATA_OFFSET(bitmap->processed_pixel_data);

                                break;
                            }

                            case TagClassInt::TAG_CLASS_UI_WIDGET_DEFINITION: {
                                auto *ui_widget_definition = reinterpret_cast<UiWidgetDefinition *>(new_tag.data);

                                TRANSLATE_ADDRESS(ui_widget_definition->game_data_inputs.offset);
                                TRANSLATE_ADDRESS(ui_widget_definition->event_handlers.offset);
                                TRANSLATE_ADDRESS(ui_widget_definition->search_and_replace_functions.offset);
                                TRANSLATE_ADDRESS(ui_widget_definition->conditional_widgets.offset);
                                TRANSLATE_ADDRESS(ui_widget_definition->child_widgets.offset);

                                TRANSLATE_DEPENDENCY(ui_widget_definition->background_bitmap);
                                TRANSLATE_DEPENDENCY(ui_widget_definition->text_label_unicode_strings_list);
                                TRANSLATE_DEPENDENCY(ui_widget_definition->text_font);
                                TRANSLATE_DEPENDENCY(ui_widget_definition->list_header_bitmap);
                                TRANSLATE_DEPENDENCY(ui_widget_definition->list_footer_bitmap);
                                TRANSLATE_DEPENDENCY(ui_widget_definition->extended_description_widget);
                                
                                for(std::size_t i = 0; i < ui_widget_definition->event_handlers.count; i++) {
                                    TRANSLATE_DEPENDENCY(ui_widget_definition->event_handlers.offset[i].widget_tag);
                                    TRANSLATE_DEPENDENCY(ui_widget_definition->event_handlers.offset[i].sound_effect);
                                }

                                for(std::size_t i = 0; i < ui_widget_definition->conditional_widgets.count; i++) {
                                    TRANSLATE_DEPENDENCY(ui_widget_definition->conditional_widgets.offset[i].widget_tag);
                                }

                                for(std::size_t i = 0; i < ui_widget_definition->child_widgets.count; i++) {
                                    TRANSLATE_DEPENDENCY(ui_widget_definition->child_widgets.offset[i].widget_tag);
                                }

                                break;
                            }

                            case TagClassInt::TAG_CLASS_UNICODE_STRING_LIST: {
                                auto *unicode_string_list = reinterpret_cast<UnicodeStringList *>(new_tag.data);
                                
                                TRANSLATE_ADDRESS(unicode_string_list->strings.offset);
                                
                                for(std::size_t j = 0; j < unicode_string_list->strings.count; j++) {
                                    TRANSLATE_DATA_OFFSET(unicode_string_list->strings.offset[j].string);
                                }

                                break;
                            }

                            case TagClassInt::TAG_CLASS_SOUND: {
                                auto *sound = reinterpret_cast<Sound *>(new_tag.data);

                                TRANSLATE_ADDRESS(sound->pitch_ranges.offset);

                                TRANSLATE_DEPENDENCY(sound->promotion_sound);
                                
                                for(std::size_t i = 0; i < sound->pitch_ranges.count; i++) {
                                    auto &pitch_range = sound->pitch_ranges.offset[i];
                                    
                                    TRANSLATE_ADDRESS(pitch_range.permutations.offset);
                                    
                                    for(std::size_t j = 0; j < pitch_range.permutations.count; j++) {
                                        auto &permutation = pitch_range.permutations.offset[j];
                                        TRANSLATE_DATA_OFFSET(permutation.samples);
                                        TRANSLATE_DATA_OFFSET(permutation.mouth_data);
                                        TRANSLATE_DATA_OFFSET(permutation.subtitle_data);
                                    }
                                }

                                break;
                            }

                            case TagClassInt::TAG_CLASS_FONT: {
                                auto *font = reinterpret_cast<Font *>(new_tag.data);
                                
                                TRANSLATE_ADDRESS(font->character_tables.offset);
                                TRANSLATE_ADDRESS(font->characters.offset);

                                TRANSLATE_DEPENDENCY(font->bold);
                                TRANSLATE_DEPENDENCY(font->italic);
                                TRANSLATE_DEPENDENCY(font->condense);
                                TRANSLATE_DEPENDENCY(font->underline);

                                for(std::size_t i = 0; i < font->character_tables.count; i++) {
                                    TRANSLATE_ADDRESS(font->character_tables.offset[i].character_table.offset);
                                }

                                for(std::size_t i = 0; i < font->characters.count; i++) {
                                    font->characters.offset[i].pixels_offset += data_base_offset;
                                }

                                TRANSLATE_DATA_OFFSET(font->pixels);
                                
                                break;
                            }
                        }

                        return &new_tag.id;
                    };

                    for(std::size_t i = 0; i < map_tag_data_header.tag_count; i++) {
                        load_tag(tag_array_raw + i, false);
                    }
                }

                data_base_offset += std::filesystem::file_size(map.path);
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

        return 0;
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
    }
}
