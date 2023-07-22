// SPDX-License-Identifier: GPL-3.0-only

#include <stack>
#include <utility>
#include <balltze/hook.hpp>
#include "../logger.hpp"
#include "console_command.hpp"

namespace Balltze::Event {
    extern "C" {
        void console_command_event();
        void *console_command_function = nullptr;

        bool dispatch_console_command_event_before(const char *command) {
            ConsoleCommandEventArguments args(command);
            ConsoleCommandEvent console_command_event(EVENT_TIME_BEFORE, args);
            console_command_event.dispatch();
            return console_command_event.cancelled();
        }

        void dispatch_console_command_event_after(const char *command) {
            ConsoleCommandEventArguments args(command);
            ConsoleCommandEvent console_command_event(EVENT_TIME_AFTER, args);
            console_command_event.dispatch();
        }
    }

    template <>
    void EventHandler<ConsoleCommandEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        auto *execute_console_command_function_sig = Memory::get_signature("execute_console_command_function");
        if(!execute_console_command_function_sig) {
            throw std::runtime_error("Could not find signature for console command event");
        }

        try {
            Memory::override_function(execute_console_command_function_sig->data(), console_command_event, console_command_function);
        }
        catch(const std::runtime_error &e) {
            throw std::runtime_error("Could not hook console command event: " + std::string(e.what()));
        }
    }
}
