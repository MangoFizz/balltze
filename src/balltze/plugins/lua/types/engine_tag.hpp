// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__PLUGINS__LUA__TYPES__ENGINE_TAG_HPP
#define BALLTZE__PLUGINS__LUA__TYPES__ENGINE_TAG_HPP

#include <string>
#include <lua.hpp>
#include "engine_types.hpp"

namespace Balltze::Plugins::Lua {
    using EngineTagClass = Engine::TagClassInt;
    using EngineTag = Engine::Tag;
    using EngineTagDependency = Engine::TagDependency;
    using EngineTagDataOffset = Engine::TagDataOffset;
    using EngineTagString = Engine::TagString;

    void lua_define_engine_tag_class_enum(lua_State *state) noexcept;

    void lua_define_engine_tag_base_struct(lua_State *state) noexcept;
    void lua_push_engine_tag(lua_State *state, Engine::Tag *tag) noexcept;

    void lua_define_engine_tag_dependency_struct(lua_State *state) noexcept;
    void lua_push_engine_tag_dependency(lua_State *state, Engine::TagDependency *tag_dependency) noexcept;

    void lua_define_engine_tag_data_offset_struct(lua_State *state) noexcept;
    void lua_push_engine_tag_data_offset(lua_State *state, Engine::TagDataOffset *tag_data_offset) noexcept;
}

#endif 
