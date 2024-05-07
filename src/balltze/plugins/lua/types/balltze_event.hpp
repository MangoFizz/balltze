// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__PLUGINS__LUA__TYPES__BALLTZE_EVENT_HPP
#define BALLTZE__PLUGINS__LUA__TYPES__BALLTZE_EVENT_HPP

#include <lua.hpp>
#include <balltze/event.hpp>
#include <balltze/engine/netgame_messages.hpp>

namespace Balltze::Plugins::Lua {
    using BalltzeCameraEventArgs = Event::CameraEventArgs;
    using BalltzeGameInputEventArgs = Event::GameInputEventArgs;
    using BalltzeHudHoldForActionMessageSlice = Event::HudHoldForActionMessageSlice;
    using BalltzeHudHoldToActionMessageButtonType = Event::HudHoldToActionMessageButton::Type;
    using BalltzeHudHoldToActionMessageAxisDirection = Event::HudHoldToActionMessageButton::AxisDirection;
    using BalltzeHudHoldToActionMessageButton = Event::HudHoldToActionMessageButton;
    using BalltzeHudHoldForActionMessageArgs = Event::HudHoldForActionMessageArgs;
    using BalltzeMapFileLoadEventArgs = Event::MapFileLoadEventArgs;
    using BalltzeMapLoadEventArgs = Event::MapLoadEventArgs;
    using BalltzeNetworkGameChatMessageEventArgs = Event::NetworkGameChatMessageEventArgs;
    using BalltzeHudChatType = Engine::NetworkGameMessages::HudChatType;
    using BalltzeHudChatMessage = Engine::NetworkGameMessages::HudChat;

    void push_meta_balltze_camera_event_args(lua_State *state, BalltzeCameraEventArgs *args) noexcept;

    void define_balltze_event_types(lua_State *state) noexcept;
}

#endif
