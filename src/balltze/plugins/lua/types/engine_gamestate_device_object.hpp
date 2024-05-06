// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__PLUGINS__LUA__TYPES__ENGINE_GAMESTATE_DEVICE_OBJECT_HPP
#define BALLTZE__PLUGINS__LUA__TYPES__ENGINE_GAMESTATE_DEVICE_OBJECT_HPP

#include <lua.hpp>
#include <balltze/engine.hpp>
#include "../helpers/luacstruct.hpp"

namespace Balltze::Plugins::Lua {
    using EngineDeviceObjectState = Engine::DeviceObjectState;
    using EngineDeviceObject = Engine::DeviceObject;
    using EngineDeviceMachineObjectFlags = Engine::DeviceMachineObjectFlags;
    using EngineDeviceMachineObject = Engine::DeviceMachineObject;
    using EngineDeviceControlObjectFlags = Engine::DeviceControlObjectFlags;
    using EngineDeviceControlObject = Engine::DeviceControlObject;
    using EngineDeviceLightFixtureObject = Engine::DeviceLightFixtureObject;

    void lua_define_engine_device_object_state_struct(lua_State *state) noexcept;
    void lua_push_engine_device_object_state(lua_State *state, EngineDeviceObjectState *object_state) noexcept;

    void lua_define_engine_device_object_struct(lua_State *state) noexcept;
    void lua_push_engine_device_object(lua_State *state, EngineDeviceObject *object) noexcept;

    void lua_define_engine_device_machine_object_flags_struct(lua_State *state) noexcept;
    void lua_push_engine_device_machine_object_flags(lua_State *state, EngineDeviceMachineObjectFlags *flags) noexcept;

    void lua_define_engine_device_machine_object_struct(lua_State *state) noexcept;
    void lua_push_engine_device_machine_object(lua_State *state, EngineDeviceMachineObject *object) noexcept;

    void lua_define_engine_device_control_object_flags_struct(lua_State *state) noexcept;
    void lua_push_engine_device_control_object_flags(lua_State *state, EngineDeviceControlObjectFlags *flags) noexcept;

    void lua_define_engine_device_control_object_struct(lua_State *state) noexcept;
    void lua_push_engine_device_control_object(lua_State *state, EngineDeviceControlObject *object) noexcept;

    void lua_define_engine_device_light_fixture_object_struct(lua_State *state) noexcept;
    void lua_push_engine_device_light_fixture_object(lua_State *state, EngineDeviceLightFixtureObject *object) noexcept;

    inline void lua_define_engine_gamestate_device_object_types(lua_State *state) noexcept {
        luacs_newstruct(state, EngineDeviceObjectState);
        lua_pop(state, 1);
        luacs_newstruct(state, EngineDeviceMachineObjectFlags); 
        lua_pop(state, 1);
        luacs_newstruct(state, EngineDeviceControlObjectFlags); 
        lua_pop(state, 1);
        luacs_newderivedstruct(state, EngineDeviceObject, EngineBaseObject);
        lua_pop(state, 1);
        luacs_newderivedstruct(state, EngineDeviceMachineObject, EngineDeviceObject);
        lua_pop(state, 1);
        luacs_newderivedstruct(state, EngineDeviceControlObject, EngineDeviceObject);
        lua_pop(state, 1);
        luacs_newderivedstruct(state, EngineDeviceLightFixtureObject, EngineDeviceObject);
        lua_pop(state, 1);
    
        lua_define_engine_device_object_state_struct(state);
        lua_define_engine_device_object_struct(state);
        lua_define_engine_device_machine_object_flags_struct(state);
        lua_define_engine_device_machine_object_struct(state);
        lua_define_engine_device_control_object_flags_struct(state);
        lua_define_engine_device_control_object_struct(state);
        lua_define_engine_device_light_fixture_object_struct(state);
    }
}

#endif
