// SPDX-License-Identifier: GPL-3.0-only

#include "events.hpp"

namespace Balltze::Events {
    void set_up_events_handlers() {
        try {
            EventHandler<FrameEvent>::init();
            EventHandler<TickEvent>::init();
        }
        catch(std::runtime_error) {
            throw;
        }
    }
} 
