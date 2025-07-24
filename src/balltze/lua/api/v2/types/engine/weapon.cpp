// SPDX-License-Identifier: GPL-3.0-only

#include "../../../../libraries/luastruct.hpp"
#include "types.hpp"
#include "weapon.hpp"

namespace Balltze::Lua::Api::V2 {
    static void define_engine_weapon_trigger_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, WeaponObjectTrigger);
        LUAS_PRIMITIVE_FIELD(state, WeaponObjectTrigger, idle_time, LUAST_INT8, 0);
        LUAS_PRIMITIVE_FIELD(state, WeaponObjectTrigger, state, LUAST_INT8, 0);
        LUAS_PRIMITIVE_FIELD(state, WeaponObjectTrigger, trigger_time, LUAST_UINT16, 0);
        LUAS_PRIMITIVE_FIELD(state, WeaponObjectTrigger, not_firing, LUAST_UINT32, 0);
        LUAS_PRIMITIVE_FIELD(state, WeaponObjectTrigger, auto_reload, LUAST_UINT32, 0);
        LUAS_PRIMITIVE_FIELD(state, WeaponObjectTrigger, rounds_since_last_tracer, LUAST_UINT16, 0);
        LUAS_PRIMITIVE_FIELD(state, WeaponObjectTrigger, rate_of_fire, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, WeaponObjectTrigger, ejection_port_recovery_time, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, WeaponObjectTrigger, illumination_recovery_time, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, WeaponObjectTrigger, projectile_error_related, LUAST_FLOAT, 0);
        LUAS_OBJREF_FIELD(state, WeaponObjectTrigger, charing_effect, TableResourceHandle, 0);
        LUAS_PRIMITIVE_FIELD(state, WeaponObjectTrigger, network_delay_ticks, LUAST_INT8, 0);
        lua_pop(state, 1);
    }

    static void define_engine_weapon_magazine_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, WeaponObjectMagazine);
        LUAS_PRIMITIVE_FIELD(state, WeaponObjectMagazine, state, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, WeaponObjectMagazine, reload_ticks_remaining, LUAST_UINT16, 0);
        LUAS_PRIMITIVE_FIELD(state, WeaponObjectMagazine, reload_ticks, LUAST_UINT16, 0);
        LUAS_PRIMITIVE_FIELD(state, WeaponObjectMagazine, rounds_unloaded, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, WeaponObjectMagazine, rounds_loaded, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, WeaponObjectMagazine, rounds_left_to_recharge, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, WeaponObjectMagazine, unk_1, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, WeaponObjectMagazine, unk_2, LUAST_INT16, 0);
        lua_pop(state, 1);
    }

    static void define_engine_weapon_reload_start_data_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, WeaponReloadStartData);
        LUAS_PRIMITIVE_ARRAY_FIELD(state, WeaponReloadStartData, total_rounds, LUAST_INT16, 0);
        LUAS_PRIMITIVE_ARRAY_FIELD(state, WeaponReloadStartData, loaded_rounds, LUAST_INT16, 0);
        lua_pop(state, 1);
    }

    static void define_engine_weapon_network_data_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, WeaponNetworkData);
        LUAS_OBJREF_FIELD(state, WeaponNetworkData, position, VectorXYZ, 0);
        LUAS_OBJREF_FIELD(state, WeaponNetworkData, transitional_velocity, VectorXYZ, 0);
        LUAS_OBJREF_FIELD(state, WeaponNetworkData, angular_velocity, VectorXYZ, 0);
        LUAS_PRIMITIVE_ARRAY_FIELD(state, WeaponNetworkData, magazine_rounds_total, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, WeaponNetworkData, age, LUAST_FLOAT, 0);
        lua_pop(state, 1);
    }

    static void define_engine_weapon_network_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, WeaponNetwork);
        LUAS_PRIMITIVE_FIELD(state, WeaponNetwork, baseline_valid, LUAST_BOOL, 0);
        LUAS_PRIMITIVE_FIELD(state, WeaponNetwork, baseline_index, LUAST_INT8, 0);
        LUAS_PRIMITIVE_FIELD(state, WeaponNetwork, message_index, LUAST_INT8, 0);
        LUAS_OBJREF_FIELD(state, WeaponNetwork, update_baseline, WeaponNetworkData, 0);
        LUAS_PRIMITIVE_FIELD(state, WeaponNetwork, delta_valid, LUAST_BOOL, 0);
        LUAS_OBJREF_FIELD(state, WeaponNetwork, update_delta, WeaponNetworkData, 0);
        lua_pop(state, 1);
    }

    static void define_engine_weapon_object(lua_State *state) noexcept {
        LUAS_STRUCT_EXTENDS(state, WeaponObject, ItemObject);
        LUAS_PRIMITIVE_FIELD(state, WeaponObject, flags, LUAST_UINT32, 0);
        LUAS_PRIMITIVE_FIELD(state, WeaponObject, owner_unit_flags, LUAST_UINT16, 0);
        LUAS_PRIMITIVE_FIELD(state, WeaponObject, primary_trigger, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, WeaponObject, weapon_state, LUAST_INT8, 0);
        LUAS_PRIMITIVE_FIELD(state, WeaponObject, ready_ticks, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, WeaponObject, heat, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, WeaponObject, age, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, WeaponObject, illumination_fraction, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, WeaponObject, integrated_light_power, LUAST_FLOAT, 0);
        LUAS_OBJREF_FIELD(state, WeaponObject, tracked_object, ObjectHandle, 0);
        LUAS_PRIMITIVE_FIELD(state, WeaponObject, alt_shots_loaded, LUAST_INT16, 0);
        LUAS_OBJREF_ARRAY_FIELD(state, WeaponObject, triggers, WeaponObjectTrigger, 0);
        LUAS_OBJREF_ARRAY_FIELD(state, WeaponObject, magazines, WeaponObjectMagazine, 0);
        LUAS_PRIMITIVE_FIELD(state, WeaponObject, last_trigger_fire_tick, LUAST_UINT32, 0);
        LUAS_OBJREF_FIELD(state, WeaponObject, reload_starting_point, WeaponReloadStartData, 0);
        LUAS_OBJREF_FIELD(state, WeaponObject, network, WeaponNetwork, 0);
        lua_pop(state, 1);
    }

    void push_weapon_object(lua_State *state, const WeaponObject &weapon, bool readonly) noexcept {
        LUAS_PUSH_OBJECT(state, WeaponObject, &weapon, readonly);
    }

    void define_engine_weapon_types(lua_State *state) noexcept {
        define_engine_weapon_trigger_type(state);
        define_engine_weapon_magazine_type(state);
        define_engine_weapon_reload_start_data_type(state);
        define_engine_weapon_network_data_type(state);
        define_engine_weapon_network_type(state);
        define_engine_weapon_object(state);
    }
}
