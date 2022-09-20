// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__OUTPUT___MESSAGE_BOX_HPP
#define BALLTZE__OUTPUT___MESSAGE_BOX_HPP

#include <cstdio>

namespace Balltze {
    /**
     * Create a dialog with a given message
     * @param message   Message to display
     */
    void message_box(const char *message) noexcept;

    /**
     * Create a dialog with a given message
     * @param format    Message format
     * @param args      Format arguments
     */
    template<typename... Args> void message_box(const char *format, Args... args) noexcept {
        char message[256];
        std::snprintf(message, sizeof(message), format, args...);
        message_box(message);
    }
}

#endif
