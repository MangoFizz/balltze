-- SPDX-License-Identifier: GPL-3.0-only

local argparse = require "argparse"
local glue = require "glue"
local json = require "json"
local definitionParser = require "parse_tag_definition"

local parser = argparse("Balltze tag data luacstruct definitions generator")
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
#include <luacstruct/luacstruct.h>
#include <balltze/engine/tag.hpp>
#include <balltze/engine/tag_definitions.hpp>
#include "engine_types.hpp"
#include "engine_tag.hpp"

namespace Balltze::Plugins::Lua {
]])

for structName, _ in pairs(structs) do
    local camelCaseName = definitionParser.snakeCaseToCamelCase(structName)
    indent(1)
    add("using Engine" .. camelCaseName .. " = Engine::" .. camelCaseName .. "; \n")
    indent(1)
    add("void lua_define_engine_" .. structName .. "_struct(lua_State *state) noexcept; \n")
    indent(1)
    add("void lua_push_engine_" .. structName .. "(lua_State *state, " .. camelCaseName .. " *data) noexcept; \n")
end
add("\n")

for bitfieldName, _ in pairs(bitfields) do
    local camelCaseName = definitionParser.snakeCaseToCamelCase(bitfieldName)
    indent(1)
    add("using Engine" .. camelCaseName .. " = Engine::" .. camelCaseName .. "; \n")
    indent(1)
    add("void lua_define_engine_" .. bitfieldName .. "_struct(lua_State *state) noexcept; \n")
    indent(1)
    add("void lua_push_engine_" .. bitfieldName .. "(lua_State *state, " .. camelCaseName .. " *data) noexcept; \n")
end
add("\n")

for enumName, _ in pairs(enums) do
    local camelCaseName = definitionParser.snakeCaseToCamelCase(enumName)
    indent(1)
    add("using Engine" .. camelCaseName .. "TagData = Engine::" .. camelCaseName .. "; \n");
    indent(1)
    add("void lua_define_engine_" .. enumName .. "_enum(lua_State *state) noexcept; \n");
end
add("\n")

for structName, struct in pairs(structs) do
    local sneakCaseName = definitionParser.camelCaseToSnakeCase(structName)
    local camelCaseName = definitionParser.snakeCaseToCamelCase(sneakCaseName)
    local typename = "Engine" .. camelCaseName
    indent(1)
    add("void lua_define_engine_" .. structName .. "_struct(lua_State *state) noexcept { \n")
    indent(2)
    add("luacs_newstruct(state, " .. typename .. "); \n")
    for _, field in ipairs(struct.fields) do
        local sneakCaseFieldType = definitionParser.camelCaseToSnakeCase(field.type)
        local camelCaseFieldType = definitionParser.snakeCaseToCamelCase(sneakCaseFieldType)
        local lowerCamelCaseFieldName = definitionParser.snakeCaseToLowerCamelCase(field.name)

        if(field.name) then
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
                    add("luacs_nested_field(state, " .. typename .. ", EngineTagString, " .. field.name .. ", 0); \n");
                elseif(sneakCaseFieldType == "tag_four_c_c") then
                    add("luacs_enum_field(state, " .. typename .. ", EngineTagClass, " .. field.name .. ", 0); \n");
                elseif(sneakCaseFieldType == "matrix") then
                    add("luacs_nested_field(state, " .. typename .. ", EngineMatrix, " .. field.name .. ", 0); \n");
                elseif(sneakCaseFieldType == "tag_handle") then
                    add("luacs_nested_field(state, " .. typename .. ", EngineResourceHandle, " .. field.name .. ", 0); \n");
                elseif(sneakCaseFieldType == "scenario_script_node_value") then
                    add("lua_pushinteger(state, data->" .. field.name .. ".tag_handle.handle); \n");
                elseif(sneakCaseFieldType == "tag_block") then
                    local sneakCaseStructName = definitionParser.camelCaseToSnakeCase(field.struct)
                    local camelCaseStructName = definitionParser.snakeCaseToCamelCase(sneakCaseStructName)
                    add("lua_CFunction index = LUA_ENGINE_TAG_REFLEXIVE__INDEX_LAMBDA(" .. camelCaseStructName .. ", lua_push_meta_engine_" .. sneakCaseStructName .. "); \n");
                    indent(3)
                    add("lua_CFunction len = LUA_ENGINE_TAG_REFLEXIVE__LEN_LAMBDA(" .. camelCaseStructName .. "); \n");
                    indent(3)
                    add("lua_push_meta_object(state, data->" .. field.name .. ", index, lua_engine_tag_block__newindex, len); \n");
                else
                    add("lua_push_meta_engine_" .. sneakCaseFieldType .. "(state, data->" .. field.name .. "); \n");
                end
            end
        end
    end
end

for bitfieldName, bitfield in pairs(bitfields) do
    local sneakCaseName = definitionParser.camelCaseToSnakeCase(bitfieldName)
    local camelCaseName = definitionParser.snakeCaseToCamelCase(sneakCaseName)
    
end

for enumName, enum in pairs(enums) do
    local sneakCaseName = definitionParser.camelCaseToSnakeCase(enumName)
    local camelCaseName = definitionParser.snakeCaseToCamelCase(sneakCaseName)

end

add([[
}
]])

-- Write the file
glue.writefile(outputFile, cpp, "t")
