// SPDX-License-Identifier: GPL-3.0-only

#include "../helpers/luacstruct.hpp"
#include "engine_types.hpp"
#include "engine_rasterizer.hpp"
#include "engine_user_interface.hpp"
#include "balltze_event.hpp"

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
            auto *self = reinterpret_cast<BalltzeHudHoldForActionMessageArgs *>(lua_touserdata(state, lua_upvalueindex(1))); 
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

    void push_meta_balltze_hud_hold_for_action_message_args(lua_State *state, BalltzeHudHoldForActionMessageArgs *args) noexcept {
        luacs_newobject(state, BalltzeHudHoldForActionMessageArgs, args);
    }

    static void define_balltze_map_file_load_event_args_struct(lua_State *state) noexcept {
        luacs_newstruct(state, BalltzeMapFileLoadEventArgs);
        luacs_declare_method(state, "mapName", +[](lua_State *state) -> int {
            auto *self = reinterpret_cast<BalltzeMapFileLoadEventArgs *>(lua_touserdata(state, lua_upvalueindex(1)));
            lua_pushstring(state, self->map_name.c_str());
            return 1;
        });
        luacs_declare_method(state, "mapPath", +[](lua_State *state) -> int {
            auto *self = reinterpret_cast<BalltzeMapFileLoadEventArgs *>(lua_touserdata(state, lua_upvalueindex(1)));
            lua_pushstring(state, self->map_path.c_str());
            return 1;
        });
        lua_pop(state, 1);
    }

    void push_meta_balltze_map_file_load_event_args(lua_State *state, BalltzeMapFileLoadEventArgs *args) noexcept {
        luacs_newobject(state, BalltzeMapFileLoadEventArgs, args);
    }

    static void define_balltze_map_load_event_args_strct(lua_State *state) noexcept {
        luacs_newstruct(state, BalltzeMapLoadEventArgs);
        luacs_declare_method(state, "mapName", +[](lua_State *state) -> int {
            auto *self = reinterpret_cast<BalltzeMapLoadEventArgs *>(lua_touserdata(state, lua_upvalueindex(1)));
            lua_pushstring(state, self->name.c_str());
            return 1;
        });
        lua_pop(state, 1);
    }

    void push_meta_balltze_map_load_event_args(lua_State *state, BalltzeMapLoadEventArgs *args) noexcept {
        luacs_newobject(state, BalltzeMapLoadEventArgs, args);
    }

    void lua_define_hud_chat_type_enum(lua_State *state) noexcept {
        luacs_newenum(state, BalltzeHudChatType);
        luacs_enum_declare_value(state, "NONE", BalltzeHudChatType::HUD_CHAT_TYPE_NONE);
        luacs_enum_declare_value(state, "ALL", BalltzeHudChatType::HUD_CHAT_TYPE_ALL);
        luacs_enum_declare_value(state, "TEAM", BalltzeHudChatType::HUD_CHAT_TYPE_TEAM);
        luacs_enum_declare_value(state, "VEHICLE", BalltzeHudChatType::HUD_CHAT_TYPE_VEHICLE);
        luacs_enum_declare_value(state, "CUSTOM", BalltzeHudChatType::HUD_CHAT_TYPE_CUSTOM);
        lua_pop(state, 1);
    }

    static void lua_define_balltze_hud_chat_struct(lua_State *state) noexcept {
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
}
