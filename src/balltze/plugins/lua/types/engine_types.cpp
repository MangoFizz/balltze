// SPDX-License-Identifier: GPL-3.0-only

#include "../helpers/luacstruct.hpp"
#include "engine_types.hpp"

namespace Balltze::Plugins::Lua {
    using namespace Engine;

    void lua_define_engine_resource_handle_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineResourceHandle);
        luacs_unsigned_field(state, EngineResourceHandle, handle, 0);
        luacs_unsigned_field(state, EngineResourceHandle, id, 0);
        luacs_unsigned_field(state, EngineResourceHandle, index, 0);
        lua_pop(state, 1);
    }

    void lua_push_engine_resource_handle(lua_State *state, Engine::ResourceHandle *handle) noexcept {
        luacs_newobject(state, EngineResourceHandle, handle);
    }

    void lua_define_engine_color_a_r_g_b_int_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineColorARGBInt);
        luacs_unsigned_field(state, EngineColorARGBInt, alpha, 0);
        luacs_unsigned_field(state, EngineColorARGBInt, red, 0);
        luacs_unsigned_field(state, EngineColorARGBInt, green, 0);
        luacs_unsigned_field(state, EngineColorARGBInt, blue, 0);
        lua_pop(state, 1);
    }

    void lua_push_engine_color_a_r_g_b_int(lua_State *state, Engine::ColorARGBInt *color) noexcept {
        luacs_newobject(state, EngineColorARGBInt, color);
    }

    void lua_define_engine_color_a_r_g_b_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineColorARGB);
        luacs_float_field(state, EngineColorARGB, alpha, 0);
        luacs_float_field(state, EngineColorARGB, red, 0);
        luacs_float_field(state, EngineColorARGB, green, 0);
        luacs_float_field(state, EngineColorARGB, blue, 0);
        lua_pop(state, 1);
    }

    void lua_push_engine_color_a_r_g_b(lua_State *state, Engine::ColorARGB *color) noexcept {
        luacs_newobject(state, EngineColorARGB, color);
    }

    void lua_define_engine_color_r_g_b_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineColorRGB);
        luacs_float_field(state, EngineColorRGB, red, 0);
        luacs_float_field(state, EngineColorRGB, green, 0);
        luacs_float_field(state, EngineColorRGB, blue, 0);
        lua_pop(state, 1);
    }

    void lua_push_engine_color_r_g_b(lua_State *state, Engine::ColorRGB *color) noexcept {
        luacs_newobject(state, EngineColorRGB, color);
    }

    void lua_define_engine_point2_d_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EnginePoint2D);
        luacs_float_field(state, EnginePoint2D, x, 0);
        luacs_float_field(state, EnginePoint2D, y, 0);
        lua_pop(state, 1);
    }

    void lua_push_engine_point2_d(lua_State *state, Engine::Point2D *point) noexcept {
        luacs_newobject(state, EnginePoint2D, point);
    }

    void lua_define_engine_point2_d_int_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EnginePoint2DInt);
        luacs_int_field(state, EnginePoint2DInt, x, 0);
        luacs_int_field(state, EnginePoint2DInt, y, 0);
        lua_pop(state, 1);
    }

    void lua_push_engine_point2_d_int(lua_State *state, Engine::Point2DInt *point) noexcept {
        luacs_newobject(state, EnginePoint2DInt, point);
    }

    void lua_define_engine_point3_d_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EnginePoint3D);
        luacs_float_field(state, EnginePoint3D, x, 0);
        luacs_float_field(state, EnginePoint3D, y, 0);
        luacs_float_field(state, EnginePoint3D, z, 0);
        lua_pop(state, 1);
    }

    void lua_push_engine_point3_d(lua_State *state, Engine::Point3D *point) noexcept {
        luacs_newobject(state, EnginePoint3D, point);
    }

    void lua_define_engine_rectangle2_d_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineRectangle2D);
        luacs_int_field(state, EngineRectangle2D, left, 0);
        luacs_int_field(state, EngineRectangle2D, top, 0);
        luacs_int_field(state, EngineRectangle2D, right, 0);
        luacs_int_field(state, EngineRectangle2D, bottom, 0);
        lua_pop(state, 1);
    }

    void lua_push_engine_rectangle2_d(lua_State *state, Engine::Rectangle2D *rectangle) noexcept {
        luacs_newobject(state, EngineRectangle2D, rectangle);
    }

    void lua_define_engine_euler2_d_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineEuler2D);
        luacs_float_field(state, EngineEuler2D, yaw, 0);
        luacs_float_field(state, EngineEuler2D, pitch, 0);
        lua_pop(state, 1);
    }

    void lua_push_engine_euler2_d(lua_State *state, Engine::Euler2D *euler) noexcept {
        luacs_newobject(state, EngineEuler2D, euler);
    }

    void lua_define_engine_euler3_d_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineEuler3D);
        luacs_float_field(state, EngineEuler3D, yaw, 0);
        luacs_float_field(state, EngineEuler3D, pitch, 0);
        luacs_float_field(state, EngineEuler3D, roll, 0);
        lua_pop(state, 1);
    }

    void lua_push_engine_euler3_d(lua_State *state, Engine::Euler3D *euler) noexcept {
        luacs_newobject(state, EngineEuler3D, euler);
    }

    void lua_define_engine_vector2_d_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineVector2D);
        luacs_float_field(state, EngineVector2D, i, 0);
        luacs_float_field(state, EngineVector2D, j, 0);
        lua_pop(state, 1);
    }

    void lua_push_engine_vector2_d(lua_State *state, Engine::Vector2D *vector) noexcept {
        luacs_newobject(state, EngineVector2D, vector);
    }

    void lua_define_engine_vector3_d_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineVector3D);
        luacs_float_field(state, EngineVector3D, i, 0);
        luacs_float_field(state, EngineVector3D, j, 0);
        luacs_float_field(state, EngineVector3D, k, 0);
        lua_pop(state, 1);
    }

    void lua_push_engine_vector3_d(lua_State *state, Engine::Vector3D *vector) noexcept {
        luacs_newobject(state, EngineVector3D, vector);
    }

    void lua_define_engine_quaternion_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineQuaternion);
        luacs_float_field(state, EngineQuaternion, i, 0);
        luacs_float_field(state, EngineQuaternion, j, 0);
        luacs_float_field(state, EngineQuaternion, k, 0);
        luacs_float_field(state, EngineQuaternion, w, 0);
        lua_pop(state, 1);
    }

    void lua_push_engine_quaternion(lua_State *state, Engine::Quaternion *quaternion) noexcept {
        luacs_newobject(state, EngineQuaternion, quaternion);
    }

    void lua_define_engine_plane3_d_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EnginePlane3D);
        luacs_nested_field(state, EnginePlane3D, EngineVector3D, vector, 0);
        luacs_float_field(state, EnginePlane3D, w, 0);
        lua_pop(state, 1);
    }

    void lua_push_engine_plane3_d(lua_State *state, Engine::Plane3D *plane) noexcept {
        luacs_newobject(state, EnginePlane3D, plane);
    }

    void lua_define_engine_plane2_d_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EnginePlane2D);
        luacs_nested_field(state, EnginePlane2D, EngineVector2D, vector, 0);
        luacs_float_field(state, EnginePlane2D, w, 0);
        lua_pop(state, 1);
    }

    void lua_push_engine_plane2_d(lua_State *state, Engine::Plane2D *plane) noexcept {
        luacs_newobject(state, EnginePlane2D, plane);
    }

    void lua_define_engine_matrix_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineMatrix);
        luacs_nested_array_field(state, EngineMatrix, EngineVector3D, m, 0);
        lua_pop(state, 1);
    }

    void lua_push_engine_matrix(lua_State *state, EngineMatrix *matrix) noexcept {
        luacs_newobject(state, EngineMatrix, matrix);
    }
}
