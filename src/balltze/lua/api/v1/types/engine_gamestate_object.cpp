// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/legacy_api/engine/game_state.hpp>
#include "../../../helpers/bitfield.hpp"
#include "../../../libraries/luacstruct.hpp"
#include "engine_types.hpp"
#include "engine_tag_data.hpp"
#include "engine_gamestate_object.hpp"

namespace Balltze::Lua::Api::V1 {
    LegacyApi::Engine::ObjectType get_object_type(lua_State *state, int index) noexcept {
        return static_cast<LegacyApi::Engine::ObjectType>(luacs_checkenumvalue(state, index, EngineObjectType));
    }

    static void define_engine_base_object_attachment_type_enum(lua_State *state) noexcept {
        luacs_newenum(state, EngineBaseObjectAttachmentType);
        luacs_enum_declare_value(state, "INVALID", LegacyApi::Engine::OBJECT_ATTACHMENT_TYPE_INVALID);
        luacs_enum_declare_value(state, "LIGHT", LegacyApi::Engine::OBJECT_ATTACHMENT_TYPE_LIGHT);
        luacs_enum_declare_value(state, "LOOPING_SOUND", LegacyApi::Engine::OBJECT_ATTACHMENT_TYPE_LOOPING_SOUND);
        luacs_enum_declare_value(state, "EFFECT", LegacyApi::Engine::OBJECT_ATTACHMENT_TYPE_EFFECT);
        luacs_enum_declare_value(state, "CONTRAIL", LegacyApi::Engine::OBJECT_ATTACHMENT_TYPE_CONTRAIL);
        luacs_enum_declare_value(state, "PARTICLE", LegacyApi::Engine::OBJECT_ATTACHMENT_TYPE_PARTICLE);
        lua_pop(state, 1);
    }

    static void define_engine_object_network_role_enum(lua_State *state) noexcept {
        luacs_newenum(state, EngineObjectNetworkRole);
        luacs_enum_declare_value(state, "MASTER", LegacyApi::Engine::OBJECT_NETWORK_ROLE_MASTER);
        luacs_enum_declare_value(state, "PUPPET", LegacyApi::Engine::OBJECT_NETWORK_ROLE_PUPPET);
        luacs_enum_declare_value(state, "LOCALLY_CONTROLLED_PUPPET", LegacyApi::Engine::OBJECT_NETWORK_ROLE_LOCALLY_CONTROLLED_PUPPET);
        luacs_enum_declare_value(state, "LOCAL_ONLY", LegacyApi::Engine::OBJECT_NETWORK_ROLE_LOCAL_ONLY);
        lua_pop(state, 1);
    }

    static void define_engine_object_flags_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineGameStateObjectFlags); 
        luacs_declare_field(state, LUACS_TINT16, NULL, "flags", sizeof(std::uint32_t), 0, 0, 0); 
        luacs_declare_method(state, "noCollision", lua_bitfield_struct_method(state, EngineGameStateObjectFlags, no_collision)); 
        luacs_declare_method(state, "onGround", lua_bitfield_struct_method(state, EngineGameStateObjectFlags, on_ground));
        luacs_declare_method(state, "ignoreGravity", lua_bitfield_struct_method(state, EngineGameStateObjectFlags, ignore_gravity));
        luacs_declare_method(state, "inWater", lua_bitfield_struct_method(state, EngineGameStateObjectFlags, in_water));
        luacs_declare_method(state, "stationaty", lua_bitfield_struct_method(state, EngineGameStateObjectFlags, stationary));
        luacs_declare_method(state, "noCollision2", lua_bitfield_struct_method(state, EngineGameStateObjectFlags, no_collision2));
        luacs_declare_method(state, "hasSoundLoopingAttachment", lua_bitfield_struct_method(state, EngineGameStateObjectFlags, has_sound_looping_attachment));
        luacs_declare_method(state, "connectedToMap", lua_bitfield_struct_method(state, EngineGameStateObjectFlags, connected_to_map));
        luacs_declare_method(state, "notPlacedAutomatically", lua_bitfield_struct_method(state, EngineGameStateObjectFlags, not_placed_automatically));
        luacs_declare_method(state, "isDeviceMachine", lua_bitfield_struct_method(state, EngineGameStateObjectFlags, is_device_machine));
        luacs_declare_method(state, "isElevator", lua_bitfield_struct_method(state, EngineGameStateObjectFlags, is_elevator));
        luacs_declare_method(state, "isElevator2", lua_bitfield_struct_method(state, EngineGameStateObjectFlags, is_elevator_2));
        luacs_declare_method(state, "isGarbage", lua_bitfield_struct_method(state, EngineGameStateObjectFlags, is_garbage));
        luacs_declare_method(state, "noShadow", lua_bitfield_struct_method(state, EngineGameStateObjectFlags, no_shadow));
        luacs_declare_method(state, "deleteAtDeactivation", lua_bitfield_struct_method(state, EngineGameStateObjectFlags, delete_at_deactivation));
        luacs_declare_method(state, "doNotReactivate", lua_bitfield_struct_method(state, EngineGameStateObjectFlags, do_not_reactivate));
        luacs_declare_method(state, "outsideOfMap", lua_bitfield_struct_method(state, EngineGameStateObjectFlags, outside_of_map));
        luacs_declare_method(state, "collidable", lua_bitfield_struct_method(state, EngineGameStateObjectFlags, collidable));
        luacs_declare_method(state, "hasCollisionModel", lua_bitfield_struct_method(state, EngineGameStateObjectFlags, has_collision_model));
        lua_pop(state, 1); 
    }

    void push_engine_object_flags(lua_State *state, LegacyApi::Engine::BaseObjectFlags *flags) noexcept {
        luacs_newobject(state, EngineGameStateObjectFlags, flags); 
    }

    static void define_engine_object_network_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineBaseObjectNetwork); 
        luacs_bool_field(state, EngineBaseObjectNetwork, valid_position, 0);
        luacs_nested_field(state, EngineBaseObjectNetwork, EngineVector3D, position, 0);
        luacs_bool_field(state, EngineBaseObjectNetwork, valid_forward_and_up, 0);
        luacs_nested_array_field(state, EngineBaseObjectNetwork, EnginePoint3D, orientation, 0);
        luacs_bool_field(state, EngineBaseObjectNetwork, valid_transitional_velocity, 0);
        luacs_nested_field(state, EngineBaseObjectNetwork, EngineVector3D, transitional_velocity, 0);
        luacs_bool_field(state, EngineBaseObjectNetwork, valid_timestamp, 0);
        luacs_unsigned_field(state, EngineBaseObjectNetwork, timestamp, 0);
        lua_pop(state, 1);
    }

    void push_engine_object_network(lua_State *state, LegacyApi::Engine::BaseObjectNetwork *network) noexcept {
        luacs_newobject(state, EngineBaseObjectNetwork, network); 
    }

    static void define_engine_scenario_location_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineScenarioLocation);
        luacs_int_field(state, EngineScenarioLocation, leaf_id, 0);
        luacs_int_field(state, EngineScenarioLocation, cluster_id, 0);
        lua_pop(state, 1);
    }

    void push_engine_scenario_location(lua_State *state, EngineScenarioLocation *location) noexcept {
        luacs_newobject(state, EngineScenarioLocation, location); 
    }

    static void define_engine_object_vitals_flags_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineBaseObjectVitalsFlags); 
        luacs_declare_field(state, LUACS_TINT16, NULL, "flags", sizeof(std::uint16_t), 0, 0, 0); 
        luacs_declare_method(state, "healthDamageEffectApplied", lua_bitfield_struct_method(state, EngineBaseObjectVitalsFlags, health_damage_effect_applied)); 
        luacs_declare_method(state, "shieldDamageEffectApplied", lua_bitfield_struct_method(state, EngineBaseObjectVitalsFlags, shield_damage_effect_applied));
        luacs_declare_method(state, "healthDepleted", lua_bitfield_struct_method(state, EngineBaseObjectVitalsFlags, health_depleted));
        luacs_declare_method(state, "shieldDepleted", lua_bitfield_struct_method(state, EngineBaseObjectVitalsFlags, shield_depleted));
        luacs_declare_method(state, "killed", lua_bitfield_struct_method(state, EngineBaseObjectVitalsFlags, killed));
        luacs_declare_method(state, "killedSilent", lua_bitfield_struct_method(state, EngineBaseObjectVitalsFlags, killed_silent));
        luacs_declare_method(state, "cannotMeleeAttack", lua_bitfield_struct_method(state, EngineBaseObjectVitalsFlags, cannot_melee_attack));
        luacs_declare_method(state, "invulnerable", lua_bitfield_struct_method(state, EngineBaseObjectVitalsFlags, invulnerable));
        luacs_declare_method(state, "shieldRecharging", lua_bitfield_struct_method(state, EngineBaseObjectVitalsFlags, shield_recharging));
        luacs_declare_method(state, "killedNoStats", lua_bitfield_struct_method(state, EngineBaseObjectVitalsFlags, killed_no_stats));
        lua_pop(state, 1); 
    }

    void push_engine_object_vitals_flags(lua_State *state, LegacyApi::Engine::BaseObjectVitalsFlags *flags) noexcept {
        luacs_newobject(state, EngineBaseObjectVitalsFlags, flags); 
    }

    static void define_engine_object_vitals_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineBaseObjectVitals);
        luacs_float_field(state, EngineBaseObjectVitals, base_health, 0);
        luacs_float_field(state, EngineBaseObjectVitals, base_shield, 0);
        luacs_float_field(state, EngineBaseObjectVitals, health, 0);
        luacs_float_field(state, EngineBaseObjectVitals, shield, 0);
        luacs_float_field(state, EngineBaseObjectVitals, current_shield_damage, 0);
        luacs_float_field(state, EngineBaseObjectVitals, current_health_damage, 0);
        luacs_unsigned_field(state, EngineBaseObjectVitals, entangled_object, 0);
        luacs_float_field(state, EngineBaseObjectVitals, recent_shield_damage, 0);
        luacs_float_field(state, EngineBaseObjectVitals, recent_health_damage, 0);
        luacs_unsigned_field(state, EngineBaseObjectVitals, recent_shield_damage_time, 0);
        luacs_unsigned_field(state, EngineBaseObjectVitals, recent_health_damage_time, 0);
        luacs_unsigned_field(state, EngineBaseObjectVitals, shield_stun_time, 0);
        luacs_nested_field(state, EngineBaseObjectVitals, EngineBaseObjectVitalsFlags, flags, 0);
        lua_pop(state, 1);
    }

    void push_engine_object_vitals(lua_State *state, LegacyApi::Engine::BaseObjectVitals *vitals) noexcept {
        luacs_newobject(state, EngineBaseObjectVitals, vitals); 
    }

    static void define_engine_object_attachments_data_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineBaseObjectAttachmentsData);
        luacs_enum_array_field(state, EngineBaseObjectAttachmentsData, EngineBaseObjectAttachmentType, types, 0);
        luacs_nested_field(state, EngineBaseObjectAttachmentsData, EngineResourceHandle, attachments, 0);
        luacs_nested_field(state, EngineBaseObjectAttachmentsData, EngineResourceHandle, first_widget, 0);
        lua_pop(state, 1);
    }

    void push_engine_object_attachments_data(lua_State *state, LegacyApi::Engine::BaseObjectAttachmentsData *data) noexcept {
        luacs_newobject(state, EngineBaseObjectAttachmentsData, data); 
    }

    static void define_engine_object_region_destroyeds_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineBaseObjectRegionDestroyeds); 
        luacs_declare_field(state, LUACS_TINT16, NULL, "flags", sizeof(std::uint16_t), 0, 0, 0); 
        luacs_declare_method(state, "region0", lua_bitfield_struct_method(state, EngineBaseObjectRegionDestroyeds, region_0)); 
        luacs_declare_method(state, "region1", lua_bitfield_struct_method(state, EngineBaseObjectRegionDestroyeds, region_1));
        luacs_declare_method(state, "region2", lua_bitfield_struct_method(state, EngineBaseObjectRegionDestroyeds, region_2));
        luacs_declare_method(state, "region3", lua_bitfield_struct_method(state, EngineBaseObjectRegionDestroyeds, region_3));
        luacs_declare_method(state, "region4", lua_bitfield_struct_method(state, EngineBaseObjectRegionDestroyeds, region_4));
        luacs_declare_method(state, "region5", lua_bitfield_struct_method(state, EngineBaseObjectRegionDestroyeds, region_5));
        luacs_declare_method(state, "region6", lua_bitfield_struct_method(state, EngineBaseObjectRegionDestroyeds, region_6));
        luacs_declare_method(state, "region7", lua_bitfield_struct_method(state, EngineBaseObjectRegionDestroyeds, region_7));
        lua_pop(state, 1); 
    }

    void push_engine_object_region_destroyeds(lua_State *state, LegacyApi::Engine::BaseObjectRegionDestroyeds *destroyeds) noexcept {
        luacs_newobject(state, EngineBaseObjectRegionDestroyeds, destroyeds); 
    }

    static void define_engine_object_block_reference_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineBaseObjectBlockReference);
        luacs_unsigned_field(state, EngineBaseObjectBlockReference, size, 0);
        luacs_unsigned_field(state, EngineBaseObjectBlockReference, offset, 0);
        lua_pop(state, 1);
    }

    void push_engine_object_block_reference(lua_State *state, LegacyApi::Engine::BaseObjectBlockReference *reference) noexcept {
        luacs_newobject(state, EngineBaseObjectBlockReference, reference); 
    }

    static void define_engine_object_valid_out_going_functions_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineObjectValidOutGoingFunctions); 
        luacs_declare_field(state, LUACS_TINT8, NULL, "flags", sizeof(std::uint8_t), 0, 0, 0); 
        luacs_declare_method(state, "a", lua_bitfield_struct_method(state, EngineObjectValidOutGoingFunctions, a)); 
        luacs_declare_method(state, "b", lua_bitfield_struct_method(state, EngineObjectValidOutGoingFunctions, b));
        luacs_declare_method(state, "c", lua_bitfield_struct_method(state, EngineObjectValidOutGoingFunctions, c));
        luacs_declare_method(state, "d", lua_bitfield_struct_method(state, EngineObjectValidOutGoingFunctions, d));
        lua_pop(state, 1); 
    }

    void push_engine_object_valid_out_going_functions(lua_State *state, LegacyApi::Engine::ObjectValidOutGoingFunctions *functions) noexcept {
        luacs_newobject(state, EngineObjectValidOutGoingFunctions, functions); 
    }

    static void define_engine_object_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineBaseObject);
        luacs_nested_field(state, EngineBaseObject, EngineResourceHandle, tag_handle, 0);
        luacs_enum_field(state, EngineBaseObject, EngineObjectNetworkRole, network_role, 0);
        luacs_declare_method(state, "shouldForceBaselineUpdate", lua_bitfield_struct_method(state, EngineBaseObject, should_force_baseline_update));
        luacs_unsigned_field(state, EngineBaseObject, existence_time, 0);
        luacs_nested_field(state, EngineBaseObject, EngineGameStateObjectFlags, flags, 0);
        luacs_unsigned_field(state, EngineBaseObject, object_marker_id, 0);
        luacs_nested_field(state, EngineBaseObject, EngineBaseObjectNetwork, network, 0);
        luacs_nested_field(state, EngineBaseObject, EnginePoint3D, position, 0);
        luacs_nested_field(state, EngineBaseObject, EnginePoint3D, velocity, 0);
        luacs_nested_array_field(state, EngineBaseObject, EnginePoint3D, orientation, 0);
        luacs_nested_field(state, EngineBaseObject, EngineEuler3DPYR, rotation_velocity, 0);
        luacs_nested_field(state, EngineBaseObject, EngineScenarioLocation, scenario_location, 0);
        luacs_nested_field(state, EngineBaseObject, EnginePoint3D, center_position, 0);
        luacs_float_field(state, EngineBaseObject, bounding_radius, 0);
        luacs_float_field(state, EngineBaseObject, scale, 0);
        luacs_enum_field(state, EngineBaseObject, EngineObjectType, type, 0);
        luacs_int_field(state, EngineBaseObject, team_owner, 0);
        luacs_unsigned_field(state, EngineBaseObject, name_list_index, 0);
        luacs_unsigned_field(state, EngineBaseObject, moving_time, 0);
        luacs_unsigned_field(state, EngineBaseObject, variant_index, 0);
        luacs_nested_field(state, EngineBaseObject, EngineResourceHandle, player, 0);
        luacs_nested_field(state, EngineBaseObject, EngineResourceHandle, owner_object, 0);
        luacs_nested_field(state, EngineBaseObject, EngineResourceHandle, animation_tag_handle, 0);
        luacs_unsigned_field(state, EngineBaseObject, animation_index, 0);
        luacs_unsigned_field(state, EngineBaseObject, animation_frame, 0);
        luacs_unsigned_field(state, EngineBaseObject, animation_interpolation_frame, 0);
        luacs_unsigned_field(state, EngineBaseObject, animation_interpolation_frame_count, 0);
        luacs_nested_field(state, EngineBaseObject, EngineBaseObjectVitals, vitals, 0);
        luacs_unsigned_field(state, EngineBaseObject, cluster_partition, 0);
        luacs_nested_field(state, EngineBaseObject, EngineResourceHandle, unknown_object, 0);
        luacs_nested_field(state, EngineBaseObject, EngineResourceHandle, next_object, 0);
        luacs_nested_field(state, EngineBaseObject, EngineResourceHandle, first_object, 0);
        luacs_nested_field(state, EngineBaseObject, EngineResourceHandle, parent_object, 0);
        luacs_unsigned_field(state, EngineBaseObject, parent_attachment_node, 0);
        luacs_bool_field(state, EngineBaseObject, force_shield_update, 0);
        luacs_nested_field(state, EngineBaseObject, EngineObjectValidOutGoingFunctions, valid_outgoing_functions, 0);
        luacs_float_array_field(state, EngineBaseObject, incoming_function_values, 0);
        luacs_float_array_field(state, EngineBaseObject, outgoing_function_values, 0);
        luacs_nested_field(state, EngineBaseObject, EngineBaseObjectAttachmentsData, attachment_data, 0);
        luacs_nested_field(state, EngineBaseObject, EngineResourceHandle, cached_render_state, 0);
        luacs_nested_field(state, EngineBaseObject, EngineBaseObjectRegionDestroyeds, region_destroyeds, 0);
        luacs_int_field(state, EngineBaseObject, shader_permutation, 0);
        luacs_unsigned_array_field(state, EngineBaseObject, region_healths, 0);
        luacs_int_array_field(state, EngineBaseObject, region_permutation_ids, 0);
        luacs_nested_array_field(state, EngineBaseObject, EngineColorRGB, color_change, 0);
        luacs_nested_array_field(state, EngineBaseObject, EngineColorRGB, color_change_2, 0);
        luacs_nested_array_field(state, EngineBaseObject, EngineBaseObjectBlockReference, node_orientations, 0);
        luacs_nested_field(state, EngineBaseObject, EngineBaseObjectBlockReference, node_matrices_block, 0);
        lua_pop(state, 1);
    }

    void push_engine_object(lua_State *state, LegacyApi::Engine::BaseObject *object) noexcept {
        luacs_newobject(state, EngineBaseObject, object); 
    }

    void define_engine_gamestate_object_types(lua_State *state) noexcept {
        define_engine_base_object_attachment_type_enum(state);
        define_engine_object_network_role_enum(state);
        define_engine_object_flags_struct(state);
        define_engine_object_network_struct(state);
        define_engine_scenario_location_struct(state);
        define_engine_object_vitals_flags_struct(state);
        define_engine_object_vitals_struct(state);
        define_engine_object_attachments_data_struct(state);
        define_engine_object_region_destroyeds_struct(state);
        define_engine_object_block_reference_struct(state);
        define_engine_object_valid_out_going_functions_struct(state);
        define_engine_object_struct(state);
    }
}
