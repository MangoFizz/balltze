# SPDX-License-Identifier: GPL-3.0-only

include_directories(lib/lua_memory_snapshot/)

add_library(lua-memory-snapshot STATIC
    lib/lua_memory_snapshot/lua_memory_snapshot.c
)
