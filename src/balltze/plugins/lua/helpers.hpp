// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__PLUGINS__LUA__HELPERS_HPP
#define BALLTZE__PLUGINS__LUA__HELPERS_HPP

#include <functional>
#include <lua.hpp>
#include <balltze/plugin.hpp>
#include <balltze/engine.hpp>

namespace Balltze::Plugins {
    void lua_create_functions_table(lua_State *state, const char *name, const luaL_Reg *functions) noexcept;
    VersionNumber lua_check_version_number(lua_State* state, int index);
    
    Engine::ColorARGB lua_to_color_argb(lua_State *state, int index);
    
    std::string engine_edition_to_string(Engine::EngineEdition edition);
    Engine::EngineEdition engine_edition_from_string(const std::string &edition);
    
    std::string cache_file_engine_to_string(Engine::CacheFileEngine engine);
    Engine::CacheFileEngine cache_file_engine_from_string(const std::string &engine);
    
    std::string map_game_type_to_string(Engine::MapGameType type);
    Engine::MapGameType map_game_type_from_string(const std::string &type);
    
    std::string server_type_to_string(Engine::ServerType type);
    Engine::ServerType server_type_from_string(const std::string &type);

    std::string server_game_type_to_string(Engine::Gametype gametype);
    Engine::Gametype server_game_type_from_string(const std::string &gametype);

    void lua_push_engine_matrix(lua_State *state, Engine::Matrix &matrix) noexcept;
    std::shared_ptr<Engine::Matrix> lua_to_engine_matrix(lua_State *state, int index) noexcept;

    void lua_push_meta_engine_color_a_r_g_b_int(lua_State *state, Engine::ColorARGBInt &color) noexcept;
    void lua_push_meta_engine_tag_dependency(lua_State *state, Engine::TagDependency &dependency) noexcept;
    void lua_push_meta_engine_point2_d(lua_State *state, Engine::Point2D &point) noexcept;
    void lua_push_meta_engine_point3_d(lua_State *state, Engine::Point3D &point) noexcept;
    void lua_push_meta_engine_tag_data_offset(lua_State *state, Engine::TagDataOffset &offset) noexcept;
    void lua_push_meta_engine_color_a_r_g_b(lua_State *state, Engine::ColorARGB &color) noexcept;
    void lua_push_meta_engine_rectangle2_d(lua_State *state, Engine::Rectangle2D &rectangle) noexcept;
    void lua_push_meta_engine_point2_d_int(lua_State *state, Engine::Point2DInt &point) noexcept;
    void lua_push_meta_engine_euler2_d(lua_State *state, Engine::Euler2D &euler) noexcept;
    void lua_push_meta_engine_euler3_d(lua_State *state, Engine::Euler3D &euler) noexcept;
    void lua_push_meta_engine_vector2_d(lua_State *state, Engine::Vector2D &vector) noexcept;
    void lua_push_meta_engine_vector3_d(lua_State *state, Engine::Vector3D &vector) noexcept;
    void lua_push_meta_engine_color_r_g_b(lua_State *state, Engine::ColorRGB &color) noexcept;
    void lua_push_meta_engine_quaternion(lua_State *state, Engine::Quaternion &quaternion) noexcept;
    void lua_push_meta_engine_plane3_d(lua_State *state, Engine::Plane3D &plane) noexcept;
    void lua_push_meta_engine_plane2_d(lua_State *state, Engine::Plane2D &plane) noexcept;

    template<typename T>
    void lua_push_meta_object(lua_State *state, T &elem, lua_CFunction index, lua_CFunction newindex) noexcept {
        // Create table for vector
        lua_newtable(state);
        lua_pushlightuserdata(state, reinterpret_cast<void *>(const_cast<T *>(&elem)));
        lua_setfield(state, -2, "_data");

        // Create metatable for vector 3D
        lua_newtable(state);
        lua_pushcfunction(state, index);
        lua_setfield(state, -2, "__index");
        lua_pushcfunction(state, newindex);
        lua_setfield(state, -2, "__newindex");

        // Set metatable for vector
        lua_setmetatable(state, -2);
    }

    template<typename T>
    int lua_engine_tag_reflexive__index(lua_State *state, std::function<void (lua_State *, T &)> push_function) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto *elem = static_cast<Engine::TagReflexive<T> *>(lua_touserdata(state, -1));
        lua_pop(state, 1); 
        auto key = luaL_checkinteger(state, 2);
        if (key < 0 || key >= elem->count) {
            return luaL_error(state, "Index out of bounds");
        }
        push_function(state, elem->offset[key]);
    }
}   

#endif