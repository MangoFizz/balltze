// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/api.hpp>
#include <balltze/utils.hpp>
#include "../../libraries/preloaded_libraries.hpp"
#include "types.hpp"
#include "api.hpp"

namespace Balltze::Lua {
    void set_logger_table(lua_State *state) noexcept;
    void set_engine_table(lua_State *state) noexcept;
    void set_event_table(lua_State *state) noexcept;
    void set_features_table(lua_State *state) noexcept;
    void set_command_table(lua_State *state) noexcept;
    void set_chimera_table(lua_State *state) noexcept;
    void set_output_table(lua_State *state) noexcept;
    void set_math_table(lua_State *state) noexcept;
    void set_misc_table(lua_State *state) noexcept;
    void set_filesystem_table(lua_State *state) noexcept;
    void set_config_table(lua_State *state) noexcept;
    void set_memory_function(lua_State *state) noexcept;
    void set_devkit_table(lua_State *state) noexcept;

    void define_engine_structs(lua_State *state) noexcept {
        define_engine_types(state);
        define_engine_tag_types(state);
        define_engine_tag_data_types(state);
        define_engine_gamestate_object_types(state);
        define_engine_gamestate_item_object_types(state);
        define_engine_gamestate_device_object_types(state);
        define_engine_gamestate_unit_object_types(state);
        declare_engine_gamestate_player_types(state);
        define_engine_rasterizer_types(state);
        define_engine_map_types(state);
        define_engine_user_interface_types(state);
        define_ringworld_saved_games(state);
        define_balltze_event_types(state);
    }

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
        set_memory_function(state);
        set_misc_table(state);
        set_config_table(state);
        set_filesystem_table(state);
        set_devkit_table(state);
        if(get_balltze_side() == BALLTZE_SIDE_CLIENT) {
            set_output_table(state);
            set_features_table(state);
            set_chimera_table(state);
        }
        lua_setglobal(state, "Balltze");

        // Set up engine table
        set_engine_table(state);

        // Set up preloaded libraries
        set_preloaded_libraries(state);

        Lua::define_engine_structs(state);
    }
}
