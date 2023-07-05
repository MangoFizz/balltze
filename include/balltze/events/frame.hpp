// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__EVENTS__FRAME_HPP
#define BALLTZE_API__EVENTS__FRAME_HPP

#include "../event.hpp"

namespace Balltze::Event {
    class FrameEvent : public EventData<FrameEvent> {
    public:
        static bool cancellable() {
            return false;
        }

        FrameEvent(EventTime time) : EventData(time) {}
    };
}

#endif
