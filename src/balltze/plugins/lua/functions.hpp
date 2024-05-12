// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__PLUGINS__LUA__FUNCTIONS_HPP
#define BALLTZE__PLUGINS__LUA__FUNCTIONS_HPP

#include <lua.hpp>

namespace Balltze::Plugins::Lua {
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
}

#endif
