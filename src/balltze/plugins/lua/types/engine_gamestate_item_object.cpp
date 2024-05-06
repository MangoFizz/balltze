// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/engine/game_state.hpp>
#include "../helpers/bitfield.hpp"
#include "engine_types.hpp"
#include "engine_tag_data.hpp"
#include "engine_gamestate_object.hpp"
#include "engine_gamestate_item_object.hpp"

namespace Balltze::Plugins::Lua {
    void lua_define_engine_item_object_struct(lua_State *state) noexcept {
        luacs_newderivedstruct(state, EngineItemObject, EngineBaseObject);
        luacs_unsigned_field(state, EngineItemObject, flags, 0);
        luacs_int_field(state, EngineItemObject, ticks_until_detonation, 0);
        luacs_int_field(state, EngineItemObject, bsp_collision_surface_id, 0);
        luacs_int_field(state, EngineItemObject, bsp_collision_reference_id, 0);
        luacs_nested_field(state, EngineItemObject, EngineResourceHandle, dropped_by_unit, 0);
        luacs_int_field(state, EngineItemObject, last_update_tick, 0);
        luacs_nested_field(state, EngineItemObject, EngineResourceHandle, collision_object_handle, 0);
        luacs_nested_field(state, EngineItemObject, EngineVector3D, collision_object_position, 0);
        luacs_nested_field(state, EngineItemObject, EngineVector3D, unknown_collision_position, 0);
        luacs_nested_field(state, EngineItemObject, EngineEuler2D, unknown_collision_angle, 0);
        lua_pop(state, 1); 
    }

    void lua_push_engine_item_object(lua_State *state, EngineItemObject *object) noexcept {
        luacs_newobject(state, EngineItemObject, object);
    }

    void lua_define_engine_garbage_object_struct(lua_State *state) noexcept {
        luacs_newderivedstruct(state, EngineGarbageObject, EngineItemObject);
        luacs_unsigned_field(state, EngineGarbageObject, ticks_until_garbage_collection, 0);
        lua_pop(state, 1);
    }

    void lua_push_engine_garbage_object(lua_State *state, EngineGarbageObject *object) noexcept {
        luacs_newobject(state, EngineGarbageObject, object);
    }

    void lua_define_weapon_state_enum(lua_State *state) noexcept {
        luacs_newenum(state, EngineWeaponState);
        luacs_enum_declare_value(state, "IDLE", Engine::WEAPON_STATE_IDLE);
        luacs_enum_declare_value(state, "FIRE1", Engine::WEAPON_STATE_FIRE1);
        luacs_enum_declare_value(state, "FIRE2", Engine::WEAPON_STATE_FIRE2);
        luacs_enum_declare_value(state, "CHAMBER1", Engine::WEAPON_STATE_CHAMBER1);
        luacs_enum_declare_value(state, "CHAMBER2", Engine::WEAPON_STATE_CHAMBER2);
        luacs_enum_declare_value(state, "RELOAD1", Engine::WEAPON_STATE_RELOAD1);
        luacs_enum_declare_value(state, "RELOAD2", Engine::WEAPON_STATE_RELOAD2);
        luacs_enum_declare_value(state, "CHARGED1", Engine::WEAPON_STATE_CHARGED1);
        luacs_enum_declare_value(state, "CHARGED2", Engine::WEAPON_STATE_CHARGED2);
        luacs_enum_declare_value(state, "READY", Engine::WEAPON_STATE_READY);
        luacs_enum_declare_value(state, "PUT_AWAY", Engine::WEAPON_STATE_PUT_AWAY);
        lua_pop(state, 1);
    }

    void lua_define_engine_weapon_trigger_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineGameStateWeaponTrigger);
        luacs_int_field(state, EngineGameStateWeaponTrigger, idle_time, 0);
        luacs_enum_field(state, EngineGameStateWeaponTrigger, EngineWeaponState, state, 0);
        luacs_unsigned_field(state, EngineGameStateWeaponTrigger, trigger_time, 0);
        luacs_unsigned_field(state, EngineGameStateWeaponTrigger, not_firing, 0);
        luacs_unsigned_field(state, EngineGameStateWeaponTrigger, auto_reload, 0);
        luacs_unsigned_field(state, EngineGameStateWeaponTrigger, rounds_since_last_tracer, 0);
        luacs_float_field(state, EngineGameStateWeaponTrigger, rate_of_fire, 0);
        luacs_float_field(state, EngineGameStateWeaponTrigger, ejection_port_recovery_time, 0);
        luacs_float_field(state, EngineGameStateWeaponTrigger, illumination_recovery_time, 0);
        luacs_float_field(state, EngineGameStateWeaponTrigger, projectile_error_related, 0);
        luacs_nested_field(state, EngineGameStateWeaponTrigger, EngineResourceHandle, charing_effect, 0);
        luacs_int_field(state, EngineGameStateWeaponTrigger, network_delay_ticks, 0);
        lua_pop(state, 1); 
    }

    void lua_push_engine_weapon_trigger(lua_State *state, EngineGameStateWeaponTrigger *trigger) noexcept {
        luacs_newobject(state, EngineGameStateWeaponTrigger, trigger);
    }

    void lua_define_weapon_magazine_state_enum(lua_State *state) noexcept {
        luacs_newenum(state, EngineGameStateWeaponMagazineState);
        luacs_enum_declare_value(state, "IDLE", Engine::WEAPON_MAGAZINE_STATE_IDLE);
        luacs_enum_declare_value(state, "CHAMBERING_START", Engine::WEAPON_MAGAZINE_STATE_CHAMBERING_START);
        luacs_enum_declare_value(state, "CHAMBERING_FINISH", Engine::WEAPON_MAGAZINE_STATE_CHAMBERING_FINISH);
        luacs_enum_declare_value(state, "CHAMBERING", Engine::WEAPON_MAGAZINE_STATE_CHAMBERING);
        lua_pop(state, 1);
    }

    void lua_define_engine_weapon_magazine_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineGameStateWeaponMagazine);
        luacs_enum_field(state, EngineGameStateWeaponMagazine, EngineGameStateWeaponMagazineState, state, 0);
        luacs_unsigned_field(state, EngineGameStateWeaponMagazine, reload_ticks_remaining, 0);
        luacs_unsigned_field(state, EngineGameStateWeaponMagazine, reload_ticks, 0);
        luacs_int_field(state, EngineGameStateWeaponMagazine, rounds_unloaded, 0);
        luacs_int_field(state, EngineGameStateWeaponMagazine, rounds_loaded, 0);
        luacs_int_field(state, EngineGameStateWeaponMagazine, rounds_left_to_recharge, 0);
        luacs_int_field(state, EngineGameStateWeaponMagazine, unknown, 0);
        luacs_int_field(state, EngineGameStateWeaponMagazine, unknown2, 0);
        lua_pop(state, 1); 
    }

    void lua_push_engine_weapon_magazine(lua_State *state, EngineGameStateWeaponMagazine *magazine) noexcept {
        luacs_newobject(state, EngineGameStateWeaponMagazine, magazine);
    }

    void lua_define_engine_weapon_reload_start_data_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineWeaponReloadStartData);
        luacs_int_array_field(state, EngineWeaponReloadStartData, total_rounds, 0);
        luacs_int_array_field(state, EngineWeaponReloadStartData, loaded_rounds, 0);
        lua_pop(state, 1); 
    }

    void lua_push_engine_weapon_reload_start_data(lua_State *state, EngineWeaponReloadStartData *reload_start_data) noexcept {
        luacs_newobject(state, EngineWeaponReloadStartData, reload_start_data);
    }

    void lua_define_engine_weapon_network_data_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineWeaponNetworkData);
        luacs_nested_field(state, EngineWeaponNetworkData, EngineVector3D, position, 0);
        luacs_nested_field(state, EngineWeaponNetworkData, EngineVector3D, transitional_velocity, 0);
        luacs_nested_field(state, EngineWeaponNetworkData, EngineVector3D, angular_velocity, 0);
        luacs_int_array_field(state, EngineWeaponNetworkData, magazine_rounds_total, 0);
        luacs_float_field(state, EngineWeaponNetworkData, age, 0);
        lua_pop(state, 1); 
    }

    void lua_push_engine_weapon_network_data(lua_State *state, EngineWeaponNetworkData *network_data) noexcept {
        luacs_newobject(state, EngineWeaponNetworkData, network_data);
    }

    void lua_define_engine_weapon_network_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineWeaponNetwork);
        luacs_bool_field(state, EngineWeaponNetwork, baseline_valid, 0);
        luacs_int_field(state, EngineWeaponNetwork, baseline_index, 0);
        luacs_int_field(state, EngineWeaponNetwork, message_index, 0);
        luacs_nested_field(state, EngineWeaponNetwork, EngineWeaponNetworkData, update_baseline, 0);
        luacs_bool_field(state, EngineWeaponNetwork, delta_valid, 0);
        luacs_nested_field(state, EngineWeaponNetwork, EngineWeaponNetworkData, update_delta, 0);
        lua_pop(state, 1); 
    }

    void lua_push_engine_weapon_network(lua_State *state, EngineWeaponNetwork *network) noexcept {
        luacs_newobject(state, EngineWeaponNetwork, network);
    }

    void lua_define_engine_weapon_object_struct(lua_State *state) noexcept {
        luacs_newderivedstruct(state, EngineWeaponObject, EngineItemObject);
        luacs_unsigned_field(state, EngineWeaponObject, flags, 0);
        luacs_unsigned_field(state, EngineWeaponObject, owner_unit_flags, 0);
        luacs_float_field(state, EngineWeaponObject, primary_trigger, 0);
        luacs_enum_field(state, EngineWeaponObject, EngineWeaponState, weapon_state, 0);
        luacs_int_field(state, EngineWeaponObject, ready_ticks, 0);
        luacs_float_field(state, EngineWeaponObject, heat, 0);
        luacs_float_field(state, EngineWeaponObject, age, 0);
        luacs_float_field(state, EngineWeaponObject, illumination_fraction, 0);
        luacs_float_field(state, EngineWeaponObject, integrated_light_power, 0);
        luacs_nested_field(state, EngineWeaponObject, EngineResourceHandle, tracked_object, 0);
        luacs_int_field(state, EngineWeaponObject, alt_shots_loaded, 0);
        luacs_nested_array_field(state, EngineWeaponObject, EngineWeaponTrigger, triggers, 0);
        luacs_nested_array_field(state, EngineWeaponObject, EngineWeaponMagazine, magazines, 0);
        luacs_unsigned_field(state, EngineWeaponObject, last_trigger_fire_tick, 0);
        luacs_nested_field(state, EngineWeaponObject, EngineWeaponReloadStartData, reload_starting_point, 0);
        luacs_nested_field(state, EngineWeaponObject, EngineWeaponNetwork, network, 0);
        lua_pop(state, 1); 
    }

    void lua_push_engine_weapon_object(lua_State *state, EngineWeaponObject *object) noexcept {
        luacs_newobject(state, EngineWeaponObject, object);
    }

    void lua_define_engine_equipment_network_data_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineEquipmentNetworkData);
        luacs_nested_field(state, EngineEquipmentNetworkData, EngineVector3D, position, 0);
        luacs_nested_field(state, EngineEquipmentNetworkData, EngineVector3D, transitional_velocity, 0);
        luacs_nested_field(state, EngineEquipmentNetworkData, EngineVector3D, angular_velocity, 0);
        lua_pop(state, 1); 
    }

    void lua_push_engine_equipment_network_data(lua_State *state, EngineEquipmentNetworkData *network_data) noexcept {
        luacs_newobject(state, EngineEquipmentNetworkData, network_data);
    }

    void lua_define_engine_equipment_network_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineEquipmentNetwork);
        luacs_bool_field(state, EngineEquipmentNetwork, baseline_valid, 0);
        luacs_int_field(state, EngineEquipmentNetwork, baseline_index, 0);
        luacs_int_field(state, EngineEquipmentNetwork, message_index, 0);
        luacs_nested_field(state, EngineEquipmentNetwork, EngineEquipmentNetworkData, update_baseline, 0);
        luacs_bool_field(state, EngineEquipmentNetwork, delta_valid, 0);
        luacs_nested_field(state, EngineEquipmentNetwork, EngineEquipmentNetworkData, update_delta, 0);
        lua_pop(state, 1); 
    }

    void lua_push_engine_equipment_network(lua_State *state, EngineEquipmentNetwork *network) noexcept {
        luacs_newobject(state, EngineEquipmentNetwork, network);
    }

    void lua_define_engine_equipment_object_struct(lua_State *state) noexcept {
        luacs_newderivedstruct(state, EngineEquipmentObject, EngineItemObject);
        luacs_nested_field(state, EngineEquipmentObject, EngineEquipmentNetwork, network, 0);
        lua_pop(state, 1); 
    }

    void lua_push_engine_equipment_object(lua_State *state, EngineEquipmentObject *equipment_object) noexcept {
        luacs_newobject(state, EngineEquipmentObject, equipment_object);
    }

    void lua_define_engine_projectile_object_flags_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineProjectileObjectFlags); 
        luacs_declare_field(state, LUACS_TINT8, NULL, "flags", sizeof(std::uint8_t), 0, 0, 0); 
        luacs_declare_method(state, "tracer", lua_bitfield_struct_method(state, EngineProjectileObjectFlags, tracer)); 
        luacs_declare_method(state, "projectileUnknownBit", lua_bitfield_struct_method(state, EngineProjectileObjectFlags, projectile_unknown_bit));
        luacs_declare_method(state, "attached", lua_bitfield_struct_method(state, EngineProjectileObjectFlags, attached));
        lua_pop(state, 1); 
    }

    void lua_push_engine_projectile_object_flags(lua_State *state, EngineProjectileObjectFlags *projectile_object_flags) noexcept {
        luacs_newobject(state, EngineProjectileObjectFlags, projectile_object_flags);
    }

    void lua_define_engine_projectile_network_data_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineProjectileNetworkData);
        luacs_nested_field(state, EngineProjectileNetworkData, EngineVector3D, position, 0);
        luacs_nested_field(state, EngineProjectileNetworkData, EngineVector3D, transitional_velocity, 0);
        lua_pop(state, 1); 
    }

    void lua_push_engine_projectile_network_data(lua_State *state, EngineProjectileNetworkData *projectile_network_data) noexcept {
        luacs_newobject(state, EngineProjectileNetworkData, projectile_network_data);
    }

    void lua_define_engine_projectile_network_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineProjectileNetwork);
        luacs_bool_field(state, EngineProjectileNetwork, unknown, 0);
        luacs_bool_field(state, EngineProjectileNetwork, baseline_valid, 0);
        luacs_int_field(state, EngineProjectileNetwork, baseline_index, 0);
        luacs_int_field(state, EngineProjectileNetwork, message_index, 0);
        luacs_nested_field(state, EngineProjectileNetwork, EngineProjectileNetworkData, update_baseline, 0);
        luacs_bool_field(state, EngineProjectileNetwork, delta_valid, 0);
        luacs_nested_field(state, EngineProjectileNetwork, EngineProjectileNetworkData, update_delta, 0);
        lua_pop(state, 1); 
    }

    void lua_push_engine_projectile_network(lua_State *state, EngineProjectileNetwork *projectile_network) noexcept {
        luacs_newobject(state, EngineProjectileNetwork, projectile_network);
    }

    void lua_define_engine_projectile_object_struct(lua_State *state) noexcept {
        luacs_newderivedstruct(state, EngineProjectileObject, EngineItemObject);
        luacs_nested_field(state, EngineProjectileObject, EngineProjectileObjectFlags, projectile_flags, 0);
        luacs_int_field(state, EngineProjectileObject, action_enum, 0);
        luacs_int_field(state, EngineProjectileObject, material_id, 0);
        luacs_nested_field(state, EngineProjectileObject, EngineResourceHandle, source_unit, 0);
        luacs_nested_field(state, EngineProjectileObject, EngineResourceHandle, target_object, 0);
        luacs_int_field(state, EngineProjectileObject, contrail_attachment_block_id, 0);
        luacs_float_field(state, EngineProjectileObject, time_remaining, 0);
        luacs_float_field(state, EngineProjectileObject, arming_rate, 0);
        luacs_float_field(state, EngineProjectileObject, unknown_proj_float1, 0);
        luacs_float_field(state, EngineProjectileObject, unknown_proj_float2, 0);
        luacs_float_field(state, EngineProjectileObject, distance_travelled, 0);
        luacs_nested_field(state, EngineProjectileObject, EngineVector3D, transitional_velocity, 0);
        luacs_float_field(state, EngineProjectileObject, water_damage_upper_bound, 0);
        luacs_nested_field(state, EngineProjectileObject, EngineVector3D, angular_velocity, 0);
        luacs_nested_field(state, EngineProjectileObject, EngineEuler2D, unknown_euler, 0);
        luacs_nested_field(state, EngineProjectileObject, EngineProjectileNetwork, network, 0);
        lua_pop(state, 1); 
    }

    void lua_push_engine_projectile_object(lua_State *state, EngineProjectileObject *projectile_object) noexcept {
        luacs_newobject(state, EngineProjectileObject, projectile_object);
    }
}
