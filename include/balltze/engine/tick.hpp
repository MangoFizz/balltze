// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__ENGINE__TICK_HPP
#define BALLTZE_API__ENGINE__TICK_HPP

#include <cstddef>
#include "../api.hpp"

namespace Balltze::Engine {
    /**
     * Returns the number of ticks since the engine started.
     */
    BALLTZE_API std::size_t get_tick_count() noexcept;
}

#endif
