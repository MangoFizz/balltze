// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/api.hpp>
#include <balltze/utils.hpp>
#include "functions.hpp"
#include "libraries.hpp"
#include "types.hpp"
#include "api.hpp"

namespace Balltze::Plugins {
    void lua_open_balltze_api(lua_State *state) {
        // Set up balltze registry table
        auto balltze_module = Balltze::get_current_module();
        lua_pushlightuserdata(state, balltze_module);
        lua_newtable(state);
        lua_settable(state, LUA_REGISTRYINDEX);

        // Set up Balltze funcion tables
        lua_newtable(state);
        lua_set_logger_table(state);
        lua_set_event_table(state);
        lua_set_command_table(state);
        lua_set_math_table(state);
        lua_set_misc_table(state);
        lua_set_config_table(state);
        if(get_balltze_side() == BALLTZE_SIDE_CLIENT) {
            lua_set_output_table(state);
            lua_set_features_table(state);
            lua_set_chimera_table(state);
        }
        lua_setglobal(state, "Balltze");

        // Set up engine table
        lua_set_engine_table(state);

        // Set up prelaoded libraries
        lua_set_preloaded_libraries(state);

        Lua::lua_define_engine_structs(state);
    }
}
