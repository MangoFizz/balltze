// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__EVENTS__TICK_HPP
#define BALLTZE_API__EVENTS__TICK_HPP

#include "../event.hpp"

namespace Balltze::Event {
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

        TickEvent(EventTime time, TickEventArguments args) : EventData(time), args(args) {}
    };
}

#endif
