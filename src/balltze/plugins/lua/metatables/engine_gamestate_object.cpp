// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/engine.hpp>
#include <lua.hpp>
#include "../helpers.hpp"
#include "../metatables.hpp"

namespace Balltze::Plugins {
    static int lua_engine_object_flags__index(lua_State *state) noexcept {
        auto *flags = lua_from_meta_object<Engine::BaseObjectFlags>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "noCollision") { 
            lua_pushboolean(state, flags->no_collision);
        }
        else if(field == "onGround") {
            lua_pushboolean(state, flags->on_ground);
        }
        else if(field == "ignoreGravity") {
            lua_pushboolean(state, flags->ignore_gravity);
        }
        else if(field == "inWater") {
            lua_pushboolean(state, flags->in_water);
        }
        else if(field == "stationary") {
            lua_pushboolean(state, flags->stationary);
        }
        else if(field == "noCollision2") {
            lua_pushboolean(state, flags->no_collision2);
        }
        else if(field == "hasSoundLoopingAttachment") {
            lua_pushboolean(state, flags->has_sound_looping_attachment);
        }
        else if(field == "connectedToMap") {
            lua_pushboolean(state, flags->connected_to_map);
        }
        else if(field == "notPlacedAutomatically") {
            lua_pushboolean(state, flags->not_placed_automatically);
        }
        else if(field == "isDeviceMachine") {
            lua_pushboolean(state, flags->is_device_machine);
        }
        else if(field == "isElevator") {
            lua_pushboolean(state, flags->is_elevator);
        }
        else if(field == "isElevator_2") {
            lua_pushboolean(state, flags->is_elevator_2);
        }
        else if(field == "isGarbage") {
            lua_pushboolean(state, flags->is_garbage);
        }
        else if(field == "noShadow") {
            lua_pushboolean(state, flags->no_shadow);
        }
        else if(field == "deleteAtDeactivation") {
            lua_pushboolean(state, flags->delete_at_deactivation);
        }
        else if(field == "doNotReactivate") {
            lua_pushboolean(state, flags->do_not_reactivate);
        }
        else if(field == "outsideOfMap") {
            lua_pushboolean(state, flags->outside_of_map);
        }
        else if(field == "collidable") {
            lua_pushboolean(state, flags->collidable);
        }
        else if(field == "hasCollisionModel") {
            lua_pushboolean(state, flags->has_collision_model);
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_object_flags__newindex(lua_State *state) noexcept {
        auto *flags = lua_from_meta_object<Engine::BaseObjectFlags>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_isboolean(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        }
        
        std::string field = key;
        bool value = lua_toboolean(state, 3);
        if(field == "noCollision") { 
            flags->no_collision = value;
        }
        else if(field == "onGround") {
            flags->on_ground = value;
        }
        else if(field == "ignoreGravity") {
            flags->ignore_gravity = value;
        }
        else if(field == "inWater") {
            flags->in_water = value;
        }
        else if(field == "stationary") {
            flags->stationary = value;
        }
        else if(field == "noCollision2") {
            flags->no_collision2 = value;
        }
        else if(field == "hasSoundLoopingAttachment") {
            flags->has_sound_looping_attachment = value;
        }
        else if(field == "connectedToMap") {
            flags->connected_to_map = value;
        }
        else if(field == "notPlacedAutomatically") {
            flags->not_placed_automatically = value;
        }
        else if(field == "isDeviceMachine") {
            flags->is_device_machine = value;
        }
        else if(field == "isElevator") {
            flags->is_elevator = value;
        }
        else if(field == "isElevator2") {
            flags->is_elevator_2 = value;
        }
        else if(field == "isGarbage") {
            flags->is_garbage = value;
        }
        else if(field == "noShadow") {
            flags->no_shadow = value;
        }
        else if(field == "deleteAtDeactivation") {
            flags->delete_at_deactivation = value;
        }
        else if(field == "doNotReactivate") {
            flags->do_not_reactivate = value;
        }
        else if(field == "outsideOfMap") {
            flags->outside_of_map = value;
        }
        else if(field == "collidable") {
            flags->collidable = value;
        }
        else if(field == "hasCollisionModel") {
            flags->has_collision_model = value;
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_object_flags(lua_State *state, Engine::BaseObjectFlags &flags, bool read_only) noexcept {
        lua_push_meta_object(state, &flags, lua_engine_object_flags__index, lua_engine_object_flags__newindex, read_only); 
    }

    static int lua_engine_object_network__index(lua_State *state) noexcept {
        auto *object_network = lua_from_meta_object<Engine::BaseObjectNetwork>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "validPosition") { 
            lua_pushboolean(state, object_network->valid_position);
        }
        else if(field == "position") {
            lua_push_meta_engine_vector3_d(state, object_network->position);
        }
        else if(field == "validForwardAndUp") { 
            lua_pushboolean(state, object_network->valid_forward_and_up);
        }
        else if(field == "orientation") {
            lua_newtable(state);
            lua_push_meta_engine_point3_d(state, object_network->orientation[0]);
            lua_rawseti(state, -2, 1);
            lua_push_meta_engine_point3_d(state, object_network->orientation[1]);
            lua_rawseti(state, -2, 2);
        }
        else if(field == "validTransitionalVelocity") { 
            lua_pushboolean(state, object_network->valid_transitional_velocity);
        }
        else if(field == "transitionalVelocity") {
            lua_push_meta_engine_vector3_d(state, object_network->transitional_velocity);
        }
        else if(field == "validTimestamp") { 
            lua_pushboolean(state, object_network->valid_timestamp);
        }
        else if(field == "timestamp") { 
            lua_pushinteger(state, object_network->timestamp);
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_object_network__newindex(lua_State *state) noexcept {
        auto *object_network = lua_from_meta_object<Engine::BaseObjectNetwork>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key"); 
        } 
        
        std::string field = key;
        if(field == "validPosition") { 
            if(lua_isboolean(state, 3)) {
                object_network->valid_position = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value");
            }
        }
        else if(field == "position") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "validForwardAndUp") { 
            if(lua_isboolean(state, 3)) {
                object_network->valid_forward_and_up = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value");
            }
        }
        else if(field == "orientation") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "validTransitionalVelocity") { 
            if(lua_isboolean(state, 3)) {
                object_network->valid_transitional_velocity = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value");
            }
        }
        else if(field == "transitionalVelocity") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "validTimestamp") { 
            if(lua_isboolean(state, 3)) {
                object_network->valid_timestamp = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value");
            }
        }
        else if(field == "timestamp") { 
            if(lua_isinteger(state, 3)) {
                object_network->timestamp = static_cast<Engine::TickCount32>(lua_tointeger(state, 3));
            }
            else {
                return luaL_error(state, "Invalid value");
            }
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_object_network(lua_State *state, Engine::BaseObjectNetwork &network, bool read_only) noexcept {
        lua_push_meta_object(state, &network, lua_engine_object_network__index, lua_engine_object_network__newindex, read_only); 
    }

    static int lua_engine_scenario_location__index(lua_State *state) noexcept {
        auto *location = lua_from_meta_object<Engine::ScenarioLocation>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "leafId") { 
            lua_pushinteger(state, location->leaf_id);
        }
        else if(field == "clusterId") {
            lua_pushinteger(state, location->cluster_id);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_scenario_location__newindex(lua_State *state) noexcept {
        auto *location = lua_from_meta_object<Engine::ScenarioLocation>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key"); 
        } 
        
        std::string field = key;
        if(field == "leafId") { 
            location->leaf_id = static_cast<std::int32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "clusterId") {
            location->cluster_id = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_scenario_location(lua_State *state, Engine::ScenarioLocation &location, bool read_only) noexcept {
        lua_push_meta_object(state, &location, lua_engine_scenario_location__index, lua_engine_scenario_location__newindex, read_only); 
    }

    static int lua_engine_object_vitals_flags__index(lua_State *state) noexcept {
        auto *flags = lua_from_meta_object<Engine::BaseObjectVitalsFlags>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "healthDamageEffectApplied") { 
            lua_pushboolean(state, flags->health_damage_effect_applied);
        }
        else if(field == "shieldDamageEffectApplied") { 
            lua_pushboolean(state, flags->shield_damage_effect_applied);
        }
        else if(field == "healthDepleted") { 
            lua_pushboolean(state, flags->health_depleted);
        }
        else if(field == "shieldDepleted") { 
            lua_pushboolean(state, flags->shield_depleted);
        }
        else if(field == "killed") { 
            lua_pushboolean(state, flags->killed);
        }
        else if(field == "killedSilent") { 
            lua_pushboolean(state, flags->killed_silent);
        }
        else if(field == "cannotMeleeAttack") { 
            lua_pushboolean(state, flags->cannot_melee_attack);
        }
        else if(field == "invulnerable") { 
            lua_pushboolean(state, flags->invulnerable);
        }
        else if(field == "shieldRecharging") { 
            lua_pushboolean(state, flags->shield_recharging);
        }
        else if(field == "killedNoStats") { 
            lua_pushboolean(state, flags->killed_no_stats);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_object_vitals_flags__newindex(lua_State *state) noexcept {
        auto *flags = lua_from_meta_object<Engine::BaseObjectVitalsFlags>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_isboolean(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        bool value = lua_toboolean(state, 3);
        if(field == "healthDamageEffectApplied") { 
            flags->health_damage_effect_applied = value;
        }
        else if(field == "shieldDamageEffectApplied") { 
            flags->shield_damage_effect_applied = value;
        }
        else if(field == "healthDepleted") { 
            flags->health_depleted = value;
        }
        else if(field == "shieldDepleted") { 
            flags->shield_depleted = value;
        }
        else if(field == "killed") { 
            flags->killed = value;
        }
        else if(field == "killedSilent") { 
            flags->killed_silent = value;
        }
        else if(field == "cannotMeleeAttack") { 
            flags->cannot_melee_attack = value;
        }
        else if(field == "invulnerable") { 
            flags->invulnerable = value;
        }
        else if(field == "shieldRecharging") { 
            flags->shield_recharging = value;
        }
        else if(field == "killedNoStats") { 
            flags->killed_no_stats = value;
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_object_vitals_flags(lua_State *state, Engine::BaseObjectVitalsFlags &flags, bool read_only) noexcept {
        lua_push_meta_object(state, &flags, lua_engine_object_vitals_flags__index, lua_engine_object_vitals_flags__newindex, read_only); 
    }

    static int lua_engine_object_vitals__index(lua_State *state) noexcept {
        auto *vitals = lua_from_meta_object<Engine::BaseObjectVitals>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "baseHealth") { 
            lua_pushnumber(state, vitals->base_health);
        }
        else if(field == "baseShield") {
            lua_pushnumber(state, vitals->base_shield);
        }
        else if(field == "health") {
            lua_pushnumber(state, vitals->health);
        }
        else if(field == "shield") {
            lua_pushnumber(state, vitals->shield);
        }
        else if(field == "currentShieldDamage") {
            lua_pushnumber(state, vitals->current_shield_damage);
        }
        else if(field == "currentHealthDamage") {
            lua_pushnumber(state, vitals->current_health_damage);
        }
        else if(field == "entangledObject") {
            lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&vitals->entangled_object));
        }
        else if(field == "recentShieldDamage") {
            lua_pushnumber(state, vitals->recent_shield_damage);
        }
        else if(field == "recentHealthDamage") {
            lua_pushnumber(state, vitals->recent_health_damage);
        }
        else if(field == "recentShieldDamageTime") {
            lua_pushinteger(state, vitals->recent_shield_damage_time);
        }
        else if(field == "recentHealthDamageTime") {
            lua_pushinteger(state, vitals->recent_health_damage_time);
        }
        else if(field == "shieldStunTime") {
            lua_pushinteger(state, vitals->shield_stun_time);
        }
        else if(field == "flags") {
            lua_push_meta_engine_object_vitals_flags(state, vitals->flags);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_object_vitals__newindex(lua_State *state) noexcept {
        auto *vitals = lua_from_meta_object<Engine::BaseObjectVitals>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key"); 
        } 
        
        std::string field = key;
        if(field == "baseHealth") { 
            vitals->base_health = luaL_checknumber(state, 3);
        }
        else if(field == "baseShield") {
            vitals->base_shield = luaL_checknumber(state, 3);
        }
        else if(field == "health") {
            vitals->health = luaL_checknumber(state, 3);
        }
        else if(field == "shield") {
            vitals->shield = luaL_checknumber(state, 3);
        }
        else if(field == "currentShieldDamage") {
            vitals->current_shield_damage = luaL_checknumber(state, 3);
        }
        else if(field == "currentHealthDamage") {
            vitals->current_health_damage = luaL_checknumber(state, 3);
        }
        else if(field == "entangledObject") {
            vitals->entangled_object.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "recentShieldDamage") {
            vitals->recent_shield_damage = luaL_checknumber(state, 3);
        }
        else if(field == "recentHealthDamage") {
            vitals->recent_health_damage = luaL_checknumber(state, 3);
        }
        else if(field == "recentShieldDamageTime") {
            vitals->recent_shield_damage_time = static_cast<Engine::TickCount32>(luaL_checkinteger(state, 3));
        }
        else if(field == "recentHealthDamageTime") {
            vitals->recent_health_damage_time = static_cast<Engine::TickCount32>(luaL_checkinteger(state, 3));
        }
        else if(field == "shieldStunTime") {
            vitals->shield_stun_time = static_cast<Engine::TickCount16>(luaL_checkinteger(state, 3));
        }
        else if(field == "flags") {
            return luaL_error(state, "Invalid operation");
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_object_vitals(lua_State *state, Engine::BaseObjectVitals &flags, bool read_only) noexcept {
        lua_push_meta_object(state, &flags, lua_engine_object_vitals__index, lua_engine_object_vitals__newindex, read_only); 
    }

    static int lua_engine_object_attachments_data__index(lua_State *state) noexcept {
        auto *attachments_data = lua_from_meta_object<Engine::BaseObjectAttachmentsData>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "types") { 
            lua_newtable(state);
            for(std::size_t i = 0; i < sizeof(attachments_data->types) / sizeof(attachments_data->types[0]); i++) {
                lua_pushstring(state, object_attachment_type_to_string(attachments_data->types[i]).c_str());
                lua_rawseti(state, -2, i + 1);
            }
        }
        else if(field == "attachments") {
            lua_newtable(state);
            for(std::size_t i = 0; i < sizeof(attachments_data->attachments) / sizeof(attachments_data->attachments[0]); i++) {
                lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&attachments_data->attachments[i]));
                lua_rawseti(state, -2, i + 1);
            }
        }
        else if(field == "firstWidget") {
            lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&attachments_data->first_widget));
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_object_attachments_data__newindex(lua_State *state) noexcept {
        auto *attachments_data = lua_from_meta_object<Engine::BaseObjectAttachmentsData>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_istable(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "types") { 
            try {
                for(std::size_t i = 0; i < sizeof(attachments_data->types) / sizeof(attachments_data->types[0]); i++) {
                    lua_rawgeti(state, 3, i + 1);
                    if(!lua_isnil(state, -1)) {
                        attachments_data->types[i] = object_attachment_type_from_string(luaL_checkstring(state, -1));
                    }
                    lua_pop(state, 1);
                }
            }
            catch(std::runtime_error &e) {
                return luaL_error(state, e.what());
            }
        }
        else if(field == "attachments") {
            try {
                for(std::size_t i = 0; i < sizeof(attachments_data->attachments) / sizeof(attachments_data->attachments[0]); i++) {
                    lua_rawgeti(state, 3, i + 1);
                    if(!lua_isnil(state, -1)) {
                        attachments_data->attachments[i] = static_cast<Engine::ObjectHandle>(luaL_checkinteger(state, -1));
                    }
                    lua_pop(state, 1);
                }
            }
            catch(std::runtime_error &e) {
                return luaL_error(state, e.what());
            }
        }
        else if(field == "firstWidget") {
            attachments_data->first_widget.handle = luaL_checkinteger(state, 3);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_object_attachments_data(lua_State *state, Engine::BaseObjectAttachmentsData &attachments_data, bool read_only) noexcept {
        lua_push_meta_object(state, &attachments_data, lua_engine_object_attachments_data__index, lua_engine_object_attachments_data__newindex, read_only); 
    }

    static int lua_engine_object_region_destroyeds__index(lua_State *state) noexcept {
        auto *regions = lua_from_meta_object<Engine::BaseObjectRegionDestroyeds>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "region0") { 
            lua_pushboolean(state, regions->region_0);
        }
        else if(field == "region1") {
            lua_pushboolean(state, regions->region_1);
        }
        else if(field == "region2") {
            lua_pushboolean(state, regions->region_2);
        }
        else if(field == "region3") {
            lua_pushboolean(state, regions->region_3);
        }
        else if(field == "region4") {
            lua_pushboolean(state, regions->region_4);
        }
        else if(field == "region5") {
            lua_pushboolean(state, regions->region_5);
        }
        else if(field == "region6") {
            lua_pushboolean(state, regions->region_6);
        }
        else if(field == "region7") {
            lua_pushboolean(state, regions->region_7);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_object_region_destroyeds__newindex(lua_State *state) noexcept {
        auto *regions = lua_from_meta_object<Engine::BaseObjectRegionDestroyeds>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_isboolean(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        bool value = lua_toboolean(state, 3);
        if(field == "region0") { 
            regions->region_0 = value;
        }
        else if(field == "region1") {
            regions->region_1 = value;
        }
        else if(field == "region2") {
            regions->region_2 = value;
        }
        else if(field == "region3") {
            regions->region_3 = value;
        }
        else if(field == "region4") {
            regions->region_4 = value;
        }
        else if(field == "region5") {
            regions->region_5 = value;
        }
        else if(field == "region6") {
            regions->region_6 = value;
        }
        else if(field == "region7") {
            regions->region_7 = value;
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_object_region_destroyeds(lua_State *state, Engine::BaseObjectRegionDestroyeds &regions, bool read_only) noexcept {
        lua_push_meta_object(state, &regions, lua_engine_object_region_destroyeds__index, lua_engine_object_region_destroyeds__newindex, read_only); 
    }

    static int lua_engine_object_block_reference__index(lua_State *state) noexcept {
        auto *block_reference = lua_from_meta_object<Engine::BaseObjectBlockReference>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "size") { 
            lua_pushinteger(state, block_reference->size);
        }
        else if(field == "offset") {
            lua_pushinteger(state, block_reference->offset);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_object_block_reference__newindex(lua_State *state) noexcept {
        auto *block_reference = lua_from_meta_object<Engine::BaseObjectBlockReference>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_isinteger(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "size") { 
            block_reference->size = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "offset") {
            block_reference->offset = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_object_block_reference(lua_State *state, Engine::BaseObjectBlockReference &block_reference, bool read_only) noexcept {
        lua_push_meta_object(state, &block_reference, lua_engine_object_block_reference__index, lua_engine_object_block_reference__newindex, read_only); 
    }

    extern std::string lua_engine_scenario_team_index_to_string(Engine::TagDefinitions::ScenarioTeamIndex value) noexcept; 
    extern Engine::TagDefinitions::ScenarioTeamIndex lua_engine_scenario_team_index_from_string(std::string str); 

    int lua_engine_object__index(lua_State *state) noexcept {
        auto *object = lua_from_meta_object<Engine::BaseObject>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "tagHandle") { 
            lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&object->tag_handle));
        }
        else if(field == "networkRole") {
            lua_pushstring(state, object_network_role_to_string(object->network_role).c_str());
        }
        else if(field == "shouldForceBaselineUpdate") {
            lua_pushboolean(state, object->should_force_baseline_update == 1);
        }
        else if(field == "existenceTime") {
            lua_pushinteger(state, object->existence_time);
        }
        else if(field == "flags") {
            lua_push_meta_engine_object_flags(state, object->flags);
        }
        else if(field == "objectMarkerId") {
            lua_pushinteger(state, object->object_marker_id);
        }
        else if(field == "network") {
            lua_push_meta_engine_object_network(state, object->network);
        }
        else if(field == "position") {
            lua_push_meta_engine_point3_d(state, object->position);
        }
        else if(field == "velocity") {
            lua_push_meta_engine_point3_d(state, object->velocity);
        }
        else if(field == "orientation") {
            lua_newtable(state);
            lua_push_meta_engine_point3_d(state, object->orientation[0]);
            lua_rawseti(state, -2, 1);
            lua_push_meta_engine_point3_d(state, object->orientation[1]);
            lua_rawseti(state, -2, 2);
        }
        else if(field == "rotationVelocity") {
            lua_push_meta_engine_euler3_d_p_y_r(state, object->rotation_velocity, false);
        }
        else if(field == "scenarioLocation") {
            lua_push_meta_engine_scenario_location(state, object->scenario_location);
        }
        else if(field == "centerPosition") {
            lua_push_meta_engine_point3_d(state, object->center_position);
        }
        else if(field == "boundingRadius") {
            lua_pushnumber(state, object->bounding_radius);
        }
        else if(field == "scale") {
            lua_pushnumber(state, object->scale);
        }
        else if(field == "type") {
            lua_pushstring(state, object_type_to_string(object->type).c_str());
        }
        else if(field == "teamOwner") {
            lua_pushstring(state, lua_engine_scenario_team_index_to_string(object->team_owner).c_str());
        }
        else if(field == "multiplayerTeamOwner") {
            lua_pushstring(state, network_game_multiplayer_team_to_string(object->multiplayer_team_owner).c_str());
        }
        else if(field == "nameListIndex") {
            lua_pushinteger(state, object->name_list_index);
        }
        else if(field == "movingTime") {
            lua_pushinteger(state, object->moving_time);
        }
        else if(field == "variantIndex") {
            lua_pushinteger(state, object->variant_index);
        }
        else if(field == "player") {
            lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&object->player));
        }
        else if(field == "ownerObject") {
            lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&object->owner_object));
        }
        else if(field == "animationTagHandle") {
            lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&object->animation_tag_handle));
        }
        else if(field == "animationIndex") {
            lua_pushinteger(state, object->animation_index);
        }
        else if(field == "animationFrame") {
            lua_pushinteger(state, object->animation_frame);
        }
        else if(field == "animationInterpolationFrame") {
            lua_pushinteger(state, object->animation_interpolation_frame);
        }
        else if(field == "animationInterpolationFrameCount") {
            lua_pushinteger(state, object->animation_interpolation_frame_count);
        }
        else if(field == "vitals") {
            lua_push_meta_engine_object_vitals(state, object->vitals);
        }
        else if(field == "clusterPartition") {
            lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&object->cluster_partition));
        }
        else if(field == "unknownObject") {
            lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&object->unknown_object));
        }
        else if(field == "nextObject") {
            lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&object->next_object));
        }
        else if(field == "firstObject") {
            lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&object->first_object));
        }
        else if(field == "parentObject") {
            lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&object->parent_object));
        }
        else if(field == "parentAttachmentNode") {
            lua_pushinteger(state, object->parent_attachment_node);
        }
        else if(field == "forceShieldUpdate") {
            lua_pushboolean(state, object->force_shield_update);
        }
        else if(field == "attachmentData") {
            lua_push_meta_engine_object_attachments_data(state, object->attachment_data);
        }
        else if(field == "cachedRenderState") {
            lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&object->cached_render_state));
        }
        else if(field == "regionDestroyeds") {
            lua_push_meta_engine_object_region_destroyeds(state, object->region_destroyeds);
        }
        else if(field == "shaderPermutation") {
            lua_pushinteger(state, object->shader_permutation);
        }
        else if(field == "regionHealths") {
            lua_newtable(state);
            for(std::size_t i = 0; i < sizeof(object->region_healths) / sizeof(object->region_healths[0]); i++) {
                lua_pushinteger(state, object->region_healths[i]);
                lua_rawseti(state, -2, i + 1);
            }
        }
        else if(field == "regionPermutationIds") {
            lua_newtable(state);
            for(std::size_t i = 0; i < sizeof(object->region_permutation_ids) / sizeof(object->region_permutation_ids[0]); i++) {
                lua_pushinteger(state, object->region_permutation_ids[i]);
                lua_rawseti(state, -2, i + 1);
            }
        }
        else if(field == "colorChange") {
            lua_newtable(state);
            for(std::size_t i = 0; i < sizeof(object->color_change) / sizeof(object->color_change[0]); i++) {
                lua_push_meta_engine_color_r_g_b(state, object->color_change[i]);
                lua_rawseti(state, -2, i + 1);
            }
        }
        else if(field == "colorChange2") {
            lua_newtable(state);
            for(std::size_t i = 0; i < sizeof(object->color_change_2) / sizeof(object->color_change_2[0]); i++) {
                lua_push_meta_engine_color_r_g_b(state, object->color_change_2[i]);
                lua_rawseti(state, -2, i + 1);
            }
        }
        else if(field == "nodeOrientations") {
            lua_newtable(state);
            lua_push_meta_engine_object_block_reference(state, object->node_orientations[0]);
            lua_rawseti(state, -2, 1);
            lua_push_meta_engine_object_block_reference(state, object->node_orientations[1]);
            lua_rawseti(state, -2, 2);
        }
        else if(field == "nodeMatricesBlock") {
            lua_push_meta_engine_object_block_reference(state, object->node_matrices_block);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    int lua_engine_object__newindex(lua_State *state) noexcept {
        auto *object = lua_from_meta_object<Engine::BaseObject>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key"); 
        } 
        
        std::string field = key;
        if(field == "tagHandle") { 
            object->tag_handle.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "networkRole") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "shouldForceBaselineUpdate") {
            object->should_force_baseline_update = luaL_checkinteger(state, 3);
        }
        else if(field == "existenceTime") {
            object->existence_time = static_cast<Engine::TickCount32>(luaL_checkinteger(state, 3));
        }
        else if(field == "flags") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "objectMarkerId") {
            object->object_marker_id = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "network") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "position") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "velocity") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "orientation") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "rotationVelocity") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "scenarioLocation") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "centerPosition") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "boundingRadius") {
            object->bounding_radius = luaL_checknumber(state, 3);
        }
        else if(field == "scale") {
            object->scale = luaL_checknumber(state, 3);
        }
        else if(field == "type") {
            try {
                object->type = object_type_from_string(luaL_checkstring(state, 3));
            }
            catch(std::runtime_error &e) {
                return luaL_error(state, e.what());
            }
        }
        else if(field == "teamOwner") {
            try {
                object->team_owner = lua_engine_scenario_team_index_from_string(luaL_checkstring(state, 3));
            }
            catch(std::runtime_error &e) {
                return luaL_error(state, e.what());
            }
        }
        else if(field == "multiplayerTeamOwner") {
            try {
                object->multiplayer_team_owner = network_game_multiplayer_team_from_string(luaL_checkstring(state, 3));
            }
            catch(std::runtime_error &e) {
                return luaL_error(state, e.what());
            }
        }
        else if(field == "nameListIndex") {
            object->name_list_index = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "movingTime") {
            object->moving_time = static_cast<Engine::TickCount16>(luaL_checkinteger(state, 3));
        }
        else if(field == "variantIndex") {
            object->variant_index = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "player") {
            object->player.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "ownerObject") {
            object->owner_object.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "animationTagHandle") {
            object->animation_tag_handle.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "animationIndex") {
            object->animation_index = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "animationFrame") {
            object->animation_frame = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "animationInterpolationFrame") {
            object->animation_interpolation_frame = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "animationInterpolationFrameCount") {
            object->animation_interpolation_frame_count = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "vitals") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "clusterPartition") {
            object->cluster_partition.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "unknownObject") {
            object->unknown_object.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "nextObject") {
            object->next_object.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "firstObject") {
            object->first_object.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "parentObject") {
            object->parent_object.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "parentAttachmentNode") {
            object->parent_attachment_node = static_cast<std::uint8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "forceShieldUpdate") {
            if(lua_isboolean(state, 3)) {
                object->force_shield_update = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "attachmentData") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "cachedRenderState") {
            object->cached_render_state.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "regionDestroyeds") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "shaderPermutation") {
            object->shader_permutation = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "regionHealths") {
            for(std::size_t i = 0; i < sizeof(object->region_healths) / sizeof(object->region_healths[0]); i++) {
                lua_rawgeti(state, 3, i + 1);
                if(!lua_isnil(state, -1)) {
                    object->region_healths[i] = static_cast<std::uint8_t>(luaL_checkinteger(state, -1));
                }
                lua_pop(state, 1);
            }
        }
        else if(field == "regionPermutationIds") {
            for(std::size_t i = 0; i < sizeof(object->region_permutation_ids) / sizeof(object->region_permutation_ids[0]); i++) {
                lua_rawgeti(state, 3, i + 1);
                if(!lua_isnil(state, -1)) {
                    object->region_permutation_ids[i] = static_cast<std::int8_t>(luaL_checkinteger(state, -1));
                }
                lua_pop(state, 1);
            }
        }
        else if(field == "colorChange") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "colorChange2") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "nodeOrientations") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "nodeMatricesBlock") {
            return luaL_error(state, "Invalid operation");
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_object(lua_State *state, Engine::BaseObject &object, bool read_only) noexcept {
        lua_push_meta_object(state, &object, lua_engine_object__index, lua_engine_object__newindex, read_only); 
    }
}
