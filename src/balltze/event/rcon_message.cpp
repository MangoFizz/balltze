// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/event.hpp>
#include <balltze/engine/game_state.hpp>
#include <balltze/hook.hpp>
#include <balltze/api.hpp>
#include <balltze/command.hpp>
#include <balltze/helpers/string.hpp>
#include "../logger.hpp"

namespace Balltze::Event {
    extern "C" {
        bool rcon_message_event_before_hook();
        void rcon_message_event_after_hook();

        bool dispatch_rcon_message_event_before(char *message_data) {
            std::optional<std::string> decoded_message;
            try {
                decoded_message = string_w1252_to_utf8(message_data);
            }
            catch(std::runtime_error &e) {
                logger.warning("Failed to decode RCON message string: {}", e.what());
            }
            RconMessageEventContext args(message_data, decoded_message);
            RconMessageEvent event(EVENT_TIME_BEFORE, args);
            event.dispatch();
            return event.cancelled();
        }

        void dispatch_rcon_message_event_after(char *message_data) {
            std::optional<std::string> decoded_message;
            try {
                decoded_message = string_w1252_to_utf8(message_data);
            }
            catch(std::runtime_error &e) {
                logger.warning("Failed to decode RCON message string: {}", e.what());
            }
            RconMessageEventContext args(message_data, decoded_message);
            RconMessageEvent event(EVENT_TIME_BEFORE, args);
            event.dispatch();
        }
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
                    auto &context = event.context;
                    auto time = event_time_to_string(event.time);
                    logger.debug("Rcon message event ({}): message: {}", time, context.message.value_or("null"));
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

    template<>
    void EventHandler<RconMessageEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        if(get_balltze_side() != BALLTZE_SIDE_CLIENT) {
            return;
        }

        auto *rcon_message_function_call_sig = Memory::get_signature("rcon_message_function_call");
        if(!rcon_message_function_call_sig) {
            throw std::runtime_error("Could not find signature for rcon message event");
        }

        try {
            // Workaround for Chimera hook (NEEDS TO BE FIXED)
            std::byte *ptr = Memory::follow_32bit_jump(rcon_message_function_call_sig->data()) + 9;
            std::function<bool()> before_function = reinterpret_cast<bool(*)()>(rcon_message_event_before_hook);
            auto *hook = Memory::hook_function(ptr, before_function, reinterpret_cast<void(*)()>(rcon_message_event_after_hook));
        }
        catch(const std::runtime_error &e) {
            throw std::runtime_error("Could not hook rcon message event: " + std::string(e.what()));
        }

        // Register debug command
        register_command("debug_rcon_message_event", "debug", "Sets whenever to log rcon message event.", "[enable: boolean]", debug_rcon_message_event, true, 0, 1);
    }
}
