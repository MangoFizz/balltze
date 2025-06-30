# SPDX-License-Identifier: GPL-3.0-only

include_directories(lib/lua_fmt/)

add_library(lua-fmt STATIC
    lib/lua_fmt/lfmt.c
)
