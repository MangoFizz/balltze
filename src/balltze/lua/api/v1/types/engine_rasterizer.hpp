// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__LUA__API__TYPES__ENGINE_RASTERIZER_HPP
#define BALLTZE__LUA__API__TYPES__ENGINE_RASTERIZER_HPP

#include <lua.hpp>
#include <balltze/legacy_api/engine.hpp>
#include <balltze/legacy_api/event.hpp>

namespace Balltze::Lua::Api::V1 {
    using EngineCameraData = LegacyApi::Engine::CameraData;
    using EngineResolution = LegacyApi::Engine::Resolution;
    using EngineCameraType = LegacyApi::Engine::CameraType;
    using EngineRasterizerVertex = LegacyApi::Engine::ScreenQuad::Vertex;
    using EngineRasterizerQuad = LegacyApi::Engine::ScreenQuad;

    void push_meta_engine_resolution(lua_State *state, LegacyApi::Engine::Resolution *data) noexcept;
    void push_engine_resolution(lua_State *state, LegacyApi::Engine::Resolution const *data) noexcept;

    void push_meta_engine_camera_data(lua_State *state, LegacyApi::Engine::CameraData *data) noexcept;

    void define_engine_rasterizer_types(lua_State *state) noexcept;

    void push_meta_engine_rasterizer_vertex(lua_State *state, LegacyApi::Engine::ScreenQuad::Vertex *data) noexcept;
    void push_meta_engine_rasterizer_quad(lua_State *state, LegacyApi::Engine::ScreenQuad *data) noexcept;
}

#endif
