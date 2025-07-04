// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__EVENTS__RCON_MESSAGE_HPP
#define BALLTZE_API__EVENTS__RCON_MESSAGE_HPP

#include <cstdint>
#include <string>
#include "../event.hpp"

namespace Balltze::LegacyApi::Event {
    struct RconMessageEventContext {
        const std::optional<std::string> message;
        const char *const raw_message;

        RconMessageEventContext(const char *message_data, std::optional<std::string> message) : message(message), raw_message(message_data) {}
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
