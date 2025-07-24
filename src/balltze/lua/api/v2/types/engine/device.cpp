// SPDX-License-Identifier: GPL-3.0-only

#include "../../../../libraries/luastruct.hpp"
#include "types.hpp"
#include "object.hpp"
#include "device.hpp"

namespace Balltze::Lua::Api::V2 {
    static void define_engine_device_object_state_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, DeviceObjectState);
        LUAS_PRIMITIVE_FIELD(state, DeviceObjectState, device_group_id, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, DeviceObjectState, value, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, DeviceObjectState, change, LUAST_FLOAT, 0);
        lua_pop(state, 1);
    }

    static void define_engine_device_object_flags1_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, DeviceObjectFlags1);
        luastruct_new_struct_bit_field(state, "positionReversed", LUAST_UINT32, 0, 0, false, false);
        luastruct_new_struct_bit_field(state, "notUsableFromAnySide", LUAST_UINT32, 0, 1, false, false);
        lua_pop(state, 1);
    }

    static void define_engine_device_object_flags2_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, DeviceObjectFlags2);
        luastruct_new_struct_bit_field(state, "oneSided", LUAST_UINT32, 0, 0, false, false);
        luastruct_new_struct_bit_field(state, "operatesAutomatically", LUAST_UINT32, 0, 1, false, false);
        lua_pop(state, 1);
    }

    static void define_engine_device_object_type(lua_State *state) noexcept {
        LUAS_STRUCT_EXTENDS(state, DeviceObject, DynamicObjectBase);
        LUAS_OBJREF_FIELD(state, DeviceObject, flags_1, DeviceObjectFlags1, 0);
        LUAS_OBJREF_FIELD(state, DeviceObject, power, DeviceObjectState, 0);
        LUAS_OBJREF_FIELD(state, DeviceObject, position, DeviceObjectState, 0);
        LUAS_OBJREF_FIELD(state, DeviceObject, flags_2, DeviceObjectFlags2, 0);
        lua_pop(state, 1);
    }

    static void define_engine_device_machine_object_flags_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, DeviceMachineObjectFlags);
        luastruct_new_struct_bit_field(state, "doesNotOperateAutomatically", LUAST_UINT32, 0, 0, false, false);
        luastruct_new_struct_bit_field(state, "machineOneSided", LUAST_UINT32, 0, 1, false, false);
        luastruct_new_struct_bit_field(state, "neverAppearsLocked", LUAST_UINT32, 0, 2, false, false);
        luastruct_new_struct_bit_field(state, "openedByMeleeAttack", LUAST_UINT32, 0, 3, false, false);
        lua_pop(state, 1);
    }

    static void define_engine_device_machine_object_type(lua_State *state) noexcept {
        LUAS_STRUCT_EXTENDS(state, DeviceMachineObject, DeviceObject);
        LUAS_OBJREF_FIELD(state, DeviceMachineObject, device_flags, DeviceMachineObjectFlags, 0);
        LUAS_PRIMITIVE_FIELD(state, DeviceMachineObject, ticks_since_started_opening, LUAST_UINT32, 0);
        LUAS_OBJREF_FIELD(state, DeviceMachineObject, elevator_position, VectorXYZ, 0);
        lua_pop(state, 1);
    }

    static void define_engine_device_control_object_flags_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, DeviceControlObjectFlags);
        luastruct_new_struct_bit_field(state, "usableFromBothSides", LUAST_UINT32, 0, 0, false, false);
        lua_pop(state, 1);
    }

    static void define_engine_device_control_object_type(lua_State *state) noexcept {
        LUAS_STRUCT_EXTENDS(state, DeviceControlObject, DeviceObject);
        LUAS_OBJREF_FIELD(state, DeviceControlObject, device_control_flags, DeviceControlObjectFlags, 0);
        lua_pop(state, 1);
    }

    static void define_engine_device_light_fixture_object_type(lua_State *state) noexcept {
        LUAS_STRUCT_EXTENDS(state, DeviceLightFixtureObject, DeviceObject);
        LUAS_OBJREF_FIELD(state, DeviceLightFixtureObject, light_color, ColorRGB, 0);
        LUAS_PRIMITIVE_FIELD(state, DeviceLightFixtureObject, light_intensity, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, DeviceLightFixtureObject, light_falloff_angle, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, DeviceLightFixtureObject, light_cutoff_angle, LUAST_FLOAT, 0);
        lua_pop(state, 1);
    }

    void push_device_object(lua_State *state, const DeviceObject &device, bool readonly) noexcept {
        LUAS_PUSH_OBJECT(state, DeviceObject, &device, readonly);
    }

    void push_device_machine_object(lua_State *state, const DeviceMachineObject &device, bool readonly) noexcept {
        LUAS_PUSH_OBJECT(state, DeviceMachineObject, &device, readonly);
    }

    void push_device_control_object(lua_State *state, const DeviceControlObject &device, bool readonly) noexcept {
        LUAS_PUSH_OBJECT(state, DeviceControlObject, &device, readonly);
    }

    void push_device_light_fixture_object(lua_State *state, const DeviceLightFixtureObject &device, bool readonly) noexcept {
        LUAS_PUSH_OBJECT(state, DeviceLightFixtureObject, &device, readonly);
    }

    void define_engine_device_types(lua_State *state) noexcept {
        define_engine_device_object_state_type(state);
        define_engine_device_object_flags1_type(state);
        define_engine_device_object_flags2_type(state);
        define_engine_device_object_type(state);
        define_engine_device_machine_object_flags_type(state);
        define_engine_device_machine_object_type(state);
        define_engine_device_control_object_flags_type(state);
        define_engine_device_control_object_type(state);
        define_engine_device_light_fixture_object_type(state);
    }
}
