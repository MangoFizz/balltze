// SPDX-License-Identifier: GPL-3.0-only

#include <stack>
#include <utility>
#include <balltze/hook.hpp>
#include <balltze/command.hpp>
#include "../logger.hpp"
#include "console_command.hpp"

namespace Balltze::Event {
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

    static bool debug_console_command_event(int arg_count, const char **args) {
        static std::optional<Event::EventListenerHandle<ConsoleCommandEvent>> handle;
        if(arg_count == 1) {
            bool new_setting = STR_TO_BOOL(args[0]);
            if(new_setting) {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
                handle = Event::ConsoleCommandEvent::subscribe_const([](ConsoleCommandEvent const &event) {
                    auto &context = event.context;
                    auto time = event_time_to_string(event.time);
                    logger.debug("Console command event ({}): command: {}", time, context.command);
                });
            }
            else {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
            }
        }
        logger.info("debug_console_command_event: {}", handle.has_value());
        return true;
    }

    template<>
    void EventHandler<ConsoleCommandEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        auto *execute_console_command_function_call_sig = Memory::get_signature("execute_console_command_function_call");
        if(!execute_console_command_function_call_sig) {
            throw std::runtime_error("Could not find signature for console command event");
        }

        try {
            auto before_func = std::function<bool()>(console_command_event_before);
            if(Memory::already_hooked(execute_console_command_function_call_sig->data())) {
                auto *address = Memory::follow_32bit_jump(execute_console_command_function_call_sig->data()) + 9;
                Memory::hook_function(address, before_func, console_command_event_after);
            }
            else {
                Memory::hook_function(execute_console_command_function_call_sig->data(), before_func, console_command_event_after);
            }
        }
        catch(const std::runtime_error &e) {
            throw std::runtime_error("Could not hook console command event: " + std::string(e.what()));
        }

        // Register debug command
        register_command("debug_console_command_event", "debug", "Sets whenever to log console command event.", "[enable: boolean]", debug_console_command_event, true, 0, 1);
    }
}
