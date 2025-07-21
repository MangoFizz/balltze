# SPDX-License-Identifier: GPL-3.0-only

add_executable(lua-types-dump
    src/balltze/lua/api/v2/types/engine/cache_file.cpp
    src/balltze/lua/api/v2/types/engine/game_engine.cpp
    src/balltze/lua/api/v2/types/engine/object.cpp
    src/balltze/lua/api/v2/types/engine/player.cpp
    src/balltze/lua/api/v2/types/engine/tag_data.cpp
    src/balltze/lua/api/v2/types/engine/tag.cpp
    src/balltze/lua/api/v2/types/engine/types.cpp
    src/balltze/lua/api/v2/types/event.cpp
    src/balltze/tools/lua_types_dump.cpp
)

target_link_libraries(lua-types-dump PRIVATE luastruct lua53)
set_target_properties(lua-types-dump PROPERTIES LINK_FLAGS "-static -static-libgcc -static-libstdc++")
