// SPDX-License-Identifier: GPL-3.0-only

#include <lua.hpp>
#include <balltze/engine.hpp>
#include "../../../../helpers/function_table.hpp"
#include "../../types.hpp"

namespace Balltze::Lua {
    static int execute_script(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 0) {
            push_engine_resolution(state, &Engine::get_resolution());
            return 1;
        }
        else {
            return luaL_error(state, "Invalid number of arguments in function Engine.rasterizer.getResolution.");
        }
    }

    static const luaL_Reg engine_renderer_functions[] = {
        {"getResolution", execute_script},
        {nullptr, nullptr}
    };

    void set_engine_rasterizer_functions(lua_State *state) noexcept {
        create_functions_table(state, "rasterizer", engine_renderer_functions);
    }        

}
