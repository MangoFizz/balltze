// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/events/map_load.hpp>
#include <impl/rasterizer/rasterizer_render.h>

namespace Balltze::Features {
    static void on_map_load(Event::MapLoadEvent &event) {
        if(event.time == Event::EVENT_TIME_BEFORE) {
            static RenderGlobals *render_globals = ::rasterizer_render_get_globals();
            render_globals->fog.planar_color.r = 0.0f;
            render_globals->fog.planar_color.g = 0.0f;
            render_globals->fog.planar_color.b = 0.0f;
            render_globals->fog.planar_maximum_depth = 0.0f;
            render_globals->fog.planar_maximum_density = 0.0f;
            render_globals->fog.plane.i = 0.0f;
            render_globals->fog.plane.j = 0.0f;
            render_globals->fog.plane.k = 0.0f;
            render_globals->fog.plane.w = 0.0f;
        }
    }

    void set_up_fog_plane_reset_fix() noexcept {
        Event::MapLoadEvent::subscribe(on_map_load, Event::EVENT_PRIORITY_HIGHEST);
    }
}
