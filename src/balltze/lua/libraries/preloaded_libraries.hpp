// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__LUA__LIBRARIES__PRELOADED_LIBRARIES_HPP
#define BALLTZE__LUA__LIBRARIES__PRELOADED_LIBRARIES_HPP

#include <lua.hpp>

namespace Balltze::Lua {
    /**
     * Sets up the preloaded libraries for the Lua state.
     */
    void set_preloaded_libraries(lua_State *state) noexcept;
}

#endif
