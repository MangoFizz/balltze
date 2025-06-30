// SPDX-License-Identifier: GPL-3.0-only

#include <lua.hpp>
#include <balltze/helpers/resources.hpp>
#include <balltze/utils.hpp>
#include "../../resources.hpp"
#include "lanes.hpp"
#include "lfmt.hpp"

namespace Balltze::Lua {
    static const luaL_Reg fmt_functions[] = {
        { "format", Lformat },
        { nullptr, nullptr }
    };

    static int lua_open_lfmt(lua_State *state) noexcept {
        luaL_newlibtable(state, fmt_functions);
        luaL_setfuncs(state, fmt_functions, 0);
        return 1;
    }

    static int lua_open_json(lua_State *state) noexcept {
        auto json_module_data = load_resource_data(get_current_module(), MAKEINTRESOURCEW(ID_LUA_JSON_MODULE), L"LUA");
        if(json_module_data) {
            if(luaL_loadbufferx(state, reinterpret_cast<const char *>(json_module_data->data()), json_module_data->size(), "json", "t") == LUA_OK) {
                lua_call(state, 0, 1);
                return 1;
            }
        }
        return 0;
    }

    static int lua_open_luna(lua_State *state) noexcept {
        auto luna_module_data = load_resource_data(get_current_module(), MAKEINTRESOURCEW(ID_LUA_LUNA_MODULE), L"LUA");
        if(luna_module_data) {
            if(luaL_loadbufferx(state, reinterpret_cast<const char *>(luna_module_data->data()), luna_module_data->size(), "luna", "t") == LUA_OK) {
                lua_call(state, 0, 1);
                return 1;
            }
        }
        return 0;
    }

    static int lua_open_inspect(lua_State *state) noexcept {
        auto inspect_module_data = load_resource_data(get_current_module(), MAKEINTRESOURCEW(ID_LUA_INSPECT_MODULE), L"LUA");
        if(inspect_module_data) {
            if(luaL_loadbufferx(state, reinterpret_cast<const char *>(inspect_module_data->data()), inspect_module_data->size(), "inspect", "t") == LUA_OK) {
                lua_call(state, 0, 1);
                return 1;
            }
        }
        return 0;
    }

    int lua_open_lanes(lua_State *state) noexcept {
        auto lanes_module_data = load_resource_data(get_current_module(), MAKEINTRESOURCEW(ID_LUA_LANES_MODULE), L"LUA");
        if(lanes_module_data) {
            if(luaL_loadbufferx(state, reinterpret_cast<const char *>(lanes_module_data->data()), lanes_module_data->size(), "lanes", "t") == LUA_OK) {
                lua_call(state, 0, 1);
                return 1;
            }
        }
        return 0;
    }

    void set_preloaded_libraries(lua_State *state) noexcept {
        lua_getglobal(state, "package");
        lua_getfield(state, -1, "preload");
        int preload_table_index = lua_gettop(state);

        lua_pushcfunction(state, lua_open_lfmt);
        lua_setfield(state, preload_table_index, "lfmt");

        lua_pushcfunction(state, lua_open_json);
        lua_setfield(state, preload_table_index, "json");

        lua_pushcfunction(state, lua_open_luna);
        lua_setfield(state, preload_table_index, "luna");

        lua_pushcfunction(state, lua_open_inspect);
        lua_setfield(state, preload_table_index, "inspect");

        // Preload and configure lanes 
        luaopen_lanes_embedded(state, lua_open_lanes);
        lua_getfield(state, -1, "configure");
        lua_call(state, 0, 0);
        lua_pop(state, 1);

        lua_pop(state, 2);
    }
}
