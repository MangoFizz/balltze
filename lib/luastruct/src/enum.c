// SPDX-License-Identifier: GPL-3.0-only

#include <stdlib.h>
#include <string.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "luastruct.h"
#include "debug.h"

int luastruct_get_type(lua_State *state, const char *name);

void free_enum_variants_recursively(LuastructEnumVariant *variant) {
    if(variant->next) {
        free_enum_variants_recursively(variant->next);
    }
    free(variant);
}

int luastruct_enum_variant__index(lua_State *state) {
    LuastructEnumVariant *variant = luaL_checkudata(state, 1, ENUM_VARIANT_METATABLE_NAME);
    if(!variant) {
        return luaL_error(state, "Invalid enum variant userdata");
    }

    const char *key = luaL_checkstring(state, 2);
    if(strcmp(key, "name") == 0) {
        lua_pushstring(state, variant->name);
        return 1;
    } 
    else if(strcmp(key, "value") == 0) {
        lua_pushinteger(state, variant->value);
        return 1;
    }

    return luaL_error(state, "Unknown key for enum variant: %s", key);
}

int luastruct_enum_variant__eq(lua_State *state) {
    LuastructEnumVariant *variant1 = luaL_checkudata(state, 1, ENUM_VARIANT_METATABLE_NAME);
    LuastructEnumVariant *variant2 = luaL_checkudata(state, 2, ENUM_VARIANT_METATABLE_NAME);

    if(!variant1 || !variant2) {
        return luaL_error(state, "Invalid enum variant userdata");
    }

    lua_pushboolean(state, variant1->value == variant2->value);
    return 1;
}

int luastruct_enum_variant__tostring(lua_State *state) {
    LuastructEnumVariant *variant = luaL_checkudata(state, 1, ENUM_VARIANT_METATABLE_NAME);
    if(!variant) {
        return luaL_error(state, "Invalid enum variant userdata");
    }

    lua_pushfstring(state, "%s(%d)", variant->name, variant->value);
    return 1;
}

void get_enum_variant_metatable(lua_State *state) {
    int metatable = luaL_newmetatable(state, ENUM_VARIANT_METATABLE_NAME);
    if(metatable != 0) {
        lua_pushcfunction(state, luastruct_enum_variant__index);
        lua_setfield(state, -2, "__index");
        lua_pushcfunction(state, luastruct_enum_variant__eq);
        lua_setfield(state, -2, "__eq");
        lua_pushcfunction(state, luastruct_enum_variant__tostring);
        lua_setfield(state, -2, "__tostring");
    }
}

int luastruct_enum__index(lua_State *state) {
    LuastructEnum *enum_type = luaL_checkudata(state, 1, ENUM_METATABLE_NAME);
    if(!enum_type) {
        return luaL_error(state, "Invalid enum userdata");
    }

    const char *key = luaL_checkstring(state, 2);
    LuastructEnumVariant *variant = enum_type->variants;

    while(variant) {
        if(strcmp(variant->name, key) == 0) {
            lua_pushlightuserdata(state, variant);
            get_enum_variant_metatable(state);
            lua_setmetatable(state, -2);
            return 1;
        }
        variant = variant->next;
    }

    lua_pushnil(state);
    return 1;
}

int luastruct_enum__gc(lua_State *state) {
    LuastructEnum *em = luaL_checkudata(state, 1, ENUM_METATABLE_NAME);
    if(!em) {
        return luaL_error(state, "Invalid enum userdata");
    }
    if(em->variants) {
        free_enum_variants_recursively(em->variants);
    }
    return 0;
}

void get_enum_metatable(lua_State *state) {
    int metatable = luaL_newmetatable(state, ENUM_METATABLE_NAME);
    if(metatable != 0) {
        lua_pushcfunction(state, luastruct_enum__index);
        lua_setfield(state, -2, "__index");
        lua_pushcfunction(state, luastruct_enum__gc);
        lua_setfield(state, -2, "__gc");
    }
}

int luastruct_new_enum(lua_State *state, const char *name, size_t size) {
    if(luastruct_get_type(state, name) != 0) {
        return 1;
    }

    LuastructEnum *enum_type = lua_newuserdata(state, sizeof(LuastructEnum));
    LUAS_ASSERT(state, enum_type != NULL, "Failed to allocate memory for enum userdata");
    
    strncpy(enum_type->type_info.name, name, LUASTRUCT_TYPENAME_LENGTH);
    enum_type->type_info.name[strlen(enum_type->type_info.name)] = '\0';
    enum_type->type_info.type = LUAST_ENUM;
    enum_type->size = size;
    enum_type->max_value = 0;
    enum_type->variants = NULL;

    get_enum_metatable(state);
    lua_setmetatable(state, -2);

    luastruct_get_types_registry(state);
    lua_pushvalue(state, -2);
    lua_setfield(state, -2, enum_type->type_info.name);
    lua_pop(state, 1);

    return 1;
}

int luastruct_new_enum_variant(lua_State *state, const char *name, int32_t value) {
    LuastructEnum *enum_type = luaL_checkudata(state, -1, ENUM_METATABLE_NAME);
    LUAS_ASSERT(state, enum_type != NULL, "Failed to get enum type");

    if(strlen(name) >= LUASTRUCT_TYPENAME_LENGTH) {
        return luaL_error(state, "Enum variant name too long: %s", name);
    }

    LuastructEnumVariant *variant = malloc(sizeof(LuastructEnumVariant));
    LUAS_ASSERT(state, variant != NULL, "Failed to allocate memory for enum variant");

    strncpy(variant->name, name, LUASTRUCT_TYPENAME_LENGTH);
    variant->name[strlen(variant->name)] = '\0';
    variant->value = value;
    variant->next = enum_type->variants;
    enum_type->variants = variant;

    if(value > enum_type->max_value) {
        enum_type->max_value = value;
    }

    return 0;
}

LuastructEnum *luastruct_get_enum(lua_State *state, const char *name) {
    if(!name || strlen(name) == 0) {
        luaL_error(state, "Invalid enum name");
    }

    luastruct_get_types_registry(state);
    lua_getfield(state, -1, name);
    if(lua_isnil(state, -1)) {
        lua_pop(state, 2);
        return NULL;
    }

    LuastructEnum *enum_type = luaL_checkudata(state, -1, ENUM_METATABLE_NAME);
    if(!enum_type) {
        lua_pop(state, 2);
        return NULL;
    }

    lua_pop(state, 1); // Pop the registry
    return enum_type;
}

int luastruct_get_enum_variant_by_value(lua_State *state, const LuastructEnum *enum_type, int32_t value) {
    if(!enum_type || !enum_type->variants) {
        return luaL_error(state, "Invalid enum type or no variants defined");
    }

    LuastructEnumVariant *variant = enum_type->variants;
    while(variant) {
        if(variant->value == value) {
            lua_pushlightuserdata(state, variant);
            get_enum_variant_metatable(state);
            lua_setmetatable(state, -2);
            return 1;
        }
        variant = variant->next;
    }

    lua_pushnil(state);
    return 1;
} 

int luastruct_get_enum_variant_by_name(lua_State *state, const LuastructEnum *enum_type, const char *name) {
    if(!enum_type || !enum_type->variants) {
        return luaL_error(state, "Invalid enum type or no variants defined");
    }

    LuastructEnumVariant *variant = enum_type->variants;
    while(variant) {
        if(strcmp(variant->name, name) == 0) {
            lua_pushlightuserdata(state, variant);
            get_enum_variant_metatable(state);
            lua_setmetatable(state, -2);
            return 1;
        }
        variant = variant->next;
    }

    lua_pushnil(state);
    return 1;
}
