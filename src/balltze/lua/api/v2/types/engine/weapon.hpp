// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__LUA__API__V2__TYPES__ENGINE__WEAPON_HPP
#define BALLTZE__LUA__API__V2__TYPES__ENGINE__WEAPON_HPP

#include <optional>
#include <lua.hpp>
#include <impl/weapon/weapon.h>

namespace Balltze::Lua::Api::V2 {
    /**
     * Push a WeaponObject to the Lua stack.
     *
     * @param state Lua state
     * @param weapon The WeaponObject to push
     * @param readonly If true, the weapon will be read-only in Lua
     */
    void push_weapon_object(lua_State *state, const WeaponObject &weapon, bool readonly = false) noexcept;

    /**
     * Define the Weapon type in Lua.
     *
     * @param state Lua state
     */
    void define_engine_weapon_types(lua_State *state) noexcept;
}

#endif
