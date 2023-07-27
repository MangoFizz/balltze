// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/event.hpp>
#include <balltze/engine/game_state.hpp>
#include <balltze/hook.hpp>
#include <balltze/command.hpp>
#include "../logger.hpp"

namespace Balltze::Event {
    static bool dispatch_rcon_message_event_before(const char *message) {
        RconMessageEventArgs args(message);
        RconMessageEvent event(EVENT_TIME_BEFORE, args);
        event.dispatch();
        return event.cancelled();
    }

    static void dispatch_rcon_message_event_after(const char *message) {
        RconMessageEventArgs args(message);
        RconMessageEvent event(EVENT_TIME_AFTER, args);
        event.dispatch();
    }

    static bool debug_rcon_message_event(int arg_count, const char **args) {
        static std::optional<Event::EventListenerHandle<RconMessageEvent>> handle;
        if(arg_count == 1) {
            bool new_setting = STR_TO_BOOL(args[0]);
            if(new_setting) {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
                handle = Event::RconMessageEvent::subscribe_const([](RconMessageEvent const &event) {
                    auto &arguments = event.args;
                    auto time = event_time_to_string(event.time);
                    logger.debug("Rcon message event ({}): message: {}", time, arguments.message);
                });
            }
            else {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
            }
        }
        logger.info("debug_rcon_message_event: {}", handle.has_value());
        return true;
    }

    template <>
    void EventHandler<RconMessageEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        auto *rcon_message_function_call_sig = Memory::get_signature("rcon_message_function_call");
        if(!rcon_message_function_call_sig) {
            throw std::runtime_error("Could not find signature for rcon message event");
        }

        try {
            // Workaround for Chimera hook (NEEDS TO BE FIXED)
            std::byte *ptr = Memory::follow_32bit_jump(rcon_message_function_call_sig->data()) + 9;
            auto *camera_data_read_chimera_hook = Memory::hook_function(ptr, reinterpret_cast<void(*)()>(dispatch_rcon_message_event_before), reinterpret_cast<void(*)()>(dispatch_rcon_message_event_after));
        }
        catch(const std::runtime_error &e) {
            throw std::runtime_error("Could not hook rcon message event: " + std::string(e.what()));
        }

        // Register debug command
        register_command("debug_rcon_message_event", "debug", "Debug rcon message event", "[enable]", debug_rcon_message_event, true, 0, 1);
    }
}
