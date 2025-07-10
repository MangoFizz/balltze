// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__LUA__API__V2__API_HPP
#define BALLTZE__LUA__API__V2__API_HPP

#include <lua.hpp>
#include <semver.hpp>

namespace Balltze::Lua::Api {
    namespace V2 {
        static const semver::version api_version{2, 0, 0, semver::prerelease::beta, std::nullopt};
    }

    /**
     * Opens the Balltze API version 2 in the given Lua state.
     * 
     * @param state The Lua state to open the API in.
     */
    void open_balltze_api_v2(lua_State *state);
}

#endif
