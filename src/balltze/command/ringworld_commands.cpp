// SPDX-License-Identifier: GPL-3.0-only

#include <impl/console/console.h>
#include <impl/interface/hud.h>
#include <impl/terminal/terminal.h>
#include <version.h>
#include <balltze/command.hpp>
#include "command.hpp"

namespace Balltze {
    void register_ringworld_commands() noexcept {
        CommandBuilder()
            .name("console_enable")
            .help("Set whether the console is enabled")
            .category("ringworld")
            .param(HSC_DATA_TYPE_BOOLEAN, "enable")
            .can_call_from_console()
            .autosave()
            .default_value("true")
            .function([](const std::vector<std::string> &args) -> bool {
                ConsoleGlobals *console_globals = console_get_globals();
                console_globals->console_is_enabled = STR_TO_BOOL(args[0].c_str());
                return true;
            })
            .is_client_side()
            .create(COMMAND_SOURCE_RINGWORLD);

        CommandBuilder()
            .name("console_prompt_color")
            .help("Set the console prompt color")
            .category("ringworld")
            .param(HSC_DATA_TYPE_SHORT, "r")
            .param(HSC_DATA_TYPE_SHORT, "g")
            .param(HSC_DATA_TYPE_SHORT, "b")
            .can_call_from_console()
            .autosave()
            .default_value("255 77 255")
            .function([](const std::vector<std::string> &args) -> bool {
                ConsoleGlobals *console_globals = console_get_globals();
                try {
                    int r = std::stoi(args[0].c_str());
                    int g = std::stoi(args[1].c_str());
                    int b = std::stoi(args[2].c_str());
                    if(r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) {
                        terminal_error_printf("Error: color values must be between 0 and 255.");
                        return false;
                    }
                    console_globals->input_state.color.r = r / 255.0f;
                    console_globals->input_state.color.g = g / 255.0f;
                    console_globals->input_state.color.b = b / 255.0f;
                }
                catch(std::invalid_argument &) {
                    terminal_error_printf("Error: color values must be integers.");
                    return false;
                }
                return true;
            })
            .is_client_side()
            .create(COMMAND_SOURCE_RINGWORLD);

        CommandBuilder()
            .name("hud_safe_zones")
            .help("Set the HUD safe zones")
            .category("ringworld")
            .param(HSC_DATA_TYPE_LONG, "horizontal")
            .param(HSC_DATA_TYPE_LONG, "vertical")
            .can_call_from_console()
            .autosave()
            .default_value("0 0")
            .function([](const std::vector<std::string> &args) -> bool {
                try {
                    int16_t horizontal = std::stoi(args[0].c_str());
                    int16_t vertical = std::stoi(args[1].c_str());
                    if(horizontal < 0 || vertical < 0) {
                        terminal_error_printf("Error: safezone values must be non-negative.");
                        return false;
                    }
                    hud_set_safe_zones(horizontal, vertical);
                }
                catch(std::invalid_argument &) {
                    terminal_error_printf("Error: safezone values must be integers.");
                    return false;
                }
                return true;
            })
            .is_client_side()
            .create(COMMAND_SOURCE_RINGWORLD);

        CommandBuilder()
            .name("version")
            .help("Prints the current version of Ringworld")
            .category("ringworld")
            .can_call_from_console()
            .function([](const std::vector<std::string> &args) -> bool {
                terminal_info_printf("Ringworld version %s", RINGWORLD_VERSION_STRING);
                return true;
            })
            .is_core()
            .create(COMMAND_SOURCE_RINGWORLD);
    }
}
