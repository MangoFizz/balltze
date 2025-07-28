// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/events/frame.hpp>
#include <balltze/legacy_api/events/frame.hpp>

namespace Balltze::Events {
    void dispatch_frame_begin_event() {
        FrameBeginEvent event;
        event.dispatch();
    }

    template<>
    void EventHandler<FrameBeginEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        LegacyApi::Event::FrameEvent::subscribe([](auto &event) {
            if(event.time == LegacyApi::Event::EVENT_TIME_BEFORE) {
                dispatch_frame_begin_event();
            }
        });
    }

    void dispatch_frame_end_event() {
        FrameEndEvent event;
        event.dispatch();
    }

    template<>
    void EventHandler<FrameEndEvent>::init() {
        if(m_initialized) {
            return;
        }
        m_initialized = true;

        LegacyApi::Event::FrameEvent::subscribe([](auto &event) {
            if(event.time == LegacyApi::Event::EVENT_TIME_AFTER) {
                dispatch_frame_begin_event();
            }
        });
    }
}
