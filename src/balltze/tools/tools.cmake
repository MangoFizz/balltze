# SPDX-License-Identifier: GPL-3.0-only

add_executable(lua-types-dump
    $<TARGET_OBJECTS:balltze>
    src/balltze/tools/lua_types_dump.cpp
)

target_link_libraries(lua-types-dump PRIVATE ringworld lua53 fmt invader luastruct lanes lua-fmt lua-memory-snapshot d3d9 gdiplus ws2_32)
set_target_properties(lua-types-dump PROPERTIES LINK_FLAGS "-static -static-libgcc -static-libstdc++")
