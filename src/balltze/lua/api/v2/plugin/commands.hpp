// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__LUA__API__CONSOLE_COMMAND_HPP
#define BALLTZE__LUA__API__CONSOLE_COMMAND_HPP

#include <string>
#include "../../../../command/command.hpp"
#include "../../../../plugins/plugin.hpp"

namespace Balltze::Lua::Api::V2 {
    /**
     * Set up the commands API for a plugin
     * 
     * @param state     Lua state of the plugin
     * @param table_idx Index of the table to set up the API in
     */
    void set_up_plugin_commands(lua_State *state, int table_idx) noexcept;
}

#endif
