// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__LUA__API__V2__TYPES__ENGINE__PLAYER_HPP
#define BALLTZE__LUA__API__V2__TYPES__ENGINE__PLAYER_HPP

#include <optional>
#include <lua.hpp>
#include <impl/player/player.h>

namespace Balltze::Lua::Api::V2 {
    /**
     * Push a PlayerHandle to the Lua stack.
     * @param state Lua state
     * @param handle The PlayerHandle to push
     * @param readonly If true, the handle will be read-only in Lua
     */
    void push_player_handle(lua_State *state, const PlayerHandle &handle, bool readonly = false) noexcept;

    /**
     * Get a PlayerHandle from the Lua stack.
     * @param state Lua state
     * @param idx The index of the value on the Lua stack
     * @return An optional PlayerHandle, empty if the value is not a valid handle
     */
    std::optional<PlayerHandle> get_player_handle(lua_State *state, int idx) noexcept;
    
    /**
     * Push a Player to the Lua stack.
     * 
     * @param state Lua state
     * @param player The Player to push
     * @param readonly If true, the player will be read-only in Lua
     */
    void push_player(lua_State *state, const Player &player, bool readonly = false) noexcept;

    /**
     * Define the Player type in Lua.
     * 
     * @param state Lua state
     */
    void define_engine_player_types(lua_State *state) noexcept;
}

#endif
