// SPDX-License-Identifier: GPL-3.0-only

#include <lua.hpp>
#include <balltze/legacy_api/engine.hpp>
#include "../../../../helpers/function_table.hpp"
#include "../../types.hpp"

namespace Balltze::Lua::Api::V1 {
    static std::string network_game_server_type_to_string(LegacyApi::Engine::NetworkGameServerType type) {
        switch(type) {
            case LegacyApi::Engine::NETWORK_GAME_SERVER_NONE:
                return "none";
            case LegacyApi::Engine::NETWORK_GAME_SERVER_DEDICATED:
                return "dedicated";
            case LegacyApi::Engine::NETWORK_GAME_SERVER_LOCAL:
                return "local";
            default:
                return "unknown";
        }
    }

    static int engine_get_server_type(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 0) {
            auto server_type = LegacyApi::Engine::network_game_get_server_type();
            auto server_type_string = network_game_server_type_to_string(server_type);
            lua_pushstring(state, server_type_string.c_str());
            return 1;
        }
        else {
            return luaL_error(state, "Invalid number of arguments in function Engine.netgame.getServerType.");
        }
    }

    static std::string network_game_server_game_type_to_string(LegacyApi::Engine::NetworkGameType gametype) {
        switch(gametype) {
            case LegacyApi::Engine::NETWORK_GAMETYPE_CTF:
                return "ctf";
            case LegacyApi::Engine::NETWORK_GAMETYPE_SLAYER:
                return "slayer";
            case LegacyApi::Engine::NETWORK_GAMETYPE_ODDBALL:
                return "oddball";
            case LegacyApi::Engine::NETWORK_GAMETYPE_KING:
                return "king";
            case LegacyApi::Engine::NETWORK_GAMETYPE_RACE:
                return "race";
            default:
                return "none";
        }
    }

    static int engine_get_server_gametype(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 0) {
            auto server_gametype = LegacyApi::Engine::network_game_get_current_game_type();
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

    static int engine_current_game_is_team(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 0) {
            bool is_team_game = LegacyApi::Engine::network_game_current_game_is_team();
            lua_pushboolean(state, is_team_game);
            return 1;
        }
        else {
            return luaL_error(state, "Invalid number of arguments in function Engine.netgame.currentGameIsTeam.");
        }
    }

    static const luaL_Reg engine_netgame_functions[] = {
        {"getServerType", engine_get_server_type},
        {"getServerGametype", engine_get_server_gametype},
        {"currentGameIsTeam", engine_current_game_is_team},
        {nullptr, nullptr}
    };

    void set_engine_netgame_functions(lua_State *state) noexcept {
        create_functions_table(state, "netgame", engine_netgame_functions);
    }
}