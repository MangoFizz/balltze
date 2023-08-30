// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/engine/renderer.hpp>

namespace Balltze::Engine {
    extern "C" {
        IDirect3DTexture9 *load_bitmap_asm(TagDefinitions::BitmapData *bitmap_data, bool immediate, bool force_pixels_read);
    }

    IDirect3DTexture9 *load_bitmap(TagDefinitions::BitmapData *bitmap_data, bool immediate, bool force_pixels_read) noexcept {
        return load_bitmap_asm(bitmap_data, immediate, force_pixels_read);
    }

    void set_bitmap_texture(IDirect3DDevice9 *device, std::size_t stage, TagDefinitions::BitmapData *bitmap_data) noexcept {
        if(bitmap_data->texture_cache_id == 0xFFFFFFFF) {
            load_bitmap(bitmap_data, true, true);
        }
        device->SetTexture(stage, reinterpret_cast<IDirect3DBaseTexture9 *>(bitmap_data->texture));
    }
}
