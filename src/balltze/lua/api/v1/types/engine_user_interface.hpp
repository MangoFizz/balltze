// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__LUA__API__TYPES__ENGINE_USER_INTERFACE_HPP
#define BALLTZE__LUA__API__TYPES__ENGINE_USER_INTERFACE_HPP

#include <lua.hpp>
#include <balltze/engine/user_interface.hpp>

namespace Balltze::Lua::Api::V1 {
    using EngineInputDevice = Engine::InputDevice;
    using EngineUIWidget = Engine::Widget;
    using EngineWidgetNavigationSound = Engine::WidgetNavigationSound;
    using EngineInputBufferedKey = Engine::InputGlobals::BufferedKey;
    using EngineInputBufferedKeyModifier = Engine::InputGlobals::BufferedKey::Modifier;
    using EngineInputMouseButton = Engine::MouseButton;

    void push_meta_engine_widget(lua_State *state, Engine::Widget *widget) noexcept;
    Engine::Widget *from_meta_engine_widget(lua_State *state, int index) noexcept;

    void push_meta_engine_input_buffered_key(lua_State *state, Engine::InputGlobals::BufferedKey *key) noexcept;
    Engine::InputGlobals::BufferedKey *from_meta_engine_input_buffered_key(lua_State *state, int index) noexcept;

    void define_engine_user_interface_types(lua_State *state) noexcept;
}

#endif
