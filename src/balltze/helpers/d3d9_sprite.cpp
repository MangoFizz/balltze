// SPDX-License-Identifier: GPL-3.0-only

#include <map>
#include <windows.h>
#include <balltze/helpers/d3d9_sprite.hpp>
#include <balltze/helpers/resources.hpp>
#include "../logger.hpp"

using namespace Gdiplus;

namespace Balltze {
    #define ASSERT_D3D(x, msg) if(x != D3D_OK) { logger.error("D3D9 assertion failed: {}", std::string(msg)); return; }
    #define ASSERT_D3D_EX(x, msg, ex) if(x != D3D_OK) { logger.error("D3D9 assertion failed: {}", std::string(msg)); ex; }

    IDirect3DDevice9 *Sprite::device() {
        IDirect3DDevice9 *device = nullptr;
        m_texture->GetDevice(&device);
        return device;
    }

    /**
     * Reference: https://learn.microsoft.com/en-us/windows/win32/direct3d9/id3dxsprite--begin#remarks
     */
    void Sprite::begin() {
        auto *device = this->device();
        D3DCAPS9 caps;
        device->GetDeviceCaps(&caps);

        auto set_render_state = [this, device](D3DRENDERSTATETYPE type, DWORD value) {
            DWORD old_value = 0;
            device->GetRenderState(type, &old_value);
            device->SetRenderState(type, value);
            m_old_render_state.insert_or_assign(type, old_value);
        };

        auto set_texture_stage_state = [this, device](DWORD stage, D3DTEXTURESTAGESTATETYPE type, DWORD value) {
            DWORD old_value = 0;
            device->GetTextureStageState(stage, type, &old_value);
            device->SetTextureStageState(stage, type, value);
            if(stage == 0) {
                m_old_texture_stage_0_state.insert_or_assign(type, old_value);
            }
            else if(stage == 1) {
                m_old_texture_stage_1_state.insert_or_assign(type, old_value);
            }
        };

        auto set_sampler_state = [this, device](DWORD stage, D3DSAMPLERSTATETYPE type, DWORD value) {
            DWORD old_value = 0;
            device->GetSamplerState(stage, type, &old_value);
            device->SetSamplerState(stage, type, value);
            m_old_sampler_stage_0_state.insert_or_assign(type, old_value);
        };

        set_render_state(D3DRS_ALPHABLENDENABLE, true);
        set_render_state(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
        set_render_state(D3DRS_ALPHAREF, 0);
        set_render_state(D3DRS_ALPHATESTENABLE, true);
        set_render_state(D3DRS_BLENDOP, D3DBLENDOP_ADD);
        set_render_state(D3DRS_CLIPPING, true);
        set_render_state(D3DRS_CLIPPLANEENABLE, false);
        set_render_state(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_RED);
        set_render_state(D3DRS_CULLMODE, D3DCULL_NONE);
        set_render_state(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        set_render_state(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
        set_render_state(D3DRS_ENABLEADAPTIVETESSELLATION, FALSE);
        set_render_state(D3DRS_FILLMODE, D3DFILL_SOLID);
        set_render_state(D3DRS_FOGENABLE, false);
        set_render_state(D3DRS_INDEXEDVERTEXBLENDENABLE, false);
        set_render_state(D3DRS_LIGHTING, false);
        set_render_state(D3DRS_RANGEFOGENABLE, false);
        set_render_state(D3DRS_SEPARATEALPHABLENDENABLE, false);
        set_render_state(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
        set_render_state(D3DRS_SPECULARENABLE, false);
        set_render_state(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        set_render_state(D3DRS_SRGBWRITEENABLE, false);
        set_render_state(D3DRS_STENCILENABLE, false);
        set_render_state(D3DRS_VERTEXBLEND, false);
        set_render_state(D3DRS_WRAP0, 0);

        set_texture_stage_state(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
        set_texture_stage_state(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
        set_texture_stage_state(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
        set_texture_stage_state(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
        set_texture_stage_state(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
        set_texture_stage_state(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
        set_texture_stage_state(0, D3DTSS_TEXCOORDINDEX, 0);
        set_texture_stage_state(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
        set_texture_stage_state(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
        set_texture_stage_state(1, D3DTSS_COLOROP, D3DTOP_DISABLE);

        set_sampler_state(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
        set_sampler_state(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
        set_sampler_state(0, D3DSAMP_MAGFILTER, caps.TextureFilterCaps & D3DPTFILTERCAPS_MAGFANISOTROPIC ? D3DTEXF_ANISOTROPIC : D3DTEXF_LINEAR);
        set_sampler_state(0, D3DSAMP_MAXMIPLEVEL, 0);
        set_sampler_state(0, D3DSAMP_MAXANISOTROPY, caps.MaxAnisotropy);
        set_sampler_state(0, D3DSAMP_MINFILTER, caps.TextureFilterCaps & D3DPTFILTERCAPS_MINFANISOTROPIC ? D3DTEXF_ANISOTROPIC : D3DTEXF_LINEAR);
        set_sampler_state(0, D3DSAMP_MIPFILTER, caps.TextureFilterCaps & D3DPTFILTERCAPS_MIPFLINEAR ? D3DTEXF_LINEAR : D3DTEXF_POINT);
        set_sampler_state(0, D3DSAMP_MIPMAPLODBIAS, 0);
        set_sampler_state(0, D3DSAMP_SRGBTEXTURE, 0);
    }

    void Sprite::end() {
        auto *device = this->device();
        for(auto &[type, value] : m_old_render_state) {
            device->SetRenderState(type, value);
        }
        for(auto &[type, value] : m_old_texture_stage_0_state) {
            device->SetTextureStageState(0, type, value);
        }
        for(auto &[type, value] : m_old_texture_stage_1_state) {
            device->SetTextureStageState(1, type, value);
        }
        for(auto &[type, value] : m_old_sampler_stage_0_state) {
            device->SetSamplerState(0, type, value);
        }
    }

    bool Sprite::draw(float pos_x, float pos_y, float width, float height) {
        auto *device = this->device();
        float left = pos_x;
        float right = pos_x + width;
        float top = pos_y;
        float bottom = pos_y + height;

        // Create vertices for our sprite
        Vertex vertices[4] = {
            { left, top, 0.0f, 1.0f, D3DCOLOR_ARGB(255, 255, 255, 255), 0.0f, 0.0f },
            { right, top, 0.0f, 1.0f, D3DCOLOR_ARGB(255, 255, 255, 255), 1.0f, 0.0f },
            { left, bottom, 0.0f, 1.0f, D3DCOLOR_ARGB(255, 255, 255, 255), 0.0f, 1.0f },
            { right, bottom, 0.0f, 1.0f, D3DCOLOR_ARGB(255, 255, 255, 255), 1.0f, 1.0f }
        };

        ASSERT_D3D_EX(device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1), "Failed to set FVF", return false);
        ASSERT_D3D_EX(device->SetTexture(0, m_texture), "Failed to set texture", return false);
        ASSERT_D3D_EX(device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(Vertex)), "Failed to draw primitive", return false);

        return true;
    }

    void Sprite::update_texture(IDirect3DTexture9 *texture) {
        m_texture = texture;
    }

    Sprite::Sprite(IDirect3DTexture9 *texture) : m_texture(texture) {
        m_is_sprite_drawn = false;
    }

    HRESULT load_texture_from_file(const wchar_t *filename, IDirect3DDevice9 *device, IDirect3DTexture9 **texture) {
        GdiplusStartupInput gdip_startup_input;
        ULONG_PTR gdiplus_token;
        GdiplusStartup(&gdiplus_token, &gdip_startup_input, NULL);

        auto *bitmap = Bitmap::FromFile(filename, FALSE);
        if(!bitmap) {
            GdiplusShutdown(gdiplus_token);
            return E_FAIL;
        }

        UINT width = bitmap->GetWidth();
        UINT height = bitmap->GetHeight();

        HRESULT hr = device->CreateTexture(width, height, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, texture, NULL);
        if(FAILED(hr)) {
            delete bitmap;
            GdiplusShutdown(gdiplus_token);
            return hr;
        }

        D3DLOCKED_RECT lockedRect;
        (*texture)->LockRect(0, &lockedRect, NULL, 0);
        BitmapData bitmapData;
        Rect rect(0, 0, width, height);
        bitmap->LockBits(&rect, ImageLockModeRead, PixelFormat32bppARGB, &bitmapData);
        memcpy(lockedRect.pBits, bitmapData.Scan0, width * height * 4);
        bitmap->UnlockBits(&bitmapData);
        (*texture)->UnlockRect(0);

        delete bitmap;
        GdiplusShutdown(gdiplus_token);

        return S_OK;
    }

    

    HRESULT load_texture_from_resource(const wchar_t *resource_name, HMODULE module, IDirect3DDevice9 *device, IDirect3DTexture9 **texture) {
        GdiplusStartupInput gdiplus_startup_input;
        ULONG_PTR gdiplus_token;
        GdiplusStartup(&gdiplus_token, &gdiplus_startup_input, NULL);

        auto bitmap = load_image_from_resource(module, resource_name, L"PNG");
        if(!bitmap) {
            GdiplusShutdown(gdiplus_token);
            logger.error("Failed to load image {} from resource!", reinterpret_cast<int>(resource_name));
            return E_FAIL;
        }

        UINT width = bitmap->GetWidth();
        UINT height = bitmap->GetHeight();

        HRESULT hr = device->CreateTexture(width, height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, texture, NULL);
        if(FAILED(hr)) {
            bitmap.reset();
            GdiplusShutdown(gdiplus_token);
            logger.error("Failed to create texture for image {} from resource!", reinterpret_cast<int>(resource_name));
            return hr;
        }

        D3DLOCKED_RECT locked_rect;
        (*texture)->LockRect(0, &locked_rect, NULL, 0);
        BitmapData bitmap_data;
        Rect rect(0, 0, width, height);
        bitmap->LockBits(&rect, ImageLockModeRead, PixelFormat32bppARGB, &bitmap_data);
        memcpy(locked_rect.pBits, bitmap_data.Scan0, width * height * 4);
        bitmap->UnlockBits(&bitmap_data);
        (*texture)->UnlockRect(0);

        bitmap.reset();
        GdiplusShutdown(gdiplus_token);

        return S_OK;
    }
}
