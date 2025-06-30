// SPDX-License-Identifier: GPL-3.0-only

#include <vector>
#include "../../../../logger.hpp"
#include "../../../../plugins/loader.hpp"
#include "../../../../config/config.hpp"
#include "../../../helpers/function_table.hpp"
#include "commands.hpp"

namespace Balltze {
    extern std::vector<std::shared_ptr<Command>> commands;
}

namespace Balltze::Lua {
    using LuaPlugin = Plugins::LuaPlugin;

    static int lua_get_commands_table(lua_State *state) noexcept {
        auto balltze_module = Balltze::get_current_module();
        lua_pushlightuserdata(state, balltze_module);
        lua_gettable(state, LUA_REGISTRYINDEX);
        if(lua_isnil(state, -1)) {
            lua_pop(state, 1);
            logger.error("Could not find Lua registry table");
            return 0;
        }
        lua_getfield(state, -1, "commands");
        if(lua_isnil(state, -1)) {
            lua_pop(state, 1);

            // if commands table does not exist, create it
            lua_newtable(state);
            lua_setfield(state, -2, "commands");

            lua_getfield(state, -1, "commands");
        }
        lua_remove(state, -2);
        return 1;
    }

    CommandResult ConsoleCommand::call(std::size_t arg_count, const char **args) const noexcept {
        LuaPlugin *plugin = static_cast<LuaPlugin *>(m_plugin.value());
        lua_State *state = plugin->state();
        if(lua_get_commands_table(state) == 0) {
            return COMMAND_RESULT_FAILED_ERROR_NOT_FOUND;
        }
        lua_getfield(state, -1, m_name.c_str());
        lua_remove(state, -2);
        if(!lua_isnil(state, -1)) {
            lua_newtable(state);
            for(std::size_t i = 0; i < arg_count; i++) {
                lua_pushstring(state, args[i]);
                lua_rawseti(state, -2, i + 1);
            }
            if(lua_pcall(state, 1, 1, 0) == LUA_OK) {
                if(!lua_isnil(state, -1)) {
                    if(lua_toboolean(state, -1)) {
                        lua_pop(state, 1);
                        return COMMAND_RESULT_SUCCESS;
                    }
                }
                else {
                    logger.warning("Lua command {} returned nil", *m_full_name);
                }
                lua_pop(state, 1);
            }
            else {
                logger.debug("Lua command {} failed: {}", *m_full_name, plugin->get_error_message());
                plugin->print_traceback();
            }
        }
        else {
            logger.warning("Lua command {} not found", *m_full_name);
        }
        return COMMAND_RESULT_FAILED_ERROR;
    }

    void ConsoleCommand::register_command(LuaPlugin *plugin) {
        lua_State *state = plugin->state();
        if(lua_get_commands_table(state) == 0) {
            throw std::runtime_error("Could not find Lua commands table");
        }
        lua_pushvalue(state, -2);
        lua_setfield(state, -2, m_name.c_str());

        for(const auto &command : commands) {
            if(std::strcmp(command->name(), this->name()) == 0 && command->plugin() == reinterpret_cast<PluginHandle>(plugin)) {
                throw std::runtime_error("Command " + std::string(this->name()) + " already registered!");
            }
        }
        m_plugin = reinterpret_cast<PluginHandle>(plugin);
        m_full_name = get_full_name();
        commands.emplace_back(std::make_shared<ConsoleCommand>(*this));
    }

    static int register_command(lua_State *state) noexcept {
        auto *plugin = Plugins::get_lua_plugin(state);
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
                if(lua_isfunction(state, 10)) {
                    lua_pushvalue(state, 10);
                }
                else {
                    return luaL_error(state, "Argument 10 of Balltze.command.registerCommand must be a function.");
                }

                ConsoleCommand command(name, category, help, params_help, autosave, min_args, max_args, can_call_from_console, is_public);
                try {
                    command.register_command(plugin);
                    lua_pop(state, 1);
                }
                catch(const std::runtime_error &e) {
                    return luaL_error(state, e.what());
                }
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

    static int execute_command(lua_State *state) noexcept {
        auto *plugin = Plugins::get_lua_plugin(state);
        if(!plugin) {
            return luaL_error(state, "Missing plugin upvalue in function Balltze.command.executeCommand.");
        }

        int args = lua_gettop(state);
        if(args != 1) {
            return luaL_error(state, "Invalid number of arguments in function Balltze.command.executeCommand.");
        }

        auto *command = luaL_checkstring(state, 1);
        std::vector<std::string> arguments = split_arguments(command);
        std::size_t arg_count = arguments.size() - 1;
        std::string command_name = arguments[0];
        arguments.erase(arguments.begin());

        auto arguments_alloc(std::make_unique<const char *[]>(arg_count));
        for(std::size_t i = 0; i < arg_count; i++) {
            arguments_alloc[i] = arguments[i].data();
        }
        
        CommandResult res = COMMAND_RESULT_FAILED_ERROR_NOT_FOUND;
        for(const auto command : commands) {
            if(command->full_name() == command_name) {
                Plugins::Plugin *command_plugin = reinterpret_cast<Plugins::Plugin *>(*command->plugin());
                if(command->is_public() || plugin == command_plugin || plugin->filename() == "balltze_devkit_server.lua") {
                    res = command->call(arg_count, arguments_alloc.get());

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

    static int load_settings(lua_State *state) noexcept {
        auto *plugin = Plugins::get_lua_plugin(state);
        if(!plugin) {
            return luaL_error(state, "Missing plugin upvalue in function Balltze.command.loadSettings.");
        }
        auto directory = plugin->directory();
        auto config = Config::Config(directory / "settings.json");
        for(auto &command : commands) {
            auto command_key = std::string("commands.") + command->name();
            if(command->plugin() && command->plugin() == reinterpret_cast<void *>(plugin)) {
                if(config.exists(command_key)) {
                    auto command_value = config.get<std::string>(command_key);
                    auto arguments = split_arguments(command_value.value());
                    
                    auto arguments_alloc(std::make_unique<const char *[]>(arguments.size()));
                    for(std::size_t i = 0; i < arguments.size(); i++) {
                        arguments_alloc[i] = arguments[i].data();
                    }

                    bool res = command->call(arguments.size(), arguments_alloc.get());
                    if(res == COMMAND_RESULT_FAILED_ERROR) {
                        logger.error("Command {} failed to load from config", command->name());
                    }
                }
            }
        }
        return 0;
    }

    static const luaL_Reg command_functions[] = {
        {"registerCommand", register_command},
        {"executeCommand", execute_command},
        {"loadSettings", load_settings},
        {nullptr, nullptr}
    };

    void set_command_table(lua_State *state) noexcept {
        create_functions_table(state, "command", command_functions);
    }
}
