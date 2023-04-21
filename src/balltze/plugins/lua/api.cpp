// SPDX-License-Identifier: GPL-3.0-only

#include "api.hpp"

namespace Balltze::Plugins {
    void lua_set_logger_table(lua_State *state) noexcept;
    void lua_set_fmt_table(lua_State *state) noexcept;
    void lua_set_engine_table(lua_State *state) noexcept;
    void lua_set_chimera_table(lua_State *state) noexcept;

    void lua_open_balltze_api(lua_State *state) {
        lua_newtable(state);
        lua_set_logger_table(state);
        lua_set_fmt_table(state);
        lua_set_engine_table(state);
        lua_setglobal(state, "balltze");   
    }
}
