// SPDX-License-Identifier: GPL-3.0-only

#include <string>
#include <lua.hpp>
#include <balltze/output.hpp>
#include "../helpers.hpp"
#include "../../loader.hpp"
#include "../../../logger.hpp"

namespace Balltze::Plugins {
    static int lua_get_generic_font(lua_State *state) {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 1) {
                auto font_str = generic_font_from_string(luaL_checkstring(state, 1));
                auto font = get_generic_font(font_str);
                lua_pushinteger(state, font.handle);
                return 1;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function engine.console.printf.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
        return 0;
    } 

    static int lua_text_pixel_length(lua_State *state) {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 2) {
                auto text = luaL_checkstring(state, 1);
                std::variant<Engine::TagHandle, GenericFont> font;

                if(lua_isstring(state, 2)) {
                    auto font_str = generic_font_from_string(luaL_checkstring(state, 2));
                    font = get_generic_font(font_str);
                }
                else if(lua_isinteger(state, 2)) {
                    auto font_handle = luaL_checkinteger(state, 2);
                    font = Engine::TagHandle(font_handle);
                }
                else {
                    return luaL_error(state, "Invalid argument type in function text_pixel_length.");
                }

                auto length = get_text_pixel_length(text, font);
                lua_pushinteger(state, length);
                return 1;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function engine.console.printf.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
        return 0;
    }

    static int lua_font_pixel_height(lua_State *state) {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 1) {
                std::variant<Engine::TagHandle, GenericFont> font;
                if(lua_isstring(state, 1)) {
                    auto font_str = generic_font_from_string(luaL_checkstring(state, 1));
                    font = get_generic_font(font_str);
                }
                else if(lua_isinteger(state, 1)) {
                    auto font_handle = luaL_checkinteger(state, 1);
                    font = Engine::TagHandle(font_handle);
                }
                else {
                    return luaL_error(state, "Invalid argument type in function font_pixel_height.");
                }

                auto height = get_font_pixel_height(font);
                lua_pushinteger(state, height);
                return 1;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function engine.console.printf.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
        return 0;
    }

    static int lua_add_subtitle(lua_State *state) {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 3) {
                auto text = luaL_checkstring(state, 1);
                auto duration = luaL_checkinteger(state, 2);
                auto color = lua_to_color_argb(state, 3);
                std::chrono::milliseconds duration_ms(duration);
                add_subtitle(text, color, duration_ms);
                return 0;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function engine.console.printf.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
        return 0;
    }

    static int lua_play_bik_video(lua_State *state) {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 1) {
                auto video = luaL_checkstring(state, 1);
                play_bik_video(video);
                return 0;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function engine.console.printf.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
        return 0;
    }

    static const luaL_Reg output_functions[] = {
        {"getGenericFont", lua_get_generic_font},
        {"getTextPixelLength", lua_text_pixel_length},
        {"getFontPixelHeight", lua_font_pixel_height},
        {"addSubtitle", lua_add_subtitle},
        {"playBikVideo", lua_play_bik_video},
        {nullptr, nullptr}
    };

    void lua_set_output_table(lua_State *state) noexcept {
        lua_create_functions_table(state, "output", output_functions);
    }
}
