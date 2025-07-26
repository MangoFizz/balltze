// SPDX-License-Identifier: GPL-3.0-only

#include "../../../../libraries/luastruct.hpp"
#include "../../../../helpers/enum.hpp"
#include "types.hpp"
#include "object.hpp"

namespace Balltze::Lua::Api::V2 {
    static void define_object_handle_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, ObjectHandle);
        LUAS_PRIMITIVE_FIELD(state, ObjectHandle, value, LUAST_UINT32, 0);
        LUAS_PRIMITIVE_FIELD(state, ObjectHandle, id, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, ObjectHandle, index, LUAST_INT16, 0);
        LUAS_METHOD_FIELD(state, ObjectHandle, "isNull", [](lua_State *state) noexcept {
            auto handle = get_table_resource_handle(state, 1);
            if(!handle.has_value()) {
                return luaL_error(state, "Invalid TableResourceHandle object.");
            }
            lua_pushboolean(state, HANDLE_IS_NULL(handle.value()));
            return 1;
        });
        lua_pop(state, 1);
    }

    void push_object_handle(lua_State *state, const ObjectHandle &handle, bool readonly) noexcept {
        LUAS_NEW_OBJECT(state, ObjectHandle, readonly);
        ObjectHandle *object_handle = LUAS_CHECK_OBJECT(state, -1, ObjectHandle);
        object_handle->value = handle.value;
    }

    std::optional<ObjectHandle> get_object_handle(lua_State *state, int idx) noexcept {
        std::optional<ObjectHandle> handle;
        if(lua_istable(state, idx) || lua_isuserdata(state, idx)) {
            lua_getfield(state, idx, "value");
            if(lua_isinteger(state, -1)) {
                handle = ObjectHandle();
                handle->value = luaL_checkinteger(state, -1);
            }
            lua_pop(state, 1);
        } 
        else if(lua_isinteger(state, idx)) {
            handle = ObjectHandle();
            handle->value = luaL_checkinteger(state, idx);
        }
        return handle;
    }

    static void define_engine_base_dynamic_object_flags_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, BaseDynamicObjectFlags);
        luastruct_new_struct_bit_field(state, "noCollision", LUAST_UINT32, 0, 0, false, false);
        luastruct_new_struct_bit_field(state, "onGround", LUAST_UINT32, 0, 1, false, false);
        luastruct_new_struct_bit_field(state, "ignoreGravity", LUAST_UINT32, 0, 2, false, false);
        luastruct_new_struct_bit_field(state, "inWater", LUAST_UINT32, 0, 3, false, false);
        luastruct_new_struct_bit_field(state, "stationary", LUAST_UINT32, 0, 5, false, false);
        luastruct_new_struct_bit_field(state, "noCollision2", LUAST_UINT32, 0, 7, false, false);
        luastruct_new_struct_bit_field(state, "hasSoundLoopingAttachment", LUAST_UINT32, 0, 9, false, false);
        luastruct_new_struct_bit_field(state, "connectedToMap", LUAST_UINT32, 0, 10, false, false);
        luastruct_new_struct_bit_field(state, "notPlacedAutomatically", LUAST_UINT32, 0, 11, false, false);
        luastruct_new_struct_bit_field(state, "isDeviceMachine", LUAST_UINT32, 0, 12, false, false);
        luastruct_new_struct_bit_field(state, "isElevator", LUAST_UINT32, 0, 13, false, false);
        luastruct_new_struct_bit_field(state, "isElevator2", LUAST_UINT32, 0, 14, false, false);
        luastruct_new_struct_bit_field(state, "isGarbage", LUAST_UINT32, 0, 15, false, false);
        luastruct_new_struct_bit_field(state, "noShadow", LUAST_UINT32, 0, 17, false, false);
        luastruct_new_struct_bit_field(state, "deleteAtDeactivation", LUAST_UINT32, 0, 18, false, false);
        luastruct_new_struct_bit_field(state, "doNotReactivate", LUAST_UINT32, 0, 19, false, false);
        luastruct_new_struct_bit_field(state, "outsideOfMap", LUAST_UINT32, 0, 20, false, false);
        luastruct_new_struct_bit_field(state, "collidable", LUAST_UINT32, 0, 22, false, false);
        luastruct_new_struct_bit_field(state, "hasCollisionModel", LUAST_UINT32, 0, 23, false, false);
        lua_pop(state, 1);
    }

    static void define_engine_base_object_network_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, BaseObjectNetwork);
        LUAS_PRIMITIVE_FIELD(state, BaseObjectNetwork, valid_position, LUAST_BOOL, 0);
        LUAS_OBJREF_FIELD(state, BaseObjectNetwork, position, VectorXYZ, 0);
        LUAS_PRIMITIVE_FIELD(state, BaseObjectNetwork, valid_forward_and_up, LUAST_BOOL, 0);
        LUAS_OBJREF_ARRAY_FIELD(state, BaseObjectNetwork, orientation, VectorXYZ, 0);
        LUAS_PRIMITIVE_FIELD(state, BaseObjectNetwork, valid_transitional_velocity, LUAST_BOOL, 0);
        LUAS_OBJREF_FIELD(state, BaseObjectNetwork, transitional_velocity, VectorXYZ, 0);
        LUAS_PRIMITIVE_FIELD(state, BaseObjectNetwork, valid_timestamp, LUAST_BOOL, 0);
        LUAS_PRIMITIVE_FIELD(state, BaseObjectNetwork, timestamp, LUAST_UINT32, 0);
        lua_pop(state, 1);
    }

    static void define_engine_scenario_location_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, ScenarioLocation);
        LUAS_PRIMITIVE_FIELD(state, ScenarioLocation, leaf_id, LUAST_INT32, 0);
        LUAS_PRIMITIVE_FIELD(state, ScenarioLocation, cluster_id, LUAST_INT16, 0);
        lua_pop(state, 1);
    }

    static void define_engine_base_object_vitals_flags_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, BaseObjectVitalsFlags);
        luastruct_new_struct_bit_field(state, "healthDamageEffectApplied", LUAST_UINT16, 0, 0, false, false);
        luastruct_new_struct_bit_field(state, "shieldDamageEffectApplied", LUAST_UINT16, 0, 1, false, false);
        luastruct_new_struct_bit_field(state, "healthDepleted", LUAST_UINT16, 0, 2, false, false);
        luastruct_new_struct_bit_field(state, "shieldDepleted", LUAST_UINT16, 0, 3, false, false);
        luastruct_new_struct_bit_field(state, "killed", LUAST_UINT16, 0, 5, false, false);
        luastruct_new_struct_bit_field(state, "killedSilent", LUAST_UINT16, 0, 6, false, false);
        luastruct_new_struct_bit_field(state, "cannotMeleeAttack", LUAST_UINT16, 0, 7, false, false);
        luastruct_new_struct_bit_field(state, "invulnerable", LUAST_UINT16, 0, 11, false, false);
        luastruct_new_struct_bit_field(state, "shieldRecharging", LUAST_UINT16, 0, 12, false, false);
        luastruct_new_struct_bit_field(state, "killedNoStats", LUAST_UINT16, 0, 13, false, false);
        lua_pop(state, 1);
    }

    static void define_engine_base_object_vitals_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, BaseObjectVitals);
        LUAS_PRIMITIVE_FIELD(state, BaseObjectVitals, base_health, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, BaseObjectVitals, base_shield, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, BaseObjectVitals, health, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, BaseObjectVitals, shield, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, BaseObjectVitals, current_shield_damage, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, BaseObjectVitals, current_health_damage, LUAST_FLOAT, 0);
        LUAS_OBJREF_FIELD(state, BaseObjectVitals, entangled_object_handle, ObjectHandle, 0);
        LUAS_PRIMITIVE_FIELD(state, BaseObjectVitals, recent_shield_damage, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, BaseObjectVitals, recent_health_damage, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, BaseObjectVitals, recent_shield_damage_time, LUAST_UINT32, 0);
        LUAS_PRIMITIVE_FIELD(state, BaseObjectVitals, recent_health_damage_time, LUAST_UINT32, 0);
        LUAS_PRIMITIVE_FIELD(state, BaseObjectVitals, shield_stun_time, LUAST_UINT16, 0);
        LUAS_OBJREF_FIELD(state, BaseObjectVitals, flags, BaseObjectVitalsFlags, 0);
        lua_pop(state, 1);
    }

    static void define_engine_base_object_attachments_data(lua_State *state) noexcept {
        LUAS_STRUCT(state, BaseObjectAttachmentsData);
        LUAS_PRIMITIVE_ARRAY_FIELD(state, BaseObjectAttachmentsData, types, LUAST_UINT8, 0);
        LUAS_PRIMITIVE_ARRAY_FIELD(state, BaseObjectAttachmentsData, attachments, LUAST_UINT32, 0);
        LUAS_PRIMITIVE_FIELD(state, BaseObjectAttachmentsData, first_widget, LUAST_UINT32, 0);
        lua_pop(state, 1);
    }

    static void define_engine_base_object_region_destroyeds_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, BaseObjectRegionDestroyeds);
        luastruct_new_struct_bit_field(state, "region0", LUAST_UINT16, 0, 0, false, false);
        luastruct_new_struct_bit_field(state, "region1", LUAST_UINT16, 0, 1, false, false);
        luastruct_new_struct_bit_field(state, "region2", LUAST_UINT16, 0, 2, false, false);
        luastruct_new_struct_bit_field(state, "region3", LUAST_UINT16, 0, 3, false, false);
        luastruct_new_struct_bit_field(state, "region4", LUAST_UINT16, 0, 4, false, false);
        luastruct_new_struct_bit_field(state, "region5", LUAST_UINT16, 0, 5, false, false);
        luastruct_new_struct_bit_field(state, "region6", LUAST_UINT16, 0, 6, false, false);
        luastruct_new_struct_bit_field(state, "region7", LUAST_UINT16, 0, 7, false, false);
        lua_pop(state, 1);
    }

    static void define_engine_base_object_block_reference_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, BaseObjectBlockReference);
        LUAS_PRIMITIVE_FIELD(state, BaseObjectBlockReference, size, LUAST_UINT16, 0);
        LUAS_PRIMITIVE_FIELD(state, BaseObjectBlockReference, offset, LUAST_UINT16, 0);
        lua_pop(state, 1);
    }

    static void define_engine_object_valid_outgoing_functions_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, ObjectValidOutGoingFunctions);
        luastruct_new_struct_bit_field(state, "a", LUAST_UINT8, 0, 0, false, false);
        luastruct_new_struct_bit_field(state, "b", LUAST_UINT8, 0, 1, false, false);
        luastruct_new_struct_bit_field(state, "c", LUAST_UINT8, 0, 2, false, false);
        luastruct_new_struct_bit_field(state, "d", LUAST_UINT8, 0, 3, false, false);
        lua_pop(state, 1);
    }

    static void define_engine_object_animation_state_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, ObjectAnimationState);
        LUAS_PRIMITIVE_FIELD(state, ObjectAnimationState, index, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, ObjectAnimationState, frame, LUAST_UINT16, 0);
        lua_pop(state, 1);
    }

    static void define_engine_animation_state_interpolation_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, AnimationStateInterpolation);
        LUAS_OBJREF_FIELD(state, AnimationStateInterpolation, state, ObjectAnimationState, 0);
        LUAS_PRIMITIVE_FIELD(state, AnimationStateInterpolation, frame_index, LUAST_FLOAT, 0);
        lua_pop(state, 1);
    }

    static void define_engine_object_animation_data_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, ObjectAnimationData);
        LUAS_OBJREF_FIELD(state, ObjectAnimationData, animation_tag_handle, TagHandle, 0);
        LUAS_OBJREF_FIELD(state, ObjectAnimationData, animation_state, ObjectAnimationState, 0);
        LUAS_PRIMITIVE_FIELD(state, ObjectAnimationData, animation_interpolation_frame, LUAST_UINT16, 0);
        LUAS_PRIMITIVE_FIELD(state, ObjectAnimationData, animation_interpolation_frame_count, LUAST_UINT16, 0);
        lua_pop(state, 1);
    }

    static void define_engine_dynamic_object_base_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, DynamicObjectBase);
        LUAS_OBJREF_FIELD(state, DynamicObjectBase, tag_handle, TagHandle, 0);
        LUAS_PRIMITIVE_FIELD(state, DynamicObjectBase, network_role, LUAST_UINT32, 0);
        LUAS_PRIMITIVE_FIELD(state, DynamicObjectBase, flags_0, LUAST_UINT32, 0);
        LUAS_PRIMITIVE_FIELD(state, DynamicObjectBase, existence_time, LUAST_UINT32, 0);
        LUAS_OBJREF_FIELD(state, DynamicObjectBase, flags_1, BaseDynamicObjectFlags, 0);
        LUAS_PRIMITIVE_FIELD(state, DynamicObjectBase, object_marker_id, LUAST_UINT32, 0);
        LUAS_OBJREF_FIELD(state, DynamicObjectBase, network, BaseObjectNetwork, 0);
        LUAS_OBJREF_FIELD(state, DynamicObjectBase, position, VectorXYZ, 0);
        LUAS_OBJREF_FIELD(state, DynamicObjectBase, velocity, VectorXYZ, 0);
        LUAS_OBJREF_ARRAY_FIELD(state, DynamicObjectBase, rotation, VectorIJK, 0);
        LUAS_OBJREF_FIELD(state, DynamicObjectBase, rotation_velocity, VectorPYR, 0);
        LUAS_OBJREF_FIELD(state, DynamicObjectBase, scenario_location, ScenarioLocation, 0);
        LUAS_OBJREF_FIELD(state, DynamicObjectBase, center, VectorXYZ, 0);
        LUAS_PRIMITIVE_FIELD(state, DynamicObjectBase, bounding_radius, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, DynamicObjectBase, scale, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, DynamicObjectBase, object_type, LUAST_UINT32, 0);
        LUAS_PRIMITIVE_FIELD(state, DynamicObjectBase, team_owner, LUAST_UINT16, 0);
        LUAS_PRIMITIVE_FIELD(state, DynamicObjectBase, name_list_index, LUAST_UINT16, 0);
        LUAS_PRIMITIVE_FIELD(state, DynamicObjectBase, moving_time, LUAST_UINT16, 0);
        LUAS_PRIMITIVE_FIELD(state, DynamicObjectBase, variant_index, LUAST_UINT16, 0);
        LUAS_OBJREF_FIELD(state, DynamicObjectBase, player, TableResourceHandle, 0);
        LUAS_OBJREF_FIELD(state, DynamicObjectBase, owner_object, ObjectHandle, 0);
        LUAS_OBJREF_FIELD(state, DynamicObjectBase, animation_data, ObjectAnimationData, 0);
        LUAS_OBJREF_FIELD(state, DynamicObjectBase, vitals, BaseObjectVitals, 0);
        LUAS_OBJREF_FIELD(state, DynamicObjectBase, cluster_partition, TableResourceHandle, 0);
        LUAS_OBJREF_FIELD(state, DynamicObjectBase, unknown_object, ObjectHandle, 0);
        LUAS_OBJREF_FIELD(state, DynamicObjectBase, next_object, ObjectHandle, 0);
        LUAS_OBJREF_FIELD(state, DynamicObjectBase, first_object, ObjectHandle, 0);
        LUAS_OBJREF_FIELD(state, DynamicObjectBase, parent_object, ObjectHandle, 0);
        LUAS_PRIMITIVE_FIELD(state, DynamicObjectBase, parent_attachment_node, LUAST_UINT8, 0);
        LUAS_PRIMITIVE_FIELD(state, DynamicObjectBase, force_shield_update, LUAST_BOOL, 0);
        LUAS_OBJREF_FIELD(state, DynamicObjectBase, valid_outgoing_functions, ObjectValidOutGoingFunctions, 0);
        LUAS_PRIMITIVE_ARRAY_FIELD(state, DynamicObjectBase, incoming_function_values, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_ARRAY_FIELD(state, DynamicObjectBase, outgoing_function_values, LUAST_FLOAT, 0);
        LUAS_OBJREF_FIELD(state, DynamicObjectBase, attachment_data, BaseObjectAttachmentsData, 0);
        LUAS_OBJREF_FIELD(state, DynamicObjectBase, cached_render_state, TableResourceHandle, 0);
        LUAS_OBJREF_FIELD(state, DynamicObjectBase, region_destroyeds, BaseObjectRegionDestroyeds, 0);
        LUAS_PRIMITIVE_FIELD(state, DynamicObjectBase, shader_permutation, LUAST_INT16, 0);
        LUAS_PRIMITIVE_ARRAY_FIELD(state, DynamicObjectBase, region_healths, LUAST_UINT8, 0);
        LUAS_PRIMITIVE_ARRAY_FIELD(state, DynamicObjectBase, region_permutation_ids, LUAST_INT8, 0);
        LUAS_OBJREF_ARRAY_FIELD(state, DynamicObjectBase, color_change, ColorRGB, 0);
        LUAS_OBJREF_ARRAY_FIELD(state, DynamicObjectBase, color_change_2, ColorRGB, 0);
        LUAS_OBJREF_ARRAY_FIELD(state, DynamicObjectBase, node_orientations, BaseObjectBlockReference, 0);
        LUAS_OBJREF_FIELD(state, DynamicObjectBase, node_matrices_block, BaseObjectBlockReference, 0);
        lua_pop(state, 1);
    }

    void push_dynamic_object_base(lua_State *state, const DynamicObjectBase &object, bool readonly) noexcept {
        LUAS_PUSH_OBJECT(state, DynamicObjectBase, &object, readonly);
    }

    std::optional<ObjectType> get_object_type(lua_State *state, int idx) noexcept {
        return GET_ENUM_VARIANT_VALUE(state, idx, ObjectType);
    }

    void define_engine_object_types(lua_State *state) noexcept {
        define_object_handle_type(state);
        define_engine_base_dynamic_object_flags_type(state);
        define_engine_base_object_network_type(state);
        define_engine_scenario_location_type(state);
        define_engine_base_object_vitals_flags_type(state);
        define_engine_base_object_vitals_type(state);
        define_engine_base_object_attachments_data(state);
        define_engine_base_object_region_destroyeds_type(state);
        define_engine_base_object_block_reference_type(state);
        define_engine_object_valid_outgoing_functions_type(state);
        define_engine_object_animation_state_type(state);
        define_engine_animation_state_interpolation_type(state);
        define_engine_object_animation_data_type(state);
        define_engine_dynamic_object_base_type(state);
    }
}
