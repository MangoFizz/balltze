-- SPDX-License-Identifier: GPL-3.0-only

-- from https://github.com/Sledmine/invader2kaitai/blob/main/src/lua/

local argparse = require "argparse"
local glue = require "glue"
local json = require "json"
local inspect = require "inspect"

local parser = argparse("invader2kaitai", "Convert Invader tags to Kaitai Struct")
parser:argument("input", "Input file"):args(1)
-- parser:flag("-r --runtime", "Generate runtime structure")
parser:flag("-c --code", "Generate language code")
parser:flag("-s --struct", "Dump KSY result to a C struct")

local args = parser:parse()

local defFile = json.decode(glue.readfile(args.input))
local fileName = args.input:match("([^/]+)$")
local defFileName = "tag_" .. fileName
local defName = defFileName:match("^(.+)%..+$")
local defLabel = fileName:match("^(.+)%..+$")

---@class InvaderElement
---@field name string
---@field options string[]
---@field type string | '"struct"' | '"enum"' | '"pad"' | '"bitfield"' | '"Angle"' | '"Fraction"' | '"TagEnum"' | '"TagString"' | '"TagIndex"' | '"unint8"' | '"unint16"' | '"unint32"' | '"int8"' | '"int16"' | '"int32"' | '"float"'
---@field size number
---@field width number
---@field bounds number
---@field fields InvaderElement[]
---@field struct string
---@field description string

---@class Field
---@field name string
---@field type string
---@field is_pointer boolean
---@field size number
---@field description string
---@field struct string

---@class Type
---@field fields Field[]

---@type table<string, Type>
local types = {
    tag_name = {fields = {{name = "index", type = "u2"}, {name = "id", type = "u2"}}},
    tag_reflexive = {
        fields = {
            {name = "count", type = "u4"},
            {name = "offset", type = "u4", is_pointer = true},
            {size = 4}
        }
    },
    point2_d = {fields = {{name = "x", type = "f4"}, {name = "y", type = "f4"}}},
    point2_d_int = {fields = {{name = "x", type = "s2"}, {name = "y", type = "s2"}}},
    rectangle2_d = {
        fields = {
            {name = "top", type = "s2"},
            {name = "left", type = "s2"},
            {name = "bottom", type = "s2"},
            {name = "right", type = "s2"}
        }
    },
    color_a_r_g_b = {
        fields = {
            {name = "alpha", type = "f4"},
            {name = "red", type = "f4"},
            {name = "green", type = "f4"},
            {name = "blue", type = "f4"}
        }
    },
    color_a_r_g_b_int = {
        fields = {
            {name = "blue", type = "u1"},
            {name = "green", type = "u1"},
            {name = "red", type = "u1"},
            {name = "alpha", type = "u1"}
        }
    },
    tag_data_offset = {
        fields = {
            {name = "size", type = "u4"},
            {name = "external", type = "u4"},
            {name = "file_offset", type = "u4"},
            {name = "pointer", type = "u4"},
            {size = 4}
        }
    },
    tag_dependency = {
        fields = {
            {name = "tag_fourcc", type = "u4"},
            {name = "path_pointer", type = "u4"},
            {name = "path_size", type = "u4"},
            {name = "tag_id", type = "u4"}
        }
    },
    euler2_d = {
        fields = {
            {name = "yaw", type = "f4"}, 
            {name = "pitch", type = "f4"}
        }
    },
    vector3_d = {
        fields = {
            {name = "i", type = "f4"},
            {name = "j", type = "f4"},
            {name = "k", type = "f4"}
        }
    },
    color_r_g_b = {
        fields = {
            {name = "red", type = "f4"},
            {name = "green", type = "f4"},
            {name = "blue", type = "f4"}
        }
    },
}

local bitfields = {}

local primitiveTypes = {
    byte = "std::byte",
    u1 = "std::uint8_t",
    u2 = "std::uint16_t",
    u4 = "std::uint32_t",
    u8 = "std::uint64_t",
    s1 = "std::int8_t",
    s2 = "std::int16_t",
    s4 = "std::int32_t",
    s8 = "std::int64_t",
    f4 = "float",
    f8 = "double",
    str = "char",
    -- Careful with this we might need to remove them in order to use a custom type
    b8 = "std::uint8_t",
    b16 = "std::uint16_t",
    b32 = "std::uint32_t"
}

local function camelCaseToSnakeCase(str)
    if str then
        -- convert camelCase to snake_case but keep numbers
        local new = str:gsub("([A-Z])", function(c)
            return "_" .. c:lower()
        end)
        if new:sub(1, 1) == "_" then
            new = new:sub(2)
        end
        return new
    end
end

local function dashAndSentenceToSnakeCase(str)
    -- convert dash-and-sentence to snake_case
    return str:gsub("([A-Z])", function(c)
        return "_" .. c:lower()
    end):gsub("-", "_"):gsub(" ", "_"):gsub("'", "")
end

local function normalToSnakeCase(str)
    if str then
        -- convert normal sentence to snake_case
        return str:gsub(" ", "_"):gsub("'", "")
    end
end

local function snakeCaseToCamelCase(str)
    if str then
        -- convert snake_case to camelCase
        local new = str:gsub("_([a-z])", function(c)
            return c:upper()
        end)
        -- capitalize first letter
        new = new:sub(1, 1):upper() .. new:sub(2)
        return new
    end
end

local function toCStruct(structName, structElements, members)
    local struct = [[
// SPDX-License-Identifier: GPL-3.0-only
// This file is auto-generated. DO NOT EDIT!

#ifndef BALLTZE__ENGINE__TAG_DEFINITION__]] .. structName:upper() .. [[_HPP
#define BALLTZE__ENGINE__TAG_DEFINITION__]] .. structName:upper() .. [[_HPP

#include "../data_types.hpp"

namespace Balltze::Engine::TagDefinitions { 
]]
    local function add(text)
        struct = struct .. text
    end
    local function ident(n)
        add(string.rep("\t", n))
    end
    
    for structIndex, struct in pairs(structElements) do
        ident(1)
        add("struct " .. snakeCaseToCamelCase(struct.name) .. " {\n")
        for sequenceIndex, element in ipairs(struct.fields) do
            ident(2)
            if element.name then
                local type = element.type
                if primitiveTypes[type] then
                    type = primitiveTypes[type]
                elseif type then
                    type = snakeCaseToCamelCase(type)
                end
                if type then
                    if type == "TagReflexive" then
                        add(type .. "<" .. element.struct .. "> " .. element.name .. ";\n")
                    else
                        if element.size then
                            add(type .. " " .. element.name .. "[" .. element.size .. "];\n")
                        else
                            add(type .. " " .. element.name .. ";\n")
                        end
                    end
                else
                    add("std::byte " .. element.name .. "[" .. element.size .. "];\n")
                end
            else
                add("std::byte pad_" .. sequenceIndex  .. "[" .. element.size .. "];\n")
            end
        end
        ident(1)
        add("};\n")
    end        

    add([[
}

#endif

]])
    glue.writefile(TagStructuresOutputPath .. structName .. ".hpp", struct, "t")
end

---@type table<string, table<string>>
local enums = {}

---@type Type[]
local structs = {}

---Convert an Invader element to type definition
---@param element InvaderElement
---@return Type
local function invaderElement2Type(element)
    local fieldName = normalToSnakeCase(camelCaseToSnakeCase(element.name))
    local fieldType = normalToSnakeCase(camelCaseToSnakeCase(element.type))
    if element.type == "pad" then
        return {type = "byte", size = element.size}
    elseif element.type == "enum" then
        enums[fieldName] = {}
        if element.options then
            for index, value in pairs(element.options) do
                local value = dashAndSentenceToSnakeCase(value)
                enums[fieldName][tostring(index - 1)] = value
            end
        end
    elseif element.type == "bitfield" then
        element.width = element.width or 8
        bitfields[fieldName] = "u" .. math.ceil(element.width / 8)
    elseif element.type == "float" or element.type == "Angle" or element.type == "Fraction" then
        if element.bounds then
            return {name = fieldName, type = "f4", size = 2}
        end
        return {name = fieldName, type = "f4"}
    elseif element.type == "int16" then
        return {name = fieldName, type = "s2"}
    elseif element.type == "uint16" then
        return {name = fieldName, type = "u2"}
    elseif element.type == "uint32" then
        return {name = fieldName, type = "u4"}
    elseif element.type == "int32" then
        return {name = fieldName, type = "s4"}
    elseif element.type == "Index" or element.type == "TagEnum" then
        return {name = fieldName, type = "u2"}
    elseif element.type == "TagString" then
        return {name = fieldName, type = "str", size = 32, encoding = "ASCII"}
    elseif element.type == "TagFourCC" then
        return {name = "tag_fourcc", type = "str", size = 4, encoding = "ASCII"}
    elseif element.type == "TagID" then
        return {name = fieldName, type = "u4"}
    elseif element.type == "Pointer" then
        return {name = fieldName, type = "u4"}
    elseif element.type == "struct" then
        local fields = {}
        for _, field in pairs(element.fields) do
            table.insert(fields, invaderElement2Type(field))
        end
        types[fieldName] = {fields = fields}
        structs[#structs+1] = {name = fieldName, fields = fields}
        return fields
    elseif element.type == "TagReflexive" then
        return {name = fieldName, type = "tag_reflexive", struct = element.struct}
    elseif enums[fieldType] then
        return {name = fieldName, type = "u2", doc = element.description, enum = fieldType}
    elseif bitfields[fieldType] then
        return {name = fieldName, type = bitfields[fieldType]}
    elseif types[fieldType] then
        return {name = fieldName, type = fieldType}
    else
        return {name = fieldName, type = "u4"}
    end
end

local function tagBodyToFieldSequence()
    local fields = {}
    for _, element in pairs(defFile) do
        local fieldSequence = invaderElement2Type(element)
        if fieldSequence and _ == #defFile then
            for _, fieldSequenceElement in ipairs(fieldSequence) do
                table.insert(fields, fieldSequenceElement)
            end
        end
    end
    return fields
end

local tag = tagBodyToFieldSequence()
-- print(inspect(tag))
toCStruct(defLabel, structs)
