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
#include <balltze/engine/tag_definitions/ui_widget_definition.hpp>
#include <balltze/engine/tag_definitions/unicode_string_list.hpp>
#include <balltze/engine/tag_definitions/bitmap.hpp>
#include <balltze/engine/tag_definitions/sound.hpp>
#include <balltze/engine/tag_definitions/font.hpp>
#include <balltze/engine/tag_definitions/gbxmodel.hpp>
#include <balltze/engine/tag_definitions/shader_model.hpp>
#include <balltze/engine/tag_definitions/shader_transparent_chicago.hpp>
#include <balltze/engine/tag_definitions/shader_transparent_chicago_extended.hpp>
#include <balltze/engine/tag_definitions/shader_transparent_glass.hpp>
#include <balltze/engine/tag_definitions/scenery.hpp>
#include <balltze/engine/tag_definitions/model_animations.hpp>
#include <balltze/engine/tag_definitions/model_collision_geometry.hpp>
#include <balltze/engine/tag_definitions/physics.hpp>
#include <balltze/engine/tag_definitions/effect.hpp>
#include <balltze/engine/tag_definitions/damage_effect.hpp>
#include <balltze/engine/tag_definitions/light.hpp>
#include <balltze/engine/tag_definitions/lens_flare.hpp>
#include <balltze/engine/tag_definitions/particle_system.hpp>
#include <balltze/engine/tag_definitions/decal.hpp>
#include <balltze/engine/tag_definitions/particle.hpp>
#include <balltze/engine/tag_definitions/material_effects.hpp>
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
                            case TAG_CLASS_BITMAP: {
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

                            case TAG_CLASS_UI_WIDGET_DEFINITION: {
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

                            case TAG_CLASS_UNICODE_STRING_LIST: {
                                auto *unicode_string_list = reinterpret_cast<UnicodeStringList *>(new_tag.data);
                                
                                TRANSLATE_ADDRESS(unicode_string_list->strings.offset);
                                
                                for(std::size_t j = 0; j < unicode_string_list->strings.count; j++) {
                                    TRANSLATE_DATA_OFFSET(unicode_string_list->strings.offset[j].string);
                                }

                                break;
                            }

                            case TAG_CLASS_SOUND: {
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

                            case TAG_CLASS_FONT: {
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

                            case TAG_CLASS_GBXMODEL: {
                                auto *gbxmodel = reinterpret_cast<Gbxmodel *>(new_tag.data);

                                TRANSLATE_ADDRESS(gbxmodel->markers.offset);
                                for(std::size_t i = 0; i < gbxmodel->markers.count; i++) {
                                    TRANSLATE_ADDRESS(gbxmodel->markers.offset[i].instances.offset);
                                }

                                TRANSLATE_ADDRESS(gbxmodel->nodes.offset);

                                TRANSLATE_ADDRESS(gbxmodel->regions.offset);
                                for(std::size_t i = 0; i < gbxmodel->regions.count; i++) {  
                                    TRANSLATE_ADDRESS(gbxmodel->regions.offset[i].permutations.offset);

                                    auto &region = gbxmodel->regions.offset[i];

                                    for(std::size_t i = 0; i < region.permutations.count; i++) {
                                        TRANSLATE_ADDRESS(region.permutations.offset[i].markers.offset);
                                    }
                                }

                                TRANSLATE_ADDRESS(gbxmodel->geometries.offset);
                                for(std::size_t i = 0; i < gbxmodel->geometries.count; i++) {
                                    TRANSLATE_ADDRESS(gbxmodel->geometries.offset[i].parts.offset);

                                    for(std::size_t j = 0; j < gbxmodel->geometries.offset[i].parts.count; j++) {
                                        TRANSLATE_ADDRESS(gbxmodel->geometries.offset[i].parts.offset[j].uncompressed_vertices.offset);
                                        TRANSLATE_ADDRESS(gbxmodel->geometries.offset[i].parts.offset[j].compressed_vertices.offset);
                                        TRANSLATE_ADDRESS(gbxmodel->geometries.offset[i].parts.offset[j].triangles.offset);

                                        gbxmodel->geometries.offset[i].parts.offset[j].vertex_offset += model_data_base_offset;
                                        gbxmodel->geometries.offset[i].parts.offset[j].triangle_offset += model_data_base_offset - tag_data_header.vertex_size + map_tag_data_header.vertex_size;
                                        gbxmodel->geometries.offset[i].parts.offset[j].triangle_offset_2 += model_data_base_offset - tag_data_header.vertex_size + map_tag_data_header.vertex_size;
                                    }                               
                                }

                                TRANSLATE_ADDRESS(gbxmodel->shaders.offset);
                                for(std::size_t i = 0; i < gbxmodel->shaders.count; i++) {
                                    TRANSLATE_DEPENDENCY(gbxmodel->shaders.offset[i].shader);
                                }

                                break;
                            }

                            case TAG_CLASS_SHADER_MODEL: {
                                auto *shader_model = reinterpret_cast<ShaderModel *>(new_tag.data);

                                TRANSLATE_DEPENDENCY(shader_model->base_map);
                                TRANSLATE_DEPENDENCY(shader_model->multipurpose_map);
                                TRANSLATE_DEPENDENCY(shader_model->detail_map);
                                TRANSLATE_DEPENDENCY(shader_model->reflection_cube_map);

                                break;
                            }

                            case TAG_CLASS_MODEL_ANIMATIONS: {
                                auto *model_animations = reinterpret_cast<ModelAnimations *>(new_tag.data);

                                TRANSLATE_ADDRESS(model_animations->objects.offset);

                                TRANSLATE_ADDRESS(model_animations->units.offset);
                                for(std::size_t i = 0; i < model_animations->units.count; i++) {
                                    TRANSLATE_ADDRESS(model_animations->units.offset[i].animations.offset);
                                    TRANSLATE_ADDRESS(model_animations->units.offset[i].ik_points.offset);
                                    
                                    TRANSLATE_ADDRESS(model_animations->units.offset[i].weapons.offset);
                                    for(std::size_t j = 0; j < model_animations->units.offset[i].weapons.count; j++) {
                                        TRANSLATE_ADDRESS(model_animations->units.offset[i].weapons.offset[j].animations.offset);
                                        TRANSLATE_ADDRESS(model_animations->units.offset[i].weapons.offset[j].ik_point.offset);

                                        TRANSLATE_ADDRESS(model_animations->units.offset[i].weapons.offset[j].weapon_types.offset);
                                        for(std::size_t k = 0; k < model_animations->units.offset[i].weapons.offset[j].weapon_types.count; k++) {
                                            TRANSLATE_ADDRESS(model_animations->units.offset[i].weapons.offset[j].weapon_types.offset[k].animations.offset);
                                        }
                                    }
                                }

                                TRANSLATE_ADDRESS(model_animations->weapons.offset);
                                for(std::size_t i = 0; i < model_animations->weapons.count; i++) {
                                    TRANSLATE_ADDRESS(model_animations->weapons.offset[i].animations.offset);
                                }

                                TRANSLATE_ADDRESS(model_animations->vehicles.offset);
                                for(std::size_t i = 0; i < model_animations->vehicles.count; i++) {
                                    TRANSLATE_ADDRESS(model_animations->vehicles.offset[i].animations.offset);
                                    TRANSLATE_ADDRESS(model_animations->vehicles.offset[i].suspension_animations.offset);
                                }

                                TRANSLATE_ADDRESS(model_animations->devices.offset);
                                for(std::size_t i = 0; i < model_animations->devices.count; i++) {
                                    TRANSLATE_ADDRESS(model_animations->devices.offset[i].animations.offset);
                                }

                                TRANSLATE_ADDRESS(model_animations->unit_damage.offset);
                                
                                TRANSLATE_ADDRESS(model_animations->first_person_weapons.offset);
                                for(std::size_t i = 0; i < model_animations->first_person_weapons.count; i++) {
                                    TRANSLATE_ADDRESS(model_animations->first_person_weapons.offset[i].animations.offset);
                                }

                                TRANSLATE_ADDRESS(model_animations->sound_references.offset);
                                for(std::size_t i = 0; i < model_animations->sound_references.count; i++) {
                                    TRANSLATE_DEPENDENCY(model_animations->sound_references.offset[i].sound);
                                }

                                TRANSLATE_ADDRESS(model_animations->nodes.offset);

                                TRANSLATE_ADDRESS(model_animations->animations.offset);
                                for(std::size_t i = 0; i < model_animations->animations.count; i++) {
                                    TRANSLATE_DATA_OFFSET(model_animations->animations.offset[i].frame_info);
                                    TRANSLATE_DATA_OFFSET(model_animations->animations.offset[i].default_data);
                                    TRANSLATE_DATA_OFFSET(model_animations->animations.offset[i].frame_data);
                                }

                                break;
                            }

                            case TAG_CLASS_SHADER_TRANSPARENT_CHICAGO: {
                                auto *shader_transparent_chicago = reinterpret_cast<ShaderTransparentChicago *>(new_tag.data);
        
                                TRANSLATE_DEPENDENCY(shader_transparent_chicago->lens_flare);

                                TRANSLATE_ADDRESS(shader_transparent_chicago->extra_layers.offset);
                                for(std::size_t i = 0; i < shader_transparent_chicago->extra_layers.count; i++) {
                                    TRANSLATE_DEPENDENCY(shader_transparent_chicago->extra_layers.offset[i].shader);
                                }

                                TRANSLATE_ADDRESS(shader_transparent_chicago->maps.offset);
                                for(std::size_t i = 0; i < shader_transparent_chicago->maps.count; i++) {
                                    TRANSLATE_DEPENDENCY(shader_transparent_chicago->maps.offset[i].map);
                                }

                                break;
                            }

                            case TAG_CLASS_SHADER_TRANSPARENT_CHICAGO_EXTENDED: {
                                auto *shader_transparent_chicago_extended = reinterpret_cast<ShaderTransparentChicagoExtended *>(new_tag.data);
        
                                TRANSLATE_DEPENDENCY(shader_transparent_chicago_extended->lens_flare);

                                TRANSLATE_ADDRESS(shader_transparent_chicago_extended->extra_layers.offset);
                                for(std::size_t i = 0; i < shader_transparent_chicago_extended->extra_layers.count; i++) {
                                    TRANSLATE_DEPENDENCY(shader_transparent_chicago_extended->extra_layers.offset[i].shader);
                                }

                                TRANSLATE_ADDRESS(shader_transparent_chicago_extended->maps_4_stage.offset);
                                for(std::size_t i = 0; i < shader_transparent_chicago_extended->maps_4_stage.count; i++) {
                                    TRANSLATE_DEPENDENCY(shader_transparent_chicago_extended->maps_4_stage.offset[i].map);
                                }

                                TRANSLATE_ADDRESS(shader_transparent_chicago_extended->maps_2_stage.offset);
                                for(std::size_t i = 0; i < shader_transparent_chicago_extended->maps_2_stage.count; i++) {
                                    TRANSLATE_DEPENDENCY(shader_transparent_chicago_extended->maps_2_stage.offset[i].map);
                                }

                                break;
                            }

                            case TAG_CLASS_SHADER_TRANSPARENT_GLASS: {
                                auto *shader_transparent_generic = reinterpret_cast<ShaderTransparentGlass *>(new_tag.data);

                                TRANSLATE_DEPENDENCY(shader_transparent_generic->background_tint_map);
                                TRANSLATE_DEPENDENCY(shader_transparent_generic->reflection_map);
                                TRANSLATE_DEPENDENCY(shader_transparent_generic->bump_map);
                                TRANSLATE_DEPENDENCY(shader_transparent_generic->diffuse_map);
                                TRANSLATE_DEPENDENCY(shader_transparent_generic->diffuse_detail_map);
                                TRANSLATE_DEPENDENCY(shader_transparent_generic->specular_map);
                                TRANSLATE_DEPENDENCY(shader_transparent_generic->specular_detail_map);

                                break;
                            }

                            case TAG_CLASS_SCENERY: {
                                auto *scenery = reinterpret_cast<Scenery *>(new_tag.data);

                                TRANSLATE_DEPENDENCY(scenery->model);
                                TRANSLATE_DEPENDENCY(scenery->animation_graph);
                                TRANSLATE_DEPENDENCY(scenery->collision_model);
                                TRANSLATE_DEPENDENCY(scenery->physics);
                                TRANSLATE_DEPENDENCY(scenery->modifier_shader);
                                TRANSLATE_DEPENDENCY(scenery->creation_effect);

                                TRANSLATE_ADDRESS(scenery->attachments.offset);
                                for(std::size_t i = 0; i < scenery->attachments.count; i++) {
                                    TRANSLATE_DEPENDENCY(scenery->attachments.offset[i].type);
                                }

                                TRANSLATE_ADDRESS(scenery->widgets.offset);
                                for(std::size_t i = 0; i < scenery->widgets.count; i++) {
                                    TRANSLATE_DEPENDENCY(scenery->widgets.offset[i].reference);
                                }

                                TRANSLATE_ADDRESS(scenery->functions.offset);

                                TRANSLATE_ADDRESS(scenery->change_colors.offset);
                                for(std::size_t i = 0; i < scenery->change_colors.count; i++) {
                                    TRANSLATE_ADDRESS(scenery->change_colors.offset[i].permutations.offset);
                                }

                                TRANSLATE_ADDRESS(scenery->predicted_resources.offset);

                                break;
                            }

                            case TAG_CLASS_MODEL_COLLISION_GEOMETRY: {
                                auto *model_collision_geometry = reinterpret_cast<ModelCollisionGeometry *>(new_tag.data);

                                TRANSLATE_DEPENDENCY(model_collision_geometry->localized_damage_effect);
                                TRANSLATE_DEPENDENCY(model_collision_geometry->area_damage_effect);
                                TRANSLATE_DEPENDENCY(model_collision_geometry->body_damaged_effect);
                                TRANSLATE_DEPENDENCY(model_collision_geometry->body_depleted_effect);
                                TRANSLATE_DEPENDENCY(model_collision_geometry->body_destroyed_effect);
                                TRANSLATE_DEPENDENCY(model_collision_geometry->shield_damaged_effect);
                                TRANSLATE_DEPENDENCY(model_collision_geometry->shield_depleted_effect);
                                TRANSLATE_DEPENDENCY(model_collision_geometry->shield_recharging_effect);

                                TRANSLATE_ADDRESS(model_collision_geometry->materials.offset);

                                TRANSLATE_ADDRESS(model_collision_geometry->regions.offset);
                                for(std::size_t i = 0; i < model_collision_geometry->regions.count; i++) {
                                    TRANSLATE_DEPENDENCY(model_collision_geometry->regions.offset[i].destroyed_effect);
                                    TRANSLATE_ADDRESS(model_collision_geometry->regions.offset[i].permutations.offset);
                                }
                                
                                TRANSLATE_ADDRESS(model_collision_geometry->modifiers.offset);
                                
                                TRANSLATE_ADDRESS(model_collision_geometry->pathfinding_spheres.offset);

                                TRANSLATE_ADDRESS(model_collision_geometry->nodes.offset);
                                for(std::size_t i = 0; i < model_collision_geometry->nodes.count; i++) {
                                    TRANSLATE_ADDRESS(model_collision_geometry->nodes.offset[i].bsps.offset);
                                    for(std::size_t j = 0; j < model_collision_geometry->nodes.offset[i].bsps.count; j++) {
                                        TRANSLATE_ADDRESS(model_collision_geometry->nodes.offset[i].bsps.offset[j].bsp3d_nodes.offset);
                                        TRANSLATE_ADDRESS(model_collision_geometry->nodes.offset[i].bsps.offset[j].planes.offset);
                                        TRANSLATE_ADDRESS(model_collision_geometry->nodes.offset[i].bsps.offset[j].leaves.offset);
                                        TRANSLATE_ADDRESS(model_collision_geometry->nodes.offset[i].bsps.offset[j].bsp2d_references.offset);
                                        TRANSLATE_ADDRESS(model_collision_geometry->nodes.offset[i].bsps.offset[j].bsp2d_nodes.offset);
                                        TRANSLATE_ADDRESS(model_collision_geometry->nodes.offset[i].bsps.offset[j].surfaces.offset);
                                        TRANSLATE_ADDRESS(model_collision_geometry->nodes.offset[i].bsps.offset[j].edges.offset);
                                        TRANSLATE_ADDRESS(model_collision_geometry->nodes.offset[i].bsps.offset[j].vertices.offset);
                                    }
                                }

                                break;
                            }

                            case TAG_CLASS_PHYSICS: {
                                auto *physics = reinterpret_cast<Physics *>(new_tag.data);

                                TRANSLATE_ADDRESS(physics->inertial_matrix_and_inverse.offset);
                                TRANSLATE_ADDRESS(physics->powered_mass_points.offset);
                                TRANSLATE_ADDRESS(physics->mass_points.offset);

                                break;
                            }

                            case TAG_CLASS_EFFECT: {
                                auto *effect = reinterpret_cast<Effect *>(new_tag.data);

                                TRANSLATE_ADDRESS(effect->locations.offset);

                                TRANSLATE_ADDRESS(effect->events.offset);
                                for(std::size_t i = 0; i < effect->events.count; i++) {
                                    TRANSLATE_ADDRESS(effect->events.offset[i].parts.offset);
                                    for(std::size_t j = 0; j < effect->events.offset[i].parts.count; j++) {
                                        TRANSLATE_DEPENDENCY(effect->events.offset[i].parts.offset[j].type);
                                    }

                                    TRANSLATE_ADDRESS(effect->events.offset[i].particles.offset);
                                    for(std::size_t j = 0; j < effect->events.offset[i].particles.count; j++) {
                                        TRANSLATE_DEPENDENCY(effect->events.offset[i].particles.offset[j].particle_type);
                                    }
                                }

                                break;
                            }

                            case TAG_CLASS_DAMAGE_EFFECT: {
                                auto *damage_effect = reinterpret_cast<DamageEffect *>(new_tag.data);

                                TRANSLATE_DEPENDENCY(damage_effect->sound);

                                break;
                            }

                            case TAG_CLASS_LIGHT: {
                                auto *light = reinterpret_cast<Light *>(new_tag.data);

                                TRANSLATE_DEPENDENCY(light->primary_cube_map);
                                TRANSLATE_DEPENDENCY(light->secondary_cube_map);
                                TRANSLATE_DEPENDENCY(light->lens_flare);

                                break;
                            }

                            case TAG_CLASS_LENS_FLARE: {
                                auto *lens_flare = reinterpret_cast<LensFlare *>(new_tag.data);

                                TRANSLATE_DEPENDENCY(lens_flare->bitmap);
                                TRANSLATE_ADDRESS(lens_flare->reflections.offset);

                                break;
                            }

                            case TAG_CLASS_PARTICLE_SYSTEM: {
                                auto *particle_system = reinterpret_cast<ParticleSystem *>(new_tag.data);

                                TRANSLATE_DEPENDENCY(particle_system->point_physics);

                                TRANSLATE_ADDRESS(particle_system->physics_constants.offset);

                                TRANSLATE_ADDRESS(particle_system->particle_types.offset);
                                for(std::size_t i = 0; i < particle_system->particle_types.count; i++) {
                                    TRANSLATE_ADDRESS(particle_system->particle_types.offset[i].physics_constants.offset);

                                    TRANSLATE_ADDRESS(particle_system->particle_types.offset[i].states.offset);
                                    for(std::size_t j = 0; j < particle_system->particle_types.offset[i].states.count; j++) {
                                        TRANSLATE_ADDRESS(particle_system->particle_types.offset[i].states.offset[j].physics_constants.offset);
                                    }

                                    TRANSLATE_ADDRESS(particle_system->particle_types.offset[i].particle_states.offset);
                                    for(std::size_t j = 0; j < particle_system->particle_types.offset[i].particle_states.count; j++) {
                                        TRANSLATE_DEPENDENCY(particle_system->particle_types.offset[i].particle_states.offset[j].bitmaps);
                                        TRANSLATE_DEPENDENCY(particle_system->particle_types.offset[i].particle_states.offset[j].point_physics);
                                        TRANSLATE_DEPENDENCY(particle_system->particle_types.offset[i].particle_states.offset[j].secondary_map_bitmap);
                                        TRANSLATE_ADDRESS(particle_system->particle_types.offset[i].particle_states.offset[j].physics_constants.offset);
                                    }
                                }

                                break;
                            }

                            case TAG_CLASS_POINT_PHYSICS: {
                                break;
                            }

                            case TAG_CLASS_DECAL: {
                                auto *decal = reinterpret_cast<Decal *>(new_tag.data);

                                TRANSLATE_DEPENDENCY(decal->next_decal_in_chain);
                                TRANSLATE_DEPENDENCY(decal->map);

                                break;
                            }

                            case TAG_CLASS_PARTICLE: {
                                auto *particle = reinterpret_cast<Particle *>(new_tag.data);

                                TRANSLATE_DEPENDENCY(particle->bitmap);
                                TRANSLATE_DEPENDENCY(particle->physics);
                                TRANSLATE_DEPENDENCY(particle->sir_marty_exchanged_his_children_for_thine);
                                TRANSLATE_DEPENDENCY(particle->collision_effect);
                                TRANSLATE_DEPENDENCY(particle->death_effect);
                                TRANSLATE_DEPENDENCY(particle->secondary_bitmap);

                                break;
                            }

                            case TAG_CLASS_MATERIAL_EFFECTS: {
                                auto *material_effects = reinterpret_cast<MaterialEffects *>(new_tag.data);

                                TRANSLATE_ADDRESS(material_effects->effects.offset);
                                for(std::size_t i = 0; i < material_effects->effects.count; i++) {
                                    TRANSLATE_ADDRESS(material_effects->effects.offset[i].materials.offset);
                                    for(std::size_t j = 0; j < material_effects->effects.offset[i].materials.count; j++) {
                                        TRANSLATE_DEPENDENCY(material_effects->effects.offset[i].materials.offset[j].effect);
                                        TRANSLATE_DEPENDENCY(material_effects->effects.offset[i].materials.offset[j].sound);
                                    }
                                }

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
