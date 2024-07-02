// SPDX-License-Identifier: GPL-3.0-only

#include <lua.hpp>
#include <balltze/engine/tag.hpp>
#include <balltze/events/map_load.hpp>
#include <balltze/features.hpp>
#include "../../../features/tags_handling/map.hpp"
#include "../../../logger.hpp"
#include "../../loader.hpp"
#include "../helpers/function_table.hpp"
#include "../types.hpp"

namespace Balltze::Plugins::Lua {
    static int lua_import_tag_from_map(lua_State *state) {
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
                            plugin->add_tag_import(map_path, tag_path, tag_class_int);
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
                return luaL_error(state, "Invalid number of arguments in function Balltze.features.importTagFromMap.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
        return 0;
    }

    static int lua_import_tags_from_map(lua_State *state) {
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
                return luaL_error(state, "Invalid number of arguments in function Balltze.features.importTagsFromMap.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
        return 0;
    }

    static int lua_clear_tag_imports(lua_State *state) {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            plugin->clear_tag_imports();
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
        return 0;
    }

    static int lua_reload_tag_data(lua_State *state) {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 1 || args == 2) {
                Engine::TagHandle tag_handle;
                if(args == 1 && (lua_isnumber(state, 1) || lua_istable(state, 1) || lua_isuserdata(state, 1))) {
                    auto handle = get_engine_resource_handle(state, 1);
                    if(!handle || handle->is_null()) {
                        return luaL_error(state, "Invalid tag handle in function Balltze.features.reloadTagData.");
                    }
                    tag_handle = *handle;
                    auto *tag = Engine::get_tag(tag_handle);
                    if(!tag) {
                        return luaL_error(state, "Could not find tag in function Balltze.features.reloadTagData.");
                    }
                } 
                else {
                    const char *tag_path = luaL_checkstring(state, 1);
                    auto *tag_class_string = luaL_checkstring(state, 2);
                    Engine::TagClassInt tag_class_int = Engine::tag_class_from_string(tag_class_string);
                    auto *tag = Engine::get_tag(tag_path, tag_class_int);
                    if(tag) {
                        tag_handle = tag->handle;
                    }
                    else {
                        return luaL_error(state, "Could not find tag in function Balltze.features.reloadTagData.");
                    }
                }
                try {
                    Features::reload_tag_data(tag_handle);
                }
                catch(std::runtime_error &e) {
                    return luaL_error(state, e.what());
                }
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function Balltze.features.reloadTagData.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
        return 0;
    }

    static int lua_replace_tag_references(lua_State *state) {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 2) {
                auto tag_handle = get_engine_resource_handle(state, 1);
                if(!tag_handle || tag_handle->is_null()) {
                    return luaL_error(state, "Invalid tag handle in function Balltze.features.replaceTagReferences.");
                }
                
                auto new_tag_handle = get_engine_resource_handle(state, 2);
                if(!new_tag_handle || new_tag_handle->is_null()) {
                    return luaL_error(state, "Invalid new tag handle in function Balltze.features.replaceTagReferences.");
                }

                try {
                    Features::replace_tag_references(*tag_handle, *new_tag_handle);
                }
                catch(std::runtime_error &e) {
                    return luaL_error(state, e.what());
                }
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function Balltze.features.replaceTagReferences.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
        return 0;
    }

    static int lua_clone_tag(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 2) {
                auto tag_handle = get_engine_resource_handle(state, 1);
                if(!tag_handle || tag_handle->is_null()) {
                    return luaL_error(state, "Invalid tag handle in function Balltze.features.cloneTag.");
                }

                auto copy_name = luaL_checkstring(state, 2);
                try {
                    auto new_tag_handle = Features::clone_tag(*tag_handle, copy_name);
                    lua_pushinteger(state, new_tag_handle.value);
                    return 1;
                }
                catch(std::runtime_error &e) {
                    return luaL_error(state, e.what());
                }
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function Balltze.features.cloneTag.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static int lua_get_tag_copy(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 2) {
                auto tag_handle = get_engine_resource_handle(state, 1);
                if(!tag_handle || tag_handle->is_null()) {
                    return luaL_error(state, "Invalid tag handle in function Balltze.features.getTagCopy.");
                }
                
                auto copy_name = luaL_checkstring(state, 2);
                try {
                    auto *tag_copy = Features::get_tag_copy(*tag_handle, copy_name);
                    push_meta_engine_tag(state, tag_copy);
                    return 1;
                }
                catch(std::runtime_error &e) {
                    return luaL_error(state, e.what());
                }
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function Balltze.features.getTagCopy.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static int lua_get_imported_tag(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 3) {
                auto map_path = luaL_checkstring(state, 1);
                auto tag_path = luaL_checkstring(state, 2);
                auto tag_class_string = luaL_checkstring(state, 3);
                auto tag_class_int = Engine::tag_class_from_string(tag_class_string);
                try {
                    auto *tag = Features::get_imported_tag(map_path, tag_path, tag_class_int);
                    push_meta_engine_tag(state, tag);
                    return 1;
                }
                catch(std::runtime_error &e) {
                    return luaL_error(state, e.what());
                }
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function Balltze.features.getImportedTag.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static void on_map_load(Event::MapLoadEvent &event) {
        if(event.time == Event::EVENT_TIME_AFTER) {
            return;
        }
        auto plugins = get_lua_plugins();
        for(auto &plugin : plugins) {
            auto map_imports = plugin->imported_tags();
            for(auto &[path, tags] : map_imports) {
                auto map_path = Features::path_for_map_local(path.c_str());
                if(!tags.empty()) {
                    for(auto &[tag_path, tag_class] : tags) {
                        try {
                            Features::import_tag_from_map(map_path, tag_path, tag_class);
                        }
                        catch(std::runtime_error &e) {
                            logger.warning("Could not import tag {} from map: {}", tag_path, path);
                        }
                    }
                }
                else {
                    try {
                        Features::import_tags_from_map(map_path);
                    }
                    catch(std::runtime_error &e) {
                        logger.warning("Could not import tags from map: {}", path);
                    }
                }
            }
        }
    }

    static int lua_set_ui_aspect_ratio(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 2) {
            std::uint16_t x = luaL_checknumber(state, 1);
            std::uint16_t y = luaL_checknumber(state, 2);
            Features::set_ui_aspect_ratio(x, y);
        }
        else {
            luaL_error(state, "invalid number of arguments in function Balltze.features.setUIAspectRatio.");
        }
        return 0;
    }

    static int lua_reset_ui_aspect_ratio(lua_State *state) noexcept {
        Features::reset_ui_aspect_ratio();
        return 0;
    }

    static const luaL_Reg features_functions[] = {
        {"importTagFromMap", lua_import_tag_from_map},
        {"importTagsFromMap", lua_import_tags_from_map},
        {"clearTagImports", lua_clear_tag_imports},
        {"reloadTagData", lua_reload_tag_data},
        {"replaceTagReferences", lua_replace_tag_references},
        {"cloneTag", lua_clone_tag},
        {"getTagCopy", lua_get_tag_copy},
        {"getImportedTag", lua_get_imported_tag},
        {"setUIAspectRatio", lua_set_ui_aspect_ratio},
        {"resetUIAspectRatio", lua_reset_ui_aspect_ratio},
        {nullptr, nullptr}
    };

    void set_features_table(lua_State *state) noexcept {
        create_functions_table(state, "features", features_functions);

        static auto listener = Event::MapLoadEvent::subscribe(on_map_load, Event::EVENT_PRIORITY_LOWEST);
    }
}
