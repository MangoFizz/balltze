// SPDX-License-Identifier: GPL-3.0-only

#include <lua.hpp>
#include <balltze/legacy_api/engine/core.hpp>
#include <impl/game/game_engine.h>
#include "../../../../helpers/function_table.hpp"
#include "../../types.hpp"

namespace Balltze::Lua::Api::V2 {
    static int lua_engine_get_tick_count(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 0) {
            lua_pushinteger(state, LegacyApi::Engine::get_tick_count());
            return 1;
        }
        else {
            return luaL_error(state, "Invalid number of arguments in function Engine.game.getTickCount.");
        }
    }

    static int lua_engine_get_game_engine_type(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 0) {
            auto *game_engine = game_engine_get_current_interface();
            if(game_engine) {
                push_engine_game_engine_type(state, game_engine->type);
                return 1;
            }
            else {
                lua_pushnil(state);
                return 1;
            }
        }
        else {
            return luaL_error(state, "Invalid number of arguments in function Engine.game.getGameEngineType.");
        }
    }

    static const luaL_Reg engine_game_functions[] = {
        {"getTickCount", lua_engine_get_tick_count},
        {"getGameEngineType", lua_engine_get_game_engine_type},
        {nullptr, nullptr}
    };

    void set_engine_game_functions(lua_State *state, int table_idx) noexcept {
        int abs_idx = lua_absindex(state, table_idx);
        lua_newtable(state);
        set_functions_reg_array(state, -1, engine_game_functions);
        lua_setfield(state, abs_idx, "game");
    }
}
