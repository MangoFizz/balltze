// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/engine.hpp>
#include <lua.hpp>
#include "../helpers.hpp"
#include "../metatables.hpp"

namespace Balltze::Plugins {
    int lua_engine_object__index(lua_State *state) noexcept;
    int lua_engine_object__newindex(lua_State *state) noexcept;
    
    static int lua_engine_item_object__index(lua_State *state) noexcept {
        auto *item = lua_from_meta_object<Engine::ItemObject>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        }

        std::string field = key;
        if(field == "flags") {
            lua_pushinteger(state, item->flags);
        }
        else if(field == "ticksUntilDetonation") {
            lua_pushinteger(state, item->ticks_until_detonation);
        }
        else if(field == "bspCollisionSurfaceId") {
            lua_pushinteger(state, item->bsp_collision.surface_id);
        }
        else if(field == "bspCollisionReferenceId") {
            lua_pushinteger(state, item->bsp_collision.reference_id);
        }
        else if(field == "droppedByUnit") {
            lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&item->dropped_by_unit));
        }
        else if(field == "lastUpdateTick") {
            lua_pushinteger(state, item->last_update_tick);
        }
        else if(field == "objectCollisionObject") {
            lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&item->object_collision.object));
        }
        else if(field == "objectCollisionObjectPosition") {
            lua_push_meta_engine_vector3_d(state, item->object_collision.object_position);
        }
        else if(field == "unknownCollisionPosition") {
            lua_push_meta_engine_vector3_d(state, item->unknown_collision_position);
        }
        else if(field == "unknownCollisionAngle") {
            lua_push_meta_engine_euler2_d(state, item->unknown_collision_angle);
        }
        else {
            return lua_engine_object__index(state);
        }
        return 1;
    }

    static int lua_engine_item_object__newindex(lua_State *state) noexcept {
        auto *item = lua_from_meta_object<Engine::ItemObject>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_istable(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        }

        std::string field = key;
        if(field == "flags") {
            item->flags = static_cast<std::uint32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "ticksUntilDetonation") {
            item->ticks_until_detonation = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "bspCollisionSurfaceId") {
            item->bsp_collision.surface_id = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "bspCollisionReferenceId") {
            item->bsp_collision.reference_id = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "droppedByUnit") {
            item->dropped_by_unit.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "lastUpdateTick") {
            item->last_update_tick = static_cast<std::int32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "objectCollisionObject") {
            item->object_collision.object.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "objectCollisionObjectPosition") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "unknownCollisionPosition") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "unknownCollisionAngle") {
            return luaL_error(state, "Invalid operation");
        }
        else {
            return lua_engine_object__newindex(state);
        }
        return 0;
    }

    void lua_push_meta_engine_item_object(lua_State *state, Engine::ItemObject &object, bool read_only) noexcept {
        lua_push_meta_object(state, &object, lua_engine_item_object__index, lua_engine_item_object__newindex, read_only);
    }

    static int lua_engine_garbage_object__index(lua_State *state) noexcept {
        auto *garbage = lua_from_meta_object<Engine::GarbageObject>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        }

        std::string field = key;
        if(field == "ticksUntilGarbageCollection") {
            lua_pushinteger(state, garbage->ticks_until_garbage_collection);
        }
        else {
            return lua_engine_item_object__index(state);
        }
        return 1;
    }

    static int lua_engine_garbage_object__newindex(lua_State *state) noexcept {
        auto *garbage = lua_from_meta_object<Engine::GarbageObject>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_istable(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        }

        std::string field = key;
        if(field == "ticksUntilGarbageCollection") {
            garbage->ticks_until_garbage_collection = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else {
            return lua_engine_item_object__newindex(state);
        }
        return 0;
    }

    void lua_push_meta_engine_garbage_object(lua_State *state, Engine::GarbageObject &object, bool read_only) noexcept {
        lua_push_meta_object(state, &object, lua_engine_garbage_object__index, lua_engine_garbage_object__newindex, read_only);
    }

    static int lua_engine_weapon_trigger__index(lua_State *state) noexcept {
        auto *trigger = lua_from_meta_object<Engine::WeaponTrigger>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        }

        std::string field = key;
        if(field == "idleTime") {
            lua_pushinteger(state, trigger->idle_time);
        }
        else if(field == "state") {
            lua_pushstring(state, weapon_state_to_string(trigger->state).c_str());
        }
        else if(field == "triggerTime") {
            lua_pushinteger(state, trigger->trigger_time);
        }
        else if(field == "notFiring") {
            lua_pushinteger(state, trigger->not_firing);
        }
        else if(field == "autoReload") {
            lua_pushinteger(state, trigger->auto_reload);
        }
        else if(field == "roundsSinceLastTracer") {
            lua_pushinteger(state, trigger->rounds_since_last_tracer);
        }
        else if(field == "rateOfFire") {
            lua_pushnumber(state, trigger->rate_of_fire);
        }
        else if(field == "ejectionPortRecoveryTime") {
            lua_pushnumber(state, trigger->ejection_port_recovery_time);
        }
        else if(field == "illuminationRecoveryTime") {
            lua_pushnumber(state, trigger->illumination_recovery_time);
        }
        else if(field == "projectileErrorRelated") {
            lua_pushnumber(state, trigger->projectile_error_related);
        }
        else if(field == "charingEffect") {
            lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&trigger->charing_effect));
        }
        else if(field == "networkDelayTicks") {
            lua_pushinteger(state, trigger->network_delay_ticks);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_weapon_trigger__newindex(lua_State *state) noexcept {
        auto *trigger = lua_from_meta_object<Engine::WeaponTrigger>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_istable(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        }

        std::string field = key;
        if(field == "idleTime") {
            trigger->idle_time = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "state") {
            trigger->state = weapon_state_from_string(luaL_checkstring(state, 3));
        }
        else if(field == "triggerTime") {
            trigger->trigger_time = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "notFiring") {
            trigger->not_firing = static_cast<std::uint32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "autoReload") {
            trigger->auto_reload = static_cast<std::uint32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "roundsSinceLastTracer") {
            trigger->rounds_since_last_tracer = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "rateOfFire") {
            trigger->rate_of_fire = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "ejectionPortRecoveryTime") {
            trigger->ejection_port_recovery_time = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "illuminationRecoveryTime") {
            trigger->illumination_recovery_time = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "projectileErrorRelated") {
            trigger->projectile_error_related = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "charingEffect") {
            trigger->charing_effect.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "networkDelayTicks") {
            trigger->network_delay_ticks = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_weapon_trigger(lua_State *state, Engine::WeaponTrigger &trigger, bool read_only) noexcept {
        lua_push_meta_object(state, &trigger, lua_engine_weapon_trigger__index, lua_engine_weapon_trigger__newindex, read_only);
    }

    static int lua_engine_weapon_magazine__index(lua_State *state) noexcept {
        auto *magazine = lua_from_meta_object<Engine::WeaponMagazine>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        }

        std::string field = key;
        if(field == "state") {
            lua_pushstring(state, weapon_magazine_state_to_string(magazine->state).c_str());
        }
        else if(field == "reloadTicksRemaining") {
            lua_pushinteger(state, magazine->reload_ticks_remaining);
        }
        else if(field == "reloadTicks") {
            lua_pushinteger(state, magazine->reload_ticks);
        }
        else if(field == "roundsUnloaded") {
            lua_pushinteger(state, magazine->rounds_unloaded);
        }
        else if(field == "roundsLoaded") {
            lua_pushinteger(state, magazine->rounds_loaded);
        }
        else if(field == "roundsLeftToRecharge") {
            lua_pushinteger(state, magazine->rounds_left_to_recharge);
        }
        else if(field == "unknown") {
            lua_pushinteger(state, magazine->unknown);
        }
        else if(field == "unknown2") {
            lua_pushinteger(state, magazine->unknown2);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_weapon_magazine__newindex(lua_State *state) noexcept {
        auto *magazine = lua_from_meta_object<Engine::WeaponMagazine>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_istable(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        }

        std::string field = key;
        if(field == "state") {
            magazine->state = weapon_magazine_state_from_string(luaL_checkstring(state, 3));
        }
        else if(field == "reloadTicksRemaining") {
            magazine->reload_ticks_remaining = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "reloadTicks") {
            magazine->reload_ticks = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "roundsUnloaded") {
            magazine->rounds_unloaded = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "roundsLoaded") {
            magazine->rounds_loaded = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "roundsLeftToRecharge") {
            magazine->rounds_left_to_recharge = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "unknown") {
            magazine->unknown = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "unknown2") {
            magazine->unknown2 = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_weapon_magazine(lua_State *state, Engine::WeaponMagazine &magazine, bool read_only) noexcept {
        lua_push_meta_object(state, &magazine, lua_engine_weapon_magazine__index, lua_engine_weapon_magazine__newindex, read_only);
    }

    static int lua_engine_weapon_reload_start_data__index(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<Engine::WeaponReloadStartData>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "totalRounds") {
            lua_newtable(state);
            for(std::size_t i = 0; i < sizeof(data->total_rounds) / sizeof(data->total_rounds[0]); i++) {
                lua_pushinteger(state, data->total_rounds[i]);
                lua_rawseti(state, -2, i + 1);
            }
        }
        else if(field == "loadedRounds") {
            lua_newtable(state);
            for(std::size_t i = 0; i < sizeof(data->loaded_rounds) / sizeof(data->loaded_rounds[0]); i++) {
                lua_pushinteger(state, data->loaded_rounds[i]);
                lua_rawseti(state, -2, i + 1);
            }
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_weapon_reload_start_data__newindex(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<Engine::WeaponReloadStartData>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_istable(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "totalRounds") {
            for(std::size_t i = 0; i < sizeof(data->total_rounds) / sizeof(data->total_rounds[0]); i++) {
                lua_rawgeti(state, 3, i + 1);
                if(!lua_isnil(state, -1)) {
                    data->total_rounds[i] = static_cast<std::int16_t>(luaL_checkinteger(state, -1));
                }
                lua_pop(state, 1);
            }
        }
        else if(field == "loadedRounds") {
            for(std::size_t i = 0; i < sizeof(data->loaded_rounds) / sizeof(data->loaded_rounds[0]); i++) {
                lua_rawgeti(state, 3, i + 1);
                if(!lua_isnil(state, -1)) {
                    data->loaded_rounds[i] = static_cast<std::int16_t>(luaL_checkinteger(state, -1));
                }
                lua_pop(state, 1);
            }
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_weapon_reload_start_data(lua_State *state, Engine::WeaponReloadStartData &data, bool read_only) noexcept {
        lua_push_meta_object(state, &data, lua_engine_weapon_reload_start_data__index, lua_engine_weapon_reload_start_data__newindex, read_only);
    }

    static int lua_engine_weapon_network_data__index(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<Engine::WeaponNetworkData>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "position") {
            lua_push_meta_engine_vector3_d(state, data->position);
        } 
        else if(field == "transitionalVelocity") {
            lua_push_meta_engine_vector3_d(state, data->transitional_velocity);
        }
        else if(field == "angularVelocity") {
            lua_push_meta_engine_vector3_d(state, data->angular_velocity);
        }
        else if(field == "magazineRoundsTotal") {
            lua_newtable(state);
            for(std::size_t i = 0; i < sizeof(data->magazine_rounds_total) / sizeof(data->magazine_rounds_total[0]); i++) {
                lua_pushinteger(state, data->magazine_rounds_total[i]);
                lua_rawseti(state, -2, i + 1);
            }
        }
        else if(field == "age") {
            lua_pushnumber(state, data->age);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_weapon_network_data__newindex(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<Engine::WeaponNetworkData>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 

        std::string field = key;
        if(field == "position") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "transitionalVelocity") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "angularVelocity") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "magazineRoundsTotal") {
            for(std::size_t i = 0; i < sizeof(data->magazine_rounds_total) / sizeof(data->magazine_rounds_total[0]); i++) {
                lua_rawgeti(state, 3, i + 1);
                if(!lua_isnil(state, -1)) {
                    data->magazine_rounds_total[i] = static_cast<std::int16_t>(luaL_checkinteger(state, -1));
                }
                lua_pop(state, 1);
            }
        }
        else if(field == "age") {
            data->age = static_cast<float>(luaL_checknumber(state, 3));
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_weapon_network_data(lua_State *state, Engine::WeaponNetworkData &data, bool read_only) noexcept {
        lua_push_meta_object(state, &data, lua_engine_weapon_network_data__index, lua_engine_weapon_network_data__newindex, read_only);
    }

    static int lua_engine_weapon_network__index(lua_State *state) noexcept {
        auto *network = lua_from_meta_object<Engine::WeaponNetwork>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "baselineValid") {
            lua_pushboolean(state, network->baseline_valid);
        }
        else if(field == "baselineIndex") {
            lua_pushinteger(state, network->baseline_index);
        }
        else if(field == "messageIndex") {
            lua_pushinteger(state, network->message_index);
        }
        else if(field == "updateBaseline") {
            lua_push_meta_engine_weapon_network_data(state, network->update_baseline);
        }
        else if(field == "deltaValid") {
            lua_pushboolean(state, network->delta_valid);
        }
        else if(field == "updateDelta") {
            lua_push_meta_engine_weapon_network_data(state, network->update_delta);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_weapon_network__newindex(lua_State *state) noexcept {
        auto *network = lua_from_meta_object<Engine::WeaponNetwork>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 

        std::string field = key;
        if(field == "baselineValid") {
            if(lua_isboolean(state, 3)) {
                network->baseline_valid = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "baselineIndex") {
            network->baseline_index = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "messageIndex") {
            network->message_index = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "updateBaseline") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "deltaValid") {
            if(lua_isboolean(state, 3)) {
                network->delta_valid = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "updateDelta") {
            return luaL_error(state, "Invalid operation");
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_weapon_network(lua_State *state, Engine::WeaponNetwork &network, bool read_only) noexcept {
        lua_push_meta_object(state, &network, lua_engine_weapon_network__index, lua_engine_weapon_network__newindex, read_only);
    }

    static int lua_engine_weapon_object__index(lua_State *state) noexcept {
        auto *weapon = lua_from_meta_object<Engine::WeaponObject>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "flags") {
            lua_pushinteger(state, weapon->flags);
        }
        else if(field == "ownerUnitFlags") {
            lua_pushinteger(state, weapon->owner_unit_flags);
        }
        else if(field == "primaryTrigger") {
            lua_pushnumber(state, weapon->primary_trigger);
        }
        else if(field == "weaponState") {
            lua_pushstring(state, weapon_state_to_string(weapon->weapon_state).c_str());
        }
        else if(field == "readyTicks") {
            lua_pushinteger(state, weapon->ready_ticks);
        }
        else if(field == "heat") {
            lua_pushnumber(state, weapon->heat);
        }
        else if(field == "age") {
            lua_pushnumber(state, weapon->age);
        }
        else if(field == "illuminationFraction") {
            lua_pushnumber(state, weapon->illumination_fraction);
        }
        else if(field == "integratedLightPower") {
            lua_pushnumber(state, weapon->integrated_light_power);
        }
        else if(field == "trackedObject") {
            lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&weapon->tracked_object));
        }
        else if(field == "altShotsLoaded") {
            lua_pushinteger(state, weapon->alt_shots_loaded);
        }
        else if(field == "triggers") {
            lua_newtable(state);
            for(std::size_t i = 0; i < sizeof(weapon->triggers) / sizeof(weapon->triggers[0]); i++) {
                lua_push_meta_engine_weapon_trigger(state, weapon->triggers[i]);
                lua_rawseti(state, -2, i + 1);
            }
        }
        else if(field == "magazines") {
            lua_newtable(state);
            for(std::size_t i = 0; i < sizeof(weapon->magazines) / sizeof(weapon->magazines[0]); i++) {
                lua_push_meta_engine_weapon_magazine(state, weapon->magazines[i]);
                lua_rawseti(state, -2, i + 1);
            }
        }
        else if(field == "lastTriggerFireTick") {
            lua_pushinteger(state, weapon->last_trigger_fire_tick);
        }
        else if(field == "reloadStartingPoint") {
            lua_push_meta_engine_weapon_reload_start_data(state, weapon->reload_starting_point);
        }
        else if(field == "network") {
            lua_push_meta_engine_weapon_network(state, weapon->network);
        }
        else {
            return lua_engine_item_object__index(state);
        }
        return 1;
    }

    static int lua_engine_weapon_object__newindex(lua_State *state) noexcept {
        auto *weapon = lua_from_meta_object<Engine::WeaponObject>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_istable(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "flags") {
            weapon->flags = static_cast<std::uint32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "ownerUnitFlags") {
            weapon->owner_unit_flags = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "primaryTrigger") {
            weapon->primary_trigger = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "weaponState") {
            weapon->weapon_state = weapon_state_from_string(luaL_checkstring(state, 3));
        }
        else if(field == "readyTicks") {
            weapon->ready_ticks = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "heat") {
            weapon->heat = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "age") {
            weapon->age = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "illuminationFraction") {
            weapon->illumination_fraction = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "integratedLightPower") {
            weapon->integrated_light_power = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "trackedObject") {
            weapon->tracked_object.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "altShotsLoaded") {
            weapon->alt_shots_loaded = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "triggers") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "magazines") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "lastTriggerFireTick") {
            weapon->last_trigger_fire_tick = static_cast<std::uint32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "reloadStartingPoint") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "network") {
            return luaL_error(state, "Invalid operation");
        }
        else {
            return lua_engine_item_object__newindex(state);
        }
        return 0;
    }

    void lua_push_meta_engine_weapon_object(lua_State *state, Engine::WeaponObject &object, bool read_only) noexcept {
        lua_push_meta_object(state, &object, lua_engine_weapon_object__index, lua_engine_weapon_object__newindex, read_only);
    }

    static int lua_engine_equipment_network_data__index(lua_State *state) noexcept {
        auto *equipment_network_data = lua_from_meta_object<Engine::EquipmentNetworkData>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "position") {
            lua_push_meta_engine_vector3_d(state, equipment_network_data->position);
        }
        else if(field == "transitionalVelocity") {
            lua_push_meta_engine_vector3_d(state, equipment_network_data->transitional_velocity);
        }
        else if(field == "angularVelocity") {
            lua_push_meta_engine_vector3_d(state, equipment_network_data->angular_velocity);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

     static int lua_engine_equipment_network_data__newindex(lua_State *state) noexcept {
        auto *equipment_network_data = lua_from_meta_object<Engine::EquipmentNetworkData>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "position") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "transitionalVelocity") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "angularVelocity") {
            return luaL_error(state, "Invalid operation");
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_equipment_network_data(lua_State *state, Engine::EquipmentNetworkData &data, bool read_only) noexcept {
        lua_push_meta_object(state, &data, lua_engine_equipment_network_data__index, lua_engine_equipment_network_data__newindex, read_only);
    }

    static int lua_engine_equipment_network__index(lua_State *state) noexcept {
        auto *equipment_network = lua_from_meta_object<Engine::EquipmentNetwork>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "baselineValid") {
            lua_pushboolean(state, equipment_network->baseline_valid);
        }
        else if(field == "baselineIndex") {
            lua_pushinteger(state, equipment_network->baseline_index);
        }
        else if(field == "messageIndex") {
            lua_pushinteger(state, equipment_network->message_index);
        }
        else if(field == "updateBaseline") {
            lua_push_meta_engine_equipment_network_data(state, equipment_network->update_baseline);
        }
        else if(field == "deltaValid") {
            lua_pushboolean(state, equipment_network->delta_valid);
        }
        else if(field == "updateDelta") {
            lua_push_meta_engine_equipment_network_data(state, equipment_network->update_delta);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_equipment_network__newindex(lua_State *state) noexcept {
        auto *equipment_network = lua_from_meta_object<Engine::EquipmentNetwork>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_istable(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "baselineValid") {
            if(lua_isboolean(state, 3)) {
                equipment_network->baseline_valid = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "baselineIndex") {
            equipment_network->baseline_index = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "messageIndex") {
            equipment_network->message_index = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "updateBaseline") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "deltaValid") {
            if(lua_isboolean(state, 3)) {
                equipment_network->delta_valid = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "updateDelta") {
            return luaL_error(state, "Invalid operation");
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_equipment_network(lua_State *state, Engine::EquipmentNetwork &network, bool read_only) noexcept {
        lua_push_meta_object(state, &network, lua_engine_equipment_network__index, lua_engine_equipment_network__newindex, read_only);
    }

    static int lua_engine_equipment_object__index(lua_State *state) noexcept {
        auto *equipment = lua_from_meta_object<Engine::EquipmentObject>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "network") {
            lua_push_meta_engine_equipment_network(state, equipment->network);
        }
        else {
            return lua_engine_item_object__index(state);
        }
        return 1;
    }

    static int lua_engine_equipment_object__newindex(lua_State *state) noexcept {
        auto *equipment = lua_from_meta_object<Engine::EquipmentObject>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_istable(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "network") {
            return luaL_error(state, "Invalid operation");
        }
        else {
            return lua_engine_item_object__newindex(state);
        }
        return 0;
    }

    void lua_push_meta_engine_equipment_object(lua_State *state, Engine::EquipmentObject &object, bool read_only) noexcept {
        lua_push_meta_object(state, &object, lua_engine_equipment_object__index, lua_engine_equipment_object__newindex, read_only);
    }

    static int lua_engine_projectile_object_flags__index(lua_State *state) noexcept {
        auto *flags = lua_from_meta_object<Engine::ProjectileObjectFlags>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        }
        
        std::string field = key;
        if(field == "tracer") {
            lua_pushboolean(state, flags->tracer);
        }
        else if(field == "projectileUnknownBit") {
            lua_pushboolean(state, flags->projectile_unknown_bit);
        }
        else if(field == "attached") {
            lua_pushboolean(state, flags->attached);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_projectile_object_flags__newindex(lua_State *state) noexcept {
        auto *flags = lua_from_meta_object<Engine::ProjectileObjectFlags>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_isboolean(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        }
        
        std::string field = key;
        if(field == "tracer") {
            flags->tracer = lua_toboolean(state, 3);
        }
        else if(field == "projectileUnknownBit") {
            flags->projectile_unknown_bit = lua_toboolean(state, 3);
        }
        else if(field == "attached") {
            flags->attached = lua_toboolean(state, 3);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_projectile_object_flags(lua_State *state, Engine::ProjectileObjectFlags &flags, bool read_only) noexcept {
        lua_push_meta_object(state, &flags, lua_engine_projectile_object_flags__index, lua_engine_projectile_object_flags__newindex, read_only);
    }

    static int lua_engine_projectile_network_data__index(lua_State *state) noexcept {
        auto *flags = lua_from_meta_object<Engine::ProjectileNetworkData>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        }
        
        std::string field = key;
        if(field == "position") {
            lua_push_meta_engine_vector3_d(state, flags->position);
        }
        else if(field == "transitionalVelocity") {
            lua_push_meta_engine_vector3_d(state, flags->transitional_velocity);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_projectile_network_data__newindex(lua_State *state) noexcept {
        auto *flags = lua_from_meta_object<Engine::ProjectileNetworkData>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        }
        
        std::string field = key;
        if(field == "position") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "transitionalVelocity") {
            return luaL_error(state, "Invalid operation");
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_projectile_network_data(lua_State *state, Engine::ProjectileNetworkData &data, bool read_only) noexcept {
        lua_push_meta_object(state, &data, lua_engine_projectile_network_data__index, lua_engine_projectile_network_data__newindex, read_only);
    }

    static int lua_engine_projectile_network__index(lua_State *state) noexcept {
        auto *network = lua_from_meta_object<Engine::ProjectileNetwork>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        }
        
        std::string field = key;
        if(field == "unknown") {
            lua_pushboolean(state, network->unknown);
        }
        else if(field == "baselineValid") {
            lua_pushboolean(state, network->baseline_valid);
        }
        else if(field == "baselineIndex") {
            lua_pushinteger(state, network->baseline_index);
        }
        else if(field == "messageIndex") {
            lua_pushinteger(state, network->message_index);
        }
        else if(field == "updateBaseline") {
            lua_push_meta_engine_projectile_network_data(state, network->update_baseline);
        }
        else if(field == "deltaValid") {
            lua_pushboolean(state, network->delta_valid);
        }
        else if(field == "updateDelta") {
            lua_push_meta_engine_projectile_network_data(state, network->update_delta);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_projectile_network__newindex(lua_State *state) noexcept {
        auto *network = lua_from_meta_object<Engine::ProjectileNetwork>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_istable(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        }
        
        std::string field = key;
        if(field == "unknown") {
            if(lua_isboolean(state, 3)) {
                network->unknown = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "baselineValid") {
            if(lua_isboolean(state, 3)) {
                network->baseline_valid = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "baselineIndex") {
            network->baseline_index = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "messageIndex") {
            network->message_index = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "updateBaseline") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "deltaValid") {
            if(lua_isboolean(state, 3)) {
                network->delta_valid = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "updateDelta") {
            return luaL_error(state, "Invalid operation");
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_projectile_network(lua_State *state, Engine::ProjectileNetwork &network, bool read_only) noexcept {
        lua_push_meta_object(state, &network, lua_engine_projectile_network__index, lua_engine_projectile_network__newindex, read_only);
    }

    static int lua_engine_projectile_object__index(lua_State *state) noexcept {
        auto *projectile = lua_from_meta_object<Engine::ProjectileObject>(state, 1); 
        auto *key = lua_tostring(state, 2);
        
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        }
        
        std::string field = key;
        if(field == "projectileFlags") {
            lua_push_meta_engine_projectile_object_flags(state, projectile->projectile_flags);
        }
        else if(field == "actionEnum") {
            lua_pushinteger(state, projectile->action_enum);
        }
        else if(field == "materialId") {
            lua_pushinteger(state, projectile->material_id);
        }
        else if(field == "sourceUnit") {
            lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&projectile->source_unit));
        }
        else if(field == "targetObject") {
            lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&projectile->target_object));
        }
        else if(field == "contrailAttachmentBlockId") {
            lua_pushinteger(state, projectile->contrail_attachment_block_id);
        }
        else if(field == "timeRemaining") {
            lua_pushnumber(state, projectile->time_remaining);
        }
        else if(field == "armingRate") {
            lua_pushnumber(state, projectile->arming_rate);
        }
        else if(field == "unknownProjFloat1") {
            lua_pushnumber(state, projectile->unknown_proj_float1);
        }
        else if(field == "unknownProjFloat2") {
            lua_pushnumber(state, projectile->unknown_proj_float2);
        }
        else if(field == "distanceTravelled") {
            lua_pushnumber(state, projectile->distance_travelled);
        }
        else if(field == "transitionalVelocity") {
            lua_push_meta_engine_vector3_d(state, projectile->transitional_velocity);
        }
        else if(field == "waterDamageUpperBound") {
            lua_pushnumber(state, projectile->water_damage_upper_bound);
        }
        else if(field == "angularVelocity") {
            lua_push_meta_engine_vector3_d(state, projectile->angular_velocity);
        }
        else if(field == "unknownEuler") {
            lua_push_meta_engine_euler2_d(state, projectile->unknown_euler);
        }
        else if(field == "network") {
            lua_push_meta_engine_projectile_network(state, projectile->network);
        }
        else {
            return lua_engine_item_object__index(state);
        }
        return 1;
    }

    static int lua_engine_projectile_object__newindex(lua_State *state) noexcept {
        auto *projectile = lua_from_meta_object<Engine::ProjectileObject>(state, 1); 
        auto *key = lua_tostring(state, 2);
        
        if(key == nullptr || !lua_istable(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        }
        
        std::string field = key;
        if(field == "projectileFlags") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "actionEnum") {
            projectile->action_enum = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "materialId") {
            projectile->material_id = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "sourceUnit") {
            projectile->source_unit.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "targetObject") {
            projectile->target_object.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "contrailAttachmentBlockId") {
            projectile->contrail_attachment_block_id = static_cast<std::int32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "timeRemaining") {
            projectile->time_remaining = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "armingRate") {
            projectile->arming_rate = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "unknownProjFloat1") {
            projectile->unknown_proj_float1 = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "unknownProjFloat2") {
            projectile->unknown_proj_float2 = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "distanceTravelled") {
            projectile->distance_travelled = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "transitionalVelocity") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "waterDamageUpperBound") {
            projectile->water_damage_upper_bound = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "angularVelocity") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "unknownEuler") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "network") {
            return luaL_error(state, "Invalid operation");
        }
        else {
            return lua_engine_item_object__newindex(state);
        }
        return 0;
    }

    void lua_push_meta_engine_projectile_object(lua_State *state, Engine::ProjectileObject &object, bool read_only) noexcept {
        lua_push_meta_object(state, &object, lua_engine_projectile_object__index, lua_engine_projectile_object__newindex, read_only);
    }
}
