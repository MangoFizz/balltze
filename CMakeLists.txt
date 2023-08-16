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
include(src/lua/lua.cmake)
include(src/fmt/fmt.cmake)
include(src/invader/invader.cmake)
include(src/sound-subtitles-stapler/sss.cmake)

# Set include path
include_directories(include src/lua src/)

# Define Balltze export macro
add_definitions(-DBALLTZE_EXPORTS)

# Turn on experimental features
option(BALLTZE_ENABLE_EXPERIMENTAL "enable experimental features" OFF)
if(BALLTZE_ENABLE_EXPERIMENTAL)
    add_definitions(-DBALLTZE_ENABLE_EXPERIMENTAL)
endif()

add_library(balltze SHARED
    src/balltze/command/command.cpp
    src/balltze/command/tab_completion.cpp
    src/balltze/config/config.cpp
    src/balltze/config/ini.cpp
    src/balltze/engine/core.cpp
    src/balltze/engine/core.S
    src/balltze/engine/data_types.cpp
    src/balltze/engine/game_state.cpp
    src/balltze/engine/game_state.S
    src/balltze/engine/map.cpp
    src/balltze/engine/multiplayer.cpp
    src/balltze/engine/multiplayer.S
    src/balltze/engine/tag.cpp
    src/balltze/engine/tag.S
    src/balltze/engine/user_interface.cpp
    src/balltze/engine/user_interface.S
    src/balltze/event/camera.cpp
    src/balltze/event/console_command.cpp
    src/balltze/event/console_command.S
    src/balltze/event/d3d9_device_reset.cpp
    src/balltze/event/d3d9_device_reset.S
    src/balltze/event/d3d9_end_scene.cpp
    src/balltze/event/d3d9_end_scene.S
    src/balltze/event/event.cpp
    src/balltze/event/frame.cpp
    src/balltze/event/game_input.cpp
    src/balltze/event/game_input.S
    src/balltze/event/hud_hold_to_action_message.cpp
    src/balltze/event/hud_hold_to_action_message.S
    src/balltze/event/map_file_data_read.cpp
    src/balltze/event/map_file_data_read.S
    src/balltze/event/map_file_load.cpp
    src/balltze/event/map_load.cpp
    src/balltze/event/map_load.S
    src/balltze/event/object_damage.cpp
    src/balltze/event/object_damage.S
    src/balltze/event/rcon_message.cpp
    src/balltze/event/server_connect.cpp
    src/balltze/event/server_connect.S
    src/balltze/event/sound_playback.cpp
    src/balltze/event/sound_playback.S
    src/balltze/event/tick.cpp
    src/balltze/features/gamepad_pause_button.cpp
    src/balltze/features/hud_button_prompts.cpp
    src/balltze/features/loading_screen.cpp
    src/balltze/features/loading_screen.S
    src/balltze/features/sound_subtitles.cpp
    src/balltze/features/tag_data_importing/map.cpp
    src/balltze/features/tag_data_importing/tag_data_importing.cpp
    src/balltze/features/tag_data_importing/tag_data_importing.S
    src/balltze/lua_library/library.cpp
    src/balltze/math/bezier.cpp
    src/balltze/math/trig.cpp
    src/balltze/memory/codefinder.cpp
    src/balltze/memory/hook.cpp
    src/balltze/memory/memory.cpp
    src/balltze/output/draw_text.cpp
    src/balltze/output/draw_text.S
    src/balltze/output/logger.cpp
    src/balltze/output/messaging.cpp
    src/balltze/output/subtitles.cpp
    src/balltze/output/video.cpp
    src/balltze/output/video.S
    src/balltze/plugins/loader.cpp
    src/balltze/plugins/plugin.cpp
    src/balltze/plugins/lua/api.cpp
    src/balltze/plugins/lua/chimera.cpp
    src/balltze/plugins/lua/command.cpp
    src/balltze/plugins/lua/engine.cpp
    src/balltze/plugins/lua/features.cpp
    src/balltze/plugins/lua/event.cpp
    src/balltze/plugins/lua/helpers.cpp
    src/balltze/plugins/lua/lfmt.cpp
    src/balltze/plugins/lua/lfmt.c
    src/balltze/plugins/lua/logger.cpp
    src/balltze/plugins/lua/output.cpp
    src/balltze/balltze.cpp

    ${TAG_REBASE_OFFSETS_FUNCTION_CPP} 
    ${TAG_RESOLVE_DEPENDENCIES_FUNCTION_CPP} 
    ${TAG_LUA_METATABLE_FUNCTION_CPP}
    ${TAG_COPY_DATA_FUNCTION_CPP}
)

# Set Balltze output properties
set_target_properties(balltze PROPERTIES PREFIX "")
set_target_properties(balltze PROPERTIES OUTPUT_NAME "balltze")
set_target_properties(balltze PROPERTIES LINK_FLAGS "-static -static-libgcc -static-libstdc++")
add_dependencies(balltze tag-definitions-headers)
target_link_libraries(balltze lua53 fmt invader)

# Install Balltze
install(TARGETS balltze DESTINATION "${CMAKE_INSTALL_PREFIX}")
