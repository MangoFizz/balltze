// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__OUTPUT___MESSAGE_BOX_HPP
#define BALLTZE__OUTPUT___MESSAGE_BOX_HPP

#include <cstdio>

namespace Balltze {
    /**
     * Create a dialog with a given message
     * @param message   Message to display
     */
    void show_message_box_raw(unsigned int type, const char *message) noexcept;

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
}

#endif
