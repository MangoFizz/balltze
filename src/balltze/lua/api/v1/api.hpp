// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__LUA__API__API_HPP
#define BALLTZE__LUA__API__API_HPP

#include <lua.hpp>

namespace Balltze::Lua::Api {
    /**
     * Opens the Balltze API version 1 in the given Lua state.
     * 
     * @param state The Lua state to open the API in.
     */
    void open_balltze_api_v1(lua_State *state);
}

#endif
