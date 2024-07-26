// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__PLUGINS__LUA__TYPES__BALLTZE_EVENT_HPP
#define BALLTZE__PLUGINS__LUA__TYPES__BALLTZE_EVENT_HPP

#include <lua.hpp>
#include <balltze/event.hpp>
#include <balltze/engine/netgame_messages.hpp>

namespace Balltze::Plugins::Lua {
    using BalltzeCameraEventContext = Event::CameraEventContext;
    using BalltzeGameInputEventContext = Event::GameInputEventContext;
    using BalltzeKeyboardEventContext = Event::KeyboardInputEventContext;
    using BalltzeHudHoldForActionMessageSlice = Event::HudHoldForActionMessageSlice;
    using BalltzeHudHoldToActionMessageButtonType = Event::HudHoldToActionMessageButton::Type;
    using BalltzeHudHoldToActionMessageAxisDirection = Event::HudHoldToActionMessageButton::AxisDirection;
    using BalltzeHudHoldToActionMessageButton = Event::HudHoldToActionMessageButton;
    using BalltzeHudHoldForActionMessageContext = Event::HudHoldForActionMessageContext;
    using BalltzeMapFileLoadEventContext = Event::MapFileLoadEventContext;
    using BalltzeMapLoadEventContext = Event::MapLoadEventContext;
    using BalltzeNetworkGameChatMessageEventContext = Event::NetworkGameChatMessageEventContext;
    using BalltzeHudChatType = Engine::NetworkGameMessages::HudChatType;
    using BalltzeHudChatMessage = Engine::NetworkGameMessages::HudChat;
    using BalltzeObjectDamageEventContext = Event::ObjectDamageEventContext;
    using BalltzeRconMessageEventContext = Event::RconMessageEventContext;
    using BalltzeHudElementRenderEventContext = Event::HUDElementBitmapRenderEventContext;
    using BalltzeUIWidgetBackgroundRenderEventContext = Event::UIWidgetBackgroundRenderEventContext;
    using BalltzeServerConnectEventContext = Event::ServerConnectEventContext;
    using BalltzeSoundPlaybackEventContext = Event::SoundPlaybackEventContext;
    using BalltzeUIWidgetCreateEventContext = Event::UIWidgetCreateEventContext;
    using BalltzeUIWidgetEventContext = Event::UIWidgetEventContext;
    using BalltzeUIWidgetSoundEventContext = Event::UIWidgetSoundEventContext;
    using BalltzeUIWidgetListTabType = Event::UIWidgetListTabType;
    using BalltzeUIWidgetListTabEventContext = Event::UIWidgetListTabEventContext;
    using BalltzeUIWidgetMouseButtonPressEventContext = Event::UIWidgetMouseButtonPressEventContext;

    void push_meta_balltze_camera_event_context(lua_State *state, Event::CameraEventContext *context) noexcept;
    void push_meta_balltze_game_input_event_context(lua_State *state, Event::GameInputEventContext *context) noexcept;
    void push_meta_balltze_keyboard_input_event_context(lua_State *state, BalltzeKeyboardEventContext *context) noexcept;
    void push_meta_balltze_hud_hold_to_action_message_button(lua_State *state, Event::HudHoldToActionMessageButton *button) noexcept;
    void push_meta_balltze_hud_hold_for_action_message_event_context(lua_State *state, Event::HudHoldForActionMessageContext *context) noexcept;
    void push_meta_balltze_map_file_load_event_context(lua_State *state, Event::MapFileLoadEventContext *context) noexcept;
    void push_meta_balltze_map_load_event_context(lua_State *state, Event::MapLoadEventContext *context) noexcept;
    void push_meta_balltze_network_game_chat_message_event_context(lua_State *state, Event::NetworkGameChatMessageEventContext *context) noexcept;
    void push_meta_balltze_object_damage_event_context(lua_State *state, Event::ObjectDamageEventContext *context) noexcept;
    void push_meta_balltze_rcon_message_event_context(lua_State *state, Event::RconMessageEventContext *context) noexcept;
    void push_meta_balltze_hud_element_bitmap_render_event_context(lua_State *state, Event::HUDElementBitmapRenderEventContext *context) noexcept;
    void push_meta_balltze_ui_widget_background_render_event_context(lua_State *state, Event::UIWidgetBackgroundRenderEventContext *context) noexcept;
    void push_meta_balltze_server_connect_event_context(lua_State *state, Event::ServerConnectEventContext *context) noexcept;
    void push_meta_balltze_sound_playback_event_context(lua_State *state, Event::SoundPlaybackEventContext *context) noexcept;
    void push_meta_balltze_ui_widget_create_event_context(lua_State *state, Event::UIWidgetCreateEventContext *context) noexcept;
    void push_meta_balltze_ui_widget_event_context(lua_State *state, Event::UIWidgetEventContext *context) noexcept;
    void push_meta_balltze_ui_widget_back_event_context(lua_State *state, BalltzeUIWidgetEventContext *context) noexcept;
    void push_meta_balltze_ui_widget_focus_event_context(lua_State *state, BalltzeUIWidgetEventContext *context) noexcept;
    void push_meta_balltze_ui_widget_accept_event_context(lua_State *state, BalltzeUIWidgetEventContext *context) noexcept;
    void push_meta_balltze_ui_widget_sound_event_context(lua_State *state, Event::UIWidgetSoundEventContext *context) noexcept;
    void push_meta_balltze_ui_widget_list_tab_event_context(lua_State *state, Event::UIWidgetListTabEventContext *context) noexcept;
    void push_meta_balltze_ui_widget_mouse_button_press_event_context(lua_State *state, BalltzeUIWidgetMouseButtonPressEventContext *context) noexcept;
    void define_balltze_event_types(lua_State *state) noexcept;
}

#endif
