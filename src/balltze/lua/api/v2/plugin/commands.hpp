// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__LUA__API__CONSOLE_COMMAND_HPP
#define BALLTZE__LUA__API__CONSOLE_COMMAND_HPP

#include <string>
#include "../../../../command/command.hpp"
#include "../../../../plugins/plugin.hpp"

namespace Balltze::Lua::Api::V2 {
    class ConsoleCommand : public Balltze::Command {
    private:
        int function_ref = LUA_NOREF;

    public:
        /**
         * Call a console command with the given arguments
         * 
         * @param arg_count number of arguments to pass
         * @param args array of arguments
         * @return result of command
         */
        CommandResult call(std::size_t arg_count, const char **args) const noexcept;
        
        /**
         * Register the command to the command list
         * 
         * @param plugin the plugin that registered the command
         * @todo Refactor the commands system to remove the need for this function
         */
        void register_command(Plugins::LuaPlugin *plugin);
        
        /**
         * Instantiate a console command
         * 
         * @param name name of the command
         * @param category category of the command
         * @param help help info of the command
         * @param params_help help info of the parameters of the command
         * @param autosave auto saves if successful and at least 1 arg was passed
         * @param min_args minimum arguments
         * @param max_args maximum arguments
         * @param function_ref Lua function reference to call for the command
         * @param can_call_from_console can be called from console
         * @param is_public is public, can be called from other plugins
         * @todo Rework this to use a builder pattern
         * @todo Move the creation of the reference of the function to the constructor
         */
        ConsoleCommand(std::string name, std::string category, std::string help, std::optional<std::string> params_help, bool autosave, std::size_t min_args, std::size_t max_args, int function_ref, bool can_call_from_console = true, bool is_public = false) :
            Balltze::Command(name, category, help, params_help, autosave, min_args, max_args, can_call_from_console, is_public), function_ref(function_ref) {}
    };

    /**
     * Remove all console commands registered by a plugin
     * 
     * @param plugin the plugin to remove commands for
     */
    void remove_console_commands_for_plugin(Plugins::LuaPlugin *plugin) noexcept;
}

#endif
