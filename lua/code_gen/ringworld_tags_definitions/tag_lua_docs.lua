-- SPDX-License-Identifier: GPL-3.0-only

local argparse = require "argparse"
local glue = require "glue"
local json = require "json"
local definitionParser = require "tagDefinitionParser"

local toCamelCase = definitionParser.toCamelCase
local toSnakeCase = definitionParser.toSnakeCase
local toPascalCase = definitionParser.toPascalCase
local parseTagDefinition = definitionParser.parseTagDefinition
local getDependencies = definitionParser.getDependenciesForTagDefinition
local commonTypes = definitionParser.types
local commonEnums = definitionParser.commonEnums
local commonBitfields = definitionParser.commonBitfields
local primitiveTypes = definitionParser.primitiveTypes
local isCommonType = definitionParser.isCommonType

local argsParser = argparse("Balltze tag structures Lua annotations generator", "Convert Invader tag definitions to Lua annotations")
argsParser:argument("output", "Output file"):args(1)
argsParser:argument("files", "Tag definitions files"):args("*")

local args = argsParser:parse()
local files = args.files
local outputPath = args.output

local definitions = {}

for _, file in ipairs(files) do
    local fileName = file:match("([^/]+)$")
    local definitionName = fileName:match("^(.+)%..+$")
    local definition = json.decode(glue.readfile(file))
    local parsedDefinition = parseTagDefinition(definition)
    definitions[definitionName] = parsedDefinition
end

local dependencies = getDependencies(definitions)

local file = ""

local function add(text)
    file = file .. text
end

local function ensureNoLeadingNumber(str)
    if str:match("^%d") then
        return "_" .. str
    end
    return str
end

add([[
-- SPDX-License-Identifier: GPL-3.0-only
-- This file is used to document the Lua plugins engine API. It should not be included.

---@meta _

]])

for definitionName, definition in pairs(definitions) do
    -- generate and write header
    local definitionDependencies = dependencies[definitionName]

    for _, enum in ipairs(definition.enums) do
        local enumName = toPascalCase(enum.name)
        add("---@alias TagData" .. enumName .. " \n")
        for _, variant in ipairs(enum.values) do
            add("---| \""  .. toSnakeCase(variant.name) .. "\"\n")
        end
        add("\n")
    end

    for _, bitfield in ipairs(definition.bitfields) do
        add("---@class TagData" .. toPascalCase(bitfield.name) .. " \n")
        for _, field in ipairs(bitfield.fields) do
            add("---@field " .. ensureNoLeadingNumber(toCamelCase(field.name)) .. " boolean \n")
        end
        add("\n")
    end

    for structType, struct in pairs(definition.structs) do
        add("---@class TagData" .. toPascalCase(struct.name))
        if struct.inherits then
            add(": TagData" .. toPascalCase(struct.inherits) .. " ")
        end
        add(" \n")
        for _, field in ipairs(struct.fields) do
            if field.type and field.name then
                local fieldName = toCamelCase(field.name)
                add("---@field " .. ensureNoLeadingNumber(fieldName) .. " ")
                if field.type == "uint32_t" or field.type == "uint16_t" or field.type == "uint8_t" then
                    add("integer")
                elseif field.type == "int32_t" or field.type == "int16_t" or field.type == "int8_t" then
                    add("integer")
                elseif field.type == "byte" then
                    add("integer")
                elseif field.type == "float" then
                    add("number")
                elseif field.type == "TagBlock" then
                    add("TagData" .. field.struct .. "[]")
                elseif field.type == "ColorARGBInt" then
                    add("integer")
                elseif field.type == "String32" then
                    add("string")
                elseif field.type == "Data" then
                    add("TagRawData")
                else
                    if not isCommonType(field.type) then
                        add("TagData")
                    end

                    local fieldName = field.type

                    add(fieldName)
                end

                local size = field.size or 1
                if size > 1 then
                    add("[" .. size .. "]")
                end

                add(" \n")
            end
        end
        add("\n")
    end

    add("\n")

    -- Write the file
    glue.writefile(outputPath .. "/engine_" .. definitionName .. ".lua", file, "t")

    file = ""
end
