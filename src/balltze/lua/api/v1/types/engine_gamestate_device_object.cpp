// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/engine/game_state.hpp>
#include "../../../helpers/bitfield.hpp"
#include "../../../libraries/luacstruct.hpp"
#include "engine_types.hpp"
#include "engine_tag_data.hpp"
#include "engine_gamestate_object.hpp"
#include "engine_gamestate_device_object.hpp"

namespace Balltze::Lua::Api::V1 {
    static void define_engine_device_object_state_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineDeviceObjectState);
        luacs_int_field(state, EngineDeviceObjectState, device_group_id, 0);
        luacs_float_field(state, EngineDeviceObjectState, value, 0);
        luacs_float_field(state, EngineDeviceObjectState, change, 0);
        lua_pop(state, 1);
    }

    void push_engine_device_object_state(lua_State *state, EngineDeviceObjectState *object_state) noexcept {
        luacs_newobject(state, EngineDeviceObjectState, object_state);
    }

    static void define_engine_device_object_struct(lua_State *state) noexcept {
        luacs_newderivedstruct(state, EngineDeviceObject, EngineBaseObject);
        luacs_declare_method(state, "positionReversed", lua_bitfield_struct_method(state, EngineDeviceObject, position_reversed)); 
        luacs_declare_method(state, "notUsableFromAnySide", lua_bitfield_struct_method(state, EngineDeviceObject, not_usable_from_any_side)); 
        luacs_nested_field(state, EngineDeviceObject, EngineDeviceObjectState, power, 0);
        luacs_nested_field(state, EngineDeviceObject, EngineDeviceObjectState, position, 0);
        luacs_declare_method(state, "oneSided", lua_bitfield_struct_method(state, EngineDeviceObject, one_sided));
        luacs_declare_method(state, "operatesAutomatically", lua_bitfield_struct_method(state, EngineDeviceObject, operates_automatically));
        lua_pop(state, 1); 
    }

    void push_engine_device_object(lua_State *state, EngineDeviceObject *object) noexcept {
        luacs_newobject(state, EngineDeviceObject, object);
    }

    static void define_engine_device_machine_object_flags_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineDeviceMachineObjectFlags); 
        luacs_declare_field(state, LUACS_TINT8, NULL, "flags", sizeof(std::uint8_t), 0, 0, 0); 
        luacs_declare_method(state, "doesNotOperateAutomatically", lua_bitfield_struct_method(state, EngineDeviceMachineObjectFlags, does_not_operate_automatically)); 
        luacs_declare_method(state, "machineOneSided", lua_bitfield_struct_method(state, EngineDeviceMachineObjectFlags, machine_one_sided));
        luacs_declare_method(state, "neverAppearsLocked", lua_bitfield_struct_method(state, EngineDeviceMachineObjectFlags, never_appears_locked));
        luacs_declare_method(state, "openedByMeleeAttack", lua_bitfield_struct_method(state, EngineDeviceMachineObjectFlags, opened_by_melee_attack));
        lua_pop(state, 1); 
    }

    void push_engine_device_machine_object_flags(lua_State *state, EngineDeviceMachineObjectFlags *flags) noexcept {
        luacs_newobject(state, EngineDeviceMachineObjectFlags, flags);
    }

    static void define_engine_device_machine_object_struct(lua_State *state) noexcept {
        luacs_newderivedstruct(state, EngineDeviceMachineObject, EngineDeviceObject);
        luacs_nested_field(state, EngineDeviceMachineObject, EngineDeviceMachineObjectFlags, device_flags, 0);
        luacs_unsigned_field(state, EngineDeviceMachineObject, ticks_since_started_opening, 0);
        luacs_nested_field(state, EngineDeviceMachineObject, EngineVector3D, elevator_position, 0);
        lua_pop(state, 1);
    }

    void push_engine_device_machine_object(lua_State *state, EngineDeviceMachineObject *object) noexcept {
        luacs_newobject(state, EngineDeviceMachineObject, object);
    }

    static void define_engine_device_control_object_flags_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineDeviceControlObjectFlags); 
        luacs_declare_field(state, LUACS_TINT8, NULL, "flags", sizeof(std::uint8_t), 0, 0, 0); 
        luacs_declare_method(state, "usableFromBothSides", lua_bitfield_struct_method(state, EngineDeviceControlObjectFlags, usable_from_both_sides)); 
        lua_pop(state, 1); 
    }

    void push_engine_device_control_object_flags(lua_State *state, EngineDeviceControlObjectFlags *flags) noexcept {
        luacs_newobject(state, EngineDeviceControlObjectFlags, flags);
    }

    static void define_engine_device_control_object_struct(lua_State *state) noexcept {
        luacs_newderivedstruct(state, EngineDeviceControlObject, EngineDeviceObject);
        luacs_nested_field(state, EngineDeviceControlObject, EngineDeviceControlObjectFlags, device_control_flags, 0);
        luacs_int_field(state, EngineDeviceControlObject, custom_name_id, 0);
        lua_pop(state, 1); 
    }

    void push_engine_device_control_object(lua_State *state, EngineDeviceControlObject *object) noexcept {
        luacs_newobject(state, EngineDeviceControlObject, object);
    }

    static void define_engine_device_light_fixture_object_struct(lua_State *state) noexcept {
        luacs_newderivedstruct(state, EngineDeviceLightFixtureObject, EngineDeviceObject);
        luacs_nested_field(state, EngineDeviceLightFixtureObject, EngineColorRGB, light_color, 0);
        luacs_float_field(state, EngineDeviceLightFixtureObject, light_intensity, 0);
        luacs_float_field(state, EngineDeviceLightFixtureObject, light_falloff_angle, 0);
        luacs_float_field(state, EngineDeviceLightFixtureObject, light_cutoff_angle, 0);
        lua_pop(state, 1);
    }

    void push_engine_device_light_fixture_object(lua_State *state, EngineDeviceLightFixtureObject *object) noexcept {
        luacs_newobject(state, EngineDeviceLightFixtureObject, object);
    }

    void define_engine_gamestate_device_object_types(lua_State *state) noexcept {
        define_engine_device_object_state_struct(state);
        define_engine_device_object_struct(state);
        define_engine_device_machine_object_flags_struct(state);
        define_engine_device_machine_object_struct(state);
        define_engine_device_control_object_flags_struct(state);
        define_engine_device_control_object_struct(state);
        define_engine_device_light_fixture_object_struct(state);
    }
}
