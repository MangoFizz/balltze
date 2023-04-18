// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__PLUGINS__LUA__HELPERS_HPP
#define BALLTZE__PLUGINS__LUA__HELPERS_HPP

#include <lua.hpp>
#include <balltze/plugin.hpp>
#include <balltze/engine.hpp>

namespace Balltze::Plugins {
    void lua_create_functions_table(lua_State *state, const char *name, const luaL_Reg *functions) noexcept;
    VersionNumber lua_check_version_number(lua_State* state, int index);
    Engine::ColorARGB lua_to_color_argb(lua_State *state, int index);
    std::string engine_edition_to_string(Engine::EngineEdition edition);
    Engine::EngineEdition engine_edition_from_string(const std::string &edition);
    std::string cache_file_engine_to_string(Engine::CacheFileEngine engine);
    Engine::CacheFileEngine cache_file_engine_from_string(const std::string &engine);
    std::string map_game_type_to_string(Engine::MapGameType type);
    Engine::MapGameType map_game_type_from_string(const std::string &type);
    std::string server_type_to_string(Engine::ServerType type);
    Engine::ServerType server_type_from_string(const std::string &type);
    std::string server_game_type_to_string(Engine::Gametype gametype);
    Engine::Gametype server_game_type_from_string(std::string gametype);
}   

#endif
