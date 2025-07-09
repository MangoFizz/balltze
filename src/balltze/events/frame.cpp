// SPDX-License-Identifier: GPL-3.0-only

#include <chrono>
#include <balltze/events/frame.hpp>
#include <balltze/legacy_api/events/frame.hpp>

namespace Balltze::Events {
    void dispatch_frame_event() {
        FrameEvent event;
        event.dispatch();
    }

    template<>
    void EventHandler<FrameEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        LegacyApi::Event::FrameEvent::subscribe([](auto &event) {
            if(event.time == LegacyApi::Event::EVENT_TIME_BEFORE) {
                dispatch_frame_event();
            }
        });
    }
}
