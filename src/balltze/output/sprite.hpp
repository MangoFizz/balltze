// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__OUTPUT__DRAW_SPRITE_HPP
#define BALLTZE__OUTPUT__DRAW_SPRITE_HPP

#include <string>
#include <map>
#include <d3d9.h>
#include "../logger.hpp"

namespace Balltze {
    #define ASSERT_D3D(x, msg) if(x != D3D_OK) { logger.error("D3D9 assertion failed: {}", std::string(msg)); return; }
    #define ASSERT_D3D_EX(x, msg, ex) if(x != D3D_OK) { logger.error("D3D9 assertion failed: {}", std::string(msg)); ex; }

    struct Vertex {
        float x, y, z, rhw;
        D3DCOLOR color;
        float u, v;
    };

    /**
     * Basic sprite class.
     */
    class Sprite {
    private:
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
}

#endif
