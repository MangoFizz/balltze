// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/api.hpp>
#include <balltze/utils.hpp>
#include "functions.hpp"
#include "libraries.hpp"
#include "types.hpp"
#include "api.hpp"

namespace Balltze::Plugins::Lua {
    void open_balltze_api(lua_State *state) {
        // Set up balltze registry table
        auto balltze_module = Balltze::get_current_module();
        lua_pushlightuserdata(state, balltze_module);
        lua_newtable(state);
        lua_settable(state, LUA_REGISTRYINDEX);

        // Set up Balltze funcion tables
        lua_newtable(state);
        set_logger_table(state);
        set_event_table(state);
        set_command_table(state);
        set_math_table(state);
        set_misc_table(state);
        set_config_table(state);
        if(get_balltze_side() == BALLTZE_SIDE_CLIENT) {
            set_output_table(state);
            set_features_table(state);
            set_chimera_table(state);
        }
        lua_setglobal(state, "Balltze");

        // Set up engine table
        set_engine_table(state);

        // Set up prelaoded libraries
        set_preloaded_libraries(state);

        Lua::define_engine_structs(state);
    }
}
