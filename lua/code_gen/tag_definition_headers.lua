-- SPDX-License-Identifier: GPL-3.0-only

local argparse = require "argparse"
local glue = require "glue"
local json = require "json"
local parser = require "parse_tag_definition"

local argsParser = argparse("Balltze tag structure generator", "Convert Invader tag definitions to C structs")
argsParser:argument("output_directory", "Headers output directory"):args(1)
argsParser:argument("files", "Tag definitions files"):args("*")

local args = argsParser:parse()
local files = args.files
local outputDirectory = args.output_directory

---Generate a header file for a tag definition
---@param definitionName string
---@param tagDefinition TagDefinition
---@param dependencies table<string, table<string, boolean>>
local function generateHeader(definitionName, tagDefinition, dependencies)
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

    local function addType(typeName)
        add("T<" .. typeName .. ">")
    end

    add([[
// SPDX-License-Identifier: GPL-3.0-only
// This file is auto-generated. DO NOT EDIT!

#ifndef BALLTZE_API__ENGINE__TAG_DEFINITION__]] .. definitionName:upper() .. [[_HPP
#define BALLTZE_API__ENGINE__TAG_DEFINITION__]] .. definitionName:upper() .. [[_HPP

#include "../../../memory.hpp"
#include "../tag.hpp"
#include "../script.hpp"
#include "enum.hpp"
#include "bitfield.hpp"

#pragma pack(push)
#pragma pack(1)

]])

    if(dependencies) then
        for dependency, _ in pairs(dependencies) do
            if dependency ~= "common" and dependency ~= definitionName then
                add("#include \"" .. dependency .. ".hpp\"\n")
            end
        end
    end

    add([[

namespace Balltze::LegacyApi::Engine::TagDefinitions { 
]])
    
    for _, enum in ipairs(tagDefinition.enums) do
        ident(1)
        add("enum " .. parser.snakeCaseToCamelCase(enum.name) .. " : std::uint16_t {\n")
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
        add("struct " .. parser.snakeCaseToCamelCase(bitfield.name) .. " {\n")
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
        add("struct " .. parser.snakeCaseToCamelCase(struct.name) .. " ")
        if struct.inherits then
            add(": public " .. parser.snakeCaseToCamelCase(struct.inherits) .. " ")
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
                elseif field.type == "TagBlock" then
                    add(parser.snakeCaseToCamelCase(field.type) .. "<" .. parser.snakeCaseToCamelCase(field.struct) .. ">")
                elseif field.type == "TagFourCC" then
                    add("TagClassInt")
                else
                    add(parser.snakeCaseToCamelCase(field.type))
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
        add("static_assert(sizeof(" .. parser.snakeCaseToCamelCase(struct.name) .. ") == " .. struct.width .. ");\n\n")
    end        
    add([[
}

#pragma pack(pop)

#endif

]])
    return struct
end

---Generate a header file for a tag definition using swapped endianness 
---@param definitionName string
---@param tagDefinition TagDefinition
---@param dependencies table<string, table<string, boolean>>
local function generateBigEndianHeader(definitionName, tagDefinition, dependencies)
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

    local function isStruct(typeName)
        for _, struct in pairs(tagDefinition.structs) do
            if struct.name == typeName then
                return true
            end
        end
        if dependencies then
            for dependencyName, dependency in pairs(dependencies) do
                for dependencyTypeName, dependencyType in pairs(dependency) do
                    if dependencyTypeName == typeName then
                        return dependencyType == "struct"
                    end
                end
            end
        end
        return false
    end

    add([[
// SPDX-License-Identifier: GPL-3.0-only
// This file is auto-generated. DO NOT EDIT!

#ifndef BALLTZE_API__HEK__TAG_DEFINITION__]] .. definitionName:upper() .. [[_HPP
#define BALLTZE_API__HEK__TAG_DEFINITION__]] .. definitionName:upper() .. [[_HPP

#include "../../memory.hpp"
#include "../tag_file.hpp"
#include "enum.hpp"
#include "bitfield.hpp"

#pragma pack(push)
#pragma pack(1)

]])

    if(dependencies) then
        for dependency, _ in pairs(dependencies) do
            if dependency ~= "common" and dependency ~= definitionName then
                add("#include \"" .. dependency .. ".hpp\"\n")
            end
        end
    end

    add([[

namespace Balltze::HEK::TagDefinitions { 
]])
    
    for _, enum in ipairs(tagDefinition.enums) do
        ident(1)
        add("enum " .. parser.snakeCaseToCamelCase(enum.name) .. " : std::uint16_t {\n")
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
        add("struct " .. parser.snakeCaseToCamelCase(bitfield.name) .. " {\n")
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
        add("struct " .. parser.snakeCaseToCamelCase(struct.name) .. " ")
        if struct.inherits then
            add(": public " .. parser.snakeCaseToCamelCase(struct.inherits) .. " ")
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
                    add("Memory::BigEndian<" .. toStdType(field.type) .. ">")
                elseif field.type == "int32" or field.type == "int16" or field.type == "int8" then
                    add("Memory::BigEndian<" .. toStdType(field.type) .. ">")
                elseif field.type == "byte" then
                    add("std::byte")
                elseif field.type == "float" then
                    add("Memory::BigEndian<float>")
                elseif field.type == "TagBlock" or field.type == "tag_block" then
                    add(parser.snakeCaseToCamelCase(field.type) .. "<" .. parser.snakeCaseToCamelCase(field.struct) .. ">")
                elseif field.type == "TagFourCC" then
                    add("TagClassInt")
                else
                    if(isStruct(field.type)) then
                        add(parser.snakeCaseToCamelCase(field.type))
                    else
                        add("Memory::BigEndian<" .. parser.snakeCaseToCamelCase(field.type) .. ">")
                    end
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
        add("static_assert(sizeof(" .. parser.snakeCaseToCamelCase(struct.name) .. ") == " .. struct.width .. ");\n\n")
    end        
    add([[
}

#pragma pack(pop)

#endif

]])
    return struct
end

local function generateHeaderCollection(files) 
    local header = [[
// SPDX-License-Identifier: GPL-3.0-only
// This file is automatically generated by generate_header_collection.lua. Do not edit!

]]
    for _, file in ipairs(files) do
        local fileName = file:match("([^/]+)$")
        header = header .. "#include \"tag_definitions/" .. fileName .. "\"\n"
    end

    if(args.guard) then
        header = header .. [[ 
#endif 
]]
    end

    return header
end

local definitions = {}

for _, file in ipairs(files) do
    local fileName = file:match("([^/]+)$")
    local definitionName = fileName:match("^(.+)%..+$")
    local definition = json.decode(glue.readfile(file))
    local parsedDefinition = parser.parseDefinition(definitionName, definition)
    definitions[definitionName] = parsedDefinition
end

local dependencies = parser.getDependencies(definitions)

for definitionName, definition in pairs(definitions) do
    -- generate and write header
    local definitionDependencies = dependencies[definitionName]

    -- Generate little endian header
    local header = generateHeader(definitionName, definition, definitionDependencies)
    glue.writefile(outputDirectory .. "/legacy_api/engine/tag_definitions/" .. definitionName .. ".hpp", header, "t")

    -- Generate big endian header
    local header = generateBigEndianHeader(definitionName, definition, definitionDependencies)
    glue.writefile(outputDirectory .. "/legacy_api/hek/tag_definitions/" .. definitionName .. ".hpp", header, "t")
end

-- Generate header collection
local headers_list = {}
for _, file in ipairs(files) do
    local fileName = file:match("([^/]+)$")
    local definitionName = fileName:match("^(.+)%..+$")
    table.insert(headers_list, definitionName .. ".hpp")
end
local collection = generateHeaderCollection(headers_list)
glue.writefile(outputDirectory .. "/legacy_api/engine/tag_definitions.hpp", collection, "t")
glue.writefile(outputDirectory .. "/legacy_api/hek/tag_definitions.hpp", collection, "t")
