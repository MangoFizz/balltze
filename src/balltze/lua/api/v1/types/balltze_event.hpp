// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__LUA__API__TYPES__BALLTZE_EVENT_HPP
#define BALLTZE__LUA__API__TYPES__BALLTZE_EVENT_HPP

#include <lua.hpp>
#include <balltze/legacy_api/event.hpp>
#include <balltze/legacy_api/engine/netgame_messages.hpp>

namespace Balltze::Lua::Api::V1 {
    using BalltzeCameraEventContext = LegacyApi::Event::CameraEventContext;
    using BalltzeGameInputEventContext = LegacyApi::Event::GameInputEventContext;
    using BalltzeKeyboardEventContext = LegacyApi::Event::KeyboardInputEventContext;
    using BalltzeHudHoldForActionMessageSlice = LegacyApi::Event::HudHoldForActionMessageSlice;
    using BalltzeHudHoldToActionMessageButtonType = LegacyApi::Event::HudHoldToActionMessageButton::Type;
    using BalltzeHudHoldToActionMessageAxisDirection = LegacyApi::Event::HudHoldToActionMessageButton::AxisDirection;
    using BalltzeHudHoldToActionMessageButton = LegacyApi::Event::HudHoldToActionMessageButton;
    using BalltzeHudHoldForActionMessageContext = LegacyApi::Event::HudHoldForActionMessageContext;
    using BalltzeMapFileLoadEventContext = LegacyApi::Event::MapFileLoadEventContext;
    using BalltzeMapLoadEventContext = LegacyApi::Event::MapLoadEventContext;
    using BalltzeNetworkGameChatMessageEventContext = LegacyApi::Event::NetworkGameChatMessageEventContext;
    using BalltzeHudChatType = LegacyApi::Engine::NetworkGameMessages::HudChatType;
    using BalltzeHudChatMessage = LegacyApi::Engine::NetworkGameMessages::HudChat;
    using BalltzeObjectDamageEventContext = LegacyApi::Event::ObjectDamageEventContext;
    using BalltzeRconMessageEventContext = LegacyApi::Event::RconMessageEventContext;
    using BalltzeHudElementRenderEventContext = LegacyApi::Event::HUDElementBitmapRenderEventContext;
    using BalltzeUIWidgetBackgroundRenderEventContext = LegacyApi::Event::UIWidgetBackgroundRenderEventContext;
    using BalltzeServerConnectEventContext = LegacyApi::Event::ServerConnectEventContext;
    using BalltzeSoundPlaybackEventContext = LegacyApi::Event::SoundPlaybackEventContext;
    using BalltzeUIWidgetCreateEventContext = LegacyApi::Event::UIWidgetCreateEventContext;
    using BalltzeUIWidgetEventContext = LegacyApi::Event::UIWidgetEventContext;
    using BalltzeUIWidgetSoundEventContext = LegacyApi::Event::UIWidgetSoundEventContext;
    using BalltzeUIWidgetListTabType = LegacyApi::Event::UIWidgetListTabType;
    using BalltzeUIWidgetListTabEventContext = LegacyApi::Event::UIWidgetListTabEventContext;
    using BalltzeUIWidgetMouseButtonPressEventContext = LegacyApi::Event::UIWidgetMouseButtonPressEventContext;

    void push_meta_balltze_camera_event_context(lua_State *state, LegacyApi::Event::CameraEventContext *context) noexcept;
    void push_meta_balltze_game_input_event_context(lua_State *state, LegacyApi::Event::GameInputEventContext *context) noexcept;
    void push_meta_balltze_keyboard_input_event_context(lua_State *state, BalltzeKeyboardEventContext *context) noexcept;
    void push_meta_balltze_hud_hold_to_action_message_button(lua_State *state, LegacyApi::Event::HudHoldToActionMessageButton *button) noexcept;
    void push_meta_balltze_hud_hold_for_action_message_event_context(lua_State *state, LegacyApi::Event::HudHoldForActionMessageContext *context) noexcept;
    void push_meta_balltze_map_file_load_event_context(lua_State *state, LegacyApi::Event::MapFileLoadEventContext *context) noexcept;
    void push_meta_balltze_map_load_event_context(lua_State *state, LegacyApi::Event::MapLoadEventContext *context) noexcept;
    void push_meta_balltze_network_game_chat_message_event_context(lua_State *state, LegacyApi::Event::NetworkGameChatMessageEventContext *context) noexcept;
    void push_meta_balltze_object_damage_event_context(lua_State *state, LegacyApi::Event::ObjectDamageEventContext *context) noexcept;
    void push_meta_balltze_rcon_message_event_context(lua_State *state, LegacyApi::Event::RconMessageEventContext *context) noexcept;
    void push_meta_balltze_hud_element_bitmap_render_event_context(lua_State *state, LegacyApi::Event::HUDElementBitmapRenderEventContext *context) noexcept;
    void push_meta_balltze_ui_widget_background_render_event_context(lua_State *state, LegacyApi::Event::UIWidgetBackgroundRenderEventContext *context) noexcept;
    void push_meta_balltze_server_connect_event_context(lua_State *state, LegacyApi::Event::ServerConnectEventContext *context) noexcept;
    void push_meta_balltze_sound_playback_event_context(lua_State *state, LegacyApi::Event::SoundPlaybackEventContext *context) noexcept;
    void push_meta_balltze_ui_widget_create_event_context(lua_State *state, LegacyApi::Event::UIWidgetCreateEventContext *context) noexcept;
    void push_meta_balltze_ui_widget_event_context(lua_State *state, LegacyApi::Event::UIWidgetEventContext *context) noexcept;
    void push_meta_balltze_ui_widget_back_event_context(lua_State *state, BalltzeUIWidgetEventContext *context) noexcept;
    void push_meta_balltze_ui_widget_focus_event_context(lua_State *state, BalltzeUIWidgetEventContext *context) noexcept;
    void push_meta_balltze_ui_widget_accept_event_context(lua_State *state, BalltzeUIWidgetEventContext *context) noexcept;
    void push_meta_balltze_ui_widget_sound_event_context(lua_State *state, LegacyApi::Event::UIWidgetSoundEventContext *context) noexcept;
    void push_meta_balltze_ui_widget_list_tab_event_context(lua_State *state, LegacyApi::Event::UIWidgetListTabEventContext *context) noexcept;
    void push_meta_balltze_ui_widget_mouse_button_press_event_context(lua_State *state, BalltzeUIWidgetMouseButtonPressEventContext *context) noexcept;
    void define_balltze_event_types(lua_State *state) noexcept;
}

#endif
