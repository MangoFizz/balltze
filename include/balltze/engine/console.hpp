// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__ENGINE__CONSOLE_HPP
#define BALLTZE_API__ENGINE__CONSOLE_HPP

#include <string>
#include <cstdio>
#include "data_types.hpp"

namespace Balltze::Engine {
    /**
     * Display a message of a color
     * @param color   Color to use in the message
     * @param message Message to use
     */
    void console_print(std::string message, ColorARGB color) noexcept;
}

#endif
