// SPDX-License-Identifier: GPL-3.0-only

#include <optional>
#include <stdexcept>
#include <balltze/balltze.hpp>
#include <balltze/engine/window.hpp>

namespace Balltze::Engine {
    WindowGlobals *WindowGlobals::get() {
        static std::optional<WindowGlobals *> window_globals;

        if(!window_globals.has_value()) {
            auto &balltze = Balltze::get();
            auto &sig_manager = balltze.signature_manager();
            auto *window_globals_sig = sig_manager.get("window_globals");
        
            if(!window_globals_sig) {
                throw std::runtime_error("window_globals signature not found");
            }
    
            window_globals = *reinterpret_cast<WindowGlobals **>(window_globals_sig->data());
        }

        return *window_globals;
    }
}