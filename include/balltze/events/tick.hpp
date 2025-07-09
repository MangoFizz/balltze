// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__EVENTS__TICK_HPP
#define BALLTZE_API__EVENTS__TICK_HPP

#include "../events.hpp"

namespace Balltze::Events {
    class TickEvent : public EventData<TickEvent> {
    public:
        TickEvent() : EventData(false) {}
    };
}

#endif
