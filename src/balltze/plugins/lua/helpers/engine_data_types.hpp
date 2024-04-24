// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__PLUGINS__LUA__HELPERS__ENGINE_DATA_TYPES_HPP
#define BALLTZE__PLUGINS__LUA__HELPERS__ENGINE_DATA_TYPES_HPP

#include <string>
#include <lua.hpp>
#include <balltze/engine.hpp>

namespace Balltze::Plugins {
    void lua_push_engine_matrix(lua_State *state, Engine::Matrix &matrix) noexcept;
    void lua_push_engine_resolution(lua_State *state, Engine::Resolution &resolution) noexcept;
    void lua_push_engine_resource_handle(lua_State *state, Engine::ResourceHandle *handle) noexcept;
    void lua_push_engine_resource_handle(lua_State *state, Engine::ResourceHandle *handle) noexcept;
    void lua_push_engine_object_handle(lua_State *state, Engine::ObjectHandle *handle) noexcept;
    void lua_push_engine_tag_handle(lua_State *state, Engine::TagHandle *handle) noexcept;
    void lua_push_engine_player_handle(lua_State *state, Engine::PlayerHandle *handle) noexcept;

    Engine::ColorARGB lua_to_color_argb(lua_State *state, int index);
    Engine::Point2DInt lua_to_point2_d_int(lua_State *state, int index);
    Engine::Point3D lua_to_point3_d(lua_State *state, int index);
    Engine::ColorARGBInt lua_to_color_a_r_g_b_int(lua_State *state, int index);
    std::shared_ptr<Engine::Matrix> lua_to_engine_matrix(lua_State *state, int index) noexcept;
}

#endif 
