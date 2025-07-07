// SPDX-License-Identifier: GPL-3.0-only

#include <stdlib.h>
#include <string.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "luastruct.h"
#include "debug.h"

static const char *ARRAY_METATABLE_NAME = "luastruct_array";

LuastructTypeInfo *get_type_info(lua_State *state, LuastructType type, const char *type_name);
size_t get_type_size(lua_State *state, LuastructType type, void *type_info);

static int get_array_size(lua_State *state, LuastructArray *array) {
    if(array->array_info->count_getter) {
        return array->array_info->count_getter(state, array);
    }
    return array->array_info->array_size;
}

int luastruct_array__index(lua_State *state) {
    LuastructArray *array = luaL_checkudata(state, 1, ARRAY_METATABLE_NAME);
    if(!array) {
        return luaL_error(state, "Array is NULL in __index method");
    }

    int index = luaL_checkinteger(state, 2);
    if(index < 1 || index > get_array_size(state, array)) {
        lua_pushnil(state);
        return 1;
    }

    LuastructArrayDesc *array_info = array->array_info;
    LUAS_DEBUG_MSG("Accessing index #%d of array at 0x%.8X of type \"%s\"\n", index, array->data, luastruct_name_for_type(array_info->elements_type, array_info->elements_type_info));
    
    void *data;
    if(array_info->elements_are_pointers) {
        data = *(void **)(array->data + (index - 1) * sizeof(void *));
    } 
    else {
        if(array_info->elements_size == 0) {
            printf("Array elements size is 0, calculating...\n");
            printf("Array type: %s\n", ((LuastructTypeInfo *)array_info->elements_type_info)->name);
            array_info->elements_size = get_type_size(state, array_info->elements_type, array_info->elements_type_info);
        }
        data = array->data + (index - 1) * array_info->elements_size;
    }

    switch(array_info->elements_type) {
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
        case LUAST_ENUM:
            luastruct_new_object(state, ((LuastructTypeInfo *)array_info->elements_type_info)->name, data, array_info->elements_are_readonly);
            break;
        case LUAST_ARRAY:
            return luaL_error(state, "Nested arrays are not supported");
        case LUAST_BITFIELD:
            return luaL_error(state, "Bitfields are not supported in arrays");
        default:
            return luaL_error(state, "Unknown field type: %d", array_info->elements_type);
    }

    return 1;
}

int luastruct_array__newindex(lua_State *state) {
    LuastructArray *array = luaL_checkudata(state, 1, ARRAY_METATABLE_NAME);
    if(!array) {
        return luaL_error(state, "Array is NULL in __newindex method");
    }

    int index = luaL_checkinteger(state, 2);
    if(index < 1 || index > get_array_size(state, array)) {
        return luaL_error(state, "Tried to set an index out of bounds: %d", index);
    }

    LuastructArrayDesc *array_info = array->array_info;
    LUAS_DEBUG_MSG("Setting index #%d of array at 0x%.8X of type \"%s\"\n", index, array->data, luastruct_name_for_type(array_info->elements_type, array_info->elements_type_info));

    if(array_info->elements_are_readonly) {
        return luaL_error(state, "Array is read-only");
    }

    void *data;
    if(array_info->elements_are_pointers) {
        data = *(void **)(array->data + (index - 1) * sizeof(void *));
    } 
    else {
        if(array_info->elements_size == 0) {
            array_info->elements_size = get_type_size(state, array_info->elements_type, array_info->elements_type_info);
        }
        data = array->data + (index - 1) * array_info->elements_size;
    }

    switch(array_info->elements_type) {
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
        case LUAST_FLOAT:
            *(float *)(data) = luaL_checknumber(state, 3);
            break;
        case LUAST_BOOL:
            *(bool *)(data) = lua_toboolean(state, 3);
            break;
        case LUAST_STRUCT: {
            LuastructStructObject *obj = luaL_checkudata(state, 3, STRUCT_METATABLE_NAME);
            if(!obj || obj->invalid) {
                return luaL_error(state, "Object to copy is invalid");
            }
            if(obj->type != array_info->elements_type_info) {
                return luaL_error(state, "Object type does not match array element type");
            }
            LuastructStruct *struct_info = obj->type;
            memcpy(data, obj->data, struct_info->size);
            break;
        }
        case LUAST_ENUM: {
            LuastructEnum *enum_type = array_info->elements_type_info;
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
            return luaL_error(state, "Nested arrays are not supported");
        case LUAST_BITFIELD:
            return luaL_error(state, "Bitfields are not supported in arrays");
        default:
            return luaL_error(state, "Unknown field type: %d", array_info->elements_type);
    }

    return 0;
}

int luastruct_array__len(lua_State *state) {
    LuastructArray *array = luaL_checkudata(state, 1, ARRAY_METATABLE_NAME);
    if(!array) {
        return luaL_error(state, "Array is NULL in __len method");
    }

    int size = get_array_size(state, array);
    lua_pushinteger(state, size);
    return 1;
}

int luastruct_array__next(lua_State *state) {
    LuastructArray *array = luaL_checkudata(state, 1, ARRAY_METATABLE_NAME);
    if(!array) {
        return luaL_error(state, "Array is NULL in __next method");
    }
    
    int index = 0;
    if(!lua_isnil(state, 2)) {
        index = luaL_checkinteger(state, 2);
    }
    index++;
    if(index < 1 || index > get_array_size(state, array)) {
        lua_pushnil(state);
        return 1;
    }

    lua_pushinteger(state, index);
	lua_pushcfunction(state, luastruct_array__index);
	lua_pushvalue(state, 1);
	lua_pushinteger(state, index);
	lua_call(state, 2, 1);

    return 2;
}

int luastruct_array__pairs(lua_State *state) {
    luaL_checkudata(state, 1, ARRAY_METATABLE_NAME);

    lua_pushvalue(state, lua_upvalueindex(1));
    lua_pushvalue(state, 1);
    lua_pushnil(state);

    return 3;
}

void luastruct_new_dynamic_array_desc(lua_State *state, LuastructType type, const char *type_name, LuastructArrayCounterFunction count_getter, bool elements_are_pointers, bool readonly, LuastructArrayDesc *desc) {
    desc->count_getter = count_getter;
    desc->array_size = 0; 
    desc->elements_type = type;
    desc->elements_type_info = get_type_info(state, type, type_name);
    desc->elements_size = 0;
    desc->elements_are_pointers = elements_are_pointers;
    desc->elements_are_readonly = readonly;
}

void luastruct_new_static_array_desc(lua_State *state, LuastructType type, const char *type_name, size_t size, bool elements_are_pointers, bool readonly, LuastructArrayDesc *desc) {
    desc->count_getter = NULL;
    desc->array_size = size;
    desc->elements_type = type;
    desc->elements_type_info = get_type_info(state, type, type_name);
    desc->elements_size = 0;
    desc->elements_are_pointers = elements_are_pointers;
    desc->elements_are_readonly = readonly;
}

int luastruct_new_array(lua_State *state, void *data, void *parent, size_t offset, LuastructArrayDesc *array_info) {
    LuastructArray *array = lua_newuserdata(state, sizeof(LuastructArray));
    array->data = data;
    array->context.parent = parent;
    array->context.offset = offset;
    array->array_info = array_info;

    if(luaL_newmetatable(state, ARRAY_METATABLE_NAME) != 0) {
        lua_pushcfunction(state, luastruct_array__index);
        lua_setfield(state, -2, "__index");
        lua_pushcfunction(state, luastruct_array__newindex);
        lua_setfield(state, -2, "__newindex");
        lua_pushcfunction(state, luastruct_array__len);
        lua_setfield(state, -2, "__len");
        lua_pushcfunction(state, luastruct_array__next);
        lua_pushcclosure(state, luastruct_array__pairs, 1);
        lua_setfield(state, -2, "__pairs");
    }
    lua_setmetatable(state, -2);

    return 1;
}
