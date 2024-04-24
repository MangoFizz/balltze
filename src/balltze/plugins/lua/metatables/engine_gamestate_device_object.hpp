// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__PLUGINS__LUA__METATABLES__ENGINE_GAMESTATE_DEVICE_OBJECT_HPP
#define BALLTZE__PLUGINS__LUA__METATABLES__ENGINE_GAMESTATE_DEVICE_OBJECT_HPP

#include <string>
#include <balltze/engine.hpp>

namespace Balltze::Plugins {
    void lua_push_meta_engine_device_object_state(lua_State *state, Engine::DeviceObjectState &object_state, bool read_only = false) noexcept;
    void lua_push_meta_engine_device_object(lua_State *state, Engine::DeviceObject &object, bool read_only = false) noexcept;
    void lua_push_meta_engine_device_machine_object_flags(lua_State *state, Engine::DeviceMachineObjectFlags &flags, bool read_only = false) noexcept;
    void lua_push_meta_engine_device_machine_object(lua_State *state, Engine::DeviceMachineObject &object, bool read_only = false) noexcept;
    void lua_push_meta_engine_device_control_object_flags(lua_State *state, Engine::DeviceControlObjectFlags &flags, bool read_only = false) noexcept;
    void lua_push_meta_engine_device_control_object(lua_State *state, Engine::DeviceControlObject &object, bool read_only = false) noexcept;
    void lua_push_meta_engine_device_light_fixture_object(lua_State *state, Engine::DeviceLightFixtureObject &object, bool read_only = false) noexcept;
}

#endif
