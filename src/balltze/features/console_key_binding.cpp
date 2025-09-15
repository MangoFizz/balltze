// SPDX-License-Identifier: GPL-3.0-only

#include <chrono>
#include <balltze/legacy_api/engine/user_interface.hpp>
#include <impl/terminal/terminal.h>
#include "../command/command.hpp"
#include "../logger.hpp"

namespace Balltze::Features {
    void set_up_set_console_key_binding_command() noexcept {
        CommandBuilder()
            .name("set_console_key_binding")
            .category("features")
            .help("Set the console key binding.")
            .param(HSC_DATA_TYPE_SHORT, "key")
            .function([](const std::vector<std::string> &args) -> bool {
                LegacyApi::Engine::KeyboardKey key;
                try {
                    auto keyint = std::stoi(args[0].c_str());
                    if(keyint > 103 && keyint < 0) {
                        terminal_info_printf("Invalid key: %d", keyint);
                        return false;
                    }
                    key = static_cast<LegacyApi::Engine::KeyboardKey>(keyint);
                }
                catch(std::invalid_argument &) {
                    terminal_info_printf("Error: key must be an integer.");
                    return false;
                }
                LegacyApi::Engine::set_console_key_binding(key);
                terminal_info_printf("Console key binding set to: %s", args[0].c_str());
                return true;
            })
            .autosave()
            .is_public()
            .can_call_from_console()
            .create();
    }
}
