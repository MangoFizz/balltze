// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__OUTPUT_HPP
#define BALLTZE_API__OUTPUT_HPP

#include <filesystem>
#include <chrono>
#include <variant>
#include "engine/data_types.hpp"
#include "engine/tag.hpp"
#include "api.hpp"

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
    BALLTZE_API GenericFont generic_font_from_string(const char *str) noexcept;

    /**
     * Get the font tag for a specific GenericFont.
     * @param  font the type of generic font
     * @return      the tag's tag handle
     */
    BALLTZE_API const Engine::TagHandle &get_generic_font(GenericFont font) noexcept;

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

    /**
     * Get the number of pixels a string takes up given a font.
     * @param  text the text to measure
     * @param  font the font
     * @return      the length in pixels
     */
    BALLTZE_API std::int16_t get_text_pixel_length(const char *text, const std::variant<Engine::TagHandle, GenericFont> &font) noexcept;

    /**
     * Get the number of pixels a string takes up given a font.
     * @param  text the text to measure
     * @param  font the font
     * @return      the length in pixels
     */
    BALLTZE_API std::int16_t get_text_pixel_length(const wchar_t *text, const std::variant<Engine::TagHandle, GenericFont> &font) noexcept;

    /**
     * Get the height of the font
     * @param  font the font
     * @return      the height in pixels
     */
    BALLTZE_API std::int16_t get_font_pixel_height(const std::variant<Engine::TagHandle, GenericFont> &font) noexcept;

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
    BALLTZE_API void apply_text(std::variant<std::string, std::wstring> text, std::int16_t x, std::int16_t y, std::int16_t width, std::int16_t height, const Engine::ColorARGB &color, const std::variant<Engine::TagHandle, GenericFont> &font, FontAlignment alignment, TextAnchor anchor, bool immediate = false) noexcept;

    /**
     * Show text as a subtitle on the screen.
     * @param text The text to show.
     * @param color The color of the text.
     * @param duration The duration of the subtitle.
     */
    BALLTZE_API void add_subtitle(std::string text, Engine::ColorARGB color, std::chrono::milliseconds duration);

    /**
     * Show text as a subtitle on the screen.
     * @param text The text to show.
     * @param color The color of the text.
     * @param duration The duration of the subtitle.
     */
    BALLTZE_API void add_subtitle(std::wstring text, Engine::ColorARGB color, std::chrono::milliseconds duration);

    /**
     * Play a BIK video.
     * @param path The path to the BIK video.
     */
    BALLTZE_API void play_bik_video(std::filesystem::path const &path);
}

#endif
