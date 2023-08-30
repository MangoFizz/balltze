// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__ENGINE__RENDERER_HPP
#define BALLTZE_API__ENGINE__RENDERER_HPP

#include <d3d9.h>
#include "tag_definitions/bitmap.hpp"
#include "../api.hpp"

namespace Balltze::Engine {
    /**
     * Load a bitmap into the texture cache
     * @param bitmap_data       the bitmap to load
     * @param immediate         whether to load the bitmap immediately
     * @param force_pixels_read whether to force the pixels to be read
     */
    BALLTZE_API IDirect3DTexture9 *load_bitmap(TagDefinitions::BitmapData *bitmap_data, bool immediate, bool force_pixels_read) noexcept;

    /**
     * Set the texture of a bitmap to a stage of the device
     * @param device        the device to set the texture on
     * @param stage         the stage to set the texture on
     * @param bitmap_data   the bitmap to set the texture to
     */
    BALLTZE_API void set_bitmap_texture(IDirect3DDevice9 *device, std::size_t stage, TagDefinitions::BitmapData *bitmap_data) noexcept;
}

#endif
