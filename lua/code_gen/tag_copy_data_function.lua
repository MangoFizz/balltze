-- SPDX-License-Identifier: GPL-3.0-only

local argparse = require "argparse"
local glue = require "glue"
local json = require "json"
local definitionParser = require "parse_tag_definition"

local parser = argparse("Balltze tag copy_data function generator")
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
        if(field.type == "TagBlock" or field.type == "TagDataOffset") then
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
    if(definitionName ~= "enum" and definitionName ~= "bitfield") then
        local parsedDefinition = definitionParser.parseDefinition(definitionName, definition)
        for _, struct in ipairs(parsedDefinition.structs) do
            structs[definitionParser.snakeCaseToCamelCase(struct.name)] = struct
        end
    end
end

add([[
// SPDX-License-Identifier: GPL-3.0-only
// This file is auto-generated. DO NOT EDIT!

#include <optional>
#include <functional>
#include <cstdint>
#include <cstddef>
#include <balltze/legacy_api/engine/tag.hpp>
#include <balltze/legacy_api/engine/tag_definitions.hpp>

namespace Balltze::Features {
    using namespace LegacyApi::Engine;
    using namespace LegacyApi::Engine::TagDefinitions;

    template<typename T>
    static void displace_offset(T &value, std::ptrdiff_t offset) {
        if(value == nullptr) {
            return;
        }
        value = reinterpret_cast<T>(reinterpret_cast<std::int32_t>(value) + offset);
    }
    
]])

for structName, _ in pairs(structs) do
    indent(1)
    add("static inline std::byte *copy_struct_data(" .. definitionParser.snakeCaseToCamelCase(structName) .. " &" .. structName .. "_orig, std::function<std::byte *(std::byte *, std::size_t)> &data_allocator, bool do_not_reallocate); \n");
end

add("\n")

for structName, struct in pairs(structs) do


    indent(1)
    add("static inline std::byte *copy_struct_data(" .. definitionParser.snakeCaseToCamelCase(structName) .. " &" .. structName .. "_orig, std::function<std::byte *(std::byte *, std::size_t)> &data_allocator, bool do_not_reallocate) {\n")
    
    indent(2)
    add("auto &data = !do_not_reallocate ? *reinterpret_cast<" .. structName .. " *>(data_allocator(reinterpret_cast<std::byte *>(&" .. structName .. "_orig), sizeof(" .. structName .."))) : " .. structName .. "_orig;\n")

    if(struct.inherits and structs[definitionParser.snakeCaseToCamelCase(struct.inherits)]) then
        indent(2)
        add("copy_struct_data(static_cast<" .. definitionParser.snakeCaseToCamelCase(struct.inherits) .. " &>(data), data_allocator, true);\n")
    end
    
    for _, field in ipairs(struct.fields) do
        local fieldAccess = "data." .. (field.name or "")
        if(field.type == "TagBlock") then
            indent(2)
            add("if(" .. fieldAccess .. ".count > 0) {\n")
            indent(3)
            add("auto *new_offset = reinterpret_cast<decltype(" .. fieldAccess .. ".elements)>(data_allocator(reinterpret_cast<std::byte *>(" .. fieldAccess .. ".elements), sizeof(" .. fieldAccess .. ".elements[0]) * " .. fieldAccess .. ".count));\n")
            indent(3)
            add(fieldAccess .. ".elements = new_offset;\n")
            if(structs[definitionParser.snakeCaseToCamelCase(field.struct)]) then
                indent(3)
                add("for(std::size_t i = 0; i < " .. fieldAccess .. ".count; i++) {\n")
                indent(4)
                add("copy_struct_data(" .. fieldAccess .. ".elements[i], data_allocator, true);\n")
                indent(3)
                add("}\n")
            end
            indent(2)
            add("}\n")
        elseif(field.type == "TagDataOffset") then
            indent(2)
            add("if(" .. fieldAccess .. ".pointer) { \n")
            indent(3)
            add(fieldAccess .. ".pointer = data_allocator(" .. fieldAccess .. ".pointer, " .. fieldAccess .. ".size);\n")
            indent(2)
            add("}\n")
        elseif(field.type == "TagDependency") then
            -- Prevents the thing from crashing when the path is bullshit
            -- indent(2)
            -- add("if(" .. fieldAccess .. ".path && " .. fieldAccess .. ".path_size > 0) {\n")
            -- indent(3)
            --add(fieldAccess .. ".path = reinterpret_cast<char *>(data_allocator(reinterpret_cast<std::byte *>(" .. fieldAccess .. ".path), " .. fieldAccess .. ".path_size + 1));\n")
            -- indent(2)
            -- add("}\n")
        elseif(structs[definitionParser.snakeCaseToCamelCase(field.type)]) then
            indent(2)
            add("copy_struct_data(data, data_allocator, false);\n")
        end
    end
    indent(2)
    add("return reinterpret_cast<std::byte *>(&data);\n")
    indent(1)
    add("}\n\n")
end

add([[
    std::byte *copy_tag_data(Tag *tag, std::function<std::byte *(std::byte *, std::size_t)> data_allocator) {
        switch(tag->primary_class) {
]])

for _, file in ipairs(files) do
    local fileName = file:match("([^/]+)$")
    local definitionName = fileName:match("^(.+)%..+$")
    if(definitionName ~= "enum" and definitionName ~= "bitfield" and definitionName ~= "hud_interface_types") then
        if(definitionName == "tag_collection") then
            indent(3)
            add("case TAG_CLASS_UI_WIDGET_COLLECTION: \n")
        end
        indent(3)
        add("case TAG_CLASS_" .. definitionName:upper() .. ": { \n")
        indent(4)
        add("auto &tag_data = *reinterpret_cast<" .. definitionParser.snakeCaseToCamelCase(definitionName) .. " *>(tag->data); \n")
        if(structs[definitionParser.snakeCaseToCamelCase(definitionName)]) then
            indent(4)
            add("return copy_struct_data(tag_data, data_allocator, false); \n")
        end
        indent(3)
        add("} \n")
    end
end

add([[
            default:
                return nullptr;
        }
    }
}
]])

-- Write the file
glue.writefile(outputFile, cpp, "t")
