// SPDX-License-Identifier: GPL-3.0-only

#include <lua.hpp>

namespace Balltze::Lua::Api::V2 {
    void set_engine_tag_functions(lua_State *state, int table_idx) noexcept;
    void set_engine_console_functions(lua_State *state, int table_idx) noexcept;
    void set_engine_cache_file_functions(lua_State *state, int table_idx) noexcept;
    void set_engine_game_functions(lua_State *state, int table_idx) noexcept;
    void set_engine_script_functions(lua_State *state, int table_idx) noexcept;
    void set_engine_object_functions(lua_State *state, int table_idx) noexcept;
    void set_engine_player_functions(lua_State *state, int table_idx) noexcept;
    void set_engine_ui_widget_functions(lua_State *state, int table_idx) noexcept;

    void set_engine_table(lua_State *state) noexcept {
        lua_newtable(state);
        set_engine_tag_functions(state, -1);
        set_engine_console_functions(state, -1);
        set_engine_cache_file_functions(state, -1);
        set_engine_game_functions(state, -1);
        set_engine_script_functions(state, -1);
        set_engine_object_functions(state, -1);
        set_engine_player_functions(state, -1);
        set_engine_ui_widget_functions(state, -1);
        lua_setglobal(state, "Engine");
    }
}
