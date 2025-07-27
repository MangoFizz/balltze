# SPDX-License-Identifier: GPL-3.0-only

cmake_minimum_required(VERSION 3.16)

# Set project info
project(balltze
    VERSION 1.2.0
    LANGUAGES ASM C CXX
)

set(CMAKE_CXX_STANDARD 20)

# Minimum supported platform is Windows 7
add_definitions(-D_WIN32_WINNT=0x0601)

# Set modules path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

# Include stuff
include(lib/lua/lua.cmake)
include(lib/lua_fmt/lua_fmt.cmake)
include(lib/lua_memory_snapshot/lua_memory_snapshot.cmake)
include(lib/fmt/fmt.cmake)
include(lib/luacstruct/luacstruct.cmake)
include(lib/luastruct/luastruct.cmake)
include(lib/invader/invader.cmake)
include(lib/lanes/lanes.cmake)
include(src/balltze/tools/tools.cmake)
include(lua/code_gen/legacy_tags_definitions/legacy_tags_definitions.cmake)
include(lua/code_gen/ringworld_tags_definitions/ringworld_tags_definitions.cmake)

# Set ringworld 
set(RINGWORLD_BUILD_STATIC ON CACHE BOOL "Build a static library instead of the standalone module")
set(RINGWORLD_ENABLE_EVENTS_BUS ON CACHE BOOL "Enable the events bus")
set(RINGWORLD_ENABLE_ENHANCEMENTS ON CACHE BOOL "Enable enhancements")
add_subdirectory(src/ringworld)

# Set include path
include_directories(include include/lua lib/ lib/lua lib/lanes/src src/ringworld/src)

# Set defines for Balltze
add_definitions(-DBALLTZE_EXPORTS -DRINGWORLD_ENABLE_EVENTS_BUS -DRINGWORLD_ENABLE_ENHANCEMENTS)

# Turn on experimental features by default
add_definitions(-DBALLTZE_ENABLE_EXPERIMENTAL)

add_library(balltze SHARED
    src/balltze/command/command.cpp
    src/balltze/command/command.S
    src/balltze/command/help.cpp
    src/balltze/command/tab_completion.cpp
    src/balltze/config/chimera_preferences.cpp
    src/balltze/config/config.cpp
    src/balltze/config/ini.cpp
    src/balltze/events/events.cpp
    src/balltze/events/frame.cpp
    src/balltze/events/map.cpp
    src/balltze/events/tick.cpp
    src/balltze/features/gamepad_pause_button.cpp
    src/balltze/features/hud_button_prompts.cpp
    src/balltze/features/loading_screen/loading_screen.cpp
    src/balltze/features/loading_screen/loading_screen.S
    src/balltze/features/loading_screen/loading_screen.rc
    src/balltze/features/tags_handling/map.cpp
    src/balltze/features/tags_handling/tag_data_importing.cpp
    src/balltze/features/tags_handling/tag_data_importing.S
    src/balltze/features/console_key_binding.cpp
    src/balltze/features/map_data_read_warden.cpp
    src/balltze/features/map_textures_preloading.cpp
    src/balltze/features/extended_limits.cpp
    src/balltze/features/extended_limits.S
    src/balltze/features/echo_message.cpp
    src/balltze/helpers/d3d9.cpp
    src/balltze/helpers/resources.cpp
    src/balltze/helpers/sprite_shader.rc
    src/balltze/legacy_api/engine/core.cpp
    src/balltze/legacy_api/engine/core.S
    src/balltze/legacy_api/engine/data_types.cpp
    src/balltze/legacy_api/engine/game_state.cpp
    src/balltze/legacy_api/engine/game_state.S
    src/balltze/legacy_api/engine/map.cpp
    src/balltze/legacy_api/engine/netgame.cpp
    src/balltze/legacy_api/engine/netgame.S
    src/balltze/legacy_api/engine/rasterizer.cpp
    src/balltze/legacy_api/engine/rasterizer.S
    src/balltze/legacy_api/engine/script.cpp
    src/balltze/legacy_api/engine/script.S
    src/balltze/legacy_api/engine/tag.cpp
    src/balltze/legacy_api/engine/tag.S
    src/balltze/legacy_api/engine/saved_games.cpp
    src/balltze/legacy_api/engine/user_interface.cpp
    src/balltze/legacy_api/engine/user_interface.S
    src/balltze/legacy_api/event/camera.cpp
    src/balltze/legacy_api/event/console_command.cpp
    src/balltze/legacy_api/event/console_command.S
    src/balltze/legacy_api/event/d3d9.cpp
    src/balltze/legacy_api/event/d3d9.S
    src/balltze/legacy_api/event/event.cpp
    src/balltze/legacy_api/event/frame.cpp
    src/balltze/legacy_api/event/game_input.cpp
    src/balltze/legacy_api/event/game_input.S
    src/balltze/legacy_api/event/hud_hold_to_action_message.cpp
    src/balltze/legacy_api/event/hud_hold_to_action_message.S
    src/balltze/legacy_api/event/map_load.cpp
    src/balltze/legacy_api/event/map_load.S
    src/balltze/legacy_api/event/netgame_message.cpp
    src/balltze/legacy_api/event/netgame_message.S
    src/balltze/legacy_api/event/object_damage.cpp
    src/balltze/legacy_api/event/object_damage.S
    src/balltze/legacy_api/event/rcon_message.cpp
    src/balltze/legacy_api/event/rcon_message.S
    src/balltze/legacy_api/event/render.cpp
    src/balltze/legacy_api/event/render.S
    src/balltze/legacy_api/event/server_connect.cpp
    src/balltze/legacy_api/event/server_connect.S
    src/balltze/legacy_api/event/sound_playback.cpp
    src/balltze/legacy_api/event/sound_playback.S
    src/balltze/legacy_api/event/tick.cpp
    src/balltze/legacy_api/event/widget.cpp
    src/balltze/legacy_api/event/widget.S
    src/balltze/legacy_api/plugins/loader.cpp
    src/balltze/legacy_api/plugins/plugin.cpp
    src/balltze/lua/api/v1/api.cpp
    src/balltze/lua/api/v1/functions/config.cpp
    src/balltze/lua/api/v1/functions/devkit.cpp
    src/balltze/lua/api/v1/functions/engine/core.cpp
    src/balltze/lua/api/v1/functions/engine.cpp
    src/balltze/lua/api/v1/functions/engine/game_state.cpp
    src/balltze/lua/api/v1/functions/engine/hsc.cpp
    src/balltze/lua/api/v1/functions/engine/map.cpp
    src/balltze/lua/api/v1/functions/engine/netgame.cpp
    src/balltze/lua/api/v1/functions/engine/rasterizer.cpp
    src/balltze/lua/api/v1/functions/engine/saved_games.cpp
    src/balltze/lua/api/v1/functions/engine/tag.cpp
    src/balltze/lua/api/v1/functions/engine/user_interface.cpp
    src/balltze/lua/api/v1/functions/features.cpp
    src/balltze/lua/api/v1/functions/filesystem.cpp
    src/balltze/lua/api/v1/functions/logger.cpp
    src/balltze/lua/api/v1/functions/math.cpp
    src/balltze/lua/api/v1/functions/memory.cpp
    src/balltze/lua/api/v1/functions/misc.cpp
    src/balltze/lua/api/v1/functions/output.cpp
    src/balltze/lua/api/v1/plugin/chimera_library.cpp
    src/balltze/lua/api/v1/plugin/commands.cpp
    src/balltze/lua/api/v1/plugin/events.cpp
    src/balltze/lua/api/v1/types/balltze_event.cpp
    src/balltze/lua/api/v1/types/engine_gamestate_device_object.cpp
    src/balltze/lua/api/v1/types/engine_gamestate_item_object.cpp
    src/balltze/lua/api/v1/types/engine_gamestate_object.cpp
    src/balltze/lua/api/v1/types/engine_gamestate_player.cpp
    src/balltze/lua/api/v1/types/engine_gamestate_unit_object.cpp
    src/balltze/lua/api/v1/types/engine_map.cpp
    src/balltze/lua/api/v1/types/engine_rasterizer.cpp
    src/balltze/lua/api/v1/types/engine_tag.cpp
    src/balltze/lua/api/v1/types/engine_tag_data.cpp
    src/balltze/lua/api/v1/types/engine_types.cpp
    src/balltze/lua/api/v1/types/engine_user_interface.cpp
    src/balltze/lua/api/v1/types/ringworld_saved_games.cpp
    src/balltze/lua/api/v2/functions/engine/cache_file.cpp
    src/balltze/lua/api/v2/functions/engine/console.cpp
    src/balltze/lua/api/v2/functions/engine/game.cpp
    src/balltze/lua/api/v2/functions/engine/object.cpp
    src/balltze/lua/api/v2/functions/engine/script.cpp
    src/balltze/lua/api/v2/functions/engine/tag.cpp
    src/balltze/lua/api/v2/functions/engine.cpp
    src/balltze/lua/api/v2/functions/config.cpp
    src/balltze/lua/api/v2/functions/misc.cpp
    src/balltze/lua/api/v2/plugin/commands.cpp
    src/balltze/lua/api/v2/plugin/events.cpp
    src/balltze/lua/api/v2/plugin/filesystem.cpp
    src/balltze/lua/api/v2/plugin/logger.cpp
    src/balltze/lua/api/v2/types/engine/cache_file.cpp
    src/balltze/lua/api/v2/types/engine/device.cpp
    src/balltze/lua/api/v2/types/engine/game_engine.cpp
    src/balltze/lua/api/v2/types/engine/item.cpp
    src/balltze/lua/api/v2/types/engine/object.cpp
    src/balltze/lua/api/v2/types/engine/player.cpp
    src/balltze/lua/api/v2/types/engine/tag_data.cpp
    src/balltze/lua/api/v2/types/engine/tag.cpp
    src/balltze/lua/api/v2/types/engine/types.cpp
    src/balltze/lua/api/v2/types/engine/unit.cpp
    src/balltze/lua/api/v2/types/engine/weapon.cpp
    src/balltze/lua/api/v2/types/event.cpp
    src/balltze/lua/api/v2/api.cpp
    src/balltze/lua/debug/lua_state_debug.cpp
    src/balltze/lua/helpers/registry.cpp
    src/balltze/lua/helpers/table.cpp
    src/balltze/lua/libraries/preloaded_libraries.cpp
    src/balltze/lua/libraries/preloaded_libraries.rc
    src/balltze/math/bezier.cpp
    src/balltze/math/trig.cpp
    src/balltze/memory/codefinder.cpp
    src/balltze/memory/hook.cpp
    src/balltze/memory/memory.cpp
    src/balltze/output/logger.cpp
    src/balltze/output/messaging.cpp
    src/balltze/output/video.cpp
    src/balltze/output/video.S
    src/balltze/plugins/loader.cpp
    src/balltze/plugins/plugin.cpp
    src/balltze/balltze.cpp

    ${TAG_REBASE_OFFSETS_FUNCTION_CPP} 
    ${TAG_RESOLVE_DEPENDENCIES_FUNCTION_CPP} 
    ${TAG_LUA_TAG_DEFINITIONS_CPP}
    ${TAG_COPY_DATA_FUNCTION_CPP}
)

# Set Balltze output properties
set_target_properties(balltze PROPERTIES PREFIX "")
set_target_properties(balltze PROPERTIES OUTPUT_NAME "balltze")
set_target_properties(balltze PROPERTIES LINK_FLAGS "-static -static-libgcc -static-libstdc++")
add_dependencies(balltze tag-definitions-headers)
target_link_libraries(balltze ringworld lua53 fmt invader luacstruct luastruct lanes lua-fmt lua-memory-snapshot d3d9 gdiplus ws2_32)

# Install Balltze
install(TARGETS balltze DESTINATION "${CMAKE_INSTALL_PREFIX}")
