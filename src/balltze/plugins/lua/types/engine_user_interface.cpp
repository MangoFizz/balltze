// SPDX-License-Identifier: GPL-3.0-only

#include "../helpers/luacstruct.hpp"
#include "engine_user_interface.hpp"
#include "engine_tag_data.hpp"
#include "engine_types.hpp"

namespace Balltze::Plugins::Lua {
    void define_engine_widget_navigation_sound_enum(lua_State *state) noexcept {
        luacs_newenum(state, EngineWidgetNavigationSound);
        luacs_enum_declare_value(state, "CURSOR", Engine::WIDGET_NAVIGATION_SOUND_CURSOR);
        luacs_enum_declare_value(state, "BACK", Engine::WIDGET_NAVIGATION_SOUND_BACK);
        luacs_enum_declare_value(state, "FORWARD", Engine::WIDGET_NAVIGATION_SOUND_FORWARD);
        luacs_enum_declare_value(state, "FLAG_FAILURE", Engine::WIDGET_NAVIGATION_SOUND_FLAG_FAILURE);
        lua_pop(state, 1);
    }

    static void define_engine_input_device_enum(lua_State *state) noexcept {
        luacs_newenum(state, EngineInputDevice);
        luacs_enum_declare_value(state, "KEYBOARD", Engine::INPUT_DEVICE_KEYBOARD);
        luacs_enum_declare_value(state, "MOUSE", Engine::INPUT_DEVICE_MOUSE);
        luacs_enum_declare_value(state, "GAMEPAD", Engine::INPUT_DEVICE_GAMEPAD);
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

    void push_meta_engine_widget(lua_State *state, Engine::Widget *widget) noexcept {
        luacs_newobject(state, EngineUIWidget, widget);
    }

    Engine::Widget *from_meta_engine_widget(lua_State *state, int index) noexcept {
        return reinterpret_cast<Engine::Widget *>(luacs_object_pointer(state, index, "EngineWidget"));
    }

    void define_engine_user_interface_types(lua_State *state) noexcept {
        define_engine_widget_navigation_sound_enum(state);
        define_engine_input_device_enum(state);
        define_engine_widget_struct(state);
    }
}
