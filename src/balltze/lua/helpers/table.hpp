// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__LUA__API__HELPERS__TABLE_HPP
#define BALLTZE__LUA__API__HELPERS__TABLE_HPP

#include <optional>
#include <lua.hpp>

namespace Balltze::Lua {
    /**
     * Inserts an element into a table at the given index or at the end of the table.
     * @param state The Lua state.
     * @param table The index of the table.
     * @param val The index of the value to insert.
     * @param pos The position to insert the value at. If not provided, the value is inserted at the end of the table.
     */
    void table_insert(lua_State *state, int table, int val, std::optional<int> pos = {});
    
    /**
     * Removes an element from a table at the given index.
     * @param state The Lua state.
     * @param table The index of the table.
     * @param pos The position to remove the value at.
     */
    void table_remove(lua_State *state, int table, int pos);

    /**
     * Removes all elements from a table that are equal to the value at the given index.
     * @param state The Lua state.
     * @param table The index of the table.
     * @param val The index of the value to compare against.
     */
    void table_remove_if_equals(lua_State *state, int table, int val);
}

#endif
