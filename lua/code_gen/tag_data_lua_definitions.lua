-- SPDX-License-Identifier: GPL-3.0-only

local argparse = require "argparse"
local glue = require "glue"
local json = require "json"
local definitionParser = require "parse_tag_definition"

local parser = argparse("Balltze tag data luacstruct definitions generator")
parser:argument("outputSource", "Output source file"):args(1)
parser:argument("outputHeader", "Output header file"):args(1)
parser:argument("files", "Header files"):args("*")

local args = parser:parse()
local outputFile = args.outputSource
local outputHeader = args.outputHeader
local files = args.files

local function toStdType(typeName)
    return "std::" .. typeName .. "_t"
end

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

#ifndef BALLTZE__PLUGINS__LUA__TYPES__ENGINE_TAG_DATA_HPP
#define BALLTZE__PLUGINS__LUA__TYPES__ENGINE_TAG_DATA_HPP

#include <memory>
#include <balltze/engine/tag.hpp>
#include <balltze/engine/tag_definitions.hpp>
#include "engine_types.hpp"
#include "engine_tag.hpp"

namespace Balltze::Plugins::Lua {
]])

for structName, _ in pairs(structs) do
    local camelCaseName = definitionParser.snakeCaseToCamelCase(structName)
    indent(1)
    add("using Engine" .. camelCaseName .. " = Engine::TagDefinitions::" .. camelCaseName .. "; \n")
end

for bitfieldName, _ in pairs(bitfields) do
    local camelCaseName = definitionParser.snakeCaseToCamelCase(bitfieldName)
    indent(1)
    add("using Engine" .. camelCaseName .. " = Engine::TagDefinitions::" .. camelCaseName .. "; \n")
end

for enumName, _ in pairs(enums) do
    local camelCaseName = definitionParser.snakeCaseToCamelCase(enumName)
    indent(1)
    add("using Engine" .. camelCaseName .. " = Engine::TagDefinitions::" .. camelCaseName .. "; \n");
end

add([[

    void define_engine_tag_data_types(lua_State *state) noexcept;
}


#endif

]])

glue.writefile(outputHeader, cpp, "t")

cpp = ""

add([[
// SPDX-License-Identifier: GPL-3.0-only
// This file is auto-generated. DO NOT EDIT!

#include "../helpers/luacstruct.hpp"
#include "../helpers/enum.hpp"
#include "../helpers/bitfield.hpp"
#include "engine_tag_data.hpp"

namespace Balltze::Plugins::Lua {
    #define define_tag_block(state, parentType, type) \
        using parentType##TagBlock##type = Engine::TagBlock<Engine::TagDefinitions::type>; \
        luacs_newstruct(state, parentType##TagBlock##type); \
        luacs_unsigned_field(state, parentType##TagBlock##type, count, LUACS_FREADONLY); \
        luacs_declare_field(state, LUACS_TOBJREF, "Engine" #type, "elements", sizeof(((parentType##TagBlock##type *)0)->elements[0]), offsetof(parentType##TagBlock##type, elements), 65535, LUACS_FREADONLY); \
        lua_pop(state, 1);

]])

add("\n")

for structName, struct in pairs(structs) do
    local sneakCaseName = definitionParser.camelCaseToSnakeCase(structName)
    local camelCaseName = definitionParser.snakeCaseToCamelCase(sneakCaseName)
    local typename = "Engine" .. camelCaseName

    indent(1)
    add("inline void create_engine_" .. structName .. "_struct(lua_State *state) noexcept { \n")
    indent(2)
    if(struct.inherits) then
        local sneakCaseParentName = definitionParser.camelCaseToSnakeCase(struct.inherits)
        local camelCaseParentName = definitionParser.snakeCaseToCamelCase(sneakCaseParentName)
        add("luacs_newderivedstruct(state, " .. typename .. ", Engine" .. camelCaseParentName .. "); \n")
    else 
        add("luacs_newstruct(state, " .. typename .. "); \n")
    end
    indent(2)
    add("lua_pop(state, 1); \n")
    indent(1)
    add("} \n\n")

    indent(1)
    add("static void define_engine_" .. structName .. "_struct(lua_State *state) noexcept { \n")
    indent(2)
    local parentTypename = "NULL"
    if(struct.inherits) then
        local sneakCaseParentName = definitionParser.camelCaseToSnakeCase(struct.inherits)
        local camelCaseParentName = definitionParser.snakeCaseToCamelCase(sneakCaseParentName)
        add("luacs_newderivedstruct(state, " .. typename .. ", Engine" .. camelCaseParentName .. "); \n")
    else 
        add("luacs_newstruct(state, " .. typename .. "); \n")
    end
    for _, field in ipairs(struct.fields) do
        if(field.name) then
            local sneakCaseFieldType = definitionParser.camelCaseToSnakeCase(field.type)
            local camelCaseFieldType = definitionParser.snakeCaseToCamelCase(sneakCaseFieldType)
            
            -- Check if type exists
            if(definitionParser.commonStructs[sneakCaseFieldType] == nil and struct[sneakCaseFieldType] == nil) then
                if(definitionParser.commonEnums[sneakCaseFieldType] == nil and enums[sneakCaseFieldType] == nil) then
                    if(definitionParser.commonBitfields[sneakCaseFieldType] == nil and bitfields[sneakCaseFieldType] == nil) then
                        if(definitionParser.primitiveTypes[sneakCaseFieldType] == nil) then
                            error("Unknown type " .. field.type .. " in " .. structName .. " " .. field.name)
                        end
                    end
                end
            end

            local size = field.size or 1
            indent(2)
            if(definitionParser.commonStructs[sneakCaseFieldType] ~= nil or struct[sneakCaseFieldType]) then
                if(sneakCaseFieldType == "angle" or sneakCaseFieldType == "fraction") then
                    if(size == 1) then
                        add("luacs_float_field(state, " .. typename .. ", " .. field.name .. ", 0); \n");
                    else
                        add("luacs_float_array_field(state, " .. typename .. ", " .. field.name .. ", 0); \n");
                    end
                elseif(sneakCaseFieldType == "index" or sneakCaseFieldType == "tag_enum") then
                    if(size == 1) then
                        add("luacs_unsigned_field(state, " .. typename .. ", " .. field.name .. ", 0); \n");
                    else
                        add("luacs_unsigned_array_field(state, " .. typename .. ", " .. field.name .. ", 0); \n");
                    end
                elseif(sneakCaseFieldType == "tag_string") then
                    add("luacs_string_field(state, " .. typename .. ", " .. field.name .. ", 0); \n")
                elseif(sneakCaseFieldType == "tag_four_c_c") then
                    add("luacs_enum_field(state, " .. typename .. ", EngineTagClass, " .. field.name .. ", 0); \n");
                elseif(sneakCaseFieldType == "matrix") then
                    add("luacs_nested_field(state, " .. typename .. ", EngineMatrix, " .. field.name .. ", 0); \n");
                elseif(sneakCaseFieldType == "tag_handle") then
                    add("luacs_nested_field(state, " .. typename .. ", EngineResourceHandle, " .. field.name .. ", 0); \n");
                elseif(sneakCaseFieldType == "scenario_script_node_value") then
                    add("luacs_unsigned_field(state, " .. typename .. ", " .. field.name .. ", 0); \n");
                elseif(sneakCaseFieldType == "tag_block") then
                    local sneakCaseStructName = definitionParser.camelCaseToSnakeCase(field.struct)
                    local camelCaseStructName = definitionParser.snakeCaseToCamelCase(sneakCaseStructName)
                    add("define_tag_block(state, Engine" .. camelCaseName .. ", " .. camelCaseStructName .. "); \n");
                    indent(2)
                    local tagBlockTypename = "Engine" .. camelCaseName .. "TagBlock" .. camelCaseStructName;
                    add("luacs_nested_field(state, " .. typename .. ", " .. tagBlockTypename .. ", " .. field.name .. ", 0); \n");
                else
                    add("luacs_nested_field(state, " .. typename .. ", Engine" .. camelCaseFieldType .. ", " .. field.name .. ", 0); \n");
                end
            elseif(definitionParser.commonEnums[sneakCaseFieldType] ~= nil or enums[sneakCaseFieldType]) then
                add("luacs_enum_field(state, " .. typename .. ", Engine" .. camelCaseFieldType .. ", " .. field.name .. ", 0); \n");
            elseif(definitionParser.commonBitfields[sneakCaseFieldType] ~= nil or bitfields[sneakCaseFieldType]) then
                add("luacs_nested_field(state, " .. typename .. ", Engine" .. camelCaseFieldType .. ", " .. field.name .. ", 0); \n");
            else
                if(field.type == "char") then
                    add("luacs_bytearray_field(state, " .. typename .. ", " .. field.name .. ", 0); \n");
                elseif(field.type == "byte") then
                    local is_pointer = field.pointer or false
                    if(not is_pointer) then
                        add("luacs_bytearray_field(state, " .. typename .. ", " .. field.name .. ", 0); \n");
                    else 
                        add("luacs_extref_field(state, " .. typename .. ", " .. field.name .. ", LUACS_FREADONLY); \n");
                    end
                elseif(field.type == "float") then
                    if(size == 1) then
                        add("luacs_float_field(state, " .. typename .. ", " .. field.name .. ", 0); \n");
                    else
                        add("luacs_float_array_field(state, " .. typename .. ", " .. field.name .. ", 0); \n");
                    end
                elseif(field.type == "uint32" or field.type == "uint16" or field.type == "uint8") then
                    if(size == 1) then
                        add("luacs_unsigned_field(state, " .. typename .. ", " .. field.name .. ", 0); \n");
                    else
                        add("luacs_unsigned_array_field(state, " .. typename .. ", " .. field.name .. ", 0); \n");
                    end
                elseif(field.type == "int32" or field.type == "int16" or field.type == "int8") then
                    if(size == 1) then
                        add("luacs_int_field(state, " .. typename .. ", " .. field.name .. ", 0); \n");
                    else
                        add("luacs_int_array_field(state, " .. typename .. ", " .. field.name .. ", 0); \n");
                    end
                else
                    error("Unknown type " .. field.type .. " in " .. structName .. " " .. field.name)
                end
            end
        end
    end

    indent(2)
    add("lua_pop(state, 1); \n")
    indent(1)
    add("} \n\n")
end

for bitfieldName, bitfield in pairs(bitfields) do
    local sneakCaseName = definitionParser.camelCaseToSnakeCase(bitfieldName)
    local camelCaseName = definitionParser.snakeCaseToCamelCase(sneakCaseName)
    local typename = "Engine" .. camelCaseName
    local bitfieldType
    if(bitfield.type == "uint32") then
        bitfieldType = "LUACS_TINT32"
    elseif(bitfield.type == "uint16") then
        bitfieldType = "LUACS_TINT16"
    elseif(bitfield.type == "uint8") then
        bitfieldType = "LUACS_TINT8"
    else
        error("Unknown bitfield type " .. bitfield.type)
    end

    indent(1)
    add("inline void create_engine_" .. bitfieldName .. "_struct(lua_State *state) noexcept { \n")
    indent(2)
    add("luacs_newstruct(state, " .. typename .. "); \n")
    indent(2)
    add("lua_pop(state, 1); \n")
    indent(1)
    add("} \n\n")

    indent(1)
    add("static void define_engine_" .. bitfieldName .. "_struct(lua_State *state) noexcept { \n")
    indent(2)
    add("luacs_newstruct(state, " .. typename .. "); \n")
    indent(2)
	add("luacs_declare_field(state, " .. bitfieldType .. ", NULL, \"flags\", sizeof(" .. toStdType(bitfield.type) .. "), 0, 0, 0); \n")

    for _, field in pairs(bitfield.fields) do
        local sneakCaseFieldName = definitionParser.camelCaseToSnakeCase(field)
        local lowerCamelCaseFieldName = definitionParser.snakeCaseToLowerCamelCase(sneakCaseFieldName)
        indent(2)
        add("luacs_declare_method(state, \"" .. lowerCamelCaseFieldName .. "\", lua_bitfield_struct_method(state, Engine" .. camelCaseName .. ", " .. field .. ")); \n")
    end

    indent(2)
    add("lua_pop(state, 1); \n")
    indent(1)
    add("} \n\n");
end

for enumName, enum in pairs(enums) do
    local sneakCaseName = definitionParser.camelCaseToSnakeCase(enumName)
    local camelCaseName = definitionParser.snakeCaseToCamelCase(sneakCaseName)
    local lowerCamelCaseName = definitionParser.snakeCaseToLowerCamelCase(sneakCaseName)

    indent(1)
    add("inline void create_engine_" .. enumName .. "_enum(lua_State *state) noexcept { \n")
    indent(2)
    add("luacs_newenum(state, Engine" .. camelCaseName .. "); \n")
    indent(2)
    add("lua_pop(state, 1); \n")
    indent(1)
    add("} \n\n")

    indent(1)
    add("static void define_engine_" .. enumName .. "_enum(lua_State *state) noexcept { \n")
    indent(2)
    add("luacs_newenum(state, Engine" .. camelCaseName .. "); \n")
    for _, value in ipairs(enum.values) do
        local valueName = value:upper()
        local lowerCamelCase = definitionParser.snakeCaseToLowerCamelCase(value:match(sneakCaseName .. "_(.+)"))
        indent(2)
        add("luacs_enum_declare_value(state, \"" .. lowerCamelCase .. "\", Engine::TagDefinitions::" .. valueName .. "); \n")
    end
    indent(2)
    add("publish_enum(state, \"Engine\", \"tag\", \"" .. lowerCamelCaseName .. "\", -1); \n")
    indent(2)
    add("lua_pop(state, 1); \n")
    indent(1)
    add("} \n\n")
end

for _, class in ipairs(definitionParser.tagClasses) do
    local sneakCaseName = class
    if sneakCaseName == "tag_collection" then
        sneakCaseName = "tag_collection_tag"
    end
    local dataStruct = class
    if dataStruct == "ui_widget_collection" then
        dataStruct = "tag_collection"
    end
    local classCamelCaseName = definitionParser.snakeCaseToCamelCase(sneakCaseName)
    local dataStructName = definitionParser.snakeCaseToCamelCase(dataStruct)

    indent(1)
    add("inline void define_engine_" .. sneakCaseName .. "_tag_struct(lua_State *state) noexcept { \n")
    indent(2)
    add("luacs_newstruct0(state, \"Engine" .. classCamelCaseName .. "Tag\", \"EngineTag\"); \n")
    indent(2)
    add("luacs_declare_field(state, LUACS_TOBJREF, \"Engine" .. dataStructName .. "\", \"data\", sizeof(void *), OFFSET_OF(EngineTag, data), 0, 0); \n")
    indent(2)
    add("lua_pop(state, 1); \n")
    indent(1)
    add("} \n\n")
end

add([[
    void push_meta_engine_tag(lua_State *state, Engine::Tag *tag) noexcept {
        switch(tag->primary_class) {
]])

for _, class in ipairs(definitionParser.tagClasses) do
    local classEnumName = class:upper()
    local sneakCaseName = class
    local classCamelCaseName = definitionParser.snakeCaseToCamelCase(sneakCaseName)
    if classCamelCaseName == "TagCollection" then
        classCamelCaseName = "TagCollectionTag"
    end
    indent(3)
    add("case Engine::TAG_CLASS_" .. classEnumName .. ": \n");
    indent(4)
    add("luacs_newobject0(state, \"Engine" .. classCamelCaseName .. "Tag\", tag); \n");
    indent(4)
    add("break; \n");
end

add([[
            default: {
                lua_pushnil(state);
                break;
            }
        }
    }

]])

indent(1)
add("void define_engine_tag_data_types(lua_State *state) noexcept { \n")

for bitfieldName, _ in pairs(bitfields) do
    indent(2)
    add("create_engine_" .. bitfieldName .. "_struct(state); \n")
end

for enumName, _ in pairs(enums) do
    indent(2)
    add("create_engine_" .. enumName .. "_enum(state); \n")
end

local definedStructs = {}
local undefinedStructFound = true
while undefinedStructFound do
    undefinedStructFound = false
    for structName, struct in pairs(structs) do
        if struct.inherits then
            local parentStructName = definitionParser.camelCaseToSnakeCase(struct.inherits)
            if definedStructs[parentStructName] then
                indent(2)
                add("create_engine_" .. structName .. "_struct(state); \n")
                definedStructs[structName] = true
            else
                undefinedStructFound = true
            end
        else
            if not definedStructs[structName] then
                indent(2)
                add("create_engine_" .. structName .. "_struct(state); \n")
                definedStructs[structName] = true
            end
        end
    end
end

for structName, _ in pairs(structs) do
    indent(2)
    add("define_engine_" .. structName .. "_struct(state); \n")
end

for bitfieldName, _ in pairs(bitfields) do
    indent(2)
    add("define_engine_" .. bitfieldName .. "_struct(state); \n")
end

for enumName, _ in pairs(enums) do
    indent(2)
    add("define_engine_" .. enumName .. "_enum(state); \n")
end

for _, class in ipairs(definitionParser.tagClasses) do
    if class == "tag_collection" then
        class = "tag_collection_tag"
    end
    indent(2)
    add("define_engine_" .. class .. "_tag_struct(state); \n")
end

add([[
    }
}

#undef define_tag_block

]])

-- Write the file
glue.writefile(outputFile, cpp, "t")
