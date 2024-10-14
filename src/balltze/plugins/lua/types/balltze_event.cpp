// SPDX-License-Identifier: GPL-3.0-only

#include "../helpers/luacstruct.hpp"
#include "../helpers/enum.hpp"
#include "engine_types.hpp"
#include "engine_rasterizer.hpp"
#include "engine_user_interface.hpp"
#include "engine_tag_data.hpp"
#include "balltze_event.hpp"

namespace Balltze::Event {
    extern std::string ip_address_int_to_string(std::uint32_t address_int) noexcept;
}

namespace Balltze::Plugins::Lua {
    using namespace Event;

    static void define_balltze_camera_event_context_struct(lua_State *state) noexcept {
        luacs_newstruct(state, BalltzeCameraEventContext);
        luacs_objref_field(state, BalltzeCameraEventContext, EngineCameraData, camera, LUACS_FREADONLY);
        luacs_enum_field(state, BalltzeCameraEventContext, EngineCameraType, type, LUACS_FREADONLY);
        lua_pop(state, 1);
    }

    void push_meta_balltze_camera_event_context(lua_State *state, BalltzeCameraEventContext *context) noexcept {
        luacs_newobject(state, BalltzeCameraEventContext, context);
    }

    /**
     * @todo Add gamepad button enum to game input event arguments
     */

    static void define_balltze_game_input_event_context_struct(lua_State *state) noexcept {
        luacs_newstruct(state, BalltzeGameInputEventContext);
        luacs_enum_field(state, BalltzeGameInputEventContext, EngineInputDevice, device, LUACS_FREADONLY);
        luacs_bool_field(state, BalltzeGameInputEventContext, mapped, LUACS_FREADONLY);
        luacs_declare_field(state, LUACS_TUINT32, NULL, "keyCode", sizeof(((BalltzeGameInputEventContext *)0)->button.key_code), OFFSET_OF(BalltzeGameInputEventContext, button.key_code), 0, 0);
        lua_pop(state, 1);
    }

    void push_meta_balltze_game_input_event_context(lua_State *state, BalltzeGameInputEventContext *context) noexcept {
        luacs_newobject(state, BalltzeGameInputEventContext, context);
    }

    static void define_balltze_keyboard_input_event_context_struct(lua_State *state) noexcept {
        luacs_newstruct(state, BalltzeKeyboardEventContext);
        luacs_nested_field(state, BalltzeKeyboardEventContext, EngineInputBufferedKey, key, LUACS_FREADONLY);
        lua_pop(state, 1);
    }

    void push_meta_balltze_keyboard_input_event_context(lua_State *state, BalltzeKeyboardEventContext *context) noexcept {
        luacs_newobject(state, BalltzeKeyboardEventContext, context);
    }

    static void define_hud_hold_for_action_message_slice_enum(lua_State *state) noexcept {
        luacs_newenum(state, BalltzeHudHoldForActionMessageSlice);
        luacs_enum_declare_value(state, "MESSAGE", BalltzeHudHoldForActionMessageSlice::MESSAGE);
        luacs_enum_declare_value(state, "BUTTON_NAME_LEFT_QUOTE", BalltzeHudHoldForActionMessageSlice::BUTTON_NAME_LEFT_QUOTE);
        luacs_enum_declare_value(state, "BUTTON_NAME_RIGHT_QUOTE", BalltzeHudHoldForActionMessageSlice::BUTTON_NAME_RIGHT_QUOTE);
        luacs_enum_declare_value(state, "BUTTON_NAME", BalltzeHudHoldForActionMessageSlice::BUTTON_NAME);
        luacs_enum_declare_value(state, "WEAPON_ICON", BalltzeHudHoldForActionMessageSlice::WEAPON_ICON);
        lua_pop(state, 1);
    }

    static void define_balltze_event_hud_message_button_type_enum(lua_State *state) noexcept {
        luacs_newenum(state, BalltzeHudHoldToActionMessageButtonType);
        luacs_enum_declare_value(state, "BUTTON", BalltzeHudHoldToActionMessageButton::Type::BUTTON);
        luacs_enum_declare_value(state, "AXIS", BalltzeHudHoldToActionMessageButton::Type::AXIS);
        lua_pop(state, 1);
    }

    static void define_balltze_event_hud_message_axis_direction_enum(lua_State *state) noexcept {
        luacs_newenum(state, BalltzeHudHoldToActionMessageAxisDirection);
        luacs_enum_declare_value(state, "POSITIVE", BalltzeHudHoldToActionMessageAxisDirection::POSITIVE);
        luacs_enum_declare_value(state, "NEGATIVE", BalltzeHudHoldToActionMessageAxisDirection::NEGATIVE);
        lua_pop(state, 1);
    }

    static void define_balltze_hud_hold_to_action_message_button_struct(lua_State *state) noexcept {
        luacs_newstruct(state, BalltzeHudHoldToActionMessageButton);
        luacs_enum_field(state, BalltzeHudHoldToActionMessageButton, EngineInputDevice, device, LUACS_FREADONLY);
        luacs_enum_field(state, BalltzeHudHoldToActionMessageButton, BalltzeHudHoldToActionMessageButtonType, type, LUACS_FREADONLY);
        luacs_unsigned_field(state, BalltzeHudHoldToActionMessageButton, index, LUACS_FREADONLY);
        luacs_enum_field(state, BalltzeHudHoldToActionMessageButton, BalltzeHudHoldToActionMessageAxisDirection, axis_direction, LUACS_FREADONLY);
        lua_pop(state, 1); 
    }

    void push_meta_balltze_hud_hold_to_action_message_button(lua_State *state, BalltzeHudHoldToActionMessageButton *button) noexcept {
        luacs_newobject(state, BalltzeHudHoldToActionMessageButton, button);
    }

    static void define_balltze_hud_hold_for_action_message_context_struct(lua_State *state) noexcept {
        luacs_newstruct(state, BalltzeHudHoldForActionMessageContext);
        luacs_enum_field(state, BalltzeHudHoldForActionMessageContext, BalltzeHudHoldForActionMessageSlice, slice, LUACS_FREADONLY);
        luacs_nested_field(state, BalltzeHudHoldForActionMessageContext, EnginePoint2DInt, offset, 0);
        luacs_nested_field(state, BalltzeHudHoldForActionMessageContext, EngineColorARGBInt, color, LUACS_FREADONLY);
        luacs_string_field(state, BalltzeHudHoldForActionMessageContext, text, 0);
        luacs_declare_method(state, "button", +[](lua_State *state) -> int {
            auto *self = luacs_to_object(state, 1, BalltzeHudHoldForActionMessageContext);
            if(self->button.has_value()) {
                push_meta_balltze_hud_hold_to_action_message_button(state, const_cast<BalltzeHudHoldToActionMessageButton *>(&self->button.value()));
            } 
            else {
                lua_pushnil(state);
            }
            return 1;
        });
        lua_pop(state, 1); 
    }

    void push_meta_balltze_hud_hold_for_action_message_event_context(lua_State *state, BalltzeHudHoldForActionMessageContext *context) noexcept {
        luacs_newobject(state, BalltzeHudHoldForActionMessageContext, context);
    }

    static void define_balltze_map_file_load_event_context_struct(lua_State *state) noexcept {
        luacs_newstruct(state, BalltzeMapFileLoadEventContext);
        luacs_declare_method(state, "mapName", +[](lua_State *state) -> int {
            auto *self = luacs_to_object(state, 1, BalltzeMapFileLoadEventContext);
            lua_pushstring(state, self->map_name.c_str());
            return 1;
        });
        luacs_declare_method(state, "mapPath", +[](lua_State *state) -> int {
            auto *self = luacs_to_object(state, 1, BalltzeMapFileLoadEventContext);
            lua_pushstring(state, self->map_path.c_str());
            return 1;
        });
        lua_pop(state, 1);
    }

    void push_meta_balltze_map_file_load_event_context(lua_State *state, BalltzeMapFileLoadEventContext *context) noexcept {
        luacs_newobject(state, BalltzeMapFileLoadEventContext, context);
    }

    static void define_balltze_map_load_event_context_struct(lua_State *state) noexcept {
        luacs_newstruct(state, BalltzeMapLoadEventContext);
        luacs_declare_method(state, "mapName", [](lua_State *state) -> int {
            auto *self = luacs_to_object(state, 1, BalltzeMapLoadEventContext);
            if(!self) {
                return luaL_error(state, "self is null");
            }
            lua_pushstring(state, self->name.c_str());
            return 1;
        });
        lua_pop(state, 1);
    }

    void push_meta_balltze_map_load_event_context(lua_State *state, BalltzeMapLoadEventContext *context) noexcept {
        luacs_newobject(state, BalltzeMapLoadEventContext, context);
    }

    void define_balltze_hud_chat_type_enum(lua_State *state) noexcept {
        luacs_newenum(state, BalltzeHudChatType);
        luacs_enum_declare_value(state, "NONE", BalltzeHudChatType::NONE);
        luacs_enum_declare_value(state, "ALL", BalltzeHudChatType::ALL);
        luacs_enum_declare_value(state, "TEAM", BalltzeHudChatType::TEAM);
        luacs_enum_declare_value(state, "VEHICLE", BalltzeHudChatType::VEHICLE);
        luacs_enum_declare_value(state, "CUSTOM", BalltzeHudChatType::CUSTOM);
        lua_pop(state, 1);
    }

    static void define_balltze_hud_chat_struct(lua_State *state) noexcept {
        luacs_newstruct(state, BalltzeHudChatMessage);
        luacs_enum_field(state, BalltzeHudChatMessage, BalltzeHudChatType, msg_type, LUACS_FREADONLY);
        luacs_int_field(state, BalltzeHudChatMessage, player_id, LUACS_FREADONLY);
        luacs_string_field(state, BalltzeHudChatMessage, message, LUACS_FREADONLY);
        lua_pop(state, 1); 
    }


    static void define_balltze_network_game_chat_message_event_context_struct(lua_State *state) noexcept {
        luacs_newstruct(state, BalltzeNetworkGameChatMessageEventContext);
        luacs_objref_field(state, BalltzeNetworkGameChatMessageEventContext, BalltzeHudChatMessage, chat_message, LUACS_FREADONLY);
        lua_pop(state, 1);
    }

    void push_meta_balltze_network_game_chat_message_event_context(lua_State *state, BalltzeNetworkGameChatMessageEventContext *context) noexcept {
        luacs_newobject(state, BalltzeNetworkGameChatMessageEventContext, context);
    }

    static void define_balltze_object_damage_event_context_struct(lua_State *state) noexcept {
        luacs_newstruct(state, BalltzeObjectDamageEventContext);
        luacs_nested_field(state, BalltzeObjectDamageEventContext, EngineResourceHandle, object, LUACS_FREADONLY);
        luacs_nested_field(state, BalltzeObjectDamageEventContext, EngineResourceHandle, damage_effect, LUACS_FREADONLY);
        luacs_float_field(state, BalltzeObjectDamageEventContext, multiplier, LUACS_FREADONLY);
        luacs_nested_field(state, BalltzeObjectDamageEventContext, EngineResourceHandle, causer_player, LUACS_FREADONLY);
        luacs_nested_field(state, BalltzeObjectDamageEventContext, EngineResourceHandle, causer_object, LUACS_FREADONLY);
        lua_pop(state, 1);
    }

    void push_meta_balltze_object_damage_event_context(lua_State *state, BalltzeObjectDamageEventContext *context) noexcept {
        luacs_newobject(state, BalltzeObjectDamageEventContext, context);
    }

    static void define_balltze_rcon_message_event_context_struct(lua_State *state) noexcept {
        luacs_newstruct(state, BalltzeRconMessageEventContext);
        luacs_declare_method(state, "message", +[](lua_State *state) -> int {
            auto *self = luacs_to_object(state, 1, BalltzeRconMessageEventContext);
            lua_pushstring(state, self->message->c_str());
            return 1;
        });
        luacs_string_field(state, BalltzeRconMessageEventContext, raw_message, 0);
        lua_pop(state, 1);
    }

    void push_meta_balltze_rcon_message_event_context(lua_State *state, BalltzeRconMessageEventContext *context) noexcept {
        luacs_newobject(state, BalltzeRconMessageEventContext, context);
    }

    static void define_balltze_hud_element_render_event_context(lua_State *state) noexcept {
        luacs_newstruct(state, BalltzeHudElementRenderEventContext);
        luacs_objref_field(state, BalltzeHudElementRenderEventContext, EngineBitmapData, bitmap_data, 0);
        luacs_nested_field(state, BalltzeHudElementRenderEventContext, EngineRasterizerQuad, quad, LUACS_FREADONLY);
        lua_pop(state, 1);
    }

    void push_meta_balltze_hud_element_bitmap_render_event_context(lua_State *state, BalltzeHudElementRenderEventContext *context) noexcept {
        luacs_newobject(state, BalltzeHudElementRenderEventContext, context);
    }

    static void define_balltze_ui_widget_background_render_event_context(lua_State *state) noexcept {
        luacs_newstruct(state, BalltzeUIWidgetBackgroundRenderEventContext);
        luacs_objref_field(state, BalltzeUIWidgetBackgroundRenderEventContext, EngineUIWidget, widget, LUACS_FREADONLY);
        luacs_nested_field(state, BalltzeUIWidgetBackgroundRenderEventContext, EngineRasterizerQuad, quad, LUACS_FREADONLY);
        lua_pop(state, 1);
    }

    void push_meta_balltze_ui_widget_background_render_event_context(lua_State *state, BalltzeUIWidgetBackgroundRenderEventContext *context) noexcept {
        luacs_newobject(state, BalltzeUIWidgetBackgroundRenderEventContext, context);
    }

    static void define_balltze_server_connect_event_context(lua_State *state) noexcept {
        luacs_newstruct(state, BalltzeServerConnectEventContext);
        luacs_unsigned_field(state, BalltzeServerConnectEventContext, address, 0);
        luacs_unsigned_field(state, BalltzeServerConnectEventContext, port, 0);
        luacs_extref_field(state, BalltzeServerConnectEventContext, password, 0);
        luacs_declare_method(state, "formattedAddress", +[](lua_State *state) -> int {
            auto *self = luacs_to_object(state, 1, BalltzeServerConnectEventContext);
            lua_pushstring(state, Event::ip_address_int_to_string(self->address).c_str());
            return 1;
        });
        lua_pop(state, 1);
    }

    void push_meta_balltze_server_connect_event_context(lua_State *state, BalltzeServerConnectEventContext *context) noexcept {
        luacs_newobject(state, BalltzeServerConnectEventContext, context);
    }

    static void define_balltze_sound_playback_event_context(lua_State *state) noexcept {
        luacs_newstruct(state, BalltzeSoundPlaybackEventContext);
        luacs_objref_field(state, BalltzeSoundPlaybackEventContext, EngineSound, sound, LUACS_FREADONLY);
        luacs_objref_field(state, BalltzeSoundPlaybackEventContext, EngineSoundPermutation, permutation, LUACS_FREADONLY);
        lua_pop(state, 1);
    }

    void push_meta_balltze_sound_playback_event_context(lua_State *state, BalltzeSoundPlaybackEventContext *context) noexcept {
        luacs_newobject(state, BalltzeSoundPlaybackEventContext, context);
    }

    static void define_balltze_ui_widget_create_event_context(lua_State *state) noexcept {
        luacs_newstruct(state, BalltzeUIWidgetCreateEventContext);
        luacs_objref_field(state, BalltzeUIWidgetCreateEventContext, EngineUIWidget, widget, LUACS_FREADONLY);
        luacs_nested_field(state, BalltzeUIWidgetCreateEventContext, EngineResourceHandle, definition_tag_handle, LUACS_FREADONLY);
        luacs_bool_field(state, BalltzeUIWidgetCreateEventContext, is_root_widget, LUACS_FREADONLY);
        luacs_objref_field(state, BalltzeUIWidgetCreateEventContext, EngineUIWidget, parent_widget, LUACS_FREADONLY);
        lua_pop(state, 1);
    }

    void push_meta_balltze_ui_widget_create_event_context(lua_State *state, BalltzeUIWidgetCreateEventContext *context) noexcept {
        luacs_newobject(state, BalltzeUIWidgetCreateEventContext, context);
    }

    static void define_balltze_ui_widget_event_context(lua_State *state) noexcept {
        luacs_newstruct(state, BalltzeUIWidgetEventContext);
        luacs_objref_field(state, BalltzeUIWidgetEventContext, EngineUIWidget, widget, LUACS_FREADONLY);
        lua_pop(state, 1);
    }

    void push_meta_balltze_ui_widget_event_context(lua_State *state, BalltzeUIWidgetEventContext *context) noexcept {
        luacs_newobject(state, BalltzeUIWidgetEventContext, context);
    }

    void push_meta_balltze_ui_widget_back_event_context(lua_State *state, BalltzeUIWidgetEventContext *context) noexcept {
        luacs_newobject(state, BalltzeUIWidgetEventContext, context);
    }

    void push_meta_balltze_ui_widget_focus_event_context(lua_State *state, BalltzeUIWidgetEventContext *context) noexcept {
        luacs_newobject(state, BalltzeUIWidgetEventContext, context);
    }

    void push_meta_balltze_ui_widget_accept_event_context(lua_State *state, BalltzeUIWidgetEventContext *context) noexcept {
        luacs_newobject(state, BalltzeUIWidgetEventContext, context);
    }

    static void define_balltze_ui_widget_sound_event_context(lua_State *state) noexcept {
        luacs_newstruct(state, BalltzeUIWidgetSoundEventContext);
        luacs_enum_field(state, BalltzeUIWidgetSoundEventContext, EngineWidgetNavigationSound, sound, LUACS_FREADONLY);
        lua_pop(state, 1);
    }

    void push_meta_balltze_ui_widget_sound_event_context(lua_State *state, BalltzeUIWidgetSoundEventContext *context) noexcept {
        luacs_newobject(state, BalltzeUIWidgetSoundEventContext, context);
    }

    static void define_balltze_ui_widget_list_tab_event_type_enum(lua_State *state) noexcept {
        luacs_newenum(state, BalltzeUIWidgetListTabType);
        luacs_enum_declare_value(state, "tabThruChildrenNextHorizontal", BalltzeUIWidgetListTabType::TAB_THRU_CHILDREN_NEXT_HORIZONTAL);
        luacs_enum_declare_value(state, "tabThruChildrenNextVertical", BalltzeUIWidgetListTabType::TAB_THRU_CHILDREN_NEXT_VERTICAL);
        luacs_enum_declare_value(state, "tabThruChildrenPrev", BalltzeUIWidgetListTabType::TAB_THRU_CHILDREN_PREV);
        luacs_enum_declare_value(state, "tabThruItemListItemsNextHorizontal", BalltzeUIWidgetListTabType::TAB_THRU_ITEM_LIST_ITEMS_NEXT_HORIZONTAL);
        luacs_enum_declare_value(state, "tabThruItemListItemsNextVertical", BalltzeUIWidgetListTabType::TAB_THRU_ITEM_LIST_ITEMS_NEXT_VERTICAL);
        luacs_enum_declare_value(state, "tabThruItemListItemsPrevHorizontal", BalltzeUIWidgetListTabType::TAB_THRU_ITEM_LIST_ITEMS_PREV_HORIZONTAL);
        luacs_enum_declare_value(state, "tabThruItemListItemsPrevVertical", BalltzeUIWidgetListTabType::TAB_THRU_ITEM_LIST_ITEMS_PREV_VERTICAL);
        publish_enum(state, "Balltze", "event", "uiWidgetListTabTypes", -1);
        lua_pop(state, 1);
    }

    static void define_balltze_ui_widget_list_tab_event_context_struct(lua_State *state) noexcept {
        luacs_newstruct(state, BalltzeUIWidgetListTabEventContext);
        luacs_objref_field(state, BalltzeUIWidgetListTabEventContext, EngineUIWidget, widget_list, LUACS_FREADONLY);
        luacs_enum_field(state, BalltzeUIWidgetListTabEventContext, BalltzeUIWidgetListTabType, tab, LUACS_FREADONLY);
        lua_pop(state, 1);
    }

    void push_meta_balltze_ui_widget_list_tab_event_context(lua_State *state, BalltzeUIWidgetListTabEventContext *context) noexcept {
        luacs_newobject(state, BalltzeUIWidgetListTabEventContext, context);
    }

    static void define_balltze_ui_widget_mouse_button_press_event_context_struct(lua_State *state) noexcept {
        luacs_newstruct(state, BalltzeUIWidgetMouseButtonPressEventContext);
        luacs_objref_field(state, BalltzeUIWidgetMouseButtonPressEventContext, EngineUIWidget, widget, LUACS_FREADONLY);
        luacs_enum_field(state, BalltzeUIWidgetMouseButtonPressEventContext, EngineInputMouseButton, button, LUACS_FREADONLY);
        lua_pop(state, 1);
    }

    void push_meta_balltze_ui_widget_mouse_button_press_event_context(lua_State *state, BalltzeUIWidgetMouseButtonPressEventContext *context) noexcept {
        luacs_newobject(state, BalltzeUIWidgetMouseButtonPressEventContext, context);
    }

    void define_balltze_event_types(lua_State *state) noexcept {
        define_balltze_camera_event_context_struct(state);
        define_balltze_game_input_event_context_struct(state);
        define_balltze_keyboard_input_event_context_struct(state);
        define_hud_hold_for_action_message_slice_enum(state);
        define_balltze_event_hud_message_button_type_enum(state);
        define_balltze_event_hud_message_axis_direction_enum(state);
        define_balltze_hud_hold_to_action_message_button_struct(state);
        define_balltze_hud_hold_for_action_message_context_struct(state);
        define_balltze_map_file_load_event_context_struct(state);
        define_balltze_map_load_event_context_struct(state);
        define_balltze_hud_chat_type_enum(state);
        define_balltze_hud_chat_struct(state);
        define_balltze_network_game_chat_message_event_context_struct(state);
        define_balltze_object_damage_event_context_struct(state);
        define_balltze_rcon_message_event_context_struct(state);
        define_balltze_hud_element_render_event_context(state);
        define_balltze_ui_widget_background_render_event_context(state);
        define_balltze_server_connect_event_context(state);
        define_balltze_sound_playback_event_context(state);
        define_balltze_ui_widget_create_event_context(state);
        define_balltze_ui_widget_event_context(state);
        define_balltze_ui_widget_sound_event_context(state);
        define_balltze_ui_widget_list_tab_event_type_enum(state);
        define_balltze_ui_widget_list_tab_event_context_struct(state);
        define_balltze_ui_widget_mouse_button_press_event_context_struct(state);
    }
}
