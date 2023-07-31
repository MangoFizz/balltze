// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__PLUGINS__LOADER_HPP
#define BALLTZE__PLUGINS__LOADER_HPP

#include <windows.h>
#include "plugin.hpp"

namespace Balltze::Plugins {
    void init_plugins() noexcept;
    std::vector<LuaPlugin *> get_lua_plugins() noexcept;
    LuaPlugin *get_lua_plugin(lua_State *state) noexcept;
    DLLPlugin *get_dll_plugin(HMODULE handle) noexcept;
    void set_up_plugins() noexcept;
}

#endif
