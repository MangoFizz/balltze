// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__PLUGINS__LUA__TYPES_HPP
#define BALLTZE__PLUGINS__LUA__TYPES_HPP

#include <chrono>
#include "types/engine_types.hpp"
#include "types/engine_tag.hpp"
#include "types/engine_tag_data.hpp"
#include "../../logger.hpp"

namespace Balltze::Plugins::Lua {
    inline void lua_define_engine_structs(lua_State *state) noexcept {
        lua_define_engine_resource_handle_struct(state);
        lua_define_engine_tag_class_enum(state);
        lua_define_engine_tag_base_struct(state);
        lua_define_engine_tag_dependency_struct(state);
        lua_define_engine_tag_data_offset_struct(state);
        lua_define_engine_color_a_r_g_b_int_struct(state);
        lua_define_engine_color_a_r_g_b_struct(state);
        lua_define_engine_color_r_g_b_struct(state);
        lua_define_engine_point2_d_struct(state);
        lua_define_engine_point2_d_int_struct(state);
        lua_define_engine_point3_d_struct(state);
        lua_define_engine_rectangle2_d_struct(state);
        lua_define_engine_euler2_d_struct(state);
        lua_define_engine_euler3_d_struct(state);
        lua_define_engine_vector2_d_struct(state);
        lua_define_engine_vector3_d_struct(state);
        lua_define_engine_quaternion_struct(state);
        lua_define_engine_plane3_d_struct(state);
        lua_define_engine_plane2_d_struct(state);
        lua_define_engine_matrix_struct(state);
        lua_define_engine_tag_data_structs(state);
    }
}

#endif
