// SPDX-License-Identifier: GPL-3.0-only

#include "metaobject.hpp"

namespace Balltze::Plugins {
    int lua_meta_object_read_only__newindex(lua_State *state) noexcept {
        return luaL_error(state, "attempt to modify a read-only metaobject");
    }

    int lua_meta_object__eq(lua_State *state) noexcept {
        // this may never happen due getequalhandler function
        if(!lua_istable(state, 1) || !lua_istable(state, 2)) { 
            lua_pushboolean(state, false);
            return 1;
        }

        lua_getfield(state, 1, "_type");
        if(!lua_isinteger(state, -1)) {
            lua_pop(state, 1);
            lua_pushboolean(state, false);
            return 1;
        }
        lua_Integer type1 = lua_tointeger(state, -1);
        lua_pop(state, 1);

        lua_getfield(state, 1, "_data");
        if(!lua_islightuserdata(state, -1)) {
            lua_pop(state, 1);
            lua_pushboolean(state, false);
            return 1;
        }
        void *data1 = lua_touserdata(state, -1);
        lua_pop(state, 1);

        lua_getfield(state, 2, "_type");
        if(!lua_isinteger(state, -1)) {
            lua_pop(state, 1);
            lua_pushboolean(state, false);
            return 1;
        }
        lua_Integer type2 = lua_tointeger(state, -1);
        lua_pop(state, 1);

        lua_getfield(state, 2, "_data");
        if(!lua_islightuserdata(state, -1)) {
            lua_pop(state, 1);
            lua_pushboolean(state, false);
            return 1;
        }
        void *data2 = lua_touserdata(state, -1);
        lua_pop(state, 1);

        lua_pushboolean(state, type1 == type2 && data1 == data2);
        return 1;
    }
}
