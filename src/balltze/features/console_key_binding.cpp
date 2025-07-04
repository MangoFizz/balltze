// SPDX-License-Identifier: GPL-3.0-only

#include <chrono>
#include <balltze/command.hpp>
#include <balltze/legacy_api/engine/user_interface.hpp>
#include "../logger.hpp"

namespace Balltze::Features {
    void set_up_set_console_key_binding_command() noexcept {
        register_command("set_console_key_binding", "features", "Set the console key binding.", "<key>", [](int argc, const char **argv) -> bool {
            LegacyApi::Engine::KeyboardKey key;
            try {
                auto keyint = std::stoi(argv[0]);
                if(keyint > 103 && keyint < 0) {
                    LegacyApi::Engine::console_printf("Invalid key: {}", keyint);
                    return false;
                }
                key = static_cast<LegacyApi::Engine::KeyboardKey>(keyint);
            }
            catch(std::invalid_argument &) {
                LegacyApi::Engine::console_print("Error: key must be an integer.");
                return false;
            }
            LegacyApi::Engine::set_console_key_binding(key);
            LegacyApi::Engine::console_printf("Console key binding set to: %s", argv[0]);
            return true;
        }, true, 1, 1, true, true);
    }
}
