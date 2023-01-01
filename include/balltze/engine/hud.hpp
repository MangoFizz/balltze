// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__ENGINE__BITMAP_HPP
#define BALLTZE_API__ENGINE__BITMAP_HPP

#include <utility>
#include "../api.hpp"
#include "data_types.hpp"
#include "tag_definitions/hud_globals.hpp"

namespace Balltze::Engine {
    using HudGlobals = TagDefinitions::HudGlobals;

    /**
     * Gets the HUD globals tag.
     * @return  The HUD globals tag.
     */
    BALLTZE_API HudGlobals &get_hud_globals();

    /**
     * Gets the size of a sprite in a bitmap.
     * @param bitmap_tag_handle     Tag handle of the bitmap.
     * @param sequence_index        Sequence index of the bitmap.
     * @param sprite_index          The index of the sprite from the sequence.
     * @return                      The size of the sprite.
     * @throws std::runtime_error   If the bitmap tag handle is invalid.
     * @throws std::runtime_error   If the sequence index is invalid.
     * @throws std::runtime_error   If the sprite index is invalid.
     */
    BALLTZE_API std::pair<std::size_t, std::size_t> get_bitmap_sprite_size(TagHandle bitmap_tag_handle, std::size_t sequence_index, std::size_t sprite_index);

    /**
     * Draws a icon bitmap on a HUD message.
     * @param bitmap_tag_handle     Tag handle of the bitmap to draw.
     * @param sequence_index        Sequence index of the bitmap.
     * @param sprite_index          The index of the sprite from the sequence.
     * @param position              The position of the bitmap on the HUD.
     * @param color                 The color of the bitmap.
     * @throws std::runtime_error   If the bitmap tag handle is invalid.
     * @throws std::runtime_error   If the sequence index is invalid.
     * @throws std::runtime_error   If the sprite index is invalid.
     */
    BALLTZE_API void draw_hud_message_sprite(TagHandle bitmap_tag_handle, std::size_t sequence_index, std::size_t sprite_index, Point2DInt position, ColorARGBInt color);
}

#endif
