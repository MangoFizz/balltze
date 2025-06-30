// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__LUA__API__HELPERS__REGISTRY_HPP
#define BALLTZE__LUA__API__HELPERS__REGISTRY_HPP

#include <lua.hpp>

namespace Balltze::Lua {
    /**
     * Get a table from Lua Balltze registry table or create it if it doesn't exist.
     * Table is returned on top of the stack.
     * @param state Lua state
     * @param name  Table name
     */
    void get_or_create_registry_table(lua_State *state, const char *name);
}

#endif
