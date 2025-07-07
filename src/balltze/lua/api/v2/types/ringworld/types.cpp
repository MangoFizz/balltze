// SPDX-License-Identifier: GPL-3.0-only

#include "../../../../libraries/luastruct.hpp"
#include "types.hpp"

namespace Balltze::Lua::Api::V2 {
    static void define_table_resource_handle_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, TableResourceHandle);
        LUAS_PRIMITIVE_FIELD(state, TableResourceHandle, value, LUAST_INT32, 0);
        LUAS_PRIMITIVE_FIELD(state, TableResourceHandle, id, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, TableResourceHandle, index, LUAST_INT16, 0);
        lua_pop(state, 1);
    }

    void push_table_resource_handle(lua_State *state, const TableResourceHandle &handle, bool readonly) noexcept {
        LUAS_NEW_OBJECT(state, TableResourceHandle, readonly);
        lua_pushinteger(state, handle.value);
        lua_setfield(state, -2, "value");
    }

    std::optional<TableResourceHandle> get_table_resource_handle(lua_State *state, int idx) noexcept {
        std::optional<TableResourceHandle> handle;
        if(lua_istable(state, idx) || lua_isuserdata(state, idx)) {
            lua_getfield(state, idx, "value");
            if(lua_isinteger(state, -1)) {
                handle = TableResourceHandle();
                handle->value = luaL_checkinteger(state, -1);
            }
            lua_pop(state, 1);
        }
        else if(lua_isinteger(state, idx)) {
            handle = TableResourceHandle();
            handle->value = luaL_checkinteger(state, idx);
        }
        return handle;
    }

    static void define_color_argb_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, ColorARGB);
        LUAS_PRIMITIVE_FIELD(state, ColorARGB, a, LUAST_INT8, 0);
        LUAS_PRIMITIVE_FIELD(state, ColorARGB, r, LUAST_INT8, 0);
        LUAS_PRIMITIVE_FIELD(state, ColorARGB, g, LUAST_INT8, 0);
        LUAS_PRIMITIVE_FIELD(state, ColorARGB, b, LUAST_INT8, 0);
        lua_pop(state, 1);
    }

    void push_color_argb(lua_State *state, const ColorARGB &color, bool readonly) noexcept {
        LUAS_PUSH_OBJECT(state, ColorARGB, &color, readonly);
    }

    std::optional<ColorARGB> get_color_argb(lua_State *state, int idx) noexcept {
        std::optional<ColorARGB> color;
        if(lua_istable(state, idx) || lua_isuserdata(state, idx)) {
            lua_getfield(state, idx, "a");
            if(lua_isinteger(state, -1)) {
                color = ColorARGB();
                color->a = luaL_checkinteger(state, -1);
            }
            lua_pop(state, 1);
            lua_getfield(state, idx, "r");
            if(lua_isinteger(state, -1)) {
                color = color.value_or(ColorARGB());
                color->r = luaL_checkinteger(state, -1);
            }
            lua_pop(state, 1);
            lua_getfield(state, idx, "g");
            if(lua_isinteger(state, -1)) {
                color = color.value_or(ColorARGB());
                color->g = luaL_checkinteger(state, -1);
            }
            lua_pop(state, 1);
            lua_getfield(state, idx, "b");
            if(lua_isinteger(state, -1)) {
                color = color.value_or(ColorARGB());
                color->b = luaL_checkinteger(state, -1);
            }
            lua_pop(state, 1);
        }
        return color;
    }

    static void define_color_rgb_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, ColorRGB);
        LUAS_PRIMITIVE_FIELD(state, ColorRGB, r, LUAST_INT8, 0);
        LUAS_PRIMITIVE_FIELD(state, ColorRGB, g, LUAST_INT8, 0);
        LUAS_PRIMITIVE_FIELD(state, ColorRGB, b, LUAST_INT8, 0);
        lua_pop(state, 1);
    }

    void push_color_rgb(lua_State *state, const ColorRGB &color, bool readonly) noexcept {
        LUAS_PUSH_OBJECT(state, ColorRGB, &color, readonly);
    }

    std::optional<ColorRGB> get_color_rgb(lua_State *state, int idx) noexcept {
        std::optional<ColorRGB> color;
        if(lua_istable(state, idx) || lua_isuserdata(state, idx)) {
            lua_getfield(state, idx, "r");
            if(lua_isinteger(state, -1)) {
                color = ColorRGB();
                color->r = luaL_checkinteger(state, -1);
            }
            lua_pop(state, 1);
            lua_getfield(state, idx, "g");
            if(lua_isinteger(state, -1)) {
                color = color.value_or(ColorRGB());
                color->g = luaL_checkinteger(state, -1);
            }
            lua_pop(state, 1);
            lua_getfield(state, idx, "b");
            if(lua_isinteger(state, -1)) {
                color = color.value_or(ColorRGB());
                color->b = luaL_checkinteger(state, -1);
            }
            lua_pop(state, 1);
        }
        return color;
    }

    static void define_vector_xy_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, VectorXY);
        LUAS_PRIMITIVE_FIELD(state, VectorXY, x, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, VectorXY, y, LUAST_FLOAT, 0);
        lua_pop(state, 1);
    }

    void push_vector_xy(lua_State *state, const VectorXY &vector, bool readonly) noexcept {
        LUAS_PUSH_OBJECT(state, VectorXY, &vector, readonly);
    }

    std::optional<VectorXY> get_vector_xy(lua_State *state, int idx) noexcept {
        std::optional<VectorXY> vector;
        if(lua_istable(state, idx) || lua_isuserdata(state, idx)) {
            lua_getfield(state, idx, "x");
            if(lua_isnumber(state, -1)) {
                vector = VectorXY();
                vector->x = static_cast<float>(luaL_checknumber(state, -1));
            }
            lua_pop(state, 1);
            lua_getfield(state, idx, "y");
            if(lua_isnumber(state, -1)) {
                vector = vector.value_or(VectorXY());
                vector->y = static_cast<float>(luaL_checknumber(state, -1));
            }
            lua_pop(state, 1);
        }
        return vector;
    }

    static void define_vector_xy_int_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, VectorXYInt);
        LUAS_PRIMITIVE_FIELD(state, VectorXYInt, x, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, VectorXYInt, y, LUAST_INT16, 0);
        lua_pop(state, 1);
    }

    void push_vector_xy_int(lua_State *state, const VectorXYInt &vector, bool readonly) noexcept {
        LUAS_PUSH_OBJECT(state, VectorXYInt, &vector, readonly);
    }

    std::optional<VectorXYInt> get_vector_xy_int(lua_State *state, int idx) noexcept {
        std::optional<VectorXYInt> vector;
        if(lua_istable(state, idx) || lua_isuserdata(state, idx)) {
            lua_getfield(state, idx, "x");
            if(lua_isinteger(state, -1)) {
                vector = VectorXYInt();
                vector->x = static_cast<int16_t>(luaL_checkinteger(state, -1));
            }
            lua_pop(state, 1);
            lua_getfield(state, idx, "y");
            if(lua_isinteger(state, -1)) {
                vector = vector.value_or(VectorXYInt());
                vector->y = static_cast<int16_t>(luaL_checkinteger(state, -1));
            }
            lua_pop(state, 1);
        }
        return vector;
    }

    static void define_vector_xyz_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, VectorXYZ);
        LUAS_PRIMITIVE_FIELD(state, VectorXYZ, x, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, VectorXYZ, y, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, VectorXYZ, z, LUAST_FLOAT, 0);
        lua_pop(state, 1);
    }

    void push_vector_xyz(lua_State *state, const VectorXYZ &vector, bool readonly) noexcept {
        LUAS_PUSH_OBJECT(state, VectorXYZ, &vector, readonly);
    }

    std::optional<VectorXYZ> get_vector_xyz(lua_State *state, int idx) noexcept {
        std::optional<VectorXYZ> vector;
        if(lua_istable(state, idx) || lua_isuserdata(state, idx)) {
            lua_getfield(state, idx, "x");
            if(lua_isnumber(state, -1)) {
                vector = VectorXYZ();
                vector->x = static_cast<float>(luaL_checknumber(state, -1));
            }
            lua_pop(state, 1);
            lua_getfield(state, idx, "y");
            if(lua_isnumber(state, -1)) {
                vector = vector.value_or(VectorXYZ());
                vector->y = static_cast<float>(luaL_checknumber(state, -1));
            }
            lua_pop(state, 1);
            lua_getfield(state, idx, "z");
            if(lua_isnumber(state, -1)) {
                vector = vector.value_or(VectorXYZ());
                vector->z = static_cast<float>(luaL_checknumber(state, -1));
            }
            lua_pop(state, 1);
        }
        return vector;
    }

    static void define_rectangle_2d_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, Rectangle2D);
        LUAS_PRIMITIVE_FIELD(state, Rectangle2D, left, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, Rectangle2D, top, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, Rectangle2D, right, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, Rectangle2D, bottom, LUAST_INT16, 0);
        lua_pop(state, 1);
    }

    void push_rectangle_2d(lua_State *state, const Rectangle2D &rectangle, bool readonly) noexcept {
        LUAS_PUSH_OBJECT(state, Rectangle2D, &rectangle, readonly);
    }

    std::optional<Rectangle2D> get_rectangle_2d(lua_State *state, int idx) noexcept {
        std::optional<Rectangle2D> rectangle;
        if(lua_istable(state, idx) || lua_isuserdata(state, idx)) {
            lua_getfield(state, idx, "left");
            if(lua_isinteger(state, -1)) {
                rectangle = Rectangle2D();
                rectangle->left = static_cast<int16_t>(luaL_checkinteger(state, -1));
            }
            lua_pop(state, 1);
            lua_getfield(state, idx, "top");
            if(lua_isinteger(state, -1)) {
                rectangle = rectangle.value_or(Rectangle2D());
                rectangle->top = static_cast<int16_t>(luaL_checkinteger(state, -1));
            }
            lua_pop(state, 1);
            lua_getfield(state, idx, "right");
            if(lua_isinteger(state, -1)) {
                rectangle = rectangle.value_or(Rectangle2D());
                rectangle->right = static_cast<int16_t>(luaL_checkinteger(state, -1));
            }
            lua_pop(state, 1);
            lua_getfield(state, idx, "bottom");
            if(lua_isinteger(state, -1)) {
                rectangle = rectangle.value_or(Rectangle2D());
                rectangle->bottom = static_cast<int16_t>(luaL_checkinteger(state, -1));
            }
            lua_pop(state, 1);
        }
        return rectangle;
    }

    static void define_bounds_2d_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, Bounds2D);
        LUAS_PRIMITIVE_FIELD(state, Bounds2D, left, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, Bounds2D, top, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, Bounds2D, right, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, Bounds2D, bottom, LUAST_FLOAT, 0);
        lua_pop(state, 1);
    }

    void push_bounds_2d(lua_State *state, const Bounds2D &bounds, bool readonly) noexcept {
        LUAS_PUSH_OBJECT(state, Bounds2D, &bounds, readonly);
    }

    std::optional<Bounds2D> get_bounds_2d(lua_State *state, int idx) noexcept {
        std::optional<Bounds2D> bounds;
        if(lua_istable(state, idx) || lua_isuserdata(state, idx)) {
            lua_getfield(state, idx, "left");
            if(lua_isnumber(state, -1)) {
                bounds = Bounds2D();
                bounds->left = static_cast<float>(luaL_checknumber(state, -1));
            }
            lua_pop(state, 1);
            lua_getfield(state, idx, "top");
            if(lua_isnumber(state, -1)) {
                bounds = bounds.value_or(Bounds2D());
                bounds->top = static_cast<float>(luaL_checknumber(state, -1));
            }
            lua_pop(state, 1);
            lua_getfield(state, idx, "right");
            if(lua_isnumber(state, -1)) {
                bounds = bounds.value_or(Bounds2D());
                bounds->right = static_cast<float>(luaL_checknumber(state, -1));
            }
            lua_pop(state, 1);
            lua_getfield(state, idx, "bottom");
            if(lua_isnumber(state, -1)) {
                bounds = bounds.value_or(Bounds2D());
                bounds->bottom = static_cast<float>(luaL_checknumber(state, -1));
            }
            lua_pop(state, 1);
        }
        return bounds;
    }

    static void define_vector_ij_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, VectorIJ);
        LUAS_PRIMITIVE_FIELD(state, VectorIJ, i, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, VectorIJ, j, LUAST_INT16, 0);
        lua_pop(state, 1);
    }

    void push_vector_ij(lua_State *state, const VectorIJ &vector, bool readonly) noexcept {
        LUAS_PUSH_OBJECT(state, VectorIJ, &vector, readonly);
    }

    std::optional<VectorIJ> get_vector_ij(lua_State *state, int idx) noexcept {
        std::optional<VectorIJ> vector;
        if(lua_istable(state, idx) || lua_isuserdata(state, idx)) {
            lua_getfield(state, idx, "i");
            if(lua_isinteger(state, -1)) {
                vector = VectorIJ();
                vector->i = static_cast<int16_t>(luaL_checkinteger(state, -1));
            }
            lua_pop(state, 1);
            lua_getfield(state, idx, "j");
            if(lua_isinteger(state, -1)) {
                vector = vector.value_or(VectorIJ());
                vector->j = static_cast<int16_t>(luaL_checkinteger(state, -1));
            }
            lua_pop(state, 1);
        }
        return vector;
    }

    static void define_vector_ijk_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, VectorIJK);
        LUAS_PRIMITIVE_FIELD(state, VectorIJK, i, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, VectorIJK, j, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, VectorIJK, k, LUAST_INT16, 0);
        lua_pop(state, 1);
    }

    void push_vector_ijk(lua_State *state, VectorIJK const &vector, bool readonly) noexcept {
        LUAS_PUSH_OBJECT(state, VectorIJK, &vector, readonly);
    }

    std::optional<VectorIJK> get_vector_ijk(lua_State *state, int idx) noexcept {
        std::optional<VectorIJK> vector;
        if(lua_istable(state, idx) || lua_isuserdata(state, idx)) {
            lua_getfield(state, idx, "i");
            if(lua_isinteger(state, -1)) {
                vector = VectorIJK();
                vector->i = static_cast<int16_t>(luaL_checkinteger(state, -1));
            }
            lua_pop(state, 1);
            lua_getfield(state, idx, "j");
            if(lua_isinteger(state, -1)) {
                vector = vector.value_or(VectorIJK());
                vector->j = static_cast<int16_t>(luaL_checkinteger(state, -1));
            }
            lua_pop(state, 1);
            lua_getfield(state, idx, "k");
            if(lua_isinteger(state, -1)) {
                vector = vector.value_or(VectorIJK());
                vector->k = static_cast<int16_t>(luaL_checkinteger(state, -1));
            }
            lua_pop(state, 1);
        }
        return vector;
    }

    static void define_quaternion_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, Quaternion);
        LUAS_PRIMITIVE_FIELD(state, Quaternion, i, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, Quaternion, j, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, Quaternion, k, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, Quaternion, l, LUAST_FLOAT, 0);
        lua_pop(state, 1);
    }

    void push_quaternion(lua_State *state, const Quaternion &quaternion, bool readonly) noexcept {
        LUAS_PUSH_OBJECT(state, Quaternion, &quaternion, readonly);
    }

    std::optional<Quaternion> get_quaternion(lua_State *state, int idx) noexcept {
        std::optional<Quaternion> quaternion;
        if(lua_istable(state, idx) || lua_isuserdata(state, idx)) {
            lua_getfield(state, idx, "i");
            if(lua_isnumber(state, -1)) {
                quaternion = Quaternion();
                quaternion->i = static_cast<float>(luaL_checknumber(state, -1));
            }
            lua_pop(state, 1);
            lua_getfield(state, idx, "j");
            if(lua_isnumber(state, -1)) {
                quaternion = quaternion.value_or(Quaternion());
                quaternion->j = static_cast<float>(luaL_checknumber(state, -1));
            }
            lua_pop(state, 1);
            lua_getfield(state, idx, "k");
            if(lua_isnumber(state, -1)) {
                quaternion = quaternion.value_or(Quaternion());
                quaternion->k = static_cast<float>(luaL_checknumber(state, -1));
            }
            lua_pop(state, 1);
            lua_getfield(state, idx, "l");
            if(lua_isnumber(state, -1)) {
                quaternion = quaternion.value_or(Quaternion());
                quaternion->l = static_cast<float>(luaL_checknumber(state, -1));
            }
            lua_pop(state, 1);
        }
        return quaternion;
    }

    static void define_vector_pyr_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, VectorPYR);
        LUAS_PRIMITIVE_FIELD(state, VectorPYR, pitch, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, VectorPYR, yaw, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, VectorPYR, rotation, LUAST_FLOAT, 0);
        lua_pop(state, 1);
    }

    void push_vector_pyr(lua_State *state, const VectorPYR &vector, bool readonly) noexcept {
        LUAS_PUSH_OBJECT(state, VectorPYR, &vector, readonly);
    }

    std::optional<VectorPYR> get_vector_pyr(lua_State *state, int idx) noexcept {
        std::optional<VectorPYR> vector;
        if(lua_istable(state, idx) || lua_isuserdata(state, idx)) {
            lua_getfield(state, idx, "pitch");
            if(lua_isnumber(state, -1)) {
                vector = VectorPYR();
                vector->pitch = static_cast<float>(luaL_checknumber(state, -1));
            }
            lua_pop(state, 1);
            lua_getfield(state, idx, "yaw");
            if(lua_isnumber(state, -1)) {
                vector = vector.value_or(VectorPYR());
                vector->yaw = static_cast<float>(luaL_checknumber(state, -1));
            }
            lua_pop(state, 1);
            lua_getfield(state, idx, "rotation");
            if(lua_isnumber(state, -1)) {
                vector = vector.value_or(VectorPYR());
                vector->rotation = static_cast<float>(luaL_checknumber(state, -1));
            }
            lua_pop(state, 1);
        }
        return vector;
    }

    static void define_vector_py_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, VectorPY);
        LUAS_PRIMITIVE_FIELD(state, VectorPY, pitch, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, VectorPY, yaw, LUAST_FLOAT, 0);
        lua_pop(state, 1);
    }

    void push_vector_py(lua_State *state, const VectorPY &vector, bool readonly) noexcept {
        LUAS_PUSH_OBJECT(state, VectorPY, &vector, readonly);
    }

    std::optional<VectorPY> get_vector_py(lua_State *state, int idx) noexcept {
        std::optional<VectorPY> vector;
        if(lua_istable(state, idx) || lua_isuserdata(state, idx)) {
            lua_getfield(state, idx, "pitch");
            if(lua_isnumber(state, -1)) {
                vector = VectorPY();
                vector->pitch = static_cast<float>(luaL_checknumber(state, -1));
            }
            lua_pop(state, 1);
            lua_getfield(state, idx, "yaw");
            if(lua_isnumber(state, -1)) {
                vector = vector.value_or(VectorPY());
                vector->yaw = static_cast<float>(luaL_checknumber(state, -1));
            }
            lua_pop(state, 1);
        }
        return vector;
    }

    static void define_plane_2d_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, Plane2D);
        LUAS_PRIMITIVE_FIELD(state, Plane2D, i, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, Plane2D, j, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, Plane2D, w, LUAST_FLOAT, 0);
        lua_pop(state, 1);
    }

    void push_plane_2d(lua_State *state, const Plane2D &plane, bool readonly) noexcept {
        LUAS_PUSH_OBJECT(state, Plane2D, &plane, readonly);
    }

    std::optional<Plane2D> get_plane_2d(lua_State *state, int idx) noexcept {
        std::optional<Plane2D> plane;
        if(lua_istable(state, idx) || lua_isuserdata(state, idx)) {
            lua_getfield(state, idx, "i");
            if(lua_isnumber(state, -1)) {
                plane = Plane2D();
                plane->i = static_cast<float>(luaL_checknumber(state, -1));
            }
            lua_pop(state, 1);
            lua_getfield(state, idx, "j");
            if(lua_isnumber(state, -1)) {
                plane = plane.value_or(Plane2D());
                plane->j = static_cast<float>(luaL_checknumber(state, -1));
            }
            lua_pop(state, 1);
            lua_getfield(state, idx, "w");
            if(lua_isnumber(state, -1)) {
                plane = plane.value_or(Plane2D());
                plane->w = static_cast<float>(luaL_checknumber(state, -1));
            }
            lua_pop(state, 1);
        }
        return plane;
    }

    static void define_plane_3d_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, Plane3D);
        LUAS_PRIMITIVE_FIELD(state, Plane3D, i, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, Plane3D, j, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, Plane3D, k, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, Plane3D, w, LUAST_FLOAT, 0);
        lua_pop(state, 1);
    }

    void push_plane_3d(lua_State *state, const Plane3D &plane, bool readonly) noexcept {
        LUAS_PUSH_OBJECT(state, Plane3D, &plane, readonly);
    }

    std::optional<Plane3D> get_plane_3d(lua_State *state, int idx) noexcept {
        std::optional<Plane3D> plane;
        if(lua_istable(state, idx) || lua_isuserdata(state, idx)) {
            lua_getfield(state, idx, "i");
            if(lua_isnumber(state, -1)) {
                plane = Plane3D();
                plane->i = static_cast<float>(luaL_checknumber(state, -1));
            }
            lua_pop(state, 1);
            lua_getfield(state, idx, "j");
            if(lua_isnumber(state, -1)) {
                plane = plane.value_or(Plane3D());
                plane->j = static_cast<float>(luaL_checknumber(state, -1));
            }
            lua_pop(state, 1);
            lua_getfield(state, idx, "k");
            if(lua_isnumber(state, -1)) {
                plane = plane.value_or(Plane3D());
                plane->k = static_cast<float>(luaL_checknumber(state, -1));
            }
            lua_pop(state, 1);
            lua_getfield(state, idx, "w");
            if(lua_isnumber(state, -1)) {
                plane = plane.value_or(Plane3D());
                plane->w = static_cast<float>(luaL_checknumber(state, -1));
            }
            lua_pop(state, 1);
        }
        return plane;
    }

    static void define_scenario_script_node_value_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, ScenarioScriptNodeValue);
        lua_pop(state, 1);
    }

    void push_scenario_script_node_value(lua_State *state, const ScenarioScriptNodeValue &value, bool readonly) noexcept {
        LUAS_PUSH_OBJECT(state, ScenarioScriptNodeValue, &value, readonly);
    }

    std::optional<ScenarioScriptNodeValue> get_scenario_script_node_value(lua_State *state, int idx) noexcept {
        std::optional<ScenarioScriptNodeValue> value;
        if(lua_istable(state, idx) || lua_isuserdata(state, idx)) {
            // The union is not directly accessible in Lua, so we cannot extract it.
            // This is a placeholder for future implementation if needed.
            value = ScenarioScriptNodeValue();
        }
        return value;
    }

    static void define_matrix_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, Matrix);
        LUAS_OBJREF_ARRAY_FIELD(state, Matrix, m, VectorXYZ, 0);
        lua_pop(state, 1);
    }

    void push_matrix(lua_State *state, const Matrix &matrix, bool readonly) noexcept {
        LUAS_PUSH_OBJECT(state, Matrix, &matrix, readonly);
    }

    std::optional<Matrix> get_matrix(lua_State *state, int idx) noexcept {
        std::optional<Matrix> matrix;
        if(lua_istable(state, idx) || lua_isuserdata(state, idx)) {
            lua_getfield(state, idx, "m");
            if(lua_istable(state, -1)) {
                matrix = Matrix();
                for(int i = 0; i < 3; ++i) {
                    lua_rawgeti(state, -1, i + 1);
                    if(lua_isuserdata(state, -1)) {
                        VectorXYZ vec = *static_cast<VectorXYZ *>(lua_touserdata(state, -1));
                        matrix->m[i] = vec;
                    }
                    lua_pop(state, 1);
                }
            }
            lua_pop(state, 1);
        }
        return matrix;
    }

    void define_ringworld_types(lua_State *state) noexcept {
        define_table_resource_handle_type(state);
        define_color_argb_type(state);
        define_color_rgb_type(state);
        define_vector_xy_type(state);
        define_vector_xy_int_type(state);
        define_vector_xyz_type(state);
        define_rectangle_2d_type(state);
        define_bounds_2d_type(state);
        define_vector_ij_type(state);
        define_vector_ijk_type(state);
        define_quaternion_type(state);
        define_vector_pyr_type(state);
        define_vector_py_type(state);
        define_plane_2d_type(state);
        define_plane_3d_type(state);
        define_scenario_script_node_value_type(state);
        define_matrix_type(state);
    }
}
