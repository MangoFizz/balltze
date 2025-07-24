// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__LUA__API__V2__TYPES__ENGINE__UNIT_HPP
#define BALLTZE__LUA__API__V2__TYPES__ENGINE__UNIT_HPP

#include <optional>
#include <lua.hpp>
#include <impl/unit/unit.h>

namespace Balltze::Lua::Api::V2 {
    /**
     * Push a BipedObject to the Lua stack.
     *
     * @param state Lua state
     * @param biped The BipedObject to push
     * @param readonly If true, the biped will be read-only in Lua
     */
    void push_biped_object(lua_State *state, const BipedObject &biped, bool readonly = false) noexcept;

    /**
     * Push a VehicleObject to the Lua stack.
     * 
     * @param state Lua state
     * @param vehicle The VehicleObject to push
     * @param readonly If true, the vehicle will be read-only in Lua
     */
    void push_vehicle_object(lua_State *state, const VehicleObject &vehicle, bool readonly = false) noexcept;

    /**
     * Define the Unit type in Lua.
     *
     * @param state Lua state
     */
    void define_engine_unit_types(lua_State *state) noexcept;
}

#endif
