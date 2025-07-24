// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__LUA__API__V2__TYPES__ENGINE__DEVICE_HPP
#define BALLTZE__LUA__API__V2__TYPES__ENGINE__DEVICE_HPP

#include <optional>
#include <lua.hpp>
#include <impl/object/object_device.h>

namespace Balltze::Lua::Api::V2 {
    /**
     * Push a DeviceObject to the Lua stack.
     *
     * @param state Lua state
     * @param device The DeviceObject to push
     * @param readonly If true, the device will be read-only in Lua
     */
    void push_device_object(lua_State *state, const DeviceObject &device, bool readonly = false) noexcept;

    /**
     * Define the Device type in Lua.
     *
     * @param state Lua state
     */
    void define_engine_device_types(lua_State *state) noexcept;
}

#endif
