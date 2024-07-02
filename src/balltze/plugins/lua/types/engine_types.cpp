// SPDX-License-Identifier: GPL-3.0-only

#include "../../../logger.hpp"
#include "../helpers/luacstruct.hpp"
#include "engine_types.hpp"

namespace Balltze::Plugins::Lua {
    using namespace Engine;

    static void define_engine_resource_handle_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineResourceHandle);
        luacs_unsigned_field(state, EngineResourceHandle, value, 0);
        luacs_unsigned_field(state, EngineResourceHandle, id, 0);
        luacs_unsigned_field(state, EngineResourceHandle, index, 0);
        lua_pop(state, 1);
    }

    void push_meta_engine_resource_handle(lua_State *state, Engine::ResourceHandle *handle) noexcept {
        luacs_newobject(state, EngineResourceHandle, handle);
    }

    void push_engine_resource_handle(lua_State *state, Engine::ResourceHandle const &handle) noexcept {
        luacs_newobject(state, EngineResourceHandle, NULL);
        auto *self = luacs_to_object(state, -1, EngineResourceHandle);
        *self = handle;
    }

    void push_meta_engine_object_handle(lua_State *state, Engine::ObjectHandle *handle) noexcept {
        luacs_newobject(state, EngineResourceHandle, handle);
    }

    void push_engine_object_handle(lua_State *state, Engine::ObjectHandle const &handle) noexcept {
        luacs_newobject(state, EngineResourceHandle, NULL);
        auto *self = luacs_to_object(state, -1, EngineResourceHandle);
        *self = handle;
    }

    void push_meta_engine_tag_handle(lua_State *state, Engine::TagHandle *handle) noexcept {
        luacs_newobject(state, EngineResourceHandle, handle);
    }

    void push_engine_tag_handle(lua_State *state, Engine::TagHandle const &handle) noexcept {
        luacs_newobject(state, EngineResourceHandle, NULL);
        auto *self = luacs_to_object(state, -1, EngineResourceHandle);
        *self = handle;
    }

    void push_meta_engine_player_handle(lua_State *state, Engine::PlayerHandle *handle) noexcept {
        luacs_newobject(state, EngineResourceHandle, handle);
    }

    void push_engine_player_handle(lua_State *state, Engine::PlayerHandle const &handle) noexcept {
        luacs_newobject(state, EngineResourceHandle, NULL);
        auto *self = luacs_to_object(state, -1, EngineResourceHandle);
        *self = handle;
    }

    std::optional<Engine::ResourceHandle> get_engine_resource_handle(lua_State *state, int index) noexcept {
        std::optional<Engine::ResourceHandle> handle;
        if(lua_istable(state, index) || lua_isuserdata(state, index)) {
            lua_getfield(state, index, "value");
            if(lua_isinteger(state, -1)) {
                handle = Engine::ResourceHandle();
                handle->value = luaL_checkinteger(state, -1);
            }
            lua_pop(state, 1);
        }
        else if(lua_isinteger(state, index)) {
            handle = Engine::ResourceHandle();
            handle->value = luaL_checkinteger(state, index);
        }
        return handle;
    }

    static void define_engine_color_a_r_g_b_int_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineColorARGBInt);
        luacs_unsigned_field(state, EngineColorARGBInt, alpha, 0);
        luacs_unsigned_field(state, EngineColorARGBInt, red, 0);
        luacs_unsigned_field(state, EngineColorARGBInt, green, 0);
        luacs_unsigned_field(state, EngineColorARGBInt, blue, 0);
        lua_pop(state, 1);
    }

    void push_meta_engine_color_a_r_g_b_int(lua_State *state, Engine::ColorARGBInt *color) noexcept {
        luacs_newobject(state, EngineColorARGBInt, color);
    }

    void push_engine_color_a_r_g_b_int(lua_State *state, Engine::ColorARGBInt const &color) noexcept {
        luacs_newobject(state, EngineColorARGBInt, NULL);
        auto *self = luacs_to_object(state, -1, EngineColorARGBInt);
        *self = color;
    }

    std::optional<Engine::ColorARGBInt> get_color_a_r_g_b_int(lua_State *state, int index) noexcept {
        if(lua_istable(state, index)) {
            Engine::ColorARGBInt color;
            
            auto get_field = [state](std::uint8_t &field, const char *name) -> bool {
                lua_getfield(state, -1, name);
                if(lua_isinteger(state, -1)) {
                    field = luaL_checkinteger(state, -1);
                    lua_pop(state, 1);
                    return true;
                }
                lua_pop(state, 1);
                return false;
            };
            
            bool success = true;
            success &= get_field(color.alpha, "alpha");
            success &= get_field(color.red, "red");
            success &= get_field(color.green, "green");
            success &= get_field(color.blue, "blue");
            if(success) {
                return color;
            }
        }
        return std::nullopt;
    }

    static void define_engine_color_a_r_g_b_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineColorARGB);
        luacs_float_field(state, EngineColorARGB, alpha, 0);
        luacs_float_field(state, EngineColorARGB, red, 0);
        luacs_float_field(state, EngineColorARGB, green, 0);
        luacs_float_field(state, EngineColorARGB, blue, 0);
        lua_pop(state, 1);
    }

    void push_meta_engine_color_a_r_g_b(lua_State *state, Engine::ColorARGB *color) noexcept {
        luacs_newobject(state, EngineColorARGB, color);
    }

    void push_engine_color_a_r_g_b(lua_State *state, Engine::ColorARGB const &color) noexcept {
        luacs_newobject(state, EngineColorARGB, NULL);
        auto *self = luacs_to_object(state, -1, EngineColorARGB);
        *self = color;
    }

    std::optional<Engine::ColorARGB> get_color_a_r_g_b(lua_State *state, int index) noexcept {
        if(lua_istable(state, index) || lua_isuserdata(state, index)) {
            lua_pushvalue(state, index);

            Engine::ColorARGB color;
            
            auto get_field = [state](float &field, const char *name) -> bool {
                lua_getfield(state, -1, name);
                if(lua_isnumber(state, -1)) {
                    field = luaL_checknumber(state, -1);
                    lua_pop(state, 1);
                    return true;
                }
                lua_pop(state, 1);
                return false;
            };
            
            bool success = true;
            success &= get_field(color.alpha, "alpha");
            success &= get_field(color.red, "red");
            success &= get_field(color.green, "green");
            success &= get_field(color.blue, "blue");

            lua_pop(state, 1);
            
            if(success) {
                return color;
            }
        }
        return std::nullopt;
    }

    static void define_engine_color_r_g_b_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineColorRGB);
        luacs_float_field(state, EngineColorRGB, red, 0);
        luacs_float_field(state, EngineColorRGB, green, 0);
        luacs_float_field(state, EngineColorRGB, blue, 0);
        lua_pop(state, 1);
    }

    void push_meta_engine_color_r_g_b(lua_State *state, Engine::ColorRGB *color) noexcept {
        luacs_newobject(state, EngineColorRGB, color);
    }

    void push_engine_color_r_g_b(lua_State *state, Engine::ColorRGB const &color) noexcept {
        luacs_newobject(state, EngineColorRGB, NULL);
        auto *self = luacs_to_object(state, -1, EngineColorRGB);
        *self = color;
    }

    std::optional<Engine::ColorRGB> get_color_r_g_b(lua_State *state, int index) noexcept {
        if(lua_istable(state, index)) {
            Engine::ColorRGB color;
            
            auto get_field = [state](float &field, const char *name) -> bool {
                lua_getfield(state, -1, name);
                if(lua_isnumber(state, -1)) {
                    field = luaL_checknumber(state, -1);
                    lua_pop(state, 1);
                    return true;
                }
                lua_pop(state, 1);
                return false;
            };
            
            bool success = true;
            success &= get_field(color.red, "red");
            success &= get_field(color.green, "green");
            success &= get_field(color.blue, "blue");
            if(success) {
                return color;
            }
        }
        return std::nullopt;
    }

    static void define_engine_point2_d_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EnginePoint2D);
        luacs_float_field(state, EnginePoint2D, x, 0);
        luacs_float_field(state, EnginePoint2D, y, 0);
        lua_pop(state, 1);
    }

    void push_meta_engine_point2_d(lua_State *state, Engine::Point2D *point) noexcept {
        luacs_newobject(state, EnginePoint2D, point);
    }

    void push_engine_point2_d(lua_State *state, Engine::Point2D const &point) noexcept {
        luacs_newobject(state, EnginePoint2D, NULL);
        auto *self = luacs_to_object(state, -1, EnginePoint2D);
        *self = point;
    }

    std::optional<Engine::Point2D> get_point2_d(lua_State *state, int index) noexcept {
        if(lua_istable(state, index)) {
            Engine::Point2D point;
            
            auto get_field = [state](float &field, const char *name) -> bool {
                lua_getfield(state, -1, name);
                if(lua_isnumber(state, -1)) {
                    field = luaL_checknumber(state, -1);
                    lua_pop(state, 1);
                    return true;
                }
                lua_pop(state, 1);
                return false;
            };
            
            bool success = true;
            success &= get_field(point.x, "x");
            success &= get_field(point.y, "y");
            if(success) {
                return point;
            }
        }
        return std::nullopt;
    }

    static void define_engine_point2_d_int_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EnginePoint2DInt);
        luacs_int_field(state, EnginePoint2DInt, x, 0);
        luacs_int_field(state, EnginePoint2DInt, y, 0);
        lua_pop(state, 1);
    }

    void push_meta_engine_point2_d_int(lua_State *state, Engine::Point2DInt *point) noexcept {
        luacs_newobject(state, EnginePoint2DInt, point);
    }

    void push_engine_point2_d_int(lua_State *state, Engine::Point2DInt const &point) noexcept {
        luacs_newobject(state, EnginePoint2DInt, NULL);
        auto *self = luacs_to_object(state, -1, EnginePoint2DInt);
        *self = point;
    }

    std::optional<Engine::Point2DInt> get_point2_d_int(lua_State *state, int index) noexcept {
        if(lua_istable(state, index)) {
            Engine::Point2DInt point;
            
            auto get_field = [state](std::int16_t &field, const char *name) -> bool {
                lua_getfield(state, -1, name);
                if(lua_isinteger(state, -1)) {
                    field = luaL_checkinteger(state, -1);
                    lua_pop(state, 1);
                    return true;
                }
                lua_pop(state, 1);
                return false;
            };
            
            bool success = true;
            success &= get_field(point.x, "x");
            success &= get_field(point.y, "y");
            if(success) {
                return point;
            }
        }
        return std::nullopt;
    }

    static void define_engine_point3_d_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EnginePoint3D);
        luacs_float_field(state, EnginePoint3D, x, 0);
        luacs_float_field(state, EnginePoint3D, y, 0);
        luacs_float_field(state, EnginePoint3D, z, 0);
        lua_pop(state, 1);
    }

    void push_meta_engine_point3_d(lua_State *state, Engine::Point3D *point) noexcept {
        luacs_newobject(state, EnginePoint3D, point);
    }

    void push_engine_point3_d(lua_State *state, Engine::Point3D const &point) noexcept {
        luacs_newobject(state, EnginePoint3D, NULL);
        auto *self = luacs_to_object(state, -1, EnginePoint3D);
        *self = point;
    }

    std::optional<Engine::Point3D> get_point3_d(lua_State *state, int index) noexcept {
        if(lua_istable(state, index)) {
            Engine::Point3D point;
            
            auto get_field = [state](float &field, const char *name) -> bool {
                lua_getfield(state, -1, name);
                if(lua_isnumber(state, -1)) {
                    field = luaL_checknumber(state, -1);
                    lua_pop(state, 1);
                    return true;
                }
                lua_pop(state, 1);
                return false;
            };
            
            bool success = true;
            success &= get_field(point.x, "x");
            success &= get_field(point.y, "y");
            success &= get_field(point.z, "z");
            if(success) {
                return point;
            }
        }
        return std::nullopt;
    }

    static void define_engine_rectangle2_d_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineRectangle2D);
        luacs_int_field(state, EngineRectangle2D, left, 0);
        luacs_int_field(state, EngineRectangle2D, top, 0);
        luacs_int_field(state, EngineRectangle2D, right, 0);
        luacs_int_field(state, EngineRectangle2D, bottom, 0);
        lua_pop(state, 1);
    }

    void push_meta_engine_rectangle2_d(lua_State *state, Engine::Rectangle2D *rectangle) noexcept {
        luacs_newobject(state, EngineRectangle2D, rectangle);
    }

    void push_engine_rectangle2_d(lua_State *state, Engine::Rectangle2D const &rectangle) noexcept {
        luacs_newobject(state, EngineRectangle2D, NULL);
        auto *self = luacs_to_object(state, -1, EngineRectangle2D);
        *self = rectangle;
    }

    std::optional<Engine::Rectangle2D> get_rectangle2_d(lua_State *state, int index) noexcept {
        if(lua_istable(state, index)) {
            Engine::Rectangle2D rectangle;
            
            auto get_field = [state](std::int16_t &field, const char *name) -> bool {
                lua_getfield(state, -1, name);
                if(lua_isinteger(state, -1)) {
                    field = luaL_checkinteger(state, -1);
                    lua_pop(state, 1);
                    return true;
                }
                lua_pop(state, 1);
                return false;
            };
            
            bool success = true;
            success &= get_field(rectangle.left, "left");
            success &= get_field(rectangle.top, "top");
            success &= get_field(rectangle.right, "right");
            success &= get_field(rectangle.bottom, "bottom");
            if(success) {
                return rectangle;
            }
        }
        return std::nullopt;
    }

    static void define_engine_rectangle2_d_f_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineRectangle2DF);
        luacs_float_field(state, EngineRectangle2DF, left, 0);
        luacs_float_field(state, EngineRectangle2DF, top, 0);
        luacs_float_field(state, EngineRectangle2DF, right, 0);
        luacs_float_field(state, EngineRectangle2DF, bottom, 0);
        lua_pop(state, 1);
    }

    void push_meta_engine_rectangle2_d_f(lua_State *state, Engine::Rectangle2DF *rectangle) noexcept {
        luacs_newobject(state, EngineRectangle2DF, rectangle);
    }

    void push_engine_rectangle2_d_f(lua_State *state, Engine::Rectangle2DF const &rectangle) noexcept {
        luacs_newobject(state, EngineRectangle2DF, NULL);
        auto *self = luacs_to_object(state, -1, EngineRectangle2DF);
        *self = rectangle;
    }

    std::optional<Engine::Rectangle2DF> get_rectangle2_d_f(lua_State *state, int index) noexcept {
        if(lua_istable(state, index)) {
            Engine::Rectangle2DF rectangle;
            
            auto get_field = [state](float &field, const char *name) -> bool {
                lua_getfield(state, -1, name);
                if(lua_isnumber(state, -1)) {
                    field = luaL_checknumber(state, -1);
                    lua_pop(state, 1);
                    return true;
                }
                lua_pop(state, 1);
                return false;
            };
            
            bool success = true;
            success &= get_field(rectangle.left, "left");
            success &= get_field(rectangle.top, "top");
            success &= get_field(rectangle.right, "right");
            success &= get_field(rectangle.bottom, "bottom");
            if(success) {
                return rectangle;
            }
        }
        return std::nullopt;
    }

    static void define_engine_euler2_d_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineEuler2D);
        luacs_float_field(state, EngineEuler2D, yaw, 0);
        luacs_float_field(state, EngineEuler2D, pitch, 0);
        lua_pop(state, 1);
    }

    void push_meta_engine_euler2_d(lua_State *state, Engine::Euler2D *euler) noexcept {
        luacs_newobject(state, EngineEuler2D, euler);
    }
    
    void push_engine_euler2_d(lua_State *state, Engine::Euler2D const &euler) noexcept {
        luacs_newobject(state, EngineEuler2D, NULL);
        auto *self = luacs_to_object(state, -1, EngineEuler2D);
        *self = euler;
    }

    std::optional<Engine::Euler2D> get_euler2_d(lua_State *state, int index) noexcept {
        if(lua_istable(state, index)) {
            Engine::Euler2D euler;
            
            auto get_field = [state](float &field, const char *name) -> bool {
                lua_getfield(state, -1, name);
                if(lua_isnumber(state, -1)) {
                    field = luaL_checknumber(state, -1);
                    lua_pop(state, 1);
                    return true;
                }
                lua_pop(state, 1);
                return false;
            };
            
            bool success = true;
            success &= get_field(euler.yaw, "yaw");
            success &= get_field(euler.pitch, "pitch");
            if(success) {
                return euler;
            }
        }
        return std::nullopt;
    }

    static void define_engine_euler3_d_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineEuler3D);
        luacs_float_field(state, EngineEuler3D, yaw, 0);
        luacs_float_field(state, EngineEuler3D, pitch, 0);
        luacs_float_field(state, EngineEuler3D, roll, 0);
        lua_pop(state, 1);
    }

    void push_meta_engine_euler3_d(lua_State *state, Engine::Euler3D *euler) noexcept {
        luacs_newobject(state, EngineEuler3D, euler);
    }

    void push_engine_euler3_d(lua_State *state, Engine::Euler3D const &euler) noexcept {
        luacs_newobject(state, EngineEuler3D, NULL);
        auto *self = luacs_to_object(state, -1, EngineEuler3D);
        *self = euler;
    }

    std::optional<Engine::Euler3D> get_euler3_d(lua_State *state, int index) noexcept {
        if(lua_istable(state, index)) {
            Engine::Euler3D euler;
            
            auto get_field = [state](float &field, const char *name) -> bool {
                lua_getfield(state, -1, name);
                if(lua_isnumber(state, -1)) {
                    field = luaL_checknumber(state, -1);
                    lua_pop(state, 1);
                    return true;
                }
                lua_pop(state, 1);
                return false;
            };
            
            bool success = true;
            success &= get_field(euler.yaw, "yaw");
            success &= get_field(euler.pitch, "pitch");
            success &= get_field(euler.roll, "roll");
            if(success) {
                return euler;
            }
        }
        return std::nullopt;
    }

    static void define_engine_euler3_d_p_y_r_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineEuler3DPYR);
        luacs_float_field(state, EngineEuler3DPYR, pitch, 0);
        luacs_float_field(state, EngineEuler3DPYR, yaw, 0);
        luacs_float_field(state, EngineEuler3DPYR, roll, 0);
        lua_pop(state, 1);
    }

    void push_meta_engine_euler3_d_p_y_r(lua_State *state, Engine::Euler3DPYR *euler) noexcept {
        luacs_newobject(state, EngineEuler3DPYR, euler);
    }

    void push_engine_euler3_d_p_y_r(lua_State *state, Engine::Euler3DPYR const &euler) noexcept {
        luacs_newobject(state, EngineEuler3DPYR, NULL);
        auto *self = luacs_to_object(state, -1, EngineEuler3DPYR);
        *self = euler;
    }

    std::optional<Engine::Euler3DPYR> get_euler3_d_p_y_r(lua_State *state, int index) noexcept {
        if(lua_istable(state, index)) {
            Engine::Euler3DPYR euler;
            
            auto get_field = [state](float &field, const char *name) -> bool {
                lua_getfield(state, -1, name);
                if(lua_isnumber(state, -1)) {
                    field = luaL_checknumber(state, -1);
                    lua_pop(state, 1);
                    return true;
                }
                lua_pop(state, 1);
                return false;
            };
            
            bool success = true;
            success &= get_field(euler.pitch, "pitch");
            success &= get_field(euler.yaw, "yaw");
            success &= get_field(euler.roll, "roll");
            if(success) {
                return euler;
            }
        }
        return std::nullopt;
    }

    static void define_engine_vector2_d_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineVector2D);
        luacs_float_field(state, EngineVector2D, i, 0);
        luacs_float_field(state, EngineVector2D, j, 0);
        lua_pop(state, 1);
    }

    void push_meta_engine_vector2_d(lua_State *state, Engine::Vector2D *vector) noexcept {
        luacs_newobject(state, EngineVector2D, vector);
    }

    void push_engine_vector2_d(lua_State *state, Engine::Vector2D const &vector) noexcept {
        luacs_newobject(state, EngineVector2D, NULL);
        auto *self = luacs_to_object(state, -1, EngineVector2D);
        *self = vector;
    }

    std::optional<Engine::Vector2D> get_vector2_d(lua_State *state, int index) noexcept {
        if(lua_istable(state, index)) {
            Engine::Vector2D vector;
            
            auto get_field = [state](float &field, const char *name) -> bool {
                lua_getfield(state, -1, name);
                if(lua_isnumber(state, -1)) {
                    field = luaL_checknumber(state, -1);
                    lua_pop(state, 1);
                    return true;
                }
                lua_pop(state, 1);
                return false;
            };
            
            bool success = true;
            success &= get_field(vector.i, "i");
            success &= get_field(vector.j, "j");
            if(success) {
                return vector;
            }
        }
        return std::nullopt;
    }

    static void define_engine_vector3_d_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineVector3D);
        luacs_float_field(state, EngineVector3D, i, 0);
        luacs_float_field(state, EngineVector3D, j, 0);
        luacs_float_field(state, EngineVector3D, k, 0);
        lua_pop(state, 1);
    }

    void push_meta_engine_vector3_d(lua_State *state, Engine::Vector3D *vector) noexcept {
        luacs_newobject(state, EngineVector3D, vector);
    }

    void push_engine_vector3_d(lua_State *state, Engine::Vector3D const &vector) noexcept {
        luacs_newobject(state, EngineVector3D, NULL);
        auto *self = luacs_to_object(state, -1, EngineVector3D);
        *self = vector;
    }

    std::optional<Engine::Vector3D> get_vector3_d(lua_State *state, int index) noexcept {
        if(lua_istable(state, index)) {
            Engine::Vector3D vector;
            
            auto get_field = [state](float &field, const char *name) -> bool {
                lua_getfield(state, -1, name);
                if(lua_isnumber(state, -1)) {
                    field = luaL_checknumber(state, -1);
                    lua_pop(state, 1);
                    return true;
                }
                lua_pop(state, 1);
                return false;
            };
            
            bool success = true;
            success &= get_field(vector.i, "i");
            success &= get_field(vector.j, "j");
            success &= get_field(vector.k, "k");
            if(success) {
                return vector;
            }
        }
        return std::nullopt;
    }

    static void define_engine_quaternion_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineQuaternion);
        luacs_float_field(state, EngineQuaternion, i, 0);
        luacs_float_field(state, EngineQuaternion, j, 0);
        luacs_float_field(state, EngineQuaternion, k, 0);
        luacs_float_field(state, EngineQuaternion, w, 0);
        lua_pop(state, 1);
    }

    void push_meta_engine_quaternion(lua_State *state, Engine::Quaternion *quaternion) noexcept {
        luacs_newobject(state, EngineQuaternion, quaternion);
    }

    void push_engine_quaternion(lua_State *state, Engine::Quaternion const &quaternion) noexcept {
        luacs_newobject(state, EngineQuaternion, NULL);
        auto *self = luacs_to_object(state, -1, EngineQuaternion);
        *self = quaternion;
    }

    std::optional<Engine::Quaternion> get_quaternion(lua_State *state, int index) noexcept {
        if(lua_istable(state, index)) {
            Engine::Quaternion quaternion;
            
            auto get_field = [state](float &field, const char *name) -> bool {
                lua_getfield(state, -1, name);
                if(lua_isnumber(state, -1)) {
                    field = luaL_checknumber(state, -1);
                    lua_pop(state, 1);
                    return true;
                }
                lua_pop(state, 1);
                return false;
            };
            
            bool success = true;
            success &= get_field(quaternion.i, "i");
            success &= get_field(quaternion.j, "j");
            success &= get_field(quaternion.k, "k");
            success &= get_field(quaternion.w, "w");
            if(success) {
                return quaternion;
            }
        }
        return std::nullopt;
    }

    static void define_engine_plane3_d_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EnginePlane3D);
        luacs_nested_field(state, EnginePlane3D, EngineVector3D, vector, 0);
        luacs_float_field(state, EnginePlane3D, w, 0);
        lua_pop(state, 1);
    }

    void push_meta_engine_plane3_d(lua_State *state, Engine::Plane3D *plane) noexcept {
        luacs_newobject(state, EnginePlane3D, plane);
    }

    void push_engine_plane3_d(lua_State *state, Engine::Plane3D const &plane) noexcept {
        luacs_newobject(state, EnginePlane3D, NULL);
        auto *self = luacs_to_object(state, -1, EnginePlane3D);
        *self = plane;
    }

    std::optional<Engine::Plane3D> get_plane3_d(lua_State *state, int index) noexcept {
        if(lua_istable(state, index)) {
            Engine::Plane3D plane;
            
            auto get_field = [state](float &field, const char *name) -> bool {
                lua_getfield(state, -1, name);
                if(lua_isnumber(state, -1)) {
                    field = luaL_checknumber(state, -1);
                    lua_pop(state, 1);
                    return true;
                }
                lua_pop(state, 1);
                return false;
            };
            
            bool success = true;
            
            lua_getfield(state, index, "vector");
            auto vector = get_vector3_d(state, -1);
            if(vector.has_value()) {
                plane.vector = vector.value();
            }
            success &= vector.has_value();
            lua_pop(state, 1);

            success &= get_field(plane.w, "w");
            if(success) {
                return plane;
            }
        }
        return std::nullopt;
    }

    static void define_engine_plane2_d_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EnginePlane2D);
        luacs_nested_field(state, EnginePlane2D, EngineVector2D, vector, 0);
        luacs_float_field(state, EnginePlane2D, w, 0);
        lua_pop(state, 1);
    }

    void push_meta_engine_plane2_d(lua_State *state, Engine::Plane2D *plane) noexcept {
        luacs_newobject(state, EnginePlane2D, plane);
    }

    void push_engine_plane2_d(lua_State *state, Engine::Plane2D const &plane) noexcept {
        luacs_newobject(state, EnginePlane2D, NULL);
        auto *self = luacs_to_object(state, -1, EnginePlane2D);
        *self = plane;
    }

    std::optional<Engine::Plane2D> get_plane2_d(lua_State *state, int index) noexcept {
        if(lua_istable(state, index)) {
            Engine::Plane2D plane;
            
            auto get_field = [state](float &field, const char *name) -> bool {
                lua_getfield(state, -1, name);
                if(lua_isnumber(state, -1)) {
                    field = luaL_checknumber(state, -1);
                    lua_pop(state, 1);
                    return true;
                }
                lua_pop(state, 1);
                return false;
            };
            
            bool success = true;
            
            lua_getfield(state, index, "vector");
            auto vector = get_vector2_d(state, -1);
            if(vector.has_value()) {
                plane.vector = vector.value();
            }
            success &= vector.has_value();
            lua_pop(state, 1);

            success &= get_field(plane.w, "w");
            if(success) {
                return plane;
            }
        }
        return std::nullopt;
    }

    static void define_engine_matrix_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineMatrix);
        luacs_nested_array_field(state, EngineMatrix, EngineVector3D, m, 0);
        lua_pop(state, 1);
    }

    void push_meta_engine_matrix(lua_State *state, EngineMatrix *matrix) noexcept {
        luacs_newobject(state, EngineMatrix, matrix);
    }

    void push_engine_matrix(lua_State *state, EngineMatrix const &matrix) noexcept {
        luacs_newobject(state, EngineMatrix, NULL);
        auto *self = luacs_to_object(state, -1, EngineMatrix);
        *self = matrix;
    }

    std::optional<EngineMatrix> get_matrix(lua_State *state, int index) noexcept {
        if(lua_istable(state, index)) {
            EngineMatrix matrix;
            
            auto get_field = [state](std::size_t index, Engine::Vector3D &field) -> bool {
                lua_geti(state, -1, index);
                auto vector = get_vector3_d(state, -1);
                if(vector.has_value()) {
                    field = vector.value();
                }
                lua_pop(state, 1);
                return vector.has_value();
            };
            
            bool success = true;
            for(std::size_t i = 0; i < 3; ++i) {
                success &= get_field(i + 1, matrix.m[i]);
            }
            if(success) {
                return matrix;
            }
        }
        return std::nullopt;
    }

    void define_engine_types(lua_State *state) noexcept {
        define_engine_resource_handle_struct(state);
        define_engine_color_a_r_g_b_int_struct(state);
        define_engine_color_a_r_g_b_struct(state);
        define_engine_color_r_g_b_struct(state);
        define_engine_point2_d_struct(state);
        define_engine_point2_d_int_struct(state);
        define_engine_point3_d_struct(state);
        define_engine_rectangle2_d_struct(state);
        define_engine_rectangle2_d_f_struct(state);
        define_engine_euler2_d_struct(state);
        define_engine_euler3_d_struct(state);
        define_engine_euler3_d_p_y_r_struct(state);
        define_engine_vector2_d_struct(state);
        define_engine_vector3_d_struct(state);
        define_engine_quaternion_struct(state);
        define_engine_plane3_d_struct(state);
        define_engine_plane2_d_struct(state);
        define_engine_matrix_struct(state);
    }
}
