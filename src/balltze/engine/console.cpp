// SPDX-License-Identifier: GPL-3.0-only

#include <string>
#include <cstring>
#include <balltze/engine/console.hpp>

namespace Balltze::Engine {
    extern "C" {
        void console_print_asm(const ColorARGB &color, const char *message);
    }

    void console_print(std::string message, ColorARGB color) noexcept {
        console_print_asm(color, message.c_str());
    }
}
