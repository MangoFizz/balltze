# SPDX-License-Identifier: GPL-3.0-only

cmake_minimum_required(VERSION 3.16)

# Find Lua runtime
find_package(LuaRuntime REQUIRED)

# Tag definitions
set(JSON_TAG_DEFINITIONS_PATH "${CMAKE_SOURCE_DIR}/src/ringworld/ringhopper/src/ringhopper-definitions/json/tag")
file(GLOB JSON_TAG_DEFINITION_FILES ${JSON_TAG_DEFINITIONS_PATH}/*.json)

# Output files
set(TAG_LUA_DEFINITIONS_CPP "${CMAKE_SOURCE_DIR}/src/balltze/lua/api/v2/types/engine/tag_data.cpp")
set(TAG_LUA_DEFINITIONS_HPP "${CMAKE_SOURCE_DIR}/src/balltze/lua/api/v2/types/engine/tag_data.hpp")

# Tell CMake how to generate the tag struct headers
set(LUA_ENVIRONMENT_SCRIPT ${CMAKE_SOURCE_DIR}/lua/env.lua)
set(LUA_COMMAND ${CMAKE_COMMAND} -E env LUA_INIT="@${LUA_ENVIRONMENT_SCRIPT}" ${LUA_EXECUTABLE})
set(RINGWORLD_TAG_DEFS_PARSER_SCRIPT ${CMAKE_SOURCE_DIR}/src/ringworld/lua/tagDefinitionParser.lua)
set(TAG_LUA_DEFS_GENERATOR_SCRIPT ${CMAKE_SOURCE_DIR}/lua/code_gen/ringworld_tags_definitions/tagsLuaDefinitions.lua)

add_custom_command(
    OUTPUT ${TAG_LUA_DEFINITIONS_CPP} ${TAG_LUA_DEFINITIONS_HPP}
    COMMAND ${LUA_COMMAND} ${TAG_LUA_DEFS_GENERATOR_SCRIPT} ${TAG_LUA_DEFINITIONS_CPP} ${TAG_LUA_DEFINITIONS_HPP} ${JSON_TAG_DEFINITION_FILES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Generating Lua definitions for ringworld tag definitions..."
    DEPENDS ${RINGWORLD_TAG_DEFS_PARSER_SCRIPT} ${TAG_LUA_DEFS_GENERATOR_SCRIPT} ${JSON_TAG_DEFINITION_FILES}
)
