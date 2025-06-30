// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__LUA__API__TYPES__RINGWORLD_SAVED_GAMES_HPP
#define BALLTZE__LUA__API__TYPES__RINGWORLD_SAVED_GAMES_HPP

#include <lua.hpp>
#include <impl/saved_games/player_profile.h>

namespace Balltze::Lua {
    void push_ringworld_player_profile(lua_State *state, PlayerProfile *profile) noexcept;
    void define_ringworld_saved_games(lua_State *state) noexcept;
}

#endif
