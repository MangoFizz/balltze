// SPDX-License-Identifier: GPL-3.0-only

#include <lua.hpp>
#include <balltze/engine.hpp>
#include "../../../../logger.hpp"
#include "../../../plugin.hpp"
#include "../../../loader.hpp"
#include "../../libraries.hpp"
#include "../../helpers.hpp"
#include "../../metatables.hpp"

namespace Balltze::Plugins {
    static int lua_engine_find_widget(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 1 || args == 2) {
                Engine::TagHandle tag_handle;

                if(lua_isinteger(state, 1) || lua_istable(state, 1)) {
                    tag_handle = lua_to_engine_tag_handle(state, 1);
                    if(tag_handle.is_null()) {
                        return luaL_error(state, "Invalid tag handle in function Engine.userInterface.findWidget.");
                    }
                    auto *tag = Engine::get_tag(tag_handle);
                    if(!tag) {
                        return luaL_error(state, "Could not find tag in function Engine.userInterface.findWidget.");
                    }
                    if(tag->primary_class != Engine::TAG_CLASS_UI_WIDGET_DEFINITION) {
                        return luaL_error(state, "Tag is not a widget definition in function Engine.userInterface.findWidget.");
                    }
                }
                else {
                    const char *tag_path = luaL_checkstring(state, 1);
                    auto *tag = Engine::get_tag(tag_path, Engine::TAG_CLASS_UI_WIDGET_DEFINITION);
                    if(!tag) {
                        return luaL_error(state, "Could not find tag in function Engine.userInterface.findWidget.");
                    }
                    tag_handle = tag->handle;
                }

                Engine::Widget *base_widget = nullptr;
                if(args == 2) {
                    base_widget = lua_from_meta_object<Engine::Widget>(state, 2);
                    if(!base_widget) {
                        return luaL_error(state, "Invalid base widget.");
                    }
                }

                auto search_result = Engine::find_widgets(tag_handle, true, base_widget);
                if(!search_result.empty()) {
                    auto *widget = search_result[0];
                    lua_push_meta_engine_widget(state, *widget);
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
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static int lua_engine_find_widgets(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);

            if(args == 1 || args == 2) {
                Engine::TagHandle tag_handle;

                if(lua_isinteger(state, 1) || lua_istable(state, 1)) {
                    tag_handle = lua_to_engine_tag_handle(state, 1);
                    if(tag_handle.is_null()) {
                        return luaL_error(state, "Invalid tag handle in function Engine.userInterface.findWidgets.");
                    }
                    auto *tag = Engine::get_tag(tag_handle);
                    if(!tag) {
                        return luaL_error(state, "Could not find tag in function Engine.userInterface.findWidgets.");
                    }
                    if(tag->primary_class != Engine::TAG_CLASS_UI_WIDGET_DEFINITION) {
                        return luaL_error(state, "Tag is not a widget definition in function Engine.userInterface.findWidgets.");
                    }
                }
                else {
                    const char *tag_path = luaL_checkstring(state, 1);
                    auto *tag = Engine::get_tag(tag_path, Engine::TAG_CLASS_UI_WIDGET_DEFINITION);
                    if(!tag) {
                        return luaL_error(state, "Could not find tag in function Engine.userInterface.findWidget.");
                    }
                    tag_handle = tag->handle;
                }

                Engine::Widget *base_widget = nullptr;
                if(args == 2) {
                    base_widget = lua_from_meta_object<Engine::Widget>(state, 2);
                    if(!base_widget) {
                        return luaL_error(state, "Invalid base widget in function Engine.userInterface.findWidgets.");
                    }
                }

                auto search_result = Engine::find_widgets(tag_handle, false, base_widget);
                lua_newtable(state);
                for(std::size_t i = 0; i < search_result.size(); i++) {
                    lua_push_meta_engine_widget(state, *search_result[i]);
                    lua_rawseti(state, -2, i + 1);
                }
                return 1;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function Engine.userInterface.findWidgets.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }    
    }

    static int lua_engine_open_widget(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);

            if(args == 1 || args == 2) {
                Engine::TagHandle tag_handle;

                if(lua_isinteger(state, 1) || lua_istable(state, 1)) {
                    tag_handle = lua_to_engine_tag_handle(state, 1);
                    if(tag_handle.is_null()) {
                        return luaL_error(state, "Invalid tag handle in function Engine.userInterface.openWidget.");
                    }
                    auto *tag = Engine::get_tag(tag_handle);
                    if(!tag) {
                        return luaL_error(state, "Could not find tag in function Engine.userInterface.openWidget.");
                    }
                    if(tag->primary_class != Engine::TAG_CLASS_UI_WIDGET_DEFINITION) {
                        return luaL_error(state, "Tag is not a widget definition in function Engine.userInterface.openWidget.");
                    }
                }
                else {
                    const char *tag_path = luaL_checkstring(state, 1);
                    auto *tag = Engine::get_tag(tag_path, Engine::TAG_CLASS_UI_WIDGET_DEFINITION);
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

                auto *widget = Engine::open_widget(tag_handle, push_history);
                if(widget) {
                    lua_push_meta_engine_widget(state, *widget);
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
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }    
    }

    static int lua_engine_close_widget(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 0) {
                Engine::close_widget();
                return 0;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function Engine.userInterface.closeWidget.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }   
    }

    static int lua_engine_replace_widget(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 2) {
                Engine::Widget *target_widget = lua_from_meta_object<Engine::Widget>(state, 1);
                if(!target_widget) {
                    return luaL_error(state, "Invalid target widget in function Engine.userInterface.replaceWidget.");
                }

                Engine::TagHandle tag_handle;
                if(lua_isinteger(state, 1) || lua_istable(state, 1)) {
                    tag_handle = lua_to_engine_tag_handle(state, 2);
                    if(tag_handle.is_null()) {
                        return luaL_error(state, "Invalid tag handle in function Engine.userInterface.replaceWidget.");
                    }
                    auto *tag = Engine::get_tag(tag_handle);
                    if(!tag) {
                        return luaL_error(state, "Could not find tag in function Engine.userInterface.replaceWidget.");
                    }
                    if(tag->primary_class != Engine::TAG_CLASS_UI_WIDGET_DEFINITION) {
                        return luaL_error(state, "Tag is not a widget definition in function Engine.userInterface.replaceWidget.");
                    }
                }
                else {
                    const char *tag_path = luaL_checkstring(state, 2);
                    auto *tag = Engine::get_tag(tag_path, Engine::TAG_CLASS_UI_WIDGET_DEFINITION);
                    if(!tag) {
                        return luaL_error(state, "Could not find tag in function Engine.userInterface.replaceWidget.");
                    }
                    tag_handle = tag->handle;
                }

                auto *widget = Engine::replace_widget(target_widget, tag_handle);
                if(widget) {
                    lua_push_meta_engine_widget(state, *widget);
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
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static int lua_engine_reload_widget(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 1) {
                Engine::Widget *target_widget = lua_from_meta_object<Engine::Widget>(state, 1);
                if(!target_widget) {
                    return luaL_error(state, "Invalid target widget in function Engine.userInterface.reloadWidget.");
                }

                auto *widget = Engine::reload_widget(target_widget);
                if(widget) {
                    lua_push_meta_engine_widget(state, *widget);
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
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }    
    }

    static int lua_engine_focus_widget(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 1) {
                Engine::Widget *target_widget = lua_from_meta_object<Engine::Widget>(state, 1);
                if(!target_widget) {
                    return luaL_error(state, "Invalid target widget in function Engine.userInterface.focusWidget.");
                }

                Engine::focus_widget(target_widget);
                return 0;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function Engine.userInterface.focusWidget.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }    
    }

    static int lua_engine_get_root_widget(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 0) {
                auto *root_widget = Engine::get_root_widget();
                if(root_widget) {
                    lua_push_meta_engine_widget(state, *root_widget);
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
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }    
    }

    static int lua_engine_open_pause_menu(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 0) {
                Engine::open_pause_menu();
                return 0;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function Engine.userInterface.openPauseMenu.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }   
    }

    extern void lua_push_meta_engine_hud_globals(lua_State *state, Engine::TagDefinitions::HudGlobals &data) noexcept; 

    static int lua_engine_get_hud_globals(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 0) {
                auto &hud_globals = Engine::get_hud_globals();
                lua_push_meta_engine_hud_globals(state, hud_globals);
                return 1;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function Engine.userInterface.getHudGlobals.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static int lua_get_bitmap_sprite_resolution(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);

            if(args == 3) {
                auto *bitmap = lua_from_meta_object<Engine::TagDefinitions::Bitmap>(state, 1);
                if(!bitmap) {
                    return luaL_error(state, "Invalid bitmap in function Engine.userInterface.getBitmapSpriteResolution.");
                }

                auto sequence_index = luaL_checkinteger(state, 2);
                auto sprite_index = luaL_checkinteger(state, 3);

                try {
                    auto resolution = Engine::get_bitmap_sprite_resolution(bitmap, sequence_index, sprite_index);
                    lua_push_engine_resolution(state, resolution);
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
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }    
    }

    static int lua_engine_draw_hud_message_sprite(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);

            if(args == 4 || args == 5) {
                auto *bitmap = lua_from_meta_object<Engine::TagDefinitions::Bitmap>(state, 1);
                if(!bitmap) {
                    return luaL_error(state, "Invalid bitmap in function Engine.userInterface.drawHudMessageSprite.");
                }

                auto sequence_index = luaL_checkinteger(state, 2);
                auto sprite_index = luaL_checkinteger(state, 3);

                try {
                    auto position = lua_to_point2_d_int(state, 4);
                    Engine::ColorARGBInt color = {255, 255, 255, 255};
                    if(args == 5) {
                        color = lua_to_color_a_r_g_b_int(state, 5);
                    }
                    Engine::draw_hud_message_sprite(bitmap, sequence_index, sprite_index, position, color);
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
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }    
    }

    static int lua_engine_play_sound(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);

            if(args == 1) {
                Engine::TagHandle tag_handle;
                if(lua_isinteger(state, 1) || lua_istable(state, 1)) {
                    tag_handle = lua_to_engine_tag_handle(state, 1);
                    if(tag_handle.is_null()) {
                        return luaL_error(state, "Invalid tag handle in function Engine.userInterface.playSound.");
                    }
                    auto *tag = Engine::get_tag(tag_handle);
                    if(!tag) {
                        return luaL_error(state, "Could not find tag in function Engine.userInterface.playSound.");
                    }
                    if(tag->primary_class != Engine::TAG_CLASS_SOUND) {
                        return luaL_error(state, "Tag is not a sound in function Engine.userInterface.playSound.");
                    }
                }
                else {
                    const char *tag_path = luaL_checkstring(state, 1);
                    auto *tag = Engine::get_tag(tag_path, Engine::TAG_CLASS_UI_WIDGET_DEFINITION);
                    if(!tag) {
                        return luaL_error(state, "Could not find tag in function Engine.userInterface.playSound.");
                    }
                    tag_handle = tag->handle;
                }

                try {
                    Engine::play_sound(tag_handle);
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
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }    
    }

    static int lua_engine_get_sound_permutation_samples_duration(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);

            if(args == 1) {
                auto *permutation = lua_from_meta_object<Engine::TagDefinitions::SoundPermutation>(state, 1);
                try {
                    auto duration = Engine::get_sound_permutation_samples_duration(permutation);
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
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }    
    }

    static const luaL_Reg engine_user_interface_functions[] = {
        {"findWidget", lua_engine_find_widget},
        {"findWidgets", lua_engine_find_widgets},
        {"openWidget", lua_engine_open_widget},
        {"closeWidget", lua_engine_close_widget},
        {"replaceWidget", lua_engine_replace_widget},
        {"reloadWidget", lua_engine_reload_widget},
        {"focusWidget", lua_engine_focus_widget},
        {"getRootWidget", lua_engine_get_root_widget},
        {"openPauseMenu", lua_engine_open_pause_menu},
        {"getHudGlobals", lua_engine_get_hud_globals},
        {"getBitmapSpriteResolution", lua_get_bitmap_sprite_resolution},
        {"drawHudMessageSprite", lua_engine_draw_hud_message_sprite},
        {"playSound", lua_engine_play_sound},
        {"getSoundPermutationSamplesDuration", lua_engine_get_sound_permutation_samples_duration},
        {nullptr, nullptr}
    };

    void set_engine_user_interface_functions(lua_State *state) noexcept {
        luaL_newlibtable(state, engine_user_interface_functions);
        luaL_setfuncs(state, engine_user_interface_functions, 0);
        lua_setfield(state, -2, "userInterface");
    }
}