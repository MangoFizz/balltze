#ifndef LUA_MEMORY_SNAPSHOT_H
#define LUA_MEMORY_SNAPSHOT_H

#include <lua.h>

/**
 * Callback to report each object info and relationship;
 * when size is 0, report object and parent for relationship only
 * @param obj Pointer to the object
 * @param type Type of the object: -1 (global state), 4 (string), 5 (table), 6 (function), 7 (userdata), 8 (proto)
 * @param size Size of the object
 * @param name Name of the object
 * @param desc Description of the object
 * @param value Value of the object (for strings, tables, etc.)
 * @param parent Pointer to the parent object (if any)
 */
typedef void(*LuaSnapshotCallback)(const void *obj, int type, size_t size, const char *name, const char *desc, const char *value, const void *parent);

/**
 * Take a memory snapshot of the Lua state by traversing all living objects.
 * The callback is called for each object and relationship.
 * @param state Lua state
 * @param callback Callback function to report each object info and relationship
 */
void lua_memory_snapshot(lua_State *state, LuaSnapshotCallback callback);

#endif
