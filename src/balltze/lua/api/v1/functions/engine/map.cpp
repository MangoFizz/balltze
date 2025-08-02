// SPDX-License-Identifier: GPL-3.0-only

#include <lua.hpp>
#include <balltze/legacy_api/engine.hpp>
#include "../../../../helpers/plugin.hpp"
#include "../../types.hpp"

namespace Balltze::Lua::Api::V1 {
    static int engine_get_current_map_header(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 0) {
            auto header = LegacyApi::Engine::get_map_header();
            push_meta_engine_map_header(state, &header);
            return 1;
        }
        else {
            return luaL_error(state, "Invalid number of arguments in function Engine.map.getCurrentMapHeader.");
        }
    }

    static int engine_get_map_list(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 0) {
            auto map_list = LegacyApi::Engine::get_map_list();
            auto engine_edition = LegacyApi::Engine::get_engine_edition();
            if(engine_edition == LegacyApi::Engine::ENGINE_TYPE_RETAIL) {
                auto *map_list_array = reinterpret_cast<LegacyApi::Engine::MapIndexRetail *>(map_list.map_list);
                lua_newtable(state);
                for(std::size_t i = 0; i < map_list.map_count; i++) {
                    lua_pushstring(state, map_list_array[i].file_name);
                    lua_rawseti(state, -2, i + 1);
                }
            }
            else if(engine_edition == LegacyApi::Engine::ENGINE_TYPE_CUSTOM_EDITION) {
                auto *map_list_array = reinterpret_cast<LegacyApi::Engine::MapIndexCustomEdition *>(map_list.map_list);
                lua_newtable(state);
                for(std::size_t i = 0; i < map_list.map_count; i++) {
                    lua_pushstring(state, map_list_array[i].file_name);
                    lua_rawseti(state, -2, i + 1);
                }
            }
            else {
                auto *map_list_array = reinterpret_cast<LegacyApi::Engine::MapIndex *>(map_list.map_list);
                lua_newtable(state);
                for(std::size_t i = 0; i < map_list.map_count; i++) {
                    lua_pushstring(state, map_list_array[i].file_name);
                    lua_rawseti(state, -2, i + 1);
                }
            }
            return 1;
        }
        else {
            return luaL_error(state, "Invalid number of arguments in function Engine.map.getMapList.");
        }
    }

    static const luaL_Reg engine_map_functions[] = {
        {"getCurrentMapHeader", engine_get_current_map_header},
        {"getMapList", engine_get_map_list},
        {nullptr, nullptr}
    };

    void set_engine_map_functions(lua_State *state) noexcept {
        push_plugin_functions_table(state, "map", -1, engine_map_functions);
    }
}