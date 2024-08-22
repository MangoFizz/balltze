// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__ENGINE__RASTERIZER_HPP
#define BALLTZE_API__ENGINE__RASTERIZER_HPP

#include <d3d9.h>
#include "tag_definitions/bitmap.hpp"
#include "tag_definitions/shader.hpp"
#include "tag_definitions/shader_transparent_chicago.hpp"
#include "tag_definitions/hud_interface_types.hpp"
#include "../api.hpp"

namespace Balltze::Engine {
    struct WindowGlobals {
        /** A handle to the application instance. */
        HINSTANCE hInstance;

        /** Halo's main window handle. */
        HWND hWnd;

        /** possibly used for error dialog menus? */
        HWND hWndUnknown;

        /** 
         * wShow from GetStartupInfo(), if the STARTF_USESHOWWINDOW flag is set. 
         * Otherwise, takes on the value `SW_SHOWDEFAULT`.
         */ 
        int nCmdShow;

        /** The WindowProc callback function as registered with the window class. */
        WNDPROC lpfnWndProc;
        
        /** Halo's small icon resource. */
        HICON hIconSm;
    };
    static_assert(sizeof(WindowGlobals) == 0x18);

    struct Resolution {
        std::uint16_t height;
        std::uint16_t width;
    };

    /**
     * Pixel shader struct.
     */
    struct PixelShader {
        /** Pointer to pixel shader interface */
        IDirect3DPixelShader9 *shader;
        
        PADDING(8);

        /** Name of the pixel shader. */
        const char *name;
    };
    static_assert(sizeof(PixelShader) == 0x10);

    /** 
     * Vertex shader struct.
    */
    struct VertexShader {
        /** Pointer to vertex shader interface */
        IDirect3DVertexShader9* shader;

        /** Filepath of the vertex shader. This is only used on Sapien. */
        const char *filepath;
    };
    static_assert(sizeof(VertexShader) == 0x8);

    /** Number of vertex shaders */
    constexpr const std::uint8_t NUMBER_OF_VERTEX_SHADERS = 64;

    enum VertexShaderIndex {
        VERTEX_SHADER_TYPE_CONVOLUTION = 0,
        VERTEX_SHADER_TYPE_DEBUG,
        VERTEX_SHADER_TYPE_DECAL,
        VERTEX_SHADER_TYPE_DETAIL_OBJECT_TYPE0,
        VERTEX_SHADER_TYPE_DETAIL_OBJECT_TYPE1,
        VERTEX_SHADER_TYPE_EFFECT,
        VERTEX_SHADER_TYPE_EFFECT_MULTITEXTURE,
        VERTEX_SHADER_TYPE_EFFECT_MULTITEXTURE_SCREENSPACE,
        VERTEX_SHADER_TYPE_EFFECT_ZSPRITE,
        VERTEX_SHADER_TYPE_ENVIRONMENT_DIFFUSE_LIGHT,
        VERTEX_SHADER_TYPE_ENVIRONMENT_DIFFUSE_LIGHT_FF,
        VERTEX_SHADER_TYPE_ENVIRONMENT_FOG,
        VERTEX_SHADER_TYPE_ENVIRONMENT_FOG_SCREEN,
        VERTEX_SHADER_TYPE_ENVIRONMENT_LIGHTMAP,
        VERTEX_SHADER_TYPE_ENVIRONMENT_REFLECTION_BUMPED,
        VERTEX_SHADER_TYPE_ENVIRONMENT_REFLECTION_FLAT,
        VERTEX_SHADER_TYPE_ENVIRONMENT_REFLECTION_LIGHTMAP_MASK,
        VERTEX_SHADER_TYPE_ENVIRONMENT_REFLECTION_MIRROR,
        VERTEX_SHADER_TYPE_ENVIRONMENT_REFLECTION_RADIOSITY,
        VERTEX_SHADER_TYPE_ENVIRONMENT_SHADOW,
        VERTEX_SHADER_TYPE_ENVIRONMENT_SPECULAR_LIGHT,
        VERTEX_SHADER_TYPE_ENVIRONMENT_SPECULAR_SPOT_LIGHT,
        VERTEX_SHADER_TYPE_ENVIRONMENT_SPECULAR_LIGHTMAP,
        VERTEX_SHADER_TYPE_ENVIRONMENT_TEXTURE,
        VERTEX_SHADER_TYPE_LENS_FLARE,
        VERTEX_SHADER_TYPE_MODEL_FOGGED,
        VERTEX_SHADER_TYPE_MODEL,
        VERTEX_SHADER_TYPE_MODEL_FF,
        VERTEX_SHADER_TYPE_MODEL_FAST,
        VERTEX_SHADER_TYPE_MODEL_SCENERY,
        VERTEX_SHADER_TYPE_MODEL_ACTIVE_CAMOUFLAGE,
        VERTEX_SHADER_TYPE_MODEL_ACTIVE_CAMOUFLAGE_FF,
        VERTEX_SHADER_TYPE_MODEL_FOG_SCREEN,
        VERTEX_SHADER_TYPE_MODEL_SHADOW,
        VERTEX_SHADER_TYPE_MODEL_ZBUFFER,
        VERTEX_SHADER_TYPE_SCREEN,
        VERTEX_SHADER_TYPE_SCREEN2,
        VERTEX_SHADER_TYPE_TRANSPARENT_GENERIC,
        VERTEX_SHADER_TYPE_TRANSPARENT_GENERIC_LIT_M,
        VERTEX_SHADER_TYPE_TRANSPARENT_GENERIC_M,
        VERTEX_SHADER_TYPE_TRANSPARENT_GENERIC_OBJECT_CENTERED,
        VERTEX_SHADER_TYPE_TRANSPARENT_GENERIC_OBJECT_CENTERED_M,
        VERTEX_SHADER_TYPE_TRANSPARENT_GENERIC_REFLECTION,
        VERTEX_SHADER_TYPE_TRANSPARENT_GENERIC_REFLECTION_M,
        VERTEX_SHADER_TYPE_TRANSPARENT_GENERIC_SCREENSPACE,
        VERTEX_SHADER_TYPE_TRANSPARENT_GENERIC_SCREENSPACE_M,
        VERTEX_SHADER_TYPE_TRANSPARENT_GENERIC_VIEWER_CENTERED,
        VERTEX_SHADER_TYPE_TRANSPARENT_GENERIC_VIEWER_CENTERED_M,
        VERTEX_SHADER_TYPE_TRANSPARENT_GLASS_DIFFUSE_LIGHT,
        VERTEX_SHADER_TYPE_TRANSPARENT_GLASS_DIFFUSE_LIGHT_M,
        VERTEX_SHADER_TYPE_TRANSPARENT_GLASS_REFLECTION_BUMPED,
        VERTEX_SHADER_TYPE_TRANSPARENT_GLASS_REFLECTION_BUMPED_M,
        VERTEX_SHADER_TYPE_TRANSPARENT_GLASS_REFLECTION_FLAT,
        VERTEX_SHADER_TYPE_TRANSPARENT_GLASS_REFLECTION_FLAT_M,
        VERTEX_SHADER_TYPE_TRANSPARENT_GLASS_REFLECTION_MIRROR,
        VERTEX_SHADER_TYPE_TRANSPARENT_GLASS_TINT,
        VERTEX_SHADER_TYPE_TRANSPARENT_GLASS_TINT_M,
        VERTEX_SHADER_TYPE_TRANSPARENT_METER,
        VERTEX_SHADER_TYPE_TRANSPARENT_METER_M,
        VERTEX_SHADER_TYPE_TRANSPARENT_PLASMA_M,
        VERTEX_SHADER_TYPE_TRANSPARENT_WATER_OPACITY,
        VERTEX_SHADER_TYPE_TRANSPARENT_WATER_OPACITY_M,
        VERTEX_SHADER_TYPE_TRANSPARENT_WATER_REFLECTION,
        VERTEX_SHADER_TYPE_TRANSPARENT_WATER_REFLECTION_M
    };

    /**
     * Vertex declaration struct.
     */
    struct VertexDeclaration {
        /** Pointer to vertex declaration interface */
        IDirect3DVertexDeclaration9 *declaration;

        /** FVF of the vertex declaration */
        std::uint32_t fvf;

        /** Seems to be another D3DFVF (?) */
        std::uint32_t fvf_2;
    };
    static_assert(sizeof(VertexDeclaration) == 0xC);

    /**
     * This struct represents one of those render targets.
     * 
     * The game holds an array of 9 render targets. The first 
     * one is the backbuffer, the rest are used for various 
     * things depending on the game state.
     */
    struct RenderTarget {
        std::uint32_t width;
        std::uint32_t height;
        D3DFORMAT format;
        IDirect3DSurface9 *surface;
        IUnknown *unknown; // texture?
    };

    struct TransparentGeometryGroup {
        std::uint32_t flags;
        TagHandle bitmap_tag_handle;
        PADDING(4);
        std::byte *shader_tag_data;
        short bitmap_index;
        PADDING(2);
        short field_0x14;
        PADDING(2);
        float field_0x18;
        PADDING(32);
        float map_u_scale;
        float map_v_scale;
        PADDING(16);
        int map_index; 
        short *field65_0x58;
        PADDING(24);
        float **field_0x74;
        float distance;
        PADDING(44);
    };
    static_assert(sizeof(TransparentGeometryGroup) == 0xA8);

    struct ScreenQuad {
        struct Vertex {
            float x, y, z, rhw;
            float u, v;
        };

        Vertex top_left;
        Vertex top_right;
        Vertex bottom_right;
        Vertex bottom_left;
    };

    struct HudInterfaceMeter {
        Point2DInt anchor_offset;
		float width_scale;
		float height_scale;
		TagDefinitions::HUDInterfaceScalingFlags scaling_flags;
		PADDING(2);
		PADDING(20);
		TagDependency meter_bitmap;
		ColorARGBInt color_at_meter_minimum;
		ColorARGBInt color_at_meter_maximum;
		ColorARGBInt flash_color;
		ColorARGBInt empty_color;
		TagDefinitions::HUDInterfaceMeterFlags flags;
		std::int8_t minimum_meter_value;
		Index sequence_index;
		std::int8_t alpha_multiplier;
		std::int8_t alpha_bias;
		std::int16_t value_scale;
		float opacity;
		float translucency;
		float min_alpha;
        PADDING(12);
		ColorARGBInt overcharge_empty_color;
    }; static_assert(sizeof(HudInterfaceMeter) == 0x68);

    /**
     * Get the window globals.
     * @return      Reference to the window globals.
     * @exception   If window globals signature was not found... yet
     */
    BALLTZE_API WindowGlobals *get_window_globals();

    /**
     * Get the current resolution of the game.
     * @return current resolution.
     */
    BALLTZE_API Resolution &get_resolution() noexcept;

    /**
     * Get the pointer to the Direct3D device.
     * @return pointer to D3D9 device
     */
    BALLTZE_API IDirect3DDevice9 *get_d3d9_device() noexcept;

    /**
     * Get a pixel shader by index.
     * @param index index of the pixel shader
     * @return      pointer to the pixel shader
     * @exception   if the index is out of bounds
     */
    BALLTZE_API PixelShader *get_pixel_shader(std::size_t index);

    /**
     * Get a vertex shader by index.
     * @param index index of the vertex shader
     * @return      pointer to the vertex shader, or nullptr if index is out of bounds
     */
    BALLTZE_API VertexShader *get_vertex_shader(std::size_t index);

    /**
     * Get a vertex shader by name.
     * @param name name of the vertex shader
     * @return     pointer to the vertex shader, or nullptr if not found
     */
    BALLTZE_API VertexShader *get_vertex_shader(std::string name);

    /**
     * Get a vertex declaration by index.
     * @param index index of the vertex declaration
     * @return      pointer to the vertex declaration
     * @exception   if the index is out of bounds
     */
    BALLTZE_API VertexDeclaration *get_vertex_declaration(std::size_t index);

    /**
     * Get the vertex shader permutation of a shader.
     * @param shader    shader tag data to get the permutation of
     * @return          permutation index of the shader
     */
    BALLTZE_API std::uint16_t get_vertex_shader_permutation_index(TagDefinitions::Shader *shader);

    /**
     * Get the vertex shader index for a permutation.
     * @param vertex_type               type of the dynamic vertices
     * @param permutation_index         index of the permutation
     * @return                          pointer to vertex shader
     */
    BALLTZE_API VertexShader *get_vertex_shader_index_for_permutation(std::size_t vertex_type, std::size_t permutation_index);

    /**
     * Get the render target at the specified index
     * @param index     index of the render target (0-8)
     * @return          pointer to render target
     * @throws          std::runtime_error if the index is out of bounds
     */
    BALLTZE_API RenderTarget *get_render_target(std::size_t index = 0);

    /**
     * Load a bitmap into the texture cache
     * @param bitmap_data       the bitmap to load
     * @param immediate         whether to load the bitmap immediately (?)
     * @param force_pixels_read whether to force the pixels to be read (?)
     */
    BALLTZE_API IDirect3DTexture9 *load_bitmap_data_texture(TagDefinitions::BitmapData *bitmap_data, bool immediate, bool force_pixels_read);

    /**
     * Set the texture of a bitmap to a stage of the device
     * @param device        device to set the texture on
     * @param stage         stage to set the texture on
     * @param bitmap_data   bitmap to set the texture to
     */
    BALLTZE_API bool set_bitmap_data_texture(std::size_t stage, TagDefinitions::BitmapData *bitmap_data);

    /**
     * Set the texture of a bitmap to a stage of the device
     * @param texture_stage         stage to set the texture on
     * @param bitmap_type           type of the bitmap
     * @param bitmap_data_index     index of the bitmap data
     * @param bitmap_tag_handle     handle of the bitmap tag
     * @return                      whether the texture was set successfully
     */
    BALLTZE_API bool set_bitmap_data_texture(std::uint16_t stage, std::uint16_t bitmap_type, std::uint16_t bitmap_data_index, TagHandle bitmap_tag_handle);

    /** 
     * Apply shader animation transforms
     * @param anim_data
     * @param map_u_scale
     * @param map_v_scale
     * @param map_u_offset
     * @param map_v_offset
     * @param map_rotation
     * @param frame_params
     * @param param_8
     * @param param_9
     * @param param_10
     */
    BALLTZE_API void apply_shader_texture_animation(void *anim_data, float map_u_scale, float map_v_scale, float map_u_offset, float map_v_offset, float map_rotation, float frame_params, float **param_8, float *param_9, float *param_10);

    /**
     * Get the frame parameters
     * @return pointer to frame parameters
     */
    BALLTZE_API void *get_frame_paramaters();

    /**
     * Prepare texture stage to render a given transparent chicago shader
     * @param shader_data   transparent chicago shader data
     * @return              whether the texture stage was prepared successfully
     */
    BALLTZE_API bool prepare_shader_transparent_chicago(TagDefinitions::ShaderTransparentChicago *shader_data);

    /**
     * Set the framebuffer blend function
     * @param blend_function    blend function to set
     */
    BALLTZE_API void set_framebuffer_blend_function(TagDefinitions::FramebufferBlendFunction blend_function);

    /**
     * Get the transparent geometry group vertex type.
     * @param group     transparent geometry group
     * @return          dynamic vertices type
     */
    BALLTZE_API std::int16_t get_transparent_geometry_group_vertex_type(TransparentGeometryGroup *group);

    /**
     * Render the transparent geometry group
     * @param group     transparent geometry group
     * @param param_2   unknown parameter
     */
    BALLTZE_API void render_transparent_geometry_group(TransparentGeometryGroup *group, std::uint32_t *param_2);

    /**
     * Render the transparent geometry group vertices
     * @param group     transparent geometry group
     * @param param_2   unknown parameter
     */
    BALLTZE_API void render_transparent_geometry_group_vertices(TransparentGeometryGroup *group, bool param_2);

    /**
     * Render the user interface
     * @param player_index  the index of the player (0-3); -1 for all players (?)
     */
    BALLTZE_API void render_user_interface_widgets(std::uint16_t player_index);

    /**
     * Render the HUD
     */
    BALLTZE_API void render_player_hud();

    /**
     * Render the post carnage report
     */
    BALLTZE_API void render_netgame_post_carnage_report();
}

#endif
