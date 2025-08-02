// SPDX-License-Identifier: GPL-3.0-only

#include <lua.hpp>
#include <balltze/legacy_api/engine.hpp>
#include <balltze/legacy_api/engine/saved_games.hpp>
#include "../../types/ringworld_saved_games.hpp"
#include "../../../../helpers/plugin.hpp"

namespace Balltze::Lua::Api::V1 {
    static int saved_games_reload_player_profile(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 0) {
            LegacyApi::Engine::saved_games_reload_player_profile();
            return 0;
        }
        else {
            return luaL_error(state, "Invalid number of arguments in function Engine.savedGames.reloadPlayerProfile.");
        }
    }

    static int saved_games_read_player_profile(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 0) {
            LegacyApi::Engine::saved_games_read_player_profile();
            return 0;
        }
        else {
            return luaL_error(state, "Invalid number of arguments in function Engine.savedGames.readPlayerProfile.");
        }
    }

    static int saved_games_save_player_profile(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 0) {
            LegacyApi::Engine::saved_games_save_player_profile();
            return 0;
        }
        else {
            return luaL_error(state, "Invalid number of arguments in function Engine.savedGames.savePlayerProfile.");
        }
    }
    
    static int saved_games_get_player_profile(lua_State *state) {
        int args = lua_gettop(state);
        if(args == 0) {
            PlayerProfile *profile = ::saved_games_get_player_profile(0);
            push_ringworld_player_profile(state, profile);
            return 1;
        }
        else {
            return luaL_error(state, "Invalid number of arguments in function Engine.savedGames.getPlayerProfile.");
        }
    }

    static const luaL_Reg engine_saved_games_functions[] = {
        {"reloadPlayerProfile", saved_games_reload_player_profile},
        {"readPlayerProfile", saved_games_read_player_profile},
        {"savePlayerProfile", saved_games_save_player_profile},
        {"getPlayerProfile", saved_games_get_player_profile},
        {nullptr, nullptr}
    };

    void set_engine_saved_games_functions(lua_State *state) noexcept {
        push_plugin_functions_table(state, "savedGames", -1, engine_saved_games_functions);
    }        
}
