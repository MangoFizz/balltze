// SPDX-License-Identifier: GPL-3.0-only

#include <luacstruct/luacstruct.h>
#include "../helpers/table.hpp"
#include "engine_types.hpp"

namespace Balltze::Plugins::Lua {
    using namespace Engine;

    void lua_define_engine_resource_handle_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineResourceHandle);
        luacs_unsigned_field(state, EngineResourceHandle, handle, NULL);
        luacs_unsigned_field(state, EngineResourceHandle, id, NULL);
        luacs_unsigned_field(state, EngineResourceHandle, index, NULL);
        lua_pop(state, 1);
    }

    void lua_push_engine_resource_handle(lua_State *state, Engine::ResourceHandle *handle) noexcept {
        luacs_newobject(state, EngineResourceHandle, handle);
    }

    void lua_define_engine_color_a_r_g_b_int_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineColorARGBInt);
        luacs_unsigned_field(state, EngineColorARGBInt, alpha, NULL);
        luacs_unsigned_field(state, EngineColorARGBInt, red, NULL);
        luacs_unsigned_field(state, EngineColorARGBInt, green, NULL);
        luacs_unsigned_field(state, EngineColorARGBInt, blue, NULL);
        lua_pop(state, 1);
    }

    void lua_push_engine_color_a_r_g_b_int(lua_State *state, Engine::ColorARGBInt *color) noexcept {
        luacs_newobject(state, EngineColorARGBInt, color);
    }

    void lua_define_engine_color_a_r_g_b_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineColorARGB);
        luacs_float_field(state, EngineColorARGB, alpha, NULL);
        luacs_float_field(state, EngineColorARGB, red, NULL);
        luacs_float_field(state, EngineColorARGB, green, NULL);
        luacs_float_field(state, EngineColorARGB, blue, NULL);
        lua_pop(state, 1);
    }

    void lua_push_engine_color_a_r_g_b(lua_State *state, Engine::ColorARGB *color) noexcept {
        luacs_newobject(state, EngineColorARGB, color);
    }

    void lua_define_engine_point2_d_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EnginePoint2D);
        luacs_float_field(state, EnginePoint2D, x, NULL);
        luacs_float_field(state, EnginePoint2D, y, NULL);
        lua_pop(state, 1);
    }

    void lua_push_engine_point2_d(lua_State *state, Engine::Point2D *point) noexcept {
        luacs_newobject(state, EnginePoint2D, point);
    }

    void lua_define_engine_point2_d_int_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EnginePoint2DInt);
        luacs_int_field(state, EnginePoint2DInt, x, NULL);
        luacs_int_field(state, EnginePoint2DInt, y, NULL);
        lua_pop(state, 1);
    }

    void lua_push_engine_point2_d_int(lua_State *state, Engine::Point2DInt *point) noexcept {
        luacs_newobject(state, EnginePoint2DInt, point);
    }

    void lua_define_engine_point3_d_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EnginePoint3D);
        luacs_float_field(state, EnginePoint3D, x, NULL);
        luacs_float_field(state, EnginePoint3D, y, NULL);
        luacs_float_field(state, EnginePoint3D, z, NULL);
        lua_pop(state, 1);
    }

    void lua_push_engine_point3_d(lua_State *state, Engine::Point3D *point) noexcept {
        luacs_newobject(state, EnginePoint3D, point);
    }

    void lua_define_engine_rectangle2_d_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineRectangle2D);
        luacs_int_field(state, EngineRectangle2D, left, NULL);
        luacs_int_field(state, EngineRectangle2D, top, NULL);
        luacs_int_field(state, EngineRectangle2D, right, NULL);
        luacs_int_field(state, EngineRectangle2D, bottom, NULL);
        lua_pop(state, 1);
    }

    void lua_push_engine_rectangle2_d(lua_State *state, Engine::Rectangle2D *rectangle) noexcept {
        luacs_newobject(state, EngineRectangle2D, rectangle);
    }

    void lua_define_engine_euler2_d_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineEuler2D);
        luacs_float_field(state, EngineEuler2D, yaw, NULL);
        luacs_float_field(state, EngineEuler2D, pitch, NULL);
        lua_pop(state, 1);
    }

    void lua_push_engine_euler2_d(lua_State *state, Engine::Euler2D *euler) noexcept {
        luacs_newobject(state, EngineEuler2D, euler);
    }

    void lua_define_engine_euler3_d_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineEuler3D);
        luacs_float_field(state, EngineEuler3D, yaw, NULL);
        luacs_float_field(state, EngineEuler3D, pitch, NULL);
        luacs_float_field(state, EngineEuler3D, roll, NULL);
        lua_pop(state, 1);
    }

    void lua_push_engine_euler3_d(lua_State *state, Engine::Euler3D *euler) noexcept {
        luacs_newobject(state, EngineEuler3D, euler);
    }

    void lua_define_engine_vector2_d_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineVector2D);
        luacs_float_field(state, EngineVector2D, i, NULL);
        luacs_float_field(state, EngineVector2D, j, NULL);
        lua_pop(state, 1);
    }

    void lua_push_engine_vector2_d(lua_State *state, Engine::Vector2D *vector) noexcept {
        luacs_newobject(state, EngineVector2D, vector);
    }

    void lua_define_engine_vector3_d_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineVector3D);
        luacs_float_field(state, EngineVector3D, i, NULL);
        luacs_float_field(state, EngineVector3D, j, NULL);
        luacs_float_field(state, EngineVector3D, k, NULL);
        lua_pop(state, 1);
    }

    void lua_push_engine_vector3_d(lua_State *state, Engine::Vector3D *vector) noexcept {
        luacs_newobject(state, EngineVector3D, vector);
    }

    void lua_define_engine_quaternion_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineQuaternion);
        luacs_float_field(state, EngineQuaternion, i, NULL);
        luacs_float_field(state, EngineQuaternion, j, NULL);
        luacs_float_field(state, EngineQuaternion, k, NULL);
        luacs_float_field(state, EngineQuaternion, w, NULL);
        lua_pop(state, 1);
    }

    void lua_push_engine_quaternion(lua_State *state, Engine::Quaternion *quaternion) noexcept {
        luacs_newobject(state, EngineQuaternion, quaternion);
    }

    void lua_define_engine_plane3_d_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EnginePlane3D);
        luacs_nested_field(state, EnginePlane3D, EngineVector3D, vector, NULL);
        luacs_float_field(state, EnginePlane3D, w, NULL);
        lua_pop(state, 1);
    }

    void lua_push_engine_plane3_d(lua_State *state, Engine::Plane3D *plane) noexcept {
        luacs_newobject(state, EnginePlane3D, plane);
    }

    void lua_define_engine_plane2_d_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EnginePlane2D);
        luacs_nested_field(state, EnginePlane2D, EngineVector2D, vector, NULL);
        luacs_float_field(state, EnginePlane2D, w, NULL);
        lua_pop(state, 1);
    }

    void lua_push_engine_plane2_d(lua_State *state, Engine::Plane2D *plane) noexcept {
        luacs_newobject(state, EnginePlane2D, plane);
    }

    void lua_define_engine_matrix_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineMatrix);
        luacs_nested_array_field(state, EngineMatrix, EngineVector3D, m, NULL);
        lua_pop(state, 1);
    }
}
