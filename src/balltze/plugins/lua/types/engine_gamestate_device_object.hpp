// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__PLUGINS__LUA__TYPES__ENGINE_GAMESTATE_DEVICE_OBJECT_HPP
#define BALLTZE__PLUGINS__LUA__TYPES__ENGINE_GAMESTATE_DEVICE_OBJECT_HPP

#include <lua.hpp>
#include <balltze/engine.hpp>

namespace Balltze::Plugins::Lua {
    using EngineDeviceObjectState = Engine::DeviceObjectState;
    using EngineDeviceObject = Engine::DeviceObject;
    using EngineDeviceMachineObjectFlags = Engine::DeviceMachineObjectFlags;
    using EngineDeviceMachineObject = Engine::DeviceMachineObject;
    using EngineDeviceControlObjectFlags = Engine::DeviceControlObjectFlags;
    using EngineDeviceControlObject = Engine::DeviceControlObject;
    using EngineDeviceLightFixtureObject = Engine::DeviceLightFixtureObject;

    void push_engine_device_object_state(lua_State *state, EngineDeviceObjectState *object_state) noexcept;

    void push_engine_device_object(lua_State *state, EngineDeviceObject *object) noexcept;

    void push_engine_device_machine_object_flags(lua_State *state, EngineDeviceMachineObjectFlags *flags) noexcept;

    void push_engine_device_machine_object(lua_State *state, EngineDeviceMachineObject *object) noexcept;

    void push_engine_device_control_object_flags(lua_State *state, EngineDeviceControlObjectFlags *flags) noexcept;

    void push_engine_device_control_object(lua_State *state, EngineDeviceControlObject *object) noexcept;

    void push_engine_device_light_fixture_object(lua_State *state, EngineDeviceLightFixtureObject *object) noexcept;

    void define_engine_gamestate_device_object_types(lua_State *state) noexcept;
}

#endif
