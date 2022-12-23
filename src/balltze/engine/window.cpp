// SPDX-License-Identifier: GPL-3.0-only

#include <optional>
#include <stdexcept>
#include <balltze/engine/window.hpp>
#include <balltze/memory.hpp>

namespace Balltze::Engine {
    WindowGlobals *get_window_globals() {
        static std::optional<WindowGlobals *> window_globals;
        if(!window_globals.has_value()) {
            auto *window_globals_sig = Memory::get_signature("window_globals");
            if(!window_globals_sig) {
                throw std::runtime_error("window_globals signature not found");
            }
            window_globals = *reinterpret_cast<WindowGlobals **>(window_globals_sig->data());
        }
        return *window_globals;
    }
}
