// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__EVENTS__CONSOLE_HPP
#define BALLTZE_API__EVENTS__CONSOLE_HPP

#include "../events.hpp"

namespace Balltze::Events {
    class ConsoleCommandEvent : public EventData<ConsoleCommandEvent> {
    public:
        /**
         * Get the console input command.
         * 
         * @return The console input command.
         */
        const std::string &console_input() const noexcept;

        /**
         * Constructor for ConsoleCommandEvent.
         * This event is dispatched when a console command is executed.
         */
        ConsoleCommandEvent(const std::string &command) : EventData(true), m_console_input(command) {}
    private:
        std::string m_console_input;
    };
}

#endif
