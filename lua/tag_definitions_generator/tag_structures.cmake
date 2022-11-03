# SPDX-License-Identifier: GPL-3.0-only

cmake_minimum_required(VERSION 3.16)

# Find Lua runtime
find_package(LuaRuntime REQUIRED)

# Our super Lua scripts
set(TAG_STRUCTURES_GENERATOR_SCRIPT ${CMAKE_SOURCE_DIR}/lua/tag_definitions_generator/generate_struct.lua)
set(HEADER_COLLECTION_GENERATOR_SCRIPT ${CMAKE_SOURCE_DIR}/lua/generate_header_collection.lua)
set(LUA_ENVIRONMENT_SCRIPT ${CMAKE_SOURCE_DIR}/lua/env.lua)

# Tag definitions
set(TAG_DEFINITIONS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/ext/invader/tag/hek/definition")
file(GLOB TAG_DEFINITION_FILES ${TAG_DEFINITIONS_PATH}/*.json)

# Output files
set(TAG_STRUCTURES_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include/balltze/engine/tag_definitions")
set(TAG_STRUCTURE_OUTPUT_FILES)

# Create output directory for tag structures
file(MAKE_DIRECTORY ${TAG_STRUCTURES_OUTPUT_PATH})

# Tell CMake how to generate the tag structures
foreach(TAG_DEFINITION_FILE ${TAG_DEFINITION_FILES})
    get_filename_component(TAG_DEFINITION_NAME ${TAG_DEFINITION_FILE} NAME_WE)
    set(TAG_STRUCTURE_FILE "${TAG_STRUCTURES_OUTPUT_PATH}/${TAG_DEFINITION_NAME}.hpp")
    add_custom_command(
        OUTPUT ${TAG_STRUCTURE_FILE}
        COMMAND ${CMAKE_COMMAND} -E env LUA_INIT="@${LUA_ENVIRONMENT_SCRIPT}" ${LUA_EXECUTABLE} ${TAG_STRUCTURES_GENERATOR_SCRIPT} ${TAG_DEFINITION_FILE} ${TAG_STRUCTURE_FILE}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
    set(TAG_STRUCTURE_OUTPUT_FILES ${TAG_STRUCTURE_OUTPUT_FILES} ${TAG_STRUCTURE_FILE})
endforeach()

# Add the tag structures to a header
set(TAG_DEFINITIONS_HEADER "${TAG_STRUCTURES_OUTPUT_PATH}/definitions.hpp")
add_custom_command(
    OUTPUT ${TAG_DEFINITIONS_HEADER}
    COMMAND ${CMAKE_COMMAND} -E env LUA_INIT="@${LUA_ENVIRONMENT_SCRIPT}" ${LUA_EXECUTABLE} ${HEADER_COLLECTION_GENERATOR_SCRIPT} ${TAG_DEFINITIONS_HEADER} ${TAG_STRUCTURE_OUTPUT_FILES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# Add tag structures as a custom target, so we can add it as a dependency to Balltze
add_custom_target(tag-structures
    DEPENDS ${TAG_STRUCTURE_OUTPUT_FILES} ${TAG_DEFINITIONS_HEADER}
)
