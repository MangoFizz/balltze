// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__LUA__API__HELPERS__FUNCTION_TABLE_HPP
#define BALLTZE__LUA__API__HELPERS__FUNCTION_TABLE_HPP

#include <lua.hpp>

namespace Balltze::Lua {
    /**
     * Pushes a Lua functions table onto the stack and sets the functions
     * from the provided array of luaL_Reg.
     * 
     * @param state The Lua state.
     * @param table_idx The index of the table to set the functions in.
     * @param funcs The array of functions to set in the table.
     */
    inline void set_functions_reg_array(lua_State *state, int table_idx, const luaL_Reg *funcs) noexcept {
        lua_pushvalue(state, lua_absindex(state, table_idx));
        luaL_setfuncs(state, funcs, 0);
        lua_pop(state, 1);
    }
}

#endif
