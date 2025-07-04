// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__LUA__API__TYPES__ENGINE_RASTERIZER_HPP
#define BALLTZE__LUA__API__TYPES__ENGINE_RASTERIZER_HPP

#include <lua.hpp>
#include <balltze/engine.hpp>
#include <balltze/event.hpp>

namespace Balltze::Lua::Api::V1 {
    using EngineCameraData = Engine::CameraData;
    using EngineResolution = Engine::Resolution;
    using EngineCameraType = Engine::CameraType;
    using EngineRasterizerVertex = Engine::ScreenQuad::Vertex;
    using EngineRasterizerQuad = Engine::ScreenQuad;

    void push_meta_engine_resolution(lua_State *state, Engine::Resolution *data) noexcept;
    void push_engine_resolution(lua_State *state, Engine::Resolution const *data) noexcept;

    void push_meta_engine_camera_data(lua_State *state, Engine::CameraData *data) noexcept;

    void define_engine_rasterizer_types(lua_State *state) noexcept;

    void push_meta_engine_rasterizer_vertex(lua_State *state, Engine::ScreenQuad::Vertex *data) noexcept;
    void push_meta_engine_rasterizer_quad(lua_State *state, Engine::ScreenQuad *data) noexcept;
}

#endif
