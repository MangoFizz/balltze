-- SPDX-License-Identifier: GPL-3.0-only

local argparse = require "argparse"
local glue = require "glue"
local json = require "json"
local definitionParser = require "parse_tag_definition"

local parser = argparse("Balltze tag functions generator")
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

local function hasOffsetFields(tagDefinition)
    for _, field in ipairs(tagDefinition.fields) do
        if(field.type == "TagReflexive" or field.type == "TagDataOffset") then
            return true
        end
    end
    return false
end

local structs = {}
for _, file in ipairs(files) do
    local fileName = file:match("([^/]+)$")
    local definition = json.decode(glue.readfile(file))
    local definitionName = fileName:match("^(.+)%..+$")
    if(definitionName ~= "enum" and definitionName ~= "bitfield" and definitionName ~= "hud_interface_types") then
        local parsedDefinition = definitionParser.parseDefinition(definitionName, definition)
        for _, struct in ipairs(parsedDefinition.structs) do
            if(struct.inherits or hasOffsetFields(struct)) then
                structs[definitionParser.snakeCaseToCamelCase(struct.name)] = struct
            end
        end
    end
end

add([[
// SPDX-License-Identifier: GPL-3.0-only
// This file is auto-generated. DO NOT EDIT!

#include <balltze/engine/tag.hpp>

namespace Balltze::Engine {
    using namespace TagDefinitions;

    template <typename T>
    static void displace_offset(T value, std::int32_t offset) {
        value = reinterpret_cast<T>(reinterpret_cast<std::byte *>(value) + offset);
    }
    
]])

for structName, _ in pairs(structs) do
    indent(1)
    add("static void displace_offsets(" .. definitionParser.snakeCaseToCamelCase(structName) .. " &" .. structName .. ", std::int32_t disp); \n");
end

add("\n")

for structName, struct in pairs(structs) do
    indent(1)
    add("static void displace_offsets(" .. definitionParser.snakeCaseToCamelCase(structName) .. " &" .. structName .. ", std::int32_t disp) {\n")
    
    if(struct.inherits and structs[definitionParser.snakeCaseToCamelCase(struct.inherits)]) then
        indent(2)
        add("displace_offsets(static_cast<" .. definitionParser.snakeCaseToCamelCase(struct.inherits) .. " &>(" .. structName .. "), disp);\n")
    end
    
    for _, field in ipairs(struct.fields) do
        local fieldAccess = structName .. "." .. (field.name or "")
        if(field.type == "TagReflexive") then
            indent(2)
            add("if(" .. fieldAccess .. ".count > 0) {\n")
            indent(3)
            add("displace_offset(" .. fieldAccess .. ".offset, disp);\n")
            if(structs[definitionParser.snakeCaseToCamelCase(field.struct)]) then
                indent(3)
                add("for(std::size_t i = 0; i < " .. fieldAccess .. ".count; i++) {\n")
                indent(4)
                add("displace_offsets(" .. fieldAccess .. ".offset[i], disp);\n")
                indent(3)
                add("}\n")
            end
            indent(2)
            add("}\n")
        elseif(field.type == "TagDataOffset") then
            indent(2)
            add("if(!" .. fieldAccess .. ".external) { \n")
            indent(3)
            add("displace_offset(" .. fieldAccess .. ".pointer, disp);\n")
            indent(2)
            add("}\n")
        elseif(structs[definitionParser.snakeCaseToCamelCase(field.type)]) then
            indent(2)
            add("displace_offsets(" .. fieldAccess .. ", disp);\n")
        end
    end
    indent(1)
    add("}\n\n")
end

add([[
    void Tag::rebase_offsets(std::byte *new_data_address) {
        auto offset_displacement = reinterpret_cast<std::int32_t>(new_data_address - this->data);

        switch(this->primary_class) {
]])

for _, file in ipairs(files) do
    local fileName = file:match("([^/]+)$")
    local definitionName = fileName:match("^(.+)%..+$")
    if(definitionName ~= "enum" and definitionName ~= "bitfield" and definitionName ~= "hud_interface_types") then
        indent(3)
        add("case TAG_CLASS_" .. definitionName:upper() .. ": { \n")
        indent(4)
        add("auto *tag_data = reinterpret_cast<TagDefinitions::" .. definitionParser.snakeCaseToCamelCase(definitionName) .. " *>(this->data); \n")
        if(structs[definitionParser.snakeCaseToCamelCase(definitionName)]) then
            indent(4)
            add("displace_offsets(*tag_data, offset_displacement); \n")
        end
        indent(4)
        add("break; \n")
        indent(3)
        add("} \n")
    end
end

add([[
        }
    }
}
]])

-- Write the file
glue.writefile(outputFile, cpp, "t")
