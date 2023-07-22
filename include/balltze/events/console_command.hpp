// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__EVENTS__CONSOLE_COMMAND_HPP
#define BALLTZE_API__EVENTS__CONSOLE_COMMAND_HPP

#include <string>
#include "../event.hpp"

namespace Balltze::Event {
    struct ConsoleCommandEventArguments {
        std::string command;

        ConsoleCommandEventArguments(std::string command) : command(command) {}
    };

    class ConsoleCommandEvent : public EventData<ConsoleCommandEvent> {
    public:
        ConsoleCommandEventArguments args;

        bool cancellable() const {
            return true;
        }

        ConsoleCommandEvent(EventTime time, ConsoleCommandEventArguments args) : EventData(time), args(args) {}
    };
}

#endif
