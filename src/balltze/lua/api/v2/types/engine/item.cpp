// SPDX-License-Identifier: GPL-3.0-only

#include "../../../../libraries/luastruct.hpp"
#include "types.hpp"
#include "object.hpp"
#include "item.hpp"

namespace Balltze::Lua::Api::V2 {
    static void define_engine_item_object_type(lua_State *state) noexcept {
        LUAS_STRUCT_EXTENDS(state, ItemObject, DynamicObjectBase);
        LUAS_PRIMITIVE_FIELD(state, ItemObject, flags, LUAST_UINT32, 0);
        LUAS_PRIMITIVE_FIELD(state, ItemObject, ticks_until_detonation, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, ItemObject, bsp_collision_surface_id, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, ItemObject, bsp_collision_reference_id, LUAST_INT16, 0);
        LUAS_OBJREF_FIELD(state, ItemObject, dropped_by_unit, ObjectHandle, 0);
        LUAS_PRIMITIVE_FIELD(state, ItemObject, last_update_tick, LUAST_INT32, 0);
        LUAS_OBJREF_FIELD(state, ItemObject, collision_object, ObjectHandle, 0);
        LUAS_OBJREF_FIELD(state, ItemObject, collision_object_position, VectorXYZ, 0);
        LUAS_OBJREF_FIELD(state, ItemObject, unknown_collision_position, VectorXYZ, 0);
        LUAS_OBJREF_FIELD(state, ItemObject, unknown_collision_angle, VectorPY, 0);
        lua_pop(state, 1);
    }

    static void define_engine_garbage_object_type(lua_State *state) noexcept {
        LUAS_STRUCT_EXTENDS(state, GarbageObject, ItemObject);
        LUAS_PRIMITIVE_FIELD(state, GarbageObject, ticks_until_garbage_collection, LUAST_UINT16, 0);
        lua_pop(state, 1);
    }

    static void define_engine_equipment_network_data_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, EquipmentNetworkData);
        LUAS_OBJREF_FIELD(state, EquipmentNetworkData, position, VectorXYZ, 0);
        LUAS_OBJREF_FIELD(state, EquipmentNetworkData, transitional_velocity, VectorXYZ, 0);
        LUAS_OBJREF_FIELD(state, EquipmentNetworkData, angular_velocity, VectorXYZ, 0);
        lua_pop(state, 1);
    }

    static void define_engine_equipment_network_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, EquipmentNetwork);
        LUAS_PRIMITIVE_FIELD(state, EquipmentNetwork, baseline_valid, LUAST_BOOL, 0);
        LUAS_PRIMITIVE_FIELD(state, EquipmentNetwork, baseline_index, LUAST_INT8, 0);
        LUAS_PRIMITIVE_FIELD(state, EquipmentNetwork, message_index, LUAST_INT8, 0);
        LUAS_OBJREF_FIELD(state, EquipmentNetwork, update_baseline, EquipmentNetworkData, 0);
        LUAS_PRIMITIVE_FIELD(state, EquipmentNetwork, delta_valid, LUAST_BOOL, 0);
        LUAS_OBJREF_FIELD(state, EquipmentNetwork, update_delta, EquipmentNetworkData, 0);
        lua_pop(state, 1);
    }

    static void define_engine_equipment_object_type(lua_State *state) noexcept {
        LUAS_STRUCT_EXTENDS(state, EquipmentObject, ItemObject);
        LUAS_OBJREF_FIELD(state, EquipmentObject, network, EquipmentNetwork, 0);
        lua_pop(state, 1);
    }

    static void define_engine_projectile_object_flags_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, ProjectileObjectFlags);
        luastruct_new_struct_bit_field(state, "tracer", LUAST_UINT32, 0, 0, false, false);
        luastruct_new_struct_bit_field(state, "projectileUnknownFlag", LUAST_UINT32, 0, 1, false, false);
        luastruct_new_struct_bit_field(state, "attached", LUAST_UINT32, 0, 2, false, false);
        lua_pop(state, 1);
    }

    static void define_engine_projectile_network_data_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, ProjectileNetworkData);
        LUAS_OBJREF_FIELD(state, ProjectileNetworkData, position, VectorXYZ, 0);
        LUAS_OBJREF_FIELD(state, ProjectileNetworkData, transitional_velocity, VectorXYZ, 0);
        lua_pop(state, 1);
    }

    static void define_engine_projectile_network_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, ProjectileNetwork);
        LUAS_PRIMITIVE_FIELD(state, ProjectileNetwork, unknown, LUAST_BOOL, 0);
        LUAS_PRIMITIVE_FIELD(state, ProjectileNetwork, baseline_valid, LUAST_BOOL, 0);
        LUAS_PRIMITIVE_FIELD(state, ProjectileNetwork, baseline_index, LUAST_INT8, 0);
        LUAS_PRIMITIVE_FIELD(state, ProjectileNetwork, message_index, LUAST_INT8, 0);
        LUAS_OBJREF_FIELD(state, ProjectileNetwork, update_baseline, ProjectileNetworkData, 0);
        LUAS_PRIMITIVE_FIELD(state, ProjectileNetwork, delta_valid, LUAST_BOOL, 0);
        LUAS_OBJREF_FIELD(state, ProjectileNetwork, update_delta, ProjectileNetworkData, 0);
        lua_pop(state, 1);
    }

    static void define_engine_projectile_object_type(lua_State *state) noexcept {
        LUAS_STRUCT_EXTENDS(state, ProjectileObject, ItemObject);
        LUAS_OBJREF_FIELD(state, ProjectileObject, projectile_flags, ProjectileObjectFlags, 0);
        LUAS_PRIMITIVE_FIELD(state, ProjectileObject, action_enum, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, ProjectileObject, material_id, LUAST_INT16, 0);
        LUAS_OBJREF_FIELD(state, ProjectileObject, source_unit, ObjectHandle, 0);
        LUAS_OBJREF_FIELD(state, ProjectileObject, target_object, ObjectHandle, 0);
        LUAS_PRIMITIVE_FIELD(state, ProjectileObject, contrail_attachment_block_id, LUAST_INT32, 0);
        LUAS_PRIMITIVE_FIELD(state, ProjectileObject, time_remaining, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, ProjectileObject, arming_rate, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, ProjectileObject, unknown_proj_float1, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, ProjectileObject, unknown_proj_float2, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, ProjectileObject, distance_travelled, LUAST_FLOAT, 0);
        LUAS_OBJREF_FIELD(state, ProjectileObject, transitional_velocity, VectorXYZ, 0);
        LUAS_PRIMITIVE_FIELD(state, ProjectileObject, water_damage_upper_bound, LUAST_FLOAT, 0);
        LUAS_OBJREF_FIELD(state, ProjectileObject, angular_velocity, VectorXYZ, 0);
        LUAS_OBJREF_FIELD(state, ProjectileObject, unknown_euler, VectorPY, 0);
        LUAS_OBJREF_FIELD(state, ProjectileObject, network, ProjectileNetwork, 0);
        lua_pop(state, 1);
    }

    void push_equipment_object(lua_State *state, const EquipmentObject &object, bool readonly) noexcept {
        LUAS_PUSH_OBJECT(state, EquipmentObject, &object, readonly);
    }

    void push_garbage_object(lua_State *state, const GarbageObject &object, bool readonly) noexcept {
        LUAS_PUSH_OBJECT(state, GarbageObject, &object, readonly);
    }

    void push_projectile_object(lua_State *state, const ProjectileObject &object, bool readonly) noexcept {
        LUAS_PUSH_OBJECT(state, ProjectileObject, &object, readonly);
    }

    void define_engine_item_types(lua_State *state) noexcept {
        define_engine_item_object_type(state);
        define_engine_garbage_object_type(state);
        define_engine_equipment_network_data_type(state);
        define_engine_equipment_network_type(state);
        define_engine_equipment_object_type(state);
        define_engine_projectile_object_flags_type(state);
        define_engine_projectile_network_data_type(state);
        define_engine_projectile_network_type(state);
        define_engine_projectile_object_type(state);
    }
}
