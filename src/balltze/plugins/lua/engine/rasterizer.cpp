// SPDX-License-Identifier: GPL-3.0-only

#include <lua.hpp>
#include <balltze/engine.hpp>
#include "../../../logger.hpp"
#include "../../plugin.hpp"
#include "../../loader.hpp"
#include "../helpers.hpp"

namespace Balltze::Plugins {
    static int lua_engine_get_resolution(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 0) {
                auto resolution = Engine::Rasterizer::get_resolution();
                lua_newtable(state);
                lua_pushinteger(state, resolution.width);
                lua_setfield(state, -2, "width");
                lua_pushinteger(state, resolution.height);
                lua_setfield(state, -2, "height");
                return 1;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function engine.get_resolution.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static const luaL_Reg engine_renderer_functions[] = {
        {"getResolution", lua_engine_get_resolution},
        {nullptr, nullptr}
    };

    void set_engine_rasterizer_functions(lua_State *state) noexcept {
        luaL_newlibtable(state, engine_renderer_functions);
        luaL_setfuncs(state, engine_renderer_functions, 0);
        lua_setfield(state, -2, "rasterizer");
    }
}
