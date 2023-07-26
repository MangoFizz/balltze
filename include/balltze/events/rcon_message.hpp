// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__EVENTS__RCON_MESSAGE_HPP
#define BALLTZE_API__EVENTS__RCON_MESSAGE_HPP

#include <cstdint>
#include <string>
#include "../event.hpp"

namespace Balltze::Event {
    struct RconMessageEventArgs {
        const std::string message;

        RconMessageEventArgs(const char *const message) : message(message) {}
    };

    class RconMessageEvent : public EventData<RconMessageEvent> {
    public:
        RconMessageEventArgs args;

        bool cancellable() const {
            return true;
        }

        RconMessageEvent(EventTime time, const RconMessageEventArgs &args) : EventData(time), args(args) {}
    };
}

#endif
