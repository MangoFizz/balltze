// SPDX-License-Identifier: GPL-3.0-only

#include <vector>
#include "../../../../config/config.hpp"
#include "../../../../plugins/plugin.hpp"
#include "../../../../plugins/loader.hpp"
#include "../../../../logger.hpp"
#include "../../../helpers/plugin.hpp"
#include "../../../helpers/function_table.hpp"
#include "commands.hpp"

namespace Balltze::Lua::Api::V2 {
    static auto LUA_CONSOLE_COMMANDS_TABLE = "console_commands";

    using LuaPlugin = Plugins::LuaPlugin;

    static auto to_lower(std::string str) {
        std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
            return std::tolower(c);
        });
        return str;
    }

    static void get_commands_table(lua_State *state) noexcept {
        get_or_create_plugin_registry_table(state, LUA_CONSOLE_COMMANDS_TABLE);
    }

    static void clear_commands_table(lua_State *state) noexcept {
        clear_registry_plugin_registry_table(state, LUA_CONSOLE_COMMANDS_TABLE);
    }

    static CommandFunction get_command_function(LuaPlugin *plugin, std::string command_name, int function_ref) noexcept {
        return [plugin, command_name, function_ref](const std::vector<std::string> &arguments) -> bool {
            lua_State *state = plugin->lua_state();
            get_commands_table(state);
            lua_rawgeti(state, -1, function_ref);
            lua_remove(state, -2); 
            if(!lua_isnil(state, -1)) {
                lua_pushcfunction(state, plugin_error_handler);
                lua_pushvalue(state, -2);
                lua_newtable(state);
                for(std::size_t i = 0; i < arguments.size(); i++) {
                    lua_pushstring(state, arguments[i].c_str());
                    lua_rawseti(state, -2, i + 1);
                }
                if(lua_pcall(state, 1, 1, -3) == LUA_OK) {
                    if(!lua_isnil(state, -1)) {
                        if(lua_toboolean(state, -1)) {
                            lua_pop(state, 2);
                            return true;
                        }
                    }
                    else {
                        logger.warning("Lua command {} returned nil in plugin {}", command_name, plugin->name());
                    }
                    lua_pop(state, 1);
                }
                else {
                    logger.debug("Lua command {} from plugin {} failed: {}", command_name, plugin->name(), plugin->pop_error_message());
                }
            }
            else {
                logger.warning("Lua command {} not found in plugin {}", command_name, plugin->name());
            }
            lua_pop(state, 1);
            return false;
        };
    }
    
    static int lua_register_command(lua_State *state) noexcept {
        auto *plugin = get_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 10) {
                auto *name = luaL_checkstring(state, 1);
                auto *category = luaL_checkstring(state, 2);
                auto *help = luaL_checkstring(state, 3);
                std::optional<std::string> params_help = std::nullopt;
                if(!lua_isnil(state, 4)) {
                    params_help = luaL_checkstring(state, 4);
                }
                bool autosave;
                if(lua_isboolean(state, 5)) {
                    autosave = lua_toboolean(state, 5);
                }
                else {
                    return luaL_error(state, "Argument 5 of Balltze.command.registerCommand must be a boolean.");
                }
                std::size_t min_args = luaL_checkinteger(state, 6);
                std::size_t max_args = luaL_checkinteger(state, 7);
                bool can_call_from_console;
                if(lua_isboolean(state, 8)) {
                    can_call_from_console = lua_toboolean(state, 8);
                }
                else {
                    return luaL_error(state, "Argument 8 of Balltze.command.registerCommand must be a boolean.");
                }
                bool is_public;
                if(lua_isboolean(state, 9)) {
                    is_public = lua_toboolean(state, 9);
                }
                else {
                    return luaL_error(state, "Argument 9 of Balltze.command.registerCommand must be a boolean.");
                }
                if(!lua_isfunction(state, 10)) {
                    return luaL_error(state, "Argument 10 of Balltze.command.registerCommand must be a function.");
                }
                
                get_commands_table(state);
                lua_pushvalue(state, 10);
                int function_ref = luaL_ref(state, -2);
                lua_pop(state, 1); 

                auto function = get_command_function(plugin, name, function_ref);

                add_command(Command(name, category, help, params_help, function, 
                    autosave, min_args, max_args, can_call_from_console, is_public, "", COMMAND_SOURCE_PLUGIN, plugin));

                return 0;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function Balltze.command.registerCommand.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static int lua_execute_command(lua_State *state) noexcept {
        auto *plugin = get_plugin(state);
        if(!plugin) {
            return luaL_error(state, "Missing plugin upvalue in function Balltze.command.executeCommand.");
        }

        int args = lua_gettop(state);
        if(args != 1) {
            return luaL_error(state, "Invalid number of arguments in function Balltze.command.executeCommand.");
        }

        auto *command = luaL_checkstring(state, 1);
        std::vector<std::string> arguments = split_arguments(command);
        std::string command_name = arguments[0];
        arguments.erase(arguments.begin());

        CommandResult res = COMMAND_RESULT_FAILED_ERROR_NOT_FOUND;
        auto &commands = get_commands();
        for(const auto &command : commands) {
            if(command->full_name() == command_name) {
                Plugins::Plugin *command_plugin = reinterpret_cast<Plugins::Plugin *>(*command->plugin());
                if(command->is_public() || plugin == command_plugin) {
                    res = command->call(arguments);

                    // Save if autosave is enabled
                    if(command->autosave() && res == COMMAND_RESULT_SUCCESS) {
                        bool is_balltze_command = command_plugin == nullptr;
                        if(is_balltze_command) {
                            auto &config = Config::get_config();
                            config.set(std::string("commands.") + command->name(), unsplit_arguments(arguments));
                            config.save();
                        }
                        else {
                            auto directory = command_plugin->directory();
                            auto config = Config::Config(directory / "settings.json");
                            config.set(std::string("commands.") + command->name(), unsplit_arguments(arguments));
                            config.save();
                        }
                    }
                }
                else {
                    return luaL_error(state, "Command is not public.");
                }              
                break;
            }
        }
        return 0;
    }

    static int lua_load_settings(lua_State *state) noexcept {
        auto *plugin = get_plugin(state);
        if(!plugin) {
            return luaL_error(state, "Missing plugin upvalue in function Balltze.command.loadSettings.");
        }
        auto directory = plugin->directory();
        auto config = Config::Config(directory / "settings.json");
        auto &commands = get_commands();
        for(const auto &command : commands) {
            auto command_key = std::string("commands.") + command->name();
            if(command->plugin() && command->plugin() == reinterpret_cast<void *>(plugin)) {
                if(config.exists(command_key)) {
                    auto command_value = config.get<std::string>(command_key);
                    auto arguments = split_arguments(command_value.value());
                    bool res = command->call(arguments);
                    if(res == COMMAND_RESULT_FAILED_ERROR) {
                        logger.error("Command {} failed to load from config", command->name());
                    }
                }
            }
        }
        return 0;
    }

    void set_up_plugin_commands(lua_State *state, int table_idx) noexcept {
        lua_pushvalue(state, table_idx);
        push_plugin_function(state, lua_register_command);
        lua_setfield(state, -2, "registerCommand");
        push_plugin_function(state, lua_execute_command);
        lua_setfield(state, -2, "executeCommand");
        push_plugin_function(state, lua_load_settings);
        lua_setfield(state, -2, "loadSettings");
        lua_pop(state, 1); 
    }
}
