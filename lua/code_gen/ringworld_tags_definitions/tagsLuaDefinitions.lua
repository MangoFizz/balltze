-- SPDX-License-Identifier: GPL-3.0-only

local argparse = require "argparse"
local glue = require "glue"
local json = require "json"
local definitionParser = require "tagDefinitionParser"

local toCamelCase = definitionParser.toCamelCase
local toSnakeCase = definitionParser.toSnakeCase
local toPascalCase = definitionParser.toPascalCase
local parseTagDefinition = definitionParser.parseTagDefinition
local commonTypes = definitionParser.types
local commonEnums = definitionParser.commonEnums
local commonBitfields = definitionParser.commonBitfields
local primitiveTypes = definitionParser.primitiveTypes

local parser = argparse("Ringworld tag Lua definitions generator")
parser:argument("outputSource", "Output source file"):args(1)
parser:argument("outputHeader", "Output header file"):args(1)
parser:argument("files", "Header files"):args("*")
local args = parser:parse()
local outputFile = args.outputSource
local outputHeader = args.outputHeader
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
    local filePath = file:match("([^/]+)$")
    local fileName = filePath:match("^(.+)%..+$")
    local definition = json.decode(glue.readfile(file))
    local parsedDefinition = parseTagDefinition(definition)
    if(fileName ~= "enum" and fileName ~= "bitfield") then
        for _, struct in ipairs(parsedDefinition.structs) do
            structs[toPascalCase(struct.name)] = struct
        end
    end
    if(fileName ~= "bitfield") then
        for _, enum in ipairs(parsedDefinition.enums) do
            enums[toPascalCase(enum.name)] = enum
        end
    end
    for _, bitfield in ipairs(parsedDefinition.bitfields) do
        bitfields[toPascalCase(bitfield.name)] = bitfield
    end
end

local function typeExists(typeName)
    if primitiveTypes[typeName] ~= nil then
        return true
    end

    for _, typeInfo in pairs(commonTypes) do
        if typeInfo.alias == typeName then
            return true
        end
    end

    if structs[typeName] ~= nil then
        return true
    end
    if commonEnums[typeName] ~= nil or enums[typeName] ~= nil then
        return true
    end
    if commonBitfields[typeName] ~= nil or bitfields[typeName] ~= nil then
        return true
    end
    if primitiveTypes[typeName] ~= nil then
        return true
    end
    return false
end

local isCommonType = function(typeName)
    for _, typeInfo in pairs(commonTypes) do
        if typeInfo.alias == typeName then
            return true
        end
    end
    return false
end

local isPrimitiveType = function(typeName)
    for _, primitiveType in ipairs(primitiveTypes) do
        if primitiveType == typeName then
            return true
        end
    end
    return false
end

local function typeForLuaStruct(typeName)
    if typeName == "float" then
        return "LUAST_FLOAT"
    elseif typeName == "uint32_t" then
        return "LUAST_UINT32"
    elseif typeName == "uint16_t" then
        return "LUAST_UINT16"
    elseif typeName == "uint8_t" then
        return "LUAST_UINT8"
    elseif typeName == "int32_t" then
        return "LUAST_INT32"
    elseif typeName == "int16_t" then
        return "LUAST_INT16"
    elseif typeName == "int8_t" then
        return "LUAST_INT8"
    end
end

add([[
// SPDX-License-Identifier: GPL-3.0-only
// This file is auto-generated. DO NOT EDIT!

#ifndef BALLTZE__LUA__API__V2__TYPES__RINGWORLD_TAG_DATA_HPP
#define BALLTZE__LUA__API__V2__TYPES__RINGWORLD_TAG_DATA_HPP

#include <memory>
#include "types.hpp"
#include "tag.hpp"

]])

for _, group in ipairs(definitionParser.tagGroups) do
    add("#include <impl/tag/definitions/" .. toSnakeCase(group) .. ".h>\n")
end

add([[

namespace Balltze::Lua::Api::V2 {
    void push_tag_data(lua_State *state, TagEntry *tag_entry) noexcept;
    void define_ringworld_tag_data_types(lua_State *state) noexcept;
}


#endif

]])

glue.writefile(outputHeader, cpp, "t")

cpp = ""

add([[
// SPDX-License-Identifier: GPL-3.0-only
// This file is auto-generated. DO NOT EDIT!

#include "../../../../libraries/luastruct.hpp"
#include "../../../../helpers/enum.hpp"
#include "../../../../helpers/bitfield.hpp"
#include "types.hpp"
#include "tag.hpp"
#include "tag_data.hpp"

namespace Balltze::Lua::Api::V2 {
    static size_t tag_block_array_counter(lua_State *L, struct LuastructArray *array) {
        return *(reinterpret_cast<uint32_t *>(array->context.parent + array->context.offset) - 1);
    }

    #define DEFINE_TAG_BLOCK_FIELD(state, type, field, elements_type) { \
        LuastructArrayDesc array_desc; \
        luastruct_new_dynamic_array_desc(state, LUAST_STRUCT, #elements_type, tag_block_array_counter, false, false, &array_desc); \
	    luastruct_new_struct_array_field(state, SNAKE_TO_CAMEL(#field), &array_desc, offsetof(type, field.elements), true, false); \
    }
    
    #define DEFINE_STRING32_FIELD(state, type, field) { \
        LuastructArrayDesc array_desc; \
        luastruct_new_static_array_desc(state, LUAST_CHAR, NULL, LUAS_SIZEOF_ARRAY(type, field.string), false, true, &array_desc); \
        luastruct_new_struct_array_field(state, SNAKE_TO_CAMEL(#field), &array_desc, offsetof(type, field), false, false); \
    }

]])

add("\n")

-- structName = "snake_case"
-- structType = "PascalCase"

for structType, struct in pairs(structs) do
    local structName = toSnakeCase(structType)

    indent(1)
    add("static void define_" .. structName .. "_type(lua_State *state) noexcept { \n")
    
    indent(2)
    if struct.inherits then
        local parentStructType = toPascalCase(struct.inherits)
        add("LUAS_STRUCT_EXTENDS(state, " .. structType .. ", " .. parentStructType .. "); \n")
    else 
        add("LUAS_STRUCT(state, " .. structType .. "); \n")
    end

    for _, field in ipairs(struct.fields) do
        if field.name then
            local fieldType = toSnakeCase(field.type)
            local fieldTypeName = field.type

            -- Check if type exists
            if not typeExists(fieldTypeName) then
                error("Unknown type " .. field.type .. " in " .. structType .. " " .. field.name)
            end

            local size = field.size or 1
            indent(2)
            if structs[fieldTypeName] or definitionParser.commonBitfields[fieldTypeName] ~= nil or bitfields[fieldTypeName] then
                add("LUAS_OBJREF_FIELD(state, " .. structType .. ", " .. field.name .. ", " .. fieldTypeName .. ", 0); \n");
            elseif definitionParser.commonEnums[fieldTypeName] ~= nil or enums[fieldTypeName] then
                add("LUAS_ENUM_FIELD(state, " .. structType .. ", " .. field.name .. ", " .. fieldTypeName .. ", 0); \n");
            else
                if fieldTypeName == "String32" then
                    add("DEFINE_STRING32_FIELD(state, " .. structType .. ", " .. field.name .. "); \n")
                elseif fieldTypeName == "ColorARGBInt" then
                    add("LUAS_PRIMITIVE_FIELD(state, " .. structType .. ", " .. field.name .. ", LUAST_UINT32, 0); \n")
                elseif fieldTypeName == "Data" then
                    add("LUAS_OBJREF_FIELD(state, " .. structType .. ", " .. field.name .. ", TagRawData, 0); \n");
                elseif fieldTypeName == "TagGroup" then
                    add("LUAS_ENUM_FIELD(state, " .. structType .. ", " .. field.name .. ", TagGroup, 0); \n");
                elseif(fieldTypeName == "TagBlock") then
                    local elementsType = toPascalCase(field.struct)
                    add("DEFINE_TAG_BLOCK_FIELD(state, " .. structType .. ", " .. field.name .. ", " .. elementsType .. "); \n");
                elseif fieldTypeName == "void *" then
                    add("LUAS_PRIMITIVE_FIELD(state, " .. structType .. ", " .. field.name .. ", LUAST_UINT32, LUAS_FIELD_READONLY); \n")
                elseif(fieldTypeName == "float") then
                    if(size == 1) then
                        add("LUAS_PRIMITIVE_FIELD(state, " .. structType .. ", " .. field.name .. ", LUAST_FLOAT, 0); \n");
                    else
                        add("LUAS_PRIMITIVE_ARRAY_FIELD(state, " .. structType .. ", " .. field.name .. ", LUAST_FLOAT, 0); \n");
                    end
                elseif(fieldTypeName == "uint32_t" or fieldTypeName == "uint16_t" or fieldTypeName == "uint8_t") then
                    if(size == 1) then
                        add("LUAS_PRIMITIVE_FIELD(state, " .. structType .. ", " .. field.name .. ", " .. typeForLuaStruct(field.type) .. ", 0); \n");
                    else
                        add("LUAS_PRIMITIVE_ARRAY_FIELD(state, " .. structType .. ", " .. field.name .. ", " .. typeForLuaStruct(field.type) .. ", 0); \n");
                    end
                elseif(fieldTypeName == "int32_t" or fieldTypeName == "int16_t" or fieldTypeName == "int8_t") then
                    if(size == 1) then
                        add("LUAS_PRIMITIVE_FIELD(state, " .. structType .. ", " .. field.name .. ", " .. typeForLuaStruct(field.type) .. ", 0); \n");
                    else
                        add("LUAS_PRIMITIVE_ARRAY_FIELD(state, " .. structType .. ", " .. field.name .. ", " .. typeForLuaStruct(field.type) .. ", 0); \n");
                    end
                elseif isCommonType(fieldTypeName) then
                    add("LUAS_OBJREF_FIELD(state, " .. structType .. ", " .. field.name .. ", " .. fieldTypeName .. ", 0); \n");
                else
                    error("Unknown type " .. field.type .. " in " .. structType .. " field " .. field.name)
                end
            end
        end
    end

    indent(2)
    add("lua_pop(state, 1); \n")
    indent(1)
    add("} \n\n")
end

for bitfieldType, bitfield in pairs(bitfields) do
    local bitfieldName = toSnakeCase(bitfieldType)
    indent(1)
    add("static void define_" .. bitfieldName .. "_type(lua_State *state) noexcept { \n")
    indent(2)
    add("LUAS_STRUCT(state, " .. bitfieldType .. "); \n")
    for _, field in pairs(bitfield.fields) do
        indent(2)
        add("LUAS_METHOD_FIELD(state, " .. bitfieldType .. ", \"" .. field.name .. "\", luastruct_bitfield_method(state, " .. bitfieldType .. ", " .. field.name .. ")); \n")
    end
    indent(2)
    add("lua_pop(state, 1); \n")
    indent(1)
    add("} \n\n");
end

for enumType, enum in pairs(enums) do
    local enumName = toSnakeCase(enumType)

    indent(1)
    add("static void define_" .. enumName .. "_type(lua_State *state) noexcept { \n")
    indent(2)
    add("LUAS_ENUM(state, " .. enumType .. "); \n")
    for _, variant in ipairs(enum.values) do
        local variantName = toCamelCase(variant.name)
        local variantValue = (toSnakeCase(enumName) .. "_" .. toSnakeCase(variant.name)):upper()
        indent(2)
        add("LUAS_ENUM_VARIANT(state, ".. enumType .. ", \"" .. variantName .. "\", " .. variantValue .. "); \n")
    end
    indent(2)
    add("lua_pop(state, 1); \n")
    indent(1)
    add("} \n\n")
end

add([[
    void push_tag_data(lua_State *state, TagEntry *tag_entry) noexcept {
        switch(tag_entry->primary_group) {
]])

for _, group in ipairs(definitionParser.tagGroups) do
    local tagGroupEnumVariant = "TAG_GROUP_" .. group:upper()
    local tagType = toPascalCase(group)
    indent(3)
    add("case " .. tagGroupEnumVariant .. ": \n");
    indent(4)
    add("LUAS_PUSH_OBJECT(state, " .. tagType .. ", tag_entry->data, false); \n");
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
add("void define_ringworld_tag_data_types(lua_State *state) noexcept { \n")

local definedStructs = {}

for bitfieldName, _ in pairs(bitfields) do
    indent(2)
    add("define_" .. toSnakeCase(bitfieldName) .. "_type(state); \n")
    definedStructs[bitfieldName] = true
end

for enumName, _ in pairs(enums) do
    indent(2)
    add("define_" .. toSnakeCase(enumName) .. "_type(state); \n")
    definedStructs[enumName] = true
end

for _, typeInfo in pairs(commonTypes) do
    definedStructs[typeInfo.alias] = true
end

for enumType, _ in pairs(definitionParser.commonEnums) do
    definedStructs[enumType] = true
end

for bitfieldType, _ in pairs(definitionParser.commonBitfields) do
    definedStructs[bitfieldType] = true
end

for typeName, _ in pairs(definitionParser.primitiveTypes) do
    definedStructs[typeName] = true
end

local function defineStruct(structType)
    local struct = structs[structType]
    if not struct then
        error("Struct " .. structType .. " not found")
        return
    end

    if definedStructs[structType] then
        return
    end

    if struct.inherits then
        defineStruct(struct.inherits)
    end

    for _, field in ipairs(struct.fields) do
        if field.name and not isPrimitiveType(field.type) then
            if field.type == "TagBlock" then
                if not definedStructs[field.struct] then
                    defineStruct(field.struct)
                end
            else
                if not definedStructs[field.type] then
                    defineStruct(field.type)
                end
            end
        end
    end

    indent(2)
    add("define_" .. toSnakeCase(structType) .. "_type(state); \n")
    definedStructs[structType] = true
end

for structName, struct in pairs(structs) do
    if not definedStructs[structName] then
        defineStruct(structName)
    end
end

add([[
    }
}

#undef DEFINE_TAG_BLOCK_FIELD

]])

-- Write the file
glue.writefile(outputFile, cpp, "t")
