// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/events/tick.hpp>
#include <balltze/legacy_api/events/tick.hpp>

namespace Balltze::Events {
    void dispatch_tick_event() {
        TickEvent event;
        event.dispatch();
    }

    template<>
    void EventHandler<TickEvent>::init() {
        if(m_initialized) {
            return;
        }
        m_initialized = true;

        LegacyApi::Event::TickEvent::subscribe([](auto &event) {
            if(event.time == LegacyApi::Event::EVENT_TIME_BEFORE) {
                dispatch_tick_event();
            }
        });
    }
}
