// SPDX-License-Identifier: GPL-3.0-only

#include <chrono>
#include <lua.hpp>
#include "helpers.hpp"

namespace Balltze::Plugins {
    static int lua_get_elapsed_milliseconds(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 1) {
            if(!lua_istable(state, 1)) {
                return luaL_error(state, "invalid timestamp in Balltze.misc.getTimestampElapsedMilliseconds function");
            }
            lua_getfield(state, 1, "_timestamp");
            auto *timestamp = reinterpret_cast<std::chrono::steady_clock::time_point *>(lua_touserdata(state, -1));
            auto elapsed_time = std::chrono::steady_clock::now() - *timestamp;
            auto elapsed_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_time);
            lua_pushinteger(state, elapsed_milliseconds.count());
            return 1;
        }
        else {
            return luaL_error(state, "invalid number of arguments in Balltze.misc.getTimestampElapsedMilliseconds function");
        }
    }

    static int lua_get_elapsed_seconds(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 1) {
            if(!lua_istable(state, 1)) {
                return luaL_error(state, "invalid timestamp in Balltze.misc.getTimestampElapsedSeconds function");
            }
            lua_getfield(state, 1, "_timestamp");
            auto *timestamp = reinterpret_cast<std::chrono::steady_clock::time_point *>(lua_touserdata(state, -1));
            auto elapsed_time = std::chrono::steady_clock::now() - *timestamp;
            auto elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(elapsed_time);
            lua_pushinteger(state, elapsed_seconds.count());
            return 1;
        }
        else {
            return luaL_error(state, "invalid number of arguments in Balltze.misc.getTimestampElapsedSeconds function");
        }
    }

    static int lua_reset_timestamp(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 1) {
            if(!lua_istable(state, 1)) {
                return luaL_error(state, "invalid timestamp in Balltze.misc.resetTimestamp function");
            }
            lua_getfield(state, 1, "_timestamp");
            auto *timestamp = reinterpret_cast<std::chrono::steady_clock::time_point *>(lua_touserdata(state, -1));
            *timestamp = std::chrono::steady_clock::now();
            return 0;
        }
        else {
            return luaL_error(state, "invalid number of arguments in Balltze.misc.resetTimestamp function");
        }
    }

    static int lua_set_timestamp(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 0) {
            auto now = std::chrono::steady_clock::now();

            lua_newtable(state);
            auto *timestamp = reinterpret_cast<std::chrono::steady_clock::time_point *>(lua_newuserdata(state, sizeof(std::chrono::steady_clock::time_point)));
            *timestamp = now;
            lua_setfield(state, -2, "_timestamp");

            lua_pushcfunction(state, lua_get_elapsed_milliseconds);
            lua_setfield(state, -2, "getElapsedMilliseconds");

            lua_pushcfunction(state, lua_get_elapsed_seconds);
            lua_setfield(state, -2, "getElapsedSeconds");

            lua_pushcfunction(state, lua_reset_timestamp);
            lua_setfield(state, -2, "reset");

            return 1;
        }
        else {
            return luaL_error(state, "invalid number of arguments in harmony set_timestamp function");
        }
    }

    static int lua_deep_copy_table(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 1) {
            if(lua_istable(state, 1)) {
                lua_newtable(state);
                lua_pushnil(state);
                while(lua_next(state, 1) != 0) {
                    if(lua_istable(state, -1)) {
                        lua_deep_copy_table(state);
                    }
                    else {
                        lua_pushvalue(state, -1);
                    }
                    lua_pushvalue(state, -2);
                    lua_settable(state, -4);
                }
                return 1;
            }
            else {
                return luaL_error(state, "invalid table in Balltze.misc.deepCopy function");
            }
        }
        else {
            return luaL_error(state, "invalid number of arguments in Balltze.misc.deepCopy function");
        }
    }

    static const luaL_Reg misc_functions[] = {
        {"setTimestamp", lua_set_timestamp},
        {"getTimestampElapsedMilliseconds", lua_get_elapsed_milliseconds},
        {"getTimestampElapsedSeconds", lua_get_elapsed_seconds},
        {"resetTimestamp", lua_reset_timestamp},
        {"deepCopyTable", lua_deep_copy_table},
        {nullptr, nullptr}
    };

    void lua_set_misc_table(lua_State *state) noexcept {
        lua_create_functions_table(state, "misc", misc_functions);
    }
}
