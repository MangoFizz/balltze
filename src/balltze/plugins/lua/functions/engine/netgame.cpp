// SPDX-License-Identifier: GPL-3.0-only

#include <lua.hpp>
#include <balltze/engine.hpp>
#include "../../../../logger.hpp"
#include "../../../plugin.hpp"
#include "../../../loader.hpp"
#include "../../libraries.hpp"
#include "../../types.hpp"

namespace Balltze::Plugins::Lua {
    static std::string network_game_server_type_to_string(Engine::NetworkGameServerType type) {
        switch(type) {
            case Engine::NETWORK_GAME_SERVER_NONE:
                return "none";
            case Engine::NETWORK_GAME_SERVER_DEDICATED:
                return "dedicated";
            case Engine::NETWORK_GAME_SERVER_LOCAL:
                return "local";
            default:
                return "unknown";
        }
    }

    static int engine_get_server_type(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 0) {
                auto server_type = Engine::network_game_get_server_type();
                auto server_type_string = network_game_server_type_to_string(server_type);
                lua_pushstring(state, server_type_string.c_str());
                return 1;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function Engine.netgame.getServerType.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static std::string network_game_server_game_type_to_string(Engine::NetworkGameType gametype) {
        switch(gametype) {
            case Engine::NETWORK_GAMETYPE_CTF:
                return "ctf";
            case Engine::NETWORK_GAMETYPE_SLAYER:
                return "slayer";
            case Engine::NETWORK_GAMETYPE_ODDBALL:
                return "oddball";
            case Engine::NETWORK_GAMETYPE_KING:
                return "king";
            case Engine::NETWORK_GAMETYPE_RACE:
                return "race";
            default:
                return "none";
        }
    }

    static int engine_get_server_gametype(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 0) {
                auto server_gametype = Engine::network_game_get_current_game_type();
                auto server_gametype_string = network_game_server_game_type_to_string(server_gametype);
                if(server_gametype_string == "none") {
                    lua_pushnil(state);
                }
                else {
                    lua_pushstring(state, server_gametype_string.c_str());
                }
                return 1;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function Engine.netgame.getServerGametype.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static int engine_current_game_is_team(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 0) {
                bool is_team_game = Engine::network_game_current_game_is_team();
                lua_pushboolean(state, is_team_game);
                return 1;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function Engine.netgame.currentGameIsTeam.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static const luaL_Reg engine_netgame_functions[] = {
        {"getServerType", engine_get_server_type},
        {"getServerGametype", engine_get_server_gametype},
        {"currentGameIsTeam", engine_current_game_is_team},
        {nullptr, nullptr}
    };

    void set_engine_netgame_functions(lua_State *state) noexcept {
        lua_newtable(state);
        luaL_setfuncs(state, engine_netgame_functions, 0);
        lua_setfield(state, -2, "netgame");
    }
}