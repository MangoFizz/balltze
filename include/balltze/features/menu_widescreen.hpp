// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__FEATURES__MENU_WIDESCREEN_HPP
#define BALLTZE_API__FEATURES__MENU_WIDESCREEN_HPP

#include <utility>
#include <cstdint>
#include "../api.hpp"

namespace Balltze::Features { 
    /**
     * Get the current menu aspect ratio
     */
    BALLTZE_API float get_menu_aspect_ratio() noexcept;

    /**
     * Set the current menu aspect ratio
     */
    BALLTZE_API void set_menu_aspect_ratio(std::uint16_t x, std::uint16_t y) noexcept;

    /**
     * Reset the current menu aspect ratio
     */
    BALLTZE_API void reset_menu_aspect_ratio() noexcept;

    /**
     * Get the current mouse horizontal bounds
     * @return  Left and right mouse bounds
     */
    BALLTZE_API std::pair<std::uint16_t, std::uint16_t> get_menu_cursor_horizontal_bounds() noexcept;
}

#endif
