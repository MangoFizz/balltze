-- SPDX-License-Identifier: GPL-3.0-only

local argparse = require "argparse"
local glue = require "glue"
local json = require "json"
local definitionParser = require "parse_tag_definition"

local parser = argparse("Balltze tag data lua metatables generator")
parser:argument("output", "Output file"):args(1)
parser:argument("files", "Header files"):args("*")

local args = parser:parse()
local outputFile = args.output
local files = args.files

local cpp = ""

local function add(text)
    cpp = cpp .. text
end

local function indent(n)
    add(string.rep(" ", n * 4))
end

local structs = {}
local enums = {}
local bitfields = {}
for _, file in ipairs(files) do
    local fileName = file:match("([^/]+)$")
    local definition = json.decode(glue.readfile(file))
    local definitionName = fileName:match("^(.+)%..+$")
    local parsedDefinition = definitionParser.parseDefinition(definitionName, definition)
    if(definitionName ~= "enum" and definitionName ~= "bitfield") then
        for _, struct in ipairs(parsedDefinition.structs) do
            structs[definitionParser.camelCaseToSnakeCase(struct.name)] = struct
        end
    end
    if(definitionName ~= "bitfield") then
        for _, enum in ipairs(parsedDefinition.enums) do
            enums[definitionParser.camelCaseToSnakeCase(enum.name)] = enum
        end
    end
    for _, bitfield in ipairs(parsedDefinition.bitfields) do
        bitfields[definitionParser.camelCaseToSnakeCase(bitfield.name)] = bitfield
    end
end

add([[
// SPDX-License-Identifier: GPL-3.0-only
// This file is auto-generated. DO NOT EDIT!

#include <memory>
#include <lua.hpp>
#include <balltze/engine/tag.hpp>
#include <balltze/engine/tag_definitions.hpp>

namespace Balltze::Plugins {
    using namespace Engine;
    using namespace Engine::TagDefinitions;

    void lua_push_engine_matrix(lua_State *state, Engine::Matrix &matrix) noexcept;
    std::shared_ptr<Engine::Matrix> lua_to_engine_matrix(lua_State *state, int index) noexcept;

    void lua_push_meta_engine_color_a_r_g_b_int(lua_State *state, Engine::ColorARGBInt &color) noexcept;
    void lua_push_meta_engine_tag_dependency(lua_State *state, Engine::TagDependency &dependency) noexcept;
    void lua_push_meta_engine_point2_d(lua_State *state, Engine::Point2D &point) noexcept;
    void lua_push_meta_engine_point3_d(lua_State *state, Engine::Point3D &point) noexcept;
    void lua_push_meta_engine_tag_data_offset(lua_State *state, Engine::TagDataOffset &offset) noexcept;
    void lua_push_meta_engine_color_a_r_g_b(lua_State *state, Engine::ColorARGB &color) noexcept;
    void lua_push_meta_engine_rectangle2_d(lua_State *state, Engine::Rectangle2D &rectangle) noexcept;
    void lua_push_meta_engine_point2_d_int(lua_State *state, Engine::Point2DInt &point) noexcept;
    void lua_push_meta_engine_euler2_d(lua_State *state, Engine::Euler2D &euler) noexcept;
    void lua_push_meta_engine_euler3_d(lua_State *state, Engine::Euler3D &euler) noexcept;
    void lua_push_meta_engine_vector2_d(lua_State *state, Engine::Vector2D &vector) noexcept;
    void lua_push_meta_engine_vector3_d(lua_State *state, Engine::Vector3D &vector) noexcept;
    void lua_push_meta_engine_color_r_g_b(lua_State *state, Engine::ColorRGB &color) noexcept;
    void lua_push_meta_engine_quaternion(lua_State *state, Engine::Quaternion &quaternion) noexcept;
    void lua_push_meta_engine_plane3_d(lua_State *state, Engine::Plane3D &plane) noexcept;
    void lua_push_meta_engine_plane2_d(lua_State *state, Engine::Plane2D &plane) noexcept;

    template<typename T>
    void lua_push_meta_object(lua_State *state, T &elem, lua_CFunction index, lua_CFunction newindex) noexcept {
        // Create table for vector
        lua_newtable(state);
        lua_pushlightuserdata(state, reinterpret_cast<void *>(const_cast<T *>(&elem)));
        lua_setfield(state, -2, "_data");

        // Create metatable for vector 3D
        lua_newtable(state);
        lua_pushcfunction(state, index);
        lua_setfield(state, -2, "__index");
        lua_pushcfunction(state, newindex);
        lua_setfield(state, -2, "__newindex");

        // Set metatable for vector
        lua_setmetatable(state, -2);
    }

    #define LUA_ENGINE_TAG_REFLEXIVE__INDEX_LAMBDA(type, push_function) \
        +[](lua_State *state) -> int { \
            lua_getfield(state, 1, "_data"); \
            auto *elem = static_cast<Engine::TagReflexive<type> *>(lua_touserdata(state, -1)); \
            lua_pop(state, 1); \
            auto key = luaL_checkinteger(state, 2); \
            if (key < 0 || key >= elem->count) { \
                return luaL_error(state, "Index out of bounds"); \
            } \
            push_function(state, elem->offset[key]); \
            return 1; \
        }

    int lua_engine_tag_reflexive__newindex(lua_State *state) noexcept {
        return luaL_error(state, "Invalid operation");
    }
    
]])

for structName, _ in pairs(structs) do
    local camelCaseName = definitionParser.snakeCaseToCamelCase(structName)
    indent(1)
    add("int lua_engine_" .. structName .. "__index(lua_State *state) noexcept; \n");
    indent(1)
    add("int lua_engine_" .. structName .. "__newindex(lua_State *state) noexcept; \n");
    indent(1)
    add("void lua_push_meta_engine_" .. structName .. "(lua_State *state, " .. camelCaseName .. " &data) noexcept; \n");
end
add("\n")

for bitfieldName, _ in pairs(bitfields) do
    local camelCaseName = definitionParser.snakeCaseToCamelCase(bitfieldName)
    indent(1)
    add("int lua_engine_" .. bitfieldName .. "__index(lua_State *state) noexcept; \n");
    indent(1)
    add("int lua_engine_" .. bitfieldName .. "__newindex(lua_State *state) noexcept; \n");
    indent(1)
    add("void lua_push_meta_engine_" .. bitfieldName .. "(lua_State *state, " .. camelCaseName .. " &data) noexcept; \n");
end
add("\n")

for enumName, _ in pairs(enums) do
    local camelCaseName = definitionParser.snakeCaseToCamelCase(enumName)
    indent(1)
    add("std::string lua_engine_" .. enumName .. "_to_string(" .. camelCaseName .. " value) noexcept; \n");
    indent(1)
    add(camelCaseName .. " lua_engine_" .. enumName .. "_from_string(std::string str); \n");
end
add("\n")

for structName, struct in pairs(structs) do
    local sneakCaseName = definitionParser.camelCaseToSnakeCase(structName)
    local camelCaseName = definitionParser.snakeCaseToCamelCase(sneakCaseName)
    indent(1)
    add("int lua_engine_" .. sneakCaseName .. "__index(lua_State *state) noexcept { \n");
    indent(2)
    add("lua_getfield(state, 1, \"_data\"); \n");
    indent(2)
    add("auto data = static_cast<" .. camelCaseName .. " *>(lua_touserdata(state, -1)); \n");
    indent(2)
    add("lua_pop(state, 1); \n");
    indent(2)
    add("auto *key = lua_tostring(state, 2); \n");
    indent(2)
    add("if(key == nullptr) { \n");
    indent(3)
    add("return luaL_error(state, \"Invalid key type\"); \n");
    indent(2)
    add("} \n");
    indent(2)
    add("std::string field = key; \n");
    indent(2)
    add("if(field == \"_data\") { \n");
    indent(3)
    add("lua_pushnil(state); \n");
    indent(3)
    add("return 1; \n");
    indent(2)
    add("} \n");
    for _, field in ipairs(struct.fields) do
        local sneakCaseFieldType = definitionParser.camelCaseToSnakeCase(field.type)
        local camelCaseFieldType = definitionParser.snakeCaseToCamelCase(sneakCaseFieldType)

        if(field.name == nil) then
            goto continue_index_fields
        end

        if(definitionParser.commonStructs[sneakCaseFieldType] == nil and struct[sneakCaseFieldType] == nil) then
            if(definitionParser.commonEnums[sneakCaseFieldType] == nil and enums[sneakCaseFieldType] == nil) then
                if(definitionParser.commonBitfields[sneakCaseFieldType] == nil and bitfields[sneakCaseFieldType] == nil) then
                    if(definitionParser.primitiveTypes[sneakCaseFieldType] == nil) then
                        error("Unknown type " .. field.type .. " in " .. structName .. " " .. field.name)
                    end
                end
            end
        end

        indent(2)
        add("else if(field == \"" .. field.name .. "\") { \n");
        indent(3)

        local size = field.size or 1

        if(definitionParser.commonStructs[sneakCaseFieldType] ~= nil or struct[sneakCaseFieldType]) then
            if(sneakCaseFieldType == "angle" or sneakCaseFieldType == "fraction") then
                if(size == 1) then
                    add("lua_pushnumber(state, static_cast<float>(data->" .. field.name .. ")); \n");
                else
                    add("lua_newtable(state); \n");
                    for i = 0, size - 1 do
                        indent(3)
                        add("lua_pushinteger(state, static_cast<float>(data->" .. field.name .. "[" .. i .. "])); \n");
                        indent(3)
                        add("lua_rawseti(state, -2, " .. i + 1 .. "); \n");
                    end
                end
            elseif(sneakCaseFieldType == "index" or sneakCaseFieldType == "tag_enum") then
                if(size == 1) then
                    add("lua_pushinteger(state, data->" .. field.name .. "); \n");
                else
                    add("lua_newtable(state); \n");
                    for i = 0, size - 1 do
                        indent(3)
                        add("lua_pushinteger(state, data->" .. field.name .. "[" .. i .. "]); \n");
                        indent(3)
                        add("lua_rawseti(state, -2, " .. i + 1 .. "); \n");
                    end
                end
            elseif(sneakCaseFieldType == "tag_string") then
                add("lua_pushlstring(state, data->" .. field.name .. ".string, sizeof(data->" .. field.name .. ")); \n");
            elseif(sneakCaseFieldType == "tag_four_c_c") then
                add("auto tag_class = tag_class_to_string(data->".. field.name .. "); \n");
                indent(3)
                add("lua_pushstring(state, tag_class.c_str()); \n");
            elseif(sneakCaseFieldType == "matrix") then
                add("lua_push_engine_matrix(state, data->" .. field.name .. "); \n");
            elseif(sneakCaseFieldType == "tag_handle") then
                add("lua_pushinteger(state, data->" .. field.name .. ".handle); \n");
            elseif(sneakCaseFieldType == "scenario_script_node_value") then
                add("lua_pushinteger(state, data->" .. field.name .. ".tag_handle.handle); \n");
            elseif(sneakCaseFieldType == "tag_reflexive") then
                local sneakCaseStructName = definitionParser.camelCaseToSnakeCase(field.struct)
                local camelCaseStructName = definitionParser.snakeCaseToCamelCase(sneakCaseStructName)
                add("lua_CFunction index = LUA_ENGINE_TAG_REFLEXIVE__INDEX_LAMBDA(" .. camelCaseStructName .. ", lua_push_meta_engine_" .. sneakCaseStructName .. "); \n");
                indent(3)
                add("lua_push_meta_object(state, data->" .. field.name .. ", index, lua_engine_tag_reflexive__newindex); \n");
            else
                add("lua_push_meta_engine_" .. sneakCaseFieldType .. "(state, data->" .. field.name .. "); \n");
            end
        elseif(definitionParser.commonEnums[sneakCaseFieldType] ~= nil or enums[sneakCaseFieldType]) then
            add("lua_pushstring(state, lua_engine_" .. sneakCaseFieldType .. "_to_string(data->" .. field.name .. ").c_str()); \n");
        elseif(definitionParser.commonBitfields[sneakCaseFieldType] ~= nil or bitfields[sneakCaseFieldType]) then
            add("lua_push_meta_engine_" .. sneakCaseFieldType .. "(state, data->" .. field.name .. "); \n");
        else
            if(field.type == "char") then
                if(size == 1) then
                    add("char buffer[2] = {data->" .. field.name .. ", '\\0'}; \n");
                    indent(3)
                    add("lua_pushstring(state, buffer); \n");
                else
                    add("lua_newtable(state); \n");
                    for i = 0, size - 1 do
                        indent(3)
                        add("char buffer[2] = {data->" .. field.name .. "[" .. i .. "], '\\0'}; \n");
                        indent(3)
                        add("lua_pushstring(state, buffer); \n");
                        indent(3)
                        add("lua_rawseti(state, -2, " .. i + 1 .. "); \n");
                    end
                end
            elseif(field.type == "byte") then
                if(size == 1) then
                    local is_pointer = field.pointer or false
                    if(not is_pointer) then
                        add("lua_pushinteger(state, reinterpret_cast<std::uint8_t>(data->" .. field.name .. ")); \n");
                    else 
                        add("lua_pushinteger(state, reinterpret_cast<std::uint32_t>(data->" .. field.name .. ")); \n");
                    end
                else
                    add("lua_newtable(state); \n");
                    for i = 0, size - 1 do
                        indent(3)
                        add("lua_pushinteger(state, reinterpret_cast<std::uint8_t>(data->" .. field.name .. "[" .. i .. "])); \n");
                        indent(3)
                        add("lua_rawseti(state, -2, " .. i + 1 .. "); \n");
                    end
                end
            elseif(field.type == "float") then
                if(size == 1) then
                    add("lua_pushnumber(state, data->" .. field.name .. "); \n");
                else
                    add("lua_newtable(state); \n");
                    for i = 0, size - 1 do
                        indent(3)
                        add("lua_pushnumber(state, data->" .. field.name .. "[" .. i .. "]); \n");
                        indent(3)
                        add("lua_rawseti(state, -2, " .. i + 1 .. "); \n");
                    end
                end
            elseif(field.type == "int32" or field.type == "int16" or field.type == "int8" or field.type == "uint32" or field.type == "uint16" or field.type == "uint8") then
                if(size == 1) then
                    add("lua_pushinteger(state, data->" .. field.name .. "); \n");
                else 
                    add("lua_newtable(state); \n");
                    for i = 0, size - 1 do
                        indent(3)
                        add("lua_pushinteger(state, data->" .. field.name .. "[" .. i .. "]); \n");
                        indent(3)
                        add("lua_rawseti(state, -2, " .. i + 1 .. "); \n");
                    end
                end
            else
                error("Unknown type " .. field.type .. " in " .. structName .. " " .. field.name)
            end
        end

        indent(2)
        add("} \n");
        
        ::continue_index_fields::
    end

    indent(2)
    add("else { \n")
    if(struct.inherits) then
        indent(3)
        add("return lua_engine_" .. definitionParser.camelCaseToSnakeCase(struct.inherits) .. "__index(state); \n");
    else
        indent(3)
        add("return luaL_error(state, \"Unknown field %s\", key); \n");
    end
    indent(2)
    add("} \n");

    indent(2)
    add("return 1; \n");
    indent(1)
    add("} \n");

    indent(1)
    add("int lua_engine_" .. sneakCaseName .. "__newindex(lua_State *state) noexcept { \n");
    indent(2)
    add("lua_getfield(state, 1, \"_data\"); \n");
    indent(2)
    add("auto data = static_cast<" .. camelCaseName .. " *>(lua_touserdata(state, -1)); \n");
    indent(2)
    add("lua_pop(state, 1); \n");
    indent(2)
    add("auto *key = lua_tostring(state, 2); \n");
    indent(2)
    add("if(key == nullptr) { \n");
    indent(3)
    add("return luaL_error(state, \"Invalid key type\"); \n");
    indent(2)
    add("} \n");
    indent(2)
    add("std::string field = key; \n");
    indent(2)
    add("if(field == \"_data\") { \n");
    indent(3)
    add("lua_pushnil(state); \n");
    indent(3)
    add("return 1; \n");
    indent(2)
    add("} \n");
    for _, field in ipairs(struct.fields) do
        local sneakCaseFieldType = definitionParser.camelCaseToSnakeCase(field.type)
        local camelCaseFieldType = definitionParser.snakeCaseToCamelCase(sneakCaseFieldType)

        if(field.name == nil) then
            goto continue_new_index_fields
        end

        if(definitionParser.commonStructs[sneakCaseFieldType] == nil and struct[sneakCaseFieldType] == nil) then
            if(definitionParser.commonEnums[sneakCaseFieldType] == nil and enums[sneakCaseFieldType] == nil) then
                if(definitionParser.commonBitfields[sneakCaseFieldType] == nil and bitfields[sneakCaseFieldType] == nil) then
                    if(definitionParser.primitiveTypes[sneakCaseFieldType] == nil) then
                        error("Unknown type " .. field.type .. " in " .. structName .. " " .. field.name)
                    end
                end
            end
        end

        indent(2)
        add("else if(field == \"" .. field.name .. "\") { \n");
        indent(3)

        local size = field.size or 1

        if(definitionParser.commonStructs[sneakCaseFieldType] ~= nil or struct[sneakCaseFieldType]) then
            if(sneakCaseFieldType == "angle" or sneakCaseFieldType == "fraction") then
                if(size == 1) then
                    add("float value = luaL_checknumber(state, 3); \n");
                    indent(3)
                    add("data->" .. field.name .. " = static_cast<" .. camelCaseFieldType .. ">(value); \n");
                else
                    add("if(lua_type(state, 3) != LUA_TTABLE) { \n");
                    indent(4)
                    add("return luaL_error(state, \"Invalid value type\"); \n");
                    indent(3)
                    add("} \n");
                    for i = 0, size - 1 do
                        indent(3)
                        add("lua_rawgeti(state, 3, " .. i + 1 .. "); \n");
                        indent(3)
                        add("if(!lua_isnil(state, -1)) { \n");
                        indent(4)
                        add("float value = luaL_checknumber(state, -1); \n");
                        indent(4)
                        add("data->" .. field.name .. "[" .. i .. "] = static_cast<" .. camelCaseFieldType .. ">(value); \n");
                        indent(3)
                        add("} \n");
                        indent(3)
                        add("lua_pop(state, 1); \n");
                    end
                end
            elseif(sneakCaseFieldType == "index" or sneakCaseFieldType == "tag_enum") then
                if(size == 1) then
                    add("int value = luaL_checkinteger(state, 3); \n");
                    indent(3)
                    add("data->" .. field.name .. " = static_cast<" .. camelCaseFieldType .. ">(value); \n");
                else
                    add("if(lua_type(state, 3) != LUA_TTABLE) { \n");
                    indent(4)
                    add("return luaL_error(state, \"Invalid value type\"); \n");
                    indent(3)
                    add("} \n");
                    for i = 0, size - 1 do
                        indent(3)
                        add("lua_rawgeti(state, 3, " .. i + 1 .. "); \n");
                        indent(3)
                        add("if(!lua_isnil(state, -1)) { \n");
                        indent(4)
                        add("int value = luaL_checkinteger(state, -1); \n");
                        indent(4)
                        add("data->" .. field.name .. "[" .. i .. "] = static_cast<" .. camelCaseFieldType .. ">(value); \n");
                        indent(3)
                        add("} \n");
                        indent(3)
                        add("lua_pop(state, 1); \n");
                    end
                end
            elseif(sneakCaseFieldType == "tag_string") then
                add("const char *value = luaL_checkstring(state, 3); \n");
                indent(3)
                add("if(strlen(value) > sizeof(" .. camelCaseFieldType .. ")) { \n");
                indent(4)
                add("return luaL_error(state, \"String too long\"); \n");
                indent(3)
                add("} \n");
                indent(3)
                add("strcpy(data->" .. field.name .. ".string, value); \n");
            elseif(sneakCaseFieldType == "tag_four_c_c") then
                add("const char *value = luaL_checkstring(state, 3); \n");
                indent(3)
                add("data->" .. field.name .. " = *reinterpret_cast<TagClassInt const *>(value); \n");
            elseif(sneakCaseFieldType == "matrix") then
                add("auto matrix = lua_to_engine_matrix(state, 3); \n");
                indent(3)
                add("for(int i = 0; i < 3; i++) { \n");
                indent(4)
                add("for(int j = 0; j < 3; j++) { \n");
                indent(5)
                add("data->" .. field.name .. "[i][j] = matrix[i][j]; \n");
                indent(4)
                add("} \n");
                indent(3)
                add("} \n");
            elseif(sneakCaseFieldType == "tag_handle") then
                add("auto tag_handle = luaL_checkinteger(state, 3); \n");
                indent(3)
                add("data->" .. field.name .. ".handle = tag_handle; \n");
            elseif(sneakCaseFieldType == "scenario_script_node_value") then
                add("auto tag_handle = luaL_checkinteger(state, 3); \n");
                indent(3)
                add("data->" .. field.name .. ".tag_handle.handle = tag_handle; \n");
            else
                add("return luaL_error(state, \"Invalid operation\"); \n");
            end
        elseif(definitionParser.commonEnums[sneakCaseFieldType] ~= nil or enums[sneakCaseFieldType]) then
            add("const char *value = luaL_checkstring(state, 3); \n");
            indent(3)
            add("try { \n");
            indent(4)
            add(camelCaseFieldType .. " enum_value = lua_engine_" .. sneakCaseFieldType .. "_from_string(value); \n");
            indent(4)
            add("data->" .. field.name .. " = enum_value; \n");
            indent(3)
            add("} \n");
            indent(3)
            add("catch(const std::runtime_error &e) { \n");
            indent(4)
            add("return luaL_error(state, e.what()); \n");
            indent(3)
            add("} \n");
        elseif(definitionParser.commonBitfields[sneakCaseFieldType] ~= nil or bitfields[sneakCaseFieldType]) then
            add("return luaL_error(state, \"Invalid operation\"); \n");
        else
            if(field.type == "char") then
                if(size == 1) then
                    add("const char *value = luaL_checkstring(state, 3); \n");
                    indent(3)
                    add("if(strlen(value) > sizeof(" .. camelCaseFieldType .. ")) { \n");
                    indent(4)
                    add("return luaL_error(state, \"String too long\"); \n");
                    indent(3)
                    add("} \n");
                    indent(3)
                    add("strcpy(data->" .. field.name .. ", value); \n");
                else
                    add("if(!lua_istable(state, 3)) { \n");
                    indent(4)
                    add("return luaL_error(state, \"Expected table\"); \n");
                    indent(3)
                    add("} \n");
                    indent(3)
                    add("for(int i = 0; i < " .. size .. "; i++) { \n");
                    indent(4)
                    add("lua_rawgeti(state, 3, i + 1); \n");
                    indent(4)
                    add("const char *value = luaL_checkstring(state, -1); \n");
                    indent(4)
                    add("if(strlen(value) > sizeof(" .. camelCaseFieldType .. ")) { \n");
                    indent(5)
                    add("return luaL_error(state, \"String too long\"); \n");
                    indent(4)
                    add("} \n");
                    indent(4)
                    add("strcpy(data->" .. field.name .. "[i], value); \n");
                    indent(4)
                    add("lua_pop(state, 1); \n");
                    indent(3)
                    add("} \n");
                end
            elseif(field.type == "byte") then
                if(size == 1) then
                    local is_pointer = field.pointer or false
                    if(not is_pointer) then
                        add("std::uint8_t value = luaL_checkinteger(state, 3); \n");
                        indent(3)
                        add("data->" .. field.name .. " = reinterpret_cast<std::byte>(value); \n");
                    else
                        add("std::uint32_t value = luaL_checkinteger(state, 3); \n");
                        indent(3)
                        add("data->" .. field.name .. " = reinterpret_cast<std::byte *>(value); \n");
                    end
                else
                    add("if(!lua_istable(state, 3)) { \n");
                    indent(4)
                    add("return luaL_error(state, \"Expected table\"); \n");
                    indent(3)
                    add("} \n");
                    indent(3)
                    add("for(int i = 0; i < " .. size .. "; i++) { \n");
                    indent(4)
                    add("lua_rawgeti(state, 3, i + 1); \n");
                    indent(4)
                    add("std::uint8_t value = luaL_checkinteger(state, -1); \n");
                    indent(4)
                    add("data->" .. field.name .. "[i] = reinterpret_cast<std::byte>(value); \n");
                    indent(4)
                    add("lua_pop(state, 1); \n");
                    indent(3)
                    add("} \n");
                end
            elseif(field.type == "float") then
                if(size == 1) then
                    add("float value = luaL_checknumber(state, 3); \n");
                    indent(3)
                    add("data->" .. field.name .. " = value; \n");
                else
                    add("if(!lua_istable(state, 3)) { \n");
                    indent(4)
                    add("return luaL_error(state, \"Expected table\"); \n");
                    indent(3)
                    add("} \n");
                    indent(3)
                    add("for(int i = 0; i < " .. size .. "; i++) { \n");
                    indent(4)
                    add("lua_rawgeti(state, 3, i + 1); \n");
                    indent(4)
                    add("float value = luaL_checknumber(state, -1); \n");
                    indent(4)
                    add("data->" .. field.name .. "[i] = value; \n");
                    indent(4)
                    add("lua_pop(state, 1); \n");
                    indent(3)
                    add("} \n");
                end
            elseif(field.type == "int32" or field.type == "int16" or field.type == "int8" or field.type == "uint32" or field.type == "uint16" or field.type == "uint8") then
                if(size == 1) then
                    add("std::" .. field.type .. "_t value = luaL_checkinteger(state, 3); \n");
                    indent(3)
                    add("data->" .. field.name .. " = value; \n");
                else
                    add("if(!lua_istable(state, 3)) { \n");
                    indent(4)
                    add("return luaL_error(state, \"Expected table\"); \n");
                    indent(3)
                    add("} \n");
                    indent(3)
                    add("for(int i = 0; i < " .. size .. "; i++) { \n");
                    indent(4)
                    add("lua_rawgeti(state, 3, i + 1); \n");
                    indent(4)
                    add("std::" .. field.type .. "_t value = luaL_checkinteger(state, -1); \n");
                    indent(4)
                    add("data->" .. field.name .. "[i] = value; \n");
                    indent(4)
                    add("lua_pop(state, 1); \n");
                    indent(3)
                    add("} \n");
                end
            else
                error("Unknown type " .. field.type .. " in " .. structName .. " " .. field.name)
            end
        end

        indent(2)
        add("} \n");

        ::continue_new_index_fields::
    end

    indent(2)
    add("else { \n")
    if(struct.inherits) then
        indent(3)
        add("return lua_engine_" .. definitionParser.camelCaseToSnakeCase(struct.inherits) .. "__newindex(state); \n");
    else
        indent(3)
        add("return luaL_error(state, \"Unknown field %s\", key); \n");
    end
    indent(2)
    add("} \n");

    indent(2)
    add("return 1; \n");
    indent(1)
    add("} \n");

    indent(1)
    add("void lua_push_meta_engine_" .. structName .. "(lua_State *state, " .. camelCaseName .. " &data) noexcept { \n")
    indent(2)
    add("lua_push_meta_object(state, data, lua_engine_" .. structName .. "__index, lua_engine_" .. structName .. "__newindex); \n");
    indent(1)
    add("} \n");
end

for bitfieldName, bitfield in pairs(bitfields) do
    local sneakCaseName = definitionParser.camelCaseToSnakeCase(bitfieldName)
    local camelCaseName = definitionParser.snakeCaseToCamelCase(sneakCaseName)
    indent(1)
    add("int lua_engine_" .. sneakCaseName .. "__index(lua_State *state) noexcept { \n");
    indent(2)
    add("lua_getfield(state, 1, \"_data\"); \n");
    indent(2)
    add("auto data = static_cast<" .. camelCaseName .. " *>(lua_touserdata(state, -1)); \n");
    indent(2)
    add("lua_pop(state, 1); \n");
    indent(2)
    add("auto *key = lua_tostring(state, 2); \n");
    indent(2)
    add("if(key == nullptr) { \n");
    indent(3)
    add("return luaL_error(state, \"Invalid key type\"); \n");
    indent(2)
    add("} \n");
    indent(2)
    add("std::string field = key; \n");
    indent(2)
    add("if(field == \"_data\") { \n");
    indent(3)
    add("lua_pushnil(state); \n");
    indent(3)
    add("return 1; \n");
    indent(2)
    add("} \n");
    for _, field in pairs(bitfield.fields) do
        local sneakCaseFieldName = definitionParser.camelCaseToSnakeCase(field)
        indent(2)
        add("else if(field == \"" .. sneakCaseFieldName .. "\") { \n");
        indent(3)
        add("lua_pushboolean(state, data->" .. field .. "); \n");
        indent(3)
        add("return 1; \n");
        indent(2)
        add("} \n");
    end
    indent(2)
    add("return luaL_error(state, \"Unknown field %s\", key); \n");
    indent(1)
    add("} \n");

    indent(1)
    add("int lua_engine_" .. sneakCaseName .. "__newindex(lua_State *state) noexcept { \n");
    indent(2)
    add("lua_getfield(state, 1, \"_data\"); \n");
    indent(2)
    add("auto data = static_cast<" .. camelCaseName .. " *>(lua_touserdata(state, -1)); \n");
    indent(2)
    add("lua_pop(state, 1); \n");
    indent(2)
    add("auto *key = lua_tostring(state, 2); \n");
    indent(2)
    add("if(key == nullptr) { \n");
    indent(3)
    add("return luaL_error(state, \"Invalid key type\"); \n");
    indent(2)
    add("} \n");
    indent(2)
    add("std::string field = key; \n");
    indent(2)
    add("if(field == \"_data\") { \n");
    indent(3)
    add("lua_pushnil(state); \n");
    indent(3)
    add("return 1; \n");
    indent(2)
    add("} \n");
    for _, field in pairs(bitfield.fields) do
        local sneakCaseFieldName = definitionParser.camelCaseToSnakeCase(field)
        indent(2)
        add("else if(field == \"" .. sneakCaseFieldName .. "\") { \n");
        indent(3)
        add("if(!lua_isboolean(state, 3)) { \n");
        indent(4)
        add("return luaL_error(state, \"Expected boolean\"); \n");
        indent(3)
        add("} \n");
        indent(3)
        add("data->" .. field .. " = lua_toboolean(state, 3); \n");
        indent(3)
        add("return 1; \n");
        indent(2)
        add("} \n");
    end
    indent(2)
    add("return luaL_error(state, \"Unknown field %s\", key); \n");
    indent(1)
    add("} \n");

    indent(1)
    add("void lua_push_meta_engine_" .. sneakCaseName .. "(lua_State *state, " .. camelCaseName .. " &data) noexcept { \n")
    indent(2)
    add("lua_push_meta_object(state, data, lua_engine_" .. sneakCaseName .. "__index, lua_engine_" .. sneakCaseName .. "__newindex); \n");
    indent(1)
    add("} \n");
end

for enumName, enum in pairs(enums) do
    local sneakCaseName = definitionParser.camelCaseToSnakeCase(enumName)
    local camelCaseName = definitionParser.snakeCaseToCamelCase(sneakCaseName)

    indent(1)
    add("std::string lua_engine_" .. enumName .. "_to_string(" .. camelCaseName .. " value) noexcept { \n");
    indent(2)
    add("switch(value) { \n");
    for _, value in pairs(enum.values) do
        local sneakCaseValue = value
        local camelCaseValue = definitionParser.snakeCaseToCamelCase(sneakCaseValue)
        local clean = definitionParser.camelCaseToSnakeCase(camelCaseValue)
        indent(3)
        add("case " .. value:upper() .. ": return \"" .. clean .. "\"; \n");
    end
    indent(3)
    add("default: return \"unknown\"; \n");
    indent(2)
    add("} \n");
    indent(1)
    add("} \n");

    indent(1)
    add(camelCaseName .. " lua_engine_" .. enumName .. "_from_string(std::string str) { \n");
    indent(2)
    add("if(str == \"unknown\") { \n");
    indent(3)
    add("throw std::runtime_error(\"Unknown value for " .. camelCaseName .. " enum\"); \n");
    indent(2)
    add("} \n");
    for _, value in pairs(enum.values) do
        local sneakCaseValue = value
        local camelCaseValue = definitionParser.snakeCaseToCamelCase(sneakCaseValue)
        local clean = definitionParser.camelCaseToSnakeCase(camelCaseValue)
        indent(2)
        add("if(str == \"" .. clean .. "\") { \n");
        indent(3)
        add("return " .. value:upper() .. "; \n");
        indent(2)
        add("} \n");
    end
    indent(2)
    add("throw std::runtime_error(\"Unknown value for " .. camelCaseName .. " enum\"); \n");
    indent(1)
    add("} \n");
end

add([[
    void lua_attach_tag_data_metatable(lua_State *state) noexcept {
        lua_getfield(state, -1, "_tag_class");
        auto tag_class_int = static_cast<TagClassInt>(luaL_checkinteger(state, -1));
        lua_pop(state, 1);

        switch(tag_class_int) {
]])

for _, class in ipairs(definitionParser.tagClasses) do
    local classEnumName = class:upper()
    local sneakCaseName = class
    if(sneakCaseName == "ui_widget_collection") then
        sneakCaseName = "tag_collection"
    end
    local classCamelCaseName = definitionParser.snakeCaseToCamelCase(sneakCaseName)
    indent(3)
    add("case TAG_CLASS_" .. classEnumName .. ": { \n");
    indent(4)
    add("lua_getfield(state, -1, \"_tag_data\"); \n");
    indent(4)
    add("auto &obj = *reinterpret_cast<" .. classCamelCaseName .. " *>(lua_touserdata(state, -1)); \n");
    indent(4)
    add("lua_pop(state, 1); \n");
    indent(4)
    add("lua_push_meta_engine_" .. sneakCaseName .. "(state, obj); \n");
    indent(4)
    add("break; \n");
    indent(3)
    add("} \n");
end

add([[
            default: {
                lua_pushnil(state);
                break;
            }
        }
        lua_remove(state, -2);
    }
}
]])

-- Write the file
glue.writefile(outputFile, cpp, "t")
