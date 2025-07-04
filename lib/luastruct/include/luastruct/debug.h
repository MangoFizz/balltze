// SPDX-License-Identifier: GPL-3.0-only

/**
 * debug.h
 * This header file contains functions and helpers used for debugging LuaStruct 
 * types and general Lua state operations. Most of this functions have no return 
 * value, the output of the operations are printed to stdout.
 */

#ifndef LUASTRUCT_DEBUG_H
#define LUASTRUCT_DEBUG_H

#ifndef __cplusplus
#include <lua.h>
#else
#include <lua.hpp>
extern "C" {
#endif

#include "luastruct.h"

#define STR2(s) # s
#define STR(s) STR2(s)

#ifdef DEBUG
#define LUAS_DEBUG_MSG(...) \
    do { \
        fprintf(stderr, __FILE__ ":" STR(__LINE__) ": " __VA_ARGS__); \
    } while(false)
#else
#define LUAS_DEBUG_MSG(...) \
    do { \
    } while(false)
#endif

#define LUAS_ASSERT(state, condition, message) \
    do { \
        if(!(condition)) { \
            luaL_error(state, "Assertion failed: %s", message); \
        } \
    } while(false)

/**
 * Retrieves the name of a Luastruct type as a string.
 * @param type The LuastructType value for which the name is to be retrieved.
 * @return A constant string representing the name of the specified LuastructType.
 */
const char *luastruct_name_for_type(LuastructType type);

/**
 * Print to stdout the registered types in the Lua state.
 * @param state Lua state.
 */
void luastruct_print_registered_types(lua_State *state);

/**
 * Print to stdout the definition of a struct type.
 * @param state Lua state.
 * @param name Name of the struct type.
 */
void luastruct_print_struct_definition(lua_State *state, const char *name);

/**
 * Loads and executes a Lua script file into the given Lua state.
 * @param state A pointer to the Lua state where the script will be loaded.
 * @param filename The path to the Lua script file to be loaded.
 */
void luastruct_load_lua_script(lua_State *state, const char *filename);

/**
 * Calls a Lua function with the given name and number of arguments.
 * @note This function does not handle the return value of the Lua function.
 * @param state A pointer to the Lua state where the function is defined.
 * @param function_name The name of the Lua function to be called.
 * @param n_args The number of arguments to pass to the function.
 */
void luastruct_call_lua_function(lua_State *state, const char *function_name, int n_args);

#ifdef __cplusplus
}
#endif

#endif 
