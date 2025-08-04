// SPDX-License-Identifier: GPL-3.0-only

#include <lua.hpp>
#include "../../../../logger.hpp"
#include "../../../../plugins/plugin.hpp"
#include "../../../../plugins/loader.hpp"
#include "../../../helpers/plugin.hpp"
#include "../../../libraries/lfmt.hpp"

namespace Balltze::Lua::Api::V2 {
    #define LUA_LOGGER_FUNCTION(name) \
        static int lua_logger_##name(lua_State *state) { \
            auto *plugin = get_plugin(state); \
            if(plugin) { \
                int args = lua_gettop(state); \
                if(args >= 1) { \
                    /* Format the message using lfmt */ \
                    std::string message; \
                    if(args > 1) { \
                        Lformat(state); \
                        message = luaL_checkstring(state, -1); \
                        lua_pop(state, 1); \
                        lua_insert(state, 1); \
                    } \
                    else { \
                        message = luaL_checkstring(state, 1); \
                    } \
                    Logger *llogger = plugin->plugin_logger(); \
                    llogger->name(message); \
                } \
                else { \
                    logger.warning("Invalid number of arguments for Balltze.logger." #name "."); \
                    return luaL_error(state, "Invalid number of arguments for logger print function."); \
                } \
            } \
            else { \
                logger.warning("Could not get plugin for lua state."); \
                return luaL_error(state, "Unknown plugin."); \
            } \
            return 0; \
        }

    LUA_LOGGER_FUNCTION(debug)
    LUA_LOGGER_FUNCTION(info)
    LUA_LOGGER_FUNCTION(warning)
    LUA_LOGGER_FUNCTION(error)
    LUA_LOGGER_FUNCTION(fatal)

    static int lua_logger_mute_ingame(lua_State *state) noexcept {
        auto *plugin = get_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 0 || args == 1) {
                Logger *llogger = plugin->plugin_logger();
                if(args == 0) {
                    bool mute = lua_toboolean(state, 2);
                    llogger->mute_ingame(mute);
                    return 0;
                }
                else {
                    lua_pushboolean(state, llogger->mute_ingame());
                    return 1;
                }
            }
            else {
                logger.warning("Invalid number of arguments for Balltze.logger.muteIngame.");
                return luaL_error(state, "Invalid number of arguments for Balltze.logger.muteIngame.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
        return 0;
    }

    static int lua_logger_mute_debug(lua_State *state) noexcept {
        auto *plugin = get_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 0 || args == 1) {
                Logger *llogger = plugin->plugin_logger();
                if(args == 1) {
                    /* Get the mute value */
                    bool mute = lua_toboolean(state, 2);
                    llogger->mute_debug(mute);
                    return 0;
                }
                else {
                    lua_pushboolean(state, llogger->mute_debug());
                    return 1;
                }
            }
            else {
                logger.warning("Invalid number of arguments for Balltze.logger.muteDebug.");
                return luaL_error(state, "Invalid number of arguments for Balltze.logger.muteDebug.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
        return 0;
    }

    static const luaL_Reg logger_functions[] = {
        {"debug", lua_logger_debug},
        {"info", lua_logger_info},
        {"warning", lua_logger_warning},
        {"error", lua_logger_error},
        {"fatal", lua_logger_fatal},
        {"muteConsole", lua_logger_mute_ingame},
        {"muteDebug", lua_logger_mute_debug},
        {nullptr, nullptr}
    };

    void set_up_plugin_logger(lua_State *state) noexcept {
        push_plugin_functions_table(state, "logger", -1, logger_functions);
    }
}
