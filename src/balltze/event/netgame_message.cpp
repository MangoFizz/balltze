// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/event.hpp>
#include <balltze/engine/game_state.hpp>
#include <balltze/memory.hpp>
#include <balltze/hook.hpp>
#include <balltze/api.hpp>
#include <balltze/command.hpp>
#include "../logger.hpp"

namespace Balltze::Event {
    extern "C" {
        void network_game_client_decode_hud_message_call_override();
        void network_game_server_decode_hud_message_call_override();
        void *network_game_decode_hud_message_call_override_return = nullptr;

        bool dispatch_network_game_chat_message_event_before(NetworkGameChatMessage *chat_message) {
            NetworkGameChatMessageEventContext args(chat_message);
            NetworkGameChatMessageEvent event(EVENT_TIME_BEFORE, args);
            event.dispatch();
            return event.cancelled();
        }

        void dispatch_network_game_chat_message_event_after(NetworkGameChatMessage *chat_message) {
            NetworkGameChatMessageEventContext args(chat_message);
            NetworkGameChatMessageEvent event(EVENT_TIME_AFTER, args);
            event.dispatch();
        }
    }

    static bool debug_network_game_chat_message_event_command(int argc, const char **argv) {
        static std::optional<Event::EventListenerHandle<NetworkGameChatMessageEvent>> handle;
        if(argc == 1) {
            bool new_setting = STR_TO_BOOL(argv[0]);
            if(new_setting) {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
                handle = NetworkGameChatMessageEvent::subscribe_const([](NetworkGameChatMessageEvent const &event) {
                    auto &context = event.context;
                    if(event.time == EVENT_TIME_BEFORE) {
                        logger.debug("Network game message event: message type: {}", static_cast<int>(context.chat_message->msg_type));
                    }                    
                }, EVENT_PRIORITY_HIGHEST);
            }
            else {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
            }
        }
        logger.info("debug_network_game_message_event: {}", handle.has_value());
        return true;
    }

    template<>
    void EventHandler<NetworkGameChatMessageEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        std::string signature;
        std::function<void()> override_function;
        if(get_balltze_side() == BALLTZE_SIDE_CLIENT) {
            signature = "network_game_client_decode_hud_message_call";
            override_function = network_game_client_decode_hud_message_call_override;
        }
        else {
            signature = "network_game_server_decode_hud_message_call";
            override_function = network_game_server_decode_hud_message_call_override;
        }

        auto *decode_received_chat_message_call_sig = Memory::get_signature(signature);
        if(!decode_received_chat_message_call_sig) {
            throw std::runtime_error("Could not find signature for network game chat message event");
        }

        try {
            void *address = decode_received_chat_message_call_sig->data();
            if(*reinterpret_cast<uint8_t *>(address) == 0xE9) {
                address = Memory::follow_32bit_jump(address);
            }
            auto *hook = Memory::override_function(address, override_function, network_game_decode_hud_message_call_override_return);
            Memory::Codecave &cave = hook->cave();
            auto *code = cave.data();
            for(size_t i = 5; i < 10; i++) {
                code[i] = static_cast<std::byte>(0x90);
            }
        }
        catch(const std::runtime_error &e) {
            throw std::runtime_error("Could not hook network game chat message event: " + std::string(e.what()));
        }

        register_command("debug_network_game_chat_message_event", "debug", "Sets whenever to log when a network game chat message is received.", "[enable: boolean]", debug_network_game_chat_message_event_command, true, 0, 1);
    }
}
