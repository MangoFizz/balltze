// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__LUA__API__TYPES__ENGINE_USER_INTERFACE_HPP
#define BALLTZE__LUA__API__TYPES__ENGINE_USER_INTERFACE_HPP

#include <lua.hpp>
#include <balltze/legacy_api/engine/user_interface.hpp>

namespace Balltze::Lua::Api::V1 {
    using EngineInputDevice = LegacyApi::Engine::InputDevice;
    using EngineUIWidget = LegacyApi::Engine::Widget;
    using EngineWidgetNavigationSound = LegacyApi::Engine::WidgetNavigationSound;
    using EngineInputBufferedKey = LegacyApi::Engine::InputGlobals::BufferedKey;
    using EngineInputBufferedKeyModifier = LegacyApi::Engine::InputGlobals::BufferedKey::Modifier;
    using EngineInputMouseButton = LegacyApi::Engine::MouseButton;

    void push_meta_engine_widget(lua_State *state, LegacyApi::Engine::Widget *widget) noexcept;
    LegacyApi::Engine::Widget *from_meta_engine_widget(lua_State *state, int index) noexcept;

    void push_meta_engine_input_buffered_key(lua_State *state, LegacyApi::Engine::InputGlobals::BufferedKey *key) noexcept;
    LegacyApi::Engine::InputGlobals::BufferedKey *from_meta_engine_input_buffered_key(lua_State *state, int index) noexcept;

    void define_engine_user_interface_types(lua_State *state) noexcept;
}

#endif
