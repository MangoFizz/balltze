// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__LUA__API__HELPERS__FUNCTION_TABLE_HPP
#define BALLTZE__LUA__API__HELPERS__FUNCTION_TABLE_HPP

#include <lua.hpp>

namespace Balltze::Lua {
    /**
     * Creates a Lua functions table with the given name and 
     * functions in the table at the top of the stack.
     * 
     * @param state The Lua state.
     * @param name The name of the table to create.
     * @param functions The functions to add to the table.
     */
    inline void create_functions_table(lua_State *state, const char *name, const luaL_Reg *functions) noexcept {
        lua_pushstring(state, name);
        luaL_newlibtable(state, functions);
        lua_pushlightuserdata(state, state);
        luaL_setfuncs(state, functions, 1);
        lua_settable(state, -3);
    }

    /**
     * Pushes a Lua function onto the stack that can be used as a plugin function.
     * The function will have the Lua state as its first upvalue.
     * 
     * @param state The Lua state.
     * @param func The C function to push.
     * @param upvalue_count The number of upvalues to pass to the function.
     */
    inline void push_plugin_function(lua_State *state, lua_CFunction func, int upvalue_count = 0) noexcept {
        lua_pushlightuserdata(state, state);
        lua_pushcclosure(state, func, upvalue_count + 1);
    }

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
