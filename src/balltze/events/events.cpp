// SPDX-License-Identifier: GPL-3.0-only

#include "events.hpp"

namespace Balltze::Events {
    void set_up_events_handlers() {
        try {
            EventHandler<FrameBeginEvent>::init();
            EventHandler<FrameEndEvent>::init();
            EventHandler<TickEvent>::init();
            EventHandler<MapLoadEvent>::init();
            EventHandler<MapLoadedEvent>::init();
            EventHandler<PlayerInputEvent>::init();
            EventHandler<WidgetEventDispatchEvent>::init();
        }
        catch(std::runtime_error) {
            throw;
        }
    }
} 
