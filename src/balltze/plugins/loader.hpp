// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__PLUGINS__LOADER_HPP
#define BALLTZE__PLUGINS__LOADER_HPP

#include <vector>
#include <filesystem>
#include <lua.hpp>

namespace Balltze::Plugins {
    class LuaPlugin;

    /**
     * Get the path to the plugins directory.
     * 
     * @return the path to the plugins directory
     */
    std::filesystem::path get_plugins_path() noexcept;

    /**
     * Initialize the plugins path.
     * This function creates the plugins directory if it does not exist.
     */
    void init_plugins_path();

    /**
     * Get the path to a specific plugin.
     * 
     * @param handle the handle of the plugin
     * @return the path to the plugin
     */
    std::vector<LuaPlugin *> get_lua_plugins() noexcept;

    /**
     * Get the Lua plugin for a given Lua state.
     * 
     * @param state the Lua state
     * @return the Lua plugin or nullptr if not found
     */
    LuaPlugin *get_lua_plugin(lua_State *state) noexcept;

    /**
     * Set up plugins loader.
     */
    void set_up_plugins_loader() noexcept;
}

#endif
