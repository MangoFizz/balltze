// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__OUTPUT__DRAW_TEXT_HPP
#define BALLTZE__OUTPUT__DRAW_TEXT_HPP

#include <string>
#include <variant>
#include <balltze/output.hpp>
#include <balltze/engine/tag.hpp>

namespace Balltze {
    struct FontData {
        // Font being used
        Engine::TagHandle font;

        // I don't know
        std::uint32_t unknown1;

        // I still don't know
        std::int16_t unknown_ffff;

        // Alignment
        FontAlignment alignment;

        // Text to use
        Engine::ColorARGB color;

        // Tabs!
        std::int16_t tabs[0x12];

        // Offset
        std::uint32_t xy_offset;
    };
    static_assert(sizeof(FontData) == 0x44);

    /**
     * Get the font data for the text currently being drawn
     * @return font data
     */
    FontData &get_current_font_data() noexcept;

    /**
     * Set up the text hook for showing text.
     */
    void set_up_text_hook() noexcept;
}

#endif
