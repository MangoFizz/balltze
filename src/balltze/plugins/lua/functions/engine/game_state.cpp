// SPDX-License-Identifier: GPL-3.0-only

#include <lua.hpp>
#include <balltze/engine.hpp>
#include <balltze/helpers/string_literal.hpp>
#include "../../../../logger.hpp"
#include "../../../plugin.hpp"
#include "../../../loader.hpp"
#include "../../libraries.hpp"
#include "../../types.hpp"
#include "../../helpers/function_table.hpp"

namespace Balltze::Plugins::Lua {
    static int engine_get_object(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 1 || args == 2) {
            auto object_handle = get_engine_resource_handle(state, 1);
            if(!object_handle) {
                return luaL_error(state, "invalid object handle");
            }

            if(object_handle->is_null()) {
                lua_pushnil(state);
                return 1;
            }

            auto &object_table = Engine::get_object_table();
            Engine::BaseObject *object;
            if(object_handle->id != 0) {
                object = object_table.get_object(*object_handle);
            }
            else {
                object = object_table.get_object(object_handle->index);
            }

            if(object) {
                if(args == 2) {
                    try {
                        auto object_type = get_object_type(state, 2);
                        if(object->type != object_type) {
                            return luaL_error(state, "Requested object type does not match object type in function Engine.getObject.");
                        }
                    }
                    catch(std::runtime_error &e) {
                        return luaL_error(state, e.what());
                    }
                }

                switch(object->type) {
                    case Engine::OBJECT_TYPE_BIPED:
                        Lua::push_engine_biped_object(state, reinterpret_cast<Engine::BipedObject *>(object));
                        break;
                    case Engine::OBJECT_TYPE_VEHICLE:
                        Lua::push_engine_vehicle_object(state, reinterpret_cast<Engine::VehicleObject *>(object));
                        break;
                    case Engine::OBJECT_TYPE_WEAPON:
                        Lua::push_engine_weapon_object(state, reinterpret_cast<Engine::WeaponObject *>(object));
                        break;
                    case Engine::OBJECT_TYPE_EQUIPMENT:
                        Lua::push_engine_equipment_object(state, reinterpret_cast<Engine::EquipmentObject *>(object));
                        break;
                    case Engine::OBJECT_TYPE_GARBAGE:
                        Lua::push_engine_garbage_object(state, reinterpret_cast<Engine::GarbageObject *>(object));
                        break;
                    case Engine::OBJECT_TYPE_PROJECTILE:
                        Lua::push_engine_projectile_object(state, reinterpret_cast<Engine::ProjectileObject *>(object));
                        break;
                    case Engine::OBJECT_TYPE_SCENERY:
                        Lua::push_engine_object(state, object);
                        break;
                    case Engine::OBJECT_TYPE_DEVICE_MACHINE:
                        Lua::push_engine_device_machine_object(state, reinterpret_cast<Engine::DeviceMachineObject *>(object));
                        break;
                    case Engine::OBJECT_TYPE_DEVICE_CONTROL:
                        Lua::push_engine_device_control_object(state, reinterpret_cast<Engine::DeviceControlObject *>(object));
                        break;
                    case Engine::OBJECT_TYPE_DEVICE_LIGHT_FIXTURE:
                        Lua::push_engine_device_light_fixture_object(state, reinterpret_cast<Engine::DeviceLightFixtureObject *>(object));
                        break;
                    case Engine::OBJECT_TYPE_PLACEHOLDER:
                        Lua::push_engine_object(state, object);
                        break;
                    case Engine::OBJECT_TYPE_SOUND_SCENERY:
                        Lua::push_engine_object(state, object);
                        break;
                    default:
                        logger.warning("Unknown object type {}.", static_cast<int>(object->type));
                        Lua::push_engine_object(state, object);
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

    static int engine_create_object(lua_State *state) noexcept {
        int args = lua_gettop(state);

        if(args == 3 || args == 5) {
            auto object_tag_handle = get_engine_resource_handle(state, 1);
            if(!object_tag_handle || object_tag_handle->is_null()) {
                return luaL_error(state, "Invalid object tag handle in function Engine.gameState.createObject.");
            }
            Engine::ObjectHandle parent_object_handle = Engine::ObjectHandle::null();
            if(!lua_isnil(state, 2)) {
                auto handle  = get_engine_resource_handle(state, 2);
                if(!handle) {
                    return luaL_error(state, "Invalid parent object handle in function Engine.gameState.createObject.");
                }
                parent_object_handle = *handle;
            }
            auto position = get_point3_d(state, 3);
            if(!position) {
                return luaL_error(state, "Invalid position in function Engine.gameState.createObject.");
            }

            auto &object_table = Engine::get_object_table();
            auto object_handle = object_table.create_object(*object_tag_handle, *position, parent_object_handle);
            if(object_handle != Engine::ObjectHandle::null()) {
                auto *object = object_table.get_object(object_handle);
                if(object) {
                    Lua::push_engine_object_handle(state, object_handle);
                    return 1;
                }
                else {
                    return luaL_error(state, "Could not get object after creation in function Engine.gameState.createObject.");
                }
            }
            else {
                return luaL_error(state, "Could not create object in function Engine.gameState.createObject.");
            }
        }
        else {
            return luaL_error(state, "Invalid number of arguments in function Engine.gameState.createObject.");
        }
    }

    static int engine_delete_object(lua_State *state) noexcept {
        int args = lua_gettop(state);

        if(args == 1) {
            auto &object_table = Engine::get_object_table();
            auto object_handle = get_engine_resource_handle(state, 1);
            if(!object_handle || object_handle->is_null()) {
                return luaL_error(state, "Invalid object handle in function Engine.gameState.deleteObject.");
            }
            if(object_handle->id != 0) {
                object_table.delete_object(*object_handle);
            }
            else {
                object_table.delete_object(object_handle->index);
            }
            return 0;
        }
        else {
            return luaL_error(state, "Invalid number of arguments in function Engine.gameState.deleteObject.");
        }
    }

    static int engine_unit_enter_vehicle(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 3) {
            auto unit_handle = get_engine_resource_handle(state, 1);
            if(!unit_handle || unit_handle->is_null()) {
                return luaL_error(state, "invalid unit object handle in function Engine.gameState.unitEnterVehicle");
            }
            
            auto vehicle_handle = get_engine_resource_handle(state, 2);
            if(!vehicle_handle || vehicle_handle->is_null()) {
                return luaL_error(state, "invalid vehicle object handle in function Engine.gameState.unitEnterVehicle");
            }

            auto &object_table = Engine::get_object_table();
            auto *unit = reinterpret_cast<Engine::UnitObject *>(object_table.get_object(*unit_handle));
            if(!unit) {
                return luaL_error(state, "invalid unit handle in function Engine.gameState.unitEnterVehicle");
            }
            if(!unit->type == Engine::OBJECT_TYPE_BIPED && !unit->type == Engine::OBJECT_TYPE_VEHICLE) {
                return luaL_error(state, "invalid object type in function Engine.gameState.unitEnterVehicle, expected biped or vehicle");
            }
            
            try {
                if(lua_isinteger(state, 3)) {
                    auto seat_index = luaL_checkinteger(state, 3);
                    Engine::unit_scripting_enter_vehicle(*unit_handle, *vehicle_handle, seat_index);
                }
                else if(lua_isstring(state, 3)) {
                    auto seat_label = luaL_checkstring(state, 3);
                    Engine::unit_scripting_enter_vehicle(*unit_handle, *vehicle_handle, seat_label);
                }
                else {
                    return luaL_error(state, "invalid seat index or label in function Engine.gameState.unitEnterVehicle, expected integer or string");
                }
                return 0;
            }
            catch(std::runtime_error &e) {
                return luaL_error(state, "%s in function Engine.gameState.unitEnterVehicle", e.what());
            }
        }
        else {
            return luaL_error(state, "invalid number of arguments in function Engine.gameState.unitEnterVehicle");
        }
    }

    static int engine_unit_exit_vehicle(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 1) {
            auto unit_handle = get_engine_resource_handle(state, 1);
            if(!unit_handle || unit_handle->is_null()) {
                return luaL_error(state, "invalid unit object handle in function Engine.gameState.exitVehicle");
            }
            auto &object_table = Engine::get_object_table();
            auto *unit = reinterpret_cast<Engine::UnitObject *>(object_table.get_object(*unit_handle));
            if(!unit) {
                return luaL_error(state, "invalid unit handle in function Engine.gameState.exitVehicle");
            }
            if(!unit->type == Engine::OBJECT_TYPE_BIPED && !unit->type == Engine::OBJECT_TYPE_VEHICLE) {
                return luaL_error(state, "invalid object type in function Engine.gameState.exitVehicle, expected biped or vehicle");
            }
            try {
                Engine::unit_scripting_exit_vehicle(*unit_handle);
                return 0;
            }
            catch(std::runtime_error &e) {
                return luaL_error(state, "%s in function Engine.gameState.exitVehicle", e.what());
            }
        }
        else {
            return luaL_error(state, "invalid number of arguments in function Engine.gameState.exitVehicle");
        }
    }

    static int get_player(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 0 || args == 1) {
            auto &player_table = Engine::get_player_table();
            Engine::Player *player = nullptr;
            if(args == 1) {
                auto player_handle = get_engine_resource_handle(state, 1);
                if(!player_handle || player_handle->is_null()) {
                    return luaL_error(state, "invalid player handle in function Engine.gameState.getPlayer");
                }
                if(player_handle->id != 0) {
                    player = player_table.get_player(*player_handle);
                }
                else {
                    auto *player_entry = player_table.get_element(player_handle->index);
                    if(player_entry && player_entry->player_id != 0xFFFF) {
                        player = player_entry;
                    }
                }
            }
            else {
                player = player_table.get_client_player();
            }
            if(player) {
                push_meta_engine_player(state, player);
            }
            else {
                lua_pushnil(state);
            }
            return 1;
        }
        else {
            return luaL_error(state, "invalid number of arguments in function Engine.gameState.getPlayer");
        }
        return 0;
    }

    static int get_player_by_rcon_handle(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args > 0) {
            auto &player_table = Engine::get_player_table();
            auto rcon_handle = luaL_checkinteger(state, 1);
            Engine::Player *player = player_table.get_player_by_rcon_handle(rcon_handle);
            if(player) {
                push_meta_engine_player(state, player);
            }
            else {
                lua_pushnil(state);
            }
            return 1;
        }
        else {
            return luaL_error(state, "invalid number of arguments in function Engine.gameState.getPlayerByRconHandle");
        }
        return 0;
    }

    static int engine_get_camera_data(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 0) {
            auto &camera_data = Engine::get_camera_data();
            push_meta_engine_camera_data(state, &camera_data);
            return 1;
        }
        else {
            return luaL_error(state, "Invalid number of arguments in function Engine.gameState.getCameraData.");
        }
    }

    static int engine_get_camera_type(lua_State *state) noexcept {
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

    static int engine_unit_delete_all_weapons(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 1) {
            auto unit_handle = get_engine_resource_handle(state, 1);
            if(!unit_handle) {
                return luaL_error(state, "invalid argument #1, expected object handle in function Engine.gameState.unitDeleteAllWeapons");
            }
            try {
                Engine::unit_delete_all_weapons(*unit_handle);
            }
            catch(std::runtime_error &e) {
                return luaL_error(state, "%s in function Engine.gameState.unitDeleteAllWeapons", e.what());
            }
            return 0;
        }
        else {
            return luaL_error(state, "invalid number of arguments in function Engine.gameState.unitDeleteAllWeapons");
        }
    }

    static int engine_unit_add_weapon_to_inventory(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 3) {
            auto unit_handle = get_engine_resource_handle(state, 1);
            if(!unit_handle) {
                return luaL_error(state, "invalid argument #1, expected object handle in function Engine.gameState.unitAddWeaponToInventory");
            }
            auto weapon_handle = get_engine_resource_handle(state, 2);
            if(!weapon_handle) {
                return luaL_error(state, "invalid argument #2, expected object handle in function Engine.gameState.unitAddWeaponToInventory");
            }
            auto weapon_slot = luaL_checkinteger(state, 3);
            try {
                Engine::unit_add_weapon_to_inventory(*unit_handle, *weapon_handle, weapon_slot);
            }
            catch(std::runtime_error &e) {
                return luaL_error(state, "%s in function Engine.gameState.unitAddWeaponToInventory", e.what());
            }
            return 0;
        }
        else {
            return luaL_error(state, "invalid number of arguments in function Engine.gameState.unitAddWeaponToInventory");
        }
    }

    int engine_object_attach_to_marker(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 4) {
            auto object_handle = get_engine_resource_handle(state, 1);
            if(!object_handle) {
                return luaL_error(state, "invalid argument #1, expected object handle in function Engine.gameState.objectAttachToMarker");
            }
            std::string object_marker = luaL_optstring(state, 2, "");
            auto attachment_handle = get_engine_resource_handle(state, 3);
            if(!attachment_handle) {
                return luaL_error(state, "invalid argument #3, expected object handle in function Engine.gameState.objectAttachToMarker");
            }
            std::string attachment_marker = luaL_optstring(state, 4, "");
            try {
                Engine::object_attach_to_marker(*object_handle, object_marker, *attachment_handle, attachment_marker);
            }
            catch(std::runtime_error &e) {
                return luaL_error(state, "%s in function Engine.gameState.objectAttachToMarker", e.what());
            }
            return 0;
        }
        else {
            return luaL_error(state, "invalid number of arguments in function Engine.gameState.objectAttachToMarker");
        }
    }

    static const luaL_Reg engine_game_state_functions[] = {
        {"getObject", engine_get_object},
        {"createObject", engine_create_object},
        {"deleteObject", engine_delete_object},
        {"unitEnterVehicle", engine_unit_enter_vehicle},
        {"unitExitVehicle", engine_unit_exit_vehicle},
        {"unitDeleteAllWeapons", engine_unit_delete_all_weapons},
        {"unitAddWeaponToInventory", engine_unit_add_weapon_to_inventory},
        {"objectAttachToMarker", engine_object_attach_to_marker},
        {"getPlayer", get_player},
        {"getPlayerByRconHandle", get_player_by_rcon_handle},
        {"getCameraType", engine_get_camera_type},
        {"getCameraData", engine_get_camera_data},
        {nullptr, nullptr}
    };

    void set_engine_game_state_functions(lua_State *state) noexcept {
        create_functions_table(state, "gameState", engine_game_state_functions);
    }
}
