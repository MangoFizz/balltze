// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/api.hpp>
#include <balltze/utils.hpp>
#include "../../libraries/preloaded_libraries.hpp"
#include "../../helpers/registry.hpp"
#include "types.hpp"
#include "api.hpp"

namespace Balltze::Lua::Api {
    namespace V2 {
        void set_engine_table(lua_State *state) noexcept;
        void set_filesystem_functions(lua_State *state, int table_idx) noexcept;
        void set_misc_functions(lua_State *state, int table_idx) noexcept;
        void set_config_functions(lua_State *state, int table_idx) noexcept;
        void set_up_plugin_commands(lua_State *state, int table_idx) noexcept;
        void set_up_plugin_events(lua_State *state, int table_idx) noexcept;
        void set_up_plugin_logger(lua_State *state) noexcept;

        void define_types(lua_State *state) noexcept {
            define_engine_types(state);
            define_engine_tag_types(state);
            define_engine_tag_data_types(state);
            define_engine_cache_file_types(state);
            define_engine_game_engine_types(state);
            define_engine_object_types(state);
            define_engine_player_types(state);
            define_engine_unit_types(state);
            define_engine_item_types(state);
            define_engine_weapon_types(state);
            define_engine_device_types(state);
        }
    }

    using namespace V2;

    void open_balltze_api_v2(lua_State *state) {
        create_balltze_registry(state);

        define_types(state);
        
        lua_newtable(state);
        set_config_functions(state, -1);
        set_filesystem_functions(state, -1);
        set_misc_functions(state, -1);
        set_up_plugin_commands(state, -1);
        set_up_plugin_events(state, -1);
        set_up_plugin_logger(state);
        lua_setglobal(state, "Balltze");

        set_engine_table(state);
    }
}
