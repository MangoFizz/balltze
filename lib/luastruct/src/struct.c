#include <stdlib.h>
#include <string.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "luastruct.h"

const char *types_registry_name = "luastruct_types";

int luastruct_new_array(lua_State *state, void *data, LuastructArrayDesc *array_info);

static void free_struct_fields_recursively(LuastructStructField *field) {
    if(field->next_by_offset) {
        free_struct_fields_recursively(field->next_by_offset);
    }
    free(field);
}

static void insert_struct_field(LuastructStruct *st, const LuastructStructField *field) {
    LuastructStructField *new_field = malloc(sizeof(LuastructStructField));
    memcpy(new_field, field, sizeof(LuastructStructField));
    new_field->next_by_offset = NULL;
    new_field->next_by_name = NULL;
    
    // Insert by offset
    LuastructStructField *prev = NULL;
    LuastructStructField *current = st->fields;
    while(current && current->offset < new_field->offset) {
        prev = current;
        current = current->next_by_offset;
    }
    if(prev) {
        prev->next_by_offset = new_field;
    } 
    else {
        st->fields = new_field;
    }
    new_field->next_by_offset = current;
    
    // Insert by name
    prev = NULL;
    current = st->fields_by_name;
    while(current && strcmp(current->field_name, new_field->field_name) < 0) {
        prev = current;
        current = current->next_by_name;
    }
    if(prev) {
        prev->next_by_name = new_field;
    }
    else {
        st->fields_by_name = new_field;
    }
    new_field->next_by_name = current;
}

int luastruct_get_type(lua_State *state, const char *name) {
    luastruct_get_types_registry(state);
    lua_getfield(state, -1, name);
    if(lua_isnil(state, -1)) {
        lua_pop(state, 2);
        return 0;
    }
    lua_remove(state, -2);
    return 1;
}

LuastructTypeInfo *get_type_info(lua_State *state, LuastructType type, const char *type_name) {
    switch(type) {
        case LUAST_STRUCT:
        case LUAST_ENUM:
            if(type_name == NULL) {
                luaL_error(state, "Type name required for struct/enum field");
            }
            luastruct_get_type(state, type_name);
            if(lua_isnil(state, -1)) {
                lua_pop(state, 1);
                return NULL;
            }
            void *info = lua_touserdata(state, -1);
            lua_pop(state, 1);
            if(info == NULL) {
                luaL_error(state, "Type info is NULL");
            }
            return info;
        default:
            return NULL;
    }
}

size_t get_type_size(lua_State *state, LuastructType type, void *type_info) {
    switch(type) {
        case LUAST_INT8:
        case LUAST_UINT8:
            return sizeof(int8_t);
        case LUAST_INT16:
        case LUAST_UINT16:
            return sizeof(int16_t);
        case LUAST_INT32:
        case LUAST_UINT32:
            return sizeof(int32_t);
        case LUAST_INT64:
        case LUAST_UINT64:
            return sizeof(int64_t);
        case LUAST_FLOAT:
            return sizeof(float);
        case LUAST_BOOL:
            return sizeof(bool);
        case LUAST_STRUCT: {
            if(type_info == NULL) {
                return luaL_error(state, "Type info is NULL");
            }
            LuastructStruct *st = type_info;
            return st->size;
        }
        case LUAST_ENUM: {
            if(type_info == NULL) {
                return luaL_error(state, "Type info is NULL");
            }
            LuastructEnum *enum_type = type_info;
            switch(enum_type->size) {
                case LUAS_ENUM_INT8:
                    return sizeof(int8_t);
                case LUAS_ENUM_INT16:
                    return sizeof(int16_t);
                case LUAS_ENUM_INT32:
                    return sizeof(int32_t);
                default:
                    return luaL_error(state, "Invalid enum type");
            }
        }
        default:
            return luaL_error(state, "Invalid type for size: %d", type);
    }
}

int luastruct_get_types_registry(lua_State *state) {
    lua_getfield(state, LUA_REGISTRYINDEX, types_registry_name);
    if(lua_isnil(state, -1)) {
        lua_pop(state, 1);
        lua_newtable(state);
        lua_pushvalue(state, -1);
        lua_setfield(state, LUA_REGISTRYINDEX, types_registry_name);
        lua_getfield(state, LUA_REGISTRYINDEX, types_registry_name);
    }
    return 1;
}

int luastruct_struct__gc(lua_State *state) {
    LuastructStruct *st = luaL_checkudata(state, 1, STRUCT_METATABLE_NAME);
    if(!st) {
        return luaL_error(state, "Invalid struct object");
    }
    if(st->fields) {
        free_struct_fields_recursively(st->fields);
    }
    return 0;
}

int luastruct_new_struct(lua_State *state, const char *name, const char *super_name, uint32_t size) {
    if(luastruct_get_type(state, name) != 0) {
        return 1;
    }

    LuastructStruct *super = NULL;
    if(super_name) {
        if(luastruct_get_type(state, super_name) == 0) {
            return luaL_error(state, "Super struct type does not exist: %s", super_name);
        }
        super = luaL_checkudata(state, -1, STRUCT_METATABLE_NAME);
    }

    if(strlen(name) >= LUASTRUCT_TYPENAME_LENGTH) {
        return luaL_error(state, "Type name too long: %s", name);
    }

    LuastructStruct *st = lua_newuserdata(state, sizeof(LuastructStruct));
    strncpy(st->type_info.name, name, LUASTRUCT_TYPENAME_LENGTH);
    st->type_info.name[strlen(st->type_info.name)] = '\0';
    st->type_info.type = LUAST_STRUCT;
    st->super = super;
    st->fields_by_name = NULL;
    st->fields = NULL;
    st->size = size;

    int metatable = luaL_newmetatable(state, STRUCT_METATABLE_NAME);
    if(metatable != 0) {
        lua_pushcfunction(state, luastruct_struct__gc);
        lua_setfield(state, -2, "__gc");
    }
    lua_setmetatable(state, -2);

    luastruct_get_types_registry(state);
    lua_pushvalue(state, -2);
    lua_setfield(state, -2, st->type_info.name);
    lua_pop(state, 1);

    return 1;
}

LuastructStruct *luastruct_check_struct(lua_State *state, int index) {
    return luaL_checkudata(state, index, STRUCT_METATABLE_NAME);
}

void luastruct_new_struct_field(lua_State *state, const char *name, LuastructType type, const char *type_name, uint32_t offset, bool pointer, bool readonly) {
    LuastructStruct *st = luastruct_check_struct(state, -1);
    if(!st) {
        luaL_error(state, "Invalid struct object");
    }

    if(strlen(name) >= LUASTRUCT_TYPENAME_LENGTH) {
        luaL_error(state, "Field name too long: %s", name);
    }

    LuastructStructField field;
    strncpy(field.field_name, name, LUASTRUCT_TYPENAME_LENGTH);
    field.field_name[strlen(field.field_name)] = '\0';
    field.type = type;
    field.type_info = NULL;
    field.offset = offset;
    field.pointer = pointer;
    field.readonly = readonly;

    if(type == LUAST_STRUCT || type == LUAST_ENUM) {
        if(!type_name) {
            luaL_error(state, "Type name required for struct/enum field");
        }
        luastruct_get_type(state, type_name);
        if(lua_isnil(state, -1)) {
            luaL_error(state, "Type not found: %s", type_name);
        }
        field.type_info = lua_touserdata(state, -1);
        lua_pop(state, 1);
    }

    insert_struct_field(st, &field);
}

void luastruct_new_struct_array_field(lua_State *state, const char *name, LuastructArrayDesc *array_info, uint32_t offset, bool pointer, bool readonly) {
    LuastructStruct *st = luastruct_check_struct(state, -1);
    if(!st) {
        luaL_error(state, "Invalid struct object");
    }

    if(strlen(name) >= LUASTRUCT_TYPENAME_LENGTH) {
        luaL_error(state, "Field name too long: %s", name);
    }

    if(array_info == NULL) {
        luaL_error(state, "Array info is NULL");
    }

    if(array_info->count_getter == NULL && array_info->array_size == 0) {
        luaL_error(state, "Array info is invalid");
    }

    LuastructStructField field;
    strncpy(field.field_name, name, LUASTRUCT_TYPENAME_LENGTH);
    field.field_name[strlen(field.field_name)] = '\0';
    field.type = LUAST_ARRAY;
    field.type_info = NULL;
    field.offset = offset;
    field.pointer = pointer;
    field.readonly = readonly;
    field.array = *array_info;

    insert_struct_field(st, &field);
}

void luastruct_new_struct_bit_field(lua_State *state, const char *name, LuastructType type, uint32_t offset, uint32_t bit_offset, bool pointer, bool readonly) {
    LuastructStruct *st = luastruct_check_struct(state, -1);
    if(!st) {
        luaL_error(state, "Invalid struct object");
    }

    if(strlen(name) >= LUASTRUCT_TYPENAME_LENGTH) {
        luaL_error(state, "Field name too long: %s", name);
    }

    uint32_t size = 0;
    switch(type) {
        case LUAST_INT8:
        case LUAST_UINT8:
            size = 1;
            break;
        case LUAST_INT16:
        case LUAST_UINT16:
            size = 2;
            break;
        case LUAST_INT32:
        case LUAST_UINT32:
            size = 4;
            break;
        default:
            luaL_error(state, "Invalid bitfield type: %d", type);
    }
    if(bit_offset >= size * 8) {
        luaL_error(state, "Bit offset out of range: %d", bit_offset);
    }

    LuastructStructField field;
    strncpy(field.field_name, name, LUASTRUCT_TYPENAME_LENGTH);
    field.field_name[strlen(field.field_name)] = '\0';
    field.type = LUAST_BITFIELD;
    field.type_info = NULL;
    field.offset = offset;
    field.pointer = pointer;
    field.readonly = readonly;
    field.bitfield.size = size;
    field.bitfield.offset = bit_offset;

    insert_struct_field(st, &field);
}


