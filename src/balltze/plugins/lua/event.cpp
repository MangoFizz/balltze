// SPDX-License-Identifier: GPL-3.0-only

#include <lua.hpp>
#include <balltze/event.hpp>
#include <balltze/utils.hpp>
#include "../../logger.hpp"
#include "../plugin.hpp"
#include "../loader.hpp"
#include "helpers.hpp"

namespace Balltze::Event {
    extern std::string ip_address_int_to_string(std::uint32_t address_int) noexcept;
}

namespace Balltze::Plugins {
    using handle_t = std::size_t;

    static std::size_t tick_event_count = 0;

    static void set_up_events_registry_table(lua_State *state) noexcept {
        auto balltze_module = Balltze::get_current_module();
        lua_pushlightuserdata(state, balltze_module);
        lua_gettable(state, LUA_REGISTRYINDEX);
        if(lua_isnil(state, -1)) {
            lua_pop(state, 1);
            logger.error("Could not find balltze Lua registry table");
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
            
            auto priority_str = Event::event_priority_to_string(priority);

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

                auto priority_str = Event::event_priority_to_string(priority);
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
        lua_pushstring(state, Balltze::Event::event_time_to_string(context.time).c_str());
        lua_setfield(state, -2, "time");
    }

    void call_events_by_priority(lua_State *state, std::string name, Event::EventPriority priority, int event_table_index) noexcept {
        lua_get_events_registry_table(state);
        lua_getfield(state, -1, name.c_str());
        lua_remove(state, -2);

        auto priority_str = Event::event_priority_to_string(priority);
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
                    logger.error("Invalid event listener in events.tick: expected function, got {}.", lua_typename(state, lua_type(state, -1)));
                    lua_pop(state, 1);
                }
            }

            // Call all event listeners
            bool cancelled = false;
            int size = lua_rawlen(state, -1);
            for(int i = 1; i <= size; i++) {
                lua_rawgeti(state, -1, i);
                lua_pushvalue(state, event_table_index - 4);
                int res = lua_pcall(state, 1, 0, 0);
                if(res != LUA_OK) {
                    logger.error("Error in event listener in events.tick: {}.", lua_tostring(state, -1));
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
                    auto priority = Event::event_priority_from_string(priority_str);
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

            call_events_by_priority(state, "tick", priority, -1);

            lua_pop(state, 1);
        }
    }

    static int lua_event_frame_remove_listener(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 1) {
                remove_event_listener(state, "frame", 1);
                return 0;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function events.frame.remove_listener.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static int lua_event_frame_add_listener(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 2) {
                auto priority_str = luaL_checkstring(state, 2);
                try {
                    auto priority = Event::event_priority_from_string(priority_str);
                    return add_event_listener(state, "frame", 1, priority, lua_event_frame_remove_listener);
                }
                catch(const std::invalid_argument &e) {
                    return luaL_error(state, "Invalid priority argument in function events.frame.add_listener: %s.", priority_str);
                }
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function events.frame.add_listener.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static int lua_event_frame_remove_all_listeners(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 0) {
                remove_all_event_listeners(state, "frame");
                return 0;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function events.frame.remove_all_listeners.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static void populate_frame_events(Event::FrameEvent &context, Event::EventPriority priority) noexcept {
        auto plugins = get_lua_plugins();
        
        for(auto *&plugin : plugins) {
            auto *state = plugin->state();
            create_event_data_table(state, context);

            // Set arguments table
            lua_newtable(state);
            lua_setfield(state, -2, "args");

            call_events_by_priority(state, "frame", priority, -1);

            lua_pop(state, 1);
        }
    }

    static int lua_event_map_file_load_remove_listener(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 1) {
                remove_event_listener(state, "map_file_load", 1);
                return 0;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function events.map_file_load.remove_listener.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static int lua_event_map_file_load_add_listener(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 2) {
                auto priority_str = luaL_checkstring(state, 2);
                try {
                    auto priority = Event::event_priority_from_string(priority_str);
                    return add_event_listener(state, "map_file_load", 1, priority, lua_event_map_file_load_remove_listener);
                }
                catch(const std::invalid_argument &e) {
                    return luaL_error(state, "Invalid priority argument in function events.map_file_load.add_listener: %s.", priority_str);
                }
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function events.map_file_load.add_listener.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static int lua_event_map_file_load_remove_all_listeners(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 0) {
                remove_all_event_listeners(state, "map_file_load");
                return 0;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function events.map_file_load.remove_all_listeners.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static void populate_map_file_load_events(Event::MapFileLoadEvent &context, Event::EventPriority priority) noexcept {
        auto plugins = get_lua_plugins();
        
        for(auto *&plugin : plugins) {
            auto *state = plugin->state();
            create_event_data_table(state, context);

            // Set arguments table
            lua_newtable(state);
            lua_pushstring(state, context.args.map_name.c_str());
            lua_setfield(state, -2, "map_name");
            lua_pushstring(state, context.args.map_path.c_str());
            lua_setfield(state, -2, "map_file");
            lua_setfield(state, -2, "args");

            call_events_by_priority(state, "map_file_load", priority, -1);

            lua_pop(state, 1);
        }
    }

    static int lua_event_server_connect_remove_listener(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 1) {
                remove_event_listener(state, "server_connect", 1);
                return 0;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function events.server_connect.remove_listener.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static int lua_event_server_connect_add_listener(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 2) {
                auto priority_str = luaL_checkstring(state, 2);
                try {
                    auto priority = Event::event_priority_from_string(priority_str);
                    return add_event_listener(state, "server_connect", 1, priority, lua_event_server_connect_remove_listener);
                }
                catch(const std::invalid_argument &e) {
                    return luaL_error(state, "Invalid priority argument in function events.server_connect.add_listener: %s.", priority_str);
                }
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function events.server_connect.add_listener.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static int lua_event_server_connect_remove_all_listeners(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 0) {
                remove_all_event_listeners(state, "server_connect");
                return 0;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function events.server_connect.remove_all_listeners.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static void populate_server_connect_events(Event::ServerConnectEvent &context, Event::EventPriority priority) noexcept {
        auto plugins = get_lua_plugins();
        
        for(auto *&plugin : plugins) {
            auto *state = plugin->state();
            create_event_data_table(state, context);

            auto address = Event::ip_address_int_to_string(context.args.address);
            auto password = std::string(context.args.password.begin(), context.args.password.end());
            
            // Set arguments table
            lua_newtable(state);
            lua_pushstring(state, address.c_str());
            lua_setfield(state, -2, "address");
            lua_pushinteger(state, context.args.port);
            lua_setfield(state, -2, "port");
            lua_pushstring(state, password.c_str());
            lua_setfield(state, -2, "password");
            lua_setfield(state, -2, "args");

            call_events_by_priority(state, "server_connect", priority, -1);

            lua_pop(state, 1);
        }
    }

    static int lua_event_camera_remove_listener(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 1) {
                remove_event_listener(state, "camera", 1);
                return 0;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function events.camera.remove_listener.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static int lua_event_camera_add_listener(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 2) {
                auto priority_str = luaL_checkstring(state, 2);
                try {
                    auto priority = Event::event_priority_from_string(priority_str);
                    return add_event_listener(state, "camera", 1, priority, lua_event_camera_remove_listener);
                }
                catch(const std::invalid_argument &e) {
                    return luaL_error(state, "Invalid priority argument in function events.camera.add_listener: %s.", priority_str);
                }
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function events.camera.add_listener.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static int lua_event_camera_remove_all_listeners(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 0) {
                remove_all_event_listeners(state, "camera");
                return 0;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function events.camera.remove_all_listeners.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static void populate_camera_events(Event::CameraEvent &context, Event::EventPriority priority) noexcept {
        auto plugins = get_lua_plugins();
        
        for(auto *&plugin : plugins) {
            auto *state = plugin->state();
            create_event_data_table(state, context);

            // Set arguments table
            lua_newtable(state);
            lua_push_meta_engine_camera_data(state, *context.args.camera);
            lua_setfield(state, -2, "camera");
            lua_pushstring(state, camera_type_to_string(context.args.type).c_str());
            lua_setfield(state, -2, "type");
            lua_setfield(state, -2, "args");

            call_events_by_priority(state, "camera", priority, -1);

            lua_pop(state, 1);
        }
    }

    static int lua_event_game_input_remove_listener(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args >= 1) {
                remove_event_listener(state, "game_input", 1);
                return 0;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function events.game_input.remove_listener.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static int lua_event_game_input_add_listener(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);

            if(args >= 2) {
                auto priority_str = luaL_checkstring(state, 2);
                try {
                    auto priority = Event::event_priority_from_string(priority_str);
                    return add_event_listener(state, "game_input", 1, priority, lua_event_game_input_remove_listener);
                }
                catch(const std::invalid_argument &e) {
                    return luaL_error(state, "Invalid priority argument in function events.game_input.add_listener: %s.", priority_str);
                }
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function events.game_input.add_listener.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static int lua_event_game_input_remove_all_listeners(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);

            if(args == 0) {
                remove_all_event_listeners(state, "game_input");
                return 0;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function events.game_input.remove_all_listeners.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static void populate_game_input_events(Event::GameInputEvent &context, Event::EventPriority priority) noexcept {
        auto plugins = get_lua_plugins();
        
        for(auto *&plugin : plugins) {
            auto *state = plugin->state();
            create_event_data_table(state, context);

            // Set arguments table
            lua_newtable(state);
            lua_pushstring(state, Engine::get_input_device_name(context.args.device).c_str());
            lua_setfield(state, -2, "device");
            lua_pushboolean(state, context.args.mapped);
            lua_setfield(state, -2, "mapped");
            switch(context.args.device) {
                case Engine::INPUT_DEVICE_MOUSE:
                    lua_pushstring(state, Engine::get_mouse_button_name(static_cast<Engine::MouseButton>(context.args.button.key_code)).c_str());
                    break;

                case Engine::INPUT_DEVICE_GAMEPAD:
                    lua_pushstring(state, Engine::get_gamepad_button_name(context.args.button.gamepad_button).c_str());
                    break;

                default:
                    lua_pushinteger(state, context.args.button.key_code);
                    break;
            }
            lua_setfield(state, -2, "button");
            lua_setfield(state, -2, "args");

            call_events_by_priority(state, "game_input", priority, -1);

            lua_pop(state, 1);
        }
    }

    static int lua_event_map_load_remove_listener(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);

            if(args == 1) {
                remove_event_listener(state, "map_load", 1);
                return 0;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function events.map_load.remove_listener.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }

        return 0;
    }

    static int lua_event_map_load_add_listener(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);

            if(args == 2) {
                auto priority_str = luaL_checkstring(state, 2);
                try {
                    auto priority = Event::event_priority_from_string(priority_str);
                    return add_event_listener(state, "map_load", 1, priority, lua_event_map_load_remove_listener);
                }
                catch(const std::invalid_argument &e) {
                    return luaL_error(state, "Invalid priority argument in function events.map_load.add_listener: %s.", priority_str);
                }
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function events.map_load.add_listener.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }

        return 0;
    }

    static int lua_event_map_load_remove_all_listeners(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);

            if(args == 0) {
                remove_all_event_listeners(state, "map_load");
                return 0;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function events.map_load.remove_all_listeners.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }

        return 0;
    }

    static void populate_map_load_events(Event::MapLoadEvent &context, Event::EventPriority priority) noexcept {
        auto plugins = get_lua_plugins();
        
        for(auto *&plugin : plugins) {
            auto *state = plugin->state();
            create_event_data_table(state, context);

            // Set arguments table
            lua_newtable(state);
            lua_pushstring(state, context.args.name.c_str());
            lua_setfield(state, -2, "map_name");
            lua_setfield(state, -2, "args");

            call_events_by_priority(state, "map_load", priority, -1);

            lua_pop(state, 1);
        }
    }

    void lua_set_event_table(lua_State *state) noexcept {
        set_up_events_registry_table(state);

        // Events functions table
        lua_newtable(state);
        set_up_event_table(state, "tick", lua_event_tick_add_listener, lua_event_tick_remove_listener, lua_event_tick_remove_all_listeners);
        set_up_event_table(state, "frame", lua_event_frame_add_listener, lua_event_frame_remove_listener, lua_event_frame_remove_all_listeners);
        set_up_event_table(state, "map_file_load", lua_event_map_file_load_add_listener, lua_event_map_file_load_remove_listener, lua_event_map_file_load_remove_all_listeners);
        set_up_event_table(state, "server_connect", lua_event_server_connect_add_listener, lua_event_server_connect_remove_listener, lua_event_server_connect_remove_all_listeners);
        set_up_event_table(state, "camera", lua_event_camera_add_listener, lua_event_camera_remove_listener, lua_event_camera_remove_all_listeners);
        set_up_event_table(state, "game_input", lua_event_game_input_add_listener, lua_event_game_input_remove_listener, lua_event_game_input_remove_all_listeners);
        set_up_event_table(state, "map_load", lua_event_map_load_add_listener, lua_event_map_load_remove_listener, lua_event_map_load_remove_all_listeners);
        lua_setfield(state, -2, "event");

        // Set up tick event
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

        // Set up frame event
        static auto frame_event_highest = Event::FrameEvent::subscribe([](Event::FrameEvent &context) {
            populate_frame_events(context, Event::EVENT_PRIORITY_HIGHEST);
        }, Event::EVENT_PRIORITY_HIGHEST);

        static auto frame_event_above_default = Event::FrameEvent::subscribe([](Event::FrameEvent &context) {
            populate_frame_events(context, Event::EVENT_PRIORITY_ABOVE_DEFAULT);
        }, Event::EVENT_PRIORITY_ABOVE_DEFAULT);

        static auto frame_event_default = Event::FrameEvent::subscribe([](Event::FrameEvent &context) {
            populate_frame_events(context, Event::EVENT_PRIORITY_DEFAULT);
        }, Event::EVENT_PRIORITY_DEFAULT);

        static auto frame_event_lowest = Event::FrameEvent::subscribe([](Event::FrameEvent &context) {
            populate_frame_events(context, Event::EVENT_PRIORITY_LOWEST);
        }, Event::EVENT_PRIORITY_LOWEST);

        // Set up map file load event
        static auto map_file_load_event_highest = Event::MapFileLoadEvent::subscribe([](Event::MapFileLoadEvent &context) {
            populate_map_file_load_events(context, Event::EVENT_PRIORITY_HIGHEST);
        }, Event::EVENT_PRIORITY_HIGHEST);

        static auto map_file_load_event_above_default = Event::MapFileLoadEvent::subscribe([](Event::MapFileLoadEvent &context) {
            populate_map_file_load_events(context, Event::EVENT_PRIORITY_ABOVE_DEFAULT);
        }, Event::EVENT_PRIORITY_ABOVE_DEFAULT);

        static auto map_file_load_event_default = Event::MapFileLoadEvent::subscribe([](Event::MapFileLoadEvent &context) {
            populate_map_file_load_events(context, Event::EVENT_PRIORITY_DEFAULT);
        }, Event::EVENT_PRIORITY_DEFAULT);

        static auto map_file_load_event_lowest = Event::MapFileLoadEvent::subscribe([](Event::MapFileLoadEvent &context) {
            populate_map_file_load_events(context, Event::EVENT_PRIORITY_LOWEST);
        }, Event::EVENT_PRIORITY_LOWEST);

        // Set up server connect event
        static auto server_connect_event_highest = Event::ServerConnectEvent::subscribe([](Event::ServerConnectEvent &context) {
            populate_server_connect_events(context, Event::EVENT_PRIORITY_HIGHEST);
        }, Event::EVENT_PRIORITY_HIGHEST);

        static auto server_connect_event_above_default = Event::ServerConnectEvent::subscribe([](Event::ServerConnectEvent &context) {
            populate_server_connect_events(context, Event::EVENT_PRIORITY_ABOVE_DEFAULT);
        }, Event::EVENT_PRIORITY_ABOVE_DEFAULT);

        static auto server_connect_event_default = Event::ServerConnectEvent::subscribe([](Event::ServerConnectEvent &context) {
            populate_server_connect_events(context, Event::EVENT_PRIORITY_DEFAULT);
        }, Event::EVENT_PRIORITY_DEFAULT);

        static auto server_connect_event_lowest = Event::ServerConnectEvent::subscribe([](Event::ServerConnectEvent &context) {
            populate_server_connect_events(context, Event::EVENT_PRIORITY_LOWEST);
        }, Event::EVENT_PRIORITY_LOWEST);

        // Set up camera event
        static auto camera_event_highest = Event::CameraEvent::subscribe([](Event::CameraEvent &context) {
            populate_camera_events(context, Event::EVENT_PRIORITY_HIGHEST);
        }, Event::EVENT_PRIORITY_HIGHEST);

        static auto camera_event_above_default = Event::CameraEvent::subscribe([](Event::CameraEvent &context) {
            populate_camera_events(context, Event::EVENT_PRIORITY_ABOVE_DEFAULT);
        }, Event::EVENT_PRIORITY_ABOVE_DEFAULT);

        static auto camera_event_default = Event::CameraEvent::subscribe([](Event::CameraEvent &context) {
            populate_camera_events(context, Event::EVENT_PRIORITY_DEFAULT);
        }, Event::EVENT_PRIORITY_DEFAULT);

        static auto camera_event_lowest = Event::CameraEvent::subscribe([](Event::CameraEvent &context) {
            populate_camera_events(context, Event::EVENT_PRIORITY_LOWEST);
        }, Event::EVENT_PRIORITY_LOWEST);

        // Set up game input event
        static auto game_input_event_highest = Event::GameInputEvent::subscribe([](Event::GameInputEvent &context) {
            populate_game_input_events(context, Event::EVENT_PRIORITY_HIGHEST);
        }, Event::EVENT_PRIORITY_HIGHEST);

        static auto game_input_event_above_default = Event::GameInputEvent::subscribe([](Event::GameInputEvent &context) {
            populate_game_input_events(context, Event::EVENT_PRIORITY_ABOVE_DEFAULT);
        }, Event::EVENT_PRIORITY_ABOVE_DEFAULT);

        static auto game_input_event_default = Event::GameInputEvent::subscribe([](Event::GameInputEvent &context) {
            populate_game_input_events(context, Event::EVENT_PRIORITY_DEFAULT);
        }, Event::EVENT_PRIORITY_DEFAULT);

        static auto game_input_event_lowest = Event::GameInputEvent::subscribe([](Event::GameInputEvent &context) {
            populate_game_input_events(context, Event::EVENT_PRIORITY_LOWEST);
        }, Event::EVENT_PRIORITY_LOWEST);

        // Set up map load event
        static auto map_load_event_highest = Event::MapLoadEvent::subscribe([](Event::MapLoadEvent &context) {
            populate_map_load_events(context, Event::EVENT_PRIORITY_HIGHEST);
        }, Event::EVENT_PRIORITY_HIGHEST);

        static auto map_load_event_above_default = Event::MapLoadEvent::subscribe([](Event::MapLoadEvent &context) {
            populate_map_load_events(context, Event::EVENT_PRIORITY_ABOVE_DEFAULT);
        }, Event::EVENT_PRIORITY_ABOVE_DEFAULT);

        static auto map_load_event_default = Event::MapLoadEvent::subscribe([](Event::MapLoadEvent &context) {
            populate_map_load_events(context, Event::EVENT_PRIORITY_DEFAULT);
        }, Event::EVENT_PRIORITY_DEFAULT);

        static auto map_load_event_lowest = Event::MapLoadEvent::subscribe([](Event::MapLoadEvent &context) {
            populate_map_load_events(context, Event::EVENT_PRIORITY_LOWEST);
        }, Event::EVENT_PRIORITY_LOWEST);
    }
}
