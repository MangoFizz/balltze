// SPDX-License-Identifier: GPL-3.0-only

#include <lua.hpp>
#include <balltze/legacy_api/engine.hpp>
#include "../../../../helpers/function_table.hpp"

namespace Balltze::Lua::Api::V2 {
    static int execute_script(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 1 || args == 4 || args == 5) {
            const char *script = luaL_checkstring(state, 1);
            LegacyApi::Engine::execute_script(script);
            return 0;
        }
        else {
            return luaL_error(state, "Invalid number of arguments in function Engine.script.executeScript");
        }
    }

    static const luaL_Reg engine_script_functions[] = {
        {"execute", execute_script},
        {nullptr, nullptr}
    };

    void set_engine_script_functions(lua_State *state, int table_idx) noexcept {
        int abs_idx = lua_absindex(state, table_idx);
        lua_newtable(state);
        set_functions_reg_array(state, -1, engine_script_functions);
        lua_setfield(state, abs_idx, "script");
    }
}
