// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__LUA__API__V2__TYPES__ENGINE__GAME_ENGINE_HPP
#define BALLTZE__LUA__API__V2__TYPES__ENGINE__GAME_ENGINE_HPP

#include <lua.hpp>
#include <impl/game/game_engine.h>

namespace Balltze::Lua::Api::V2 {
    /**
     * Push a GameEngineType to the Lua stack.
     * @param state Lua state
     * @param header The GameEngineType to push
     * @param readonly If true, the header will be read-only in Lua
     */
    void push_engine_game_engine_type(lua_State *state, const GameEngineType &header, bool readonly = false) noexcept;

    /**
     * Define all game engine types in the Lua state.
     */
    void define_engine_game_engine_types(lua_State *state) noexcept;
}

#endif
