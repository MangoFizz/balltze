// SPDX-License-Identifier: GPL-3.0-only

/**
 * Table manipulation helper functions based on Lua's table library functions
 * https://www.lua.org/source/5.3/ltablib.c.html
 */

#include <stdexcept>
#include "table.hpp"

/*
 * Operations that an object must define to mimic a table
 * (some functions only need some of them)
 */
#define TAB_R 1                /* read */
#define TAB_W 2                /* write */
#define TAB_L 4                /* length */
#define TAB_RW (TAB_R | TAB_W) /* read/write */

#define aux_getn(L, n, w) (checktab(L, n, (w) | TAB_L), luaL_len(L, n))

namespace Balltze::Lua {
    static int checkfield (lua_State *L, const char *key, int n) {
        lua_pushstring(L, key);
        return (lua_rawget(L, -n) != LUA_TNIL);
    }

    /*
    ** Check that 'arg' either is a table or can behave like one (that is,
    ** has a metatable with the required metamethods)
    */
    static void checktab(lua_State *L, int arg, int what) {
        if (lua_type(L, arg) != LUA_TTABLE) { /* is it not a table? */
            int n = 1; /* number of elements to pop */
            if (lua_getmetatable(L, arg) && /* must have metatable */
                (!(what & TAB_R) || checkfield(L, "__index", ++n)) &&
                (!(what & TAB_W) || checkfield(L, "__newindex", ++n)) &&
                (!(what & TAB_L) || checkfield(L, "__len", ++n))) {
                lua_pop(L, n); /* pop metatable and tested metamethods */
            }
            else
                luaL_checktype(L, arg, LUA_TTABLE); /* force an error */
        }
    }

    void table_insert(lua_State *state, int table, int val, std::optional<int> pos) {
        lua_Integer t = lua_absindex(state, table);
        lua_Integer v = lua_absindex(state, val);
        lua_Integer e = aux_getn(state, table, TAB_RW) + 1; /* first empty element */
        if(pos) {
            lua_Integer i;
            if(*pos < 1 || *pos > e) {
                throw std::invalid_argument("position out of bounds");
            }
            for (i = e; i > pos; i--) { /* move up elements */
                lua_geti(state, t, i - 1);
                lua_seti(state, t, i); /* t[i] = t[i - 1] */
            }
        }
        else {
            pos = e; /* insert new element at the end */
        }

        lua_pushvalue(state, v);
        lua_seti(state, t, *pos); /* t[pos] = v */
        return;
    }

    void table_remove(lua_State *state, int table, int pos) {
        lua_Integer t = lua_absindex(state, table);
        lua_Integer size = aux_getn(state, t, TAB_RW);
        if (pos > size) /* validate 'pos' if given */
            throw std::invalid_argument("position out of bounds");
        lua_geti(state, t, pos); /* result = t[pos] */
        for (; pos < size; pos++) {
            lua_geti(state, t, pos + 1);
            lua_seti(state, t, pos); /* t[pos] = t[pos + 1] */
        }
        lua_pushnil(state);
        lua_seti(state, t, pos); /* t[pos] = nil */
        return;
    }

    void table_remove_if_equals(lua_State *state, int table, int val) {
        lua_Integer t = lua_absindex(state, table);
        lua_Integer v = lua_absindex(state, val);
        lua_Integer size = aux_getn(state, t, TAB_RW);
        lua_Integer pos = 1;
        while(pos <= size) {
            lua_rawgeti(state, t, pos);
            if(lua_rawequal(state, -1, v)) {
                lua_pop(state, 1);
                table_remove(state, t, pos);
            }
            else {
                lua_pop(state, 1);
                pos++;
            }
        }
        return;
    }
}
