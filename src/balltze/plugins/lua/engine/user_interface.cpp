// SPDX-License-Identifier: GPL-3.0-only

#include <lua.hpp>
#include <balltze/engine.hpp>
#include "../../../logger.hpp"
#include "../../plugin.hpp"
#include "../../loader.hpp"
#include "../helpers.hpp"

namespace Balltze::Plugins {
    static int lua_engine_find_widget(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 1 || args == 2) {
                Engine::TagHandle tag_handle;

                if(lua_isinteger(state, 1)) {
                    tag_handle.handle = luaL_checkinteger(state, 1);
                    auto *tag = Engine::get_tag(tag_handle);
                    if(!tag) {
                        return luaL_error(state, "Could not find tag.");
                    }
                    if(tag->primary_class != Engine::TAG_CLASS_UI_WIDGET_DEFINITION) {
                        return luaL_error(state, "Tag is not a widget definition.");
                    }
                }
                else {
                    const char *tag_path = luaL_checkstring(state, 1);
                    auto *tag = Engine::get_tag(tag_path, Engine::TAG_CLASS_UI_WIDGET_DEFINITION);
                    if(!tag) {
                        return luaL_error(state, "Could not find tag.");
                    }
                    tag_handle.handle = tag->handle.handle;
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
                return luaL_error(state, "Invalid number of arguments in function engine.find_widget.");
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

                if(lua_isinteger(state, 1)) {
                    tag_handle.handle = luaL_checkinteger(state, 1);
                    auto *tag = Engine::get_tag(tag_handle);
                    if(!tag) {
                        return luaL_error(state, "Could not find tag.");
                    }
                    if(tag->primary_class != Engine::TAG_CLASS_UI_WIDGET_DEFINITION) {
                        return luaL_error(state, "Tag is not a widget definition.");
                    }
                }
                else {
                    const char *tag_path = luaL_checkstring(state, 1);
                    auto *tag = Engine::get_tag(tag_path, Engine::TAG_CLASS_UI_WIDGET_DEFINITION);
                    if(!tag) {
                        return luaL_error(state, "Could not find tag.");
                    }
                    tag_handle.handle = tag->handle.handle;
                }

                Engine::Widget *base_widget = nullptr;
                if(args == 2) {
                    base_widget = lua_from_meta_object<Engine::Widget>(state, 2);
                    if(!base_widget) {
                        return luaL_error(state, "Invalid base widget.");
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
                return luaL_error(state, "Invalid number of arguments in function engine.find_widgets.");
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

                if(lua_isinteger(state, 1)) {
                    tag_handle.handle = luaL_checkinteger(state, 1);
                    auto *tag = Engine::get_tag(tag_handle);
                    if(!tag) {
                        return luaL_error(state, "Could not find tag.");
                    }
                    if(tag->primary_class != Engine::TAG_CLASS_UI_WIDGET_DEFINITION) {
                        return luaL_error(state, "Tag is not a widget definition.");
                    }
                }
                else {
                    const char *tag_path = luaL_checkstring(state, 1);
                    auto *tag = Engine::get_tag(tag_path, Engine::TAG_CLASS_UI_WIDGET_DEFINITION);
                    if(!tag) {
                        return luaL_error(state, "Could not find tag.");
                    }
                    tag_handle.handle = tag->handle.handle;
                }

                bool push_history = false;
                if(args == 2) {
                    if(lua_isboolean(state, 2)) {
                        push_history = lua_toboolean(state, 2);
                    }
                    else {
                        return luaL_error(state, "Invalid push history argument.");
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
                return luaL_error(state, "Invalid number of arguments in function engine.open_widget.");
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
                return luaL_error(state, "Invalid number of arguments in function engine.close_widget.");
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
                    return luaL_error(state, "Invalid target widget.");
                }

                Engine::TagHandle tag_handle;
                if(lua_isinteger(state, 2)) {
                    tag_handle.handle = luaL_checkinteger(state, 2);
                    auto *tag = Engine::get_tag(tag_handle);
                    if(!tag) {
                        return luaL_error(state, "Could not find tag.");
                    }
                    if(tag->primary_class != Engine::TAG_CLASS_UI_WIDGET_DEFINITION) {
                        return luaL_error(state, "Tag is not a widget definition.");
                    }
                }
                else {
                    const char *tag_path = luaL_checkstring(state, 2);
                    auto *tag = Engine::get_tag(tag_path, Engine::TAG_CLASS_UI_WIDGET_DEFINITION);
                    if(!tag) {
                        return luaL_error(state, "Could not find tag.");
                    }
                    tag_handle.handle = tag->handle.handle;
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
                return luaL_error(state, "Invalid number of arguments in function engine.replace_widget.");
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
                    return luaL_error(state, "Invalid target widget.");
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
                return luaL_error(state, "Invalid number of arguments in function engine.replace_widget.");
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
                    return luaL_error(state, "Invalid target widget.");
                }

                Engine::focus_widget(target_widget);
                return 0;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function engine.focus_widget.");
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
                return luaL_error(state, "Invalid number of arguments in function engine.open_pause_menu.");
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
                return luaL_error(state, "Invalid number of arguments in function engine.get_hud_globals.");
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
                    return luaL_error(state, "Invalid bitmap.");
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
                return luaL_error(state, "Invalid number of arguments in function engine.get_bitmap_sprite_resolution.");
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
                    return luaL_error(state, "Invalid bitmap.");
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
                return luaL_error(state, "Invalid number of arguments in function engine.draw_hud_message_sprite.");
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
                if(lua_isinteger(state, 1)) {
                    tag_handle.handle = luaL_checkinteger(state, 1);
                    auto *tag = Engine::get_tag(tag_handle);
                    if(!tag) {
                        return luaL_error(state, "Could not find tag.");
                    }
                    if(tag->primary_class != Engine::TAG_CLASS_SOUND) {
                        return luaL_error(state, "Tag is not a sound.");
                    }
                }
                else {
                    const char *tag_path = luaL_checkstring(state, 1);
                    auto *tag = Engine::get_tag(tag_path, Engine::TAG_CLASS_UI_WIDGET_DEFINITION);
                    if(!tag) {
                        return luaL_error(state, "Could not find tag.");
                    }
                    tag_handle.handle = tag->handle.handle;
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
                return luaL_error(state, "Invalid number of arguments in function engine.play_sound.");
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
                return luaL_error(state, "Invalid number of arguments in function engine.get_sound_permutation_samples_duration.");
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
        {"openPauseMenu", lua_engine_open_pause_menu},
        {"getHudGlobals", lua_engine_get_hud_globals},
        {"getBitmapSpriteResolution", lua_get_bitmap_sprite_resolution},
        {"drawHudMessageSprite", lua_engine_draw_hud_message_sprite},
        {"playSound", lua_engine_play_sound},
        {"getSoundPermutationSamplesDuration", lua_engine_get_sound_permutation_samples_duration},
        {nullptr, nullptr}
    };

    void set_engine_multiplayer_functions(lua_State *state) noexcept {
        luaL_newlibtable(state, engine_user_interface_functions);
        luaL_setfuncs(state, engine_user_interface_functions, 0);
        lua_setglobal(state, "userInterface");
    }
}