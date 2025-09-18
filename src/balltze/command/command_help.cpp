// SPDX-License-Identifier: GPL-3.0-only

#include <event/event.h>
#include <impl/terminal/terminal.h>
#include "../logger.hpp"
#include "command.hpp"

namespace Balltze {
    void set_up_commands_help() {
        ringworld_event_subscribe(RW_EVENT_HSC_FUNCTION_HELP_NOT_FOUND, +[](void *data) -> bool {
            std::string command = reinterpret_cast<char *>(data);
            auto &commands = get_commands();
            for(const auto &cmd : commands) {
                if(cmd->full_name() == command) {
                    auto *params_help = cmd->params_help();
                    if(params_help) {
                        terminal_info_printf("(%s %s)", cmd->full_name(), params_help);
                    }
                    else {
                        terminal_info_printf("(%s)", cmd->full_name());
                    }
                    terminal_info_printf("%s", cmd->help());
                    return true;
                }
            }
            return false;
        });
    }
}
