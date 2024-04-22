// SPDX-License-Identifier: GPL-3.0-only

#include <cstdint>
#include <cmath>
#include <balltze/engine/rasterizer.hpp>
#include <balltze/engine/tag.hpp>
#include <balltze/engine/tag_definitions.hpp>
#include <balltze/engine/rasterizer.hpp>
#include <balltze/engine/script.hpp>
#include <balltze/event.hpp>
#include <balltze/memory.hpp>
#include <balltze/hook.hpp>
#include <balltze/command.hpp>
#include "../../logger.hpp"

namespace Balltze::Features {
    using namespace Engine;
    using namespace Engine::TagDefinitions;
    using namespace Engine::Rasterizer;

    extern "C" {
        bool use_shader_transparent_chicago_reimplementation = true;
        bool draw_shader_transparent_chicago_hook();

        void draw_shader_transparent_chicago(TransparentGeometryGroup *transparent_geometry_group, std::uint32_t *param_2) {
            auto *device = get_d3d9_device();
            auto *shader_data = reinterpret_cast<ShaderTransparentChicago *>(transparent_geometry_group->shader_tag_data);
            
            auto *maps_elements = shader_data->maps.elements;
            if(maps_elements == nullptr) {
                return;
            }
            if(maps_elements->map.path == 0) {
                return;
            }

            auto vertex_shader_permutation = get_vertex_shader_permutation_index(shader_data);
            auto vertex_type = get_transparent_geometry_group_vertex_type(transparent_geometry_group);
            auto *vertex_shader = get_vertex_shader_index_for_permutation(vertex_type, vertex_shader_permutation);
            auto *vertex_declaration = get_vertex_declaration(vertex_type);
            device->SetVertexShader(vertex_shader->shader);
            device->SetVertexDeclaration(vertex_declaration->declaration);
            device->SetPixelShader(nullptr);
            
            for(int i = 0; i < shader_data->extra_layers.count; i++) {
                TransparentGeometryGroup group_copy;
                auto *extra_layer_elements = shader_data->extra_layers.elements;
                memcpy(&group_copy, transparent_geometry_group, sizeof(TransparentGeometryGroup));
                auto *extra_layer_shader_tag = get_tag(extra_layer_elements[i].shader.tag_handle);
                group_copy.shader_tag_data = extra_layer_shader_tag->data;
                render_transparent_geometry_group(&group_copy, param_2);
            }

            auto &flags = shader_data->shader_transparent_chicago_flags;
            device->SetRenderState(D3DRS_CULLMODE, flags.decal ? D3DCULL_CCW : D3DCULL_NONE);
            device->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
            device->SetRenderState(D3DRS_ALPHABLENDENABLE, D3DBLENDOP_ADD);
            device->SetRenderState(D3DRS_ALPHATESTENABLE, flags.alpha_tested ? TRUE : FALSE);
            device->SetRenderState(D3DRS_ALPHAREF, 0x0000007F);
            device->SetRenderState(D3DRS_FOGENABLE, FALSE);

            set_framebuffer_blend_function(shader_data->framebuffer_blend_function);

            short bitmap_data_index = transparent_geometry_group->bitmap_index;
            if(flags.numeric && transparent_geometry_group->field_0x74 != nullptr && shader_data->maps.count > 0) {
                auto *bitmap_tag = get_tag(shader_data->maps.elements[0].map.tag_handle);
                auto *bitmap_data = reinterpret_cast<Bitmap *>(bitmap_tag->data);
                auto bitmap_count = bitmap_data->bitmap_data.count;
                auto extra_flags = *reinterpret_cast<std::uint32_t *>(&shader_data->extra_flags);
                
                if(shader_data->extra_flags.numeric_countdown_timer) {
                    bitmap_data_index = get_numeric_countdown_timer(transparent_geometry_group->bitmap_index);
                }
                else {
                    int numeric_count = shader_data->numeric_counter_limit;
                    short index = (bitmap_count != 8) ? 0 : 3;
                    float res = floor(numeric_count * transparent_geometry_group->field_0x74[1][index] + 0.5f);
                    if(round(res) < 0) {
                        numeric_count = 0;
                    }
                    else if(round(res) <= numeric_count) {
                        numeric_count = round(res);
                    }

                    for(int i = transparent_geometry_group->bitmap_index; i > 0; i--) {
                        numeric_count = numeric_count / bitmap_count;
                    }
                    bitmap_data_index = numeric_count % bitmap_count;
                }
            }

            float vertex_shader_constants[8 * 4];

            int map_index = 0;
            for(int i = 0; i < 4; i++) {
                map_index = i;
                if(shader_data->maps.count > map_index) {
                    float first_map_type = shader_data->first_map_type;
                    auto *map = shader_data->maps.elements + map_index;
                    BitmapDataType bitmap_data_type;
                    if(map_index == 0) {
                        bitmap_data_type = reinterpret_cast<BitmapDataType *>(0x005fc9d0)[shader_data->first_map_type];
                    }
                    else {
                        bitmap_data_type = BitmapDataType::BITMAP_DATA_TYPE_2D_TEXTURE;
                    }

                    set_bitmap_data_texture(map_index, bitmap_data_type, bitmap_data_index, map->map.tag_handle);

                    D3DTEXTUREADDRESS u_texture_mode;
                    D3DTEXTUREADDRESS v_texture_mode;
                    D3DTEXTUREADDRESS w_texture_mode;
                    D3DTEXTUREADDRESS alternative_mode = reinterpret_cast<D3DTEXTUREADDRESS *>(0x005fc9d8)[shader_data->first_map_type];
                    if(bitmap_data_type == BitmapDataType::BITMAP_DATA_TYPE_2D_TEXTURE && map->flags.u_clamped) {
                        u_texture_mode = D3DTADDRESS_CLAMP;
                        if(map->flags.v_clamped) {
                            v_texture_mode = D3DTADDRESS_CLAMP;
                        }
                        else {
                            if(map_index == 0) {
                                v_texture_mode = alternative_mode;
                            }
                            else {
                                v_texture_mode = D3DTADDRESS_WRAP;
                            }
                        }
                    } 
                    else {
                        if(map_index == 0) {
                            u_texture_mode = alternative_mode;
                        }
                        else {
                            u_texture_mode = D3DTADDRESS_WRAP;
                        }

                        if(bitmap_data_type == BitmapDataType::BITMAP_DATA_TYPE_2D_TEXTURE) {
                            if(map->flags.v_clamped) {
                                v_texture_mode = D3DTADDRESS_CLAMP;
                            }
                            else {
                                if(map_index == 0) {
                                    v_texture_mode = alternative_mode;
                                }
                                else {
                                    v_texture_mode = D3DTADDRESS_WRAP;
                                }
                            }
                        }
                        else {
                            if(map_index == 0) {
                                v_texture_mode = alternative_mode;
                            }
                            else {
                                v_texture_mode = D3DTADDRESS_WRAP;
                            }
                        }
                    }
                    if(map_index == 0) {
                        w_texture_mode = alternative_mode;
                    }
                    else {
                        w_texture_mode = D3DTADDRESS_WRAP;
                    }

                    device->SetSamplerState(map_index, D3DSAMP_ADDRESSU, u_texture_mode);
                    device->SetSamplerState(map_index, D3DSAMP_ADDRESSV, v_texture_mode);
                    device->SetSamplerState(map_index, D3DSAMP_ADDRESSW, w_texture_mode);
                    device->SetSamplerState(map_index, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
                    device->SetSamplerState(map_index, D3DSAMP_MINFILTER, map->flags.unfiltered ? D3DTEXF_POINT : D3DTEXF_LINEAR);
                    device->SetSamplerState(map_index, D3DSAMP_MIPFILTER, map->flags.unfiltered ? D3DTEXF_POINT : D3DTEXF_LINEAR);
                }

                auto maps_count = shader_data->maps.count;
                if(map_index < maps_count) {
                    if(map_index == 0 && shader_data->first_map_type != SHADER_FIRST_MAP_TYPE_2D_MAP) {
                        if(maps_count <= map_index || shader_data->shader_transparent_chicago_flags.first_map_is_in_screenspace == 0) {
                            vertex_shader_constants[map_index * 8 + 0] = 1.0;
                            vertex_shader_constants[map_index * 8 + 1] = 0.0;
                            vertex_shader_constants[map_index * 8 + 2] = 0.0;
                            vertex_shader_constants[map_index * 8 + 3] = 0.0;
                            vertex_shader_constants[map_index * 8 + 4] = 0.0;
                            vertex_shader_constants[map_index * 8 + 5] = 1.0;
                            vertex_shader_constants[map_index * 8 + 6] = 0.0;
                            vertex_shader_constants[map_index * 8 + 7] = 0.0;
                        } 
                        else {
                            vertex_shader_constants[map_index * 8 + 0] = *reinterpret_cast<float *>(0x0075c624);
                            vertex_shader_constants[map_index * 8 + 1] = *reinterpret_cast<float *>(0x0075c628);
                            vertex_shader_constants[map_index * 8 + 2] = *reinterpret_cast<float *>(0x0075c62c);
                            vertex_shader_constants[map_index * 8 + 3] = 0.0;
                            vertex_shader_constants[map_index * 8 + 4] = *reinterpret_cast<float *>(0x0075c630);
                            vertex_shader_constants[map_index * 8 + 5] = *reinterpret_cast<float *>(0x0075c634);
                            vertex_shader_constants[map_index * 8 + 6] = *reinterpret_cast<float *>(0x0075c638);
                            vertex_shader_constants[map_index * 8 + 7] = 0.0;
                        }
                    }
                    else {
                        auto *map_elements = shader_data->maps.elements;
                        auto map_v_scale = map_elements[map_index].map_v_scale;
                        auto map_u_scale = map_elements[map_index].map_u_scale;

                        if(map_index == 0) {
                            if(shader_data->shader_transparent_chicago_flags.scale_first_map_with_distance) {
                                map_v_scale = map_v_scale * transparent_geometry_group->distance * -1;
                                map_u_scale = map_u_scale * transparent_geometry_group->distance * -1;
                            }

                            if(!shader_data->shader_transparent_chicago_flags.first_map_is_in_screenspace) {
                                map_v_scale = map_v_scale * transparent_geometry_group->map_v_scale;
                                map_u_scale = map_u_scale * transparent_geometry_group->map_u_scale;
                            }
                        }
                        else {
                            if(map_index >= 1 || shader_data->shader_transparent_chicago_flags.first_map_is_in_screenspace) {
                                map_v_scale = map_v_scale * transparent_geometry_group->map_v_scale;
                                map_u_scale = map_u_scale * transparent_geometry_group->map_u_scale;
                            }
                        }

                        auto &map = map_elements[map_index];
                        auto *animation_data = reinterpret_cast<void *>(&map.u_animation_source);
                        auto map_u_offset = map.map_u_offset;
                        auto map_v_offset = map.map_v_offset;
                        auto map_rotation = map.map_rotation;
                        float seconds_elapsed_since_startup = *reinterpret_cast<double *>(get_frame_paramaters());
                        apply_shader_texture_animation(animation_data, map_u_scale, map_v_scale, map_u_offset, map_v_offset, map_rotation, seconds_elapsed_since_startup, transparent_geometry_group->field_0x74, &vertex_shader_constants[map_index * 8 + 0], &vertex_shader_constants[map_index * 8 + 4]);
                    }
                }
                else {
                    vertex_shader_constants[map_index * 8 + 0] = 1.0;
                    vertex_shader_constants[map_index * 8 + 1] = 0.0;
                    vertex_shader_constants[map_index * 8 + 2] = 0.0;
                    vertex_shader_constants[map_index * 8 + 3] = 0.0;
                    vertex_shader_constants[map_index * 8 + 4] = 0.0;
                    vertex_shader_constants[map_index * 8 + 5] = 1.0;
                    vertex_shader_constants[map_index * 8 + 6] = 0.0;
                    vertex_shader_constants[map_index * 8 + 7] = 0.0;
                }
            }

            auto res = device->SetVertexShaderConstantF(13, vertex_shader_constants, 8);
            if(res == D3D_OK) {
                prepare_shader_transparent_chicago(reinterpret_cast<ShaderTransparentChicago *>(transparent_geometry_group->shader_tag_data));
            }

            auto maps_count = shader_data->maps.count;
            if(transparent_geometry_group->flags & 0x10 != 0 && shader_data->framebuffer_blend_function == FRAMEBUFFER_BLEND_FUNCTION_ALPHA_BLEND) {
                device->SetTexture(maps_count, nullptr);
                device->SetTextureStageState(maps_count, D3DTSS_COLOROP, D3DTOP_DISABLE);
                device->SetTextureStageState(maps_count, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
                render_transparent_geometry_group_vertices(transparent_geometry_group, false);
                device->SetRenderState(D3DRS_BLENDOP, D3DTOP_DISABLE);
                return;
            }

            float vertex_shader_constants_2[3 * 4];
            vertex_shader_constants_2[0] = 0.0f;
            vertex_shader_constants_2[1] = 0.0f;
            vertex_shader_constants_2[2] = 0.0f;
            vertex_shader_constants_2[3] = 0.0f;
            vertex_shader_constants_2[4] = 0.0f;
            vertex_shader_constants_2[5] = 0.0f;
            vertex_shader_constants_2[6] = 0.0f;
            vertex_shader_constants_2[7] = 0.0f;
            vertex_shader_constants_2[8] = 0.0f;
            vertex_shader_constants_2[9] = 0.0f;
            vertex_shader_constants_2[10] = 1.0f;
            vertex_shader_constants_2[11] = 0.0f;

            auto framebuffer_fade_source = shader_data->framebuffer_fade_source;
            if(transparent_geometry_group->field_0x14 == 1 && shader_data->extra_flags.dont_fade_active_camouflage == 0) {
                vertex_shader_constants_2[10] = 1.0f - transparent_geometry_group->field_0x18;
                if(vertex_shader_constants_2[10] < 0.0f == std::isnan(vertex_shader_constants_2[10])) {
                    if(vertex_shader_constants_2[10] > 1.0f) {
                        vertex_shader_constants_2[10] = 1.0f;
                    }
                }
                else {
                    vertex_shader_constants_2[10] = 0.0f;
                }
            }

            if(framebuffer_fade_source > 0 && transparent_geometry_group->field_0x74 != nullptr && transparent_geometry_group->field_0x74[1] != 0) {
                auto unknown_val = *reinterpret_cast<float *>(transparent_geometry_group->field_0x74[1] - 4 + framebuffer_fade_source * 4);
                if(std::isnan(unknown_val) != unknown_val == 0.0f && *reinterpret_cast<std::uint32_t *>(0x0075c4ec) < 0xFFFF0101) {
                    return;
                }
                vertex_shader_constants_2[10] = vertex_shader_constants_2[10] * unknown_val;
            }

            device->SetVertexShaderConstantF(10, vertex_shader_constants_2, 3);

            int tss_option_argument;
            if(framebuffer_fade_source == 0) {
                tss_option_argument = D3DTA_ALPHAREPLICATE;
            }
            else if(framebuffer_fade_source == 1) {
                tss_option_argument = D3DTA_ALPHAREPLICATE | D3DTA_SPECULAR;
            }
            else {
                tss_option_argument = D3DTA_SPECULAR;
            }

            auto *unknown_val = reinterpret_cast<std::uint32_t *>(0x0075c4b8);
            switch(shader_data->framebuffer_blend_function) {
                case FRAMEBUFFER_BLEND_FUNCTION_ALPHA_BLEND: {
                    if(*unknown_val == 2 && shader_data->maps.count > 1) {
                        map_index = shader_data->maps.count - 1;
                        if(map_index < 2) {
                            map_index = 1;
                        }
                        device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTA_TFACTOR);
                        device->SetTextureStageState(0, D3DTSS_ALPHAARG2, tss_option_argument);
                        break;
                    }
                    map_index = shader_data->maps.count;
                    device->SetTextureStageState(map_index, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
                    device->SetTextureStageState(map_index, D3DTSS_COLORARG1, D3DTA_CURRENT);
                    device->SetTextureStageState(map_index, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
                    device->SetTextureStageState(map_index, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
                    device->SetTextureStageState(map_index, D3DTSS_ALPHAARG2, tss_option_argument);
                    break;
                }

                case FRAMEBUFFER_BLEND_FUNCTION_MULTIPLY:
                case FRAMEBUFFER_BLEND_FUNCTION_COMPONENT_MIN: {
                    if(*unknown_val < 3) {
                        map_index = shader_data->maps.count - 1;
                        if(map_index < 2) {
                            map_index = 1;
                        }
                    }
                    else {
                        map_index = shader_data->maps.count;
                    }

                    device->SetTextureStageState(map_index, D3DTSS_COLOROP, D3DTOP_MULTIPLYADD);
                    device->SetTextureStageState(map_index, D3DTSS_COLORARG1, tss_option_argument | D3DTOP_BLENDCURRENTALPHA);
                    device->SetTextureStageState(map_index, D3DTSS_COLORARG2, D3DTA_CURRENT);
                    device->SetTextureStageState(map_index, D3DTSS_COLORARG0, tss_option_argument);
                    device->SetTextureStageState(map_index, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
                    device->SetTextureStageState(map_index, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
                    break;
                }

                case FRAMEBUFFER_BLEND_FUNCTION_DOUBLE_MULTIPLY: {
                    if(*unknown_val < 3) {
                        map_index = shader_data->maps.count - 1;
                        if(map_index < 2) {
                            map_index = 1;
                        }
                    }
                    else {
                        map_index = shader_data->maps.count;
                    }

                    device->SetRenderState(D3DRS_TEXTUREFACTOR, 0x7f7f7f7f);
                    device->SetTextureStageState(map_index, D3DTSS_COLOROP, D3DTOP_LERP);
                    device->SetTextureStageState(map_index, D3DTSS_COLORARG1, tss_option_argument);
                    device->SetTextureStageState(map_index, D3DTSS_COLORARG2, D3DTA_CURRENT);
                    device->SetTextureStageState(map_index, D3DTSS_COLORARG0, D3DTA_TFACTOR);
                    device->SetTextureStageState(map_index, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
                    device->SetTextureStageState(map_index, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
                    break;
                }

                case FRAMEBUFFER_BLEND_FUNCTION_ADD:
                case FRAMEBUFFER_BLEND_FUNCTION_SUBTRACT:
                case FRAMEBUFFER_BLEND_FUNCTION_COMPONENT_MAX: {
                    if(*unknown_val != 2 || shader_data->maps.count < 2) {
                        map_index = shader_data->maps.count;
                        device->SetTextureStageState(map_index, D3DTSS_COLOROP, D3DTOP_MODULATE);
                        device->SetTextureStageState(map_index, D3DTSS_COLORARG1, D3DTA_CURRENT);
                        device->SetTextureStageState(map_index, D3DTSS_COLORARG2, tss_option_argument);
                        device->SetTextureStageState(map_index, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
                        device->SetTextureStageState(map_index, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
                        break;
                    }

                    auto stage = shader_data->maps.count - 1;
                    if(stage < 2) {
                        stage = 1;
                    }
                    device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
                    device->SetTextureStageState(0, D3DTSS_COLORARG2, tss_option_argument);
                    break;
                }

                case FRAMEBUFFER_BLEND_FUNCTION_ALPHA_MULTIPLY_ADD: {
                    if(*unknown_val == 2 && shader_data->maps.count > 1) {
                        map_index = shader_data->maps.count - 1;
                        if(map_index < 2) {
                            map_index = 1;
                        }
                        device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
                        device->SetTextureStageState(0, D3DTSS_COLORARG2, tss_option_argument);
                        device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
                    }
                    else {
                        map_index = shader_data->maps.count;
                        device->SetTextureStageState(map_index, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
                        device->SetTextureStageState(map_index, D3DTSS_COLORARG1, D3DTA_CURRENT);
                        device->SetTextureStageState(map_index, D3DTSS_COLORARG2, tss_option_argument);
                        device->SetTextureStageState(map_index, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
                        device->SetTextureStageState(map_index, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
                    }
                    device->SetTextureStageState(0, D3DTSS_ALPHAARG2, tss_option_argument);
                    break;
                }

                default: {
                    device->SetTexture(map_index, nullptr);
                    device->SetTextureStageState(map_index, D3DTSS_COLOROP, D3DTA_CURRENT);
                    device->SetTextureStageState(map_index, D3DTSS_ALPHAOP, D3DTA_CURRENT);
                    render_transparent_geometry_group_vertices(transparent_geometry_group, false);
                    device->SetRenderState(D3DRS_BLENDOP, D3DSHADE_FLAT);
                    return;
                }
            }

            map_index = map_index + 1;
            device->SetTexture(map_index, nullptr);
            device->SetTextureStageState(map_index, D3DTSS_COLOROP, D3DTA_CURRENT);
            device->SetTextureStageState(map_index, D3DTSS_ALPHAOP, D3DTA_CURRENT);
            render_transparent_geometry_group_vertices(transparent_geometry_group, false);
            device->SetRenderState(D3DRS_BLENDOP, D3DSHADE_FLAT);
            return;
        }
    }

    static bool switch_command(int arg_count, const char **args) {
        if(arg_count == 1) {
            bool new_setting = STR_TO_BOOL(args[0]);
            use_shader_transparent_chicago_reimplementation = new_setting;
        }
        logger.info("use_shader_transparent_chicago_reimplementation: {}", use_shader_transparent_chicago_reimplementation);
        Engine::console_printf("use_shader_transparent_chicago_reimplementation: %s", BOOL_TO_STR(use_shader_transparent_chicago_reimplementation));
        return true;
    }

    void set_up_shader_transparent_chicago_impl() {
        static auto *sig = Memory::get_signature("draw_shader_transparent_chicago_function_call");
        if(!sig) {
            throw std::runtime_error("Failed to set up shader transparent chicago reimplementation: signature not found");
        }
        
        try {
            std::function<bool()> function_hook = draw_shader_transparent_chicago_hook;
            Memory::hook_function(sig->data(), function_hook);   
        } 
        catch(std::runtime_error &e) {
            throw std::runtime_error("Failed to set up shader transparent chicago reimplementation: " + std::string(e.what()));
        }

        // Set up switch command
        register_command("use_shader_transparent_chicago_reimplementation", "features", "Use the reimplementation of the shader transparent chicago", std::nullopt, switch_command, true, 0, 1);
    }
}
