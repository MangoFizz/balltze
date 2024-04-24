// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__PLUGINS__LUA__METATABLES__ENGINE_RASTERIZER_HPP
#define BALLTZE__PLUGINS__LUA__METATABLES__ENGINE_RASTERIZER_HPP

#include <balltze/engine.hpp>
#include <balltze/event.hpp>

namespace Balltze::Plugins {
    void lua_push_meta_engine_camera_data(lua_State *state, Engine::CameraData &data, bool read_only = false) noexcept;
    void lua_push_meta_event_widget_render_vertex(lua_State *state, Event::UIWidgetRenderVertices::Vertex &vertex, bool read_only = false) noexcept;
    void lua_push_meta_event_widget_render_vertices(lua_State *state, Event::UIWidgetRenderVertices &vertices, bool read_only = false) noexcept;
}

#endif
