// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/engine.hpp>
#include <lua.hpp>
#include "../helpers.hpp"
#include "../metatables.hpp"

namespace Balltze::Plugins {
    int lua_engine_object__index(lua_State *state) noexcept;
    int lua_engine_object__newindex(lua_State *state) noexcept;

    static int lua_engine_device_object_state__index(lua_State *state) noexcept {
        auto *state_data = lua_from_meta_object<Engine::DeviceObjectState>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EngineDeviceObjectState metaobject.");
        }
        
        std::string field = key;
        if(field == "deviceGroupId") {
            lua_pushinteger(state, state_data->device_group_id);
        }
        else if(field == "value") {
            lua_pushnumber(state, state_data->value);
        }
        else if(field == "change") {
            lua_pushnumber(state, state_data->change);
        }
        else {
            return luaL_error(state, "Invalid field in EngineDeviceObjectState metaobject.");
        }
        return 1;
    }

    static int lua_engine_device_object_state__newindex(lua_State *state) noexcept {
        auto *state_data = lua_from_meta_object<Engine::DeviceObjectState>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EngineDeviceObjectState metaobject.");
        }
        
        std::string field = key;
        if(field == "deviceGroupId") {
            state_data->device_group_id = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "value") {
            state_data->value = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "change") {
            state_data->change = static_cast<float>(luaL_checknumber(state, 3));
        }
        else {
            return luaL_error(state, "Invalid field in EngineDeviceObjectState metaobject.");
        }
        return 0;
    }

    void lua_push_meta_engine_device_object_state(lua_State *state, Engine::DeviceObjectState &state_data, bool read_only) noexcept {
        lua_push_meta_object(state, &state_data, lua_engine_device_object_state__index, lua_engine_device_object_state__newindex, read_only);
    }

    static int lua_engine_device_object__index(lua_State *state) noexcept {
        auto *device = lua_from_meta_object<Engine::DeviceObject>(state, 1); 
        auto *key = lua_tostring(state, 2);
        
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EngineDeviceObject metaobject.");
        }
        
        std::string field = key;
        if(field == "positionReversed") {
            lua_pushboolean(state, device->position_reversed);
        }
        else if(field == "notUsableFromAnySide") {
            lua_pushboolean(state, device->not_usable_from_any_side);
        }
        else if(field == "power") {
            lua_push_meta_engine_device_object_state(state, device->power);
        }
        else if(field == "position") {
            lua_push_meta_engine_device_object_state(state, device->position);
        }
        else if(field == "oneSided") {
            lua_pushboolean(state, device->one_sided);
        }
        else if(field == "operatesAutomatically") {
            lua_pushboolean(state, device->operates_automatically);
        }
        else {
            return lua_engine_object__index(state);
        }
        return 1;        
    }

    static int lua_engine_device_object__newindex(lua_State *state) noexcept {
        auto *device = lua_from_meta_object<Engine::DeviceObject>(state, 1); 
        auto *key = lua_tostring(state, 2);
        
        if(key == nullptr || !lua_istable(state, 3)) {  
            return luaL_error(state, "Invalid key in EngineDeviceObject metaobject.");
        }
        
        std::string field = key;
        if(field == "positionReversed") {
            if(lua_isboolean(state, 3)) {
                device->position_reversed = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type on field positionReversed in EngineDeviceObject metaobject.");
            }
        }
        else if(field == "notUsableFromAnySide") {
            if(lua_isboolean(state, 3)) {
                device->not_usable_from_any_side = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type on field notUsableFromAnySide in EngineDeviceObject metaobject.");
            }
        }
        else if(field == "power") {
            return luaL_error(state, "Invalid operation on field power in EngineDeviceObject metaobject.");
        }
        else if(field == "position") {
            return luaL_error(state, "Invalid operation on field position in EngineDeviceObject metaobject.");
        }
        else if(field == "oneSided") {
            if(lua_isboolean(state, 3)) {
                device->one_sided = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type on field oneSided in EngineDeviceObject metaobject.");
            }
        }
        else if(field == "operatesAutomatically") {
            if(lua_isboolean(state, 3)) {
                device->operates_automatically = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type on field operatesAutomatically in EngineDeviceObject metaobject.");
            }
        }
        else {
            return lua_engine_object__newindex(state);
        }
        return 0;        
    }

    void lua_push_meta_engine_device_object(lua_State *state, Engine::DeviceObject &object, bool read_only) noexcept {
        lua_push_meta_object(state, &object, lua_engine_device_object__index, lua_engine_device_object__newindex, read_only);
    }

    static int lua_engine_device_machine_object_flags__index(lua_State *state) noexcept {
        auto *flags = lua_from_meta_object<Engine::DeviceMachineObjectFlags>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EngineDeviceMachineObjectFlags metaobject.");
        }
        
        std::string field = key;
        if(field == "doesNotOperateAutomatically") {
            lua_pushboolean(state, flags->does_not_operate_automatically);
        }
        else if(field == "machineOneSided") {
            lua_pushboolean(state, flags->machine_one_sided);
        }
        else if(field == "neverAppearsLocked") {
            lua_pushboolean(state, flags->never_appears_locked);
        }
        else if(field == "openedByMeleeAttack") {
            lua_pushboolean(state, flags->opened_by_melee_attack);
        }
        else {
            return luaL_error(state, "Invalid field in EngineDeviceMachineObjectFlags metaobject.");
        }
        return 1;
    }

    static int lua_engine_device_machine_object_flags__newindex(lua_State *state) noexcept {
        auto *flags = lua_from_meta_object<Engine::DeviceMachineObjectFlags>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_isboolean(state, 3)) {  
            return luaL_error(state, "Invalid key in EngineDeviceMachineObjectFlags metaobject.");
        }
        
        std::string field = key;
        if(field == "doesNotOperateAutomatically") {
            flags->does_not_operate_automatically = lua_toboolean(state, 3);
        }
        else if(field == "machineOneSided") {
            flags->machine_one_sided = lua_toboolean(state, 3);
        }
        else if(field == "neverAppearsLocked") {
            flags->never_appears_locked = lua_toboolean(state, 3);
        }
        else if(field == "openedByMeleeAttack") {
            flags->opened_by_melee_attack = lua_toboolean(state, 3);
        }
        else {
            return luaL_error(state, "Invalid field in EngineDeviceMachineObjectFlags metaobject.");
        }
        return 0;
    }

    void lua_push_meta_engine_device_machine_object_flags(lua_State *state, Engine::DeviceMachineObjectFlags &flags, bool read_only) noexcept {
        lua_push_meta_object(state, &flags, lua_engine_device_machine_object_flags__index, lua_engine_device_machine_object_flags__newindex, read_only);
    }

    static int lua_engine_device_machine_object__index(lua_State *state) noexcept {
        auto *device = lua_from_meta_object<Engine::DeviceMachineObject>(state, 1); 
        auto *key = lua_tostring(state, 2);
        
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EngineDeviceMachineObject metaobject.");
        }
        
        std::string field = key;
        if(field == "deviceFlags") {
            lua_push_meta_engine_device_machine_object_flags(state, device->device_flags);
        }
        else if(field == "ticksSinceStartedOpening") {
            lua_pushinteger(state, device->ticks_since_started_opening);
        }
        else if(field == "elevatorPosition") {
            lua_push_meta_engine_vector3_d(state, device->elevator_position);
        }
        else {
            return lua_engine_device_object__index(state);
        }
        return 1;
    }

    static int lua_engine_device_machine_object__newindex(lua_State *state) noexcept {
        auto *device = lua_from_meta_object<Engine::DeviceMachineObject>(state, 1); 
        auto *key = lua_tostring(state, 2);
        
        if(key == nullptr || !lua_istable(state, 3)) {  
            return luaL_error(state, "Invalid key in EngineDeviceMachineObject metaobject.");
        }
        
        std::string field = key;
        if(field == "deviceFlags") {
            return luaL_error(state, "Invalid operation on field deviceFlags in EngineDeviceMachineObject metaobject.");
        }
        else if(field == "ticksSinceStartedOpening") {
            device->ticks_since_started_opening = static_cast<std::uint32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "elevatorPosition") {
            return luaL_error(state, "Invalid operation on field elevatorPosition in EngineDeviceMachineObject metaobject.");
        }
        else {
            return lua_engine_device_object__newindex(state);
        }
        return 0;
    }

    void lua_push_meta_engine_device_machine_object(lua_State *state, Engine::DeviceMachineObject &object, bool read_only) noexcept {
        lua_push_meta_object(state, &object, lua_engine_device_machine_object__index, lua_engine_device_machine_object__newindex, read_only);
    }

    static int lua_engine_device_control_object_flags__index(lua_State *state) noexcept {
        auto *flags = lua_from_meta_object<Engine::DeviceControlObjectFlags>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EngineDeviceControlObjectFlags metaobject.");
        }
        
        std::string field = key;
        if(field == "usableFromBothSides") {
            lua_pushboolean(state, flags->usable_from_both_sides);
        }
        else {
            return luaL_error(state, "Invalid field in EngineDeviceControlObjectFlags metaobject.");
        }
        return 1;
    }
    
    static int lua_engine_device_control_object_flags__newindex(lua_State *state) noexcept {
        auto *flags = lua_from_meta_object<Engine::DeviceControlObjectFlags>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_isboolean(state, 3)) {  
            return luaL_error(state, "Invalid key in EngineDeviceControlObjectFlags metaobject.");
        }
        
        std::string field = key;
        if(field == "usableFromBothSides") {
            flags->usable_from_both_sides = lua_toboolean(state, 3);
        }
        else {
            return luaL_error(state, "Invalid key in EngineDeviceControlObjectFlags metaobject.");
        }
        return 0;
    }

    void lua_push_meta_engine_device_control_object_flags(lua_State *state, Engine::DeviceControlObjectFlags &flags, bool read_only) noexcept {
        lua_push_meta_object(state, &flags, lua_engine_device_control_object_flags__index, lua_engine_device_control_object_flags__newindex, read_only);
    }

    static int lua_engine_device_control_object__index(lua_State *state) noexcept {
        auto *device = lua_from_meta_object<Engine::DeviceControlObject>(state, 1); 
        auto *key = lua_tostring(state, 2);
        
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EngineDeviceControlObject metaobject.");
        }
        
        std::string field = key;
        if(field == "deviceControlFlags") {
            lua_push_meta_engine_device_control_object_flags(state, device->device_control_flags);
        }
        else if(field == "customNameId") {
            lua_pushinteger(state, device->custom_name_id);
        }
        else {
            return lua_engine_device_object__index(state);
        }
        return 1;
    }

    static int lua_engine_device_control_object__newindex(lua_State *state) noexcept {
        auto *device = lua_from_meta_object<Engine::DeviceControlObject>(state, 1); 
        auto *key = lua_tostring(state, 2);
        
        if(key == nullptr || !lua_istable(state, 3)) {  
            return luaL_error(state, "Invalid key type in EngineDeviceControlObject metaobject.");
        }
        
        std::string field = key;
        if(field == "deviceControlFlags") {
            return luaL_error(state, "Invalid operation on field deviceControlFlags in EngineDeviceControlObject metaobject.");
        }
        else if(field == "customNameId") {
            device->custom_name_id = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else {
            return lua_engine_device_object__newindex(state);
        }
        return 0;
    }

    void lua_push_meta_engine_device_control_object(lua_State *state, Engine::DeviceControlObject &object, bool read_only) noexcept {
        lua_push_meta_object(state, &object, lua_engine_device_control_object__index, lua_engine_device_control_object__newindex, read_only);
    }

    static int lua_engine_device_light_fixture_object__index(lua_State *state) noexcept {
        auto *device = lua_from_meta_object<Engine::DeviceLightFixtureObject>(state, 1); 
        auto *key = lua_tostring(state, 2);
        
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EngineDeviceLightFixtureObject metaobject.");
        }
        
        std::string field = key;
        if(field == "lightColor") {
            lua_push_meta_engine_color_r_g_b(state, device->light_color);
        }
        else if(field == "lightIntensity") {
            lua_pushnumber(state, device->light_intensity);
        }
        else if(field == "lightFalloffAngle") {
            lua_pushnumber(state, device->light_falloff_angle);
        }
        else if(field == "lightCutoffAngle") {
            lua_pushnumber(state, device->light_cutoff_angle);
        }
        else {
            return lua_engine_device_object__index(state);
        }
        return 1;
    }

    static int lua_engine_device_light_fixture_object__newindex(lua_State *state) noexcept {
        auto *device = lua_from_meta_object<Engine::DeviceLightFixtureObject>(state, 1); 
        auto *key = lua_tostring(state, 2);
        
        if(key == nullptr || !lua_istable(state, 3)) {  
            return luaL_error(state, "Invalid key in EngineDeviceLightFixtureObject metaobject.");
        }
        
        std::string field = key;
        if(field == "light_color") {
            return luaL_error(state, "Invalid operation on field lightColor in EngineDeviceLightFixtureObject metaobject.");
        }
        else if(field == "lightIntensity") {
            device->light_intensity = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "lightFalloffAngle") {
            device->light_falloff_angle = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "lightCutoffAngle") {
            device->light_cutoff_angle = static_cast<float>(luaL_checknumber(state, 3));
        }
        else {
            return lua_engine_device_object__newindex(state);
        }
        return 0;
    }

    void lua_push_meta_engine_device_light_fixture_object(lua_State *state, Engine::DeviceLightFixtureObject &object, bool read_only) noexcept {
        lua_push_meta_object(state, &object, lua_engine_device_light_fixture_object__index, lua_engine_device_light_fixture_object__newindex, read_only);
    }
}
