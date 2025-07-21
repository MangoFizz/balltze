// SPDX-License-Identifier: GPL-3.0-only

#include <lua.hpp>
#include <balltze/legacy_api/engine.hpp>
#include "../../../../helpers/function_table.hpp"
#include "../../types.hpp"

namespace Balltze::Lua::Api::V2 {
    static int engine_cache_file_get_loaded_header(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 0) {
            auto header = LegacyApi::Engine::get_map_header();
            push_cache_file_header(state, reinterpret_cast<const CacheFileHeader &>(header), true);
            return 1;
        }
        else {
            return luaL_error(state, "Invalid number of arguments in function Engine.cacheFile.getLoadedCacheFileHeader.");
        }
    }

    static int engine_cache_file_get_list(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 0) {
            auto map_list = LegacyApi::Engine::get_map_list();
            auto engine_edition = LegacyApi::Engine::get_engine_edition();
            auto *map_list_array = reinterpret_cast<LegacyApi::Engine::MapIndexCustomEdition *>(map_list.map_list);
            lua_newtable(state);
            for(std::size_t i = 0; i < map_list.map_count; i++) {
                lua_pushstring(state, map_list_array[i].file_name);
                lua_rawseti(state, -2, i + 1);
            }
            return 1;
        }
        else {
            return luaL_error(state, "Invalid number of arguments in function Engine.cacheFile.getMapList.");
        }
    }

    static const luaL_Reg engine_cache_file_functions[] = {
        {"getLoadedCacheFileHeader", engine_cache_file_get_loaded_header},
        {"getList", engine_cache_file_get_list},
        {nullptr, nullptr}
    };

    void set_engine_cache_file_functions(lua_State *state, int table_idx) noexcept {
        int abs_idx = lua_absindex(state, table_idx);
        lua_newtable(state);
        set_functions_reg_array(state, -1, engine_cache_file_functions);
        lua_setfield(state, abs_idx, "cacheFile");
    }
}
