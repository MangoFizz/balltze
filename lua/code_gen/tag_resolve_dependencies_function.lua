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

namespace Balltze::Engine {
    using namespace TagDefinitions;
    
]])

for structName, _ in pairs(structs) do
    indent(1)
    add("static void fix_dependencies_req(" .. definitionParser.snakeCaseToCamelCase(structName) .. " &" .. structName .. ", std::function<TagDependency(TagDependency)> dependency_resolver); \n");
end

add("\n")

for structName, struct in pairs(structs) do
    indent(1)
    add("static void fix_dependencies_req(" .. definitionParser.snakeCaseToCamelCase(structName) .. " &" .. structName .. ", std::function<TagDependency(TagDependency)> dependency_resolver) {\n")
    
    if(struct.inherits and structs[definitionParser.snakeCaseToCamelCase(struct.inherits)]) then
        indent(2)
        add("fix_dependencies_req(static_cast<" .. definitionParser.snakeCaseToCamelCase(struct.inherits) .. " &>(" .. structName .. "), dependency_resolver);\n")
    end
    
    for _, field in ipairs(struct.fields) do
        local fieldAccess = structName .. "." .. (field.name or "")
        if(field.type == "TagDependency") then
            indent(2)
            add(fieldAccess .. " = " .. "dependency_resolver(" .. fieldAccess .. ");\n")
        elseif(field.type == "TagReflexive") then
            indent(2)
            add("for(std::size_t i = 0; i < " .. fieldAccess .. ".count; i++) {\n")
            indent(3)
            add("fix_dependencies_req(" .. fieldAccess .. ".offset[i], dependency_resolver);\n")
            indent(2)
            add("}\n")
        elseif(structs[field.type]) then
            indent(2)
            add("fix_dependencies_req(" .. fieldAccess .. ", dependency_resolver);\n")
        end
    end
    indent(1)
    add("}\n\n")
end

add([[
    void Tag::fix_dependencies(std::function<TagDependency(TagDependency)> dependency_resolver) {
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
        add("fix_dependencies_req(*tag_data, dependency_resolver); \n")
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
