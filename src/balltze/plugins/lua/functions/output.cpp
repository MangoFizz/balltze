// SPDX-License-Identifier: GPL-3.0-only

#include <string>
#include <lua.hpp>
#include <balltze/output.hpp>
#include "../helpers/function_table.hpp"
#include "../types.hpp"
#include "../../loader.hpp"
#include "../../../logger.hpp"

namespace Balltze::Plugins::Lua {
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
                return luaL_error(state, "Invalid number of arguments in function Balltze.output.getGenericFont.");
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
                else if(lua_isinteger(state, 1) || lua_istable(state, 1)) {
                    auto tag_handle = get_engine_resource_handle(state, 1);
                    if(!tag_handle || tag_handle->is_null()) {
                        return luaL_error(state, "Invalid tag handle in function Balltze.output.textPixelLength.");
                    }
                    auto *tag = Engine::get_tag(*tag_handle);
                    if(!tag) {
                        return luaL_error(state, "Could not find tag in function Balltze.output.textPixelLength.");
                    }
                    if(tag->primary_class != Engine::TAG_CLASS_FONT) {
                        return luaL_error(state, "Tag is not a font in function Balltze.output.textPixelLength.");
                    }
                    font = *tag_handle;
                }
                else {
                    return luaL_error(state, "Invalid argument type in function Balltze.output.textPixelLength.");
                }

                auto length = get_text_pixel_length(text, font);
                lua_pushinteger(state, length);
                return 1;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function Balltze.output.textPixelLength.");
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
                else if(lua_isinteger(state, 1) || lua_istable(state, 1)) {
                    auto tag_handle = get_engine_resource_handle(state, 1);
                    if(!tag_handle || tag_handle->is_null()) {
                        return luaL_error(state, "Invalid tag handle in function Balltze.output.textPixelLength.");
                    }
                    auto *tag = Engine::get_tag(*tag_handle);
                    if(!tag) {
                        return luaL_error(state, "Could not find tag in function Balltze.output.textPixelLength.");
                    }
                    if(tag->primary_class != Engine::TAG_CLASS_FONT) {
                        return luaL_error(state, "Tag is not a font in function Balltze.output.textPixelLength.");
                    }
                    font = *tag_handle;
                }
                else {
                    return luaL_error(state, "Invalid argument type in function Balltze.output.fontPixelHeight.");
                }

                auto height = get_font_pixel_height(font);
                lua_pushinteger(state, height);
                return 1;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function Balltze.output.fontPixelHeight.");
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
                auto color = get_color_a_r_g_b(state, 3);
                if(!color) {
                    return luaL_error(state, "Invalid color in function Balltze.output.addSubtitle.");
                }
                std::chrono::milliseconds duration_ms(duration);
                add_subtitle(text, *color, duration_ms);
                return 0;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function Balltze.output.addSubtitle.");
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
                return luaL_error(state, "Invalid number of arguments in function Balltze.output.playBikVideo.");
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

    void set_output_table(lua_State *state) noexcept {
        create_functions_table(state, "output", output_functions);
    }
}
