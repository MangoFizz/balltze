// SPDX-License-Identifier: GPL-3.0-only

#include <string>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <lua.hpp>
#include <balltze/events.hpp>
#include "../../../../plugins/loader.hpp"
#include "../../../../plugins/plugin.hpp"
#include "../../../../logger.hpp"
#include "../../../helpers/function_table.hpp"
#include "../../../helpers/table.hpp"
#include "../../../helpers/plugin.hpp"

namespace Balltze::Lua::Api::V2 {
    using time_point_t = std::chrono::steady_clock::time_point;

    struct LuaTimer {
        lua_State *lua_state = nullptr;
        time_point_t timestamp;
        int function_ref = LUA_NOREF;
        uint32_t interval = 0;

        int lua_get_function();
        LuaTimer(lua_State *state, int table_idx, uint32_t interval);
        ~LuaTimer();
    };

    static std::vector<std::unique_ptr<LuaTimer>> timers;
    static bool timers_initialized = false;

    static void get_or_create_timer_functions_table(lua_State *state) noexcept {
        get_or_create_plugin_registry_table(state, "timer_functions");
    }

    LuaTimer::LuaTimer(lua_State *state, int function_idx, uint32_t interval) : lua_state(state), interval(interval) {
        int function_abs_idx = lua_absindex(state, function_idx);
        timestamp = std::chrono::steady_clock::now();
        get_or_create_timer_functions_table(state);
        lua_pushvalue(state, function_abs_idx);
        function_ref = luaL_ref(state, -2);
        lua_pop(state, 1);
    }

    LuaTimer::~LuaTimer() {
        get_or_create_timer_functions_table(lua_state);
        luaL_unref(lua_state, -1, function_ref);
        lua_pop(lua_state, 1);
    }

    int LuaTimer::lua_get_function() {
        lua_rawgeti(lua_state, LUA_REGISTRYINDEX, function_ref);
        return 1;
    }

    static int lua_timer_stop_method(lua_State *state) noexcept {
        if(lua_isnil(state, lua_upvalueindex(1))) {
            return luaL_error(state, "timer has already been stopped");
        }
        
        LuaTimer *self = reinterpret_cast<LuaTimer *>(lua_touserdata(state, lua_upvalueindex(1)));
        if(!self) {
            return luaL_error(state, "Invalid timer");
        }
        
        timers.erase(std::remove_if(timers.begin(), timers.end(), [&](const std::unique_ptr<LuaTimer> &timer) {
            return timer->lua_state == state && timer.get() == self;
        }), timers.end());
        
        return 0;
    }

    static int lua_set_timer(lua_State *state) noexcept {
        auto *plugin = Plugins::get_lua_plugin(state);
        if(!plugin) {
            return luaL_error(state, "Missing plugin upvalue in function Balltze.setTimer");
        }

        int args = lua_gettop(state);
        if(args != 2) {
            return luaL_error(state, "Invalid number of arguments in Balltze.setTimer function");
        }

        uint32_t interval = luaL_checkinteger(state, 1);
        if(interval < (1000.0f / LegacyApi::Engine::get_tick_rate())) {
            logger.warning("Interval is less than 100 milliseconds in Balltze.misc.setTimer function at plugin {}. This could impact performance.", plugin->name());
        }

        if(!lua_isfunction(state, 2)) {
            return luaL_error(state, "Invalid function in Balltze.misc.setTimer function");
        }
        
        // Create timer
        timers.emplace_back(std::make_unique<LuaTimer>(state, 2, interval));
        auto *timer = timers.back().get();

        // Create timer handle
        lua_newtable(state);
        lua_pushlightuserdata(state, timer);
        lua_pushcclosure(state, lua_timer_stop_method, 1);
        lua_setfield(state, -2, "stop");

        return 1;
    }

    static void check_timers() {
        auto it = timers.begin();
        while(it != timers.end()) {
            auto &timer = *it;
            auto plugin = Plugins::get_lua_plugin(timer->lua_state);
            if(plugin) {
                lua_State *state = plugin->lua_state();
                lua_pushcfunction(state, Plugins::LuaPlugin::error_message_handler);
                timer->lua_get_function();
                if(lua_pcall(state, 0, 0, -2) != LUA_OK) {
                    logger.error("Error in Lua timer function: {}", plugin->pop_error_message());
                }
                it++;
            }
            else {
                it = timers.erase(it);
            }
        }
    }

    void set_up_plugin_timers(lua_State *state, int table_idx) noexcept {
        if(!timers_initialized) {
            Events::FrameEvent::subscribe([](Events::FrameEvent &) {
                check_timers();
            });
        }
        timers_initialized = true;

        int table_abs_idx = lua_absindex(state, table_idx);
        lua_pushvalue(state, table_abs_idx);
        push_plugin_function(state, lua_set_timer);
        lua_setfield(state, -2, "setTimer");
        lua_pop(state, 1); 
    }
}
