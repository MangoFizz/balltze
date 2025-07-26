// SPDX-License-Identifier: GPL-3.0-only

#include <lua.hpp>
#include <balltze/legacy_api/engine.hpp>
#include <balltze/helpers/string_literal.hpp>
#include <impl/object/object.h>
#include "../../../../../logger.hpp"
#include "../../../../helpers/function_table.hpp"
#include "../../types.hpp"

namespace Balltze::Lua::Api::V2 {
    static int engine_get_object(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 1 || args == 2) {
            auto object_handle = get_object_handle(state, 1);
            if(!object_handle) {
                return luaL_error(state, "invalid object handle");
            }

            if(HANDLE_IS_NULL(*object_handle)) {
                lua_pushnil(state);
                return 1;
            }

            auto *object_table = object_table_get();
            auto *object = (DynamicObjectBase *)table_get_element(object_table, *object_handle);

            if(object) {
                if(args == 2) {
                    try {
                        auto object_type = get_object_type(state, 2);
                        if(!object_type) {
                            return luaL_error(state, "Invalid object type in function Engine.object.getObject.");
                        }
                        if(object->object_type != object_type) {
                            return luaL_error(state, "Requested object type does not match object type in function Engine.object.getObject.");
                        }
                    }
                    catch(std::runtime_error &e) {
                        return luaL_error(state, e.what());
                    }
                }

                switch(object->object_type) {
                    case OBJECT_TYPE_BIPED:
                        push_biped_object(state, *reinterpret_cast<BipedObject *>(object));
                        break;
                    case OBJECT_TYPE_VEHICLE:
                        push_vehicle_object(state, *reinterpret_cast<VehicleObject *>(object));
                        break;
                    case OBJECT_TYPE_WEAPON:
                        push_weapon_object(state, *reinterpret_cast<WeaponObject *>(object));
                        break;
                    case OBJECT_TYPE_EQUIPMENT:
                        push_equipment_object(state, *reinterpret_cast<EquipmentObject *>(object));
                        break;
                    case OBJECT_TYPE_GARBAGE:
                        push_garbage_object(state, *reinterpret_cast<GarbageObject *>(object));
                        break;
                    case OBJECT_TYPE_PROJECTILE:
                        push_projectile_object(state, *reinterpret_cast<ProjectileObject *>(object));
                        break;
                    case OBJECT_TYPE_SCENERY:
                        push_dynamic_object_base(state, *object);
                        break;
                    case OBJECT_TYPE_DEVICE_MACHINE:
                        push_device_machine_object(state, *reinterpret_cast<DeviceMachineObject *>(object));
                        break;
                    case OBJECT_TYPE_DEVICE_CONTROL:
                        push_device_control_object(state, *reinterpret_cast<DeviceControlObject *>(object));
                        break;
                    case OBJECT_TYPE_DEVICE_LIGHT_FIXTURE:
                        push_device_light_fixture_object(state, *reinterpret_cast<DeviceLightFixtureObject *>(object));
                        break;
                    case OBJECT_TYPE_PLACEHOLDER:
                        push_dynamic_object_base(state, *object);
                        break;
                    case OBJECT_TYPE_SOUND_SCENERY:
                        push_dynamic_object_base(state, *object);
                        break;
                    default:
                        logger.warning("Unknown object type {}.", static_cast<int>(object->object_type));
                        push_dynamic_object_base(state, *object);
                        break;
                }
            }
            else {
                lua_pushnil(state);
            }
            return 1;
        }
        else {
            return luaL_error(state, "Invalid number of arguments in function Engine.object.getObject.");
        }
    }

    static int engine_create_object(lua_State *state) noexcept {
        int args = lua_gettop(state);

        if(args == 3) {
            auto object_tag_handle = get_tag_handle(state, 1);
            if(!object_tag_handle || HANDLE_IS_NULL(*object_tag_handle)) {
                return luaL_error(state, "Invalid object tag handle in function Engine.object.createObject.");
            }
            ObjectHandle parent_object_handle = NULL_HANDLE;
            if(!lua_isnil(state, 2)) {
                auto handle  = get_tag_handle(state, 2);
                if(!handle) {
                    return luaL_error(state, "Invalid parent object handle in function Engine.object.createObject.");
                }
                parent_object_handle = *handle;
            }
            auto position = get_vector_xyz(state, 3);
            if(!position) {
                return luaL_error(state, "Invalid position in function Engine.object.createObject.");
            }

            auto &object_table = LegacyApi::Engine::get_object_table();
            LegacyApi::Engine::TagHandle tag_handle;
            tag_handle.value = object_tag_handle->value;
            LegacyApi::Engine::ObjectHandle parent;
            parent.value = parent_object_handle.value;
            LegacyApi::Engine::Point3D pos;
            pos.x = position->x;
            pos.y = position->y;
            pos.z = position->z;
            auto object_handle = object_table.create_object(tag_handle, pos, parent);
            if(object_handle != LegacyApi::Engine::ObjectHandle::null()) {
                auto *object = object_table.get_object(object_handle);
                if(object) {
                    push_object_handle(state, *reinterpret_cast<ObjectHandle *>(&object_handle));
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
            auto &object_table = LegacyApi::Engine::get_object_table();
            auto object_handle = get_object_handle(state, 1);
            if(!object_handle || HANDLE_IS_NULL(*object_handle)) {
                return luaL_error(state, "Invalid object handle in function Engine.gameState.deleteObject.");
            }
            object_table.delete_object(object_handle->value);
            return 0;
        }
        else {
            return luaL_error(state, "Invalid number of arguments in function Engine.gameState.deleteObject.");
        }
    }

    int engine_object_attach_to_marker(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args != 4) {
            return luaL_error(state, "invalid number of arguments in function Engine.gameState.objectAttachToMarker");
        }

        auto object_handle = get_object_handle(state, 1);
        if(!object_handle) {
            return luaL_error(state, "invalid argument #1, expected object handle in function Engine.gameState.objectAttachToMarker");
        }
        
        const char *object_marker = luaL_optstring(state, 2, "");
        auto attachment_handle = get_object_handle(state, 3);
        if(!attachment_handle) {
            return luaL_error(state, "invalid argument #3, expected object handle in function Engine.gameState.objectAttachToMarker");
        }
        
        const char *attachment_marker = luaL_optstring(state, 4, "");
        try {
            object_attach_to_marker(*object_handle, object_marker, *attachment_handle, attachment_marker);
        }
        catch(std::runtime_error &e) {
            return luaL_error(state, "%s in function Engine.gameState.objectAttachToMarker", e.what());
        }
        
        return 0;
    }

    static const luaL_Reg engine_object_functions[] = {
        {"getObject", engine_get_object},
        {"createObject", engine_create_object},
        {"deleteObject", engine_delete_object},
        {"objectAttachToMarker", engine_object_attach_to_marker},
        {nullptr, nullptr}
    };

    void set_engine_object_functions(lua_State *state, int table_idx) noexcept {
        int abs_idx = lua_absindex(state, table_idx);
        lua_newtable(state);
        set_functions_reg_array(state, -1, engine_object_functions);
        lua_setfield(state, abs_idx, "object");
    }
}
