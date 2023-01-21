# SPDX-License-Identifier: GPL-3.0-only

# We need C99 to build Lua
set(CMAKE_C_STANDARD 99)

# Source path
set(LUA53_SRC src/lua)

# Set Lua source files
add_library(lua53 STATIC
    # core
    ${LUA53_SRC}/lapi.c
    ${LUA53_SRC}/lcode.c
    ${LUA53_SRC}/lctype.c
    ${LUA53_SRC}/ldebug.c
    ${LUA53_SRC}/ldo.c
    ${LUA53_SRC}/ldump.c
    ${LUA53_SRC}/lfunc.c
    ${LUA53_SRC}/lgc.c
    ${LUA53_SRC}/llex.c
    ${LUA53_SRC}/lmem.c
    ${LUA53_SRC}/lobject.c
    ${LUA53_SRC}/lopcodes.c
    ${LUA53_SRC}/lparser.c
    ${LUA53_SRC}/lstate.c
    ${LUA53_SRC}/lstring.c
    ${LUA53_SRC}/ltable.c
    ${LUA53_SRC}/ltm.c
    ${LUA53_SRC}/lundump.c
    ${LUA53_SRC}/lvm.c
    ${LUA53_SRC}/lzio.c

    # libraries
    ${LUA53_SRC}/lauxlib.c
)

include_directories(LUA53_SRC)
