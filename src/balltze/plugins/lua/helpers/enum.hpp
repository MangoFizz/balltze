// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__PLUGINS__LUA__HELPERS__ENUM_HPP
#define BALLTZE__PLUGINS__LUA__HELPERS__ENUM_HPP

#include <lua.hpp>

namespace Balltze::Plugins::Lua {
    inline void publish_enum(lua_State *state, const char *global_table, const char *table, const char *enum_name, int index) {
        lua_pushvalue(state, index);

        lua_getglobal(state, global_table);
        if(lua_isnil(state, -1)) {
            lua_pop(state, 1);
            luaL_error(state, "Global table %s does not exist", global_table);
            return;
        }

        lua_getfield(state, -1, table);
        if(lua_isnil(state, -1)) {
            lua_pop(state, 2);
            luaL_error(state, "Table %s does not exist", table);
            return;
        }

        lua_pushvalue(state, -3);
        lua_setfield(state, -2, enum_name);
        lua_pop(state, 3);
    }
}

#endif
