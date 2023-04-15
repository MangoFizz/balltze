// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__PLUGINS__LUA__HELPERS_HPP
#define BALLTZE__PLUGINS__LUA__HELPERS_HPP

#include <lua.hpp>

namespace Balltze::Plugins {
    void lua_create_functions_table(lua_State *state, const char *name, const luaL_Reg *functions) noexcept;
}

#endif
