// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__EVENTS__FRAME_HPP
#define BALLTZE_API__EVENTS__FRAME_HPP

#include "../events.hpp"

namespace Balltze::Events {
    class FrameBeginEvent : public EventData<FrameBeginEvent> {
    public:
        FrameBeginEvent() : EventData(false) {}
    };

    class FrameEndEvent : public EventData<FrameEndEvent> {
    public:
        FrameEndEvent() : EventData(false) {}
    };

    using FrameEvent = FrameEndEvent;
}

#endif
