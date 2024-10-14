// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/helpers/resources.hpp>

std::optional<std::vector<std::byte>> load_resource_data(HMODULE module, const wchar_t *resource_id, const wchar_t *resource_type) noexcept {
    HRSRC resource = FindResourceW(module, resource_id, resource_type);
    if(!resource) {
        return std::nullopt;
    }
    
    std::uint32_t resource_size = SizeofResource(module, resource);
    if(resource_size == 0) {
        return std::nullopt;
    }

    HGLOBAL resource_global_handle = LoadResource(module, resource);
    if(!resource_global_handle) {
        return std::nullopt;
    }
    
    void *resource_bytes = LockResource(resource_global_handle);
    std::vector<std::byte> buffer;
    buffer.resize(resource_size);
    memcpy(buffer.data(), resource_bytes, resource_size);
    return std::move(buffer);
}

std::unique_ptr<Gdiplus::Bitmap> load_image_from_resource(HMODULE module, const wchar_t *resource_id, const wchar_t *resource_type) noexcept {
    HRSRC resource = FindResourceW(module, resource_id, resource_type);
    if(!resource) {
        return nullptr;
    }
    
    DWORD resource_size = SizeofResource(module, resource);
    if(resource_size == 0) {
        return nullptr;
    }

    HGLOBAL resource_global_handle = LoadResource(module, resource);
    if(!resource_global_handle) {
        return nullptr;
    }
    
    void *image_bytes = LockResource(resource_global_handle);
    HGLOBAL global_handle = GlobalAlloc(GHND, resource_size);
    if(!global_handle) {
        return nullptr;
    }
    
    void *buffer = GlobalLock(global_handle);
    if(!buffer) {
        GlobalFree(global_handle);
        return nullptr;
    }
    
    IStream *stream = nullptr;
    memcpy(buffer, image_bytes, resource_size);
    HRESULT hr = CreateStreamOnHGlobal(global_handle, TRUE, &stream);
    if(SUCCEEDED(hr)) {
        std::unique_ptr<Gdiplus::Bitmap> bitmap = std::make_unique<Gdiplus::Bitmap>(stream);
        stream->Release();
        stream = nullptr;
        return bitmap;
    }
    else {
        GlobalFree(global_handle);
        global_handle = nullptr;
        return nullptr;
    }
}
