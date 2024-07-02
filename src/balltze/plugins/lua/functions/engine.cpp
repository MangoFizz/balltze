// SPDX-License-Identifier: GPL-3.0-only

#include <lua.hpp>
#include <balltze/api.hpp>

namespace Balltze::Plugins::Lua {
    void set_engine_core_functions(lua_State *state) noexcept;
    void set_engine_map_functions(lua_State *state) noexcept;
    void set_engine_netgame_functions(lua_State *state) noexcept;
    void set_engine_rasterizer_functions(lua_State *state) noexcept;
    void set_engine_tag_functions(lua_State *state) noexcept;
    void set_engine_game_state_functions(lua_State *state) noexcept;
    void set_engine_user_interface_functions(lua_State *state) noexcept;
    void set_engine_hsc_functions(lua_State *state) noexcept;

    void set_engine_table(lua_State *state) noexcept {
        lua_newtable(state);
        set_engine_core_functions(state);
        set_engine_map_functions(state);
        set_engine_tag_functions(state);
        set_engine_netgame_functions(state);
        set_engine_game_state_functions(state);
        set_engine_hsc_functions(state);

        if(get_balltze_side() == BALLTZE_SIDE_CLIENT) {
            set_engine_rasterizer_functions(state);
            set_engine_user_interface_functions(state);
        }
        
        lua_setglobal(state, "Engine");
    }
}
