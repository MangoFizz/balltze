// SPDX-License-Identifier: GPL-3.0-only

#include <cstring>
#include "../../../debug/lua_state_snapshot.hpp"
#include "../../../helpers/function_table.hpp"

namespace Balltze::Lua {
    /**
     * Captures a snapshot of the Lua state and saves it to a file.
     * This function is intended for debugging purposes and provides a detailed
     * view of the Lua state, including all objects, their types, sizes, names,
     * descriptions, values, and parent relationships.
     * 
     * #param filename The name of the file to save the snapshot to.
     */
    int lua_devkit_capture_state_snapshot(lua_State *state) {
        int args = lua_gettop(state);
        if(args != 1) {
            return luaL_error(state, "Invalid number of arguments in function Balltze.devkit.captureStateSnapshot.");
        }
        const char *filename = luaL_checkstring(state, 1);
        if(!filename || std::strlen(filename) == 0) {
            return luaL_error(state, "Invalid filename in function Balltze.devkit.captureStateSnapshot.");
        }
        capture_lua_state_snapshot(state, filename);
        return 0;
    }

    static const luaL_Reg devkit_functions[] = {
        {"captureStateSnapshot", lua_devkit_capture_state_snapshot},
        {nullptr, nullptr}
    };

    void set_devkit_table(lua_State *state) noexcept {
        create_functions_table(state, "devkit", devkit_functions);
    }
}
