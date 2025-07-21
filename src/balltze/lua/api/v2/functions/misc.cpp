// SPDX-License-Identifier: GPL-3.0-only

#include <chrono>
#include <lua.hpp>
#include <clipboardxx/clipboardxx.hpp>
#include "../../../helpers/function_table.hpp"

namespace Balltze::Lua::Api::V2 {
    static auto LUA_TIMESTAMP_METATABLE = "balltze_timestamp";

    static int lua_get_elapsed_milliseconds(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 1) {
            if(!lua_isuserdata(state, 1)) {
                return luaL_error(state, "invalid timestamp in Balltze.misc.getTimestampElapsedMilliseconds function");
            }
            auto *timestamp = reinterpret_cast<std::chrono::steady_clock::time_point *>(lua_touserdata(state, 1));
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
            if(!lua_isuserdata(state, 1)) {
                return luaL_error(state, "invalid timestamp in Balltze.misc.getTimestampElapsedSeconds function");
            }
            auto *timestamp = reinterpret_cast<std::chrono::steady_clock::time_point *>(lua_touserdata(state, 1));
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
            if(!lua_isuserdata(state, 1)) {
                return luaL_error(state, "invalid timestamp in Balltze.misc.resetTimestamp function");
            }
            auto *timestamp = reinterpret_cast<std::chrono::steady_clock::time_point *>(lua_touserdata(state, 1));
            *timestamp = std::chrono::steady_clock::now();
            return 0;
        }
        else {
            return luaL_error(state, "invalid number of arguments in Balltze.misc.resetTimestamp function");
        }
    }

    static const luaL_Reg timestamp_functions[] = {
        {"getElapsedMilliseconds", lua_get_elapsed_milliseconds},
        {"getElapsedSeconds", lua_get_elapsed_seconds},
        {"reset", lua_reset_timestamp},
        {nullptr, nullptr}
    };

    static int lua_create_timestamp(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 0) {
            auto *timestamp = reinterpret_cast<std::chrono::steady_clock::time_point *>(lua_newuserdata(state, sizeof(std::chrono::steady_clock::time_point)));
            *timestamp = std::chrono::steady_clock::now();
            if(luaL_newmetatable(state, LUA_TIMESTAMP_METATABLE)) {
                lua_newtable(state);
                luaL_newlib(state, timestamp_functions);
                lua_setfield(state, -2, "__index");
            }
            lua_setmetatable(state, -2);
            return 1;
        }
        else {
            return luaL_error(state, "invalid number of arguments in function Balltze.misc.setTimestamp");
        }
    }

    static int get_clipboard(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 0) {
            clipboardxx::clipboard clipboard;
            lua_pushstring(state, clipboard.paste().c_str());
        }
        else {
            return luaL_error(state, "invalid number of arguments in Balltze.misc.getClipboard");
        }
        return 1;
    }

    static int set_clipboard(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 1) {
            std::string text = luaL_checkstring(state, 1);
            clipboardxx::clipboard clipboard;
            clipboard.copy(text);
        }
        else {
            return luaL_error(state, "invalid number of arguments in Balltze.misc.setClipboard");
        }
        return 0;
    }

    static const luaL_Reg misc_functions[] = {
        {"createTimestamp", lua_create_timestamp},
        {"getClipboard", get_clipboard},
        {"setClipboard", set_clipboard},
        {nullptr, nullptr}
    };

    void set_misc_functions(lua_State *state, int table_idx) noexcept {
        set_functions_reg_array(state, table_idx, misc_functions);
    }
}
