// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/engine.hpp>
#include <lua.hpp>
#include "../helpers.hpp"
#include "../metatables.hpp"

namespace Balltze::Plugins {
    int lua_engine_object__index(lua_State *state) noexcept;
    int lua_engine_object__newindex(lua_State *state) noexcept;
    
    static int lua_engine_unit_recent_damager__index(lua_State *state) noexcept {
        auto *object = lua_from_meta_object<Engine::UnitRecentDamager>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "lastDamageTime") {
            lua_pushinteger(state, object->last_damage_time);
        }
        else if(field == "totalDamage") {
            lua_pushnumber(state, object->total_damage);
        }
        else if(field == "object") {
            lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&object->object));
        }
        else if(field == "player") {
            lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&object->player));
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_unit_recent_damager__newindex(lua_State *state) noexcept {
        auto *object = lua_from_meta_object<Engine::UnitRecentDamager>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key"); 
        } 
        
        std::string field = key;
        if(field == "lastDamageTime") {
            object->last_damage_time = static_cast<Engine::TickCount32>(luaL_checkinteger(state, 3));
        }
        else if(field == "totalDamage") {
            object->total_damage = luaL_checknumber(state, 3);
        }
        else if(field == "object") {
            object->object.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "player") {
            object->player.handle = luaL_checkinteger(state, 3);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_unit_recent_damager(lua_State *state, Engine::UnitRecentDamager &object, bool read_only) noexcept {
        lua_push_meta_object(state, &object, lua_engine_unit_recent_damager__index, lua_engine_unit_recent_damager__newindex, read_only); 
    }

    static int lua_engine_unit_flags__index(lua_State *state) {
        auto *flags = lua_from_meta_object<Engine::UnitFlags>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "unknownBipedSpeechRelated") {
            lua_pushboolean(state, flags->unknown_biped_speech_related);
        }
        else if(field == "powerUp") {
            lua_pushboolean(state, flags->power_up);
        }
        else if(field == "powerUpAddition") {
            lua_pushboolean(state, flags->power_up_addition);
        }
        else if(field == "controllable") {
            lua_pushboolean(state, flags->controllable);
        }
        else if(field == "berserking") {
            lua_pushboolean(state, flags->berserking);
        }
        else if(field == "unknownIntegratedLightRelated") {
            lua_pushboolean(state, flags->unknown_integrated_light_related);
        }
        else if(field == "willNotDropItems") {
            lua_pushboolean(state, flags->will_not_drop_items);
        }
        else if(field == "unknown") {
            lua_pushboolean(state, flags->unknown);
        }
        else if(field == "canBlink") {
            lua_pushboolean(state, flags->can_blink);
        }
        else if(field == "impervious") {
            lua_pushboolean(state, flags->impervious);
        }
        else if(field == "suspended") {
            lua_pushboolean(state, flags->suspended);
        }
        else if(field == "blind") {
            lua_pushboolean(state, flags->blind);
        }
        else if(field == "unknownNvRelated") {
            lua_pushboolean(state, flags->unknown_nv_related);
        }
        else if(field == "possessed") {
            lua_pushboolean(state, flags->possessed);
        }
        else if(field == "desiresFlashlightOn") {
            lua_pushboolean(state, flags->desires_flashlight_on);
        }
        else if(field == "desiresFlashlightOff") {
            lua_pushboolean(state, flags->desires_flashlight_off);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_unit_flags__newindex(lua_State *state) {
        auto *flags = lua_from_meta_object<Engine::UnitFlags>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_isboolean(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        bool value = lua_toboolean(state, 3);
        if(field == "unknownBipedSpeechRelated") {
            flags->unknown_biped_speech_related = value;
        }
        else if(field == "powerUp") {
            flags->power_up = value;
        }
        else if(field == "powerUpAddition") {
            flags->power_up_addition = value;
        }
        else if(field == "controllable") {
            flags->controllable = value;
        }
        else if(field == "berserking") {
            flags->berserking = value;
        }
        else if(field == "unknownIntegratedLightRelated") {
            flags->unknown_integrated_light_related = value;
        }
        else if(field == "willNotDropItems") {
            flags->will_not_drop_items = value;
        }
        else if(field == "unknown") {
            flags->unknown = value;
        }
        else if(field == "canBlink") {
            flags->can_blink = value;
        }
        else if(field == "impervious") {
            flags->impervious = value;
        }
        else if(field == "suspended") {
            flags->suspended = value;
        }
        else if(field == "blind") {
            flags->blind = value;
        }
        else if(field == "unknownNvRelated") {
            flags->unknown_nv_related = value;
        }
        else if(field == "possessed") {
            flags->possessed = value;
        }
        else if(field == "desiresFlashlightOn") {
            flags->desires_flashlight_on = value;
        }
        else if(field == "desiresFlashlightOff") {
            flags->desires_flashlight_off = value;
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_unit_flags(lua_State *state, Engine::UnitFlags &flags, bool read_only) noexcept {
        lua_push_meta_object(state, &flags, lua_engine_unit_flags__index, lua_engine_unit_flags__newindex, read_only); 
    }

    static int lua_engine_unit_control_flags__index(lua_State *state) noexcept {
        auto *flags = lua_from_meta_object<Engine::UnitControlFlags>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "crouch") {
            lua_pushboolean(state, flags->crouch);
        }
        else if(field == "jump") {
            lua_pushboolean(state, flags->jump);
        }
        else if(field == "user1") {
            lua_pushboolean(state, flags->user1);
        }
        else if(field == "user2") {
            lua_pushboolean(state, flags->user2);
        }
        else if(field == "light") {
            lua_pushboolean(state, flags->light);
        }
        else if(field == "exactFacing") {
            lua_pushboolean(state, flags->exact_facing);
        }
        else if(field == "action") {
            lua_pushboolean(state, flags->action);
        }
        else if(field == "melee") {
            lua_pushboolean(state, flags->melee);
        }
        else if(field == "lookDontTurn") {
            lua_pushboolean(state, flags->look_dont_turn);
        }
        else if(field == "forceAlert") {
            lua_pushboolean(state, flags->force_alert);
        }
        else if(field == "reload") {
            lua_pushboolean(state, flags->reload);
        }
        else if(field == "primaryTrigger") {
            lua_pushboolean(state, flags->primary_trigger);
        }
        else if(field == "secondaryTrigger") {
            lua_pushboolean(state, flags->secondary_trigger);
        }
        else if(field == "grenade") {
            lua_pushboolean(state, flags->grenade);
        }
        else if(field == "exchangeWeapon") {
            lua_pushboolean(state, flags->exchange_weapon);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_unit_control_flags__newindex(lua_State *state) noexcept {
        auto *flags = lua_from_meta_object<Engine::UnitControlFlags>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_isboolean(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        bool value = lua_toboolean(state, 3);
        if(field == "crouch") {
            flags->crouch = value;
        }
        else if(field == "jump") {
            flags->jump = value;
        }
        else if(field == "user1") {
            flags->user1 = value;
        }
        else if(field == "user2") {
            flags->user2 = value;
        }
        else if(field == "light") {
            flags->light = value;
        }
        else if(field == "exactFacing") {
            flags->exact_facing = value;
        }
        else if(field == "action") {
            flags->action = value;
        }
        else if(field == "melee") {
            flags->melee = value;
        }
        else if(field == "lookDontTurn") {
            flags->look_dont_turn = value;
        }
        else if(field == "forceAlert") {
            flags->force_alert = value;
        }
        else if(field == "reload") {
            flags->reload = value;
        }
        else if(field == "primaryTrigger") {
            flags->primary_trigger = value;
        }
        else if(field == "secondaryTrigger") {
            flags->secondary_trigger = value;
        }
        else if(field == "grenade") {
            flags->grenade = value;
        }
        else if(field == "exchangeWeapon") {
            flags->exchange_weapon = value;
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_unit_control_flags(lua_State *state, Engine::UnitControlFlags &flags, bool read_only) noexcept {
        lua_push_meta_object(state, &flags, lua_engine_unit_control_flags__index, lua_engine_unit_control_flags__newindex, read_only); 
    }

    static int lua_engine_unit_animation_state_data__index(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<Engine::UnitAnimationStateData>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "animationIndex") {
            lua_pushinteger(state, data->animation_index);
        }
        else if(field == "frameIndex") {
            lua_pushinteger(state, data->frame_index);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_unit_animation_state_data__newindex(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<Engine::UnitAnimationStateData>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_isinteger(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        std::int16_t value = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        if(field == "animationIndex") {
            data->animation_index = value;
        }
        else if(field == "frameIndex") {
            data->frame_index = value;
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_unit_animation_state_data(lua_State *state, Engine::UnitAnimationStateData &data, bool read_only) noexcept {
        lua_push_meta_object(state, &data, lua_engine_unit_animation_state_data__index, lua_engine_unit_animation_state_data__newindex, read_only); 
    }

    static int lua_engine_unit_animation_flags__index(lua_State *state) noexcept {
        auto *flags = lua_from_meta_object<Engine::UnitAnimationFlags>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "animationBit0Unknown") {
            lua_pushboolean(state, flags->animation_bit0_unknown);
        }
        else if(field == "animationBit1Unknown") {
            lua_pushboolean(state, flags->animation_bit1_unknown);
        }
        else if(field == "animationBit2Unknown") {
            lua_pushboolean(state, flags->animation_bit2_unknown);
        }
        else if(field == "animationBit3Unknown") {
            lua_pushboolean(state, flags->animation_bit3_unknown);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_unit_animation_flags__newindex(lua_State *state) noexcept {
        auto *flags = lua_from_meta_object<Engine::UnitAnimationFlags>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_isboolean(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        bool value = lua_toboolean(state, 3);
        if(field == "animationBit0Unknown") {
            flags->animation_bit0_unknown = value;
        }
        else if(field == "animationBit1Unknown") {
            flags->animation_bit1_unknown = value;
        }
        else if(field == "animationBit2Unknown") {
            flags->animation_bit2_unknown = value;
        }
        else if(field == "animationBit3Unknown") {
            flags->animation_bit3_unknown = value;
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_unit_animation_flags(lua_State *state, Engine::UnitAnimationFlags &flags, bool read_only) noexcept {
        lua_push_meta_object(state, &flags, lua_engine_unit_animation_flags__index, lua_engine_unit_animation_flags__newindex, read_only); 
    }

    static int lua_engine_unit_animation_data__index(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<Engine::UnitAnimationData>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "flags") {
            lua_push_meta_engine_unit_animation_flags(state, data->flags);
        }
        else if(field == "unknownSomeAnimationIndexMaybe") {
            lua_pushinteger(state, data->unknown_some_animation_index_maybe);
        }
        else if(field == "unknownSomeAnimationIndex") {
            lua_pushinteger(state, data->unknown_some_animation_index);
        }
        else if(field == "seatIndex") {
            lua_pushinteger(state, data->seat_index);
        }
        else if(field == "seatWeaponIndex") {
            lua_pushinteger(state, data->seat_weapon_index);
        }
        else if(field == "weaponTypeIndex") {
            lua_pushinteger(state, data->weapon_type_index);
        }
        else if(field == "state") {
            lua_pushstring(state, unit_animation_state_to_string(data->state).c_str());
        }
        else if(field == "replacementState") {
            lua_pushstring(state, unit_replacement_animation_state_to_string(data->replacement_state).c_str());
        }
        else if(field == "overlayState") {
            lua_pushstring(state, unit_overlay_animation_state_to_string(data->overlay_state).c_str());
        }
        else if(field == "desiredState") {
            lua_pushstring(state, unit_animation_state_to_string(data->desired_state).c_str());
        }
        else if(field == "baseSeat") {
            lua_pushstring(state, unit_base_seat_to_string(data->base_seat).c_str());
        }
        else if(field == "emotion") {
            lua_pushinteger(state, data->emotion);
        }
        else if(field == "replacementAnimation") {
            lua_push_meta_engine_unit_animation_state_data(state, data->replacement_animation);
        }
        else if(field == "overlayStateAnimation") {
            lua_push_meta_engine_unit_animation_state_data(state, data->overlay_state_animation);
        }
        else if(field == "weaponIk") {
            lua_push_meta_engine_unit_animation_state_data(state, data->weapon_ik);
        }
        else if(field == "updateLook") {
            lua_pushboolean(state, data->update_look);
        }
        else if(field == "updateAim") {
            lua_pushboolean(state, data->update_aim);
        }
        else if(field == "lookingBounds") {
            lua_push_meta_engine_rectangle2_d_f(state, data->looking_bounds);
        }
        else if(field == "aimingBounds") {
            lua_push_meta_engine_rectangle2_d_f(state, data->aiming_bounds);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_unit_animation_data__newindex(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<Engine::UnitAnimationData>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "flags") {
            lua_push_meta_engine_unit_animation_flags(state, data->flags);
        }
        else if(field == "unknownSomeAnimationIndexMaybe") {
            data->unknown_some_animation_index_maybe = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "unknownSomeAnimationIndex") {
            data->unknown_some_animation_index = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "seatIndex") {
            data->seat_index = static_cast<std::uint8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "seatWeaponIndex") {
            data->seat_weapon_index = static_cast<std::uint8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "weaponTypeIndex") {
            data->weapon_type_index = static_cast<std::uint8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "state") {
            try {
                data->state = unit_animation_state_from_string(luaL_checkstring(state, 3));
            }
            catch(std::runtime_error &e) {
                return luaL_error(state, e.what());
            }
        }
        else if(field == "replacementState") {
            try {
                data->replacement_state = unit_replacement_animation_state_from_string(luaL_checkstring(state, 3));
            }
            catch(std::runtime_error &e) {
                return luaL_error(state, e.what());
            }
        }
        else if(field == "overlayState") {
            try {
                data->overlay_state = unit_overlay_animation_state_from_string(luaL_checkstring(state, 3));
            }
            catch(std::runtime_error &e) {
                return luaL_error(state, e.what());
            }
        }
        else if(field == "desiredState") {
            try {
                data->desired_state = unit_animation_state_from_string(luaL_checkstring(state, 3));
            }
            catch(std::runtime_error &e) {
                return luaL_error(state, e.what());
            }
        }
        else if(field == "baseSeat") {
            try {
                data->base_seat = unit_base_seat_from_string(luaL_checkstring(state, 3));
            }
            catch(std::runtime_error &e) {
                return luaL_error(state, e.what());
            }
        }
        else if(field == "emotion") {
            data->emotion = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "replacementAnimation") {
            lua_push_meta_engine_unit_animation_state_data(state, data->replacement_animation);
        }
        else if(field == "overlayStateAnimation") {
            lua_push_meta_engine_unit_animation_state_data(state, data->overlay_state_animation);
        }
        else if(field == "weaponIk") {
            lua_push_meta_engine_unit_animation_state_data(state, data->weapon_ik);
        }
        else if(field == "updateLook") {
            if(lua_isboolean(state, 3)) {
                data->update_look = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "updateAim") {
            if(lua_isboolean(state, 3)) {
                data->update_aim = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "lookingBounds") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "aimingBounds") {
            return luaL_error(state, "Invalid operation");
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_unit_animation_data(lua_State *state, Engine::UnitAnimationData &data, bool read_only) noexcept {
        lua_push_meta_object(state, &data, lua_engine_unit_animation_data__index, lua_engine_unit_animation_data__newindex, read_only); 
    }

    static int lua_engine_ai_communication_packet__index(lua_State *state) noexcept {
        auto *packet = lua_from_meta_object<Engine::AiCommunicationPacket>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "type") {
            lua_pushstring(state, ai_communication_type_to_string(packet->type).c_str());
        }
        else if(field == "broken") {
            lua_pushboolean(state, packet->broken);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_ai_communication_packet__newindex(lua_State *state) noexcept {
        auto *packet = lua_from_meta_object<Engine::AiCommunicationPacket>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key"); 
        } 
        
        std::string field = key;
        if(field == "type") {
            try {
                packet->type = ai_communication_type_from_string(luaL_checkstring(state, 3));
            }
            catch(std::runtime_error &e) {
                return luaL_error(state, e.what());
            }
        }
        else if(field == "broken") {
            if(lua_isboolean(state, 3)) {
                packet->broken = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_ai_communication_packet(lua_State *state, Engine::AiCommunicationPacket &packet, bool read_only) noexcept {
        lua_push_meta_object(state, &packet, lua_engine_ai_communication_packet__index, lua_engine_ai_communication_packet__newindex, read_only); 
    }

    static int lua_engine_unit_speech__index(lua_State *state) noexcept {
        auto *speech = lua_from_meta_object<Engine::UnitSpeech>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "priority") {
            lua_pushstring(state, unit_speech_priority_to_string(speech->priority).c_str());
        }
        else if(field == "screamType") {
            lua_pushstring(state, unit_scream_type_to_string(speech->scream_type).c_str());
        }
        else if(field == "soundTag") {
            lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&speech->sound_tag));
        }
        else if(field == "ticks") {
            lua_pushinteger(state, speech->ticks);
        }
        else if(field == "unknown2") {
            lua_pushinteger(state, speech->unknown2);
        }
        else if(field == "unknown") {
            lua_pushinteger(state, speech->unknown);
        }
        else if(field == "aiCommunicationInfo") {
            lua_push_meta_engine_ai_communication_packet(state, speech->ai_communication_info);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_unit_speech__newindex(lua_State *state) noexcept {
        auto *speech = lua_from_meta_object<Engine::UnitSpeech>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key"); 
        } 
        
        std::string field = key;
        if(field == "priority") {
            try {
                speech->priority = unit_speech_priority_from_string(luaL_checkstring(state, 3));
            }
            catch(std::runtime_error &e) {
                return luaL_error(state, e.what());
            }
        }
        else if(field == "screamType") {
            try {
                speech->scream_type = unit_scream_type_from_string(luaL_checkstring(state, 3));
            }
            catch(std::runtime_error &e) {
                return luaL_error(state, e.what());
            }
        }
        else if(field == "soundTag") {
            speech->sound_tag.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "ticks") {
            speech->ticks = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "unknown2") {
            speech->unknown2 = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "unknown") {
            speech->unknown = static_cast<std::int32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "aiCommunicationInfo") {
            return luaL_error(state, "Invalid operation");
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_unit_speech(lua_State *state, Engine::UnitSpeech &speech, bool read_only) noexcept {
        lua_push_meta_object(state, &speech, lua_engine_unit_speech__index, lua_engine_unit_speech__newindex, read_only); 
    }

    static int lua_engine_unit_speech_data__index(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<Engine::UnitSpeechData>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "current") {
            lua_push_meta_engine_unit_speech(state, data->current);
        }
        else if(field == "next") {
            lua_push_meta_engine_unit_speech(state, data->next);
        }
        else if(field == "unknown0") {
            lua_pushinteger(state, data->unknown0);
        }
        else if(field == "unknown1") {
            lua_pushinteger(state, data->unknown1);
        }
        else if(field == "unknown2") {
            lua_pushinteger(state, data->unknown2);
        }
        else if(field == "unknown3") {
            lua_pushinteger(state, data->unknown3);
        }
        else if(field == "unknown4") {
            lua_pushinteger(state, data->unknown4);
        }
        else if(field == "unknown6") {
            lua_pushboolean(state, data->unknown6);
        }
        else if(field == "unknown7") {
            lua_pushboolean(state, data->unknown7);
        }
        else if(field == "unknown8") {
            lua_pushboolean(state, data->unknown8);
        }
        else if(field == "unknown9") {
            lua_pushinteger(state, data->unknown9);
        }
        else if(field == "unknown10") {
            lua_pushinteger(state, data->unknown10);
        }
        else if(field == "unknown11") {
            lua_pushinteger(state, data->unknown11);
        }
        else if(field == "unknown12") {
            lua_pushinteger(state, data->unknown12);
        }
        else if(field == "unknown13") {
            lua_pushinteger(state, data->unknown13);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_unit_speech_data__newindex(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<Engine::UnitSpeechData>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key"); 
        } 
        
        std::string field = key;
        if(field == "current") {
            lua_push_meta_engine_unit_speech(state, data->current);
        }
        else if(field == "next") {
            lua_push_meta_engine_unit_speech(state, data->next);
        }
        else if(field == "unknown0") {
            data->unknown0 = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "unknown1") {
            data->unknown1 = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "unknown2") {
            data->unknown2 = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "unknown3") {
            data->unknown3 = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "unknown4") {
            data->unknown4 = static_cast<std::int32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "unknown6") {
            if(lua_isboolean(state, 3)) {
                data->unknown6 = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "unknown7") {
            if(lua_isboolean(state, 3)) {
                data->unknown7 = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "unknown8") {
            if(lua_isboolean(state, 3)) {
                data->unknown8 = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "unknown9") {
            data->unknown9 = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "unknown10") {
            data->unknown10 = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "unknown11") {
            data->unknown11 = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "unknown12") {
            data->unknown12 = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "unknown13") {
            data->unknown13 = static_cast<std::int32_t>(luaL_checkinteger(state, 3));
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_unit_speech_data(lua_State *state, Engine::UnitSpeechData &data, bool read_only) noexcept {
        lua_push_meta_object(state, &data, lua_engine_unit_speech_data__index, lua_engine_unit_speech_data__newindex, read_only); 
    }

    static int lua_engine_unit_control_data__index(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<Engine::UnitControlData>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "animationState") {
            lua_pushinteger(state, data->animation_state);
        }
        else if(field == "aimingSpeed") {
            lua_pushinteger(state, data->aiming_speed);
        }
        else if(field == "controlFlags") {
            lua_push_meta_engine_unit_control_flags(state, data->control_flags);
        }
        else if(field == "weaponIndex") {
            lua_pushinteger(state, data->weapon_index);
        }
        else if(field == "grenadeIndex") {
            lua_pushinteger(state, data->grenade_index);
        }
        else if(field == "zoomIndex") {
            lua_pushinteger(state, data->zoom_index);
        }
        else if(field == "throttle") {
            lua_push_meta_engine_vector3_d(state, data->throttle);
        }
        else if(field == "primaryTrigger") {
            lua_pushnumber(state, data->primary_trigger);
        }
        else if(field == "facingVector") {
            lua_push_meta_engine_vector3_d(state, data->facing_vector);
        }
        else if(field == "aimingVector") {
            lua_push_meta_engine_vector3_d(state, data->aiming_vector);
        }
        else if(field == "lookingVector") {
            lua_push_meta_engine_vector3_d(state, data->looking_vector);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_unit_control_data__newindex(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<Engine::UnitControlData>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_isnumber(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "animationState") {
            data->animation_state = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "aimingSpeed") {
            data->aiming_speed = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "controlFlags") {
            lua_push_meta_engine_unit_control_flags(state, data->control_flags);
        }
        else if(field == "weaponIndex") {
            data->weapon_index = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "grenadeIndex") {
            data->grenade_index = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "zoomIndex") {
            data->zoom_index = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "throttle") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "primaryTrigger") {
            data->primary_trigger = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "facingVector") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "aimingVector") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "lookingVector") {
            return luaL_error(state, "Invalid operation");
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_unit_control_data(lua_State *state, Engine::UnitControlData &data, bool read_only) noexcept {
        lua_push_meta_object(state, &data, lua_engine_unit_control_data__index, lua_engine_unit_control_data__newindex, read_only); 
    }

    extern std::string lua_engine_damage_effect_category_to_string(Engine::TagDefinitions::DamageEffectCategory value) noexcept; 
    extern Engine::TagDefinitions::DamageEffectCategory lua_engine_damage_effect_category_from_string(std::string str); 

    static int lua_engine_unit_object__index(lua_State *state) noexcept {
        auto *unit = lua_from_meta_object<Engine::UnitObject>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "actor") {
            lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&unit->actor));
        }
        else if(field == "swarmActor") {
            lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&unit->swarm_actor));
        }
        else if(field == "swarmNextUnit") {
            lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&unit->swarm_next_unit));
        }
        else if(field == "swarmPreviousUnit") {
            lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&unit->swarm_previous_unit));
        }
        else if(field == "unitFlags") {
            lua_push_meta_engine_unit_flags(state, unit->unit_flags);
        }
        else if(field == "unitControlFlags") {
            lua_push_meta_engine_unit_control_flags(state, unit->unit_control_flags);
        }
        else if(field == "shieldSnapping") {
            lua_pushinteger(state, unit->shield_snapping);
        }
        else if(field == "baseSeatIndex") {
            lua_pushinteger(state, unit->base_seat_index);
        }
        else if(field == "persistentControlTicksRemaining") {
            lua_pushinteger(state, unit->persistent_control.ticks_remaining);
        }
        else if(field == "persistentControlControlFlags") {
            lua_push_meta_engine_unit_control_flags(state, unit->persistent_control.control_flags);
        }
        else if(field == "controllingPlayer") {
            lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&unit->controlling_player));
        }
        else if(field == "aiEffectType") {
            lua_pushinteger(state, unit->ai_effect_type);
        }
        else if(field == "emotionAnimationIndex") {
            lua_pushinteger(state, unit->emotion_animation_index);
        }
        else if(field == "nextAiEffectTick") {
            lua_pushinteger(state, unit->next_ai_effect_tick);
        }
        else if(field == "desiredFacingVector") {
            lua_push_meta_engine_vector3_d(state, unit->desired_facing_vector);
        }
        else if(field == "desiredAimingVector") {
            lua_push_meta_engine_vector3_d(state, unit->desired_aiming_vector);
        }
        else if(field == "aimingVector") {
            lua_push_meta_engine_vector3_d(state, unit->aiming_vector);
        }
        else if(field == "aimingVelocity") {
            lua_push_meta_engine_vector3_d(state, unit->aiming_velocity);
        }
        else if(field == "lookingAngles") {
            lua_push_meta_engine_euler3_d(state, unit->looking_angles);
        }
        else if(field == "lookingVector") {
            lua_push_meta_engine_vector3_d(state, unit->looking_vector);
        }
        else if(field == "lookingVelocity") {
            lua_push_meta_engine_vector3_d(state, unit->looking_velocity);
        }
        else if(field == "throttle") {
            lua_push_meta_engine_vector3_d(state, unit->throttle);
        }
        else if(field == "primaryTrigger") {
            lua_pushnumber(state, unit->primary_trigger);
        }
        else if(field == "aimingSpeed") {
            lua_pushinteger(state, unit->aiming_speed);
        }
        else if(field == "meleeState") {
            lua_pushinteger(state, unit->melee_state);
        }
        else if(field == "meleeTimer") {
            lua_pushinteger(state, unit->melee_timer);
        }
        else if(field == "ticksUntilFlameToDeath") {
            lua_pushinteger(state, unit->ticks_until_flame_to_death);
        }
        else if(field == "pingAnimationTicksLeft") {
            lua_pushinteger(state, unit->ping_animation_ticks_left);
        }
        else if(field == "grenadeState") {
            lua_pushstring(state, unit_throwing_grenade_state_to_string(unit->grenade_state).c_str());
        }
        else if(field == "unknown725") {
            lua_pushinteger(state, unit->unknown_725);
        }
        else if(field == "unknown726") {
            lua_pushinteger(state, unit->unknown_726);
        }
        else if(field == "grenadeProjectile") {
            lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&unit->grenade_projectile));
        }
        else if(field == "animation") {
            lua_push_meta_engine_unit_animation_data(state, unit->animation);
        }
        else if(field == "ambient") {
            lua_pushnumber(state, unit->ambient);
        }
        else if(field == "illumination") {
            lua_pushnumber(state, unit->illumination);
        }
        else if(field == "mouthFactor") {
            lua_pushnumber(state, unit->mouth_factor);
        }
        else if(field == "vehicleSeatId") {
            lua_pushinteger(state, unit->vehicle_seat_id);
        }
        else if(field == "currentWeaponId") {
            lua_pushinteger(state, unit->current_weapon_id);
        }
        else if(field == "nextWeaponId") {
            lua_pushinteger(state, unit->next_weapon_id);
        }
        else if(field == "weapons") {
            lua_newtable(state);
            for(std::size_t i = 0; i < 4; ++i) {
                lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&unit->weapons[i]));
                lua_rawseti(state, -2, i + 1);
            }
        }
        else if(field == "weaponReadyTicks") {
            lua_newtable(state);
            for(std::size_t i = 0; i < 4; ++i) {
                lua_pushinteger(state, unit->weapon_ready_ticks[i]);
                lua_rawseti(state, -2, i + 1);
            }
        }
        else if(field == "equipmentHandle") {
            lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&unit->equipment_handle));
        }
        else if(field == "currentGrenadeIndex") {
            lua_pushinteger(state, unit->current_grenade_index);
        }
        else if(field == "nextGrenadeIndex") {
            lua_pushinteger(state, unit->next_grenade_index);
        }
        else if(field == "grenadeCounts") {
            lua_newtable(state);
            for(std::size_t i = 0; i < 2; ++i) {
                lua_pushinteger(state, unit->grenade_counts[i]);
                lua_rawseti(state, -2, i + 1);
            }
        }
        else if(field == "zoomLevel") {
            lua_pushinteger(state, unit->zoom_level);
        }
        else if(field == "desiredZoomLevel") {
            lua_pushinteger(state, unit->desired_zoom_level);
        }
        else if(field == "ticksSinceLastVehicleSpeech") {
            lua_pushinteger(state, unit->ticks_since_last_vehicle_speech);
        }
        else if(field == "aimingChange") {
            lua_pushinteger(state, unit->aiming_change);
        }
        else if(field == "poweredSeatsRiders") {
            lua_newtable(state);
            for(std::size_t i = 0; i < 2; ++i) {
                lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&unit->powered_seats_riders[i]));
                lua_rawseti(state, -2, i + 1);
            }
        }
        else if(field == "_unknown22") {
            lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&unit->_unknown22));
        }
        else if(field == "_someTickTime") {
            lua_pushinteger(state, unit->_some_tick_time);
        }
        else if(field == "encounterId") {
            lua_pushinteger(state, unit->encounter_id);
        }
        else if(field == "squadId") {
            lua_pushinteger(state, unit->squad_id);
        }
        else if(field == "poweredSeatsPower") {
            lua_newtable(state);
            for(std::size_t i = 0; i < 2; ++i) {
                lua_pushnumber(state, unit->powered_seats_power[i]);
                lua_rawseti(state, -2, i + 1);
            }
        }
        else if(field == "integratedLightPower") {
            lua_pushnumber(state, unit->integrated_light_power);
        }
        else if(field == "integratedLightTogglePower") {
            lua_pushnumber(state, unit->integrated_light_toggle_power);
        }
        else if(field == "integratedNightVisionTogglePower") {
            lua_pushnumber(state, unit->integrated_night_vision_toggle_power);
        }
        else if(field == "seatRelated") {
            lua_newtable(state);
            for(std::size_t i = 0; i < 4; ++i) {
                lua_push_meta_engine_vector3_d(state, unit->seat_related[i]);
                lua_rawseti(state, -2, i + 1);
            }
        }
        else if(field == "camoPower") {
            lua_pushnumber(state, unit->camo_power);
        }
        else if(field == "fullSpectrumVisionPower") {
            lua_pushnumber(state, unit->full_spectrum_vision_power);
        }
        else if(field == "dialogueDefinition") {
            lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&unit->dialogue_definition));
        }
        else if(field == "speech") {
            lua_push_meta_engine_unit_speech_data(state, unit->speech, false);
        }
        else if(field == "damageResultCategory") {
            lua_pushstring(state, lua_engine_damage_effect_category_to_string(unit->damage_result.category).c_str());
        }
        else if(field == "damageResultAiTicksUntilHandle") {
            lua_pushinteger(state, unit->damage_result.ai_ticks_until_handle);
        }
        else if(field == "damageResultAmount") {
            lua_pushnumber(state, unit->damage_result.amount);
        }
        else if(field == "damageResultResponsibleUnit") {
            lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&unit->damage_result.responsible_unit));
        }
        else if(field == "objectFlameCauser") {
            lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&unit->object_flame_causer));
        }
        else if(field == "_unknown23") {
            lua_pushnumber(state, unit->_unknown23);
        }
        else if(field == "diedAtTick") {
            lua_pushinteger(state, unit->died_at_tick);
        }
        else if(field == "feignDeathTimer") {
            lua_pushinteger(state, unit->feign_death_timer);
        }
        else if(field == "camoRegrowth") {
            lua_pushboolean(state, unit->camo_regrowth);
        }
        else if(field == "stun") {
            lua_pushnumber(state, unit->stun);
        }
        else if(field == "stunTicks") {
            lua_pushinteger(state, unit->stun_ticks);
        }
        else if(field == "spreeCount") {
            lua_pushinteger(state, unit->spree_count);
        }
        else if(field == "spreeStartingTime") {
            lua_pushinteger(state, unit->spree_starting_time);
        }
        else if(field == "recentDamage") {
            lua_newtable(state);
            for(std::size_t i = 0; i < 4; ++i) {
                lua_push_meta_engine_unit_recent_damager(state, unit->recent_damage[i]);
                lua_rawseti(state, -2, i + 1);
            }
        }
        else if(field == "opensauceZoomLevel") {
            lua_pushinteger(state, unit->opensauce_zoom_level);
        }
        else if(field == "opensauceDesiredZoomLevel") {
            lua_pushinteger(state, unit->opensauce_desired_zoom_level);
        }
        else if(field == "controlData") {
            lua_push_meta_engine_unit_control_data(state, unit->control_data);
        }
        else if(field == "lastCompletedClientUpdateValid") {
            lua_pushboolean(state, unit->last_completed_client_update_valid);
        }
        else if(field == "lastCompletedClientUpdateId") {
            lua_pushinteger(state, unit->last_completed_client_update_id);
        }
        else {
            return lua_engine_object__index(state); 
        }
        return 1;
    }

    static int lua_engine_unit_object__newindex(lua_State *state) noexcept {
        auto *unit = lua_from_meta_object<Engine::UnitObject>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "actor") {
            unit->actor.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "swarmActor") {
            unit->swarm_actor.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "swarmNextUnir") {
            unit->swarm_next_unit.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "swarmPreviousUnit") {
            unit->swarm_previous_unit.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "unitFlags") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "unitControlFlags") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "ShieldSnapping") {
            unit->shield_snapping = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "BaseSeatIndex") {
            unit->base_seat_index = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "PersistentControlTicksRemaining") {
            unit->persistent_control.ticks_remaining = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "persistentControlControlFlags") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "controllingPlayer") {
            unit->controlling_player.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "aiEffectType") {
            unit->ai_effect_type = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "emotionAnimationIndex") {
            unit->emotion_animation_index = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "nextAiEffectTick") {
            unit->next_ai_effect_tick = static_cast<std::uint32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "desiredFacingVector") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "desiredAimingVector") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "aimingVector") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "aimingVelocity") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "lookingAngles") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "lookingVector") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "lookingVelocity") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "throttle") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "primaryTrigger") {
            unit->primary_trigger = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "aimingSpeed") {
            unit->aiming_speed = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "meleeState") {
            unit->melee_state = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "meleeTimer") {
            unit->melee_timer = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "ticksUntilFlameToDeath") {
            unit->ticks_until_flame_to_death = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "pingAnimationTicksLeft") {
            unit->ping_animation_ticks_left = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "grenadeState") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "unknown725") {
            unit->unknown_725 = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "unknown726") {
            unit->unknown_726 = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "grenadeProjectile") {
            unit->grenade_projectile.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "animation") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "ambient") {
            unit->ambient = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "illumination") {
            unit->illumination = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "mouthFactor") {
            unit->mouth_factor = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "vehicleSeatId") {
            unit->vehicle_seat_id = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "currentWeaponId") {
            unit->current_weapon_id = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "nextWeaponId") {
            unit->next_weapon_id = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "weapons") {
            for(std::size_t i = 0; i < sizeof(unit->weapons) / sizeof(unit->weapons[0]); i++) {
                lua_rawgeti(state, 3, i + 1);
                if(!lua_isnil(state, -1)) {
                    unit->weapons[i] = Engine::ObjectHandle(luaL_checkinteger(state, -1));
                }
                lua_pop(state, 1);
            }
        }
        else if(field == "weaponReadyTicks") {
            for(std::size_t i = 0; i < sizeof(unit->weapon_ready_ticks) / sizeof(unit->weapon_ready_ticks[0]); i++) {
                lua_rawgeti(state, 3, i + 1);
                if(!lua_isnil(state, -1)) {
                    unit->weapon_ready_ticks[i] = static_cast<std::int16_t>(luaL_checkinteger(state, -1));
                }
                lua_pop(state, 1);
            }
        }
        else if(field == "equipmentHandle") {
            unit->equipment_handle.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "currentGrenadeIndex") {
            unit->current_grenade_index = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "nextGrenadeIndex") {
            unit->next_grenade_index = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "grenadeCounts") {
            for(std::size_t i = 0; i < sizeof(unit->grenade_counts) / sizeof(unit->grenade_counts[0]); i++) {
                lua_rawgeti(state, 3, i + 1);
                if(!lua_isnil(state, -1)) {
                    unit->grenade_counts[i] = static_cast<std::uint8_t>(luaL_checkinteger(state, -1));
                }
                lua_pop(state, 1);
            }
        }
        else if(field == "zoomLevel") {
            unit->zoom_level = static_cast<std::uint8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "desiredZoomLevel") {
            unit->desired_zoom_level = static_cast<std::uint8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "ticksSinceLastVehicleSpeech") {
            unit->ticks_since_last_vehicle_speech = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "aimingChange") {
            unit->aiming_change = static_cast<std::uint8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "poweredSeatsRiders") {
            for(std::size_t i = 0; i < sizeof(unit->powered_seats_riders) / sizeof(unit->powered_seats_riders[0]); i++) {
                lua_rawgeti(state, 3, i + 1);
                if(!lua_isnil(state, -1)) {
                    unit->powered_seats_riders[i] = Engine::ObjectHandle(luaL_checkinteger(state, -1));
                }
                lua_pop(state, 1);
            }
        }
        else if(field == "_unknown22") {
            unit->_unknown22.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "_someTickTime") {
            unit->_some_tick_time = static_cast<std::int32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "encounterId") {
            unit->encounter_id = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "squadId") {
            unit->squad_id = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "poweredSeatsPower") {
            for(std::size_t i = 0; i < sizeof(unit->powered_seats_power) / sizeof(unit->powered_seats_power[0]); i++) {
                lua_rawgeti(state, 3, i + 1);
                if(!lua_isnil(state, -1)) {
                    unit->powered_seats_power[i] = static_cast<float>(luaL_checknumber(state, -1));
                }
                lua_pop(state, 1);
            }
        }
        else if(field == "integratedLightPower") {
            unit->integrated_light_power = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "integratedLightTogglePower") {
            unit->integrated_light_toggle_power = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "integratedNightVisionTogglePower") {
            unit->integrated_night_vision_toggle_power = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "seatRelated") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "camoPower") {
            unit->camo_power = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "fullSpectrumVisionPower") {
            unit->full_spectrum_vision_power = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "dialogueDefinition") {
            unit->dialogue_definition.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "speech") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "damageResultCatagory") {
            try {
                unit->damage_result.category = lua_engine_damage_effect_category_from_string(luaL_checkstring(state, 3));
            }
            catch(const std::exception &e) {
                return luaL_error(state, e.what());
            }
        }
        else if(field == "damageResultAiTicksUntilHandle") {
            unit->damage_result.ai_ticks_until_handle = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "damageResultAmount") {
            unit->damage_result.amount = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "damageResultResponsibleUnit") {
            unit->damage_result.responsible_unit.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "objectFlameCauser") {
            unit->object_flame_causer.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "_unknown23") {
            unit->_unknown23 = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "diedAtTick") {
            unit->died_at_tick = static_cast<std::int32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "feignDeathTimer") {
            unit->feign_death_timer = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "camoRegrowth") {
            if(lua_isboolean(state, 3)) {
                unit->camo_regrowth = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "stun") {
            unit->stun = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "stunTicks") {
            unit->stun_ticks = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "spreeCount") {
            unit->spree_count = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "spreeStartingTime") {
            unit->spree_starting_time = static_cast<std::int32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "recentDamage") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "opensauceZoomLevel") {
            unit->opensauce_zoom_level = static_cast<std::uint8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "opensauceDesiredZoomLevel") {
            unit->opensauce_desired_zoom_level = static_cast<std::uint8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "controlData") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "lastCompletedClientUpdateValid") {
            if(lua_isboolean(state, 3)) {
                unit->last_completed_client_update_valid = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "lastCompletedClientUpdateId") {
            unit->last_completed_client_update_id = static_cast<std::int32_t>(luaL_checkinteger(state, 3));
        }
        else {
            return lua_engine_object__newindex(state); 
        }
        return 0;
    }

    void lua_push_meta_engine_unit_object(lua_State *state, Engine::UnitObject &object, bool read_only) noexcept {
        lua_push_meta_object(state, &object, lua_engine_unit_object__index, lua_engine_unit_object__newindex, read_only);
    }

    static int lua_engine_biped_flags__index(lua_State *state) noexcept {
        auto *flags = lua_from_meta_object<Engine::BipedFlags>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "airborne") {
            lua_pushboolean(state, flags->airborne);
        }
        else if(field == "slipping") {
            lua_pushboolean(state, flags->slipping);
        }
        else if(field == "absoluteMovement") {
            lua_pushboolean(state, flags->absolute_movement);
        }
        else if(field == "noCollision") {
            lua_pushboolean(state, flags->no_collision);
        }
        else if(field == "passesThroughOtherBipeds") {
            lua_pushboolean(state, flags->passes_through_other_bipeds);
        }
        else if(field == "limping2") {
            lua_pushboolean(state, flags->limping2);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_biped_flags__newindex(lua_State *state) noexcept {
        auto *flags = lua_from_meta_object<Engine::BipedFlags>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_isboolean(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        }

        std::string field = key;
        if(field == "airborne") {
            flags->airborne = lua_toboolean(state, 3);
        }
        else if(field == "slipping") {
            flags->slipping = lua_toboolean(state, 3);
        }
        else if(field == "absoluteMovement") {
            flags->absolute_movement = lua_toboolean(state, 3);
        }
        else if(field == "noCollision") {
            flags->no_collision = lua_toboolean(state, 3);
        }
        else if(field == "passesThroughOtherBipeds") {
            flags->passes_through_other_bipeds = lua_toboolean(state, 3);
        }
        else if(field == "limping2") {
            flags->limping2 = lua_toboolean(state, 3);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_biped_flags(lua_State *state, Engine::BipedFlags &flags, bool read_only) noexcept {
        lua_push_meta_object(state, &flags, lua_engine_biped_flags__index, lua_engine_biped_flags__newindex, read_only);
    }

    static int lua_engine_biped_network_delta__index(lua_State *state) noexcept {
        auto *delta = lua_from_meta_object<Engine::BipedNetworkDelta>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "grenadeCounts") {
            lua_newtable(state);
            for(std::size_t i = 0; i < 2; ++i) {
                lua_pushinteger(state, delta->grenade_counts[i]);
                lua_rawseti(state, -2, i + 1);
            }
        }
        else if(field == "bodyVitality") {
            lua_pushnumber(state, delta->body_vitality);
        }
        else if(field == "shieldVitality") {
            lua_pushnumber(state, delta->shield_vitality);
        }
        else if(field == "shieldStunTicksGreaterThanZero") {
            lua_pushboolean(state, delta->shield_stun_ticks_greater_than_zero);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_biped_network_delta__newindex(lua_State *state) noexcept {
        auto *delta = lua_from_meta_object<Engine::BipedNetworkDelta>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "grenadeCounts") {
            for(std::size_t i = 0; i < sizeof(delta->grenade_counts) / sizeof(delta->grenade_counts[0]); i++) {
                lua_rawgeti(state, 3, i + 1);
                if(!lua_isnil(state, -1)) {
                    delta->grenade_counts[i] = static_cast<std::int8_t>(luaL_checkinteger(state, -1));
                }
                lua_pop(state, 1);
            }
        }
        else if(field == "bodyVitality") {
            delta->body_vitality = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "shieldVitality") {
            delta->shield_vitality = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "shieldStunTicksGreaterThanZero") {
            if(lua_isboolean(state, 3)) {
                delta->shield_stun_ticks_greater_than_zero = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_biped_network_delta(lua_State *state, Engine::BipedNetworkDelta &delta, bool read_only) noexcept {
        lua_push_meta_object(state, &delta, lua_engine_biped_network_delta__index, lua_engine_biped_network_delta__newindex, read_only);
    }

    static int lua_engine_biped_network__index(lua_State *state) noexcept {
        auto *network = lua_from_meta_object<Engine::BipedNetwork>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "baselineValid") {
            lua_pushboolean(state, network->baseline_valid);
        }
        else if(field == "baselineId") {
            lua_pushinteger(state, network->baseline_id);
        }
        else if(field == "messageId") {
            lua_pushinteger(state, network->message_id);
        }
        else if(field == "updateBaseline") {
            lua_push_meta_engine_biped_network_delta(state, network->update_baseline);
        }
        else if(field == "deltaValid") {
            lua_pushboolean(state, network->delta_valid);
        }
        else if(field == "updateDelta") {
            lua_push_meta_engine_biped_network_delta(state, network->update_delta);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_biped_network__newindex(lua_State *state) noexcept {
        auto *network = lua_from_meta_object<Engine::BipedNetwork>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_istable(state, 3)) {  
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
        else if(field == "baselineId") {
            network->baseline_id = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "messageId") {
            network->message_id = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
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

    void lua_push_meta_engine_biped_network(lua_State *state, Engine::BipedNetwork &network, bool read_only) noexcept {
        lua_push_meta_object(state, &network, lua_engine_biped_network__index, lua_engine_biped_network__newindex, read_only);
    }

    static int lua_engine_biped_object__index(lua_State *state) noexcept {
        auto *biped = lua_from_meta_object<Engine::BipedObject>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "bipedFlags") {
            lua_push_meta_engine_biped_flags(state, biped->biped_flags);
        }
        else if(field == "landingTimer") {
            lua_pushinteger(state, biped->landing_timer);
        }
        else if(field == "landingForce") {
            lua_pushinteger(state, biped->landing_force);
        }
        else if(field == "movementState") {
            lua_pushstring(state, biped_movement_state_to_string(biped->movement_state).c_str());
        }
        else if(field == "_bipedUnknown3") {
            lua_pushinteger(state, biped->_biped_unknown3);
        }
        else if(field == "actionFlags") {
            lua_pushinteger(state, biped->action_flags);
        }
        else if(field == "_bipedUnknown4") {
            lua_pushinteger(state, biped->_biped_unknown4);
        }
        else if(field == "bipedPosition") {
            lua_push_meta_engine_vector3_d(state, biped->biped_position);
        }
        else if(field == "walkingCounter") {
            lua_pushinteger(state, biped->walking_counter);
        }
        else if(field == "bumpObject") {
            lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&biped->bump_object));
        }
        else if(field == "ticksSinceLastBump") {
            lua_pushinteger(state, biped->ticks_since_last_bump);
        }
        else if(field == "airborneTicks") {
            lua_pushinteger(state, biped->airborne_ticks);
        }
        else if(field == "slippingTicks") {
            lua_pushinteger(state, biped->slipping_ticks);
        }
        else if(field == "digitalThrottle") {
            lua_pushinteger(state, biped->digital_throttle);
        }
        else if(field == "jumpTicks") {
            lua_pushinteger(state, biped->jump_ticks);
        }
        else if(field == "meleeTicks") {
            lua_pushinteger(state, biped->melee_ticks);
        }
        else if(field == "meleeInflictTicks") {
            lua_pushinteger(state, biped->melee_inflict_ticks);
        }
        else if(field == "unknownBiped2") {
            lua_pushinteger(state, biped->unknown_biped2);
        }
        else if(field == "crouchScale") {
            lua_pushnumber(state, biped->crouch_scale);
        }
        else if(field == "unknownBiped1") {
            lua_pushnumber(state, biped->unknown_biped1);
        }
        else if(field == "unknownBipedPhysicsRelated") {
            lua_push_meta_engine_plane3_d(state, biped->unknown_biped_physics_related);
        }
        else if(field == "network") {
            lua_push_meta_engine_biped_network(state, biped->network);
        }
        else {
            return lua_engine_unit_object__index(state);
        }
        return 1;
    }

    static int lua_engine_biped_object__newindex(lua_State *state) noexcept {
        auto *biped = lua_from_meta_object<Engine::BipedObject>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "bipedFlags") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "landingTimer") {
            biped->landing_timer = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "landingForce") {
            biped->landing_force = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "movementState") {
            try {
                biped->movement_state = biped_movement_state_from_string(luaL_checkstring(state, 3));
            }
            catch(const std::exception &e) {
                return luaL_error(state, e.what());
            }
        }
        else if(field == "_bipedUnknown3") {
            biped->_biped_unknown3 = static_cast<std::int32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "actionFlags") {
            biped->action_flags = static_cast<std::uint32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "_bipedUnknown4") {
            biped->_biped_unknown4 = static_cast<std::int32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "bipedPosition") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "walkingCounter") {
            biped->walking_counter = static_cast<std::int32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "bumpObject") {
            biped->bump_object.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "ticksSinceLastBump") {
            biped->ticks_since_last_bump = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "airborneTicks") {
            biped->airborne_ticks = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "slippingTicks") {
            biped->slipping_ticks = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "digitalThrottle") {
            biped->digital_throttle = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "jumpTicks") {
            biped->jump_ticks = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "meleeTicks") {
            biped->melee_ticks = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "meleeInflictTicks") {
            biped->melee_inflict_ticks = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "unknownBiped2") {
            biped->unknown_biped2 = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "crouchScale") {
            biped->crouch_scale = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "unknownBiped1") {
            biped->unknown_biped1 = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "unknownBipedPhysicsRelated") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "network") {
            return luaL_error(state, "Invalid operation");
        }
        else {
            return lua_engine_unit_object__newindex(state);
        }
        return 0;
    }

    void lua_push_meta_engine_biped_object(lua_State *state, Engine::BipedObject &object, bool read_only) noexcept {
        lua_push_meta_object(state, &object, lua_engine_biped_object__index, lua_engine_biped_object__newindex, read_only);
    }

    static int lua_engine_vehicle_flags__index(lua_State *state) noexcept {
        auto *flags = lua_from_meta_object<Engine::VehicleFlags>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "vehicleUnknown0") {
            lua_pushboolean(state, flags->vehicle_unknown0);
        }
        else if(field == "hovering") {
            lua_pushboolean(state, flags->hovering);
        }
        else if(field == "crouched") {
            lua_pushboolean(state, flags->crouched);
        }
        else if(field == "jumping") {
            lua_pushboolean(state, flags->jumping);
        }
        else if(field == "unknownVehicle1") {
            lua_pushboolean(state, flags->unknown_vehicle1);
        }
        else if(field == "unknownVehicle2") {
            lua_pushinteger(state, flags->unknown_vehicle2);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_vehicle_flags__newindex(lua_State *state) noexcept {
        auto *flags = lua_from_meta_object<Engine::VehicleFlags>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_isboolean(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        }

        std::string field = key;
        if(field == "vehicleUnknown0") {
            flags->vehicle_unknown0 = lua_toboolean(state, 3);
        }
        else if(field == "hovering") {
            flags->hovering = lua_toboolean(state, 3);
        }
        else if(field == "crouched") {
            flags->crouched = lua_toboolean(state, 3);
        }
        else if(field == "jumping") {
            flags->jumping = lua_toboolean(state, 3);
        }
        else if(field == "unknownVehicle1") {
            flags->unknown_vehicle1 = lua_toboolean(state, 3);
        }
        else if(field == "unknownVehicle2") {
            flags->unknown_vehicle2 = static_cast<std::uint8_t>(luaL_checkinteger(state, 3));
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_vehicle_flags(lua_State *state, Engine::VehicleFlags &flags, bool read_only) noexcept {
        lua_push_meta_object(state, &flags, lua_engine_vehicle_flags__index, lua_engine_vehicle_flags__newindex, read_only);
    }

    static int lua_engine_vehicle_network_data__index(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<Engine::VehicleNetworkData>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "_unknown1") {
            lua_pushboolean(state, data->at_rest);
        }
        else if(field == "_unknown2") {
            lua_push_meta_engine_vector3_d(state, data->position);
        }
        else if(field == "_unknown3") {
            lua_push_meta_engine_vector3_d(state, data->transitional_velocity);
        }
        else if(field == "_unknown4") {
            lua_push_meta_engine_vector3_d(state, data->angular_velocity);
        }
        else if(field == "_unknown5") {
            lua_push_meta_engine_vector3_d(state, data->forward);
        }
        else if(field == "_unknown6") {
            lua_push_meta_engine_vector3_d(state, data->up);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_vehicle_network_data__newindex(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<Engine::VehicleNetworkData>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        }

        std::string field = key;
        if(field == "_unknown1") {
            if(lua_isboolean(state, 3)) {
                data->at_rest = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "_unknown2") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "_unknown3") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "_unknown4") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "_unknown5") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "_unknown6") {
            return luaL_error(state, "Invalid operation");
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_vehicle_network_data(lua_State *state, Engine::VehicleNetworkData &data, bool read_only) noexcept {
        lua_push_meta_object(state, &data, lua_engine_vehicle_network_data__index, lua_engine_vehicle_network_data__newindex, read_only);
    }

    static int lua_engine_vehicle_network__index(lua_State *state) noexcept {
        auto *network = lua_from_meta_object<Engine::VehicleNetwork>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        }

        std::string field = key;
        if(field == "timeValid") {
            lua_pushboolean(state, network->time_valid);
        }
        else if(field == "baselineValid") {
            lua_pushboolean(state, network->baseline_valid);
        }
        else if(field == "baselineId") {
            lua_pushinteger(state, network->baseline_id);
        }
        else if(field == "messageId") {
            lua_pushinteger(state, network->message_id);
        }
        else if(field == "updateBaseline") {
            lua_push_meta_engine_vehicle_network_data(state, network->update_baseline);
        }
        else if(field == "deltaValid") {
            lua_pushboolean(state, network->delta_valid);
        }
        else if(field == "updateDelta") {
            lua_push_meta_engine_vehicle_network_data(state, network->update_delta);
        }
        else if(field == "lastMovedAtTick") {
            lua_pushinteger(state, network->last_moved_at_tick);
        }
        else if(field == "scenarioRespawnId") {
            lua_pushinteger(state, network->scenario_respawn_id);
        }
        else if(field == "respawnPosition") {
            lua_push_meta_engine_vector3_d(state, network->respawn_position);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_vehicle_network__newindex(lua_State *state) noexcept {
        auto *network = lua_from_meta_object<Engine::VehicleNetwork>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_istable(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        }

        std::string field = key;
        if(field == "timeValid") {
            if(lua_isboolean(state, 3)) {
                network->time_valid = lua_toboolean(state, 3);
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
        else if(field == "baselineId") {
            network->baseline_id = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "messageId") {
            network->message_id = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
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
        else if(field == "lastMovedAtTick") {
            network->last_moved_at_tick = static_cast<std::uint32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "scenarioRespawnId") {
            network->scenario_respawn_id = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "respawnPosition") {
            return luaL_error(state, "Invalid operation");
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_vehicle_network(lua_State *state, Engine::VehicleNetwork &network, bool read_only) noexcept {
        lua_push_meta_object(state, &network, lua_engine_vehicle_network__index, lua_engine_vehicle_network__newindex, read_only);
    }

    static int lua_engine_vehicle_object__index(lua_State *state) noexcept {
        auto *vehicle = lua_from_meta_object<Engine::VehicleObject>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        }

        std::string field = key;
        if(field == "vehicleFlags") {
            lua_push_meta_engine_vehicle_flags(state, vehicle->vehicle_flags);
        }
        else if(field == "speed") {
            lua_pushnumber(state, vehicle->speed);
        }
        else if(field == "slide") {
            lua_pushnumber(state, vehicle->slide);
        }
        else if(field == "turn") {
            lua_pushnumber(state, vehicle->turn);
        }
        else if(field == "tirePosition") {
            lua_pushnumber(state, vehicle->tire_position);
        }
        else if(field == "threadPositionLeft") {
            lua_pushnumber(state, vehicle->thread_position_left);
        }
        else if(field == "threadPositionRight") {
            lua_pushnumber(state, vehicle->thread_position_right);
        }
        else if(field == "hover") {
            lua_pushnumber(state, vehicle->hover);
        }
        else if(field == "thrust") {
            lua_pushnumber(state, vehicle->thrust);
        }
        else if(field == "suspensionStates") {
            lua_newtable(state);
            for(std::size_t i = 0; i < sizeof(vehicle->suspension_states) / sizeof(vehicle->suspension_states[0]); i++) {
                lua_pushinteger(state, vehicle->suspension_states[i]);
                lua_rawseti(state, -2, i + 1);
            }
        }
        else if(field == "hoverPosition") {
            lua_push_meta_engine_vector3_d(state, vehicle->hover_position);
        }
        else if(field == "unknownVehicle3") {
            lua_push_meta_engine_vector3_d(state, vehicle->unknown_vehicle3);
        }
        else if(field == "unknownVehicle4") {
            lua_push_meta_engine_vector3_d(state, vehicle->unknown_vehicle4);
        }
        else if(field == "unknownVehicle5") {
            lua_pushinteger(state, vehicle->unknown_vehicle5);
        }
        else if(field == "network") {
            lua_push_meta_engine_vehicle_network(state, vehicle->network);
        }
        else {
            return lua_engine_unit_object__index(state);
        }
        return 1;
    }

    static int lua_engine_vehicle_object__newindex(lua_State *state) noexcept {
        auto *vehicle = lua_from_meta_object<Engine::VehicleObject>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_istable(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        }

        std::string field = key;
        if(field == "vehicleFlags") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "speed") {
            vehicle->speed = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "slide") {
            vehicle->slide = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "turn") {
            vehicle->turn = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "tirePosition") {
            vehicle->tire_position = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "threadPositionLeft") {
            vehicle->thread_position_left = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "threadPositionRight") {
            vehicle->thread_position_right = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "hover") {
            vehicle->hover = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "thrust") {
            vehicle->thrust = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "suspensionStates") {
            for(std::size_t i = 0; i < sizeof(vehicle->suspension_states) / sizeof(vehicle->suspension_states[0]); i++) {
                lua_rawgeti(state, 3, i + 1);
                if(!lua_isnil(state, -1)) {
                    vehicle->suspension_states[i] = static_cast<std::int8_t>(luaL_checkinteger(state, -1));
                }
                lua_pop(state, 1);
            }
        }
        else if(field == "hoverPosition") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "unknownVehicle3") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "unknownVehicle4") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "unknownVehicle5") {
            vehicle->unknown_vehicle5 = static_cast<std::int32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "network") {
            return luaL_error(state, "Invalid operation");
        }
        else {
            return lua_engine_unit_object__newindex(state);
        }
        return 0;
    }

    void lua_push_meta_engine_vehicle_object(lua_State *state, Engine::VehicleObject &object, bool read_only) noexcept {
        lua_push_meta_object(state, &object, lua_engine_vehicle_object__index, lua_engine_vehicle_object__newindex, read_only);
    }
}
