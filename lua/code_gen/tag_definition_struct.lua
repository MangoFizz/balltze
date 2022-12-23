-- SPDX-License-Identifier: GPL-3.0-only

local argparse = require "argparse"
local glue = require "glue"
local json = require "json"
local definitionParser = require "parse_tag_definition"

local parser = argparse("Balltze tag structure generator", "Convert Invader tag definitions to C structs")
parser:argument("input", "Input file"):args(1)
parser:argument("output", "Output file"):args(1)

local args = parser:parse()

local fileName = args.input:match("([^/]+)$")
local invaderDefinition = json.decode(glue.readfile(args.input))
local definitionName = fileName:match("^(.+)%..+$")
local outputFile = args.output

---Parse a tag definition
---@param tagDefinition TagDefinition
local function generateCHeader(tagDefinition)
    local struct = ""

    local function add(text)
        struct = struct .. text
    end

    local function ident(n)
        add(string.rep("\t", n))
    end

    local function toStdType(typeName)
        return "std::" .. typeName .. "_t"
    end

    add([[
// SPDX-License-Identifier: GPL-3.0-only
// This file is auto-generated. DO NOT EDIT!

#ifndef BALLTZE__ENGINE__TAG_DEFINITION__]] .. definitionName:upper() .. [[_HPP
#define BALLTZE__ENGINE__TAG_DEFINITION__]] .. definitionName:upper() .. [[_HPP

#include "../../memory.hpp"
#include "../data_types.hpp"
#include "enum.hpp"
#include "bitfield.hpp"
]])

    if(definitionParser.dependencies[definitionName]) then
        for _, dependency in ipairs(definitionParser.dependencies[definitionName]) do
            add("#include \"" .. dependency.name .. ".hpp\"\n")
        end
    end

    add([[

namespace Balltze::Engine::TagDefinitions { 
]])
    
    for _, enum in ipairs(tagDefinition.enums) do
        ident(1)
        add("enum class " .. definitionParser.snakeCaseToCamelCase(enum.name) .. " : std::uint16_t {\n")
        for index, value in ipairs(enum.values) do
            ident(2)
            local valueName = value:upper()
            if(index == 1) then
                add(valueName .. " = 0,\n")
            else
                add(valueName .. ",\n")
            end
        end
        ident(1)
        add("};\n\n")
    end

    for _, bitfield in ipairs(tagDefinition.bitfields) do
        local bitfieldType = toStdType(bitfield.type)
        ident(1)
        add("struct " .. definitionParser.snakeCaseToCamelCase(bitfield.name) .. " {\n")
        for _, field in ipairs(bitfield.fields) do
            ident(2)
            add(bitfieldType .. " " .. field .. " : 1;\n")
        end
        ident(1)
        add("};\n")
        ident(1)
        add("static_assert(sizeof(" .. bitfield.name .. ") == sizeof(" .. bitfieldType .. "));\n\n")
    end
    
    for _, struct in pairs(tagDefinition.structs) do
        ident(1)
        add("struct " .. definitionParser.snakeCaseToCamelCase(struct.name) .. " ")
        if struct.inherits then
            add(": public " .. definitionParser.snakeCaseToCamelCase(struct.inherits) .. " ")
        end
        add("{\n")
        for _, field in ipairs(struct.fields) do
            ident(2)
            if not field.type then
                add("std::byte " .. field.name .. "[" .. field.size .. "];\n")
            elseif field.type == "pad" then
                add("PADDING(" .. field.size .. ");\n")
            else
                -- Field type
                if field.type == "uint32" or field.type == "uint16" or field.type == "uint8" then
                    add(toStdType(field.type))
                elseif field.type == "int32" or field.type == "int16" or field.type == "int8" then
                    add(toStdType(field.type))
                elseif field.type == "byte" then
                    add("std::byte")
                elseif field.type == "float" then
                    add(field.type)
                elseif field.type == "TagReflexive" then
                    add(definitionParser.snakeCaseToCamelCase(field.type) .. "<" .. definitionParser.snakeCaseToCamelCase(field.struct) .. ">")
                else
                    add(definitionParser.snakeCaseToCamelCase(field.type))
                end
                
                -- Field name
                add(" ")
                if field.pointer then
                    add("*")
                end
                add(field.name)
                if field.size then
                    add("[" .. field.size .. "]")
                end
                
                add(";\n")
            end
        end
        ident(1)
        add("};\n")
        ident(1)
        add("static_assert(sizeof(" .. definitionParser.snakeCaseToCamelCase(struct.name) .. ") == " .. struct.width .. ");\n\n")
    end        
    add([[
}

#endif

]])
    return struct
end

-- Generate the C++ header
local hpp = generateCHeader(definitionParser.parseDefinition(definitionName, invaderDefinition))

-- Write the file
glue.writefile(outputFile, hpp, "t")
