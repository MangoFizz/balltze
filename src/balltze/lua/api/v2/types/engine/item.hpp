// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__LUA__API__V2__TYPES__ENGINE__ITEM_HPP
#define BALLTZE__LUA__API__V2__TYPES__ENGINE__ITEM_HPP

#include <optional>
#include <lua.hpp>
#include <impl/object/object_item.h>

namespace Balltze::Lua::Api::V2 {
    /**
     * Push a EquipmentObject to the Lua stack.
     * 
     * @param state Lua state
     * @param object The EquipmentObject to push
     * @param readonly If true, the object will be read-only in Lua
     */
    void push_equipment_object(lua_State *state, const EquipmentObject &object, bool readonly = false) noexcept;

    /**
     * Push a GarbageObject to the Lua stack.
     * 
     * @param state Lua state
     * @param object The GarbageObject to push
     * @param readonly If true, the object will be read-only in Lua
     */
    void push_garbage_object(lua_State *state, const GarbageObject &object, bool readonly = false) noexcept;

    /**
     * Push a ProjectileObject to the Lua stack.
     * 
     * @param state Lua state
     * @param object The ProjectileObject to push
     * @param readonly If true, the object will be read-only in Lua
     */
    void push_projectile_object(lua_State *state, const ProjectileObject &object, bool readonly = false) noexcept;

    /**
     * Define the Item type in Lua.
     *
     * @param state Lua state
     */
    void define_engine_item_types(lua_State *state) noexcept;
}

#endif
