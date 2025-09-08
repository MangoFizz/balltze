// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/legacy_api/event.hpp>
#include <balltze/legacy_api/engine/game_state.hpp>
#include <balltze/hook.hpp>
#include <balltze/api.hpp>
#include <balltze/command.hpp>
#include <balltze/helpers/string.hpp>
#include "../../logger.hpp"

namespace Balltze::LegacyApi::Event {
    extern "C" {

        void rcon_message_event_before_hook();
        bool rcon_message_event_cancellation_hook();
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
            Memory::hook_function(rcon_message_function_call_sig->data(), std::nullopt, rcon_message_event_before_hook);
            std::byte *chimera_function_call = Memory::follow_32bit_jump(rcon_message_function_call_sig->data() + 5) + 2;
            std::byte *original_function_call = Memory::follow_32bit_jump(rcon_message_function_call_sig->data() + 5) + 9;
            std::function<bool()> before_function = reinterpret_cast<bool(*)()>(rcon_message_event_cancellation_hook);
            Memory::hook_function(chimera_function_call, before_function);
            Memory::hook_function(original_function_call, before_function, reinterpret_cast<void(*)()>(rcon_message_event_after_hook));
        }
        catch(const std::runtime_error &e) {
            throw std::runtime_error("Could not hook rcon message event: " + std::string(e.what()));
        }
    }
}
