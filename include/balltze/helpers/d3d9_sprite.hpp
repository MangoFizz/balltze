// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__HELPERS__D3D9_SPRITE_HPP
#define BALLTZE_API__HELPERS__D3D9_SPRITE_HPP

#include <string>
#include <map>
#include <d3d9.h>
#include "../api.hpp"

namespace Balltze {
    struct Vertex {
        float x, y, z, rhw;
        D3DCOLOR color;
        float u, v;
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
        bool m_is_sprite_drawn;

        IDirect3DDevice9 *device();

    public:
        void begin();
        void end();
        bool draw(float pos_x, float pos_y, float width, float height);
        void update_texture(IDirect3DTexture9 *texture);
        Sprite(IDirect3DTexture9 *texture);
        Sprite() = default;
        ~Sprite() = default;
    };

    BALLTZE_API HRESULT load_texture_from_file(const wchar_t *filename, IDirect3DDevice9 *device, IDirect3DTexture9 **texture);
    BALLTZE_API HRESULT load_texture_from_resource(const wchar_t *resource_name, HMODULE module, IDirect3DDevice9 *device, IDirect3DTexture9 **texture);
}

#endif
