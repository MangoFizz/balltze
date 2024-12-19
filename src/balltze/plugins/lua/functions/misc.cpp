// SPDX-License-Identifier: GPL-3.0-only

#include <chrono>
#include <lua.hpp>
#include <clipboardxx/clipboardxx.hpp>
#include <balltze/events/render.hpp>
#include <balltze/events/tick.hpp>
#include "../helpers/function_table.hpp"
#include "../helpers/registry.hpp"
#include "../helpers/table.hpp"
#include "../libraries/preloaded_libraries.hpp"
#include "../../loader.hpp"
#include "../../../logger.hpp"

namespace Balltze::Plugins::Lua {
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

    static void get_timers_registry_table(lua_State *state) noexcept {
        get_or_create_registry_table(state, "timers");
    }

    static int stop_timer(lua_State *state) noexcept {
        if(lua_isnil(state, lua_upvalueindex(1))) {
            return luaL_error(state, "timer has already been stopped");
        }

        get_timers_registry_table(state);
        lua_pushvalue(state, lua_upvalueindex(1));
        table_remove_if_equals(state, -2, -1);

        // Remove the upvalue
        lua_pushnil(state);
        lua_replace(state, lua_upvalueindex(1));

        return 0;
    }

    static int set_timer(lua_State *state) noexcept {
        using time_point_t = std::chrono::steady_clock::time_point;

        auto *plugin = get_lua_plugin(state);
        if(!plugin) {
            return luaL_error(state, "Missing plugin upvalue in function Balltze.misc.setTimer");
        }

        int args = lua_gettop(state);
        if(args >= 2) {
            auto interval = luaL_checknumber(state, 1);
            if(interval < 100) {
                logger.warning("Interval is less than 100 milliseconds in Balltze.misc.setTimer function at plugin {}. This could impact performance.", plugin->name());
            }

            if(!lua_isfunction(state, 2)) {
                return luaL_error(state, "Invalid function in Balltze.misc.setTimer function");
            }

            lua_newtable(state);
            lua_pushvalue(state, 2);
            lua_setfield(state, -2, "function");
            lua_pushvalue(state, 1);
            lua_setfield(state, -2, "interval");
            auto *timestamp = reinterpret_cast<time_point_t *>(lua_newuserdata(state, sizeof(time_point_t)));
            *timestamp = std::chrono::steady_clock::now();
            lua_setfield(state, -2, "timestamp");

            // append table to the timers array
            get_timers_registry_table(state);
            table_insert(state, -1, -2);
            lua_pop(state, 1);

            lua_newtable(state);
            lua_pushvalue(state, -2);
            lua_pushcclosure(state, stop_timer, 1);
            lua_setfield(state, -2, "stop");

            lua_remove(state, -2);

            return 1;
        }
        else {
            return luaL_error(state, "Invalid number of arguments in Balltze.misc.setTimer function");
        }
    }

    static void check_timers(LuaPlugin *plugin) {
        if(plugin->loaded()) {
            auto *state = plugin->state();
            get_timers_registry_table(state);
            lua_pushnil(state);
            while(lua_next(state, -2) != 0) {
                lua_getfield(state, -1, "timestamp");
                auto *timestamp = reinterpret_cast<std::chrono::steady_clock::time_point *>(lua_touserdata(state, -1));
                lua_pop(state, 1);

                lua_getfield(state, -1, "interval");
                auto interval = lua_tonumber(state, -1);
                lua_pop(state, 1);

                auto now = std::chrono::steady_clock::now();
                auto elapsed_time = now - *timestamp;
                auto elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_time);
                if(elapsed_seconds.count() >= interval) {
                    lua_getfield(state, -1, "function");
                    if(lua_pcall(state, 0, 0, 0) != 0) {
                        logger.error("Error in timer function at plugin {}: {}", plugin->name(), plugin->get_error_message());
                        plugin->print_traceback();
                        lua_pop(state, 1);
                    }
                    *timestamp = now;
                }
                lua_pop(state, 1);
            }
            lua_pop(state, 1);
        }
    }

    static const luaL_Reg misc_functions[] = {
        {"setTimestamp", lua_set_timestamp},
        {"getTimestampElapsedMilliseconds", lua_get_elapsed_milliseconds},
        {"getTimestampElapsedSeconds", lua_get_elapsed_seconds},
        {"resetTimestamp", lua_reset_timestamp},
        {"getClipboard", get_clipboard},
        {"setClipboard", set_clipboard},
        {"setTimer", set_timer},
        {nullptr, nullptr}
    };

    void set_misc_table(lua_State *state) noexcept {
        create_functions_table(state, "misc", misc_functions);

        static auto tickHandler = Event::TickEvent::subscribe_const([](const Event::TickEvent &event) {
            if(event.time == Event::EVENT_TIME_BEFORE) {
                auto plugins = get_lua_plugins();
                for(auto *plugin : plugins) {
                    check_timers(plugin);
                }
            }
        });

        static auto frameHandler = Event::FrameEvent::subscribe_const([](const Event::FrameEvent &event) {
            if(event.time == Event::EVENT_TIME_AFTER) {
                auto plugins = get_lua_plugins();
                for(auto *plugin : plugins) {
                    check_timers(plugin);
                }
            }
        });
    }
}
