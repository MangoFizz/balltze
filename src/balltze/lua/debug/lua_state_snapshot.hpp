// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__DEVKIT__STATE_SNAPSHOT_HPP
#define BALLTZE__DEVKIT__STATE_SNAPSHOT_HPP

#include <lua.hpp>
#include <string>

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
}

#endif 
