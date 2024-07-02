// SPDX-License-Identifier: GPL-3.0-only

#include <lua.hpp>
#include <balltze/engine.hpp>
#include "../../../../logger.hpp"
#include "../../../plugin.hpp"
#include "../../../loader.hpp"
#include "../../libraries.hpp"
#include "../../types.hpp"

namespace Balltze::Plugins::Lua {
    static int execute_script(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 1 || args == 4 || args == 5) {
            const char *script = luaL_checkstring(state, 1);
            Engine::execute_script(script);
            return 0;
        }
        else {
            return luaL_error(state, "Invalid number of arguments in function Engine.script.executeScript");
        }
    }

    static const luaL_Reg engine_renderer_functions[] = {
        {"executeScript", execute_script},
        {nullptr, nullptr}
    };

    void set_engine_hsc_functions(lua_State *state) noexcept {
        lua_newtable(state);
        luaL_setfuncs(state, engine_renderer_functions, 0);
        lua_setfield(state, -2, "hsc");
    }
}
