// SPDX-License-Identifier: GPL-3.0-only

#include <lua.hpp>
#include <balltze/engine.hpp>
#include "../../../../logger.hpp"
#include "../../../plugin.hpp"
#include "../../../loader.hpp"
#include "../../libraries.hpp"
#include "../../helpers.hpp"

namespace Balltze::Plugins {
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
                    lua_setfield(state, -2, "engineType");

                    lua_pushinteger(state, header.file_size);
                    lua_setfield(state, -2, "fileSize");

                    lua_pushinteger(state, header.tag_data_offset);
                    lua_setfield(state, -2, "tagDataOffset");

                    lua_pushinteger(state, header.tag_data_size);
                    lua_setfield(state, -2, "tagDataSize");

                    lua_pushstring(state, header.name);
                    lua_setfield(state, -2, "name");

                    lua_pushstring(state, header.build);
                    lua_setfield(state, -2, "build");

                    auto map_game_type = map_game_type_to_string(header.game_type);
                    lua_pushstring(state, map_game_type.c_str());
                    lua_setfield(state, -2, "gameType");

                    lua_pushinteger(state, header.crc32);
                    lua_setfield(state, -2, "crc32");

                    return 1;
                }
                else {
                    auto demo_header = Engine::get_demo_map_header();

                    lua_newtable(state);

                    auto map_game_type = map_game_type_to_string(demo_header.game_type);
                    lua_pushstring(state, map_game_type.c_str());
                    lua_setfield(state, -2, "gameType");

                    lua_pushinteger(state, demo_header.tag_data_size);
                    lua_setfield(state, -2, "tagDataSize");

                    lua_pushstring(state, demo_header.build);
                    lua_setfield(state, -2, "build");

                    auto cache_file_edition = cache_file_engine_to_string(demo_header.engine_type);
                    lua_pushstring(state, cache_file_edition.c_str());
                    lua_setfield(state, -2, "engineType");

                    lua_pushstring(state, demo_header.name);
                    lua_setfield(state, -2, "name");

                    lua_pushinteger(state, demo_header.crc32);
                    lua_setfield(state, -2, "crc32");

                    lua_pushinteger(state, demo_header.file_size);
                    lua_setfield(state, -2, "fileSize");

                    lua_pushinteger(state, demo_header.tag_data_offset);
                    lua_setfield(state, -2, "tagDataOffset");

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

    static const luaL_Reg engine_map_functions[] = {
        {"getCurrentMapHeader", lua_engine_get_current_map_header},
        {"getMapList", lua_engine_get_map_list},
        {nullptr, nullptr}
    };

    void set_engine_map_functions(lua_State *state) noexcept {
        luaL_newlibtable(state, engine_map_functions);
        luaL_setfuncs(state, engine_map_functions, 0);
        lua_setfield(state, -2, "map");
    }
}