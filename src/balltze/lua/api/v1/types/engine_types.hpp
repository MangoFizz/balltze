// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__LUA__API__TYPES__ENGINE_TYPES_HPP
#define BALLTZE__LUA__API__TYPES__ENGINE_TYPES_HPP

#include <optional>
#include <lua.hpp>
#include <balltze/legacy_api/engine.hpp>

namespace Balltze::Lua::Api::V1 {
    using EngineResourceHandle = LegacyApi::Engine::ResourceHandle;
    using EngineColorARGBInt = LegacyApi::Engine::ColorARGBInt;
    using EngineColorARGB = LegacyApi::Engine::ColorARGB;
    using EngineColorRGB = LegacyApi::Engine::ColorRGB;
    using EnginePoint2D = LegacyApi::Engine::Point2D;
    using EnginePoint2DInt = LegacyApi::Engine::Point2DInt;
    using EnginePoint3D = LegacyApi::Engine::Point3D;
    using EngineRectangle2D = LegacyApi::Engine::Rectangle2D;
    using EngineRectangle2DF = LegacyApi::Engine::Rectangle2DF;
    using EngineEuler2D = LegacyApi::Engine::Euler2D;
    using EngineEuler3D = LegacyApi::Engine::Euler3D;
    using EngineEuler3DPYR = LegacyApi::Engine::Euler3DPYR;
    using EngineVector2D = LegacyApi::Engine::Vector2D;
    using EngineVector3D = LegacyApi::Engine::Vector3D;
    using EngineQuaternion = LegacyApi::Engine::Quaternion;
    using EnginePlane3D = LegacyApi::Engine::Plane3D;
    using EnginePlane2D = LegacyApi::Engine::Plane2D;
    using EngineMatrix = LegacyApi::Engine::Matrix;

    /**
     * @todo Fix structure snake_case naming in function names.
     * Examples: 
     * lua_define_engine_point2_d_struct -> lua_define_engine_point_2d_struct
     * lua_push_engine_color_a_r_g_b_int -> lua_push_engine_color_argb_int
     */

    void push_engine_resource_handle(lua_State *state, LegacyApi::Engine::ResourceHandle const &handle) noexcept;
    void push_meta_engine_resource_handle(lua_State *state, LegacyApi::Engine::ResourceHandle *handle) noexcept;
    void push_engine_object_handle(lua_State *state, LegacyApi::Engine::ObjectHandle const &handle) noexcept;
    void push_meta_engine_object_handle(lua_State *state, LegacyApi::Engine::ObjectHandle *handle) noexcept;
    void push_engine_tag_handle(lua_State *state, LegacyApi::Engine::TagHandle const &handle) noexcept;
    void push_meta_engine_tag_handle(lua_State *state, LegacyApi::Engine::TagHandle *handle) noexcept;
    void push_engine_player_handle(lua_State *state, LegacyApi::Engine::PlayerHandle const &handle) noexcept;
    void push_meta_engine_player_handle(lua_State *state, LegacyApi::Engine::PlayerHandle *handle) noexcept;
    std::optional<LegacyApi::Engine::ResourceHandle> get_engine_resource_handle(lua_State *state, int index) noexcept;

    void push_engine_color_a_r_g_b_int(lua_State *state, LegacyApi::Engine::ColorARGBInt const &color) noexcept;
    void push_meta_engine_color_a_r_g_b_int(lua_State *state, LegacyApi::Engine::ColorARGBInt *color) noexcept;
    std::optional<LegacyApi::Engine::ColorARGBInt> get_color_a_r_g_b_int(lua_State *state, int index) noexcept;

    void push_engine_color_a_r_g_b(lua_State *state, LegacyApi::Engine::ColorARGB const &color) noexcept;
    void push_meta_engine_color_a_r_g_b(lua_State *state, LegacyApi::Engine::ColorARGB *color) noexcept;
    std::optional<LegacyApi::Engine::ColorARGB> get_color_a_r_g_b(lua_State *state, int index) noexcept;

    void push_engine_color_r_g_b(lua_State *state, LegacyApi::Engine::ColorRGB const &color) noexcept;
    void push_meta_engine_color_r_g_b(lua_State *state, LegacyApi::Engine::ColorRGB *color) noexcept;
    std::optional<LegacyApi::Engine::ColorRGB> get_color_r_g_b(lua_State *state, int index) noexcept;

    void push_engine_point2_d(lua_State *state, LegacyApi::Engine::Point2D const &point) noexcept;
    void push_meta_engine_point2_d(lua_State *state, LegacyApi::Engine::Point2D *point) noexcept;
    std::optional<LegacyApi::Engine::Point2D> get_point2_d(lua_State *state, int index) noexcept;

    void push_engine_point2_d_int(lua_State *state, LegacyApi::Engine::Point2DInt const &point) noexcept;
    void push_meta_engine_point2_d_int(lua_State *state, LegacyApi::Engine::Point2DInt *point) noexcept;
    std::optional<LegacyApi::Engine::Point2DInt> get_point2_d_int(lua_State *state, int index) noexcept;

    void push_engine_point3_d(lua_State *state, LegacyApi::Engine::Point3D const &point) noexcept;
    void push_meta_engine_point3_d(lua_State *state, LegacyApi::Engine::Point3D *point) noexcept;
    std::optional<LegacyApi::Engine::Point3D> get_point3_d(lua_State *state, int index) noexcept;

    void push_engine_rectangle2_d(lua_State *state, LegacyApi::Engine::Rectangle2D const &rectangle) noexcept;
    void push_meta_engine_rectangle2_d(lua_State *state, LegacyApi::Engine::Rectangle2D *rectangle) noexcept;
    std::optional<LegacyApi::Engine::Rectangle2D> get_rectangle2_d(lua_State *state, int index) noexcept;

    void push_engine_rectangle2_d_f(lua_State *state, LegacyApi::Engine::Rectangle2DF const &rectangle) noexcept;
    void push_meta_engine_rectangle2_d_f(lua_State *state, LegacyApi::Engine::Rectangle2DF *rectangle) noexcept;
    std::optional<LegacyApi::Engine::Rectangle2DF> get_rectangle2_d_f(lua_State *state, int index) noexcept;

    void push_engine_euler2_d(lua_State *state, LegacyApi::Engine::Euler2D const &euler) noexcept;
    void push_meta_engine_euler2_d(lua_State *state, LegacyApi::Engine::Euler2D *euler) noexcept;
    std::optional<LegacyApi::Engine::Euler2D> get_euler2_d(lua_State *state, int index) noexcept;

    void push_engine_euler3_d(lua_State *state, LegacyApi::Engine::Euler3D const &euler) noexcept;
    void push_meta_engine_euler3_d(lua_State *state, LegacyApi::Engine::Euler3D *euler) noexcept;
    std::optional<LegacyApi::Engine::Euler3D> get_euler3_d(lua_State *state, int index) noexcept;

    void push_engine_euler3_d_p_y_r(lua_State *state, LegacyApi::Engine::Euler3DPYR const &euler) noexcept;
    void push_meta_engine_euler3_d_p_y_r(lua_State *state, LegacyApi::Engine::Euler3DPYR *euler) noexcept;
    std::optional<LegacyApi::Engine::Euler3DPYR> get_euler3_d_p_y_r(lua_State *state, int index) noexcept;

    void push_engine_vector2_d(lua_State *state, LegacyApi::Engine::Vector2D const &vector) noexcept;
    void push_meta_engine_vector2_d(lua_State *state, LegacyApi::Engine::Vector2D *vector) noexcept;
    std::optional<LegacyApi::Engine::Vector2D> get_vector2_d(lua_State *state, int index) noexcept;

    void push_engine_vector3_d(lua_State *state, LegacyApi::Engine::Vector3D const &vector) noexcept;
    void push_meta_engine_vector3_d(lua_State *state, LegacyApi::Engine::Vector3D *vector) noexcept;
    std::optional<LegacyApi::Engine::Vector3D> get_vector3_d(lua_State *state, int index) noexcept;

    void push_engine_quaternion(lua_State *state, LegacyApi::Engine::Quaternion const &quaternion) noexcept;
    void push_meta_engine_quaternion(lua_State *state, LegacyApi::Engine::Quaternion *quaternion) noexcept;
    std::optional<LegacyApi::Engine::Quaternion> get_quaternion(lua_State *state, int index) noexcept;

    void push_engine_plane3_d(lua_State *state, LegacyApi::Engine::Plane3D const &plane) noexcept;
    void push_meta_engine_plane3_d(lua_State *state, LegacyApi::Engine::Plane3D *plane) noexcept;
    std::optional<LegacyApi::Engine::Plane3D> get_plane3_d(lua_State *state, int index) noexcept;

    void push_engine_plane2_d(lua_State *state, LegacyApi::Engine::Plane2D const &plane) noexcept;
    void push_meta_engine_plane2_d(lua_State *state, LegacyApi::Engine::Plane2D *plane) noexcept;
    std::optional<LegacyApi::Engine::Plane2D> get_plane2_d(lua_State *state, int index) noexcept;

    void push_engine_matrix(lua_State *state, LegacyApi::Engine::Matrix const &matrix) noexcept;
    void push_meta_engine_matrix(lua_State *state, LegacyApi::Engine::Matrix *matrix) noexcept;
    std::optional<LegacyApi::Engine::Matrix> get_matrix(lua_State *state, int index) noexcept;

    void define_engine_types(lua_State *state) noexcept;
}

#endif 
