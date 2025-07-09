// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__LUA__API__CONSOLE_COMMAND_HPP
#define BALLTZE__LUA__API__CONSOLE_COMMAND_HPP

#include <string>
#include "../../../../command/command.hpp"
#include "../../../../plugins/plugin.hpp"

namespace Balltze::Lua::Api::V2 {
    class ConsoleCommand : public Balltze::Command {
    public:
        CommandResult call(std::size_t arg_count, const char **args) const noexcept;
        
        void register_command(Plugins::LuaPlugin *plugin);
        
        ConsoleCommand(std::string name, std::string category, std::string help, std::optional<std::string> params_help, bool autosave, std::size_t min_args, std::size_t max_args, bool can_call_from_console = true, bool is_public = false) :
            Balltze::Command(name, category, help, params_help, autosave, min_args, max_args, can_call_from_console, is_public) {}
    };
}

#endif
