# SPDX-License-Identifier: GPL-3.0-only

cmake_minimum_required(VERSION 3.16)

# Find Lua runtime
find_package(LuaRuntime REQUIRED)

# Our super Lua scripts
set(TAG_DEFINITION_HEADERS_GENERATOR_SCRIPT ${CMAKE_SOURCE_DIR}/lua/code_gen/tag_definition_headers.lua)
set(TAG_REBASE_OFFSETS_FUNCTION_GENERATOR_SCRIPT ${CMAKE_SOURCE_DIR}/lua/code_gen/tag_rebase_offsets_function.lua)
set(TAG_FIX_DEPENDENCIES_FUNCTION_GENERATOR_SCRIPT ${CMAKE_SOURCE_DIR}/lua/code_gen/tag_resolve_dependencies_function.lua)
set(LUA_ENVIRONMENT_SCRIPT ${CMAKE_SOURCE_DIR}/lua/env.lua)

# Tag definitions
set(TAG_DEFINITIONS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/balltze/engine/tag_definitions")
file(GLOB TAG_DEFINITION_FILES ${TAG_DEFINITIONS_PATH}/*.json)

# Output files
set(INCLUDES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include/balltze")
set(TAG_DEFINITIONS_HPP_PATH "${INCLUDES_PATH}/engine/tag_definitions")
set(TAG_DEFINITIONS_HPP_COLLECTION "${TAG_DEFINITIONS_HPP_PATH}/definitions.hpp")
set(TAG_FIX_DEPENDENCIES_FUNCTION_CPP "${CMAKE_BINARY_DIR}/tag_fix_dependencies.cpp")
set(TAG_REBASE_OFFSETS_FUNCTION_CPP "${CMAKE_BINARY_DIR}/tag_rebase_offsets.cpp")
set(TAG_DEFINITION_HPP_FILES)
set(TAG_DEFINITION_CPP_FILES)

# Create output directory for tag structures and get output file paths
file(MAKE_DIRECTORY ${TAG_DEFINITIONS_HPP_PATH})
foreach(TAG_DEFINITION_FILE ${TAG_DEFINITION_FILES})
    get_filename_component(TAG_DEFINITION_NAME ${TAG_DEFINITION_FILE} NAME_WE)
    set(TAG_DEFINITION_HPP_FILE "${TAG_DEFINITIONS_HPP_PATH}/${TAG_DEFINITION_NAME}.hpp")
    set(TAG_DEFINITION_HPP_FILES ${TAG_DEFINITION_HPP_FILES} ${TAG_DEFINITION_HPP_FILE})
endforeach()

# Tell CMake how to generate the tag struct headers
add_custom_command(
    OUTPUT ${TAG_DEFINITION_HPP_FILES} ${TAG_DEFINITIONS_HPP_COLLECTION} ${TAG_REBASE_OFFSETS_FUNCTION_CPP} ${TAG_FIX_DEPENDENCIES_FUNCTION_CPP}
    COMMAND ${CMAKE_COMMAND} -E env LUA_INIT="@${LUA_ENVIRONMENT_SCRIPT}" ${LUA_EXECUTABLE} ${TAG_DEFINITION_HEADERS_GENERATOR_SCRIPT} ${INCLUDES_PATH} ${TAG_DEFINITION_FILES}
    COMMAND ${CMAKE_COMMAND} -E env LUA_INIT="@${LUA_ENVIRONMENT_SCRIPT}" ${LUA_EXECUTABLE} ${TAG_REBASE_OFFSETS_FUNCTION_GENERATOR_SCRIPT} ${TAG_REBASE_OFFSETS_FUNCTION_CPP} ${TAG_DEFINITION_FILES}
    COMMAND ${CMAKE_COMMAND} -E env LUA_INIT="@${LUA_ENVIRONMENT_SCRIPT}" ${LUA_EXECUTABLE} ${TAG_FIX_DEPENDENCIES_FUNCTION_GENERATOR_SCRIPT} ${TAG_FIX_DEPENDENCIES_FUNCTION_CPP} ${TAG_DEFINITION_FILES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Generating tag definitions headers"
    DEPENDS ${TAG_DEFINITION_HEADERS_GENERATOR_SCRIPT} ${TAG_DEFINITION_FILES} ${TAG_REBASE_OFFSETS_FUNCTION_GENERATOR_SCRIPT} ${TAG_FIX_DEPENDENCIES_FUNCTION_GENERATOR_SCRIPT}    
)
set(TAG_DEFINITION_HPP_FILES ${TAG_DEFINITION_HPP_FILES} ${TAG_DEFINITIONS_HPP_COLLECTION})
set(TAG_DEFINITION_CPP_FILES ${TAG_DEFINITION_CPP_FILES} ${TAG_REBASE_OFFSETS_FUNCTION_CPP} ${TAG_FIX_DEPENDENCIES_FUNCTION_CPP})

# Add tag stuff targets, so we can add them as a dependency to Balltze
add_custom_target(tag-definitions-headers DEPENDS ${TAG_DEFINITION_HPP_FILES})
add_library(tag-functions STATIC ${TAG_DEFINITION_CPP_FILES})

set_target_properties(tag-functions PROPERTIES LINKER_LANGUAGE CXX)
add_dependencies(tag-functions tag-definitions-headers)
