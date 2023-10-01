# SPDX-License-Identifier: GPL-3.0-only

# We need C99 to build Lua
set(CMAKE_C_STANDARD 99)

# C compiler flags
set(CMAKE_C_FLAGS "-Wall -Wextra")

# Set Lua source files
add_library(lua53 STATIC
    lib/lua/lapi.c
    lib/lua/lauxlib.c
    lib/lua/lbaselib.c
    lib/lua/lbitlib.c
    lib/lua/lcode.c
    lib/lua/lcorolib.c
    lib/lua/lctype.c
    lib/lua/ldblib.c
    lib/lua/ldebug.c
    lib/lua/ldo.c
    lib/lua/ldump.c
    lib/lua/lfunc.c
    lib/lua/lgc.c
    lib/lua/linit.c
    lib/lua/liolib.c
    lib/lua/llex.c
    lib/lua/lmathlib.c
    lib/lua/lmem.c
    lib/lua/loadlib.c
    lib/lua/lobject.c
    lib/lua/lopcodes.c
    lib/lua/loslib.c
    lib/lua/lparser.c
    lib/lua/lstate.c
    lib/lua/lstring.c
    lib/lua/lstrlib.c
    lib/lua/ltable.c
    lib/lua/ltablib.c
    lib/lua/ltm.c
    lib/lua/luac.c
    lib/lua/lundump.c
    lib/lua/lutf8lib.c
    lib/lua/lvm.c
    lib/lua/lzio.c
)

target_include_directories(lua53 PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lua
)
