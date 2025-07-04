// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__LUA_DEBUG__LUA_STATE_SNAPSHOT_HPP
#define BALLTZE__LUA_DEBUG__LUA_STATE_SNAPSHOT_HPP

#include <string>
#include <optional>
#include <lua.hpp>

namespace Balltze::Lua {
    /**
     * Takes a snapshot of the Lua state and saves it to a file.
     * The snapshot includes information about all objects in the Lua state,
     * such as their type, size, name, description, value, and parent.
     * 
     * @param state The Lua state to snapshot.
     * @param output_file The file to save the snapshot to.
     */
    void capture_lua_state_snapshot(lua_State *state, const std::string &output_file);

    /**
     * Retrieves the size of the luacstruct registry in the Lua state.
     * 
     * @param state The Lua state to inspect.
     * @return An optional containing the size of the luacstruct registry if found, otherwise std::nullopt.
     */
    std::optional<size_t> get_luacstruct_registry_size(lua_State *state);
}

#endif 
