// SPDX-License-Identifier: GPL-3.0-only

#include <lua.hpp>
#include <balltze/utils.hpp>
#include "../../logger.hpp"
#include "plugin.hpp"

namespace Balltze::Lua {
    static const char *PLUGIN_LUA_STATE_METATABLE = "plugin_lua_state";

    int push_plugin_lua_state(lua_State *state) noexcept {
        lua_pushlightuserdata(state, state);
        luaL_newmetatable(state, PLUGIN_LUA_STATE_METATABLE);
        lua_setmetatable(state, -2);
        return 1;
    }

    bool userdata_is_plugin_lua_state(lua_State *state, int index) noexcept {
        int abs_index = lua_absindex(state, index);
        if(!lua_islightuserdata(state, abs_index)) {
            return false;
        }
        lua_getmetatable(state, abs_index);
        if(!lua_istable(state, -1)) {
            lua_pop(state, 1);
            return false;
        }
        luaL_getmetatable(state, PLUGIN_LUA_STATE_METATABLE);
        bool is_plugin_lua_state = lua_rawequal(state, -1, -2);
        lua_pop(state, 2); 
        return is_plugin_lua_state;
    }

    lua_State *get_plugin_lua_state(lua_State *state) noexcept {
        if(userdata_is_plugin_lua_state(state, lua_upvalueindex(1))) {
            lua_State *upvalue_state = reinterpret_cast<lua_State *>(lua_touserdata(state, lua_upvalueindex(1)));
            if(upvalue_state) {
                return upvalue_state;
            }
        }
        return state; 
    }

    int plugin_func_upvalue_index(lua_State *state, int upvalue_index) noexcept {
        if(userdata_is_plugin_lua_state(state, lua_upvalueindex(1))) {
            return lua_upvalueindex(upvalue_index + 1);
        }
        return lua_upvalueindex(upvalue_index);
    }

    void push_plugin_function(lua_State *state, lua_CFunction func, int upvalue_count) noexcept {
        push_plugin_lua_state(state);
        lua_pushcclosure(state, func, upvalue_count + 1);
    }

    void push_plugin_functions_table(lua_State *state, const char *name, int table_index, const luaL_Reg *functions) noexcept {
        int abs_index = lua_absindex(state, table_index);
        luaL_newlibtable(state, functions);
        push_plugin_lua_state(state);
        luaL_setfuncs(state, functions, 1);
        lua_setfield(state, abs_index, name);
    }

    int plugin_error_handler(lua_State *state) noexcept {
        luaL_traceback(state, state, lua_tostring(state, 1), 1);
        return 1;
    }

    void create_plugin_registry(lua_State *state) {
        lua_pushlightuserdata(state, get_current_module());
        lua_newtable(state);
        lua_settable(state, LUA_REGISTRYINDEX);
    }

    void get_or_create_plugin_registry_table(lua_State *state, const char *name) {
        lua_pushlightuserdata(state, get_current_module());
        lua_gettable(state, LUA_REGISTRYINDEX);
        if(lua_isnil(state, -1)) {
            logger.error("Could not find balltze Lua registry table");
            return;
        }
        lua_getfield(state, -1, name);
        if(lua_isnil(state, -1)) {
            lua_pop(state, 1);
            lua_newtable(state);
            lua_pushvalue(state, -1);
            lua_setfield(state, -3, name);
        }
        lua_remove(state, -2);
    }

    void clear_registry_plugin_registry_table(lua_State *state, const char *name) {
        lua_pushlightuserdata(state, get_current_module());
        lua_gettable(state, LUA_REGISTRYINDEX);
        if(lua_isnil(state, -1)) {
            logger.error("Could not find balltze Lua registry table");
            return;
        }
        lua_pushnil(state);
        lua_setfield(state, -2, name);
        lua_pop(state, 1); 
    }
}
