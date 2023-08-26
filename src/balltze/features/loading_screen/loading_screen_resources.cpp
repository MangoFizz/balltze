// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/utils.hpp>
#include "../../logger.hpp"
#include "../../resources.hpp"
#include "loading_screen_resources.hpp"
#include <gdiplus.h>

using namespace Gdiplus;

namespace Balltze::Features {
    HRESULT load_texture_from_file(const wchar_t *filename, IDirect3DDevice9 *device, IDirect3DTexture9 **texture) {
        GdiplusStartupInput gdip_startup_input;
        ULONG_PTR gdiplusToken;
        GdiplusStartup(&gdiplusToken, &gdip_startup_input, NULL);

        auto *bitmap = Bitmap::FromFile(filename, FALSE);
        if(!bitmap) {
            GdiplusShutdown(gdiplusToken);
            return E_FAIL;
        }

        UINT width = bitmap->GetWidth();
        UINT height = bitmap->GetHeight();

        HRESULT hr = device->CreateTexture(width, height, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, texture, NULL);
        if(FAILED(hr)) {
            delete bitmap;
            GdiplusShutdown(gdiplusToken);
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
        GdiplusShutdown(gdiplusToken);

        return S_OK;
    }

    Gdiplus::Bitmap *load_image_from_resource(HMODULE module, const wchar_t *resource_id, const wchar_t *resource_type) {
        IStream *stream = nullptr;
        Gdiplus::Bitmap *bitmap = nullptr;
        HGLOBAL global_handle = nullptr;

        HRSRC resource = FindResourceW(module, resource_id, resource_type);
        if(resource) {
            DWORD resource_size = SizeofResource(module, resource);
            if(resource_size > 0) {
                HGLOBAL resource_global_handle = LoadResource(module, resource);
                if(resource_global_handle) {
                    void *image_bytes = LockResource(resource_global_handle);
                    global_handle = GlobalAlloc(GHND, resource_size);
                    if(global_handle) {
                        void* buffer = GlobalLock(global_handle);
                        if(buffer) {
                            memcpy(buffer, image_bytes, resource_size);
                            HRESULT hr = CreateStreamOnHGlobal(global_handle, TRUE, &stream);
                            if(SUCCEEDED(hr)) {
                                global_handle = nullptr;
                                bitmap = new Gdiplus::Bitmap(stream);
                            }
                        }
                    }
                }
            }
        }

        if(stream) {
            stream->Release();
            stream = nullptr;
        }

        if(global_handle) {
            GlobalFree(global_handle);
            global_handle = nullptr;
        }

        return bitmap;
    }

    HRESULT load_texture_from_resource(const wchar_t *resource_name, HMODULE module, IDirect3DDevice9 *device, IDirect3DTexture9 **texture) {
        GdiplusStartupInput gdiplus_startup_input;
        ULONG_PTR gdiplus_token;
        GdiplusStartup(&gdiplus_token, &gdiplus_startup_input, NULL);

        Bitmap *bitmap = load_image_from_resource(module, resource_name, L"PNG");
        if(!bitmap) {
            GdiplusShutdown(gdiplus_token);
            logger.error("Failed to load image {} from resource!", reinterpret_cast<int>(resource_name));
            return E_FAIL;
        }

        UINT width = bitmap->GetWidth();
        UINT height = bitmap->GetHeight();

        HRESULT hr = device->CreateTexture(width, height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, texture, NULL);
        if(FAILED(hr)) {
            delete bitmap;
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

        // Liberar recursos
        delete bitmap;
        GdiplusShutdown(gdiplus_token);

        return S_OK;
    }

    HRESULT load_loading_screen_background_texture(IDirect3DDevice9 *device, IDirect3DTexture9 **texture) {
        return load_texture_from_resource(MAKEINTRESOURCEW(ID_LOADING_SCREEN_BACKGROUND), get_current_module(), device, texture);
    }

    HRESULT load_loading_screen_shader(IDirect3DDevice9 *device, IDirect3DPixelShader9 **shader) {
        HMODULE current_module = get_current_module();
        HRSRC resource = FindResourceA(current_module, MAKEINTRESOURCEA(ID_LOADING_SCREEN_SHADER), "CSO");
        HGLOBAL resource_global_handle = LoadResource(current_module, resource);
        const void *shader_data = LockResource(resource_global_handle);
        DWORD shader_size = SizeofResource(current_module, resource);

        HRESULT hr = device->CreatePixelShader(reinterpret_cast<const DWORD *>(shader_data), shader);
        return hr;
    }
}
