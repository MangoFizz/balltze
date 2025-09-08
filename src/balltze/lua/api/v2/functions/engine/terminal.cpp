// SPDX-License-Identifier: GPL-3.0-only

#include <lua.hpp>
#include <impl/terminal/terminal.h>
#include "../../../../helpers/function_table.hpp"
#include "../../../../libraries/lfmt.hpp"
#include "../../types.hpp"

namespace Balltze::Lua::Api::V2 {
    static int lua_engine_terminal_print(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args >= 1) {
            auto color = get_color_argb(state, 1);
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

            ColorARGB color_value = color.value();
            terminal_printf(&color_value, "%s", message.c_str());
        }
        else {
            return luaL_error(state, "Invalid number of arguments in function Engine.terminal.print.");
        }
        return 0;
    }

    static const luaL_Reg engine_terminal_functions[] = {
        {"print", lua_engine_terminal_print},
        {nullptr, nullptr}
    };

    void set_engine_terminal_functions(lua_State *state, int table_idx) noexcept {
        int abs_idx = lua_absindex(state, table_idx);
        lua_newtable(state);
        set_functions_reg_array(state, -1, engine_terminal_functions);
        lua_setfield(state, abs_idx, "terminal");
    }
}
