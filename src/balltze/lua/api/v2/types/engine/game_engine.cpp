// SPDX-License-Identifier: GPL-3.0-only

#include "../../../../libraries/luastruct.hpp"
#include "types.hpp"
#include "game_engine.hpp"

namespace Balltze::Lua::Api::V2 {
    static void define_game_engine_types(lua_State *state) noexcept {
        LUAS_ENUM(state, GameEngineType);
        LUAS_ENUM_VARIANT(state, GameEngineType, "none", GAME_ENGINE_TYPE_NONE);
        LUAS_ENUM_VARIANT(state, GameEngineType, "ctf", GAME_ENGINE_TYPE_CTF);
        LUAS_ENUM_VARIANT(state, GameEngineType, "slayer", GAME_ENGINE_TYPE_SLAYER);
        LUAS_ENUM_VARIANT(state, GameEngineType, "oddball", GAME_ENGINE_TYPE_ODDBALL);
        LUAS_ENUM_VARIANT(state, GameEngineType, "king", GAME_ENGINE_TYPE_KING);
        LUAS_ENUM_VARIANT(state, GameEngineType, "race", GAME_ENGINE_TYPE_RACE);
        lua_pop(state, 1);
    }

    void push_engine_game_engine_type(lua_State *state, const GameEngineType &type, bool readonly) noexcept {
        LUAS_PUSH_OBJECT(state, GameEngineType, &type, readonly);
    }

    void define_engine_game_engine_types(lua_State *state) noexcept {
        define_game_engine_types(state);
    }
}