// SPDX-License-Identifier: GPL-3.0-only

#include <optional>
#include <balltze/engine/rasterizer.hpp>

namespace Balltze::Engine::Rasterizer {
    extern "C" {
        void *load_bitmap_function_address = nullptr;
        IDirect3DTexture9 *load_bitmap_asm(TagDefinitions::BitmapData *bitmap_data, bool immediate, bool force_pixels_read);
        std::uint16_t rasterizer_get_vertex_shader_permutation_index(TagDefinitions::Shader *shader_data);
        std::uint16_t rasterizer_get_transparent_geometry_group_vertex_type(TransparentGeometryGroup *group);
        void rasterizer_render_transparent_geometry_group(TransparentGeometryGroup *group, std::uint32_t *param_2);
        void rasterizer_render_transparent_geometry_group_vertices(bool param_2, TransparentGeometryGroup *group);
        void rasterizer_set_framebuffer_blend_function(std::uint16_t blend_function);
        bool rasterizer_set_bitmap_data_texture(std::uint16_t stage, std::uint16_t bitmap_type, std::uint16_t param_3, std::uint16_t bitmap_data_index, TagHandle bitmap_tag_handle);
        void rasterizer_apply_shader_texture_animation(void *anim_data, float map_u_scale, float map_v_scale, float map_u_offset, float map_v_offset, float map_rotation, float frame_params, float **param_8, float *param_9, float *param_10);
        bool rasterizer_prepare_shader_transparent_chicago(TagDefinitions::ShaderTransparentChicago *shader_data);
    }

    WindowGlobals *get_window_globals() {
        static std::optional<WindowGlobals *> window_globals;
        if(!window_globals.has_value()) {
            auto *window_globals_sig = Memory::get_signature("window_globals");
            if(!window_globals_sig) {
                throw std::runtime_error("window_globals signature not found");
            }
            window_globals = *reinterpret_cast<WindowGlobals **>(window_globals_sig->data());
        }
        return *window_globals;
    }
    
    Resolution &get_resolution() noexcept {
        static auto *resolution_sig = Memory::get_signature("resolution");
        static Resolution *resolution = *reinterpret_cast<Resolution **>(resolution_sig->data());
        return *resolution;
    }

    IDirect3DDevice9 *get_d3d9_device() noexcept {
        static auto *device_sig = Memory::get_signature("d3d9_device_pointer");
        static IDirect3DDevice9 **device = *reinterpret_cast<IDirect3DDevice9 ***>(device_sig->data());
        return *device;
    }

    PixelShader *get_pixel_shader(std::size_t index) {

    }

    VertexShader *get_vertex_shader(std::size_t index) {
        static auto *rasterizer_vertex_shaders_table_address_sig = Memory::get_signature("rasterizer_vertex_shaders_table_address");
        if(!rasterizer_vertex_shaders_table_address_sig) {
            throw std::runtime_error("Could not find signature for rasterizer vertex shaders table address");
        }
        static auto *rasterizer_vertex_shaders_table_address = *reinterpret_cast<VertexShader **>(rasterizer_vertex_shaders_table_address_sig->data());
        if(index >= NUMBER_OF_VERTEX_SHADERS) {
            return nullptr;
        }
        return rasterizer_vertex_shaders_table_address + index;
    }

    VertexShader *get_vertex_shader(std::string name) {
        static auto *rasterizer_vertex_shaders_table_address_sig = Memory::get_signature("rasterizer_vertex_shaders_table_address");
        if(!rasterizer_vertex_shaders_table_address_sig) {
            throw std::runtime_error("Could not find signature for rasterizer vertex shaders table address");
        }
        static auto *rasterizer_vertex_shaders_table_address = *reinterpret_cast<VertexShader **>(rasterizer_vertex_shaders_table_address_sig->data());
        for(std::size_t i = 0; i < NUMBER_OF_VERTEX_SHADERS; i++) {
            if(rasterizer_vertex_shaders_table_address[i].filepath == name) {
                return rasterizer_vertex_shaders_table_address + i;
            }
        }
        return nullptr;
    }

    VertexShader *get_vertex_shader_index_for_permutation(std::size_t vertex_type, std::size_t permutation_index) {
        static auto *rasterizer_vertex_shaders_permutations_table_address_sig = Memory::get_signature("rasterizer_vertex_shaders_permutations_table_address");
        if(!rasterizer_vertex_shaders_permutations_table_address_sig) {
            throw std::runtime_error("Could not find signature for rasterizer vertex shaders permutations table address");
        }
        static auto *vertex_shader_permutations = *reinterpret_cast<short **>(rasterizer_vertex_shaders_permutations_table_address_sig->data());
        return get_vertex_shader(vertex_shader_permutations[permutation_index + vertex_type * 6]);
    }

    VertexDeclaration *get_vertex_declaration(std::size_t index) {
        static auto *rasterizer_vertex_declarations_table_address_sig = Memory::get_signature("rasterizer_vertex_declarations_table_address");
        if(!rasterizer_vertex_declarations_table_address_sig) {
            throw std::runtime_error("Could not find signature for rasterizer vertex declarations table address");
        }
        static auto *rasterizer_vertex_declarations_table_address = *reinterpret_cast<VertexDeclaration **>(rasterizer_vertex_declarations_table_address_sig->data());
        return rasterizer_vertex_declarations_table_address + index;
    }

    std::uint16_t get_vertex_shader_permutation_index(TagDefinitions::Shader *shader_data) {
        return rasterizer_get_vertex_shader_permutation_index(shader_data);        
    }

    IDirect3DTexture9 *load_bitmap_data_texture(TagDefinitions::BitmapData *bitmap_data, bool immediate, bool force_pixels_read) {
        static auto *load_bitmap_sig = Memory::get_signature("load_bitmap_function");
        if(!load_bitmap_sig) {
            throw std::runtime_error("Could not find signature for bitmap load function");
        }
        if(!load_bitmap_function_address) {
            load_bitmap_function_address = reinterpret_cast<void *>(load_bitmap_sig->data());
        }
        return load_bitmap_asm(bitmap_data, immediate, force_pixels_read);
    }

    bool set_bitmap_data_texture(IDirect3DDevice9 *device, std::size_t stage, TagDefinitions::BitmapData *bitmap_data) {
        if(bitmap_data->texture_cache_id == 0xFFFFFFFF) {
            try {
                load_bitmap_data_texture(bitmap_data, true, true);
            }
            catch(...) {
                throw;
            }
        }
        auto res = device->SetTexture(stage, reinterpret_cast<IDirect3DBaseTexture9 *>(bitmap_data->texture));
        if(FAILED(res)) {
            return false;
        }
        return true;
    }

    bool set_bitmap_data_texture(std::uint16_t stage, std::uint16_t bitmap_type, std::uint16_t bitmap_data_index, TagHandle bitmap_tag_handle) {
        return rasterizer_set_bitmap_data_texture(stage, bitmap_type, 0, bitmap_data_index, bitmap_tag_handle);
    }

    RenderTarget *get_render_target(std::size_t index) {
        static auto *render_targets_sig = Memory::get_signature("d3d9_render_targets");
        if(!render_targets_sig) {
            throw std::runtime_error("Could not find signature for render targets");
        }
        if(index > 8) {
            throw std::runtime_error("Index out of bounds");
        }
        return *reinterpret_cast<RenderTarget **>(render_targets_sig->data()) + index;
    }

    std::int16_t get_transparent_geometry_group_vertex_type(TransparentGeometryGroup *group) {
        static auto *sig = Memory::get_signature("rasterizer_vertices_types_table_address");
        if(!sig) {
            throw std::runtime_error("Could not find signature for rasterizer vertices types table address");
        }
        static auto *vertices_types = *reinterpret_cast<short **>(sig->data());
        std::int16_t vertex_declaration_index = -1;
        if(group->field65_0x58 == nullptr) {
            if(group->map_index != -1) {
                vertex_declaration_index = vertices_types[group->map_index * 8];
            }
        }
        else {
            vertex_declaration_index = *group->field65_0x58;
        }
        return vertex_declaration_index;
    }

    void render_transparent_geometry_group(TransparentGeometryGroup *group, std::uint32_t *param_2) {
        rasterizer_render_transparent_geometry_group(group, param_2);
    }

    void render_transparent_geometry_group_vertices(TransparentGeometryGroup *group, bool param_2) {
        rasterizer_render_transparent_geometry_group_vertices(param_2, group);
    }

    void set_framebuffer_blend_function(TagDefinitions::FramebufferBlendFunction blend_function) {
        rasterizer_set_framebuffer_blend_function(static_cast<std::uint16_t>(blend_function));
    }

    void apply_shader_texture_animation(void *anim_data, float map_u_scale, float map_v_scale, float map_u_offset, float map_v_offset, float map_rotation, float frame_params, float **param_8, float *param_9, float *param_10) {
        rasterizer_apply_shader_texture_animation(anim_data, map_u_scale, map_v_scale, map_u_offset, map_v_offset, map_rotation, frame_params, param_8, param_9, param_10);
    }

    bool prepare_shader_transparent_chicago(TagDefinitions::ShaderTransparentChicago *shader_data) {
        return rasterizer_prepare_shader_transparent_chicago(shader_data);
    }

    void *get_frame_paramaters() {
        static auto *frame_parameters_sig = Memory::get_signature("rasterizer_frame_parameters_address");
        if(!frame_parameters_sig) {
            throw std::runtime_error("Could not find signature for frame parameters");
        }
        return *reinterpret_cast<void **>(frame_parameters_sig->data());
    }

    extern "C" {
        void *render_user_interface_function_address = nullptr;
        void render_user_interface_asm(std::uint32_t param_1);
    }

    void render_user_interface_widgets(std::uint16_t player_index) {
        static auto *render_user_interface_function_sig = Memory::get_signature("render_user_interface_function");
        if(!render_user_interface_function_sig) {
            throw std::runtime_error("Could not find signature for UI render function");
        }
        if(!render_user_interface_function_address) {
            render_user_interface_function_address = reinterpret_cast<void *>(render_user_interface_function_sig->data());
        }
        render_user_interface_asm(player_index);
    }

    void render_player_hud() {
        static auto *render_hud_function_sig = Memory::get_signature("render_hud_function");
        if(!render_hud_function_sig) {
            throw std::runtime_error("Could not find signature for HUD render function");
        }
        auto *render_hud_function = reinterpret_cast<void(*)()>(render_hud_function_sig->data());
        render_hud_function();
    }

    void render_netgame_post_carnage_report() {
        static auto *render_post_carnage_report_function_sig = Memory::get_signature("render_post_carnage_report_function");
        if(!render_post_carnage_report_function_sig) {
            throw std::runtime_error("Could not find signature for post carnage report render function");
        }
        auto *render_post_carnage_report_function = reinterpret_cast<void(*)()>(render_post_carnage_report_function_sig->data());
        render_post_carnage_report_function();
    }
}
