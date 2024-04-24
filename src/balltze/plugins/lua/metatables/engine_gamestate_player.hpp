// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__PLUGINS__LUA__METATABLES__ENGINE_GAMESTATE_PLAYER_HPP
#define BALLTZE__PLUGINS__LUA__METATABLES__ENGINE_GAMESTATE_PLAYER_HPP

#include <string>
#include <balltze/engine.hpp>

namespace Balltze::Plugins {
    void lua_push_meta_engine_player_multiplayer_statistics(lua_State *state, Engine::PlayerMultiplayerStatistics &statistics, bool read_only = false) noexcept;
    void lua_push_meta_engine_player(lua_State *state, Engine::Player &player, bool read_only = false) noexcept;
}

#endif
