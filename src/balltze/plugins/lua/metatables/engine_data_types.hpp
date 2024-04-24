// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__PLUGINS__LUA__METATABLES__ENGINE_DATA_TYPES_HPP
#define BALLTZE__PLUGINS__LUA__METATABLES__ENGINE_DATA_TYPES_HPP

#include <string>
#include <lua.hpp>
#include <balltze/engine.hpp>

namespace Balltze::Plugins {
    void lua_engine_attach_tag_data_metatable(lua_State *state) noexcept;

    void lua_push_meta_engine_tag(lua_State *state, Engine::Tag &tag, bool read_only = false) noexcept;
    void lua_push_meta_engine_color_a_r_g_b_int(lua_State *state, Engine::ColorARGBInt &color, bool read_only = false) noexcept;
    void lua_push_meta_engine_tag_dependency(lua_State *state, Engine::TagDependency &dependency, bool read_only = false) noexcept;
    void lua_push_meta_engine_point2_d(lua_State *state, Engine::Point2D &point, bool read_only = false) noexcept;
    void lua_push_meta_engine_point3_d(lua_State *state, Engine::Point3D &point, bool read_only = false) noexcept;
    void lua_push_meta_engine_tag_data_offset(lua_State *state, Engine::TagDataOffset &offset, bool read_only = false) noexcept;
    void lua_push_meta_engine_color_a_r_g_b(lua_State *state, Engine::ColorARGB &color, bool read_only = false) noexcept;
    void lua_push_meta_engine_rectangle2_d(lua_State *state, Engine::Rectangle2D &rectangle, bool read_only = false) noexcept;
    void lua_push_meta_engine_rectangle2_d_f(lua_State *state, Engine::Rectangle2DF &rectangle, bool read_only = false) noexcept;
    void lua_push_meta_engine_point2_d_int(lua_State *state, Engine::Point2DInt &point, bool read_only = false) noexcept;
    void lua_push_meta_engine_euler2_d(lua_State *state, Engine::Euler2D &euler, bool read_only = false) noexcept;
    void lua_push_meta_engine_euler3_d(lua_State *state, Engine::Euler3D &euler, bool read_only = false) noexcept;
    void lua_push_meta_engine_euler3_d_p_y_r(lua_State *state, Engine::Euler3DPYR &euler, bool read_only = false) noexcept;
    void lua_push_meta_engine_vector2_d(lua_State *state, Engine::Vector2D &vector, bool read_only = false) noexcept;
    void lua_push_meta_engine_vector3_d(lua_State *state, Engine::Vector3D &vector, bool read_only = false) noexcept;
    void lua_push_meta_engine_color_r_g_b(lua_State *state, Engine::ColorRGB &color, bool read_only = false) noexcept;
    void lua_push_meta_engine_quaternion(lua_State *state, Engine::Quaternion &quaternion, bool read_only = false) noexcept;
    void lua_push_meta_engine_plane3_d(lua_State *state, Engine::Plane3D &plane, bool read_only = false) noexcept;
    void lua_push_meta_engine_plane2_d(lua_State *state, Engine::Plane2D &plane, bool read_only = false) noexcept;
    void lua_push_meta_engine_rotation_matrix(lua_State *state, Engine::RotationMatrix &matrix, bool read_only = false) noexcept;
    void lua_push_meta_engine_model_node(lua_State *state, Engine::ModelNode &node, bool read_only = false) noexcept;
}

#endif 
