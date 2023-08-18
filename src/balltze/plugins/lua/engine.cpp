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
        auto *plugin = get_lua_plugin(state);
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
                    message = luaL_checkstring(state, 1); 
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
        auto *plugin = get_lua_plugin(state);
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
        auto *plugin = get_lua_plugin(state);
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
        auto *plugin = get_lua_plugin(state);
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
        auto *plugin = get_lua_plugin(state);
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

    static int lua_engine_get_map_list(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 0) {
                auto map_list = Engine::get_map_list();
                auto engine_edition = Engine::get_engine_edition();
                if(engine_edition == Engine::ENGINE_TYPE_RETAIL) {
                    auto *map_list_array = reinterpret_cast<Engine::MapIndexRetail *>(map_list.map_list);
                    lua_newtable(state);
                    for(std::size_t i = 0; i < map_list.map_count; i++) {
                        lua_pushstring(state, map_list_array[i].file_name);
                        lua_rawseti(state, -2, i + 1);
                    }
                }
                else if(engine_edition == Engine::ENGINE_TYPE_CUSTOM_EDITION) {
                    auto *map_list_array = reinterpret_cast<Engine::MapIndexCustomEdition *>(map_list.map_list);
                    lua_newtable(state);
                    for(std::size_t i = 0; i < map_list.map_count; i++) {
                        lua_pushstring(state, map_list_array[i].file_name);
                        lua_rawseti(state, -2, i + 1);
                    }
                }
                else {
                    auto *map_list_array = reinterpret_cast<Engine::MapIndex *>(map_list.map_list);
                    lua_newtable(state);
                    for(std::size_t i = 0; i < map_list.map_count; i++) {
                        lua_pushstring(state, map_list_array[i].file_name);
                        lua_rawseti(state, -2, i + 1);
                    }
                }
                return 1;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function engine.get_map_list.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static int lua_engine_get_server_type(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 0) {
                auto server_type = Engine::get_server_type();
                auto server_type_string = server_type_to_string(server_type);
                lua_pushstring(state, server_type_string.c_str());
                return 1;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function engine.get_server_type.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static int lua_engine_get_server_gametype(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 0) {
                auto server_gametype = Engine::get_current_game_type();
                auto server_gametype_string = server_game_type_to_string(server_gametype);
                if(server_gametype_string == "none") {
                    lua_pushnil(state);
                }
                else {
                    lua_pushstring(state, server_gametype_string.c_str());
                }
                return 1;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function engine.get_server_gametype.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static int lua_engine_current_game_is_team(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 0) {
                bool is_team_game = Engine::current_game_is_team();
                lua_pushboolean(state, is_team_game);
                return 1;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function engine.current_game_is_team.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static int lua_engine_get_tag_data_header(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 0) {
                auto tag_data_header = Engine::get_tag_data_header();
                
                lua_newtable(state);

                lua_pushinteger(state, reinterpret_cast<std::uint32_t>(tag_data_header.tag_array));
                lua_setfield(state, -2, "tag_array");

                lua_pushinteger(state, tag_data_header.scenario_tag.handle);
                lua_setfield(state, -2, "scenario_tag");

                lua_pushinteger(state, tag_data_header.tag_count);
                lua_setfield(state, -2, "tag_count");

                lua_pushinteger(state, tag_data_header.model_part_count);
                lua_setfield(state, -2, "model_part_count");

                lua_pushinteger(state, tag_data_header.model_data_file_offset);
                lua_setfield(state, -2, "model_data_file_offset");

                lua_pushinteger(state, tag_data_header.vertex_size);
                lua_setfield(state, -2, "vertex_size");

                lua_pushinteger(state, tag_data_header.model_data_size);
                lua_setfield(state, -2, "model_data_size");

                return 1;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function engine.get_tag_data_header.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static int lua_engine_get_tag(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            Engine::Tag *tag_entry = nullptr;
            if(args == 1) {
                auto tag_number_thing = lua_tointeger(state, 1);
                if(tag_number_thing < 0xFFFF) {
                    tag_entry = Engine::get_tag(tag_number_thing);
                }
                else {
                    Engine::TagHandle tag_handle;
                    tag_handle.handle = tag_number_thing;
                    tag_entry = Engine::get_tag(tag_handle);
                }
            }
            else if(args == 2) {
                const char *tag_path = luaL_checkstring(state, 1);
                const char *tag_class_str = luaL_checkstring(state, 2);
                auto tag_class = Engine::tag_class_from_string(tag_class_str);
                if(tag_class != Engine::TagClassInt::TAG_CLASS_NULL) {
                    tag_entry = Engine::get_tag(tag_path, tag_class);
                }
                else {
                    return luaL_error(state, "Invalid tag class.");
                }
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function engine.get_tag_entry.");
            }

            if(tag_entry) {
                lua_push_engine_tag(state, tag_entry);
            }
            else {
                lua_pushnil(state);
            }

            return 1;
            
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }
    
    void lua_attach_tag_data_metatable(lua_State *state) noexcept;
    
    static int lua_engine_get_tag_data(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 1 || args == 2) {
                Engine::Tag *tag_entry = nullptr;
                if(args == 2 && lua_isstring(state, 1)) {
                    const char *tag_path = lua_tostring(state, 1);
                    const char *tag_class_str = lua_tostring(state, 2);
                    auto tag_class = Engine::tag_class_from_string(tag_class_str);
                    if(tag_class != Engine::TagClassInt::TAG_CLASS_NULL) {
                        tag_entry = Engine::get_tag(tag_path, tag_class);
                    }
                    else {
                        return luaL_error(state, "Invalid tag class.");
                    }
                }
                else {
                    std::uint32_t tag_handle_or_index;
                    if(lua_istable(state, 1)) {
                        lua_getfield(state, 1, "handle");
                        tag_handle_or_index = lua_tointeger(state, -1);
                        lua_pop(state, 1);
                    }
                    else {
                        tag_handle_or_index = lua_tointeger(state, 1);
                    }

                    if(tag_handle_or_index < 0xFFFF) {
                        tag_entry = Engine::get_tag(tag_handle_or_index);
                    }
                    else {
                        Engine::TagHandle tag_handle;
                        tag_handle.handle = tag_handle_or_index;
                        tag_entry = Engine::get_tag(tag_handle);
                    }
                }

                if(!tag_entry) {
                    lua_pushnil(state);
                    return 1;
                }

                if(args == 2) {
                    const char *tag_class_str = lua_tostring(state, 2);
                    auto tag_class = Engine::tag_class_from_string(tag_class_str);
                    if(tag_class != Engine::TagClassInt::TAG_CLASS_NULL) {
                        if(tag_entry->primary_class != tag_class) {
                            return luaL_error(state, "Tag class does not match.");
                        }
                    }
                    else {
                        return luaL_error(state, "Invalid tag class.");
                    }
                }

                lua_newtable(state);
                lua_pushlightuserdata(state, tag_entry->data);
                lua_setfield(state, -2, "_tag_data");
                lua_pushinteger(state, tag_entry->primary_class);
                lua_setfield(state, -2, "_tag_class");
                lua_pushinteger(state, tag_entry->handle.handle);
                lua_setfield(state, -2, "_tag_handle");

                lua_attach_tag_data_metatable(state);

                return 1;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function engine.get_tag_data.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }
    
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

    static int lua_engine_get_camera_data(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 0) {
                auto &camera_data = Engine::get_camera_data();
                lua_push_meta_engine_camera_data(state, camera_data);
                return 1;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function engine.get_camera_data.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static int lua_engine_get_camera_type(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 0) {
                auto camera_type = Engine::get_camera_type();
                lua_pushinteger(state, camera_type);
                return 1;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function engine.get_camera_type.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static int lua_engine_get_object(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 1 || args == 2) {
                auto &object_table = Engine::get_object_table();
                Engine::BaseObject *object;
                Engine::ObjectHandle object_handle;
                object_handle.handle = luaL_checkinteger(state, 1);
                if(object_handle.id != 0) {
                    object = object_table.get_object(object_handle);
                }
                else {
                    object = object_table.get_object(object_handle.index);
                }
                if(object) {
                    try {
                        auto object_type = object_type_from_string(luaL_checkstring(state, 2));
                        if(object->type != object_type) {
                            return luaL_error(state, "Object type does not match.");
                        }
                    }
                    catch(std::runtime_error &e) {
                        return luaL_error(state, e.what());
                    }

                    switch(object->type) {
                        case Engine::OBJECT_TYPE_BIPED:
                            lua_push_meta_engine_biped_object(state, *reinterpret_cast<Engine::BipedObject *>(object));
                            break;
                        case Engine::OBJECT_TYPE_VEHICLE:
                            lua_push_meta_engine_vehicle_object(state, *reinterpret_cast<Engine::VehicleObject *>(object));
                            break;
                        case Engine::OBJECT_TYPE_WEAPON:
                            lua_push_meta_engine_weapon_object(state, *reinterpret_cast<Engine::WeaponObject *>(object));
                            break;
                        case Engine::OBJECT_TYPE_EQUIPMENT:
                            lua_push_meta_engine_equipment_object(state, *reinterpret_cast<Engine::EquipmentObject *>(object));
                            break;
                        case Engine::OBJECT_TYPE_GARBAGE:
                            lua_push_meta_engine_garbage_object(state, *reinterpret_cast<Engine::GarbageObject *>(object));
                            break;
                        case Engine::OBJECT_TYPE_PROJECTILE:
                            lua_push_meta_engine_projectile_object(state, *reinterpret_cast<Engine::ProjectileObject *>(object));
                            break;
                        case Engine::OBJECT_TYPE_SCENERY:
                            lua_push_meta_engine_object(state, *object);
                            break;
                        case Engine::OBJECT_TYPE_DEVICE_MACHINE:
                            lua_push_meta_engine_device_machine_object(state, *reinterpret_cast<Engine::DeviceMachineObject *>(object));
                            break;
                        case Engine::OBJECT_TYPE_DEVICE_CONTROL:
                            lua_push_meta_engine_device_control_object(state, *reinterpret_cast<Engine::DeviceControlObject *>(object));
                            break;
                        case Engine::OBJECT_TYPE_DEVICE_LIGHT_FIXTURE:
                            lua_push_meta_engine_device_light_fixture_object(state, *reinterpret_cast<Engine::DeviceLightFixtureObject *>(object));
                            break;
                        case Engine::OBJECT_TYPE_PLACEHOLDER:
                            lua_push_meta_engine_object(state, *object);
                            break;
                        case Engine::OBJECT_TYPE_SOUND_SCENERY:
                            lua_push_meta_engine_object(state, *object);
                            break;
                        default:
                            logger.warning("Unknown object type {}.", static_cast<int>(object->type));
                            lua_push_meta_engine_object(state, *object);
                            break;
                    }
                }
                else {
                    lua_pushnil(state);
                }
                return 1;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function engine.get_object.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static int lua_engine_create_object(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);

            if(args == 3 || args == 5) {
                auto object_tag = Engine::TagHandle(luaL_checkinteger(state, 1));
                Engine::ObjectHandle parent_object_handle = Engine::ObjectHandle::null();
                auto position = lua_to_point3_d(state, 3);
                if(!lua_isnil(state, 2)) {
                    parent_object_handle.handle = luaL_checkinteger(state, 2);
                }
                auto &object_table = Engine::get_object_table();
                auto object_handle = object_table.create_object(object_tag, position, parent_object_handle);
                if(object_handle != Engine::ObjectHandle::null()) {
                    auto *object = object_table.get_object(object_handle);
                    if(object) {
                        lua_push_meta_engine_object(state, *object);
                        return 1;
                    }
                    else {
                        return luaL_error(state, "Could not get object.");
                    }
                }
                else {
                    return luaL_error(state, "Could not create object.");
                }
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function engine.create_object.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static int lua_engine_delete_object(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);

            if(args == 1) {
                auto &object_table = Engine::get_object_table();
                if(lua_istable(state, 1)) {
                    auto *object = lua_from_meta_object<Engine::BaseObject>(state, 1);
                    if(object) {
                        auto object_handle = object->object_handle();
                        object_table.delete_object(object_handle);
                    }
                    else {
                        return luaL_error(state, "Invalid object.");
                    }
                }
                else if(lua_isinteger(state, 1)) {
                    Engine::ObjectHandle object_handle;
                    object_handle.handle = luaL_checkinteger(state, 1);
                    if(object_handle.id != 0) {
                        object_table.delete_object(object_handle);
                    }
                    else {
                        object_table.delete_object(object_handle.index);
                    }
                }
                else {
                    return luaL_error(state, "Invalid value type (expected table or integer).");
                }
                return 0;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function engine.delete_object.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static int lua_unit_enter_vehicle(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 3) {
                Engine::ObjectHandle unit_handle(luaL_checkinteger(state, 1));
                Engine::ObjectHandle vehicle_handle(luaL_checkinteger(state, 2));
                auto &object_table = Engine::get_object_table();

                auto *unit = reinterpret_cast<Engine::UnitObject *>(object_table.get_object(unit_handle));
                if(!unit) {
                    return luaL_error(state, "invalid unit handle in balltze unit_enter_vehicle");
                }
                if(!unit->type == Engine::OBJECT_TYPE_BIPED) {
                    return luaL_error(state, "invalid object type in balltze unit_enter_vehicle, expected biped");
                }
                
                try {
                    if(lua_isinteger(state, 3)) {
                        auto seat_index = luaL_checkinteger(state, 3);
                        unit->enter_vehicle(vehicle_handle, seat_index);
                    }
                    else if(lua_isstring(state, 3)) {
                        auto seat_label = luaL_checkstring(state, 3);
                        unit->enter_vehicle(vehicle_handle, seat_label);
                    }
                    else {
                        return luaL_error(state, "invalid seat index or label in balltze unit_enter_vehicle, expected integer or string");
                    }
                }
                catch(std::runtime_error &e) {
                    return luaL_error(state, "%s in balltze unit_enter_vehicle", e.what());
                }
            }
            else {
                return luaL_error(state, "invalid number of arguments in balltze unit_enter_vehicle");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
        return 0;
    }

    static int lua_get_player(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 0 || args == 1) {
                auto &player_table = Engine::get_player_table();
                Engine::Player *player = nullptr;
                if(args == 1) {
                    auto player_index_or_handle = luaL_checkinteger(state, 1);
                    if(player_index_or_handle > 0xFFFF) {
                        Engine::PlayerHandle player_handle(player_index_or_handle);
                        player = player_table.get_player(player_handle);
                    }
                    else {
                        auto *player_entry = player_table.get_element(player_index_or_handle);
                        if(player_entry->player_id != 0xFFFF) {
                            player = player_entry;
                        }
                    }
                }
                else {
                    player = player_table.get_client_player();
                }
                if(player) {
                    lua_push_meta_engine_player(state, *player);
                }
                else {
                    lua_pushnil(state);
                }
                return 1;
            }
            else {
                return luaL_error(state, "invalid number of arguments in balltze get_player");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
        return 0;
    }

    static int lua_get_player_by_rcon_handle(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args > 0) {
                auto &player_table = Engine::get_player_table();
                auto rcon_handle = luaL_checkinteger(state, 1);
                Engine::Player *player = player_table.get_player_by_rcon_handle(rcon_handle);
                if(player) {
                    lua_push_meta_engine_player(state, *player);
                }
                else {
                    lua_pushnil(state);
                }
                return 1;
            }
            else {
                return luaL_error(state, "invalid number of arguments in balltze get_player_by_rcon_handle");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
        return 0;
    }

    static const luaL_Reg engine_functions[] = {
        {"console_print", lua_engine_console_print},
        {"get_resolution", lua_engine_get_resolution},
        {"get_tick_count", lua_engine_get_tick_count},
        {"get_engine_edition", lua_engine_get_engine_edition},
        {"get_current_map_header", lua_engine_get_current_map_header},
        {"get_map_list", lua_engine_get_map_list},
        {"get_server_type", lua_engine_get_server_type},
        {"get_server_gametype", lua_engine_get_server_gametype},
        {"current_game_is_team", lua_engine_current_game_is_team},
        {"get_tag_data_header", lua_engine_get_tag_data_header},
        {"get_tag", lua_engine_get_tag},
        {"get_tag_data", lua_engine_get_tag_data},
        {"find_widget", lua_engine_find_widget},
        {"find_widgets", lua_engine_find_widgets},
        {"open_widget", lua_engine_open_widget},
        {"close_widget", lua_engine_close_widget},
        {"replace_widget", lua_engine_replace_widget},
        {"reload_widget", lua_engine_reload_widget},
        {"focus_widget", lua_engine_focus_widget},
        {"open_pause_menu", lua_engine_open_pause_menu},
        {"get_hud_globals", lua_engine_get_hud_globals},
        {"get_bitmap_sprite_resolution", lua_get_bitmap_sprite_resolution},
        {"draw_hud_message_sprite", lua_engine_draw_hud_message_sprite},
        {"play_sound", lua_engine_play_sound},
        {"get_sound_permutation_samples_duration", lua_engine_get_sound_permutation_samples_duration},
        {"get_camera_data", lua_engine_get_camera_data},
        {"get_camera_type", lua_engine_get_camera_type},
        {"get_object", lua_engine_get_object},
        {"create_object", lua_engine_create_object},
        {"delete_object", lua_engine_delete_object},
        {"unit_enter_vehicle", lua_unit_enter_vehicle},
        {"get_player", lua_get_player},
        {"get_player_by_rcon_handle", lua_get_player_by_rcon_handle},
        {nullptr, nullptr}
    };

    void lua_set_engine_table(lua_State *state) noexcept {
        lua_create_functions_table(state, "engine", engine_functions);
    }
}
