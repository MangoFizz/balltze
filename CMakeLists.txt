# SPDX-License-Identifier: GPL-3.0-only

cmake_minimum_required(VERSION 3.16)

# Set project info
project(balltze
    VERSION 1.0.0
    LANGUAGES ASM C CXX
)

# Set modules path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

# Include stuff
include(lua/code_gen/tag_definitions.cmake)

# Set include path
include_directories(include/)

add_library(balltze MODULE
    src/balltze/config/ini.cpp
    src/balltze/engine/map.cpp
    src/balltze/engine/path.cpp
    src/balltze/engine/tag.cpp
    src/balltze/engine/tag.S
    src/balltze/engine/version.cpp
    src/balltze/engine/window.cpp
    src/balltze/events/map_load.cpp
    src/balltze/events/tick.cpp
    src/balltze/map_loading/laa.cpp
    src/balltze/map_loading/map.cpp
    src/balltze/map_loading/map_loading.cpp
    src/balltze/map_loading/map_loading.S
    src/balltze/memory/codecave.cpp
    src/balltze/memory/codefinder.cpp
    src/balltze/memory/hook.cpp
    src/balltze/memory/memory.cpp
    src/balltze/memory/signature.cpp
    src/balltze/output/message_box.cpp
    src/balltze/balltze.cpp
    src/balltze/main.cpp
)

# Set Balltze output properties
set_target_properties(balltze PROPERTIES PREFIX "")
set_target_properties(balltze PROPERTIES OUTPUT_NAME "balltze")
set_target_properties(balltze PROPERTIES LINK_FLAGS "-static -static-libgcc -static-libstdc++")

add_dependencies(balltze tag-definitions-hpp tag-functions)

target_link_libraries(balltze tag-functions)

# Install Balltze
install(TARGETS balltze DESTINATION "${CMAKE_INSTALL_PREFIX}")
