// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__MESSAGE_BOX_HPP
#define BALLTZE_API__MESSAGE_BOX_HPP

#include <windows.h>
#include <cstdio>
#include "engine/core.hpp"
#include "api.hpp"

namespace Balltze {
    using ConsoleColor = Engine::ColorARGB;

    #define CONSOLE_COLOR_ERROR ConsoleColor{1.0, 0.0, 0.0, 1.0}

    /**
     * Create a dialog with a given message
     * @param message   Message to display
     */
    BALLTZE_API void show_message_box_raw(unsigned int type, const char *message) noexcept;

    /**
     * Create a dialog with a given message
     * @param format    Message format
     * @param args      Format arguments
     */
    template<typename... Args> void show_message_box(const char *format, Args... args) noexcept {
        char message[256];
        std::snprintf(message, sizeof(message), format, args...);
        show_message_box_raw(MB_OK, message);
    }

    /**
     * Create a dialog with a given message
     * @param format    Message format
     * @param args      Format arguments
     */
    template<typename... Args> void show_error_box(const char *format, Args... args) noexcept {
        char message[256];
        std::snprintf(message, sizeof(message), format, args...);
        show_message_box_raw(MB_OK | MB_ICONERROR, message);
    }

        /**
     * Display a message of a color
     * @param color  Color to use in the message
     * @param format String format (passed to snprintf)
     * @param args   Additional arguments to pass
     */
    template<typename... Args> void console_printf(const ConsoleColor &color, const char *format, Args... args) noexcept {
        char message[256];
        std::snprintf(message, sizeof(message), format, args...);
        Engine::console_print(message, color);
    }

    /**
     * Display a message (white)
     * @param format String format (passed to snprintf)
     * @param args   Additional arguments to pass
     */
    template<typename... Args> void console_printf(const char *format, Args... args) noexcept {
        console_printf(ConsoleColor{1.0, 1.0, 1.0, 1.0}, format, args...);
    }
}

#endif
