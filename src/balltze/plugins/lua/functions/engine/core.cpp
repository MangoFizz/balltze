// SPDX-License-Identifier: GPL-3.0-only

#include <lua.hpp>
#include <balltze/engine.hpp>
#include "../../../../logger.hpp"
#include "../../../plugin.hpp"
#include "../../../loader.hpp"
#include "../../libraries.hpp"
#include "../../types.hpp"
#include "../../helpers/function_table.hpp"

namespace Balltze::Plugins::Lua {
    static int engine_console_print(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args >= 1) {
            auto color = get_color_a_r_g_b(state, 1);
            bool has_color = color.has_value();
            if(has_color) {
                lua_remove(state, 1);
                args = lua_gettop(state);
            }
            else {
                color = {1.0, 1.0, 1.0, 1.0};
            }

            std::string message; 
            if(args > 1) { 
                Lformat(state); 
                message = luaL_checkstring(state, -1); 
                lua_pop(state, 1); 
            } 
            else { 
                message = luaL_checkstring(state, 1); 
            }

            Engine::console_print(message, *color);
        }
        else {
            return luaL_error(state, "Invalid number of arguments in function Engine.core.consolePrint.");
        }
        return 0;
    }

    static int engine_get_tick_count(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 0) {
            lua_pushinteger(state, Engine::get_tick_count());
            return 1;
        }
        else {
            return luaL_error(state, "Invalid number of arguments in function Engine.core.getTickCount.");
        }
    }

    static const luaL_Reg engine_core_functions[] = {
        {"consolePrint", engine_console_print},
        {"getTickCount", engine_get_tick_count},
        {nullptr, nullptr}
    };

    void set_engine_core_functions(lua_State *state) noexcept {
        luaL_newlibtable(state, engine_core_functions);
        luaL_setfuncs(state, engine_core_functions, 0);
        lua_setfield(state, -2, "core");
    }
}
