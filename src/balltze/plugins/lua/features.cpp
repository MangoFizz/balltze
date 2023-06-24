// SPDX-License-Identifier: GPL-3.0-only

#include <lua.hpp>
#include <balltze/engine/tag.hpp>
#include <balltze/events/map_file_load.hpp>
#include <balltze/features.hpp>
#include "../../features/tag_data_importing/map.hpp"
#include "../../logger.hpp"
#include "../loader.hpp"
#include "helpers.hpp"

namespace Balltze::Plugins {
    int lua_import_tag_from_map(lua_State *state) {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 3) {
                auto map_path = luaL_checkstring(state, 1);
                auto *tag_path = luaL_checkstring(state, 2);
                auto *tag_class_string = luaL_checkstring(state, 3);
                Engine::TagClassInt tag_class_int;

                try {
                    tag_class_int = Engine::tag_class_from_string(tag_class_string);
                }
                catch(std::runtime_error &e) {
                    return luaL_error(state, e.what());
                }

                if(plugin->path_is_valid(map_path)) {
                    try {
                        plugin->add_tag_import(map_path, tag_path, tag_class_int);
                    }
                    catch(std::runtime_error &e) {
                        return luaL_error(state, e.what());
                    }
                }
                else {
                    auto map_entry = Features::get_map_entry(map_path);
                    if(map_entry) {
                        try {
                            plugin->add_tag_import(map_entry->get_file_path().string(), tag_path, tag_class_int);
                        }
                        catch(std::runtime_error &e) {
                            return luaL_error(state, e.what());
                        }
                    }
                    else {
                        return luaL_error(state, "Map not found.");
                    }
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

    int lua_import_tags_from_map(lua_State *state) {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 1) {
                auto map_path = luaL_checkstring(state, 1);

                if(plugin->path_is_valid(map_path)) {
                    try {
                        plugin->import_all_tags(map_path);
                    }
                    catch(std::runtime_error &e) {
                        return luaL_error(state, e.what());
                    }
                }
                else {
                    auto map_entry = Features::get_map_entry(map_path);
                    if(map_entry) {
                        try {
                            plugin->import_all_tags(map_path);
                        }
                        catch(std::runtime_error &e) {
                            return luaL_error(state, e.what());
                        }
                    }
                    else {
                        return luaL_error(state, "Map not found.");
                    }
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

    void on_map_data_read(Event::MapFileLoadEvent &event) {
        auto plugins = get_lua_plugins();
        for(auto &plugin : plugins) {
            auto map_imports = plugin->imported_tags();
            for(auto &[path, tags] : map_imports) {
                if(!tags.empty()) {
                    for(auto &[tag_path, tag_class] : tags) {
                        try {
                            std::filesystem::path map_path = path;
                            Features::import_tag_from_map(map_path, tag_path, tag_class);
                        }
                        catch(std::runtime_error &e) {
                            logger.warning("Could not import tag from map: {}", tag_path);
                        }
                    }
                }
                else {
                    std::filesystem::path map_path = path;
                    Features::import_tags_from_map(map_path);
                }
            }
        }
    }

    static const luaL_Reg features_functions[] = {
        {"import_tag_from_map", lua_import_tag_from_map},
        {"import_tags_from_map", lua_import_tags_from_map},
        {nullptr, nullptr}
    };

    void lua_set_features_table(lua_State *state) noexcept {
        lua_create_functions_table(state, "features", features_functions);

        static auto listener = Event::MapFileLoadEvent::subscribe(on_map_data_read, Event::EVENT_PRIORITY_LOWEST);
    }
}
