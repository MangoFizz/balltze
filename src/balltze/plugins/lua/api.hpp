// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__PLUGINS__LUA__API_HPP
#define BALLTZE__PLUGINS__LUA__API_HPP

#include <lua.hpp>

namespace Balltze::Plugins {
    void lua_open_balltze_api(lua_State *state);
}

#endif
