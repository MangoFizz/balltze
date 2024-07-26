// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__EVENTS__RCON_MESSAGE_HPP
#define BALLTZE_API__EVENTS__RCON_MESSAGE_HPP

#include <cstdint>
#include <string>
#include "../event.hpp"

namespace Balltze::Event {
    struct RconMessageEventContext {
        const std::string message;

        RconMessageEventContext(const char *const message) : message(message) {}
    };

    class RconMessageEvent : public EventData<RconMessageEvent> {
    public:
        RconMessageEventContext context;

        bool cancellable() const {
            return true;
        }

        RconMessageEvent(EventTime time, const RconMessageEventContext &context) : EventData(time), context(context) {}
    };
}

#endif
