// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__HELPERS__DIRECT3D_HPP
#define BALLTZE_API__HELPERS__DIRECT3D_HPP

#include <string>
#include <map>
#include <d3d9.h>
#include "../engine/data_types.hpp"
#include "../api.hpp"

namespace Balltze {
    struct Vertex {
        float x, y, z, rhw;
        D3DCOLOR color;
        float u, v;
    };

    struct D3DMatrix {
        float m[4][4];
    };

    /**
     * Basic sprite class.
     */
    class BALLTZE_API Sprite {
    protected:
        IDirect3DTexture9 *m_texture;
        std::map<D3DRENDERSTATETYPE, DWORD> m_old_render_state;
        std::map<D3DTEXTURESTAGESTATETYPE, DWORD> m_old_texture_stage_0_state;
        std::map<D3DTEXTURESTAGESTATETYPE, DWORD> m_old_texture_stage_1_state;
        std::map<D3DSAMPLERSTATETYPE, DWORD> m_old_sampler_stage_0_state;
        IDirect3DPixelShader9 *m_pixel_shader = nullptr;
        IDirect3DPixelShader9 *m_old_pixel_shader = nullptr;
        bool m_is_sprite_drawn = false;

        IDirect3DDevice9 *device();

    public:
        void begin();
        void end();
        bool draw(const Engine::Rectangle2DF *source_rect, const Engine::Point2D *center, const Engine::Point2D *position, const Engine::Vector2D *scale, const Engine::ColorARGBInt *color);
        bool draw(float pos_x, float pos_y, float width, float height);
        void update_texture(IDirect3DTexture9 *texture);
        Sprite(IDirect3DTexture9 *texture);
        Sprite() = default;
        ~Sprite() = default;
    };

    BALLTZE_API HRESULT load_texture_from_file(const wchar_t *filename, IDirect3DDevice9 *device, IDirect3DTexture9 **texture);
    BALLTZE_API HRESULT load_texture_from_resource(const wchar_t *resource_name, HMODULE module, IDirect3DDevice9 *device, IDirect3DTexture9 **texture);

    BALLTZE_API void MultiplyMatrix(const D3DMatrix& a, const D3DMatrix& b, D3DMatrix& result);
    BALLTZE_API void CreateTranslationMatrix(D3DMatrix& matrix, float x, float y, float z);
    BALLTZE_API void CreateScalingMatrix(D3DMatrix& matrix, float scale_x, float scale_y, float scale_z);
}

#endif
