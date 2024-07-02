// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__PLUGINS__LUA__TYPES__BALLTZE_EVENT_HPP
#define BALLTZE__PLUGINS__LUA__TYPES__BALLTZE_EVENT_HPP

#include <lua.hpp>
#include <balltze/event.hpp>
#include <balltze/engine/netgame_messages.hpp>

namespace Balltze::Plugins::Lua {
    using BalltzeCameraEventArgs = Event::CameraEventArgs;
    using BalltzeGameInputEventArgs = Event::GameInputEventArgs;
    using BalltzeKeyboardEventArgs = Event::KeyboardInputEventArgs;
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
    using BalltzeObjectDamageEventArgs = Event::ObjectDamageEventArgs;
    using BalltzeRconMessageEventArgs = Event::RconMessageEventArgs;
    using BalltzeUIWidgetRenderVertex = Event::UIWidgetRenderVertices::Vertex;
    using BalltzeUIWidgetRenderVertices = Event::UIWidgetRenderVertices;
    using BalltzeHudElementRenderEventArgs = Event::HUDElementBitmapRenderEventArgs;
    using BalltzeUIWidgetBackgroundRenderEventArgs = Event::UIWidgetBackgroundRenderEventArgs;
    using BalltzeServerConnectEventArgs = Event::ServerConnectEventArgs;
    using BalltzeSoundPlaybackEventArgs = Event::SoundPlaybackEventArgs;
    using BalltzeUIWidgetCreateEventArgs = Event::UIWidgetCreateEventArgs;
    using BalltzeUIWidgetEventArgs = Event::UIWidgetEventArgs;
    using BalltzeUIWidgetSoundEventArgs = Event::UIWidgetSoundEventArgs;
    using BalltzeUIWidgetListTabType = Event::UIWidgetListTabType;
    using BalltzeUIWidgetListTabEventArgs = Event::UIWidgetListTabEventArgs;

    void push_meta_balltze_camera_event_args(lua_State *state, Event::CameraEventArgs *args) noexcept;
    void push_meta_balltze_game_input_event_args(lua_State *state, Event::GameInputEventArgs *args) noexcept;
    void push_meta_balltze_keyboard_input_event_args(lua_State *state, BalltzeKeyboardEventArgs *args) noexcept;
    void push_meta_balltze_hud_hold_to_action_message_button(lua_State *state, Event::HudHoldToActionMessageButton *button) noexcept;
    void push_meta_balltze_hud_hold_for_action_message_event_args(lua_State *state, Event::HudHoldForActionMessageArgs *args) noexcept;
    void push_meta_balltze_map_file_load_event_args(lua_State *state, Event::MapFileLoadEventArgs *args) noexcept;
    void push_meta_balltze_map_load_event_args(lua_State *state, Event::MapLoadEventArgs *args) noexcept;
    void push_meta_balltze_network_game_chat_message_event_args(lua_State *state, Event::NetworkGameChatMessageEventArgs *args) noexcept;
    void push_meta_balltze_object_damage_event_args(lua_State *state, Event::ObjectDamageEventArgs *args) noexcept;
    void push_meta_balltze_rcon_message_event_args(lua_State *state, Event::RconMessageEventArgs *args) noexcept;
    void push_meta_balltze_hud_element_bitmap_render_event_args(lua_State *state, Event::HUDElementBitmapRenderEventArgs *args) noexcept;
    void push_meta_balltze_ui_widget_background_render_event_args(lua_State *state, Event::UIWidgetBackgroundRenderEventArgs *args) noexcept;
    void push_meta_balltze_server_connect_event_args(lua_State *state, Event::ServerConnectEventArgs *args) noexcept;
    void push_meta_balltze_sound_playback_event_args(lua_State *state, Event::SoundPlaybackEventArgs *args) noexcept;
    void push_meta_balltze_ui_widget_create_event_args(lua_State *state, Event::UIWidgetCreateEventArgs *args) noexcept;
    void push_meta_balltze_ui_widget_event_args(lua_State *state, Event::UIWidgetEventArgs *args) noexcept;
    void push_meta_balltze_ui_widget_back_event_args(lua_State *state, BalltzeUIWidgetEventArgs *args) noexcept;
    void push_meta_balltze_ui_widget_focus_event_args(lua_State *state, BalltzeUIWidgetEventArgs *args) noexcept;
    void push_meta_balltze_ui_widget_accept_event_args(lua_State *state, BalltzeUIWidgetEventArgs *args) noexcept;
    void push_meta_balltze_ui_widget_sound_event_args(lua_State *state, Event::UIWidgetSoundEventArgs *args) noexcept;
    void push_meta_balltze_ui_widget_list_tab_event_args(lua_State *state, Event::UIWidgetListTabEventArgs *args) noexcept;
    void define_balltze_event_types(lua_State *state) noexcept;
}

#endif
