// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__EVENTS__FRAME_HPP
#define BALLTZE_API__EVENTS__FRAME_HPP

#include "../events.hpp"

namespace Balltze::Events {
    class FrameEvent : public EventData<FrameEvent> {
    public:
        FrameEvent() : EventData(false) {}
    };
}

#endif
