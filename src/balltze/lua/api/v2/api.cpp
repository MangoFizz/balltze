// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/api.hpp>
#include <balltze/utils.hpp>
#include "../../libraries/preloaded_libraries.hpp"
#include "types.hpp"
#include "api.hpp"

namespace Balltze::Lua::Api {
    namespace V2 {
        void set_command_functions(lua_State *state, int table_idx) noexcept;
        void set_event_functions(lua_State *state, int table_idx) noexcept;
        void set_engine_table(lua_State *state) noexcept;

        void define_types(lua_State *state) noexcept {
            define_engine_types(state);
            define_engine_tag_types(state);
            define_engine_tag_data_types(state);
        }
    }

    using namespace V2;

    void open_balltze_api_v2(lua_State *state) {
        define_types(state);
        
        lua_newtable(state);
        set_command_functions(state, -1);
        set_event_functions(state, -1);
        lua_setglobal(state, "Balltze");

        set_engine_table(state);
    }
}
