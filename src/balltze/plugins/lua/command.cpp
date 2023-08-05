// SPDX-License-Identifier: GPL-3.0-only

#include <vector>
#include "../../logger.hpp"
#include "../loader.hpp"
#include "helpers.hpp"
#include "command.hpp"

namespace Balltze {
    extern std::vector<std::shared_ptr<Command>> commands;
}

namespace Balltze::Plugins {
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

    CommandResult LuaCommand::call(std::size_t arg_count, const char **args) const noexcept {
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
                        return COMMAND_RESULT_SUCCESS;
                    }
                }
                else {
                    logger.warning("Lua command {} returned nil", *m_full_name);
                }
            }
            else {
                logger.debug("Lua command {} failed: {}", *m_full_name, lua_tostring(state, -1));
                lua_pop(state, 1);
            }
        }
        else {
            logger.warning("Lua command {} not found", *m_full_name);
        }
        return COMMAND_RESULT_FAILED_ERROR;
    }

    void LuaCommand::register_command(LuaPlugin *plugin) {
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
        commands.emplace_back(std::make_shared<LuaCommand>(*this));
    }

    void remove_plugin_commands(LuaPlugin *plugin) noexcept {
        logger.debug("Removing commands from plugin {}", plugin->name());
        for(auto it = commands.begin(); it != commands.end();) {
            if((*it)->plugin() == reinterpret_cast<PluginHandle>(plugin)) {
                it = commands.erase(it);
            }
            else {
                it++;
            }
        }   
    }

    static int lua_command_register_command(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 9 || args == 10) {
                std::size_t index_disp = 0;
                auto *name = luaL_checkstring(state, index_disp + 1);
                auto *category = luaL_checkstring(state, index_disp + 2);
                auto *help = luaL_checkstring(state, index_disp + 3);
                std::optional<std::string> params_help = std::nullopt;
                if(args == 10) {
                    params_help = luaL_checkstring(state, index_disp + 4);
                    index_disp++;
                }
                bool autosave;
                if(lua_isboolean(state, index_disp + 5)) {
                    autosave = lua_toboolean(state, index_disp + 5);
                }
                else {
                    return luaL_error(state, "Argument 5 of register_command must be a boolean.");
                }
                std::size_t min_args = luaL_checkinteger(state, index_disp + 6);
                std::size_t max_args = luaL_checkinteger(state, index_disp + 7);
                bool can_call_from_console;
                if(lua_isboolean(state, index_disp + 8)) {
                    can_call_from_console = lua_toboolean(state, index_disp + 8);
                }
                else {
                    return luaL_error(state, "Argument 8 of register_command must be a boolean.");
                }
                bool is_public;
                if(lua_isboolean(state, index_disp + 9)) {
                    is_public = lua_toboolean(state, index_disp + 9);
                }
                else {
                    return luaL_error(state, "Argument 9 of register_command must be a boolean.");
                }
                if(lua_isfunction(state, index_disp + 4)) {
                    lua_pushvalue(state, index_disp + 4);
                }
                else {
                    return luaL_error(state, "Argument 4 of register_command must be a function.");
                }

                LuaCommand command(name, category, help, params_help, autosave, min_args, max_args, can_call_from_console, is_public);
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
                return luaL_error(state, "Invalid number of arguments in function command.register_command.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static const luaL_Reg command_functions[] = {
        {"register_command", lua_command_register_command},
        {nullptr, nullptr}
    };

    void lua_set_command_table(lua_State *state) noexcept {
        lua_create_functions_table(state, "command", command_functions);
    }
}
