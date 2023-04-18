// SPDX-License-Identifier: GPL-3.0-only

#include <lua.hpp>
#include <balltze/engine.hpp>
#include "../../logger.hpp"
#include "../plugin.hpp"
#include "../loader.hpp"
#include "helpers.hpp"

extern "C" {
    #include "lfmt.h"
}

namespace Balltze::Plugins {
    static int lua_engine_console_print(lua_State *state) noexcept {
        auto plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 1 || args == 2) {
                const char *text = luaL_checkstring(state, 1);
                if(args == 2) {
                    auto color = lua_to_color_argb(state, 2);
                    Engine::console_print(text, color);
                }
                else {
                    Engine::console_print(text);
                }
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function engine.console.print.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
        return 0;
    }

    static int lua_engine_console_printf(lua_State *state) noexcept {
        auto plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args >= 1) {
                bool has_color = lua_istable(state, 1);

                Engine::ColorARGB color;
                if(has_color) {
                    try {
                        color = lua_to_color_argb(state, 1);
                    }
                    catch(const std::exception &e) {
                        return luaL_error(state, "Invalid color argument in function engine.console.printf.");
                    }
                }
                else {
                    color = {1.0, 1.0, 1.0, 1.0};
                }

                std::string message; 
                if(args > 1) { 
                    if(has_color) {
                        lua_remove(state, 1); 
                    }
                    Lformat(state); 
                    message = luaL_checkstring(state, -1); 
                    lua_pop(state, 1); 
                } 
                else { 
                    message = luaL_checkstring(state, 2); 
                }

                Engine::console_print(message, color);
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

    static int lua_engine_get_resolution(lua_State *state) noexcept {
        auto plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 0) {
                auto resolution = Engine::get_resolution();
                lua_newtable(state);
                lua_pushinteger(state, resolution.width);
                lua_setfield(state, -2, "width");
                lua_pushinteger(state, resolution.height);
                lua_setfield(state, -2, "height");
                return 1;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function engine.get_resolution.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static int lua_engine_get_tick_count(lua_State *state) noexcept {
        auto plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 0) {
                lua_pushinteger(state, Engine::get_tick_count());
                return 1;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function engine.get_tick_count.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static int lua_engine_get_engine_edition(lua_State *state) noexcept {
        auto plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 0) {
                auto edition = Engine::get_engine_edition();
                switch(edition) {
                    case Engine::ENGINE_TYPE_RETAIL:
                        lua_pushstring(state, "retail");
                        break;
                    case Engine::ENGINE_TYPE_DEMO:
                        lua_pushstring(state, "demo");
                        break;
                    case Engine::ENGINE_TYPE_CUSTOM_EDITION:
                        lua_pushstring(state, "custom");
                        break;
                    default:
                        lua_pushstring(state, "unknown");
                        break;
                }
                return 1;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function engine.get_engine_edition.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static int lua_engine_get_current_map_header(lua_State *state) noexcept {
        auto plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 0) {
                auto edition = Engine::get_engine_edition();
                if(edition != Engine::ENGINE_TYPE_DEMO) {
                    auto header = Engine::get_map_header();

                    lua_newtable(state);
                    
                    auto cache_file_edition = cache_file_engine_to_string(header.engine_type);
                    lua_pushstring(state, cache_file_edition.c_str());
                    lua_setfield(state, -2, "engine_type");

                    lua_pushinteger(state, header.file_size);
                    lua_setfield(state, -2, "file_size");

                    lua_pushinteger(state, header.tag_data_offset);
                    lua_setfield(state, -2, "tag_data_offset");

                    lua_pushinteger(state, header.tag_data_size);
                    lua_setfield(state, -2, "tag_data_size");

                    lua_pushstring(state, header.name);
                    lua_setfield(state, -2, "name");

                    lua_pushstring(state, header.build);
                    lua_setfield(state, -2, "build");

                    auto map_game_type = map_game_type_to_string(header.game_type);
                    lua_pushstring(state, map_game_type.c_str());
                    lua_setfield(state, -2, "game_type");

                    lua_pushinteger(state, header.crc32);
                    lua_setfield(state, -2, "crc32");

                    return 1;
                }
                else {
                    auto demo_header = Engine::get_demo_map_header();

                    lua_newtable(state);

                    auto map_game_type = map_game_type_to_string(demo_header.game_type);
                    lua_pushstring(state, map_game_type.c_str());
                    lua_setfield(state, -2, "game_type");

                    lua_pushinteger(state, demo_header.tag_data_size);
                    lua_setfield(state, -2, "tag_data_size");

                    lua_pushstring(state, demo_header.build);
                    lua_setfield(state, -2, "build");

                    auto cache_file_edition = cache_file_engine_to_string(demo_header.engine_type);
                    lua_pushstring(state, cache_file_edition.c_str());
                    lua_setfield(state, -2, "engine_type");

                    lua_pushstring(state, demo_header.name);
                    lua_setfield(state, -2, "name");

                    lua_pushinteger(state, demo_header.crc32);
                    lua_setfield(state, -2, "crc32");

                    lua_pushinteger(state, demo_header.file_size);
                    lua_setfield(state, -2, "file_size");

                    lua_pushinteger(state, demo_header.tag_data_offset);
                    lua_setfield(state, -2, "tag_data_offset");

                    return 1;
                }
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function engine.get_current_map_header.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static const luaL_Reg engine_functions[] = {
        {"console_print", lua_engine_console_print},
        {"console_printf", lua_engine_console_printf},
        {"get_resolution", lua_engine_get_resolution},
        {"get_tick_count", lua_engine_get_tick_count},
        {"get_engine_edition", lua_engine_get_engine_edition},
        {"get_current_map_header", lua_engine_get_current_map_header},
        {nullptr, nullptr}
    };

    void lua_set_engine_table(lua_State *state) noexcept {
        lua_create_functions_table(state, "engine", engine_functions);
    }
}
