// SPDX-License-Identifier: GPL-3.0-only

#include <lua.hpp>
#include <balltze/engine.hpp>
#include "../../../../logger.hpp"
#include "../../../plugin.hpp"
#include "../../../loader.hpp"
#include "../../libraries.hpp"
#include "../../types.hpp"

namespace Balltze::Plugins::Lua {
    static int engine_get_resolution(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 0) {
                push_engine_resolution(state, &Engine::get_resolution());
                return 1;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function Engine.rasterizer.getResolution.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static const luaL_Reg engine_renderer_functions[] = {
        {"getResolution", engine_get_resolution},
        {nullptr, nullptr}
    };

    void set_engine_rasterizer_functions(lua_State *state) noexcept {
        lua_newtable(state);
        luaL_setfuncs(state, engine_renderer_functions, 0);
        lua_setfield(state, -2, "rasterizer");
    }
}
