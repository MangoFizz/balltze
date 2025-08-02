// SPDX-License-Identifier: GPL-3.0-only

/**
 * @file lua/helpers/plugin.hpp
 * @brief Helper functions for managing plugin Lua states and functions.
 * 
 * @details This header provides utilities for pushing and retrieving plugin Lua states,
 *          handling upvalues for plugin functions, managing plugin registry tables,
 *          and error handling within the Lua environment for plugins.
 */

#ifndef BALLTZE__LUA__API__HELPERS__PLUGIN_HPP
#define BALLTZE__LUA__API__HELPERS__PLUGIN_HPP

#include <lua.hpp>
#include "../../plugins/plugin.hpp"

namespace Balltze::Lua {
    /**
     * Pushes the plugin's main Lua state onto the stack as a light userdata.
     *
     * This allows Lua code to reference the main plugin Lua state, even from coroutines
     * or other Lua states. Typically used to set the main state as an upvalue for plugin
     * API functions, enabling consistent access to plugin context.
     *
     * @param state The Lua state to push.
     * @return Number of values pushed onto the stack (always 1).
     */
    int push_plugin_lua_state(lua_State *state) noexcept;

    /**
     * Check if the userdata at the given index is a plugin Lua state.
     * 
     * @param state The Lua state.
     * @param index The index of the userdata to check.
     * @return true if the userdata is a plugin Lua state, false otherwise
     */
    bool userdata_is_plugin_lua_state(lua_State *state, int index) noexcept;

    /**
     * Retrieve the main plugin Lua state from the given Lua state.
     * 
     * If the provided Lua state has the plugin's main Lua state stored as an upvalue,
     * it will be returned. Otherwise, it returns the given state itself.
     *
     * @param state The Lua state (may be a coroutine or the main plugin state).
     * @return The main plugin Lua state if available, otherwise the provided state.
     */
    lua_State *get_plugin_lua_state(lua_State *state) noexcept;

    /**
     * Get the index of an upvalue in a plugin function.
     * 
     * If the first upvalue is the plugin's Lua state, this function will adjust the index
     * to account for that.
     * 
     * @param state The Lua state.
     * @param upvalue_index The base upvalue index.
     * @return The adjusted upvalue index for plugin functions if applicable.
     */
    int plugin_func_upvalue_index(lua_State *state, int upvalue_index) noexcept;

    /**
     * Pushes a Lua function onto the stack that can be used as a plugin function.
     * The function will have the Lua state as its first upvalue.
     * 
     * @param state The main Lua state of the plugin.
     * @param func The C function to push.
     * @param upvalue_count The number of upvalues to pass to the function.
     */
    void push_plugin_function(lua_State *state, lua_CFunction func, int upvalue_count = 0) noexcept;

    /**
     * Pushes a plugin functions table onto the stack and sets it as field of the table 
     * at the given index.
     * 
     * @param state The main Lua state of the plugin.
     * @param name The name of the table to create.
     * @param table_index The index of the table to create.
     * @param functions The list of Lua C functions to push into the table.
     */
    void push_plugin_functions_table(lua_State *state, const char *name, int table_index, const luaL_Reg *functions) noexcept;

    /**
     * Error handler function to return the Lua traceback on error.
     * 
     * @param state the Lua state
     */
    int plugin_error_handler(lua_State *state) noexcept;

    /**
     * Create the plugin registry table in the Lua state.
     * This table is used to store various plugin-related data.
     * 
     * @param state Lua state
     */
    void create_plugin_registry(lua_State *state);

    /**
     * Get a table from the plugin registry or create it if it doesn't exist.
     * The table is returned on top of the stack.
     * 
     * @param state Lua state
     * @param name  Table name
     */
    void get_or_create_plugin_registry_table(lua_State *state, const char *name);

    /**
     * Clear a table from the plugin registry.
     * 
     * @param state Lua state
     * @param name  Table name
     */
    void clear_registry_plugin_registry_table(lua_State *state, const char *name);
}

#endif // BALLTZE__LUA__API__HELPERS__PLUGIN_HPP
