// SPDX-License-Identifier: GPL-3.0-only

#include <lua.hpp>
#include <impl/interface/ui_widget.h>
#include "../../../../helpers/function_table.hpp"
#include "../../types.hpp"

namespace Balltze::Lua::Api::V2 {
    static int lua_ui_widget_launch(lua_State *state) noexcept {
        int args = lua_gettop(state);

        if(args != 1) {
            return luaL_error(state, "Invalid number of arguments in function Engine.uiWidget.launchWidget.");
        }

        auto tag_handle = get_tag_handle(state, 1);
        if(!tag_handle || HANDLE_IS_NULL(*tag_handle)) {
            return luaL_error(state, "Invalid tag handle in function Engine.uiWidget.launchWidget.");
        }

        Widget *widget = nullptr;
        Widget *active_widget = ui_widget_get_active_widget(0);
        if(active_widget != nullptr) {
            widget = ui_widget_launch(active_widget, *tag_handle);
        } 
        else {
            widget = ui_widget_load_by_name_or_tag(nullptr, *tag_handle, nullptr, -1, NULL_HANDLE, NULL_HANDLE, -1);
        }

        if(widget) {
            push_ui_widget(state, *widget);
        }
        else {
            lua_pushnil(state);
        }
        return 1;
    }

    static int lua_ui_widget_get_active(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args != 0) {
            return luaL_error(state, "Invalid number of arguments in function Engine.uiWidget.getActiveWidget.");
        }

        Widget *active_widget = ui_widget_get_active_widget(0);
        if(active_widget) {
            push_ui_widget(state, *active_widget);
        } 
        else {
            lua_pushnil(state);
        }
        return 1;
    }

    static int lua_ui_widget_close(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args != 0) {
            return luaL_error(state, "Invalid number of arguments in function Engine.uiWidget.closeWidget.");
        }

        ui_widget_close();
        return 0;
    }

    static int lua_ui_widget_replace(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args != 2) {
            return luaL_error(state, "Invalid number of arguments in function Engine.uiWidget.replaceWidget.");
        }

        auto target_widget = get_ui_widget(state, 1);
        if(!target_widget) {
            return luaL_error(state, "Invalid target widget in function Engine.uiWidget.replaceWidget.");
        }

        auto tag_handle = get_tag_handle(state, 2);
        if(!tag_handle || HANDLE_IS_NULL(*tag_handle)) {
            return luaL_error(state, "Invalid tag handle in function Engine.uiWidget.replaceWidget.");
        }

        Widget *new_widget = ui_widget_replace(*target_widget, *tag_handle);
        push_ui_widget(state, *new_widget);

        return 1;
    }

    static int lua_ui_widget_reload(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args != 1) {
            return luaL_error(state, "Invalid number of arguments in function Engine.uiWidget.reloadWidget.");
        }

        auto target_widget = get_ui_widget(state, 1);
        if(!target_widget) {
            return luaL_error(state, "Invalid target widget in function Engine.uiWidget.reloadWidget.");
        }

        Widget *reloaded_widget = ui_widget_reload(*target_widget);
        push_ui_widget(state, *reloaded_widget);

        return 1;
    }

    static int lua_ui_widget_focus(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args != 1) {
            return luaL_error(state, "Invalid number of arguments in function Engine.uiWidget.focusWidget.");
        }

        auto target_widget = get_ui_widget(state, 1);
        if(!target_widget) {
            return luaL_error(state, "Invalid target widget in function Engine.uiWidget.focusWidget.");
        }

        ui_widget_instance_give_focus_directly(target_widget.value()->parent, *target_widget);
        return 0;
    }

    static int lua_ui_widget_unfocus(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args != 1) {
            return luaL_error(state, "Invalid number of arguments in function Engine.uiWidget.unfocusWidget.");
        }

        auto widget_param = get_ui_widget(state, 1);
        if(!widget_param) {
            return luaL_error(state, "Invalid target widget in function Engine.uiWidget.unfocusWidget.");
        }

        Widget *widget = *widget_param;
        if(widget->parent && widget->parent->focused_child == widget) {
            widget->parent->focused_child = nullptr;
        }

        return 0;
    }

    static int lua_ui_widget_find_widgets(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args != 1 && args != 2 && args != 3) {
            return luaL_error(state, "Invalid number of arguments in function Engine.userInterface.findWidgets.");
        }

        auto tag_handle = get_tag_handle(state, 1);
        if(!tag_handle || HANDLE_IS_NULL(*tag_handle)) {
            return luaL_error(state, "Invalid tag handle in function Engine.uiWidget.findWidgets.");
        }

        auto widget_base = get_ui_widget(state, 2);
        if(!widget_base) {
            widget_base = ui_widget_get_active_widget(0);
        }

        bool first_match = false;
        if(args == 3) {
            first_match = lua_toboolean(state, 3);
        }

        std::vector<Widget *> found_widgets;

        std::function<void(Widget *)> search_instances = [&](Widget *widget) {
            if(first_match && !found_widgets.empty()) {
                return;
            }
            
            if(widget->definition_tag_handle.value == tag_handle->value) {
                found_widgets.push_back(widget);
                return;
            }

            if(widget->child) {
                search_instances(widget->child);
            }

            if(widget->next) {
                search_instances(widget->next);
            }

            if(ui_widget_is_list(widget) && widget->list_parameters.extended_description) {
                // search_instances(widget->list_parameters.extended_description);
            }
        };

        if(*widget_base) {
            search_instances(*widget_base);
        }

        lua_newtable(state);
        for(std::size_t i = 0; i < found_widgets.size(); i++) {
            push_ui_widget(state, *found_widgets[i]);
            lua_rawseti(state, -2, i + 1);
        }

        return 1;
    }

    static int lua_ui_widget_disable_widget(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args != 1) {
            return luaL_error(state, "Invalid number of arguments in function Engine.uiWidget.disableWidget.");
        }

        auto target_widget = get_ui_widget(state, 1);
        if(!target_widget) {
            return luaL_error(state, "Invalid target widget in function Engine.uiWidget.disableWidget.");
        }

        ui_widget_disable_widget(*target_widget);
        return 0;
    }

    static int lua_ui_widget_enable_widget(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args != 1) {
            return luaL_error(state, "Invalid number of arguments in function Engine.uiWidget.enableWidget.");
        }

        auto target_widget = get_ui_widget(state, 1);
        if(!target_widget) {
            return luaL_error(state, "Invalid target widget in function Engine.uiWidget.enableWidget.");
        }

        ui_widget_enable_widget(*target_widget);
        return 0;
    }

    static const luaL_Reg ui_widget_functions[] = {
        {"launchWidget", lua_ui_widget_launch},
        {"getActiveWidget", lua_ui_widget_get_active},
        {"closeWidget", lua_ui_widget_close},
        {"replaceWidget", lua_ui_widget_replace},
        {"reloadWidget", lua_ui_widget_reload},
        {"focusWidget", lua_ui_widget_focus},
        {"unfocusWidget", lua_ui_widget_unfocus},
        {"findWidgets", lua_ui_widget_find_widgets},
        {"disableWidget", lua_ui_widget_disable_widget},
        {"enableWidget", lua_ui_widget_enable_widget},
        {nullptr, nullptr}
    };

    void set_engine_ui_widget_functions(lua_State *state, int table_idx) noexcept {
        int abs_idx = lua_absindex(state, table_idx);
        lua_newtable(state);
        set_functions_reg_array(state, -1, ui_widget_functions);
        lua_setfield(state, abs_idx, "uiWidget");
    }
}
