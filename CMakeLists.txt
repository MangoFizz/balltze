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

# Define Balltze export macro
add_definitions(-DBALLTZE_EXPORTS)

add_library(balltze MODULE
    src/balltze/config/config.cpp
    src/balltze/config/ini.cpp
    src/balltze/engine/hud.cpp
    src/balltze/engine/hud.S
    src/balltze/engine/map.cpp
    src/balltze/engine/multiplayer.cpp
    src/balltze/engine/path.cpp
    src/balltze/engine/resolution.cpp
    src/balltze/engine/tag.cpp
    src/balltze/engine/tag.S
    src/balltze/engine/tick.cpp
    src/balltze/engine/version.cpp
    src/balltze/engine/widget.cpp
    src/balltze/engine/widget.S
    src/balltze/engine/window.cpp
    src/balltze/event/event.cpp
    src/balltze/event/game_input.cpp
    src/balltze/event/game_input.S
    src/balltze/event/hud_hold_to_action_message.cpp
    src/balltze/event/hud_hold_to_action_message.S
    src/balltze/event/tick.cpp
    src/balltze/features/gamepad_pause_button.cpp
    src/balltze/features/hud_button_prompts.cpp
    src/balltze/map_loading/laa.cpp
    src/balltze/map_loading/map.cpp
    src/balltze/map_loading/map_loading.cpp
    src/balltze/map_loading/map_loading.S
    src/balltze/memory/codefinder.cpp
    src/balltze/memory/hook.cpp
    src/balltze/memory/memory.cpp
    src/balltze/output/messaging.cpp
    src/balltze/balltze.cpp
)

# Set Balltze output properties
set_target_properties(balltze PROPERTIES PREFIX "")
set_target_properties(balltze PROPERTIES OUTPUT_NAME "balltze")
set_target_properties(balltze PROPERTIES LINK_FLAGS "-static -static-libgcc -static-libstdc++")

add_dependencies(balltze tag-definitions-hpp tag-functions)

target_link_libraries(balltze tag-functions)

# Install Balltze
install(TARGETS balltze DESTINATION "${CMAKE_INSTALL_PREFIX}")
