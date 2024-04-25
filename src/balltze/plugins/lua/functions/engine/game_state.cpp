// SPDX-License-Identifier: GPL-3.0-only

#include <lua.hpp>
#include <balltze/engine.hpp>
#include <balltze/helpers/string_literal.hpp>
#include "../../../../logger.hpp"
#include "../../../plugin.hpp"
#include "../../../loader.hpp"
#include "../../libraries.hpp"
#include "../../helpers.hpp"
#include "../../metatables.hpp"

namespace Balltze::Plugins {
    static int lua_engine_get_object(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 1 || args == 2) {
                auto object_handle = lua_to_engine_object_handle(state, 1);
                if(object_handle.is_null()) {
                    return luaL_error(state, "Invalid object handle.");
                }

                auto &object_table = Engine::get_object_table();
                Engine::BaseObject *object;
                if(object_handle.id != 0) {
                    object = object_table.get_object(object_handle);
                }
                else {
                    object = object_table.get_object(object_handle.index);
                }

                if(object) {
                    try {
                        auto object_type_name = luaL_checkstring(state, 2);
                        auto object_type = object_type_from_string(object_type_name);
                        if(object->type != object_type) {
                            return luaL_error(state, "Requested object type (%s) does not match object type.", object_type_name);
                        }
                    }
                    catch(std::runtime_error &e) {
                        return luaL_error(state, e.what());
                    }

                    switch(object->type) {
                        case Engine::OBJECT_TYPE_BIPED:
                            lua_push_meta_engine_biped_object(state, *reinterpret_cast<Engine::BipedObject *>(object));
                            break;
                        case Engine::OBJECT_TYPE_VEHICLE:
                            lua_push_meta_engine_vehicle_object(state, *reinterpret_cast<Engine::VehicleObject *>(object));
                            break;
                        case Engine::OBJECT_TYPE_WEAPON:
                            lua_push_meta_engine_weapon_object(state, *reinterpret_cast<Engine::WeaponObject *>(object));
                            break;
                        case Engine::OBJECT_TYPE_EQUIPMENT:
                            lua_push_meta_engine_equipment_object(state, *reinterpret_cast<Engine::EquipmentObject *>(object));
                            break;
                        case Engine::OBJECT_TYPE_GARBAGE:
                            lua_push_meta_engine_garbage_object(state, *reinterpret_cast<Engine::GarbageObject *>(object));
                            break;
                        case Engine::OBJECT_TYPE_PROJECTILE:
                            lua_push_meta_engine_projectile_object(state, *reinterpret_cast<Engine::ProjectileObject *>(object));
                            break;
                        case Engine::OBJECT_TYPE_SCENERY:
                            lua_push_meta_engine_object(state, *object);
                            break;
                        case Engine::OBJECT_TYPE_DEVICE_MACHINE:
                            lua_push_meta_engine_device_machine_object(state, *reinterpret_cast<Engine::DeviceMachineObject *>(object));
                            break;
                        case Engine::OBJECT_TYPE_DEVICE_CONTROL:
                            lua_push_meta_engine_device_control_object(state, *reinterpret_cast<Engine::DeviceControlObject *>(object));
                            break;
                        case Engine::OBJECT_TYPE_DEVICE_LIGHT_FIXTURE:
                            lua_push_meta_engine_device_light_fixture_object(state, *reinterpret_cast<Engine::DeviceLightFixtureObject *>(object));
                            break;
                        case Engine::OBJECT_TYPE_PLACEHOLDER:
                            lua_push_meta_engine_object(state, *object);
                            break;
                        case Engine::OBJECT_TYPE_SOUND_SCENERY:
                            lua_push_meta_engine_object(state, *object);
                            break;
                        default:
                            logger.warning("Unknown object type {}.", static_cast<int>(object->type));
                            lua_push_meta_engine_object(state, *object);
                            break;
                    }
                }
                else {
                    lua_pushnil(state);
                }
                return 1;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function Engine.gameState.getObject.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    template<StringLiteral functionName, StringLiteral objectType>
    int lua_engine_get_object_specialization(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 1) {
            lua_pushstring(state, objectType.value);
            return lua_engine_get_object(state);
        }
        else {
            return luaL_error(state, "Invalid number of arguments in function Engine.gameState.%s.", functionName.value);
        }
    }

    // explicitly instantiate the template for each object type
    template int lua_engine_get_object_specialization<"getBipedObject", "biped">(lua_State *state) noexcept;
    template int lua_engine_get_object_specialization<"getVehicleObject", "vehicle">(lua_State *state) noexcept;
    template int lua_engine_get_object_specialization<"getGarbageObject", "garbage">(lua_State *state) noexcept;
    template int lua_engine_get_object_specialization<"getWeaponObject", "weapon">(lua_State *state) noexcept;
    template int lua_engine_get_object_specialization<"getEquipmentObject", "equipment">(lua_State *state) noexcept;
    template int lua_engine_get_object_specialization<"getProjectileObject", "projectile">(lua_State *state) noexcept;
    template int lua_engine_get_object_specialization<"getDeviceMachineObject", "device_machine">(lua_State *state) noexcept;
    template int lua_engine_get_object_specialization<"getDeviceControlObject", "device_control">(lua_State *state) noexcept;
    template int lua_engine_get_object_specialization<"getDeviceLightFixtureObject", "device_light_fixture">(lua_State *state) noexcept;

    static int lua_engine_create_object(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);

            if(args == 3 || args == 5) {
                auto object_tag_handle = lua_to_engine_tag_handle(state, 1);
                if(object_tag_handle.is_null()) {
                    return luaL_error(state, "Invalid object tag handle.");
                }
                Engine::ObjectHandle parent_object_handle = Engine::ObjectHandle::null();
                if(!lua_isnil(state, 2)) {
                    parent_object_handle = lua_to_engine_object_handle(state, 2);
                    if(parent_object_handle.is_null()) {
                        return luaL_error(state, "Invalid parent object handle.");
                    }
                }
                auto position = lua_to_point3_d(state, 3);

                auto &object_table = Engine::get_object_table();
                auto object_handle = object_table.create_object(object_tag_handle, position, parent_object_handle);
                if(object_handle != Engine::ObjectHandle::null()) {
                    auto *object = object_table.get_object(object_handle);
                    if(object) {
                        lua_push_meta_engine_object(state, *object);
                        return 1;
                    }
                    else {
                        return luaL_error(state, "Could not get object.");
                    }
                }
                else {
                    return luaL_error(state, "Could not create object.");
                }
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function Engine.gameState.createObject.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static int lua_engine_delete_object(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);

            if(args == 1) {
                auto &object_table = Engine::get_object_table();
                if(lua_istable(state, 1)) {
                    auto *object = lua_from_meta_object<Engine::BaseObject>(state, 1);
                    if(object) {
                        auto object_handle = object->object_handle();
                        object_table.delete_object(object_handle);
                    }
                    else {
                        return luaL_error(state, "Invalid object in function Engine.gameState.deleteObject.");
                    }
                }
                else {
                    auto object_handle = lua_to_engine_object_handle(state, 1);
                    if(object_handle.is_null()) {
                        return luaL_error(state, "Invalid object handle in function Engine.gameState.deleteObject.");
                    }
                    if(object_handle.id != 0) {
                        object_table.delete_object(object_handle);
                    }
                    else {
                        object_table.delete_object(object_handle.index);
                    }
                }
                return 0;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function Engine.gameState.deleteObject.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static int lua_unit_enter_vehicle(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 3) {
                auto unit_handle = lua_to_engine_object_handle(state, 1);
                if(unit_handle.is_null()) {
                    return luaL_error(state, "invalid unit object handle in function Engine.gameState.unitEnterVehicle");
                }
                
                auto vehicle_handle = lua_to_engine_object_handle(state, 2);
                if(vehicle_handle.is_null()) {
                    return luaL_error(state, "invalid vehicle object handle in function Engine.gameState.unitEnterVehicle");
                }

                auto &object_table = Engine::get_object_table();
                auto *unit = reinterpret_cast<Engine::UnitObject *>(object_table.get_object(unit_handle));
                if(!unit) {
                    return luaL_error(state, "invalid unit handle in function Engine.gameState.unitEnterVehicle");
                }
                if(!unit->type == Engine::OBJECT_TYPE_BIPED && !unit->type == Engine::OBJECT_TYPE_VEHICLE) {
                    return luaL_error(state, "invalid object type in function Engine.gameState.unitEnterVehicle, expected biped or vehicle");
                }
                
                try {
                    if(lua_isinteger(state, 3)) {
                        auto seat_index = luaL_checkinteger(state, 3);
                        Engine::unit_scripting_enter_vehicle(unit_handle, vehicle_handle, seat_index);
                    }
                    else if(lua_isstring(state, 3)) {
                        auto seat_label = luaL_checkstring(state, 3);
                        Engine::unit_scripting_enter_vehicle(unit_handle, vehicle_handle, seat_label);
                    }
                    else {
                        return luaL_error(state, "invalid seat index or label in function Engine.gameState.unitEnterVehicle, expected integer or string");
                    }
                }
                catch(std::runtime_error &e) {
                    return luaL_error(state, "%s in function Engine.gameState.unitEnterVehicle", e.what());
                }
            }
            else {
                return luaL_error(state, "invalid number of arguments in function Engine.gameState.unitEnterVehicle");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
        return 0;
    }

    static int lua_get_player(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 0 || args == 1) {
                auto &player_table = Engine::get_player_table();
                Engine::Player *player = nullptr;
                if(args == 1) {
                    auto player_handle = lua_to_engine_player_handle(state, 1);
                    if(player_handle.is_null()) {
                        return luaL_error(state, "invalid player handle in function Engine.gameState.getPlayer");
                    }
                    if(player_handle.id != 0) {
                        player = player_table.get_player(player_handle);
                    }
                    else {
                        auto *player_entry = player_table.get_element(player_handle.index);
                        if(player_entry->player_id != 0xFFFF) {
                            player = player_entry;
                        }
                    }
                }
                else {
                    player = player_table.get_client_player();
                }
                if(player) {
                    lua_push_meta_engine_player(state, *player);
                }
                else {
                    lua_pushnil(state);
                }
                return 1;
            }
            else {
                return luaL_error(state, "invalid number of arguments in function Engine.gameState.getPlayer");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
        return 0;
    }

    static int lua_get_player_by_rcon_handle(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args > 0) {
                auto &player_table = Engine::get_player_table();
                auto rcon_handle = luaL_checkinteger(state, 1);
                Engine::Player *player = player_table.get_player_by_rcon_handle(rcon_handle);
                if(player) {
                    lua_push_meta_engine_player(state, *player);
                }
                else {
                    lua_pushnil(state);
                }
                return 1;
            }
            else {
                return luaL_error(state, "invalid number of arguments in function Engine.gameState.getPlayerByRconHandle");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
        return 0;
    }

    static int lua_engine_get_camera_data(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 0) {
                auto &camera_data = Engine::get_camera_data();
                lua_push_meta_engine_camera_data(state, camera_data);
                return 1;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function Engine.gameState.getCameraData.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static int lua_engine_get_camera_type(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 0) {
                auto camera_type = Engine::get_camera_type();
                lua_pushinteger(state, camera_type);
                return 1;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function Engine.gameState.getCameraType.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static const luaL_Reg engine_game_state_functions[] = {
        {"getObject", lua_engine_get_object},
        {"getBipedObject", lua_engine_get_object_specialization<"getBipedObject", "biped">},
        {"getVehicleObject", lua_engine_get_object_specialization<"getVehicleObject", "vehicle">},
        {"getGarbageObject", lua_engine_get_object_specialization<"getGarbageObject", "garbage">},
        {"getWeaponObject", lua_engine_get_object_specialization<"getWeaponObject", "weapon">},
        {"getEquipmentObject", lua_engine_get_object_specialization<"getEquipmentObject", "equipment">},
        {"getProjectileObject", lua_engine_get_object_specialization<"getProjectileObject", "projectile">},
        {"getDeviceMachineObject", lua_engine_get_object_specialization<"getDeviceMachineObject", "device_machine">},
        {"getDeviceControlObject", lua_engine_get_object_specialization<"getDeviceControlObject", "device_control">},
        {"getDeviceLightFixtureObject", lua_engine_get_object_specialization<"getDeviceLightFixtureObject", "device_light_fixture">},
        {"createObject", lua_engine_create_object},
        {"deleteObject", lua_engine_delete_object},
        {"unitEnterVehicle", lua_unit_enter_vehicle},
        {"getPlayer", lua_get_player},
        {"getPlayerByRconHandle", lua_get_player_by_rcon_handle},
        {"getCameraType", lua_engine_get_camera_type},
        {"getCameraData", lua_engine_get_camera_data},
        {nullptr, nullptr}
    };

    void set_engine_game_state_functions(lua_State *state) noexcept {
        luaL_newlibtable(state, engine_game_state_functions);
        luaL_setfuncs(state, engine_game_state_functions, 0);
        lua_setfield(state, -2, "gameState");
    }
}
