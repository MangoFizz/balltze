// SPDX-License-Identifier: GPL-3.0-only

#include "../../../libraries/luacstruct.hpp"
#include "../../../helpers/enum.hpp"
#include "engine_user_interface.hpp"
#include "engine_tag_data.hpp"
#include "engine_types.hpp"

namespace Balltze::Lua::Api::V1 {
    static void define_engine_widget_navigation_sound_enum(lua_State *state) noexcept {
        luacs_newenum(state, EngineWidgetNavigationSound);
        luacs_enum_declare_value(state, "cursor", LegacyApi::Engine::WIDGET_NAVIGATION_SOUND_CURSOR);
        luacs_enum_declare_value(state, "back", LegacyApi::Engine::WIDGET_NAVIGATION_SOUND_BACK);
        luacs_enum_declare_value(state, "forward", LegacyApi::Engine::WIDGET_NAVIGATION_SOUND_FORWARD);
        luacs_enum_declare_value(state, "flagFailure", LegacyApi::Engine::WIDGET_NAVIGATION_SOUND_FLAG_FAILURE);
        publish_enum(state, "Engine", "userInterface", "navigationSound", -1); 
        lua_pop(state, 1);
    }

    static void define_engine_input_device_enum(lua_State *state) noexcept {
        luacs_newenum(state, EngineInputDevice);
        luacs_enum_declare_value(state, "keyboard", LegacyApi::Engine::INPUT_DEVICE_KEYBOARD);
        luacs_enum_declare_value(state, "mouse", LegacyApi::Engine::INPUT_DEVICE_MOUSE);
        luacs_enum_declare_value(state, "gamepad", LegacyApi::Engine::INPUT_DEVICE_GAMEPAD);
        publish_enum(state, "Engine", "userInterface", "inputDevice", -1); 
        lua_pop(state, 1);
    }

    static void define_engine_widget_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineUIWidget);
        luacs_nested_field(state, EngineUIWidget, EngineResourceHandle, definition_tag_handle, 0);
        luacs_string_field(state, EngineUIWidget, name, 0);
        luacs_unsigned_field(state, EngineUIWidget, controller_index, 0);
        luacs_nested_field(state, EngineUIWidget, EnginePoint2DInt, position, 0);
        luacs_enum_field(state, EngineUIWidget, EngineUIWidgetType, type, 0);
        luacs_bool_field(state, EngineUIWidget, visible, 0);
        luacs_bool_field(state, EngineUIWidget, render_regardless_of_controller_index, 0);
        luacs_bool_field(state, EngineUIWidget, never_receive_events, 0);
        luacs_bool_field(state, EngineUIWidget, pauses_game_time, 0);
        luacs_bool_field(state, EngineUIWidget, deleted, 0);
        luacs_unsigned_field(state, EngineUIWidget, creation_process_start_time, 0);
        luacs_unsigned_field(state, EngineUIWidget, ms_to_close, 0);
        luacs_unsigned_field(state, EngineUIWidget, ms_to_close_fade_time, 0);
        luacs_float_field(state, EngineUIWidget, opacity, 0);
        luacs_objref_field(state, EngineUIWidget, EngineUIWidget, previous_widget, 0);
        luacs_objref_field(state, EngineUIWidget, EngineUIWidget, next_widget, 0);
        luacs_objref_field(state, EngineUIWidget, EngineUIWidget, parent_widget, 0);
        luacs_objref_field(state, EngineUIWidget, EngineUIWidget, child_widget, 0);
        luacs_objref_field(state, EngineUIWidget, EngineUIWidget, focused_child, 0);
        luacs_declare_field(state, LUACS_TSTRING, NULL, "textBoxText", sizeof(((EngineUIWidget *)0)->text_box.text), OFFSET_OF(EngineUIWidget, text_box.text), 0, 0);
        luacs_declare_field(state, LUACS_TSTRING, NULL, "textBoxCursorIndex", sizeof(((EngineUIWidget *)0)->text_box.cursor_index), OFFSET_OF(EngineUIWidget, text_box.cursor_index), 0, 0);
        luacs_declare_field(state, LUACS_TSTRING, NULL, "listElementIndex", sizeof(((EngineUIWidget *)0)->list.element_index), OFFSET_OF(EngineUIWidget, list.element_index), 0, 0);
        luacs_declare_field(state, LUACS_TSTRING, NULL, "listElementCount", sizeof(((EngineUIWidget *)0)->list.element_count), OFFSET_OF(EngineUIWidget, list.element_count), 0, 0);
        luacs_objref_field(state, EngineUIWidget, EngineUIWidget, extended_description, 0);
        luacs_unsigned_field(state, EngineUIWidget, bitmap_index, 0);
        lua_pop(state, 1); 
    }

    void push_meta_engine_widget(lua_State *state, LegacyApi::Engine::Widget *widget) noexcept {
        luacs_newobject(state, EngineUIWidget, widget);
    }

    LegacyApi::Engine::Widget *from_meta_engine_widget(lua_State *state, int index) noexcept {
        return reinterpret_cast<LegacyApi::Engine::Widget *>(luacs_checkobject(state, index, "EngineUIWidget"));
    }

    static void define_engine_input_buffered_key_modifier_enum(lua_State *state) noexcept {
        luacs_newenum(state, EngineInputBufferedKeyModifier);
        luacs_enum_declare_value(state, "shift", EngineInputBufferedKeyModifier::MODIFIER_SHIFT);
        luacs_enum_declare_value(state, "ctrl", EngineInputBufferedKeyModifier::MODIFIER_CTRL);
        luacs_enum_declare_value(state, "alt", EngineInputBufferedKeyModifier::MODIFIER_ALT);
        publish_enum(state, "Engine", "userInterface", "inputBufferedKeyModifier", -1); 
        lua_pop(state, 1);
    }

    static void define_engine_input_buffered_key(lua_State *state) noexcept {
        luacs_newstruct(state, EngineInputBufferedKey);
        luacs_enum_field(state, EngineInputBufferedKey, EngineInputBufferedKeyModifier, modifiers, 0);
        luacs_int_field(state, EngineInputBufferedKey, character, 0);
        luacs_int_field(state, EngineInputBufferedKey, keycode, 0);
        lua_pop(state, 1);
    }

    void push_meta_engine_input_buffered_key(lua_State *state, LegacyApi::Engine::InputGlobals::BufferedKey *key) noexcept {
        luacs_newobject(state, EngineInputBufferedKey, key);
    }
    
    LegacyApi::Engine::InputGlobals::BufferedKey *from_meta_engine_input_buffered_key(lua_State *state, int index) noexcept {
        return reinterpret_cast<LegacyApi::Engine::InputGlobals::BufferedKey *>(luacs_checkobject(state, index, "EngineInputBufferedKey"));
    }

    static void define_engine_input_mouse_button_enum(lua_State *state) noexcept {
        luacs_newenum(state, EngineInputMouseButton);
        luacs_enum_declare_value(state, "left", EngineInputMouseButton::MOUSE_BUTTON_LEFT);
        luacs_enum_declare_value(state, "middle", EngineInputMouseButton::MOUSE_BUTTON_MIDDLE);
        luacs_enum_declare_value(state, "right", EngineInputMouseButton::MOUSE_BUTTON_RIGHT);
        luacs_enum_declare_value(state, "doubleLeft", EngineInputMouseButton::MOUSE_BUTTON_DOUBLE_LEFT);
        publish_enum(state, "Engine", "userInterface", "inputMouseButton", -1); 
        lua_pop(state, 1);
    }

    void define_engine_user_interface_types(lua_State *state) noexcept {
        define_engine_widget_navigation_sound_enum(state);
        define_engine_input_device_enum(state);
        define_engine_input_buffered_key_modifier_enum(state);
        define_engine_input_mouse_button_enum(state);
        define_engine_widget_struct(state);
        define_engine_input_buffered_key(state);
    }
}
