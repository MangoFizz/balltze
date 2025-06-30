// SPDX-License-Identifier: GPL-3.0-only

#include <cstring>
#include <ctime>
#include "../../../debug/lua_state_snapshot.hpp"
#include "../../../helpers/function_table.hpp"

namespace Balltze::Lua {
    /**
     * Captures a snapshot of the Lua state and saves it to a file in the game's
     * directory. The snapshot includes all objects in the Lua state, their types,
     * sizes, names, descriptions, values, and parent relationships.
     * 
     * @param filename The name of the file to save the snapshot to; if not provided,
     *                 a default filename based on the current date and time will be used.
     */
    int lua_devkit_capture_state_snapshot(lua_State *state) {
        int args = lua_gettop(state);
        const char *filename = nullptr;
        if(args == 0 || lua_isnil(state, 1)) {
            std::time_t t = std::time(nullptr);
            std::tm tm;
            localtime_s(&tm, &t);
            char buffer[128];
            std::strftime(buffer, sizeof(buffer), "lua_state_snapshot_%Y%m%d_%H%M%S.csv", &tm);
            filename = buffer;
        } 
        else if(args == 1) {
            filename = luaL_checkstring(state, 1);
        } 
        else {
            return luaL_error(state, "Invalid number of arguments in function Balltze.devkit.captureStateSnapshot.");
        } 
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
