// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__LUA__API__V2__TYPES__ENGINE__ITEM_HPP
#define BALLTZE__LUA__API__V2__TYPES__ENGINE__ITEM_HPP

#include <optional>
#include <lua.hpp>
#include <impl/object/object_item.h>

namespace Balltze::Lua::Api::V2 {
    /**
     * Define the Item type in Lua.
     *
     * @param state Lua state
     */
    void define_engine_item_types(lua_State *state) noexcept;
}

#endif
