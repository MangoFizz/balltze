// SPDX-License-Identifier: GPL-3.0-only

#include "api.hpp"

namespace Balltze::Plugins {
    void lua_set_logger_functions(lua_State *state) noexcept;
    void lua_set_fmt_functions(lua_State *state) noexcept;

    void lua_open_balltze_api(lua_State *state) {
        lua_newtable(state);
        lua_set_logger_functions(state);
        lua_set_fmt_functions(state);
        lua_setglobal(state, "balltze");   
    }
}
