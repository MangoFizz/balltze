// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__LUA__API__V2__TYPES__RINGWORLD__TYPES_HPP
#define BALLTZE__LUA__API__V2__TYPES__RINGWORLD__TYPES_HPP

#include <optional>
#include <lua.hpp>
#include <impl/types/types.h>

namespace Balltze::Lua::Api::V2 {
    /**
     * Push a TableResourceHandle to the Lua stack.
     * @param state Lua state
     * @param handle The TableResourceHandle to push
     * @param readonly If true, the handle will be read-only in Lua
     */
    void push_table_resource_handle(lua_State *state, const TableResourceHandle &handle, bool readonly = false) noexcept;

    /**
     * Get a TableResourceHandle from the Lua stack.
     * @param state Lua state
     * @param idx The index of the value on the Lua stack
     * @return An optional TableResourceHandle, empty if the value is not a valid handle
     */
    std::optional<TableResourceHandle> get_table_resource_handle(lua_State *state, int idx) noexcept;

    /**
     * Push a ColorARGB to the Lua stack.
     * @param state Lua state
     * @param color The ColorARGB to push
     * @param readonly If true, the color will be read-only in Lua
     */
    void push_color_argb(lua_State *state, const ColorARGB &color, bool readonly = false) noexcept;

    /**
     * Get a ColorARGB from the Lua stack.
     * @param state Lua state
     * @param idx The index of the value on the Lua stack
     * @return An optional ColorARGB, empty if the value is not a valid color
     */
    std::optional<ColorARGB> get_color_argb(lua_State *state, int idx) noexcept;

    /**
     * Push a ColorRGB to the Lua stack.
     * @param state Lua state
     * @param color The ColorRGB to push
     * @param readonly If true, the color will be read-only in Lua
     */
    void push_color_rgb(lua_State *state, const ColorRGB &color, bool readonly = false) noexcept;

    /**
     * Get a ColorRGB from the Lua stack.
     * @param state Lua state
     * @param idx The index of the value on the Lua stack
     * @return An optional ColorRGB, empty if the value is not a valid color
     */
    std::optional<ColorRGB> get_color_rgb(lua_State *state, int idx) noexcept;

    /**
     * Push a VectorXY to the Lua stack.
     * @param state Lua state
     * @param vector The VectorXY to push
     * @param readonly If true, the vector will be read-only in Lua
     */
    void push_vector_xy(lua_State *state, const VectorXY &vector, bool readonly = false) noexcept;

    /**
     * Get a VectorXY from the Lua stack.
     * @param state Lua state
     * @param idx The index of the value on the Lua stack
     * @return An optional VectorXY, empty if the value is not a valid vector
     */
    std::optional<VectorXY> get_vector_xy(lua_State *state, int idx) noexcept;

    /**
     * Push a VectorXYInt to the Lua stack.
     * @param state Lua state
     * @param vector The VectorXYInt to push
     * @param readonly If true, the vector will be read-only in Lua
     */
    void push_vector_xy_int(lua_State *state, const VectorXYInt &vector, bool readonly = false) noexcept;

    /**
     * Get a VectorXYInt from the Lua stack.
     * @param state Lua state
     * @param idx The index of the value on the Lua stack
     * @return An optional VectorXYInt, empty if the value is not a valid vector
     */
    std::optional<VectorXYInt> get_vector_xy_int(lua_State *state, int idx) noexcept;

    /**
     * Push a VectorXYZ to the Lua stack.
     * @param state Lua state
     * @param vector The VectorXYZ to push
     * @param readonly If true, the vector will be read-only in Lua
     */
    void push_vector_xyz(lua_State *state, const VectorXYZ &vector, bool readonly = false) noexcept;

    /**
     * Get a VectorXYZ from the Lua stack.
     * @param state Lua state
     * @param idx The index of the value on the Lua stack
     * @return An optional VectorXYZ, empty if the value is not a valid vector
     */
    std::optional<VectorXYZ> get_vector_xyz(lua_State *state, int idx) noexcept;

    /**
     * Push a Rectangle2D to the Lua stack.
     * @param state Lua state
     * @param rectangle The Rectangle2D to push
     * @param readonly If true, the rectangle will be read-only in Lua
     */
    void push_rectangle_2d(lua_State *state, const Rectangle2D &rectangle, bool readonly = false) noexcept;

    /**
     * Get a Rectangle2D from the Lua stack.
     * @param state Lua state
     * @param idx The index of the value on the Lua stack
     * @return An optional Rectangle2D, empty if the value is not a valid rectangle
     */
    std::optional<Rectangle2D> get_rectangle_2d(lua_State *state, int idx) noexcept;

    /**
     * Push a Bounds2D to the Lua stack.
     * @param state Lua state
     * @param bounds The Bounds2D to push
     * @param readonly If true, the bounds will be read-only in Lua
     */
    void push_bounds_2d(lua_State *state, const Bounds2D &rectangle, bool readonly = false) noexcept;

    /**
     * Get a Bounds2D from the Lua stack.
     * @param state Lua state
     * @param idx The index of the value on the Lua stack
     * @return An optional Bounds2D, empty if the value is not a valid bounds
     */
    std::optional<Bounds2D> get_bounds_2d(lua_State *state, int idx) noexcept;

    /**
     * Push a VectorIJ to the Lua stack.
     * @param state Lua state
     * @param vector The VectorIJ to push
     * @param readonly If true, the vector will be read-only in Lua
     */
    void push_vector_ij(lua_State *state, const VectorIJ &vector, bool readonly = false) noexcept;

    /**
     * Get a VectorIJ from the Lua stack.
     * @param state Lua state
     * @param idx The index of the value on the Lua stack
     * @return An optional VectorIJ, empty if the value is not a valid vector
     */
    std::optional<VectorIJ> get_vector_ij(lua_State *state, int idx) noexcept;

    /**
     * Push a VectorIJK to the Lua stack.
     * @param state Lua state
     * @param vector The VectorIJK to push
     * @param readonly If true, the vector will be read-only in Lua
     */
    void push_vector_ijk(lua_State *state, VectorIJK const &vector, bool readonly = false) noexcept;

    /**
     * Get a VectorIJK from the Lua stack.
     * @param state Lua state
     * @param idx The index of the value on the Lua stack
     * @return An optional VectorIJK, empty if the value is not a valid vector
     */
    std::optional<VectorIJK> get_vector_ijk(lua_State *state, int idx) noexcept;

    /**
     * Define all ringworld basic types in the Lua state.
     * @param state Lua state
     * @note This function should be called to register all types from the functions of this header.
     */
    void define_ringworld_types(lua_State *state) noexcept;
}

#endif 
