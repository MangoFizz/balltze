// SPDX-License-Identifier: GPL-3.0-only

#include "../../../../libraries/luastruct.hpp"
#include "types.hpp"
#include "ui_widget.hpp"

namespace Balltze::Lua::Api::V2 {
    static void define_ui_widget_text_box_parameters_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, WidgetTextBoxParameters);
        LUAS_PRIMITIVE_FIELD(state, WidgetTextBoxParameters, text, LUAST_UINT32, 0);
        LUAS_PRIMITIVE_FIELD(state, WidgetTextBoxParameters, string_list_index, LUAST_INT16, 0);
        LUAS_OBJREF_FIELD(state, WidgetTextBoxParameters, text_color, ColorARGB, 0);
        LUAS_PRIMITIVE_FIELD(state, WidgetTextBoxParameters, flashing, LUAST_BOOL, 0);
        lua_pop(state, 1);
    }

    static void define_ui_widget_list_parameters_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, Widget);
        LUAS_STRUCT(state, WidgetListParameters);
        LUAS_PRIMITIVE_FIELD(state, WidgetListParameters, selected_list_item_index, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, WidgetListParameters, list_item_top_index, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, WidgetListParameters, current_list_item_index, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, WidgetListParameters, last_list_tab_direction, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, WidgetListParameters, number_of_items, LUAST_INT16, 0);
        LUAS_OBJREF_FIELD(state, WidgetListParameters, extended_description, Widget, LUAS_FIELD_POINTER);
        LUAS_PRIMITIVE_FIELD(state, WidgetListParameters, spin_activated, LUAST_INT16, 0);
        lua_pop(state, 2);
    }

    static void define_ui_widget_animation_data_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, WidgetAnimationData);
        LUAS_PRIMITIVE_FIELD(state, WidgetAnimationData, current_frame_index, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, WidgetAnimationData, first_frame_index, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, WidgetAnimationData, last_frame_index, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, WidgetAnimationData, number_of_sprite_frames, LUAST_INT16, 0);
        lua_pop(state, 1);
    }

    static void define_ui_widget_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, Widget);
        LUAS_OBJREF_FIELD(state, Widget, definition_tag_handle, TagHandle, LUAS_FIELD_READONLY);
        LUAS_PRIMITIVE_FIELD(state, Widget, name, LUAST_STRING_LITERAL, LUAS_FIELD_READONLY);
        LUAS_PRIMITIVE_FIELD(state, Widget, local_player_index, LUAST_INT16, LUAS_FIELD_READONLY);
        LUAS_OBJREF_FIELD(state, Widget, position, VectorXYInt, 0);
        LUAS_ENUM_FIELD(state, Widget, type, UIWidgetType, LUAS_FIELD_READONLY);
        LUAS_PRIMITIVE_FIELD(state, Widget, visible, LUAST_BOOL, 0);
        LUAS_PRIMITIVE_FIELD(state, Widget, render_regardless_of_controller_index, LUAST_BOOL, 0);
        LUAS_PRIMITIVE_FIELD(state, Widget, never_receive_events, LUAST_BOOL, 0);
        LUAS_PRIMITIVE_FIELD(state, Widget, pauses_game_time, LUAST_BOOL, 0);
        LUAS_PRIMITIVE_FIELD(state, Widget, deleted, LUAST_BOOL, 0);
        LUAS_PRIMITIVE_FIELD(state, Widget, is_error_dialog, LUAST_BOOL, 0);
        LUAS_PRIMITIVE_FIELD(state, Widget, close_if_local_player_controller_present, LUAST_BOOL, 0);
        LUAS_PRIMITIVE_FIELD(state, Widget, creation_process_start_time, LUAST_UINT32, 0);
        LUAS_PRIMITIVE_FIELD(state, Widget, ms_to_close, LUAST_UINT32, 0);
        LUAS_PRIMITIVE_FIELD(state, Widget, ms_to_close_fade_time, LUAST_UINT32, 0);
        LUAS_PRIMITIVE_FIELD(state, Widget, alpha_modifier, LUAST_FLOAT, 0);
        LUAS_OBJREF_FIELD(state, Widget, previous, Widget, LUAS_FIELD_POINTER | LUAS_FIELD_READONLY);
        LUAS_OBJREF_FIELD(state, Widget, next, Widget, LUAS_FIELD_POINTER | LUAS_FIELD_READONLY);
        LUAS_OBJREF_FIELD(state, Widget, parent, Widget, LUAS_FIELD_POINTER | LUAS_FIELD_READONLY);
        LUAS_OBJREF_FIELD(state, Widget, child, Widget, LUAS_FIELD_POINTER | LUAS_FIELD_READONLY);
        LUAS_OBJREF_FIELD(state, Widget, focused_child, Widget, LUAS_FIELD_POINTER | LUAS_FIELD_READONLY);
        LUAS_OBJREF_FIELD(state, Widget, text_box_parameters, WidgetTextBoxParameters, LUAS_FIELD_POINTER);
        LUAS_OBJREF_FIELD(state, Widget, list_parameters, WidgetListParameters, LUAS_FIELD_POINTER);
        LUAS_OBJREF_FIELD(state, Widget, animation_data, WidgetAnimationData, LUAS_FIELD_POINTER);
        lua_pop(state, 1);
    }

    void push_ui_widget(lua_State *state, const Widget &widget, bool readonly) noexcept {
        LUAS_PUSH_OBJECT(state, Widget, &widget, readonly);
    }

    std::optional<Widget *> get_ui_widget(lua_State *state, int index) noexcept {
        if(!lua_isuserdata(state, index)) {
            return std::nullopt;
        }
        auto *widget = LUAS_CHECK_OBJECT(state, index, Widget);
        if(!widget) {
            return std::nullopt;
        }
        return widget;
    }

    static void define_ui_widget_event_record_type_type(lua_State *state) noexcept {
        LUAS_ENUM(state, UIWidgetEventRecordType);
        LUAS_ENUM_VARIANT(state, UIWidgetEventRecordType, "none", EVENT_TYPE_NONE);
        LUAS_ENUM_VARIANT(state, UIWidgetEventRecordType, "keyboard", EVENT_TYPE_KEYBOARD);
        LUAS_ENUM_VARIANT(state, UIWidgetEventRecordType, "mouse", EVENT_TYPE_MOUSE);
        lua_pop(state, 1);
    }

    static void define_ui_widget_event_record_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, UIWidgetEventRecord);
        LUAS_ENUM_FIELD(state, UIWidgetEventRecord, type, UIWidgetEventRecordType, 0);
        LUAS_PRIMITIVE_FIELD(state, UIWidgetEventRecord, controller_index, LUAST_INT32, 0);
        lua_pop(state, 1);
    }

    void push_ui_widget_event_record(lua_State *state, const UIWidgetEventRecord &event_record, bool readonly) noexcept {
        LUAS_PUSH_OBJECT(state, UIWidgetEventRecord, &event_record, readonly);
    }

    void define_ui_widget_types(lua_State *state) noexcept {
        define_ui_widget_text_box_parameters_type(state);
        define_ui_widget_list_parameters_type(state);
        define_ui_widget_animation_data_type(state);
        define_ui_widget_type(state);
        define_ui_widget_event_record_type_type(state);
        define_ui_widget_event_record_type(state);
    }
}
