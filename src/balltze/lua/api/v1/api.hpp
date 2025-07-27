// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__LUA__API__V1__API_HPP
#define BALLTZE__LUA__API__V1__API_HPP

#include <lua.hpp>
#include <semver/semver.hpp>

namespace Balltze::Lua::Api {
    namespace V1 {
        static const semver::version api_version{1, 1, 0};
    }

    /**
     * Opens the Balltze API version 1 in the given Lua state.
     * 
     * @param state The Lua state to open the API in.
     */
    void open_balltze_api_v1(lua_State *state);
}

#endif
