// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__LUA__API__TYPES__ENGINE_MAP_HPP
#define BALLTZE__LUA__API__TYPES__ENGINE_MAP_HPP

#include <lua.hpp>
#include <balltze/legacy_api/engine.hpp>
#include <balltze/legacy_api/event.hpp>

namespace Balltze::Lua::Api::V1 {
    using EngineCacheFileEngine = LegacyApi::Engine::CacheFileEngine;
    using EngineMapGameType = LegacyApi::Engine::MapGameType;
    using EngineMapHeader = LegacyApi::Engine::MapHeader;

    void push_meta_engine_map_header(lua_State *state, LegacyApi::Engine::MapHeader *data) noexcept;

    void define_engine_map_types(lua_State *state) noexcept;
}

#endif
