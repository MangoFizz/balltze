// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__LUA__API__TYPES__ENGINE_GAMESTATE_PLAYER_HPP
#define BALLTZE__LUA__API__TYPES__ENGINE_GAMESTATE_PLAYER_HPP

#include <lua.hpp>
#include <balltze/legacy_api/engine.hpp>

namespace Balltze::Lua::Api::V1 {
    using EnginePlayerMultiplayerStatistics = LegacyApi::Engine::PlayerMultiplayerStatistics;
    using EngineNetworkPlayerColor = LegacyApi::Engine::NetworkPlayerColor;
    using EnginePlayerObjectiveMode = LegacyApi::Engine::PlayerObjectiveMode;
    using EnginePlayer = LegacyApi::Engine::Player;

    void push_meta_engine_player_multiplayer_statistics(lua_State *state, LegacyApi::Engine::PlayerMultiplayerStatistics *statistics) noexcept;

    void push_meta_engine_player(lua_State *state, LegacyApi::Engine::Player *player) noexcept;

    void declare_engine_gamestate_player_types(lua_State *state) noexcept;
}

#endif
