// SPDX-License-Identifier: GPL-3.0-only

#include "../../../../libraries/luastruct.hpp"
#include "types.hpp"
#include "unit.hpp"

namespace Balltze::Lua::Api::V2 {
    static void define_engine_unit_recent_damager_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, UnitRecentDamager);
        LUAS_PRIMITIVE_FIELD(state, UnitRecentDamager, last_damage_time, LUAST_UINT32, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitRecentDamager, total_damage, LUAST_FLOAT, 0);
        LUAS_OBJREF_FIELD(state, UnitRecentDamager, object, ObjectHandle, 0);
        LUAS_OBJREF_FIELD(state, UnitRecentDamager, player, PlayerHandle, 0);
        lua_pop(state, 1);
    }

    static void define_engine_unit_object_flags_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, UnitObjectFlags);
        luastruct_new_struct_bit_field(state, "unknownBipedSpeechRelated", LUAST_UINT32, 0, 0, false, false);
        luastruct_new_struct_bit_field(state, "powerUp", LUAST_UINT32, 0, 4, false, false);
        luastruct_new_struct_bit_field(state, "powerUpAddition", LUAST_UINT32, 0, 5, false, false);
        luastruct_new_struct_bit_field(state, "controllable", LUAST_UINT32, 0, 6, false, false);
        luastruct_new_struct_bit_field(state, "berserking", LUAST_UINT32, 0, 7, false, false);
        luastruct_new_struct_bit_field(state, "unknownIntegratedLightRelated", LUAST_UINT32, 0, 19, false, false);
        luastruct_new_struct_bit_field(state, "willNotDropItems", LUAST_UINT32, 0, 20, false, false);
        luastruct_new_struct_bit_field(state, "unknown", LUAST_UINT32, 0, 21, false, false);
        luastruct_new_struct_bit_field(state, "canBlink", LUAST_UINT32, 0, 22, false, false);
        luastruct_new_struct_bit_field(state, "impervious", LUAST_UINT32, 0, 23, false ,false);
        luastruct_new_struct_bit_field(state, "suspended", LUAST_UINT32, 0, 24, false ,false);
        luastruct_new_struct_bit_field(state, "blind", LUAST_UINT32, 0, 25, false ,false);
        luastruct_new_struct_bit_field(state, "unknownNVRelated", LUAST_UINT32, 0, 26, false, false);
        luastruct_new_struct_bit_field(state, "possessed", LUAST_UINT32, 0, 27, false, false);
        luastruct_new_struct_bit_field(state, "desiresFlashlightOn", LUAST_UINT32, 0, 28, false, false);
        luastruct_new_struct_bit_field(state, "desiresFlashlightOff", LUAST_UINT32, 0, 29, false, false);
        lua_pop(state, 1);
    }

    static void define_engine_unit_control_flags_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, UnitControlFlags);
        luastruct_new_struct_bit_field(state, "crouch", LUAST_UINT32, 0, 0, false, false);
        luastruct_new_struct_bit_field(state, "jump", LUAST_UINT32, 0, 1, false, false);
        luastruct_new_struct_bit_field(state, "user1", LUAST_UINT32, 0, 2, false, false);
        luastruct_new_struct_bit_field(state, "user2", LUAST_UINT32, 0, 3, false, false);
        luastruct_new_struct_bit_field(state, "light", LUAST_UINT32, 0, 4, false, false);
        luastruct_new_struct_bit_field(state, "exactFacing", LUAST_UINT32, 0, 5, false, false);
        luastruct_new_struct_bit_field(state, "action", LUAST_UINT32, 0, 6, false, false);
        luastruct_new_struct_bit_field(state, "melee", LUAST_UINT32, 0, 7, false ,false);
        luastruct_new_struct_bit_field(state, "lookDontTurn", LUAST_UINT32, 0, 8, false ,false);
        luastruct_new_struct_bit_field(state, "forceAlert", LUAST_UINT32, 0, 9, false ,false);
        luastruct_new_struct_bit_field(state, "reload", LUAST_UINT32, 0, 10, false ,false);
        luastruct_new_struct_bit_field(state, "primaryTrigger", LUAST_UINT32, 0, 11, false ,false);
        luastruct_new_struct_bit_field(state, "secondaryTrigger", LUAST_UINT32, 0, 12, false, false);
        luastruct_new_struct_bit_field(state, "grenade", LUAST_UINT32, 0, 13, false, false);
        luastruct_new_struct_bit_field(state, "exchangeWeapon", LUAST_UINT32, 0, 14, false,false);
        lua_pop(state, 1);
    }

    static void define_engine_unit_animation_flags_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, UnitAnimationFlags);
        luastruct_new_struct_bit_field(state, "animationBit0Unknown", LUAST_UINT32, 0, 0, false, false);
        luastruct_new_struct_bit_field(state, "animationBit1Unknown", LUAST_UINT32, 0, 1, false, false);
        luastruct_new_struct_bit_field(state, "animationBit2Unknown", LUAST_UINT32, 0, 2, false, false);
        luastruct_new_struct_bit_field(state, "animationBit3Unknown", LUAST_UINT32, 0, 3, false, false);
        lua_pop(state, 1);
    }

    static void define_engine_unit_animation_data_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, UnitAnimationData);
        LUAS_OBJREF_FIELD(state, UnitAnimationData, flags, UnitAnimationFlags, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitAnimationData, unknown_some_animation_index_maybe, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitAnimationData, unknown_some_animation_index, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitAnimationData, seat_index, LUAST_UINT8, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitAnimationData, seat_weapon_index, LUAST_UINT8, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitAnimationData, weapon_type_index, LUAST_UINT8, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitAnimationData, state, LUAST_INT8, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitAnimationData, replacement_state, LUAST_INT8, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitAnimationData, overlay_state, LUAST_INT8, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitAnimationData, desired_state, LUAST_INT8, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitAnimationData, base_seat, LUAST_INT8, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitAnimationData, emotion, LUAST_INT8, 0);
        LUAS_OBJREF_FIELD(state, UnitAnimationData, replacement_animation, ObjectAnimationState, 0);
        LUAS_OBJREF_FIELD(state, UnitAnimationData, overlay_state_animation, ObjectAnimationState, 0);
        LUAS_OBJREF_FIELD(state, UnitAnimationData, weapon_ik, ObjectAnimationState, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitAnimationData, update_look, LUAST_BOOL, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitAnimationData, update_aim, LUAST_BOOL, 0);
        LUAS_OBJREF_FIELD(state, UnitAnimationData, looking_bounds, Bounds2D, 0);
        LUAS_OBJREF_FIELD(state, UnitAnimationData, aiming_bounds, Bounds2D, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitAnimationData, external_animation_graph, LUAST_UINT32, 0);
        LUAS_OBJREF_FIELD(state, UnitAnimationData, external_animation, ObjectAnimationState, 0);
        lua_pop(state, 1);
    }

    static void define_engine_ai_communication_packet_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, AiCommunicationPacket);
        LUAS_PRIMITIVE_FIELD(state, AiCommunicationPacket, type, LUAST_UINT16, 0);
        LUAS_PRIMITIVE_FIELD(state, AiCommunicationPacket, broken, LUAST_BOOL, 0);
        lua_pop(state, 1);
    }

    static void define_engine_unit_speech_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, UnitSpeech);
        LUAS_PRIMITIVE_FIELD(state, UnitSpeech, priority, LUAST_UINT16, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitSpeech, scream_type, LUAST_UINT16, 0);
        LUAS_OBJREF_FIELD(state, UnitSpeech, sound_tag, TagHandle, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitSpeech, ticks, LUAST_UINT16, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitSpeech, unk_1, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitSpeech, unk_2, LUAST_INT32, 0);
        LUAS_OBJREF_FIELD(state, UnitSpeech, ai_communication_info, AiCommunicationPacket, 0);
        lua_pop(state, 1);
    }

    static void define_engine_unit_speech_data_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, UnitSpeechData);
        LUAS_OBJREF_FIELD(state, UnitSpeechData, current, UnitSpeech, 0);
        LUAS_OBJREF_FIELD(state, UnitSpeechData, next, UnitSpeech, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitSpeechData, unk_0, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitSpeechData, unk_1, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitSpeechData, unk_2, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitSpeechData, unk_3, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitSpeechData, unk_4, LUAST_INT32, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitSpeechData, unk_6, LUAST_BOOL, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitSpeechData, unk_7, LUAST_BOOL, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitSpeechData, unk_8, LUAST_BOOL, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitSpeechData, unk_9, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitSpeechData, unk_10, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitSpeechData, unk_11, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitSpeechData, unk_12, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitSpeechData, unk_13, LUAST_INT32, 0);
        lua_pop(state, 1);
    }

    static void define_engine_unit_control_data_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, UnitControlData);
        LUAS_PRIMITIVE_FIELD(state, UnitControlData, animation_state, LUAST_INT8, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitControlData, aiming_speed, LUAST_INT8, 0);
        LUAS_OBJREF_FIELD(state, UnitControlData, control_flags, UnitControlFlags, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitControlData, weapon_index, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitControlData, grenade_index, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitControlData, zoom_index, LUAST_INT16, 0);
        LUAS_OBJREF_FIELD(state, UnitControlData, throttle, VectorIJK, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitControlData, primary_trigger, LUAST_FLOAT, 0);
        LUAS_OBJREF_FIELD(state, UnitControlData, facing_vector, VectorIJK, 0);
        LUAS_OBJREF_FIELD(state, UnitControlData, aiming_vector, VectorIJK, 0);
        LUAS_OBJREF_FIELD(state, UnitControlData, looking_vector, VectorIJK, 0);
        lua_pop(state, 1);
    }

    static void define_engine_unit_persistent_control_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, UnitPersistentControl);
        LUAS_PRIMITIVE_FIELD(state, UnitPersistentControl, ticks_remaining, LUAST_UINT32, 0);
        LUAS_OBJREF_FIELD(state, UnitPersistentControl, control_flags, UnitControlFlags, 0);
        lua_pop(state, 1);
    }

    static void define_engine_unit_damage_result_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, UnitDamageResult);
        LUAS_PRIMITIVE_FIELD(state, UnitDamageResult, category, LUAST_UINT16, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitDamageResult, ai_ticks_until_handle, LUAST_UINT16, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitDamageResult, amount, LUAST_FLOAT, 0);
        LUAS_OBJREF_FIELD(state, UnitDamageResult, responsible_unit, TagHandle, 0);
        lua_pop(state, 1);
    }

    static void define_engine_unit_object_type(lua_State *state) noexcept {
        LUAS_STRUCT_EXTENDS(state, UnitObject, DynamicObjectBase);
        LUAS_OBJREF_FIELD(state, UnitObject, actor_tag, TagHandle, 0);
        LUAS_OBJREF_FIELD(state, UnitObject, swarm_actor_tag, TagHandle, 0);
        LUAS_OBJREF_FIELD(state, UnitObject, swarm_next_unit, ObjectHandle, 0);
        LUAS_OBJREF_FIELD(state, UnitObject, swarm_previous_unit, ObjectHandle, 0);
        LUAS_OBJREF_FIELD(state, UnitObject, unit_flags, UnitObjectFlags, 0);
        LUAS_OBJREF_FIELD(state, UnitObject, unit_control_flags, UnitControlFlags, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitObject, shield_snapping, LUAST_INT8, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitObject, base_seat_index, LUAST_INT8, 0);
        LUAS_OBJREF_FIELD(state, UnitObject, persistent_control, UnitPersistentControl, 0);
        LUAS_OBJREF_FIELD(state, UnitObject, controlling_player, PlayerHandle, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitObject, ai_effect_type, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitObject, emotion_animation_index, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitObject, next_ai_effect_tick, LUAST_UINT32, 0);
        LUAS_OBJREF_FIELD(state, UnitObject, desired_facing_vector, VectorIJK, 0);
        LUAS_OBJREF_FIELD(state, UnitObject, desired_aiming_vector, VectorIJK, 0);
        LUAS_OBJREF_FIELD(state, UnitObject, aiming_vector, VectorIJK, 0);
        LUAS_OBJREF_FIELD(state, UnitObject, aiming_velocity, VectorIJK, 0);
        LUAS_OBJREF_FIELD(state, UnitObject, looking_angles, VectorPYR, 0);
        LUAS_OBJREF_FIELD(state, UnitObject, looking_vector, VectorIJK, 0);
        LUAS_OBJREF_FIELD(state, UnitObject, looking_velocity, VectorIJK, 0);
        LUAS_OBJREF_FIELD(state, UnitObject, throttle, VectorIJK, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitObject, primary_trigger, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitObject, aiming_speed, LUAST_INT8, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitObject, melee_state, LUAST_INT8, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitObject, melee_timer, LUAST_INT8, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitObject, ticks_until_flame_to_death, LUAST_INT8, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitObject, ping_animation_ticks_left, LUAST_INT8, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitObject, grenade_state, LUAST_UINT8, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitObject, unk_1, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitObject, unk_2, LUAST_INT16, 0);
        LUAS_OBJREF_FIELD(state, UnitObject, grenade_projectile, TagHandle, 0);
        LUAS_OBJREF_FIELD(state, UnitObject, animation, UnitAnimationData, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitObject, ambient, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitObject, illumination, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitObject, mouth_factor, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitObject, vehicle_seat_id, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitObject, current_weapon_id, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitObject, next_weapon_id, LUAST_INT16, 0);
        LUAS_OBJREF_ARRAY_FIELD(state, UnitObject, weapons, ObjectHandle, 0);
        LUAS_PRIMITIVE_ARRAY_FIELD(state, UnitObject, weapon_ready_ticks, LUAST_UINT32, 0);
        LUAS_OBJREF_FIELD(state, UnitObject, equipment_handle, ObjectHandle, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitObject, current_grenade_index, LUAST_INT8, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitObject, next_grenade_index, LUAST_INT8, 0);
        LUAS_PRIMITIVE_ARRAY_FIELD(state, UnitObject, grenade_counts, LUAST_UINT8, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitObject, zoom_level, LUAST_UINT8, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitObject, desired_zoom_level, LUAST_UINT8, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitObject, ticks_since_last_vehicle_speech, LUAST_INT8, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitObject, aiming_change, LUAST_UINT8, 0);
        LUAS_OBJREF_ARRAY_FIELD(state, UnitObject, powered_seats_riders, ObjectHandle, 0);
        LUAS_OBJREF_FIELD(state, UnitObject, unk_3, TableResourceHandle, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitObject, _some_tick_time, LUAST_INT32, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitObject, encounter_id, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitObject, squad_id, LUAST_INT16, 0);
        LUAS_PRIMITIVE_ARRAY_FIELD(state, UnitObject, powered_seats_power, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitObject, integrated_light_power, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitObject, integrated_light_toggle_power, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitObject, integrated_night_vision_toggle_power, LUAST_FLOAT, 0);
        LUAS_OBJREF_ARRAY_FIELD(state, UnitObject, seat_related, VectorXYZ, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitObject, camo_power, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitObject, full_spectrum_vision_power, LUAST_FLOAT, 0);
        LUAS_OBJREF_FIELD(state, UnitObject, dialogue_definition, TagHandle, 0);
        LUAS_OBJREF_FIELD(state, UnitObject, speech, UnitSpeechData, 0);
        LUAS_OBJREF_FIELD(state, UnitObject, damage_result, UnitDamageResult, 0);
        LUAS_OBJREF_FIELD(state, UnitObject, object_flame_causer, ObjectHandle, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitObject, unk_4, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitObject, died_at_tick, LUAST_UINT32, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitObject, feign_death_timer, LUAST_UINT16, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitObject, camo_regrowth, LUAST_BOOL, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitObject, stun, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitObject, stun_ticks, LUAST_UINT16, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitObject, spree_count, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitObject, spree_starting_time, LUAST_INT32, 0);
        LUAS_OBJREF_ARRAY_FIELD(state, UnitObject, recent_damage, UnitRecentDamager, 0);
        LUAS_OBJREF_FIELD(state, UnitObject, control_data, UnitControlData, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitObject, last_completed_client_update_valid, LUAST_BOOL, 0);
        LUAS_PRIMITIVE_FIELD(state, UnitObject, last_completed_client_update_id, LUAST_INT32, 0);
        lua_pop(state, 1);
    }

    static void define_engine_biped_object_flags_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, BipedObjectFlags);
        luastruct_new_struct_bit_field(state, "airborne", LUAST_UINT32, 0, 0, false, false);
        luastruct_new_struct_bit_field(state, "slipping", LUAST_UINT32, 0, 1, false, false);
        luastruct_new_struct_bit_field(state, "absolute_movement", LUAST_UINT32, 0, 2, false, false);
        luastruct_new_struct_bit_field(state, "no_collision", LUAST_UINT32, 0, 3, false, false);
        luastruct_new_struct_bit_field(state, "passes_through_other_bipeds", LUAST_UINT32, 0, 4, false, false);
        luastruct_new_struct_bit_field(state, "limping2", LUAST_UINT32, 0, 5, false, false);
        lua_pop(state, 1);
    }

    static void define_engine_biped_network_delta_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, BipedNetworkDelta);
        LUAS_PRIMITIVE_ARRAY_FIELD(state, BipedNetworkDelta, grenade_counts, LUAST_INT8, 0);
        LUAS_PRIMITIVE_FIELD(state, BipedNetworkDelta, body_vitality, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, BipedNetworkDelta, shield_vitality, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, BipedNetworkDelta, shield_stun_ticks_greater_than_zero, LUAST_BOOL, 0);
        lua_pop(state, 1);
    }

    static void define_engine_biped_network_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, BipedNetwork);
        LUAS_PRIMITIVE_FIELD(state, BipedNetwork, baseline_valid, LUAST_BOOL, 0);
        LUAS_PRIMITIVE_FIELD(state, BipedNetwork, baseline_id, LUAST_INT8, 0);
        LUAS_PRIMITIVE_FIELD(state, BipedNetwork, message_id, LUAST_INT8, 0);
        LUAS_OBJREF_FIELD(state, BipedNetwork, update_baseline, BipedNetworkDelta, 0);
        LUAS_PRIMITIVE_FIELD(state, BipedNetwork, delta_valid, LUAST_BOOL, 0);
        LUAS_OBJREF_FIELD(state, BipedNetwork, update_delta, BipedNetworkDelta, 0);
        lua_pop(state, 1);
    }

    static void define_engine_biped_object_type(lua_State *state) noexcept {
        LUAS_STRUCT_EXTENDS(state, BipedObject, UnitObject);
        LUAS_OBJREF_FIELD(state, BipedObject, biped_flags, BipedObjectFlags, 0);
        LUAS_PRIMITIVE_FIELD(state, BipedObject, landing_timer, LUAST_INT8, 0);
        LUAS_PRIMITIVE_FIELD(state, BipedObject, landing_force, LUAST_INT8, 0);
        LUAS_PRIMITIVE_FIELD(state, BipedObject, movement_state, LUAST_UINT8, 0);
        LUAS_PRIMITIVE_FIELD(state, BipedObject, unk_5, LUAST_INT32, 0);
        LUAS_PRIMITIVE_FIELD(state, BipedObject, action_flags, LUAST_UINT32, 0);
        LUAS_PRIMITIVE_FIELD(state, BipedObject, unk_6, LUAST_INT32, 0);
        LUAS_OBJREF_FIELD(state, BipedObject, biped_position, VectorXYZ, 0);
        LUAS_PRIMITIVE_FIELD(state, BipedObject, walking_counter, LUAST_INT32, 0);
        LUAS_OBJREF_FIELD(state, BipedObject, bump_object, ObjectHandle, 0);
        LUAS_PRIMITIVE_FIELD(state, BipedObject, ticks_since_last_bump, LUAST_INT8, 0);
        LUAS_PRIMITIVE_FIELD(state, BipedObject, airborne_ticks, LUAST_INT8, 0);
        LUAS_PRIMITIVE_FIELD(state, BipedObject, slipping_ticks, LUAST_INT8, 0);
        LUAS_PRIMITIVE_FIELD(state, BipedObject, digital_throttle, LUAST_INT8, 0);
        LUAS_PRIMITIVE_FIELD(state, BipedObject, jump_ticks, LUAST_INT8, 0);
        LUAS_PRIMITIVE_FIELD(state, BipedObject, melee_ticks, LUAST_INT8, 0);
        LUAS_PRIMITIVE_FIELD(state, BipedObject, melee_inflict_ticks, LUAST_INT8, 0);
        LUAS_PRIMITIVE_FIELD(state, BipedObject, unk_7, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, BipedObject, crouch_scale, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, BipedObject, unk_8, LUAST_FLOAT, 0);
        LUAS_OBJREF_FIELD(state, BipedObject, unknown_biped_physics_related, Plane3D, 0);
        LUAS_OBJREF_FIELD(state, BipedObject, network, BipedNetwork, 0);
        lua_pop(state, 1);
    }

    static void define_engine_vehicle_object_flags_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, VehicleObjectFlags);
        luastruct_new_struct_bit_field(state, "hovering", LUAST_UINT32, 0, 1, false, false);
        luastruct_new_struct_bit_field(state, "crouched", LUAST_UINT32, 0, 2, false, false);
        luastruct_new_struct_bit_field(state, "jumping", LUAST_UINT32, 0, 3, false, false);
        lua_pop(state, 1);
    }

    static void define_engine_vehicle_network_data_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, VehicleNetworkData);
        LUAS_PRIMITIVE_FIELD(state, VehicleNetworkData, at_rest, LUAST_BOOL, 0);
        LUAS_OBJREF_FIELD(state, VehicleNetworkData, position, VectorXYZ, 0);
        LUAS_OBJREF_FIELD(state, VehicleNetworkData, transitional_velocity, VectorXYZ, 0);
        LUAS_OBJREF_FIELD(state, VehicleNetworkData, angular_velocity, VectorXYZ, 0);
        LUAS_OBJREF_FIELD(state, VehicleNetworkData, forward, VectorXYZ, 0);
        LUAS_OBJREF_FIELD(state, VehicleNetworkData, up, VectorXYZ, 0);
        lua_pop(state, 1);
    }

    static void define_engine_vehicle_network_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, VehicleNetwork);
        LUAS_PRIMITIVE_FIELD(state, VehicleNetwork, time_valid, LUAST_BOOL, 0);
        LUAS_PRIMITIVE_FIELD(state, VehicleNetwork, baseline_valid, LUAST_BOOL, 0);
        LUAS_PRIMITIVE_FIELD(state, VehicleNetwork, baseline_id, LUAST_INT8, 0);
        LUAS_PRIMITIVE_FIELD(state, VehicleNetwork, message_id, LUAST_INT8, 0);
        LUAS_OBJREF_FIELD(state, VehicleNetwork, update_baseline, VehicleNetworkData, 0);
        LUAS_PRIMITIVE_FIELD(state, VehicleNetwork, delta_valid, LUAST_BOOL, 0);
        LUAS_OBJREF_FIELD(state, VehicleNetwork, update_delta, VehicleNetworkData, 0);
        LUAS_PRIMITIVE_FIELD(state, VehicleNetwork, last_moved_at_tick, LUAST_UINT32, 0);
        LUAS_PRIMITIVE_FIELD(state, VehicleNetwork, scenario_respawn_id, LUAST_INT16, 0);
        LUAS_OBJREF_FIELD(state, VehicleNetwork, respawn_position, VectorXYZ, 0);
        lua_pop(state, 1);
    }

    static void define_engine_vehicle_object(lua_State *state) noexcept {
        LUAS_STRUCT_EXTENDS(state, VehicleObject, UnitObject);
        LUAS_OBJREF_FIELD(state, VehicleObject, vehicle_flags, VehicleObjectFlags, 0);
        LUAS_PRIMITIVE_FIELD(state, VehicleObject, speed, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, VehicleObject, slide, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, VehicleObject, turn, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, VehicleObject, tire_position, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, VehicleObject, thread_position_left, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, VehicleObject, thread_position_right, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, VehicleObject, hover, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, VehicleObject, thrust, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_ARRAY_FIELD(state, VehicleObject, suspension_states, LUAST_INT8, 0);
        LUAS_OBJREF_FIELD(state, VehicleObject, hover_position, VectorXYZ, 0);
        LUAS_OBJREF_FIELD(state, VehicleObject, unk_5, VectorXYZ, 0);
        LUAS_OBJREF_FIELD(state, VehicleObject, unk_6, VectorXYZ, 0);
        LUAS_PRIMITIVE_FIELD(state, VehicleObject, unk_7, LUAST_INT32, 0);
        LUAS_OBJREF_FIELD(state, VehicleObject, network, VehicleNetwork, 0);
        lua_pop(state, 1);
    }

    void push_biped_object(lua_State *state, const BipedObject &biped, bool readonly) noexcept {
        LUAS_PUSH_OBJECT(state, BipedObject, &biped, readonly);
    }

    void push_vehicle_object(lua_State *state, const VehicleObject &vehicle, bool readonly) noexcept {
        LUAS_PUSH_OBJECT(state, VehicleObject, &vehicle, readonly);
    }

    void define_engine_unit_types(lua_State *state) noexcept {
        define_engine_unit_recent_damager_type(state);
        define_engine_unit_object_flags_type(state);
        define_engine_unit_control_flags_type(state);
        define_engine_unit_animation_flags_type(state);
        define_engine_unit_animation_data_type(state);
        define_engine_ai_communication_packet_type(state);
        define_engine_unit_speech_type(state);
        define_engine_unit_speech_data_type(state);
        define_engine_unit_control_data_type(state);
        define_engine_unit_persistent_control_type(state);
        define_engine_unit_damage_result_type(state);
        define_engine_unit_object_type(state);
        define_engine_biped_object_flags_type(state);
        define_engine_biped_network_delta_type(state);
        define_engine_biped_network_type(state);
        define_engine_biped_object_type(state);
        define_engine_vehicle_object_flags_type(state);
        define_engine_vehicle_network_data_type(state);
        define_engine_vehicle_network_type(state);
        define_engine_vehicle_object(state);
    }
}
