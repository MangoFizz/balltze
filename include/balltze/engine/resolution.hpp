// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__ENGINE__RESOLUTION_HPP
#define BALLTZE_API__ENGINE__RESOLUTION_HPP

#include <cstdint>
#include "../api.hpp"

namespace Balltze::Engine {
    /** This is the resolution of the game. */
    struct Resolution {
        /** Height in pixels */
        std::uint16_t height;

        /** Width in pixels */
        std::uint16_t width;
    };

    /**
     * Get the current resolution of Halo.
     * @return Return the current resolution of Halo.
     */
    BALLTZE_API Resolution &get_resolution() noexcept;
}

#endif
