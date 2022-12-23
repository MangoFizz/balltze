// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__EVENT__TICKEVENT_HPP
#define BALLTZE__EVENT__TICKEVENT_HPP

#include "event.hpp"

namespace Balltze {
    struct TickEventArguments {
        std::size_t delta_time_ms;
        std::size_t tick_count;
    };

    class TickEvent : public EventData<TickEvent> {
    public:
        TickEventArguments args;

        static bool cancellable() {
            return false;
        }

        TickEvent(EventTime time, TickEventArguments args) : EventData(time, false), args(args) {}
    };
}

#endif
