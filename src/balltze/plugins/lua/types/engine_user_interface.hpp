// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__PLUGINS__LUA__TYPES__ENGINE_USER_INTERFACE_HPP
#define BALLTZE__PLUGINS__LUA__TYPES__ENGINE_USER_INTERFACE_HPP

#include <lua.hpp>
#include <balltze/engine/user_interface.hpp>

namespace Balltze::Plugins::Lua {
    using EngineInputDevice = Engine::InputDevice;
    using EngineUIWidget = Engine::Widget;
    using EngineWidgetNavigationSound = Engine::WidgetNavigationSound;

    void define_engine_widget_navigation_sound_enum(lua_State *state) noexcept;

    void push_meta_engine_widget(lua_State *state, Engine::Widget *widget) noexcept;
    Engine::Widget *from_meta_engine_widget(lua_State *state, int index) noexcept;

    void define_engine_user_interface_types(lua_State *state) noexcept;
}

#endif
