// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__LUA__API__V2__TYPES__ENGINE__OBJECT_HPP
#define BALLTZE__LUA__API__V2__TYPES__ENGINE__OBJECT_HPP

#include <optional>
#include <lua.hpp>
#include <impl/object/object.h>

namespace Balltze::Lua::Api::V2 {
    /**
     * Push a ObjectHandle to the Lua stack.
     * 
     * @param state Lua state
     * @param handle The ObjectHandle to push
     * @param readonly If true, the handle will be read-only in Lua
     */
    void push_object_handle(lua_State *state, const ObjectHandle &handle, bool readonly = false) noexcept;

    /**
     * Get a ObjectHandle from the Lua stack.
     * 
     * @param state Lua state
     * @param idx The index of the value on the Lua stack
     * @return An optional ObjectHandle, empty if the value is not a valid handle
     */
    std::optional<ObjectHandle> get_object_handle(lua_State *state, int idx) noexcept;
    
    /**
     * Push a DynamicObjectBase to the Lua stack.
     * 
     * @param state Lua state
     * @param object The DynamicObjectBase to push
     * @param readonly If true, the object will be read-only in Lua
     */
    void push_dynamic_object_base(lua_State *state, const DynamicObjectBase &object, bool readonly) noexcept;

    /**
     * Define the Object type in Lua.
     *
     * @param state Lua state
     */
    void define_engine_object_types(lua_State *state) noexcept;
}

#endif
