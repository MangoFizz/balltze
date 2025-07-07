// SPDX-License-Identifier: GPL-3.0-only

#include <lua.hpp>
#include <balltze/api.hpp>

namespace Balltze::Lua::Api::V2 {
    void set_ringworld_tag_functions(lua_State *state) noexcept;

    void set_ringworld_table(lua_State *state) noexcept {
        lua_newtable(state);
        set_ringworld_tag_functions(state);
        lua_setglobal(state, "Ringworld");
    }
}
