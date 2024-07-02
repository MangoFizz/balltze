// SPDX-License-Identifier: GPL-3.0-only

#include "../helpers/luacstruct.hpp"
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

    static void define_balltze_camera_event_args_struct(lua_State *state) noexcept {
        luacs_newstruct(state, BalltzeCameraEventArgs);
        luacs_objref_field(state, BalltzeCameraEventArgs, EngineCameraData, camera, LUACS_FREADONLY);
        luacs_enum_field(state, BalltzeCameraEventArgs, EngineCameraType, type, LUACS_FREADONLY);
        lua_pop(state, 1);
    }

    void push_meta_balltze_camera_event_args(lua_State *state, BalltzeCameraEventArgs *args) noexcept {
        luacs_newobject(state, BalltzeCameraEventArgs, args);
    }

    /**
     * @todo Add gamepad button enum to game input event arguments
     */

    static void define_balltze_game_input_event_args_struct(lua_State *state) noexcept {
        luacs_newstruct(state, BalltzeGameInputEventArgs);
        luacs_enum_field(state, BalltzeGameInputEventArgs, EngineInputDevice, device, LUACS_FREADONLY);
        luacs_bool_field(state, BalltzeGameInputEventArgs, mapped, LUACS_FREADONLY);
        luacs_declare_field(state, LUACS_TUINT32, NULL, "keyCode", sizeof(((BalltzeGameInputEventArgs *)0)->button.key_code), OFFSET_OF(BalltzeGameInputEventArgs, button.key_code), 0, 0);
        lua_pop(state, 1);
    }

    void push_meta_balltze_game_input_event_args(lua_State *state, BalltzeGameInputEventArgs *args) noexcept {
        luacs_newobject(state, BalltzeGameInputEventArgs, args);
    }

    static void define_balltze_keyboard_input_event_args_struct(lua_State *state) noexcept {
        luacs_newstruct(state, BalltzeKeyboardEventArgs);
        luacs_objref_field(state, BalltzeKeyboardEventArgs, EngineInputBufferedKey, key, LUACS_FREADONLY);
        lua_pop(state, 1);
    }

    void push_meta_balltze_keyboard_input_event_args(lua_State *state, BalltzeKeyboardEventArgs *args) noexcept {
        luacs_newobject(state, BalltzeKeyboardEventArgs, args);
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

    static void define_balltze_hud_hold_for_action_message_args_struct(lua_State *state) noexcept {
        luacs_newstruct(state, BalltzeHudHoldForActionMessageArgs);
        luacs_enum_field(state, BalltzeHudHoldForActionMessageArgs, BalltzeHudHoldForActionMessageSlice, slice, LUACS_FREADONLY);
        luacs_nested_field(state, BalltzeHudHoldForActionMessageArgs, EnginePoint2DInt, offset, 0);
        luacs_nested_field(state, BalltzeHudHoldForActionMessageArgs, EngineColorARGBInt, color, LUACS_FREADONLY);
        luacs_string_field(state, BalltzeHudHoldForActionMessageArgs, text, 0);
        luacs_declare_method(state, "button", +[](lua_State *state) -> int {
            auto *self = luacs_to_object(state, 1, BalltzeHudHoldForActionMessageArgs);
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

    void push_meta_balltze_hud_hold_for_action_message_event_args(lua_State *state, BalltzeHudHoldForActionMessageArgs *args) noexcept {
        luacs_newobject(state, BalltzeHudHoldForActionMessageArgs, args);
    }

    static void define_balltze_map_file_load_event_args_struct(lua_State *state) noexcept {
        luacs_newstruct(state, BalltzeMapFileLoadEventArgs);
        luacs_declare_method(state, "mapName", +[](lua_State *state) -> int {
            auto *self = luacs_to_object(state, 1, BalltzeMapFileLoadEventArgs);
            lua_pushstring(state, self->map_name.c_str());
            return 1;
        });
        luacs_declare_method(state, "mapPath", +[](lua_State *state) -> int {
            auto *self = luacs_to_object(state, 1, BalltzeMapFileLoadEventArgs);
            lua_pushstring(state, self->map_path.c_str());
            return 1;
        });
        lua_pop(state, 1);
    }

    void push_meta_balltze_map_file_load_event_args(lua_State *state, BalltzeMapFileLoadEventArgs *args) noexcept {
        luacs_newobject(state, BalltzeMapFileLoadEventArgs, args);
    }

    static void define_balltze_map_load_event_args_struct(lua_State *state) noexcept {
        luacs_newstruct(state, BalltzeMapLoadEventArgs);
        luacs_declare_method(state, "mapName", [](lua_State *state) -> int {
            auto *self = luacs_to_object(state, 1, BalltzeMapLoadEventArgs);
            if(!self) {
                return luaL_error(state, "self is null");
            }
            lua_pushstring(state, self->name.c_str());
            return 1;
        });
        lua_pop(state, 1);
    }

    void push_meta_balltze_map_load_event_args(lua_State *state, BalltzeMapLoadEventArgs *args) noexcept {
        luacs_newobject(state, BalltzeMapLoadEventArgs, args);
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


    static void define_balltze_network_game_chat_message_event_args_struct(lua_State *state) noexcept {
        luacs_newstruct(state, BalltzeNetworkGameChatMessageEventArgs);
        luacs_objref_field(state, BalltzeNetworkGameChatMessageEventArgs, BalltzeHudChatMessage, chat_message, LUACS_FREADONLY);
        lua_pop(state, 1);
    }

    void push_meta_balltze_network_game_chat_message_event_args(lua_State *state, BalltzeNetworkGameChatMessageEventArgs *args) noexcept {
        luacs_newobject(state, BalltzeNetworkGameChatMessageEventArgs, args);
    }

    static void define_balltze_object_damage_event_args_struct(lua_State *state) noexcept {
        luacs_newstruct(state, BalltzeObjectDamageEventArgs);
        luacs_nested_field(state, BalltzeObjectDamageEventArgs, EngineResourceHandle, object, LUACS_FREADONLY);
        luacs_nested_field(state, BalltzeObjectDamageEventArgs, EngineResourceHandle, damage_effect, LUACS_FREADONLY);
        luacs_float_field(state, BalltzeObjectDamageEventArgs, multiplier, LUACS_FREADONLY);
        luacs_nested_field(state, BalltzeObjectDamageEventArgs, EngineResourceHandle, causer_player, LUACS_FREADONLY);
        luacs_nested_field(state, BalltzeObjectDamageEventArgs, EngineResourceHandle, causer_object, LUACS_FREADONLY);
        lua_pop(state, 1);
    }

    void push_meta_balltze_object_damage_event_args(lua_State *state, BalltzeObjectDamageEventArgs *args) noexcept {
        luacs_newobject(state, BalltzeObjectDamageEventArgs, args);
    }

    static void define_balltze_rcon_message_event_args_struct(lua_State *state) noexcept {
        luacs_newstruct(state, BalltzeRconMessageEventArgs);
        luacs_declare_method(state, "message", +[](lua_State *state) -> int {
            auto *self = luacs_to_object(state, 1, BalltzeRconMessageEventArgs);
            lua_pushstring(state, self->message.c_str());
            return 1;
        });
        lua_pop(state, 1);
    }

    void push_meta_balltze_rcon_message_event_args(lua_State *state, BalltzeRconMessageEventArgs *args) noexcept {
        luacs_newobject(state, BalltzeRconMessageEventArgs, args);
    }

    static void define_balltze_ui_widget_render_vertex(lua_State *state) noexcept {
        luacs_newstruct(state, BalltzeUIWidgetRenderVertex);
        luacs_float_field(state, BalltzeUIWidgetRenderVertex, x, 0);
        luacs_float_field(state, BalltzeUIWidgetRenderVertex, y, 0);
        luacs_float_field(state, BalltzeUIWidgetRenderVertex, z, 0);
        luacs_float_field(state, BalltzeUIWidgetRenderVertex, rhw, 0);
        luacs_float_field(state, BalltzeUIWidgetRenderVertex, u, 0);
        luacs_float_field(state, BalltzeUIWidgetRenderVertex, v, 0);
        lua_pop(state, 1);
    }

    static void define_balltze_ui_widget_render_vertices(lua_State *state) noexcept {
        luacs_newstruct(state, BalltzeUIWidgetRenderVertices);
        luacs_nested_field(state, BalltzeUIWidgetRenderVertices, BalltzeUIWidgetRenderVertex, top_left, 0);
        luacs_nested_field(state, BalltzeUIWidgetRenderVertices, BalltzeUIWidgetRenderVertex, top_right, 0);
        luacs_nested_field(state, BalltzeUIWidgetRenderVertices, BalltzeUIWidgetRenderVertex, bottom_right, 0);
        luacs_nested_field(state, BalltzeUIWidgetRenderVertices, BalltzeUIWidgetRenderVertex, bottom_left, 0);
        lua_pop(state, 1);
    }

    static void define_balltze_hud_element_render_event_args(lua_State *state) noexcept {
        luacs_newstruct(state, BalltzeHudElementRenderEventArgs);
        luacs_objref_field(state, BalltzeHudElementRenderEventArgs, EngineBitmapData, bitmap_data, 0);
        luacs_nested_field(state, BalltzeHudElementRenderEventArgs, BalltzeUIWidgetRenderVertices, vertices, LUACS_FREADONLY);
        lua_pop(state, 1);
    }

    void push_meta_balltze_hud_element_bitmap_render_event_args(lua_State *state, BalltzeHudElementRenderEventArgs *args) noexcept {
        luacs_newobject(state, BalltzeHudElementRenderEventArgs, args);
    }

    static void define_balltze_ui_widget_background_render_event_args(lua_State *state) noexcept {
        luacs_newstruct(state, BalltzeUIWidgetBackgroundRenderEventArgs);
        luacs_objref_field(state, BalltzeUIWidgetBackgroundRenderEventArgs, EngineUIWidget, widget, LUACS_FREADONLY);
        luacs_nested_field(state, BalltzeUIWidgetBackgroundRenderEventArgs, BalltzeUIWidgetRenderVertices, vertices, LUACS_FREADONLY);
        lua_pop(state, 1);
    }

    void push_meta_balltze_ui_widget_background_render_event_args(lua_State *state, BalltzeUIWidgetBackgroundRenderEventArgs *args) noexcept {
        luacs_newobject(state, BalltzeUIWidgetBackgroundRenderEventArgs, args);
    }

    static void define_balltze_server_connect_event_args(lua_State *state) noexcept {
        luacs_newstruct(state, BalltzeServerConnectEventArgs);
        luacs_unsigned_field(state, BalltzeServerConnectEventArgs, address, 0);
        luacs_unsigned_field(state, BalltzeServerConnectEventArgs, port, 0);
        luacs_extref_field(state, BalltzeServerConnectEventArgs, password, 0);
        luacs_declare_method(state, "formattedAddress", +[](lua_State *state) -> int {
            auto *self = luacs_to_object(state, 1, BalltzeServerConnectEventArgs);
            lua_pushstring(state, Event::ip_address_int_to_string(self->address).c_str());
            return 1;
        });
        lua_pop(state, 1);
    }

    void push_meta_balltze_server_connect_event_args(lua_State *state, BalltzeServerConnectEventArgs *args) noexcept {
        luacs_newobject(state, BalltzeServerConnectEventArgs, args);
    }

    static void define_balltze_sound_playback_event_args(lua_State *state) noexcept {
        luacs_newstruct(state, BalltzeSoundPlaybackEventArgs);
        luacs_objref_field(state, BalltzeSoundPlaybackEventArgs, EngineSound, sound, LUACS_FREADONLY);
        luacs_objref_field(state, BalltzeSoundPlaybackEventArgs, EngineSoundPermutation, permutation, LUACS_FREADONLY);
        lua_pop(state, 1);
    }

    void push_meta_balltze_sound_playback_event_args(lua_State *state, BalltzeSoundPlaybackEventArgs *args) noexcept {
        luacs_newobject(state, BalltzeSoundPlaybackEventArgs, args);
    }

    static void define_balltze_ui_widget_create_event_args(lua_State *state) noexcept {
        luacs_newstruct(state, BalltzeUIWidgetCreateEventArgs);
        luacs_objref_field(state, BalltzeUIWidgetCreateEventArgs, EngineUIWidget, widget, LUACS_FREADONLY);
        luacs_nested_field(state, BalltzeUIWidgetCreateEventArgs, EngineResourceHandle, definition_tag_handle, LUACS_FREADONLY);
        luacs_bool_field(state, BalltzeUIWidgetCreateEventArgs, is_root_widget, LUACS_FREADONLY);
        luacs_objref_field(state, BalltzeUIWidgetCreateEventArgs, EngineUIWidget, parent_widget, LUACS_FREADONLY);
        lua_pop(state, 1);
    }

    void push_meta_balltze_ui_widget_create_event_args(lua_State *state, BalltzeUIWidgetCreateEventArgs *args) noexcept {
        luacs_newobject(state, BalltzeUIWidgetCreateEventArgs, args);
    }

    static void define_balltze_ui_widget_event_args(lua_State *state) noexcept {
        luacs_newstruct(state, BalltzeUIWidgetEventArgs);
        luacs_objref_field(state, BalltzeUIWidgetEventArgs, EngineUIWidget, widget, LUACS_FREADONLY);
        lua_pop(state, 1);
    }

    void push_meta_balltze_ui_widget_event_args(lua_State *state, BalltzeUIWidgetEventArgs *args) noexcept {
        luacs_newobject(state, BalltzeUIWidgetEventArgs, args);
    }

    void push_meta_balltze_ui_widget_back_event_args(lua_State *state, BalltzeUIWidgetEventArgs *args) noexcept {
        luacs_newobject(state, BalltzeUIWidgetEventArgs, args);
    }

    void push_meta_balltze_ui_widget_focus_event_args(lua_State *state, BalltzeUIWidgetEventArgs *args) noexcept {
        luacs_newobject(state, BalltzeUIWidgetEventArgs, args);
    }

    void push_meta_balltze_ui_widget_accept_event_args(lua_State *state, BalltzeUIWidgetEventArgs *args) noexcept {
        luacs_newobject(state, BalltzeUIWidgetEventArgs, args);
    }

    static void define_balltze_ui_widget_sound_event_args(lua_State *state) noexcept {
        luacs_newstruct(state, BalltzeUIWidgetSoundEventArgs);
        luacs_enum_field(state, BalltzeUIWidgetSoundEventArgs, EngineWidgetNavigationSound, sound, LUACS_FREADONLY);
        lua_pop(state, 1);
    }

    void push_meta_balltze_ui_widget_sound_event_args(lua_State *state, BalltzeUIWidgetSoundEventArgs *args) noexcept {
        luacs_newobject(state, BalltzeUIWidgetSoundEventArgs, args);
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
        lua_pop(state, 1);
    }

    static void define_balltze_ui_widget_list_tab_event_args_struct(lua_State *state) noexcept {
        luacs_newstruct(state, BalltzeUIWidgetListTabEventArgs);
        luacs_objref_field(state, BalltzeUIWidgetListTabEventArgs, EngineUIWidget, widget_list, LUACS_FREADONLY);
        luacs_enum_field(state, BalltzeUIWidgetListTabEventArgs, BalltzeUIWidgetListTabType, tab, LUACS_FREADONLY);
        lua_pop(state, 1);
    }

    void push_meta_balltze_ui_widget_list_tab_event_args(lua_State *state, BalltzeUIWidgetListTabEventArgs *args) noexcept {
        luacs_newobject(state, BalltzeUIWidgetListTabEventArgs, args);
    }

    void define_balltze_event_types(lua_State *state) noexcept {
        define_balltze_camera_event_args_struct(state);
        define_balltze_game_input_event_args_struct(state);
        define_hud_hold_for_action_message_slice_enum(state);
        define_balltze_event_hud_message_button_type_enum(state);
        define_balltze_event_hud_message_axis_direction_enum(state);
        define_balltze_hud_hold_to_action_message_button_struct(state);
        define_balltze_hud_hold_for_action_message_args_struct(state);
        define_balltze_map_file_load_event_args_struct(state);
        define_balltze_map_load_event_args_struct(state);
        define_balltze_hud_chat_type_enum(state);
        define_balltze_hud_chat_struct(state);
        define_balltze_network_game_chat_message_event_args_struct(state);
        define_balltze_object_damage_event_args_struct(state);
        define_balltze_rcon_message_event_args_struct(state);
        define_balltze_ui_widget_render_vertex(state);
        define_balltze_ui_widget_render_vertices(state);
        define_balltze_hud_element_render_event_args(state);
        define_balltze_ui_widget_background_render_event_args(state);
        define_balltze_server_connect_event_args(state);
        define_balltze_sound_playback_event_args(state);
        define_balltze_ui_widget_create_event_args(state);
        define_balltze_ui_widget_event_args(state);
        define_balltze_ui_widget_sound_event_args(state);
        define_balltze_ui_widget_list_tab_event_type_enum(state);
        define_balltze_ui_widget_list_tab_event_args_struct(state);
    }
}
