// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__PLUGINS__LUA__TYPES__ENGINE_GAMESTATE_PLAYER_HPP
#define BALLTZE__PLUGINS__LUA__TYPES__ENGINE_GAMESTATE_PLAYER_HPP

#include <lua.hpp>
#include <balltze/engine.hpp>

namespace Balltze::Plugins {
    using EnginePlayerMultiplayerStatistics = Engine::PlayerMultiplayerStatistics;
    using EngineNetworkPlayerColor = Engine::NetworkPlayerColor;
    using EnginePlayerObjectiveMode = Engine::PlayerObjectiveMode;
    using EnginePlayer = Engine::Player;

    void push_meta_engine_player_multiplayer_statistics(lua_State *state, Engine::PlayerMultiplayerStatistics *statistics) noexcept;

    void push_meta_engine_player(lua_State *state, Engine::Player *player) noexcept;

    void declare_engine_gamestate_player_types(lua_State *state) noexcept;
}

#endif
