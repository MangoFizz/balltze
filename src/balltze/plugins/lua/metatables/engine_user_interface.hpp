// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__PLUGINS__LUA__METATABLES__ENGINE_USER_INTERFACE_HPP
#define BALLTZE__PLUGINS__LUA__METATABLES__ENGINE_USER_INTERFACE_HPP

#include <balltze/engine.hpp>
#include <balltze/plugin.hpp>

namespace Balltze::Plugins {
    void lua_push_meta_engine_widget(lua_State *state, Engine::Widget &widget, bool read_only = false) noexcept;
}

#endif
