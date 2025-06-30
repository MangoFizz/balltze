// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__LUA__API__HELPERS__FUNCTION_TABLE_HPP
#define BALLTZE__LUA__API__HELPERS__FUNCTION_TABLE_HPP

#include <lua.hpp>

namespace Balltze::Lua {
    inline void create_functions_table(lua_State *state, const char *name, const luaL_Reg *functions) noexcept {
        lua_pushstring(state, name);
        luaL_newlibtable(state, functions);
        lua_pushlightuserdata(state, state);
        luaL_setfuncs(state, functions, 1);
        lua_settable(state, -3);
    }

    inline void push_plugin_function(lua_State *state, lua_CFunction func) {
        lua_pushlightuserdata(state, state);
        lua_pushcclosure(state, func, 1);
    }
}

#endif
