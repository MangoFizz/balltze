-- SPDX-License-Identifier: GPL-3.0-only

local argparse = require "argparse"
local glue = require "glue"
local json = require "json"
local parser = require "parse_tag_definition"

local argsParser = argparse("Balltze tag structures Lua annotations generator", "Convert Invader tag definitions to Lua annotations")
argsParser:argument("output", "Output file"):args(1)
argsParser:argument("files", "Tag definitions files"):args("*")

local args = argsParser:parse()
local files = args.files
local outputFile = args.output

local definitions = {}

for _, file in ipairs(files) do
    local fileName = file:match("([^/]+)$")
    local definitionName = fileName:match("^(.+)%..+$")
    local definition = json.decode(glue.readfile(file))
    local parsedDefinition = parser.parseDefinition(definitionName, definition)
    definitions[definitionName] = parsedDefinition
end

local dependencies = parser.getDependencies(definitions)

local file = ""

local function add(text)
    file = file .. text
end

add([[
-- SPDX-License-Identifier: GPL-3.0-only
-- This file is used to document the Lua plugins engine API. It should not be included.

---@meta _

]])

for definitionName, definition in pairs(definitions) do
    -- generate and write header
    local definitionDependencies = dependencies[definitionName]

    local function isMeta(value)
        for _, enum in ipairs(definition.enums) do
            if parser.snakeCaseToCamelCase(enum.name) == value then
                return false
            end
        end
        for enumName, _ in pairs(parser.commonEnums) do
            if parser.snakeCaseToCamelCase(enumName) == value then
                return false
            end
        end
        for _, fields in pairs(definitionDependencies) do
            for dataType, fieldType in pairs(fields) do
                if fieldType == "enum" and parser.snakeCaseToCamelCase(dataType) == value then
                    return false
                end
            end
        end
        if value == "TagFourCC" or value == "TagHandle" or value == "ScenarioScriptNodeValue" then
            return false
        end
        return true
    end

    local function isCommon(value) 
        for structName, _ in pairs(parser.commonStructs) do
            if parser.snakeCaseToCamelCase(structName) == value then
                return true
            end
        end
    end

    for _, enum in ipairs(definition.enums) do
        local camelCase = parser.snakeCaseToCamelCase(enum.name)
        add("---@alias EngineTagData" .. camelCase .. " \n")
        for _, value in ipairs(enum.values) do
            add("---| '" .. value .. "'\n")
        end
        add("\n")
    end

    for _, bitfield in ipairs(definition.bitfields) do
        add("---@class MetaEngineTagData" .. parser.snakeCaseToCamelCase(bitfield.name) .. " \n")
        for _, field in ipairs(bitfield.fields) do
            local camelCaseName = parser.snakeCaseToCamelCase(field)
            local lowerCamelCaseName = camelCaseName:sub(1, 1):lower() .. camelCaseName:sub(2)
            add("---@field " .. lowerCamelCaseName .. " boolean \n")
        end
        add("\n")
    end

    for _, struct in pairs(definition.structs) do
        add("---@class MetaEngineTagData" .. parser.snakeCaseToCamelCase(struct.name))
        if struct.inherits then
            add(": MetaEngineTagData" .. parser.snakeCaseToCamelCase(struct.inherits) .. " ")
        end
        add(" \n")
        for _, field in ipairs(struct.fields) do
            if field.type and field.name then
                local camelCaseName = parser.snakeCaseToCamelCase(field.name)
                local lowerCamelCaseName = camelCaseName:sub(1, 1):lower() .. camelCaseName:sub(2)
                add("---@field " .. lowerCamelCaseName .. " ")
                if field.type == "uint32" or field.type == "uint16" or field.type == "uint8" then
                    add("integer")
                elseif field.type == "int32" or field.type == "int16" or field.type == "int8" then
                    add("integer")
                elseif field.type == "byte" then
                    add("integer")
                elseif field.type == "float" then
                    add("number")
                elseif field.type == "TagReflexive" then
                    add("table<MetaEngineTagData" .. parser.snakeCaseToCamelCase(field.struct) .. ">")
                else
                    if isMeta(field.type) then
                        add("Meta")
                    end

                    add("Engine")

                    if not isCommon(field.type) then
                        add("TagData")
                    end

                    local fieldName = field.type

                    if fieldName == "TagFourCC" then
                        fieldName = "TagClass"
                    end

                    add(parser.snakeCaseToCamelCase(fieldName))
                end
                add(" \n")
            end
        end
        add("\n")
    end
end

add("\n")

-- Write the file
glue.writefile(outputFile, file, "t")