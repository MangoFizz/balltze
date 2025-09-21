// SPDX-License-Identifier: GPL-3.0-only

#include <stack>
#include <utility>
#include <balltze/hook.hpp>
#include <balltze/command.hpp>
#include "../../logger.hpp"
#include "console_command.hpp"

namespace Balltze::LegacyApi::Event {
    extern "C" {
        bool console_command_event_before();
        void console_command_event_after();
        void *console_command_function = nullptr;

        bool dispatch_console_command_event_before(const char *command) {
            ConsoleCommandEventContext args(command);
            ConsoleCommandEvent console_command_event(EVENT_TIME_BEFORE, args);
            console_command_event.dispatch();
            return console_command_event.cancelled();
        }

        void dispatch_console_command_event_after(const char *command) {
            ConsoleCommandEventContext args(command);
            ConsoleCommandEvent console_command_event(EVENT_TIME_AFTER, args);
            console_command_event.dispatch();
        }
    }

    template<>
    void EventHandler<ConsoleCommandEvent>::init() {}
}
