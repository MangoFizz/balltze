// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__PLUGINS__LUA__TYPES__ENGINE_MAP_HPP
#define BALLTZE__PLUGINS__LUA__TYPES__ENGINE_MAP_HPP

#include <lua.hpp>
#include <balltze/engine.hpp>
#include <balltze/event.hpp>

namespace Balltze::Plugins::Lua {
    using EngineCacheFileEngine = Engine::CacheFileEngine;
    using EngineMapGameType = Engine::MapGameType;
    using EngineMapHeader = Engine::MapHeader;

    void push_meta_engine_map_header(lua_State *state, Engine::MapHeader *data) noexcept;

    void define_engine_map_types(lua_State *state) noexcept;
}

#endif
