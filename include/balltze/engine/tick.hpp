// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__ENGINE__TICK_HPP
#define BALLTZE__ENGINE__TICK_HPP

#include <cstddef>

namespace Balltze::Engine {
    /**
     * Returns the number of ticks since the engine started.
     */
    std::size_t get_tick_count() noexcept;
}

#endif
