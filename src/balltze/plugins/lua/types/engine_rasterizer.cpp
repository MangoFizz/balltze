// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/engine/rasterizer.hpp>
#include "../helpers/bitfield.hpp"
#include "../helpers/luacstruct.hpp"
#include "engine_rasterizer.hpp"

namespace Balltze::Plugins::Lua {
    static void define_engine_resolution_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineResolution);
        luacs_unsigned_field(state, EngineResolution, width, 0);
        luacs_unsigned_field(state, EngineResolution, height, 0);
        lua_pop(state, 1);
    }

    void push_meta_engine_resolution(lua_State *state, Engine::Resolution *data) noexcept {
        luacs_newobject(state, EngineResolution, data);
    }

    void push_engine_resolution(lua_State *state, Engine::Resolution const *data) noexcept {
        luacs_newobject(state, EngineResolution, NULL);
        auto *self = luacs_to_object(state, -1, EngineResolution);
        *self = *data;
    }

    static void define_camera_type_enum(lua_State *state) noexcept {
        luacs_newenum(state, EngineCameraType);
        luacs_enum_declare_value(state, "CAMERA_FIRST_PERSON", Engine::CAMERA_FIRST_PERSON);
        luacs_enum_declare_value(state, "CAMERA_VEHICLE", Engine::CAMERA_VEHICLE);
        luacs_enum_declare_value(state, "CAMERA_CINEMATIC", Engine::CAMERA_CINEMATIC);
        luacs_enum_declare_value(state, "CAMERA_DEBUG", Engine::CAMERA_DEBUG);
        lua_pop(state, 1);
    }

    static void define_camera_data_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineCameraData);
        luacs_nested_field(state, EngineCameraData, EnginePoint3D, position, 0);
        luacs_unsigned_array_field(state, EngineCameraData, unknown, 0);
        luacs_nested_array_field(state, EngineCameraData, EnginePoint3D, orientation, 0);
        luacs_float_field(state, EngineCameraData, fov, 0);
        lua_pop(state, 1); 
    }

    void push_meta_engine_camera_data(lua_State *state, Engine::CameraData *data) noexcept {
        luacs_newobject(state, EngineCameraData, data);
    }

    void define_engine_rasterizer_types(lua_State *state) noexcept {
        define_camera_type_enum(state);
        define_camera_data_struct(state);
    }
}
