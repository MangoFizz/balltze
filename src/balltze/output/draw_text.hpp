// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__OUTPUT__DRAW_TEXT_HPP
#define BALLTZE__OUTPUT__DRAW_TEXT_HPP

#include <string>
#include <variant>
#include <balltze/engine/tag.hpp>

namespace Balltze {
    /**
     * Refer to whatever font tag is referenced in the globals tag
     */
    enum GenericFont {
        /** Fonts used for console */
        FONT_CONSOLE = 0,

        /** Fonts used for system (e.g. menu text) */
        FONT_SYSTEM = 1,

        /** Fonts used for small text */
        FONT_SMALL = 2,

        /** Fonts used for large text */
        FONT_LARGE = 3,

        /** Fonts used for names above heads (basically small but smaller) */
        FONT_SMALLER = 4,

        /** Fonts used for the ticker in the user interface */
        FONT_TICKER = 5
    };

    /**
     * Get a generic font from the string
     * @param  str string value
     * @return     generic font
     */
    GenericFont generic_font_from_string(const char *str) noexcept;

    enum FontAlignment : std::int16_t {
        ALIGN_LEFT = 0,
        ALIGN_RIGHT,
        ALIGN_CENTER
    };

    enum TextAnchor {
        ANCHOR_TOP_LEFT,
        ANCHOR_TOP_RIGHT,
        ANCHOR_BOTTOM_RIGHT,
        ANCHOR_BOTTOM_LEFT,
        ANCHOR_CENTER
    };

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
     * Get the font tag for a specific GenericFont.
     * @param  font the type of generic font
     * @return      the tag's tag handle
     */
    const Engine::TagHandle &get_generic_font(GenericFont font) noexcept;

    /**
     * Set up the text hook for showing text.
     */
    void set_up_text_hook() noexcept;

    /**
     * Get the number of pixels a string takes up given a font.
     * @param  text the text to measure
     * @param  font the font
     * @return      the length in pixels
     */
    std::int16_t text_pixel_length(const char *text, const std::variant<Engine::TagHandle, GenericFont> &font) noexcept;

    /**
     * Get the number of pixels a string takes up given a font.
     * @param  text the text to measure
     * @param  font the font
     * @return      the length in pixels
     */
    std::int16_t text_pixel_length(const wchar_t *text, const std::variant<Engine::TagHandle, GenericFont> &font) noexcept;

    /**
     * Get the height of the font
     * @param  font the font
     * @return      the height in pixels
     */
    std::int16_t font_pixel_height(const std::variant<Engine::TagHandle, GenericFont> &font) noexcept;

    /**
     * Display text on the screen for one frame.
     * @param text      text to display
     * @param x         pixels for left side
     * @param y         pixels for top side
     * @param width     width of textbox
     * @param height    height of textbox
     * @param color     color to use
     * @param font      font to use; can be a generic font type or a specific font tag
     * @param alignment alignment to use
     * @param anchor    anchor to use
     * @param immediate attempt to render it immediately
     */
    void apply_text(std::variant<std::string, std::wstring> text, std::int16_t x, std::int16_t y, std::int16_t width, std::int16_t height, const Engine::ColorARGB &color, const std::variant<Engine::TagHandle, GenericFont> &font, FontAlignment alignment, TextAnchor anchor, bool immediate = false) noexcept;
}

#endif
