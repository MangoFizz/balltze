// SPDX-License-Identifier: GPL-3.0-only

#include <chrono>
#include <balltze/events/tick.hpp>
#include <balltze/legacy_api/events/tick.hpp>

namespace Balltze::Events {
    void dispatch_tick_event() {
        TickEvent event;
        event.dispatch();
    }

    template<>
    void EventHandler<TickEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        LegacyApi::Event::TickEvent::subscribe([](auto &event) {
            if(event.time == LegacyApi::Event::EVENT_TIME_BEFORE) {
                dispatch_tick_event();
            }
        });
    }
}
