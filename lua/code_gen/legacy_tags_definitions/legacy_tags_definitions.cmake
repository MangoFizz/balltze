# SPDX-License-Identifier: GPL-3.0-only

cmake_minimum_required(VERSION 3.16)

# Find Lua runtime
find_package(LuaRuntime REQUIRED)

# Tag definitions
set(TAG_DEFINITIONS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/invader/tag/hek/definition")
file(GLOB TAG_DEFINITION_FILES ${TAG_DEFINITIONS_PATH}/*.json)

# Output files
set(INCLUDES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include/balltze")
set(TAG_DEFINITIONS_HPP_PATH "${INCLUDES_PATH}/legacy_api/engine/tag_definitions")
set(TAG_DEFINITIONS_HPP_COLLECTION "${INCLUDES_PATH}/legacy_api/engine/tag_definitions.hpp")
set(TAG_RESOLVE_DEPENDENCIES_FUNCTION_CPP "${CMAKE_BINARY_DIR}/tag_resolve_dependencies.cpp")
set(TAG_REBASE_OFFSETS_FUNCTION_CPP "${CMAKE_BINARY_DIR}/tag_rebase_offsets.cpp")
set(TAG_COPY_DATA_FUNCTION_CPP "${CMAKE_BINARY_DIR}/tag_copy_data.cpp")
set(TAG_DEFINITION_HPP_FILES)
set(TAG_DEFINITION_CPP_FILES)

function(to_upper initial_string output_string)
    string(TOUPPER "${initial_string}" ${output_string})
    set(${output_string} "${${output_string}}" PARENT_SCOPE)
endfunction()

function(to_lower_camelcase input_string output_variable)
    set(camel_case "")
    string(REGEX MATCHALL "_." matches "${input_string}")
    foreach(match IN LISTS matches)
        string(SUBSTRING "${match}" 1 1 char)
        to_upper("${char}" upper_char)
        string(REPLACE "${match}" "${upper_char}" input_string "${input_string}")
    endforeach()
    string(REGEX REPLACE "_" "" camel_case "${input_string}")
    set(${output_variable} "${camel_case}" PARENT_SCOPE)
endfunction()

# Create output directory for tag structures and get output file paths
file(MAKE_DIRECTORY ${TAG_DEFINITIONS_HPP_PATH})
file(MAKE_DIRECTORY ${TAG_LUA_ANNOTATIONS_PATH})
foreach(TAG_DEFINITION_FILE ${TAG_DEFINITION_FILES})
    get_filename_component(TAG_DEFINITION_NAME ${TAG_DEFINITION_FILE} NAME_WE)
    set(TAG_DEFINITION_HPP_FILES ${TAG_DEFINITION_HPP_FILES} "${TAG_DEFINITIONS_HPP_PATH}/${TAG_DEFINITION_NAME}.hpp")
    to_lower_camelcase("engine_tag_data_${TAG_DEFINITION_NAME}" CAMEL_CASE_DEF_NAME)
endforeach()

# Tell CMake how to generate the tag struct headers
set(LUA_ENVIRONMENT_SCRIPT ${CMAKE_SOURCE_DIR}/lua/env.lua)
set(LUA_COMMNAD ${CMAKE_COMMAND} -E env LUA_INIT="@${LUA_ENVIRONMENT_SCRIPT}" ${LUA_EXECUTABLE})
set(TAG_DEFINITION_PARSER_SCRIPT ${CMAKE_SOURCE_DIR}/lua/code_gen/legacy_tags_definitions/parse_tag_definition.lua)
set(TAG_DEFINITION_HEADERS_GENERATOR_SCRIPT ${CMAKE_SOURCE_DIR}/lua/code_gen/legacy_tags_definitions/tag_definition_headers.lua)
set(TAG_REBASE_OFFSETS_FUNCTION_GENERATOR_SCRIPT ${CMAKE_SOURCE_DIR}/lua/code_gen/legacy_tags_definitions/tag_rebase_offsets_function.lua)
set(TAG_RESOLVE_DEPENDENCIES_FUNCTION_GENERATOR_SCRIPT ${CMAKE_SOURCE_DIR}/lua/code_gen/legacy_tags_definitions/tag_resolve_dependencies_function.lua)
set(TAG_COPY_DATA_FUNCTION_GENERATOR_SCRIPT ${CMAKE_SOURCE_DIR}/lua/code_gen/legacy_tags_definitions/tag_copy_data_function.lua)

add_custom_command(
    OUTPUT ${TAG_DEFINITION_HPP_FILES} ${TAG_DEFINITIONS_HPP_COLLECTION} 
    COMMAND ${LUA_COMMNAD} ${TAG_DEFINITION_HEADERS_GENERATOR_SCRIPT} ${INCLUDES_PATH} ${TAG_DEFINITION_FILES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Generating tag definitions headers..."
    DEPENDS ${TAG_DEFINITION_PARSER_SCRIPT} ${TAG_DEFINITION_HEADERS_GENERATOR_SCRIPT} ${TAG_DEFINITION_FILES}
)

add_custom_command(
    OUTPUT ${TAG_REBASE_OFFSETS_FUNCTION_CPP} 
    COMMAND ${LUA_COMMNAD} ${TAG_REBASE_OFFSETS_FUNCTION_GENERATOR_SCRIPT} ${TAG_REBASE_OFFSETS_FUNCTION_CPP} ${TAG_DEFINITION_FILES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Generating tag rebase offsets functions..."
    DEPENDS ${TAG_DEFINITION_PARSER_SCRIPT} ${TAG_REBASE_OFFSETS_FUNCTION_GENERATOR_SCRIPT} ${TAG_DEFINITION_FILES}
)

add_custom_command(
    OUTPUT ${TAG_RESOLVE_DEPENDENCIES_FUNCTION_CPP}
    COMMAND ${LUA_COMMNAD} ${TAG_RESOLVE_DEPENDENCIES_FUNCTION_GENERATOR_SCRIPT} ${TAG_RESOLVE_DEPENDENCIES_FUNCTION_CPP} ${TAG_DEFINITION_FILES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Generating tag fix dependencies functions..."
    DEPENDS ${TAG_DEFINITION_PARSER_SCRIPT} ${TAG_RESOLVE_DEPENDENCIES_FUNCTION_GENERATOR_SCRIPT} ${TAG_DEFINITION_FILES}
)

add_custom_command(
    OUTPUT ${TAG_COPY_DATA_FUNCTION_CPP}
    COMMAND ${LUA_COMMNAD} ${TAG_COPY_DATA_FUNCTION_GENERATOR_SCRIPT} ${TAG_COPY_DATA_FUNCTION_CPP} ${TAG_DEFINITION_FILES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Generating tag copy data functions..."
    DEPENDS ${TAG_DEFINITION_PARSER_SCRIPT} ${TAG_COPY_DATA_FUNCTION_GENERATOR_SCRIPT} ${TAG_DEFINITION_FILES}
)

set(TAG_DEFINITION_HPP_FILES ${TAG_DEFINITION_HPP_FILES} ${TAG_DEFINITIONS_HPP_COLLECTION})

# Add definitions headers target, so we can add them as a dependency to Balltze
add_custom_target(tag-definitions-headers ALL DEPENDS ${TAG_DEFINITION_HPP_FILES})
