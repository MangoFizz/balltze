// SPDX-License-Identifier: GPL-3.0-only

#include <functional>
#include <lua.hpp>
#include "../../../../events/events.hpp"
#include "../../../../plugins/plugin.hpp"
#include "../../../../plugins/loader.hpp"
#include "../../../../logger.hpp"
#include "../../../helpers/function_table.hpp"
#include "../../../helpers/registry.hpp"
#include "../types.hpp"

using namespace Balltze::Events;

namespace Balltze::Lua::Api::V2 {
    static void get_or_create_events_table(lua_State *state) noexcept {
        get_or_create_registry_table(state, "events");
    }

    static void create_event_table(lua_State *state, const char *name) noexcept {
        get_or_create_events_table(state);
        lua_newtable(state);
        lua_pushinteger(state, 0);
        lua_setfield(state, -2, "listenersCount");
        lua_setfield(state, -2, name);
        lua_pop(state, 1); 
    }

    static void get_event_table(lua_State *state, const char *name) noexcept {
        get_or_create_events_table(state);
        lua_getfield(state, -1, name);
    }

    static void get_or_create_event_listeners_table(lua_State *state, const char *name, const char *priority) noexcept {
        get_event_table(state, name);
        
        const char *listeners_table_name = "listeners";
        lua_getfield(state, -1, listeners_table_name);
        if(lua_isnil(state, -1)) {
            lua_pop(state, 1);
            lua_newtable(state);
            lua_setfield(state, -2, listeners_table_name);
            lua_getfield(state, -1, listeners_table_name);
        }
        lua_remove(state, -2);

        lua_getfield(state, -1, priority);
        if(lua_isnil(state, -1)) {
            lua_pop(state, 1);
            lua_newtable(state);
            lua_setfield(state, -2, priority);
            lua_getfield(state, -1, priority);
        }
        lua_remove(state, -2); 
    }

    static std::size_t get_event_listeners_count(lua_State *state, const char *name) noexcept {
        get_event_table(state, name);
        lua_getfield(state, -1, "listenersCount");
        std::size_t count = luaL_checkinteger(state, -1);
        lua_pop(state, 2);
        return count;
    }

    static int add_event_listener(lua_State *state, const char *name, int function_index, EventPriority priority) noexcept {
        if(lua_isfunction(state, function_index)) {
            auto priority_name = event_priority_to_string(priority);
            get_or_create_event_listeners_table(state, name, priority_name);

            // Push function to the priority table
            auto listeners_count = get_event_listeners_count(state, name);
            auto handle = std::to_string(listeners_count + 1);
            lua_pushvalue(state, function_index);
            lua_setfield(state, -2, handle.c_str());

            // Create listener handle
            lua_newtable(state);
            lua_pushstring(state, handle.c_str());
            lua_setfield(state, -2, "handle");
            lua_pushstring(state, priority_name);
            lua_setfield(state, -2, "priority");
            lua_pushstring(state, name);
            lua_setfield(state, -2, "event");

            lua_pushvalue(state, -2); // listeners table
            lua_pushstring(state, handle.c_str());
            lua_pushcclosure(state, +[](lua_State *state) -> int {
                lua_pushvalue(state, lua_upvalueindex(2));
                lua_pushnil(state);
                const char *handle = luaL_checkstring(state, lua_upvalueindex(2));
                lua_setfield(state, -2, handle);
                return 0;
            }, 2);
            lua_setfield(state, -4, "remove");

            lua_remove(state, -2); // Remove the listener table from the stack

            return 1;
        }
        else {
            return luaL_error(state, "Invalid listener argument in function Balltze.events.%s.subscribe.", name);
        }
    }

    static void remove_all_event_listeners(lua_State *state, const char *name) noexcept {
        get_event_table(state, name);
        lua_pushnil(state);
        lua_setfield(state, -2, "listeners");
        lua_pop(state, 1);
    }

    template<typename Event>
    static void create_event_data_table(lua_State *state, Event &context) noexcept {
        lua_newtable(state);
        
        lua_pushboolean(state, context.cancelled());
        lua_setfield(state, -2, "cancelled");

        // Push a closure for cancel function, capturing the event context pointer
        lua_pushlightuserdata(state, &context);
        lua_pushcclosure(state, +[](lua_State *state) -> int {
            auto *context = static_cast<Event *>(lua_touserdata(state, lua_upvalueindex(1)));
            if(context && context->cancellable()) {
                context->cancel();
                lua_pushboolean(state, true);
                lua_setfield(state, 1, "cancelled");
                return 0;
            } 
            else if(context) {
                return luaL_error(state, "Attempted to cancel non-cancellable event.");
            } 
            else {
                return luaL_error(state, "Invalid event context in event.cancel.");
            }
        }, 1);
        lua_setfield(state, -2, "cancel");
    }

    void call_events_by_priority(lua_State *state, const char *name, EventPriority priority, std::function<void(lua_State *)> create_event_object) noexcept {
        auto priority_name = event_priority_to_string(priority);
        get_or_create_event_listeners_table(state, name, priority_name);

        // Get all event listeners
        lua_newtable(state);
        lua_pushnil(state);
        while(lua_next(state, -3)) {
            if(lua_isfunction(state, -1)) {
                lua_rawseti(state, -3, lua_rawlen(state, -3) + 1);
            }
            else {
                logger.error("Invalid event listener in Balltze.events.{}: expected function, got {}.", name, lua_typename(state, lua_type(state, -1)));
            }
            lua_pop(state, 1);
        }

        bool has_context = false;
        if(create_event_object) {
            create_event_object(state);
            has_context = true;
        }
        else {
            lua_pushnil(state);
        }

        // Call all event listeners
        bool cancelled = false;
        int size = lua_rawlen(state, -2);
        for(int i = 1; i <= size; i++) {
            lua_rawgeti(state, -2, i);
            lua_pushvalue(state, -2);
            int res = lua_pcall(state, 1, 0, 0);
            if(res != LUA_OK) {
                logger.error("Error in event listener in Balltze.events.{}: {}.", name, lua_tostring(state, -1));
                lua_pop(state, 1);
            }
        }

        lua_pop(state, 2);
    };

    int lua_event_add_listener(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 2 || args == 3) {
            auto event_name = luaL_checkstring(state, 1);
            auto function_index = lua_absindex(state, 2);
            auto priority = EVENT_PRIORITY_DEFAULT;
            if(args == 3) {
                auto priority_str = luaL_checkstring(state, 3);
                try {
                    priority = event_priority_from_string(priority_str);
                } 
                catch(const std::invalid_argument &e) {
                    return luaL_error(state, "Invalid priority argument in function events.addListener: %s.", priority_str);
                }
            }

            get_event_table(state, event_name);
            if(lua_isnil(state, -1)) {
                lua_pop(state, 1);
                return luaL_error(state, "Invalid event name %s in function events.addListener.", event_name);
            }
            lua_pop(state, 1);

            add_event_listener(state, event_name, function_index, priority);

            return 0;
        }
        else {
            return luaL_error(state, "Invalid number of arguments in function events.addListener.");
        }
    }

    int lua_event_remove_listeners(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args != 1) {
            return luaL_error(state, "Invalid number of arguments in function events.removeListeners.");
        }
        auto event_name = luaL_checkstring(state, 1);
        remove_all_event_listeners(state, event_name);
        return 0;
    }

    #define POPULATE_EVENT_WITH_NO_CONTEXT_FUNCTION(Event, event_name) \
        static void populate_##event_name##_events(Event &event, EventPriority priority) noexcept { \
            auto plugins = Plugins::get_lua_plugins(); \
            for(auto *&plugin : plugins) { \
                if(plugin->loaded()) { \
                    auto *state = plugin->state(); \
                    call_events_by_priority(state, #event_name, priority, nullptr); \
                } \
            } \
        }

    #define SET_UP_EVENT(Event, event_name) { \
        create_event_table(state, #event_name); \
        Event::subscribe([](Event &context) { \
            populate_##event_name##_events(context, EVENT_PRIORITY_HIGHEST); \
        }, EVENT_PRIORITY_HIGHEST); \
        \
        Event::subscribe([](Event &context) { \
            populate_##event_name##_events(context, EVENT_PRIORITY_ABOVE_DEFAULT); \
        }, EVENT_PRIORITY_ABOVE_DEFAULT); \
        \
        Event::subscribe([](Event &context) { \
            populate_##event_name##_events(context, EVENT_PRIORITY_DEFAULT); \
        }, EVENT_PRIORITY_DEFAULT); \
        \
        Event::subscribe([](Event &context) { \
            populate_##event_name##_events(context, EVENT_PRIORITY_LOWEST); \
        }, EVENT_PRIORITY_LOWEST); \
    }

    POPULATE_EVENT_WITH_NO_CONTEXT_FUNCTION(FrameEvent, frame)
    POPULATE_EVENT_WITH_NO_CONTEXT_FUNCTION(TickEvent, tick)

    static const luaL_Reg events_functions[] = {
        {"addListener", lua_event_add_listener},
        {"removeListeners", lua_event_remove_listeners},
        {nullptr, nullptr}
    };

    void set_up_events_table(lua_State *state, int table_idx) noexcept {
        int table_abs_idx = lua_absindex(state, table_idx);
        
        SET_UP_EVENT(FrameEvent, frame);
        SET_UP_EVENT(TickEvent, tick);

        lua_pushvalue(state, table_abs_idx);
        create_functions_table(state, "events", events_functions);
        lua_pop(state, 1); 
    }

    #undef POPULATE_EVENT_WITH_NO_CONTEXT_FUNCTION
    #undef SET_UP_EVENT
}
