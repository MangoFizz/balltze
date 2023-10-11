// SPDX-License-Identifier: GPL-3.0-only

#include <lua.hpp>
#include <balltze/engine.hpp>
#include "../../../logger.hpp"
#include "../../plugin.hpp"
#include "../../loader.hpp"
#include "../helpers.hpp"

namespace Balltze::Plugins {
    static int lua_engine_get_server_type(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 0) {
                auto server_type = Engine::get_server_type();
                auto server_type_string = server_type_to_string(server_type);
                lua_pushstring(state, server_type_string.c_str());
                return 1;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function engine.get_server_type.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static int lua_engine_get_server_gametype(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 0) {
                auto server_gametype = Engine::get_current_game_type();
                auto server_gametype_string = server_game_type_to_string(server_gametype);
                if(server_gametype_string == "none") {
                    lua_pushnil(state);
                }
                else {
                    lua_pushstring(state, server_gametype_string.c_str());
                }
                return 1;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function engine.get_server_gametype.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static int lua_engine_current_game_is_team(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 0) {
                bool is_team_game = Engine::current_game_is_team();
                lua_pushboolean(state, is_team_game);
                return 1;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function engine.current_game_is_team.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static const luaL_Reg engine_multiplayer_functions[] = {
        {"getServerType", lua_engine_get_server_type},
        {"getServerGametype", lua_engine_get_server_gametype},
        {"currentGameIsTeam", lua_engine_current_game_is_team},
        {nullptr, nullptr}
    };

    void set_engine_multiplayer_functions(lua_State *state) noexcept {
        luaL_newlibtable(state, engine_multiplayer_functions);
        luaL_setfuncs(state, engine_multiplayer_functions, 0);
        lua_setfield(state, -2, "multiplayer");
    }
}