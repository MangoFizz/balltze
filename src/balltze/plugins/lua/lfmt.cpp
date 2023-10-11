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
        luaL_newlibtable(state, fmt_functions);
        luaL_setfuncs(state, fmt_functions, 0);
        lua_setglobal(state, "Fmt");
    }
}
