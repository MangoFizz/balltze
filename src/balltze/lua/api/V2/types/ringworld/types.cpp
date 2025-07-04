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
        LUAS_OBJECT(state, TableResourceHandle, &handle, readonly);
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
        LUAS_OBJECT(state, ColorARGB, &color, readonly);
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
        LUAS_OBJECT(state, ColorRGB, &color, readonly);
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
        LUAS_OBJECT(state, VectorXY, &vector, readonly);
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
        LUAS_OBJECT(state, VectorXYInt, &vector, readonly);
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
        LUAS_OBJECT(state, VectorXYZ, &vector, readonly);
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
        LUAS_OBJECT(state, Rectangle2D, &rectangle, readonly);
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
        LUAS_OBJECT(state, Bounds2D, &bounds, readonly);
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
        LUAS_OBJECT(state, VectorIJ, &vector, readonly);
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
        LUAS_OBJECT(state, VectorIJK, &vector, readonly);
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
    }
}
