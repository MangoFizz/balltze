// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/api.hpp>
#include <balltze/utils.hpp>
#include "../../libraries/preloaded_libraries.hpp"
#include "types.hpp"
#include "api.hpp"

namespace Balltze::Lua::Api {
    namespace V2 {
        void set_ringworld_table(lua_State *state) noexcept;

        void define_types(lua_State *state) noexcept {
            define_ringworld_types(state);
            define_ringworld_tag_types(state);
            define_ringworld_tag_data_types(state);
        }
    }

    using namespace V2;

    void open_balltze_api_v2(lua_State *state) {
        set_ringworld_table(state);
        define_types(state);
    }
}
