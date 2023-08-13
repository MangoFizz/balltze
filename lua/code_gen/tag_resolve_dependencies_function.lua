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

#include <balltze/engine/tag.hpp>
#include <balltze/engine/tag_definitions.hpp>

namespace Balltze::Engine {
    using namespace TagDefinitions;
    
]])

for structName, _ in pairs(structs) do
    indent(1)
    add("static inline void resolve_dependencies_req(" .. structName .. " &" .. definitionParser.camelCaseToSnakeCase(structName) .. ", std::function<TagHandle(TagHandle)> &dependency_resolver); \n");
end

add("\n")

for structName, struct in pairs(structs) do
    local paramName = definitionParser.camelCaseToSnakeCase(structName)

    indent(1)
    add("static inline void resolve_dependencies_req(" .. structName .. " &" .. paramName .. ", std::function<TagHandle(TagHandle)> &dependency_resolver) {\n")
    
    if(struct.inherits and structs[definitionParser.snakeCaseToCamelCase(struct.inherits)]) then
        indent(2)
        add("resolve_dependencies_req(static_cast<" .. definitionParser.snakeCaseToCamelCase(struct.inherits) .. " &>(" .. paramName .. "), dependency_resolver);\n")
    end
    
    for _, field in ipairs(struct.fields) do
        local fieldAccess = paramName .. "." .. (field.name or "")
        if(field.type == "TagDependency") then
            indent(2)
            add("if(" .. fieldAccess .. ".tag_handle != TagHandle::null()) {\n")
            indent(3)
            add(fieldAccess .. ".tag_handle = dependency_resolver(" .. fieldAccess .. ".tag_handle);\n")
            indent(2)
            add("}\n")
        elseif(field.type == "TagHandle") then
            indent(2)
            add("if(" .. fieldAccess .. " != TagHandle::null()) {\n")
            indent(3)
            add(fieldAccess .. " = dependency_resolver(" .. fieldAccess .. ");\n")
            indent(2)
            add("}\n")
        elseif(field.type == "TagReflexive") then
            indent(2)
            add("for(std::size_t i = 0; i < " .. fieldAccess .. ".count; i++) {\n")
            indent(3)
            add("resolve_dependencies_req(" .. fieldAccess .. ".offset[i], dependency_resolver);\n")
            indent(2)
            add("}\n")
        elseif(structs[field.type]) then
            indent(2)
            add("resolve_dependencies_req(" .. fieldAccess .. ", dependency_resolver);\n")
        end
    end
    indent(1)
    add("}\n\n")
end

add([[
    void Tag::resolve_dependencies(std::function<TagHandle(TagHandle)> dependency_resolver) {
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
        indent(4)
        add("resolve_dependencies_req(*tag_data, dependency_resolver); \n")
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
