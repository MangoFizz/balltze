// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__EVENTS__TICK_HPP
#define BALLTZE_API__EVENTS__TICK_HPP

#include "../event.hpp"

namespace Balltze::LegacyApi::Event {
    struct TickEventContext {
        const std::size_t delta_time_ms;
        const std::size_t tick_count;

        TickEventContext(std::size_t delta_time_ms, std::size_t tick_count) : delta_time_ms(delta_time_ms), tick_count(tick_count) {}
    };

    class TickEvent : public EventData<TickEvent> {
    public:
        TickEventContext context;

        bool cancellable() const {
            return false;
        }

        TickEvent(EventTime time, TickEventContext context) : EventData(time), context(context) {}
    };
}

#endif
