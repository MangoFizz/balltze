// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/engine.hpp>
#include <lua.hpp>
#include "../helpers.hpp"
#include "../metatables.hpp"

namespace Balltze::Plugins {
    extern std::string lua_engine_u_i_widget_type_to_string(Engine::TagDefinitions::UIWidgetType value) noexcept; 
    extern Engine::TagDefinitions::UIWidgetType lua_engine_u_i_widget_type_from_string(std::string str); 

    static int lua_engine_widget__index(lua_State *state) noexcept {
        auto *widget = lua_from_meta_object<Engine::Widget>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "definitionTagHandle") { 
            lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&widget->definition_tag_handle));
            return 1;
        }
        else if(field == "name") {
            lua_pushstring(state, widget->name);
            return 1;
        }
        else if(field == "controllerIndex") {
            lua_pushinteger(state, widget->controller_index);
            return 1;
        }
        else if(field == "position") {
            lua_push_meta_engine_point2_d_int(state, widget->position);
            return 1;
        }
        else if(field == "type") {
            std::string type = lua_engine_u_i_widget_type_to_string(widget->type);
            lua_pushstring(state, type.c_str());
            return 1;
        }
        else if(field == "visible") {
            lua_pushboolean(state, widget->visible);
            return 1;
        }
        else if(field == "renderRegardlessOfControllerIndex") {
            lua_pushboolean(state, widget->render_regardless_of_controller_index);
            return 1;
        }
        else if(field == "pausesGameTime") {
            lua_pushboolean(state, widget->pauses_game_time);
            return 1;
        }
        else if(field == "deleted") {
            lua_pushboolean(state, widget->deleted);
            return 1;
        }
        else if(field == "creationProcessStartTime") {
            lua_pushinteger(state, widget->creation_process_start_time);
            return 1;
        }
        else if(field == "msToClose") {
            lua_pushinteger(state, widget->ms_to_close);
            return 1;
        }
        else if(field == "msToCloseFadeTime") {
            lua_pushinteger(state, widget->ms_to_close_fade_time);
            return 1;
        }
        else if(field == "opacity") {
            lua_pushnumber(state, widget->opacity);
            return 1;
        }
        else if(field == "previousWidget") {
            if(widget->previous_widget == nullptr) {
                lua_pushnil(state);
            }
            else {
                lua_push_meta_engine_widget(state, *widget->previous_widget);
            }
            return 1;
        }
        else if(field == "nextWidget") {
            if(widget->next_widget == nullptr) {
                lua_pushnil(state);
            }
            else {
                lua_push_meta_engine_widget(state, *widget->next_widget);
            }
            return 1;
        }
        else if(field == "parentWidget") {
            if(widget->parent_widget == nullptr) {
                lua_pushnil(state);
            }
            else {
                lua_push_meta_engine_widget(state, *widget->parent_widget);
            }
            return 1;
        }
        else if(field == "childWidget") {
            if(widget->child_widget == nullptr) {
                lua_pushnil(state);
            }
            else {
                lua_push_meta_engine_widget(state, *widget->child_widget);
            }
            return 1;
        }
        else if(field == "focusedChild") {
            if(widget->focused_child == nullptr) {
                lua_pushnil(state);
            }
            else {
                lua_push_meta_engine_widget(state, *widget->focused_child);
            }
            return 1;
        }
        else if(field == "textAddress") {
            lua_pushinteger(state, reinterpret_cast<std::uint32_t>(const_cast<wchar_t *>(widget->text_box.text)));
            return 1;
        }
        else if(field == "cursorIndex") {
            lua_pushinteger(state, widget->text_box.cursor_index);
            return 1;
        }
        else if(field == "extendedDescription") {
            lua_push_meta_engine_widget(state, *widget->extended_description);
            return 1;
        }
        else if(field == "bitmapIndex") {
            lua_pushinteger(state, widget->bitmap_index);
            return 1;
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
    }

    static int lua_engine_widget__newindex(lua_State *state) noexcept {
        auto *widget = lua_from_meta_object<Engine::Widget>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "definitionTagHandle") { 
            widget->definition_tag_handle.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "name") {
            return luaL_error(state, "Unsupported operation");
        }
        else if(field == "controllerIndex") {
            widget->controller_index = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "position") {
            return luaL_error(state, "Unsupported operation");
        }
        else if(field == "type") {
            try {
                std::string type = luaL_checkstring(state, 3);
                widget->type = lua_engine_u_i_widget_type_from_string(type);
            }
            catch(...) {
                return luaL_error(state, "Invalid value");
            }
        }
        else if(field == "visible") {
            widget->visible = lua_toboolean(state, 3);
        }
        else if(field == "renderRegardlessOfControllerIndex") {
            widget->render_regardless_of_controller_index = lua_toboolean(state, 3);
        }
        else if(field == "pausesGameTime") {
            widget->pauses_game_time = lua_toboolean(state, 3);
        }
        else if(field == "deleted") {
            widget->deleted = lua_toboolean(state, 3);
        }
        else if(field == "creationProcessStartTime") {
            widget->creation_process_start_time = luaL_checkinteger(state, 3);
        }
        else if(field == "msToClose") {
            widget->ms_to_close = luaL_checkinteger(state, 3);
        }
        else if(field == "msToCloseFadeTime") {
            widget->ms_to_close_fade_time = luaL_checkinteger(state, 3);
        }
        else if(field == "opacity") {
            float opacity = luaL_checknumber(state, 3);
            if(opacity < 0.0f || opacity > 1.0f) {
                return luaL_error(state, "Invalid value");
            }
            widget->opacity = opacity;
        }
        else if(field == "previousWidget") {
            auto *previous_widget = lua_from_meta_object<Engine::Widget>(state, 3);
            if(previous_widget) {
                widget->previous_widget = previous_widget;
            }
            else {
                return luaL_error(state, "Invalid value");
            }
        }
        else if(field == "nextWidget") {
            auto *next_widget = lua_from_meta_object<Engine::Widget>(state, 3);
            if(next_widget) {
                widget->next_widget = next_widget;
            }
            else {
                return luaL_error(state, "Invalid value");
            }
        }
        else if(field == "parentWidget") {
            auto *parent_widget = lua_from_meta_object<Engine::Widget>(state, 3);
            if(parent_widget) {
                widget->parent_widget = parent_widget;
            }
            else {
                return luaL_error(state, "Invalid value");
            }
        }
        else if(field == "childWidget") {
            auto *child_widget = lua_from_meta_object<Engine::Widget>(state, 3);
            if(child_widget) {
                widget->child_widget = child_widget;
            }
            else {
                return luaL_error(state, "Invalid value");
            }
        }
        else if(field == "focusedChild") {
            auto *focused_child = lua_from_meta_object<Engine::Widget>(state, 3);
            if(focused_child) {
                widget->focused_child = focused_child;
            }
            else {
                return luaL_error(state, "Invalid value");
            }
        }
        else if(field == "textAddress") {
            return luaL_error(state, "Unsupported operation");
        }
        else if(field == "cursorIndex") {
            widget->text_box.cursor_index = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "extendedDescription") {
            auto *extended_description = lua_from_meta_object<Engine::Widget>(state, 3);
            if(extended_description) {
                widget->extended_description = extended_description;
            }
            else {
                return luaL_error(state, "Invalid value");
            }
        }
        else if(field == "bitmapIndex") {
            widget->bitmap_index = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_widget(lua_State *state, Engine::Widget &widget, bool read_only) noexcept {
        lua_push_meta_object(state, &widget, lua_engine_widget__index, lua_engine_widget__newindex, read_only); 
    }
}
