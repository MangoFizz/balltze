// SPDX-License-Identifier: GPL-3.0-only

#include <map>
#include <windows.h>
#include <balltze/helpers/d3d9.hpp>
#include <balltze/math.hpp>
#include <balltze/helpers/resources.hpp>
#include "../resources.hpp"
#include "../logger.hpp"

using namespace Gdiplus;

namespace Balltze {
    #define ASSERT_D3D(x, msg) if(x != D3D_OK) { logger.error("D3D9 assertion failed: {}", std::string(msg)); return; }
    #define ASSERT_D3D_EX(x, msg, ex) if(x != D3D_OK) { logger.error("D3D9 assertion failed: {}", std::string(msg)); ex; }

    static HRESULT load_sprite_shader(IDirect3DDevice9 *device, IDirect3DPixelShader9 **shader) {
        static auto shader_data = load_resource_data(get_current_module(), MAKEINTRESOURCEW(ID_SPRITE_SHADER), L"CSO");
        if(!shader_data) {
            return E_FAIL;
        }
        HRESULT hr = device->CreatePixelShader(reinterpret_cast<const DWORD *>(shader_data->data()), shader);
        return hr;
    }

    IDirect3DDevice9 *Sprite::device() {
        IDirect3DDevice9 *device = nullptr;
        if(m_texture) {
            m_texture->GetDevice(&device);
        }
        return device;
    }

    /**
     * Reference: https://learn.microsoft.com/en-us/windows/win32/direct3d9/id3dxsprite--begin#remarks
     */
    void Sprite::begin() {
        if(m_is_sprite_drawn) {
            throw std::logic_error("Duplicated call to Sprite::begin()");
        }

        auto *device = this->device();
        if(!device) {
            throw std::runtime_error("Failed to get device from texture!");
        }

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

        device->GetPixelShader(&m_old_pixel_shader);
        device->SetPixelShader(m_pixel_shader);

        m_is_sprite_drawn = true;
    }

    void Sprite::end() {
        if(!m_is_sprite_drawn) {
            throw std::logic_error("Call to Sprite::end() without Sprite::begin()");
        }

        auto *device = this->device();
        if(!device) {
            throw std::runtime_error("Failed to get device from texture!");
        }

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

        device->SetPixelShader(m_old_pixel_shader);

        m_is_sprite_drawn = false;
    }

    static Engine::Point2D rotate_point(const Engine::Point2D &point, const Engine::Point2D &center, float angle) {
        Engine::Point2D rotated_point;
        double rads = angle * PI / 180.0;
        rotated_point.x = center.x + (point.x - center.x) * cos(rads) - (point.y - center.y) * sin(rads);
        rotated_point.y = center.y + (point.x - center.x) * sin(rads) + (point.y - center.y) * cos(rads);
        return rotated_point;
    }

    bool Sprite::draw(const Engine::Point2D *position, const Engine::Point2D *center, float angle, const Engine::Vector2D *scale, const Engine::ColorARGBInt *color) {
        if(!m_is_sprite_drawn) {
            throw std::logic_error("Call to Sprite::draw() without Sprite::begin()");
        }

        auto *device = this->device();
        if(!device) {
            throw std::runtime_error("Failed to get device from texture!");
        }

        Engine::Rectangle2DF dest_rect;
        D3DSURFACE_DESC desc;
        m_texture->GetLevelDesc(0, &desc);

        auto x = position->x;
        auto y = position->y;
        auto width = desc.Width * scale->i;
        auto height = desc.Height * scale->j;

        Engine::Point2D top_left = { x, y };
        Engine::Point2D top_right = { x + width, y };
        Engine::Point2D bottom_left = { x, y + height };
        Engine::Point2D bottom_right = { x + width, y + height };

        Engine::Point2D final_center = { x, y };
        if(center) {
            final_center = *center;
        }

        if(angle) {
            top_left = rotate_point(top_left, final_center, angle);
            top_right = rotate_point(top_right, final_center, angle);
            bottom_left = rotate_point(bottom_left, final_center, angle);
            bottom_right = rotate_point(bottom_right, final_center, angle);
        }

        Vertex vertices[4] = {
            { top_left.x, top_left.y, 0.0f, 1.0f, D3DCOLOR_ARGB(255, 255, 255, 255), 0.0f, 0.0f },
            { top_right.x, top_right.y, 0.0f, 1.0f, D3DCOLOR_ARGB(255, 255, 255, 255), 1.0f, 0.0f },
            { bottom_left.x, bottom_left.y, 0.0f, 1.0f, D3DCOLOR_ARGB(255, 255, 255, 255), 0.0f, 1.0f },
            { bottom_right.x, bottom_right.y, 0.0f, 1.0f, D3DCOLOR_ARGB(255, 255, 255, 255), 1.0f, 1.0f }
        };

        IDirect3DPixelShader9 *current_pixel_shader = nullptr;
        device->GetPixelShader(&current_pixel_shader);
        if(current_pixel_shader && current_pixel_shader == m_pixel_shader) {
            if(color) {
                float c_color_mask[4] = {
                    static_cast<float>(color->red) / 255.0f,
                    static_cast<float>(color->green) / 255.0f,
                    static_cast<float>(color->blue) / 255.0f,
                    static_cast<float>(color->alpha) / 255.0f
                };
                ASSERT_D3D_EX(device->SetPixelShaderConstantF(0, c_color_mask, 1), "Failed to set pixel shader constant", return false);
            }
            else {
                float c_color_mask[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
                ASSERT_D3D_EX(device->SetPixelShaderConstantF(0, c_color_mask, 1), "Failed to set pixel shader constant", return false);
            }
        }

        ASSERT_D3D_EX(device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1), "Failed to set FVF", return false);
        ASSERT_D3D_EX(device->SetTexture(0, m_texture), "Failed to set texture", return false);
        ASSERT_D3D_EX(device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(Vertex)), "Failed to draw primitive", return false);

        return true;
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

        IDirect3DPixelShader9 *current_pixel_shader = nullptr;
        device->GetPixelShader(&current_pixel_shader);
        if(current_pixel_shader && current_pixel_shader == m_pixel_shader) {
            float c_color_mask[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
            ASSERT_D3D_EX(device->SetPixelShaderConstantF(0, c_color_mask, 1), "Failed to set pixel shader constant", return false);
        }

        ASSERT_D3D_EX(device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1), "Failed to set FVF", return false);
        ASSERT_D3D_EX(device->SetTexture(0, m_texture), "Failed to set texture", return false);
        ASSERT_D3D_EX(device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(Vertex)), "Failed to draw primitive", return false);

        return true;
    }

    void Sprite::update_texture(IDirect3DTexture9 *texture) {
        m_texture = texture;
        if(m_texture && !m_pixel_shader) {
            load_sprite_shader(device(), &m_pixel_shader);
        }
    }

    Sprite::Sprite(IDirect3DTexture9 *texture) {
        update_texture(texture);
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

    void MultiplyMatrix(const D3DMatrix& a, const D3DMatrix& b, D3DMatrix& result) {
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                result.m[row][col] = a.m[row][0] * b.m[0][col] +
                                    a.m[row][1] * b.m[1][col] +
                                    a.m[row][2] * b.m[2][col] +
                                    a.m[row][3] * b.m[3][col];
            }
        }
    }

    void CreateTranslationMatrix(D3DMatrix& matrix, float x, float y, float z) {
        matrix = D3DMatrix{
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            x, y, z, 1.0f
        };
    }

    void CreateScalingMatrix(D3DMatrix& matrix, float scale_x, float scale_y, float scale_z) {
        matrix = D3DMatrix{
            scale_x, 0.0f, 0.0f, 0.0f,
            0.0f, scale_y, 0.0f, 0.0f,
            0.0f, 0.0f, scale_z, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
    }
}
