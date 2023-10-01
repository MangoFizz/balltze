// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__PLUGINS__LUA__LFMT_H
#define BALLTZE__PLUGINS__LUA__LFMT_H

#include <lua.h>
#include <lauxlib.h>

typedef struct fmt_State {
    lua_State *L;
    luaL_Buffer B;
    int idx, top, zeroing;
    const char *p, *e;
} fmt_State;

int fmt_format(fmt_State *fmt_state);
int Lformat(lua_State *state);

#endif
