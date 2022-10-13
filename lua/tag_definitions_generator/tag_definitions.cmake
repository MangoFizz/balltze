# SPDX-License-Identifier: GPL-3.0-only

cmake_minimum_required(VERSION 3.16)

# Find Lua runtime
find_package(LuaRuntime REQUIRED)

set(TAG_DEFINITIONS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/ext/invader/tag/hek/definition")
set(TAG_STRUCTURES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include/engine/tag_definitions")
file(GLOB TAG_DEFINITION_FILES ${TAG_DEFINITIONS_PATH}/*.json)
set(TAG_STRUCTURE_OUTPUT_FILES)
set(COMMANDS)

file(MAKE_DIRECTORY ${TAG_STRUCTURES_PATH})

foreach(TAG_DEFINITION_FILE ${TAG_DEFINITION_FILES})
    get_filename_component(TAG_DEFINITION_NAME ${TAG_DEFINITION_FILE} NAME_WE)
    set(TAG_STRUCTURE_FILE "${TAG_STRUCTURES_PATH}/${TAG_DEFINITION_NAME}.hpp")
    set(TAG_STRUCTURE_OUTPUT_FILES ${TAG_STRUCTURE_OUTPUT_FILES} ${TAG_STRUCTURE_FILE})
    add_custom_command(
        OUTPUT ${TAG_STRUCTURE_FILE}
        COMMAND ${CMAKE_COMMAND} -E env LUA_INIT="@${CMAKE_SOURCE_DIR}/lua/env.lua" ${LUA_EXECUTABLE} ${CMAKE_SOURCE_DIR}/lua/tag_definitions_generator/generate_structs.lua ${TAG_DEFINITION_FILE}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
endforeach()

add_custom_target(tag-structs
    DEPENDS ${TAG_STRUCTURE_OUTPUT_FILES}
)
