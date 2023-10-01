// SPDX-License-Identifier: GPL-3.0-only

#include "helpers.hpp"

extern "C" {
    #include "lua-fmt/lfmt.h"
}

namespace Balltze::Plugins {
    static const luaL_Reg fmt_functions[] = {
        { "format", Lformat },
        { nullptr, nullptr }
    };

    void lua_set_fmt_table(lua_State *state) noexcept {
        lua_create_functions_table(state, "fmt", fmt_functions);
    }
}
