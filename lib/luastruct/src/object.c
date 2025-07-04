// SPDX-License-Identifier: GPL-3.0-only

#include <stdlib.h>
#include <string.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "luastruct.h"
#include "debug.h"

static const char *OBJECT_METATABLE_NAME = "luastruct_object";
static const char *OBJECT_REGISTRY_KEY_FORMAT = "%.8X_%s";
static const char *OBJECT_REGISTRY_NAME = "luastruct_objects";

int luastruct_get_type(lua_State *state, const char *name);
int luastruct_new_array(lua_State *state, void *data, LuastructArrayDesc *array_info);

int luastruct_get_objects_registry(lua_State *state) {
    lua_getfield(state, LUA_REGISTRYINDEX, OBJECT_REGISTRY_NAME);
    if(lua_isnil(state, -1)) {
        lua_pop(state, 1);
        lua_newtable(state);
        lua_pushvalue(state, -1);
        lua_setfield(state, LUA_REGISTRYINDEX, OBJECT_REGISTRY_NAME);
        lua_getfield(state, LUA_REGISTRYINDEX, OBJECT_REGISTRY_NAME);

        /**
         * Set the metatable for the objects registry to use weak references.
         * This allows the Lua garbage collector to collect the objects
         * when there are no strong references to them.
         */
        lua_newtable(state);
        lua_pushstring(state, "v");
        lua_setfield(state, -2, "__mode");
        lua_setmetatable(state, -2);
    }
    return 1;
}

static void get_object_key(char *buffer, void *data, bool readonly) {
    snprintf(buffer, LUASTRUCT_TYPENAME_LENGTH, OBJECT_REGISTRY_KEY_FORMAT, data, readonly ? "readonly" : "readwrite");
}

int luastruct_get_object(lua_State *state, void *data, bool readonly) {
    luastruct_get_objects_registry(state);
    char key[LUASTRUCT_TYPENAME_LENGTH];
    get_object_key(key, data, readonly);
    lua_getfield(state, -1, key);
    if(lua_isnil(state, -1)) {
        lua_pop(state, 2);
        return 0;
    }
    lua_remove(state, -2);
    return 1;
}

int luastruct_object__gc(lua_State *state) {
    LuastructStructObject *obj = luaL_checkudata(state, 1, OBJECT_METATABLE_NAME);
    LuastructTypeInfo *type_info = obj->type;
    LUAS_DEBUG_MSG("Collecting object 0x%.8X of type \"%s\"\n", obj->data, type_info->name);
    if(!obj) {
        return luaL_error(state, "Object is NULL in __gc method");
    }
    if(obj->delete_on_gc) {
        free(obj->data);
    }
    return 0;
}

int luastruct_object__index(lua_State *state) {
    LuastructStructObject *obj = luaL_checkudata(state, 1, OBJECT_METATABLE_NAME);
    
    if(!obj || obj->invalid) {
        return luaL_error(state, "Object is invalid in __index method");
    }

    const char *field_name = luaL_checkstring(state, 2);
    LuastructStruct *st = obj->type;
    LUAS_DEBUG_MSG("Indexing field \"%s\" of struct at 0x%.8X (%s) of type \"%s\"\n", field_name, obj->data, obj->readonly ? "ro" : "rw", st->type_info.name);
    
    LuastructStructField *field = st->fields_by_name;
    while(field) {
        if(strcmp(field->field_name, field_name) == 0) {
            void *data = obj->data + field->offset;
            bool readonly = obj->readonly || field->readonly;
            if(field->pointer) {
                data = *(void **)data;
            }
            switch(field->type) {
                case LUAST_INT8:
                    lua_pushinteger(state, *(int8_t *)(data));
                    break;
                case LUAST_INT16:
                    lua_pushinteger(state, *(int16_t *)(data));
                    break;
                case LUAST_INT32:
                    lua_pushinteger(state, *(int32_t *)(data));
                    break;
                case LUAST_INT64:
                    lua_pushinteger(state, *(int64_t *)(data));
                    break;
                case LUAST_UINT8:
                    lua_pushinteger(state, *(uint8_t *)(data));
                    break;
                case LUAST_UINT16:
                    lua_pushinteger(state, *(uint16_t *)(data));
                    break;
                case LUAST_UINT32:
                    lua_pushinteger(state, *(uint32_t *)(data));
                    break;
                case LUAST_FLOAT:
                    lua_pushnumber(state, *(float *)(data));
                    break;
                case LUAST_BOOL:
                    lua_pushboolean(state, *(bool *)(data));
                    break;
                case LUAST_STRUCT:
                    luastruct_new_object(state, ((LuastructTypeInfo *)field->type_info)->name, data, readonly);
                    break;
                case LUAST_ENUM: {
                    LuastructEnum *enum_type = field->type_info;
                    LuastructEnumVariant *variant = enum_type->variants;
                    while(variant) {
                        
                        variant = variant->next;
                    }
                    break;
                }
                case LUAST_ARRAY:
                    luastruct_new_array(state, data, &field->array);
                    break;
                case LUAST_BITFIELD: {
                    switch(field->bitfield.size) {
                        case 1:
                            lua_pushinteger(state, (*(uint8_t *)(data) >> field->bitfield.offset) & 1);
                            break;
                        case 2:
                            lua_pushinteger(state, (*(uint16_t *)(data) >> field->bitfield.offset) & 1);
                            break;
                        case 4:
                            lua_pushinteger(state, (*(uint32_t *)(data) >> field->bitfield.offset) & 1);
                            break;
                        default:
                            return luaL_error(state, "Invalid bitfield size: %d", field->bitfield.size);
                    }
                    break;
                }
                default:
                    return luaL_error(state, "Unknown field type: %d", field->type);
            }
            return 1;
        }
        field = field->next_by_name;
    }
    lua_pushnil(state);
    return 1;
}

int luastruct_object__newindex(lua_State *state) {
    LuastructStructObject *obj = luaL_checkudata(state, 1, OBJECT_METATABLE_NAME);
    
    if(!obj || obj->invalid) {
        return luaL_error(state, "Object is invalid in __newindex method");
    }
    if(obj->readonly) {
        return luaL_error(state, "Object is read-only in __newindex method");
    }

    const char *field_name = luaL_checkstring(state, 2);
    LuastructStruct *st = obj->type;
    LUAS_DEBUG_MSG("Setting field \"%s\" of struct at 0x%.8X (%s) of type \"%s\"\n", field_name, obj->data, obj->readonly ? "ro" : "rw", st->type_info.name);

    LuastructStructField *field = st->fields_by_name;
    while(field) {
        if(strcmp(field->field_name, field_name) == 0) {
            if(field->readonly) {
                return luaL_error(state, "Field is read-only: %s", field_name);
            }
            void *data = obj->data + field->offset;
            if(field->pointer) {
                data = *(void **)data;
            }
            switch(field->type) {
                case LUAST_INT8: {
                    lua_Integer value = luaL_checkinteger(state, 3);
                    if(value < INT8_MIN || value > INT8_MAX) {
                        return luaL_error(state, "Value out of range for int8: %d", value);
                    }
                    *(int8_t *)(data) = value;
                    break;
                }
                case LUAST_INT16: {
                    lua_Integer value = luaL_checkinteger(state, 3);
                    if(value < INT16_MIN || value > INT16_MAX) {
                        return luaL_error(state, "Value out of range for int16: %d", value);
                    }
                    *(int16_t *)(data) = value;
                    break;
                }
                case LUAST_INT32: {
                    lua_Integer value = luaL_checkinteger(state, 3);
                    if(value < INT32_MIN || value > INT32_MAX) {
                        return luaL_error(state, "Value out of range for int32: %d", value);
                    }
                    *(int32_t *)(data) = value;
                    break;
                }
                case LUAST_UINT8: {
                    lua_Integer value = luaL_checkinteger(state, 3);
                    if(value < 0 || value > UINT8_MAX) {
                        return luaL_error(state, "Value out of range for uint8: %d", value);
                    }
                    *(uint8_t *)(data) = value;
                    break;
                }
                case LUAST_UINT16: {
                    lua_Integer value = luaL_checkinteger(state, 3);
                    if(value < 0 || value > UINT16_MAX) {
                        return luaL_error(state, "Value out of range for uint16: %d", value);
                    }
                    *(uint16_t *)(data) = value;
                    break;
                }
                case LUAST_UINT32: {
                    lua_Integer value = luaL_checkinteger(state, 3);
                    if(value < 0 || value > UINT32_MAX) {
                        return luaL_error(state, "Value out of range for uint32: %d", value);
                    }
                    *(uint32_t *)(data) = value;
                    break;
                }
                case LUAST_FLOAT: {
                    lua_Number value = luaL_checknumber(state, 3);
                    *(float *)(data) = value;
                    break;
                }
                case LUAST_BOOL: {
                    bool value = lua_toboolean(state, 3);
                    *(bool *)(data) = value;
                    break;
                }
                case LUAST_STRUCT: {
                    LuastructStructObject *obj_to_copy = luaL_checkudata(state, 3, OBJECT_METATABLE_NAME);
                    LuastructStruct *field_struct = field->type_info;
                    if(!obj_to_copy || obj_to_copy->invalid) {
                        return luaL_error(state, "Object to copy is invalid");
                    }
                    if(obj_to_copy->type != field->type_info) {
                        LuastructTypeInfo *obj_type_info = obj_to_copy->type;
                        LuastructTypeInfo *field_type_info = field->type_info;
                        return luaL_error(state, "Invalid object type to copy: %s != %s", obj_type_info->name, field_type_info->name);
                    }
                    memcpy(data, obj_to_copy->data, field_struct->size);
                    break;
                }
                case LUAST_ENUM: {
                    LuastructEnum *enum_type = field->type_info;
                    switch(enum_type->size) {
                        case LUAS_ENUM_INT8:
                            *(int8_t *)(data) = luaL_checkinteger(state, 3);
                            break;
                        case LUAS_ENUM_INT16:
                            *(int16_t *)(data) = luaL_checkinteger(state, 3);
                            break;
                        case LUAS_ENUM_INT32:
                            *(int32_t *)(data) = luaL_checkinteger(state, 3);
                            break;
                        default:
                            return luaL_error(state, "Invalid enum type");
                    }
                    break;
                }
                case LUAST_ARRAY:
                    return luaL_error(state, "Array objects cannot be set directly");
                case LUAST_BITFIELD: {
                    switch(field->bitfield.size) {
                        case 1:
                            *(uint8_t *)(data) = (*(uint8_t *)(data) & ~(1 << field->bitfield.offset)) | (lua_toboolean(state, 3) << field->bitfield.offset);
                            break;
                        case 2:
                            *(uint16_t *)(data) = (*(uint16_t *)(data) & ~(1 << field->bitfield.offset)) | (lua_toboolean(state, 3) << field->bitfield.offset);
                            break;
                        case 4:
                            *(uint32_t *)(data) = (*(uint32_t *)(data) & ~(1 << field->bitfield.offset)) | (lua_toboolean(state, 3) << field->bitfield.offset);
                            break;
                        default:
                            return luaL_error(state, "Invalid bitfield size: %d", field->bitfield.size);
                    }
                    break;
                }
                default:
                    return luaL_error(state, "Unknown field type: %d", field->type);
            }
            return 0;
        }
        field = field->next_by_name;
    }
    return luaL_error(state, "Attempt to set unknown field: %s", field_name);
}

int luastruct_object__next(lua_State *state) {
    LuastructStructObject *obj = luaL_checkudata(state, 1, OBJECT_METATABLE_NAME);
    if(!obj || obj->invalid) {
        return luaL_error(state, "Object is invalid in __next method");
    }

    LuastructStruct *st = obj->type;
    if(st->fields_by_name == NULL) {
        LUAS_DEBUG_MSG("No fields found in object at 0x%.8X (%s)\n", obj->data, obj->readonly ? "ro" : "rw");
        lua_pushnil(state);
        return 1;
    }

    const char *field_name = NULL;
    if(lua_isnil(state, 2)) {
        field_name = st->fields_by_name->field_name;
        lua_pushstring(state, field_name);
        lua_pushcfunction(state, luastruct_object__index);
        lua_pushvalue(state, 1);
        lua_pushstring(state, field_name);
        lua_call(state, 2, 1);
        return 2;
    }
    
    field_name = luaL_checkstring(state, 2);
    LUAS_DEBUG_MSG("Iterating field \"%s\" of struct at 0x%.8X (%s) of type \"%s\"\n", field_name, obj->data, obj->readonly ? "ro" : "rw", st->type_info.name);
    
    LuastructStructField *field = st->fields_by_name;
    while(field) {
        if(strcmp(field->field_name, field_name) == 0) {
            if(field->next_by_name) {
                lua_pushstring(state, field->next_by_name->field_name);
                lua_pushcfunction(state, luastruct_object__index);
                lua_pushvalue(state, 1);
                lua_pushstring(state, field->next_by_name->field_name);
                lua_call(state, 2, 1);
                return 2;
            }
            break;
        }
        field = field->next_by_name;
    }
    lua_pushnil(state);
    return 1;
}

int luastruct_object__pairs(lua_State *state) {
	lua_settop(state, 1);
    LUAS_DEBUG_MSG("Iterating object at 0x%.8X (%s)\n", lua_touserdata(state, 1), lua_toboolean(state, 1) ? "ro" : "rw");
	lua_pushcfunction(state, luastruct_object__next);
	lua_pushvalue(state, 1);
	lua_pushnil(state);
	return 3;
}

int luastruct_object__string(lua_State *state) {
    LuastructStructObject *obj = luaL_checkudata(state, 1, OBJECT_METATABLE_NAME);
    if(!obj || obj->invalid) {
        return luaL_error(state, "Object is invalid in __tostring method");
    }
    LuastructStruct *st = obj->type;
    lua_pushfstring(state, "struct %s(%p)", st->type_info.name, obj->data);
    return 1;
}

int luastruct_object__eq(lua_State *state) {
    LuastructStructObject *obj1 = luaL_checkudata(state, 1, OBJECT_METATABLE_NAME);
    LuastructStructObject *obj2 = luaL_checkudata(state, 2, OBJECT_METATABLE_NAME);
    bool equal = true;
    #define ASSERT(cond) equal = equal && (cond)
    ASSERT(obj1 != NULL);
    ASSERT(obj2 != NULL);
    ASSERT(obj1->invalid == false);
    ASSERT(obj2->invalid == false);
    ASSERT(obj1->data != NULL);
    ASSERT(obj2->data != NULL);
    ASSERT(obj1->type == obj2->type);
    ASSERT(obj1->data == obj2->data);
    #undef ASSERT
    lua_pushboolean(state, equal);
    return 1;
}

static const struct luaL_Reg luastruct_object_metatable_methods[] = {
    {"__gc", luastruct_object__gc},
    {"__index", luastruct_object__index},
    {"__newindex", luastruct_object__newindex},
    {"__next", luastruct_object__next},
    {"__pairs", luastruct_object__pairs},
    {"__tostring", luastruct_object__index},
    {"__eq", luastruct_object__eq},
    {NULL, NULL}
};

int luastruct_new_object(lua_State *state, const char *type_name, void *data, bool readonly) {
    if(luastruct_get_type(state, type_name) == 0) {
        return luaL_error(state, "Type not found: %s", type_name);
    }
    LuastructTypeInfo *type_info = lua_touserdata(state, -1);
    lua_pop(state, 1);

    if(type_info->type != LUAST_STRUCT && type_info->type != LUAST_ENUM) {
        return luaL_error(state, "Invalid type for object: %s", type_name);
    }

    if(luastruct_get_object(state, data, readonly) != 0) {
        LuastructStructObject *obj = lua_touserdata(state, -1);
        LuastructTypeInfo *obj_type_info = obj->type;
        if(!obj->invalid) {
            if(obj_type_info->type != type_info->type) {
                return luaL_error(state, "An object of a different type already exists at this address: %s != %s", obj_type_info->name, type_info->name);
            }
            LUAS_DEBUG_MSG("Using existing object of type \"%s\" at 0x%.8X (%s)\n", obj_type_info->name, data, readonly ? "ro" : "rw");
            return 1;
        }
    }

    LUAS_DEBUG_MSG("Creating object of type \"%s\" at 0x%.8X (%s)\n", type_name, data, readonly ? "ro" : "rw");

    LuastructStructObject *obj = lua_newuserdata(state, sizeof(LuastructStructObject));
    obj->type = type_info;
    obj->invalid = false;
    obj->readonly = readonly;
    if(data) {
        obj->data = data;
        obj->delete_on_gc = false;
    }
    else {
        obj->delete_on_gc = true;
        switch(type_info->type) {
            case LUAST_STRUCT:
                obj->data = malloc(((LuastructStruct *)type_info)->size);
                break;
            case LUAST_ENUM: {
                LuastructEnum *enum_type = (LuastructEnum *)type_info;
                switch(enum_type->size) {
                    case LUAS_ENUM_INT8:
                        obj->data = malloc(sizeof(int8_t));
                        break;
                    case LUAS_ENUM_INT16:
                        obj->data = malloc(sizeof(int16_t));
                        break;
                    case LUAS_ENUM_INT32:
                        obj->data = malloc(sizeof(int32_t));
                        break;
                    default:
                        return luaL_error(state, "Invalid enum type");
                }
                break;
            }
            default:
                return luaL_error(state, "Invalid type for object: %s", type_name);
        }
    }

    if(!obj->data) {
        return luaL_error(state, "Failed to allocate memory for object");
    }

    if(luaL_newmetatable(state, OBJECT_METATABLE_NAME) != 0) {
        luaL_setfuncs(state, luastruct_object_metatable_methods, 0);
    }
    lua_setmetatable(state, -2);
    
    char object_key[LUASTRUCT_TYPENAME_LENGTH];
    get_object_key(object_key, data, readonly);
    luastruct_get_objects_registry(state);
    lua_pushvalue(state, -2);
    lua_setfield(state, -2, object_key);
    lua_pop(state, 1);

    return 1;
}
