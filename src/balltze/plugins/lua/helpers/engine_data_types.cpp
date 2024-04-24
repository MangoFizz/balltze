// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/engine/data_types.hpp>
#include <balltze/engine/tag_definitions.hpp>
#include "engine_data_types.hpp"

namespace Balltze::Plugins {
    Engine::ColorARGB lua_to_color_argb(lua_State *state, int index) {
        Engine::ColorARGB color;
        if(lua_istable(state, index)) {
            auto get_field = [&state, &index](const char *field, float &value) {
                lua_getfield(state, index, field);
                if(lua_isnumber(state, -1)) {
                    value = lua_tonumber(state, -1);
                }
                else {
                    auto *error = "Expected number for color field.";
                    luaL_error(state, error);
                    throw std::runtime_error(error);
                }
                lua_pop(state, 1);
            };

            try {
                get_field("red", color.red);
                get_field("green", color.green);
                get_field("blue", color.blue);
                get_field("alpha", color.alpha);
            }
            catch(std::runtime_error &e) {
                throw;
            }
        }
        else {
            if(lua_gettop(state) >= index + 3) {
                auto get_field = [&state, &index](float &value) {
                    if(lua_isnumber(state, index)) {
                        value = lua_tonumber(state, index);
                        index++;
                    }
                    else {
                        auto *error = "Expected number for color field.";
                        luaL_error(state, error);
                        throw std::runtime_error(error);
                    }
                };

                try {
                    get_field(color.red);
                    get_field(color.green);
                    get_field(color.blue);
                    get_field(color.alpha);
                }
                catch(std::runtime_error &e) {
                    throw;
                }
            }
            else {
                throw std::runtime_error("Could not get color from table or stack.");
            }
        }
        return color;
    }

    void lua_push_engine_matrix(lua_State *state, Engine::Matrix &matrix) noexcept {
        lua_newtable(state);
        for(std::size_t i = 0; i < 3; i++) {
            lua_newtable(state);
            for(std::size_t j = 0; j < 3; j++) {
                lua_pushnumber(state, matrix[i][j]);
                lua_rawseti(state, -2, j + 1);
            }
            lua_rawseti(state, -2, i + 1);
        }
    }

    std::shared_ptr<Engine::Matrix> lua_to_engine_matrix(lua_State *state, int index) noexcept {
        auto matrix_ptr = new Engine::Matrix();
        std::shared_ptr<Engine::Matrix> matrix(matrix_ptr);
        for(std::size_t i = 0; i < 3; i++) {
            lua_rawgeti(state, index, i + 1);
            for(std::size_t j = 0; j < 3; j++) {
                lua_rawgeti(state, -1, j + 1);
                matrix[i][j] = luaL_checknumber(state, -1);
                lua_pop(state, 1);
            }
            lua_pop(state, 1);
        }
        return matrix;
    }

    void lua_push_engine_resolution(lua_State *state, Engine::Resolution &resolution) noexcept {
        lua_newtable(state);
        lua_pushinteger(state, resolution.width);
        lua_setfield(state, -2, "width");
        lua_pushinteger(state, resolution.height);
        lua_setfield(state, -2, "height");
    }

    Engine::Point2DInt lua_to_point2_d_int(lua_State *state, int index) {
        Engine::Point2DInt point;
        if(lua_istable(state, index)) {
            auto get_field = [&state, &index](const char *field, std::int16_t &value) {
                lua_getfield(state, index, field);
                if(lua_isinteger(state, -1)) {
                    value = lua_tointeger(state, -1);
                }
                else {
                    auto *error = "Expected integer for Point2DInt field.";
                    throw std::runtime_error(error);
                }
                lua_pop(state, 1);
            };

            try {
                get_field("x", point.x);
                get_field("y", point.y);
            }
            catch(std::runtime_error &e) {
                throw;
            }
        }
        else {
            if(lua_gettop(state) >= index + 1) {
                auto get_field = [&state, &index](std::int16_t &value) {
                    if(lua_isinteger(state, index)) {
                        value = lua_tointeger(state, index);
                        index++;
                    }
                    else {
                        auto *error = "Expected integer for Point2DInt field.";
                        luaL_error(state, error);
                        throw std::runtime_error(error);
                    }
                };

                try {
                    get_field(point.x);
                    get_field(point.y);
                }
                catch(std::runtime_error &e) {
                    throw;
                }
            }
            else {
                throw std::runtime_error("Could not get Point2DInt from table or stack.");
            }
        }
        return point;
    }

    Engine::Point3D lua_to_point3_d(lua_State *state, int index) {
        Engine::Point3D point;
        if(lua_istable(state, index)) {
            auto get_field = [&state, &index](const char *field, float &value) {
                lua_getfield(state, index, field);
                if(lua_isnumber(state, -1)) {
                    value = lua_tonumber(state, -1);
                }
                else {
                    auto *error = "Expected number for Point3D field.";
                    throw std::runtime_error(error);
                }
                lua_pop(state, 1);
            };

            try {
                get_field("x", point.x);
                get_field("y", point.y);
                get_field("z", point.z);
            }
            catch(std::runtime_error &e) {
                throw;
            }
        }
        else {
            if(lua_gettop(state) >= index + 1) {
                auto get_field = [&state, &index](float &value) {
                    if(lua_isnumber(state, index)) {
                        value = lua_tonumber(state, index);
                        index++;
                    }
                    else {
                        auto *error = "Expected number for Point3D field.";
                        luaL_error(state, error);
                        throw std::runtime_error(error);
                    }
                };

                try {
                    get_field(point.x);
                    get_field(point.y);
                    get_field(point.z);
                }
                catch(std::runtime_error &e) {
                    throw;
                }
            }
            else {
                throw std::runtime_error("Could not get Point3D from table or stack.");
            }
        }
        return point;
    }

    Engine::ColorARGBInt lua_to_color_a_r_g_b_int(lua_State *state, int index) {
        Engine::ColorARGBInt color;
        if(lua_istable(state, index)) {
            auto get_field = [&state, &index](const char *field, std::uint8_t &value) {
                lua_getfield(state, index, field);
                if(lua_isinteger(state, -1)) {
                    value = lua_tointeger(state, -1);
                }
                else {
                    auto *error = "Expected integer for ColorARGBInt field.";
                    throw std::runtime_error(error);
                }
                lua_pop(state, 1);
            };

            try {
                get_field("red", color.red);
                get_field("green", color.green);
                get_field("blue", color.blue);
                get_field("alpha", color.alpha);
            }
            catch(std::runtime_error &e) {
                throw;
            }
        }
        else {
            if(lua_gettop(state) >= index + 1) {
                auto get_field = [&state, &index](std::uint8_t &value) {
                    if(lua_isinteger(state, index)) {
                        value = lua_tointeger(state, index);
                        index++;
                    }
                    else {
                        auto *error = "Expected integer for ColorARGBInt field.";
                        throw std::runtime_error(error);
                    }
                };

                try {
                    get_field(color.red);
                    get_field(color.green);
                    get_field(color.blue);
                    get_field(color.alpha);
                }
                catch(std::runtime_error &e) {
                    throw;
                }
            }
            else {
                throw std::runtime_error("Could not get ColorARGBInt from table or stack.");
            }
        }
        return color;
    }

    void lua_push_engine_resource_handle(lua_State *state, const Engine::ResourceHandle &handle) noexcept {
        lua_newtable(state);

        lua_pushinteger(state, handle.handle);
        lua_setfield(state, -2, "handle");

        lua_pushinteger(state, handle.id);
        lua_setfield(state, -2, "id");

        lua_pushinteger(state, handle.index);
        lua_setfield(state, -2, "index");
    }

    void lua_push_engine_resource_handle(lua_State *state, Engine::ResourceHandle *handle) noexcept {
        lua_push_engine_resource_handle(state, *handle);
    }

    void lua_push_engine_object_handle(lua_State *state, Engine::ObjectHandle *handle) noexcept {
        lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(handle));
    }

    void lua_push_engine_tag_handle(lua_State *state, Engine::TagHandle *handle) noexcept {
        lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(handle));
    }

    void lua_push_engine_player_handle(lua_State *state, Engine::PlayerHandle *handle) noexcept {
        lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(handle));
    }
}
