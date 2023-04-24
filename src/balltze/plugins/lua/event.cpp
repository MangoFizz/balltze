// SPDX-License-Identifier: GPL-3.0-only

#include <lua.hpp>
#include <balltze/event.hpp>
#include <balltze/utils.hpp>
#include "../../logger.hpp"
#include "../plugin.hpp"
#include "../loader.hpp"
#include "helpers.hpp"

namespace Balltze::Plugins {
    using handle_t = std::size_t;

    static std::size_t tick_event_count = 0;

    static std::string event_priority_to_string(Event::EventPriority priority) {
        switch(priority) {
            case Event::EVENT_PRIORITY_LOWEST:
                return "lowest";
            case Event::EVENT_PRIORITY_DEFAULT:
                return "default";
            case Event::EVENT_PRIORITY_ABOVE_DEFAULT:
                return "above_default";
            case Event::EVENT_PRIORITY_HIGHEST:
                return "highest";
            default:
                return "unknown";
        }
    }

    static Event::EventPriority event_priority_from_string(const std::string &priority) {
        if(priority == "lowest") {
            return Event::EVENT_PRIORITY_LOWEST;
        }
        else if(priority == "default") {
            return Event::EVENT_PRIORITY_DEFAULT;
        }
        else if(priority == "above_default") {
            return Event::EVENT_PRIORITY_ABOVE_DEFAULT;
        }
        else if(priority == "highest") {
            return Event::EVENT_PRIORITY_HIGHEST;
        }
        else {
            throw std::invalid_argument("Invalid event priority.");
        }
    }

    static std::string event_time_to_string(Event::EventTime time) {
        switch(time) {
            case Event::EVENT_TIME_BEFORE:
                return "before";
            case Event::EVENT_TIME_AFTER:
                return "after";
            default:
                return "unknown";
        }
    }

    static Event::EventTime event_time_from_string(const std::string &time) {
        if(time == "before") {
            return Event::EVENT_TIME_BEFORE;
        }
        else if(time == "after") {
            return Event::EVENT_TIME_AFTER;
        }
        else {
            throw std::runtime_error("Invalid event time.");
        }
    }

    static void set_up_events_registry_table(lua_State *state) noexcept {
        auto balltze_module = Balltze::get_current_module();
        lua_pushlightuserdata(state, balltze_module);
        lua_gettable(state, LUA_REGISTRYINDEX);
        if(lua_isnil(state, -1)) {
            lua_pop(state, 1);

            lua_pushlightuserdata(state, balltze_module);
            lua_newtable(state);
            lua_settable(state, LUA_REGISTRYINDEX);

            lua_pushlightuserdata(state, balltze_module);
            lua_gettable(state, LUA_REGISTRYINDEX);
        }

        lua_newtable(state);
        lua_setfield(state, -2, "events");

        lua_pop(state, 1);
    }

    static int lua_get_events_registry_table(lua_State *state) noexcept {
        auto balltze_module = Balltze::get_current_module();
        lua_pushlightuserdata(state, balltze_module);
        lua_gettable(state, LUA_REGISTRYINDEX);
        if(lua_isnil(state, -1)) {
            return 0;
        }
        lua_getfield(state, -1, "events");
        lua_remove(state, -2);
        return 1;
    }

    static void set_up_event_table(lua_State *state, const char *name, lua_CFunction add, lua_CFunction remove, lua_CFunction remove_all) noexcept {
        lua_newtable(state);
        lua_pushcfunction(state, add);
        lua_setfield(state, -2, "add_listener");
        lua_pushcfunction(state, remove);
        lua_setfield(state, -2, "remove_listener");
        lua_pushcfunction(state, remove_all);
        lua_setfield(state, -2, "remove_all_listeners");
        lua_setfield(state, -2, name);

        lua_get_events_registry_table(state);
        lua_newtable(state);
        lua_setfield(state, -2, name);
        lua_pop(state, 1);
    }

    static int add_event_listener(lua_State *state, const char *name, int function_index, Event::EventPriority priority, lua_CFunction remove_function) noexcept {
        if(lua_isfunction(state, function_index)) {
            lua_get_events_registry_table(state);
            lua_getfield(state, -1, name);
            lua_remove(state, -2);
            
            auto priority_str = event_priority_to_string(priority);

            // Create priority table if it doesn't exist
            lua_getfield(state, -1, priority_str.c_str());
            if(lua_isnil(state, -1)) {
                lua_pop(state, 1);

                lua_newtable(state);
                lua_setfield(state, -2, priority_str.c_str());

                lua_getfield(state, -1, priority_str.c_str());
            }

            // Push function to the priority table
            std::string handle = std::to_string(++tick_event_count);
            lua_pushvalue(state, function_index);
            lua_setfield(state, -2, handle.c_str());

            // Pop priority table and event table
            lua_pop(state, 2);

            // Create listener handle
            lua_newtable(state);
            lua_pushinteger(state, tick_event_count);
            lua_setfield(state, -2, "_handle");
            lua_pushinteger(state, static_cast<int>(priority));
            lua_setfield(state, -2, "_priority");
            lua_pushstring(state, name);
            lua_setfield(state, -2, "_event");
            lua_pushcfunction(state, remove_function);
            lua_setfield(state, -2, "remove");

            return 1;
        }
        else {
            return luaL_error(state, "Invalid listener argument in function events.%s.add_listener.", name);
        }
    }

    static void remove_event_listener(lua_State *state, const char *name, int handle_index) noexcept {
        if(lua_istable(state, handle_index)) {
            lua_getfield(state, handle_index, "_handle");
            int handle = luaL_checkinteger(state, -1);
            lua_pop(state, 1);

            lua_getfield(state, handle_index, "_priority");
            auto priority = static_cast<Event::EventPriority>(luaL_checkinteger(state, -1));
            lua_pop(state, 1);

            lua_getfield(state, handle_index, "_event");
            auto *event_name = luaL_checkstring(state, -1);
            lua_pop(state, 1);

            if(std::strcmp(name, event_name) == 0) {
                lua_get_events_registry_table(state);
                lua_getfield(state, -1, name);
                lua_remove(state, -2);

                auto priority_str = event_priority_to_string(priority);
                lua_getfield(state, -1, priority_str.c_str());
                lua_remove(state, -2);

                lua_pushnil(state);
                lua_setfield(state, -2, std::to_string(handle).c_str());

                lua_pop(state, 1);
            }
            else {
                luaL_error(state, "Invalid listener handle in function events.%s.remove_listener: tried to remove listener for event %s, but handle is for event %s.", name, name, event_name);
            }
        }
        else {
            luaL_error(state, "Invalid listener handle in function events.%s.remove_listener.", name);
        }
    }

    static void remove_all_event_listeners(lua_State *state, const char *name) noexcept {
        lua_get_events_registry_table(state);
        lua_pushnil(state);
        lua_setfield(state, -2, name);
        lua_pop(state, 1);
    }

    template<typename Event>
    static void create_event_data_table(lua_State *state, Event &context) noexcept {
        // Create context table in current state
        lua_newtable(state);
        lua_pushlightuserdata(state, const_cast<Event *>(&context));
        lua_setfield(state, -2, "_context");
        lua_pushboolean(state, context.cancelled());
        lua_setfield(state, -2, "cancelled");
        lua_pushcfunction(state, +[](lua_State *state) -> int {
            if(lua_istable(state, 1)) {
                lua_getfield(state, 1, "_context");
                auto *context = static_cast<Event *>(lua_touserdata(state, -1));
                lua_pop(state, 1);
                if(context->cancellable()) {
                    context->cancel();
                    lua_pushboolean(state, true);
                    lua_setfield(state, 1, "cancelled");
                    return 0;
                }
                else {
                    return luaL_error(state, "Attempted to cancel non-cancellable event.");
                }
            }
            else {
                return luaL_error(state, "Invalid context argument in function events.tick.cancel.");
            }
        });
        lua_setfield(state, -2, "cancel");
        lua_pushstring(state, event_time_to_string(context.time).c_str());
        lua_setfield(state, -2, "time");
    }

    void call_events_by_priority(lua_State *state, Event::EventPriority priority, int event_table_index) noexcept {
            lua_get_events_registry_table(state);
            lua_getfield(state, -1, "tick");
            lua_remove(state, -2);

            auto priority_str = event_priority_to_string(priority);
            lua_getfield(state, -1, priority_str.c_str());
            if(lua_isnil(state, -1)) {
                lua_pop(state, 2);
                return;
            }

            if(lua_istable(state, -1)) {
                lua_newtable(state);

                // Get all event listeners
                lua_pushnil(state);
                while(lua_next(state, -3)) {
                    if(lua_isfunction(state, -1)) {
                        lua_rawseti(state, -3, lua_rawlen(state, -3) + 1);
                    }
                    else {
                        logger.debug("Invalid event listener in events.tick: expected function, got {}.", lua_typename(state, lua_type(state, -1)));
                        lua_pop(state, 1);
                    }
                }

                // Call all event listeners
                int size = lua_rawlen(state, -1);
                for(int i = 1; i <= size; i++) {
                    lua_rawgeti(state, -1, i);
                    lua_pushvalue(state, event_table_index - 4);
                    int res = lua_pcall(state, 1, 0, 0);
                    if(res != LUA_OK) {
                        logger.debug("Error in event listener in events.tick: {}.", lua_tostring(state, -1));
                        lua_pop(state, 1);
                    }
                }
                
                lua_pop(state, 1);
            }
            else {
                logger.debug("Invalid event listener in events.tick: expected table, got {}.", lua_typename(state, lua_type(state, -1)));
            }

            lua_pop(state, 2);
        };

    static int lua_event_tick_remove_listener(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 1) {
                remove_event_listener(state, "tick", 1);
                return 0;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function events.tick.remove_listener.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static int lua_event_tick_add_listener(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 2) {
                auto priority_str = luaL_checkstring(state, 2);
                try {
                    auto priority = event_priority_from_string(priority_str);
                    return add_event_listener(state, "tick", 1, priority, lua_event_tick_remove_listener);
                }
                catch(const std::invalid_argument &e) {
                    return luaL_error(state, "Invalid priority argument in function events.tick.add_listener: %s.", priority_str);
                }
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function events.tick.add_listener.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static int lua_event_tick_remove_all_listeners(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 0) {
                remove_all_event_listeners(state, "tick");
                return 0;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function events.tick.remove_all_listeners.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static void populate_tick_events(Event::TickEvent &context, Event::EventPriority priority) noexcept {
        auto plugins = get_lua_plugins();
        
        for(auto *&plugin : plugins) {
            auto *state = plugin->state();
            create_event_data_table(state, context);

            // Set arguments table
            lua_newtable(state);
            lua_pushinteger(state, context.args.tick_count);
            lua_setfield(state, -2, "tick_count");
            lua_pushinteger(state, context.args.delta_time_ms);
            lua_setfield(state, -2, "delta_time_ms");
            lua_setfield(state, -2, "args");

            call_events_by_priority(state, priority, -1);

            lua_pop(state, 1);
        }
    } 

    void lua_set_event_table(lua_State *state) noexcept {
        set_up_events_registry_table(state);

        // Events functions table
        lua_newtable(state);
        set_up_event_table(state, "tick", lua_event_tick_add_listener, lua_event_tick_remove_listener, lua_event_tick_remove_all_listeners);
        lua_setfield(state, -2, "event");

        // Set up event listeners
        static auto tick_event_highest = Event::TickEvent::subscribe([](Event::TickEvent &context) {
            populate_tick_events(context, Event::EVENT_PRIORITY_HIGHEST);
        }, Event::EVENT_PRIORITY_HIGHEST);

        static auto tick_event_above_default = Event::TickEvent::subscribe([](Event::TickEvent &context) {
            populate_tick_events(context, Event::EVENT_PRIORITY_ABOVE_DEFAULT);
        }, Event::EVENT_PRIORITY_ABOVE_DEFAULT);

        static auto tick_event_default = Event::TickEvent::subscribe([](Event::TickEvent &context) {
            populate_tick_events(context, Event::EVENT_PRIORITY_DEFAULT);
        }, Event::EVENT_PRIORITY_DEFAULT);

        static auto tick_event_lowest = Event::TickEvent::subscribe([](Event::TickEvent &context) {
            populate_tick_events(context, Event::EVENT_PRIORITY_LOWEST);
        }, Event::EVENT_PRIORITY_LOWEST);
    }
}
