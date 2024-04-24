// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/engine.hpp>
#include <lua.hpp>
#include "../helpers.hpp"
#include "../metatables.hpp"

namespace Balltze::Plugins {
    static int lua_engine_camera_data__index(lua_State *state) noexcept {
        auto *camera = lua_from_meta_object<Engine::CameraData>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "position") { 
            lua_push_meta_engine_point3_d(state, camera->position);
            return 1;
        }
        else if(field == "orientation") {
            lua_newtable(state);
            lua_push_meta_engine_point3_d(state, camera->orientation[0]);
            lua_rawseti(state, -2, 1);
            lua_push_meta_engine_point3_d(state, camera->orientation[1]);
            lua_rawseti(state, -2, 2);
            return 1;
        }
        else if(field == "fov") {
            lua_pushnumber(state, camera->fov);
            return 1;
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
    }

    static int lua_engine_camera_data__newindex(lua_State *state) noexcept {
        auto *camera = lua_from_meta_object<Engine::CameraData>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "position") { 
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "orientation") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "fov") {
            camera->fov = luaL_checknumber(state, 3);
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_camera_data(lua_State *state, Engine::CameraData &camera, bool read_only) noexcept {
        lua_push_meta_object(state, &camera, lua_engine_camera_data__index, lua_engine_camera_data__newindex, read_only); 
    }
    
    static int lua_event_widget_render_vertex__index(lua_State *state) noexcept {
        auto *vertex = lua_from_meta_object<Event::UIWidgetRenderVertices::Vertex>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {
            return luaL_error(state, "Invalid key type");
        }
        
        std::string field = key;
        if(field == "x") {
            lua_pushnumber(state, vertex->x);
        }
        else if(field == "y") {
            lua_pushnumber(state, vertex->y);
        }
        else if(field == "z") {
            lua_pushnumber(state, vertex->z);
        }
        else if(field == "rhw") {
            lua_pushnumber(state, vertex->rhw);
        }
        else if(field == "u") {
            lua_pushnumber(state, vertex->u);
        }
        else if(field == "v") {
            lua_pushnumber(state, vertex->v);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_event_widget_render_vertex__newindex(lua_State *state) noexcept {
        auto *vertex = lua_from_meta_object<Event::UIWidgetRenderVertices::Vertex>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {
            return luaL_error(state, "Invalid key type");
        }
        
        std::string field = key;
        if(field == "x") {
            vertex->x = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "y") {
            vertex->y = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "z") {
            vertex->z = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "rhw") {
            vertex->rhw = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "u") {
            vertex->u = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "v") {
            vertex->v = static_cast<float>(luaL_checknumber(state, 3));
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_event_widget_render_vertex(lua_State *state, Event::UIWidgetRenderVertices::Vertex &vertex, bool read_only) noexcept {
        lua_push_meta_object(state, &vertex, lua_event_widget_render_vertex__index, lua_event_widget_render_vertex__newindex, read_only);
    }
    
    static int lua_event_widget_render_vertices__index(lua_State *state) noexcept {
        auto *vertices = lua_from_meta_object<Event::UIWidgetRenderVertices>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {
            return luaL_error(state, "Invalid key type");
        }
        
        std::string field = key;
        if(field == "topLeft") {
            lua_push_meta_event_widget_render_vertex(state, vertices->top_left);
        }
        else if(field == "topRight") {
            lua_push_meta_event_widget_render_vertex(state, vertices->top_right);
        }
        else if(field == "bottomLeft") {
            lua_push_meta_event_widget_render_vertex(state, vertices->bottom_left);
        }
        else if(field == "bottomRight") {
            lua_push_meta_event_widget_render_vertex(state, vertices->bottom_right);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_event_widget_render_vertices__newindex(lua_State *state) noexcept {
        auto *vertices = lua_from_meta_object<Event::UIWidgetRenderVertices>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {
            return luaL_error(state, "Invalid key type");
        }
        
        std::string field = key;
        if(field == "topLeft") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "topRight") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "bottomLeft") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "bottomRight") {
            return luaL_error(state, "Invalid operation");
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_event_widget_render_vertices(lua_State *state, Event::UIWidgetRenderVertices &vertices, bool read_only) noexcept {
        lua_push_meta_object(state, &vertices, lua_event_widget_render_vertices__index, lua_event_widget_render_vertices__newindex, read_only);
    }
}
