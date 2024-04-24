// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__PLUGINS__LUA__LIBRARIES__PRELOADED_LIBRARIES_HPP
#define BALLTZE__PLUGINS__LUA__LIBRARIES__PRELOADED_LIBRARIES_HPP

#include <lua.hpp>

namespace Balltze::Plugins {
    void lua_set_preloaded_libraries(lua_State *state) noexcept;
}

#endif
