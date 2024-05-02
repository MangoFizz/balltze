// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__PLUGINS__LUA__TYPES__ENGINE_TYPES_HPP
#define BALLTZE__PLUGINS__LUA__TYPES__ENGINE_TYPES_HPP

#include <string>
#include <lua.hpp>
#include <balltze/engine.hpp>

namespace Balltze::Plugins::Lua {
    using EngineResourceHandle = Engine::ResourceHandle;
    using EngineColorARGBInt = Engine::ColorARGBInt;
    using EngineColorARGB = Engine::ColorARGB;
    using EnginePoint2D = Engine::Point2D;
    using EnginePoint2DInt = Engine::Point2DInt;
    using EnginePoint3D = Engine::Point3D;
    using EngineRectangle2D = Engine::Rectangle2D;
    using EngineEuler2D = Engine::Euler2D;
    using EngineEuler3D = Engine::Euler3D;
    using EngineVector2D = Engine::Vector2D;
    using EngineVector3D = Engine::Vector3D;
    using EngineQuaternion = Engine::Quaternion;
    using EnginePlane3D = Engine::Plane3D;
    using EnginePlane2D = Engine::Plane2D;
    using EngineMatrix = Engine::Matrix;

    /**
     * @todo Fix structure snake_case naming in function names.
     * Examples: 
     * lua_define_engine_point2_d_struct -> lua_define_engine_point_2d_struct
     * lua_push_engine_color_a_r_g_b_int -> lua_push_engine_color_argb_int
     */

    void lua_define_engine_resource_handle_struct(lua_State *state) noexcept;
    void lua_push_engine_resource_handle(lua_State *state, Engine::ResourceHandle *handle) noexcept;

    void lua_define_engine_color_a_r_g_b_int_struct(lua_State *state) noexcept;
    void lua_push_engine_color_a_r_g_b_int(lua_State *state, Engine::ColorARGBInt *color) noexcept;

    void lua_define_engine_color_a_r_g_b_struct(lua_State *state) noexcept;
    void lua_push_engine_color_a_r_g_b(lua_State *state, Engine::ColorARGB *color) noexcept;

    void lua_define_engine_point2_d_struct(lua_State *state) noexcept;
    void lua_push_engine_point2_d(lua_State *state, Engine::Point2D *point) noexcept;

    void lua_define_engine_point2_d_int_struct(lua_State *state) noexcept;
    void lua_push_engine_point2_d_int(lua_State *state, Engine::Point2DInt *point) noexcept;

    void lua_define_engine_point3_d_struct(lua_State *state) noexcept;
    void lua_push_engine_point3_d(lua_State *state, Engine::Point3D *point) noexcept;

    void lua_define_engine_rectangle2_d_struct(lua_State *state) noexcept;
    void lua_push_engine_rectangle2_d(lua_State *state, Engine::Rectangle2D *rectangle) noexcept;

    void lua_define_engine_euler2_d_struct(lua_State *state) noexcept;
    void lua_push_engine_euler2_d(lua_State *state, Engine::Euler2D *euler) noexcept;

    void lua_define_engine_euler3_d_struct(lua_State *state) noexcept;
    void lua_push_engine_euler3_d(lua_State *state, Engine::Euler3D *euler) noexcept;

    void lua_define_engine_vector2_d_struct(lua_State *state) noexcept;
    void lua_push_engine_vector2_d(lua_State *state, Engine::Vector2D *vector) noexcept;

    void lua_define_engine_vector3_d_struct(lua_State *state) noexcept;
    void lua_push_engine_vector3_d(lua_State *state, Engine::Vector3D *vector) noexcept;

    void lua_define_engine_quaternion_struct(lua_State *state) noexcept;
    void lua_push_engine_quaternion(lua_State *state, Engine::Quaternion *quaternion) noexcept;

    void lua_define_engine_plane3_d_struct(lua_State *state) noexcept;
    void lua_push_engine_plane3_d(lua_State *state, Engine::Plane3D *plane) noexcept;

    void lua_define_engine_plane2_d_struct(lua_State *state) noexcept;
    void lua_push_engine_plane2_d(lua_State *state, Engine::Plane2D *plane) noexcept;

    void lua_define_engine_matrix_struct(lua_State *state) noexcept;
    void lua_push_engine_matrix(lua_State *state, Engine::Matrix *matrix) noexcept;
}

#endif 
