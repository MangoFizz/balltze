// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__EVENTS__CONSOLE_COMMAND_HPP
#define BALLTZE_API__EVENTS__CONSOLE_COMMAND_HPP

#include <string>
#include <balltze/legacy_api/event.hpp>

namespace Balltze::LegacyApi::Event {
    struct ConsoleCommandEventContext {
        std::string command;

        ConsoleCommandEventContext(std::string command) : command(command) {}
    };

    class ConsoleCommandEvent : public EventData<ConsoleCommandEvent> {
    public:
        ConsoleCommandEventContext context;

        bool cancellable() const {
            return true;
        }

        ConsoleCommandEvent(EventTime time, ConsoleCommandEventContext context) : EventData(time), context(context) {}
    };
}

#endif
