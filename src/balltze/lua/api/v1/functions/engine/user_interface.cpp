// SPDX-License-Identifier: GPL-3.0-only

#include <lua.hpp>
#include <balltze/legacy_api/engine.hpp>
#include "../../../../helpers/plugin.hpp"
#include "../../../../libraries/luacstruct.hpp"
#include "../../types.hpp"

namespace Balltze::Lua::Api::V1 {
    static int engine_find_widget(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 1 || args == 2) {
            LegacyApi::Engine::TagHandle tag_handle;

            if(lua_isinteger(state, 1) || lua_istable(state, 1) || lua_isuserdata(state, 1)) {
                auto handle = get_engine_resource_handle(state, 1);
                if(!handle || handle->is_null()) {
                    return luaL_error(state, "Invalid tag handle in function Engine.userInterface.findWidget.");
                }
                tag_handle = *handle;
                auto *tag = LegacyApi::Engine::get_tag(tag_handle);
                if(!tag) {
                    return luaL_error(state, "Could not find tag in function Engine.userInterface.findWidget.");
                }
                if(tag->primary_class != LegacyApi::Engine::TAG_CLASS_UI_WIDGET_DEFINITION) {
                    return luaL_error(state, "Tag is not a widget definition in function Engine.userInterface.findWidget.");
                }
            }
            else {
                const char *tag_path = luaL_checkstring(state, 1);
                auto *tag = LegacyApi::Engine::get_tag(tag_path, LegacyApi::Engine::TAG_CLASS_UI_WIDGET_DEFINITION);
                if(!tag) {
                    return luaL_error(state, "Could not find tag in function Engine.userInterface.findWidget.");
                }
                tag_handle = tag->handle;
            }

            LegacyApi::Engine::Widget *base_widget = nullptr;
            if(args == 2) {
                base_widget = from_meta_engine_widget(state, 2);
                if(!base_widget) {
                    return luaL_error(state, "Invalid base widget.");
                }
            }

            auto search_result = LegacyApi::Engine::find_widgets(tag_handle, true, base_widget);
            if(!search_result.empty()) {
                auto *widget = search_result[0];
                push_meta_engine_widget(state, widget);
            }
            else {
                lua_pushnil(state);
            }
            return 1;
        }
        else {
            return luaL_error(state, "Invalid number of arguments in function Engine.userInterface.findWidget.");
        }
    }

    static int engine_find_widgets(lua_State *state) noexcept {
        int args = lua_gettop(state);

        if(args == 1 || args == 2) {
            LegacyApi::Engine::TagHandle tag_handle;

            if(lua_isinteger(state, 1) || lua_istable(state, 1) || lua_isuserdata(state, 1)) {
                auto handle = get_engine_resource_handle(state, 1);
                if(!handle || handle->is_null()) {
                    return luaL_error(state, "Invalid tag handle in function Engine.userInterface.findWidgets.");
                }
                tag_handle = *handle;
                auto *tag = LegacyApi::Engine::get_tag(tag_handle);
                if(!tag) {
                    return luaL_error(state, "Could not find tag in function Engine.userInterface.findWidgets.");
                }
                if(tag->primary_class != LegacyApi::Engine::TAG_CLASS_UI_WIDGET_DEFINITION) {
                    return luaL_error(state, "Tag is not a widget definition in function Engine.userInterface.findWidgets.");
                }
            }
            else {
                const char *tag_path = luaL_checkstring(state, 1);
                auto *tag = LegacyApi::Engine::get_tag(tag_path, LegacyApi::Engine::TAG_CLASS_UI_WIDGET_DEFINITION);
                if(!tag) {
                    return luaL_error(state, "Could not find tag in function Engine.userInterface.findWidget.");
                }
                tag_handle = tag->handle;
            }

            LegacyApi::Engine::Widget *base_widget = nullptr;
            if(args == 2) {
                base_widget = from_meta_engine_widget(state, 2);
                if(!base_widget) {
                    return luaL_error(state, "Invalid base widget in function Engine.userInterface.findWidgets.");
                }
            }

            auto search_result = LegacyApi::Engine::find_widgets(tag_handle, false, base_widget);
            lua_newtable(state);
            for(std::size_t i = 0; i < search_result.size(); i++) {
                push_meta_engine_widget(state, search_result[i]);
                lua_rawseti(state, -2, i + 1);
            }
            return 1;
        }
        else {
            return luaL_error(state, "Invalid number of arguments in function Engine.userInterface.findWidgets.");
        }
    }

    static int engine_open_widget(lua_State *state) noexcept {
        int args = lua_gettop(state);

        if(args == 1 || args == 2) {
            LegacyApi::Engine::TagHandle tag_handle;

            if(lua_isinteger(state, 1) || lua_istable(state, 1) || lua_isuserdata(state, 1)) {
                auto handle = get_engine_resource_handle(state, 1);
                if(!handle || handle->is_null()) {
                    return luaL_error(state, "Invalid tag handle in function Engine.userInterface.openWidget.");
                }
                tag_handle = *handle;
                auto *tag = LegacyApi::Engine::get_tag(tag_handle);
                if(!tag) {
                    return luaL_error(state, "Could not find tag in function Engine.userInterface.openWidget.");
                }
                if(tag->primary_class != LegacyApi::Engine::TAG_CLASS_UI_WIDGET_DEFINITION) {
                    return luaL_error(state, "Tag is not a widget definition in function Engine.userInterface.openWidget.");
                }
            }
            else {
                const char *tag_path = luaL_checkstring(state, 1);
                auto *tag = LegacyApi::Engine::get_tag(tag_path, LegacyApi::Engine::TAG_CLASS_UI_WIDGET_DEFINITION);
                if(!tag) {
                    return luaL_error(state, "Could not find tag in function Engine.userInterface.openWidget.");
                }
                tag_handle = tag->handle;
            }

            bool push_history = false;
            if(args == 2) {
                if(lua_isboolean(state, 2)) {
                    push_history = lua_toboolean(state, 2);
                }
                else {
                    return luaL_error(state, "Invalid push history argument in function Engine.userInterface.openWidget.");
                }
            }

            auto *widget = LegacyApi::Engine::open_widget(tag_handle, push_history);
            if(widget) {
                push_meta_engine_widget(state, widget);
            }
            else {
                lua_pushnil(state);
            }
            return 1;
        }
        else {
            return luaL_error(state, "Invalid number of arguments in function Engine.userInterface.openWidget.");
        }
    }

    static int engine_close_widget(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 0) {
            LegacyApi::Engine::close_widget();
            return 0;
        }
        else {
            return luaL_error(state, "Invalid number of arguments in function Engine.userInterface.closeWidget.");
        }
    }

    static int engine_replace_widget(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 2) {
            LegacyApi::Engine::Widget *target_widget = from_meta_engine_widget(state, 1);
            if(!target_widget) {
                return luaL_error(state, "Invalid target widget in function Engine.userInterface.replaceWidget.");
            }

            LegacyApi::Engine::TagHandle tag_handle;
            if(lua_isinteger(state, 2) || lua_istable(state, 2) || lua_isuserdata(state, 2)) {
                auto handle = get_engine_resource_handle(state, 2);
                if(!handle || handle->is_null()) {
                    return luaL_error(state, "Invalid tag handle in function Engine.userInterface.replaceWidget.");
                }
                tag_handle = *handle;
                auto *tag = LegacyApi::Engine::get_tag(tag_handle);
                if(!tag) {
                    return luaL_error(state, "Could not find tag in function Engine.userInterface.replaceWidget.");
                }
                if(tag->primary_class != LegacyApi::Engine::TAG_CLASS_UI_WIDGET_DEFINITION) {
                    return luaL_error(state, "Tag is not a widget definition in function Engine.userInterface.replaceWidget.");
                }
            }
            else {
                const char *tag_path = luaL_checkstring(state, 2);
                auto *tag = LegacyApi::Engine::get_tag(tag_path, LegacyApi::Engine::TAG_CLASS_UI_WIDGET_DEFINITION);
                if(!tag) {
                    return luaL_error(state, "Could not find tag in function Engine.userInterface.replaceWidget.");
                }
                tag_handle = tag->handle;
            }

            auto *widget = LegacyApi::Engine::replace_widget(target_widget, tag_handle);
            if(widget) {
                push_meta_engine_widget(state, widget);
            }
            else {
                lua_pushnil(state);
            }
            return 1;
        }
        else {
            return luaL_error(state, "Invalid number of arguments in function Engine.userInterface.replaceWidget.");
        }
    }

    static int engine_reload_widget(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 1) {
            LegacyApi::Engine::Widget *target_widget = from_meta_engine_widget(state, 1);
            if(!target_widget) {
                return luaL_error(state, "Invalid target widget in function Engine.userInterface.reloadWidget.");
            }

            auto *widget = LegacyApi::Engine::reload_widget(target_widget);
            if(widget) {
                push_meta_engine_widget(state, widget);
            }
            else {
                lua_pushnil(state);
            }
            return 1;
        }
        else {
            return luaL_error(state, "Invalid number of arguments in function Engine.userInterface.reloadWidget.");
        }
    }

    static int engine_focus_widget(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 1) {
            LegacyApi::Engine::Widget *target_widget = from_meta_engine_widget(state, 1);
            if(!target_widget) {
                return luaL_error(state, "Invalid target widget in function Engine.userInterface.focusWidget.");
            }

            LegacyApi::Engine::focus_widget(target_widget);
            return 0;
        }
        else {
            return luaL_error(state, "Invalid number of arguments in function Engine.userInterface.focusWidget.");
        }
    }

    static int engine_get_root_widget(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 0) {
            auto *root_widget = LegacyApi::Engine::get_root_widget();
            if(root_widget) {
                push_meta_engine_widget(state, root_widget);
            }
            else {
                lua_pushnil(state);
            }
            return 1;
        }
        else {
            return luaL_error(state, "Invalid number of arguments in function Engine.userInterface.getRootWidget.");
        }
    }

    static int engine_open_pause_menu(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 0) {
            LegacyApi::Engine::open_pause_menu();
            return 0;
        }
        else {
            return luaL_error(state, "Invalid number of arguments in function Engine.userInterface.openPauseMenu.");
        }
    }

    static int engine_get_hud_globals(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 0) {
            auto &hud_globals = LegacyApi::Engine::get_hud_globals();
            luacs_newobject(state, EngineHudGlobals, &hud_globals);
            return 1;
        }
        else {
            return luaL_error(state, "Invalid number of arguments in function Engine.userInterface.getHudGlobals.");
        }
    }

    static int get_bitmap_sprite_resolution(lua_State *state) noexcept {
        int args = lua_gettop(state);

        if(args == 3) {
            auto *bitmap = reinterpret_cast<LegacyApi::Engine::TagDefinitions::Bitmap *>(luacs_checkobject(state, 1, "EngineBitmap"));
            if(!bitmap) {
                return luaL_error(state, "Invalid bitmap in function Engine.userInterface.getBitmapSpriteResolution.");
            }

            auto sequence_index = luaL_checkinteger(state, 2);
            auto sprite_index = luaL_checkinteger(state, 3);

            try {
                auto resolution = LegacyApi::Engine::get_bitmap_sprite_resolution(bitmap, sequence_index, sprite_index);
                push_engine_resolution(state, &resolution);
                return 1;
            }
            catch(std::exception &e) {
                return luaL_error(state, e.what());
            }
        }
        else {
            return luaL_error(state, "Invalid number of arguments in function Engine.userInterface.getBitmapSpriteResolution.");
        }
    }

    static int engine_draw_hud_message_sprite(lua_State *state) noexcept {
        int args = lua_gettop(state);

        if(args == 4 || args == 5) {
            auto *bitmap = reinterpret_cast<LegacyApi::Engine::TagDefinitions::Bitmap *>(luacs_checkobject(state, 1, "EngineBitmap"));
            if(!bitmap) {
                return luaL_error(state, "Invalid bitmap in function Engine.userInterface.drawHudMessageSprite.");
            }

            auto sequence_index = luaL_checkinteger(state, 2);
            auto sprite_index = luaL_checkinteger(state, 3);

            try {
                auto position = get_point2_d_int(state, 4);
                if(!position) {
                    return luaL_error(state, "Invalid position in function Engine.userInterface.drawHudMessageSprite.");
                }

                LegacyApi::Engine::ColorARGBInt color = {255, 255, 255, 255};
                if(args == 5) {
                    auto colorInput = get_color_a_r_g_b_int(state, 5);
                    if(!colorInput) {
                        return luaL_error(state, "Invalid color in function Engine.userInterface.drawHudMessageSprite.");
                    }
                    color = *colorInput;
                }
                LegacyApi::Engine::draw_hud_message_sprite(bitmap, sequence_index, sprite_index, *position, color);
                return 0;
            }
            catch(std::runtime_error &e) {
                return luaL_error(state, e.what());
            }
        }
        else {
            return luaL_error(state, "Invalid number of arguments in function Engine.userInterface.drawHudMessageSprite.");
        }
    }

    static int engine_play_sound(lua_State *state) noexcept {
        int args = lua_gettop(state);

        if(args == 1) {
            LegacyApi::Engine::TagHandle tag_handle;
            if(lua_isinteger(state, 1) || lua_istable(state, 1) || lua_isuserdata(state, 1)) {
                auto handle = get_engine_resource_handle(state, 1);
                if(!handle || handle->is_null()) {
                    return luaL_error(state, "Invalid tag handle in function Engine.userInterface.playSound.");
                }
                tag_handle = *handle;
                auto *tag = LegacyApi::Engine::get_tag(tag_handle);
                if(!tag) {
                    return luaL_error(state, "Could not find tag in function Engine.userInterface.playSound.");
                }
                if(tag->primary_class != LegacyApi::Engine::TAG_CLASS_SOUND) {
                    return luaL_error(state, "Tag is not a sound in function Engine.userInterface.playSound.");
                }
            }
            else {
                const char *tag_path = luaL_checkstring(state, 1);
                auto *tag = LegacyApi::Engine::get_tag(tag_path, LegacyApi::Engine::TAG_CLASS_UI_WIDGET_DEFINITION);
                if(!tag) {
                    return luaL_error(state, "Could not find tag in function Engine.userInterface.playSound.");
                }
                tag_handle = tag->handle;
            }

            try {
                LegacyApi::Engine::play_sound(tag_handle);
            }
            catch(std::runtime_error &e) {
                return luaL_error(state, e.what());
            }
            
            return 0;
        }
        else {
            return luaL_error(state, "Invalid number of arguments in function Engine.userInterface.playSound.");
        }
    }

    static int engine_get_sound_permutation_samples_duration(lua_State *state) noexcept {
        int args = lua_gettop(state);

        if(args == 1) {
            auto *permutation = reinterpret_cast<LegacyApi::Engine::TagDefinitions::SoundPermutation *>(luacs_checkobject(state, 1, "EngineSoundPermutation"));
            if(!permutation) {
                return luaL_error(state, "Invalid sound permutation in function Engine.userInterface.getSoundPermutationSamplesDuration.");
            }
            try {
                auto duration = LegacyApi::Engine::get_sound_permutation_samples_duration(permutation);
                lua_pushinteger(state, duration.count());
                return 1;
            }
            catch(std::runtime_error &e) {
                return luaL_error(state, e.what());
            }
        }
        else {
            return luaL_error(state, "Invalid number of arguments in function Engine.userInterface.getSoundPermutationSamplesDuration.");
        }
    }

    static int engine_draw_bitmap_in_rect(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 3 || args == 4) {
            auto *bitmap_data = reinterpret_cast<LegacyApi::Engine::TagDefinitions::BitmapData *>(luacs_checkobject(state, 1, "EngineBitmapData"));
            if(!bitmap_data) {
                return luaL_error(state, "Invalid bitmap data in function Engine.userInterface.drawBitmapInRect.");
            }

            auto rect = get_rectangle2_d(state, 2);
            if(!rect) {
                return luaL_error(state, "Invalid rect in function Engine.userInterface.drawBitmapInRect.");
            }

            if(args == 3) {
                auto color = get_color_a_r_g_b_int(state, 3);
                if(!color) {
                    return luaL_error(state, "Invalid color in function Engine.userInterface.drawBitmapInRect.");
                }

                LegacyApi::Engine::draw_bitmap_in_rect(bitmap_data, *rect, *color);
            }
            else {
                auto bounds = get_rectangle2_d(state, 3);
                if(!bounds) {
                    return luaL_error(state, "Invalid bounds in function Engine.userInterface.drawBitmapInRect.");
                }

                auto color = get_color_a_r_g_b_int(state, 4);
                if(!color) {
                    return luaL_error(state, "Invalid color in function Engine.userInterface.drawBitmapInRect.");
                }

                LegacyApi::Engine::draw_bitmap_in_rect(bitmap_data, *rect, *bounds, *color);
            }
            return 0;
        }
        else {
            return luaL_error(state, "Invalid number of arguments in function Engine.userInterface.drawBitmapInRect.");
        }
    }

    static const luaL_Reg engine_user_interface_functions[] = {
        {"findWidget", engine_find_widget},
        {"findWidgets", engine_find_widgets},
        {"openWidget", engine_open_widget},
        {"closeWidget", engine_close_widget},
        {"replaceWidget", engine_replace_widget},
        {"reloadWidget", engine_reload_widget},
        {"focusWidget", engine_focus_widget},
        {"getRootWidget", engine_get_root_widget},
        {"openPauseMenu", engine_open_pause_menu},
        {"getHudGlobals", engine_get_hud_globals},
        {"getBitmapSpriteResolution", get_bitmap_sprite_resolution},
        {"drawHudMessageSprite", engine_draw_hud_message_sprite},
        {"playSound", engine_play_sound},
        {"getSoundPermutationSamplesDuration", engine_get_sound_permutation_samples_duration},
        {"drawBitmapInRect", engine_draw_bitmap_in_rect},
        {nullptr, nullptr}
    };

    void set_engine_user_interface_functions(lua_State *state) noexcept {
        push_plugin_functions_table(state, "userInterface", -1, engine_user_interface_functions);
    }
}