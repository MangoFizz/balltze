// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__LUA__API__V2__TYPES__ENGINE__UI_WIDGET_HPP
#define BALLTZE__LUA__API__V2__TYPES__ENGINE__UI_WIDGET_HPP

#include <optional>
#include <lua.hpp>
#include <impl/interface/ui_widget.h>

namespace Balltze::Lua::Api::V2 {
    /**
     * Push a PlayerHandle to the Lua stack.
     * @param state Lua state
     * @param widget The Widget to push
     * @param readonly If true, the handle will be read-only in Lua
     */
    void push_ui_widget(lua_State *state, const Widget &widget, bool readonly = false) noexcept;

    /**
     * Get a Widget from the Lua stack.
     * @param state Lua state
     * @param index Index of the widget on the stack
     * @return Pointer to the Widget, or nullptr if not found
     */
    std::optional<Widget *> get_ui_widget(lua_State *state, int index) noexcept;

    /**
     * Push a UIWidgetEventRecord to the Lua stack.
     * @param state Lua state
     * @param event_record The UIWidgetEventRecord to push
     * @param readonly If true, the record will be read-only in Lua
     */
    void push_ui_widget_event_record(lua_State *state, const UIWidgetEventRecord &event_record, bool readonly = false) noexcept;

    /**
     * Define the UI widget types in Lua.
     * 
     * @param state Lua state
     */
    void define_ui_widget_types(lua_State *state) noexcept;
}

#endif
