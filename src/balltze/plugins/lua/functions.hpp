// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__PLUGINS__LUA__FUNCTIONS_HPP
#define BALLTZE__PLUGINS__LUA__FUNCTIONS_HPP

#include <lua.hpp>

namespace Balltze::Plugins {
    void lua_set_logger_table(lua_State *state) noexcept;
    void lua_set_engine_table(lua_State *state) noexcept;
    void lua_set_event_table(lua_State *state) noexcept;
    void lua_set_features_table(lua_State *state) noexcept;
    void lua_set_command_table(lua_State *state) noexcept;
    void lua_set_chimera_table(lua_State *state) noexcept;
    void lua_set_output_table(lua_State *state) noexcept;
    void lua_set_math_table(lua_State *state) noexcept;
    void lua_set_misc_table(lua_State *state) noexcept;
}

#endif
