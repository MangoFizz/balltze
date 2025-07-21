// SPDX-License-Identifier: GPL-3.0-only

#include <string>
#include <lua.hpp>
#include <luastruct/luastruct.h>
#include <luastruct/debug.h>
#include "../lua/api/v2/types.hpp"

using namespace Balltze::Lua::Api::V2;

const char *FILE_HEADER = R"(-- SPDX-License-Identifier: GPL-3.0-only
-- This file is used to document the Lua plugins engine API. It should not be included.

---@meta _
---@diagnostic disable: missing-return 
---@diagnostic disable: unused-local 

)";

void define_luastruct_types(lua_State *state) noexcept {
    define_engine_types(state);
    define_engine_tag_types(state);
    define_engine_tag_data_types(state);
    define_engine_cache_file_types(state);
    define_engine_game_engine_types(state);
    define_engine_object_types(state);
    define_engine_player_types(state);
}

const char *get_type_name(LuastructType type, LuastructTypeInfo *type_info) {
    switch(type) {
        case LUAST_INT8:
        case LUAST_INT16:
        case LUAST_INT32:
        case LUAST_UINT8:
        case LUAST_UINT16:
        case LUAST_UINT32:
            return "integer";
        case LUAST_FLOAT:
            return "number";
        case LUAST_BOOL:
            return "boolean";
        case LUAST_CHAR:
            return "string";
        case LUAST_STRING_LITERAL:
            return "string";
        case LUAST_BITFIELD:
            return "boolean";
        case LUAST_OBJREF:
        case LUAST_ENUM:
        case LUAST_STRUCT: {
            return type_info->name;
        }
        default:
            throw std::runtime_error("Unknown type: " + std::to_string(type));
    }
}

std::string ensure_no_leading_number(const std::string &name) {
    if(name.empty() || (name[0] >= '0' && name[0] <= '9')) {
        return "_" + name; // Prepend an underscore if the name starts with a digit
    }
    return name;
}

int main() {
    lua_State *state = luaL_newstate();
    define_luastruct_types(state);

    printf("%s", FILE_HEADER);

    luastruct_get_types_registry(state);
    lua_pushnil(state); 
    while (lua_next(state, -2) != 0) {
        const char *key = lua_tostring(state, -2);
        LuastructTypeInfo *type_info = static_cast<LuastructTypeInfo *>(lua_touserdata(state, -1));
        
        switch(type_info->type) {
            case LUAST_ENUM: {
                LuastructEnum *enum_type = reinterpret_cast<LuastructEnum *>(type_info);
                printf("---@alias %s\n", key);
                LuastructEnumVariant *variant = enum_type->variants;
                while(variant) {
                    printf("---| \"%s\"\n", variant->name);
                    variant = variant->next;
                }
                break;
            }
            case LUAST_STRUCT: {
                LuastructStruct *struct_type = reinterpret_cast<LuastructStruct *>(type_info);
                printf("---@class %s", key);
                if(struct_type->super) {
                    printf(": %s", struct_type->super->type_info.name);
                }
                printf("\n");
                LuastructStructField *field = struct_type->fields;
                while(field) {
                    if(field->type != LUAST_METHOD) {
                        printf("---@field %s ", ensure_no_leading_number(field->field_name).c_str());
                        switch(field->type) {
                            case LUAST_ARRAY: {
                                LuastructArrayDesc &array_info = field->array;
                                auto elements_type = array_info.elements_type;
                                auto *elements_type_info = reinterpret_cast<LuastructTypeInfo *>(array_info.elements_type_info);
                                auto *elements_type_name = get_type_name(elements_type, elements_type_info);
                                if(array_info.elements_type == LUAST_CHAR) {
                                    printf("%s", elements_type_name);
                                }
                                else if(!array_info.count_getter) {
                                    printf("%s[%zu]", elements_type_name, array_info.array_size);
                                }
                                else {
                                    printf("%s[]", elements_type_name);
                                }
                             
                                break;
                            }
                            default: {
                                printf("%s", get_type_name(field->type, reinterpret_cast<LuastructTypeInfo *>(field->type_info)));
                                break;
                            }
                        }
                        printf("\n");
                    }
                    field = field->next_by_offset;
                }
                break;
            }
        }
        
        printf("\n");

        lua_pop(state, 1); 
    }

    lua_pop(state, 1); 
    lua_close(state);

    return 0;
}
