// SPDX-License-Identifier: GPL-3.0-only

#include <lua.hpp>
#include <impl/player/player.h>
#include "../../../../helpers/function_table.hpp"
#include "../../types.hpp"

namespace Balltze::Lua::Api::V2 {
    static int lua_engine_get_player(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 0 || args == 1) {
            auto *player_table = player_get_table();
            Player *player = nullptr;
            if(args == 1) {
                auto player_handle = get_player_handle(state, 1);
                if(!player_handle || HANDLE_IS_NULL(*player_handle)) {
                    return luaL_error(state, "invalid player handle in function Engine.player.getPlayer");
                }
                player = reinterpret_cast<Player *>(table_get_element(player_table, *player_handle));
            }
            else {
                PlayersGlobals *globals = player_get_globals();
                player = reinterpret_cast<Player *>(table_get_element(player_table, globals->local_players[0]));
            }
            if(player) {
                push_player(state, *player);
            }
            else {
                lua_pushnil(state);
            }
            return 1;
        }
        else {
            return luaL_error(state, "invalid number of arguments in function Engine.player.getPlayer");
        }
        return 0;
    }

    static const luaL_Reg engine_player_functions[] = {
        {"getPlayer", lua_engine_get_player},
        {nullptr, nullptr}
    };

    void set_engine_player_functions(lua_State *state, int table_idx) noexcept {
        int abs_idx = lua_absindex(state, table_idx);
        lua_newtable(state);
        set_functions_reg_array(state, -1, engine_player_functions);
        lua_setfield(state, abs_idx, "player");
    }
}
