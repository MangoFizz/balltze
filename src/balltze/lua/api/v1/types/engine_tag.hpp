// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__LUA__API__TYPES__ENGINE_TAG_HPP
#define BALLTZE__LUA__API__TYPES__ENGINE_TAG_HPP

#include <string>
#include <lua.hpp>
#include "engine_types.hpp"

namespace Balltze::Lua::Api::V1 {
    using EngineTagClass = LegacyApi::Engine::TagClassInt;
    using EngineTag = LegacyApi::Engine::Tag;
    using EngineTagDependency = LegacyApi::Engine::TagDependency;
    using EngineTagDataOffset = LegacyApi::Engine::TagDataOffset;
    using EngineTagString = LegacyApi::Engine::TagString;

    LegacyApi::Engine::TagClassInt get_tag_class(lua_State *state, int index) noexcept;

    void push_meta_engine_tag(lua_State *state, LegacyApi::Engine::Tag *tag) noexcept;

    void push_meta_engine_tag_dependency(lua_State *state, LegacyApi::Engine::TagDependency *tag_dependency) noexcept;

    void push_meta_engine_tag_data_offset(lua_State *state, LegacyApi::Engine::TagDataOffset *tag_data_offset) noexcept;

    void define_engine_tag_types(lua_State *state) noexcept;
}

#endif 
