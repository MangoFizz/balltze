// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/legacy_api/event.hpp>
#include <balltze/legacy_api/engine/game_state.hpp>
#include <balltze/memory.hpp>
#include <balltze/hook.hpp>
#include <balltze/api.hpp>
#include <balltze/command.hpp>
#include "../../logger.hpp"

namespace Balltze::LegacyApi::Event {
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
        static std::optional<LegacyApi::Event::EventListenerHandle<NetworkGameChatMessageEvent>> handle;
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
            auto *hook = Memory::override_function(address, override_function, &network_game_decode_hud_message_call_override_return);
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

    extern "C" {
        void netgame_sound_override();
        void *netgame_sound_fn_return;

        bool dispatch_network_game_sound_event_before(LegacyApi::Engine::NetworkGameMultiplayerSound sound) {
            NetworkGameMultiplayerSoundEventContext args = { .sound = sound };
            NetworkGameMultiplayerSoundEvent event(EVENT_TIME_BEFORE, args);
            event.dispatch();
            return event.cancelled();
        }

        void dispatch_network_game_sound_event_after(LegacyApi::Engine::NetworkGameMultiplayerSound sound) {
            NetworkGameMultiplayerSoundEventContext args = { .sound = sound };
            NetworkGameMultiplayerSoundEvent event(EVENT_TIME_AFTER, args);
            event.dispatch();
        }
    }

    template<>
    void EventHandler<NetworkGameMultiplayerSoundEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        if(get_balltze_side() != BALLTZE_SIDE_CLIENT) {
            return;
        }

        auto *netgame_sound_sig = Memory::get_signature("network_game_multiplayer_sound_call");
        if(!netgame_sound_sig) {
            throw std::runtime_error("Could not find signature for network game sound event");
        }

        try {
            auto *hook = Memory::override_function(netgame_sound_sig->data(), netgame_sound_override, &netgame_sound_fn_return);
        }
        catch(const std::runtime_error &e) {
            throw std::runtime_error("Could not hook network game sound event: " + std::string(e.what()));
        }
    }

    extern "C" {
        bool network_game_multiplayer_hud_message_event_before_asm();
        void network_game_multiplayer_hud_message_event_after_asm();
    
        bool dispatch_network_game_multiplayer_hud_message_before(LegacyApi::Engine::NetworkGameMultiplayerHudMessage message_type, LegacyApi::Engine::PlayerHandle causer, LegacyApi::Engine::PlayerHandle victim, LegacyApi::Engine::PlayerHandle local_player) {
            NetworkGameHudMessageEventContext args = { .message_type = message_type, .causer = causer, .victim = victim, .local_player = local_player };
            NetworkGameHudMessageEvent event(EVENT_TIME_BEFORE, args);
            event.dispatch();
            return event.cancelled();
        }

        void dispatch_network_game_multiplayer_hud_message_after(LegacyApi::Engine::NetworkGameMultiplayerHudMessage message_type, LegacyApi::Engine::PlayerHandle causer, LegacyApi::Engine::PlayerHandle victim, LegacyApi::Engine::PlayerHandle local_player) {
            NetworkGameHudMessageEventContext args = { .message_type = message_type, .causer = causer, .victim = victim, .local_player = local_player };
            NetworkGameHudMessageEvent event(EVENT_TIME_AFTER, args);
            event.dispatch();
        }
    }
    
    template<>
    void EventHandler<NetworkGameHudMessageEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        if(get_balltze_side() != BALLTZE_SIDE_CLIENT) {
            return;
        }

        auto *network_game_multiplayer_hud_message_event_before_sig = Memory::get_signature("network_game_multiplayer_hud_message_dispatch_call");
        if(!network_game_multiplayer_hud_message_event_before_sig) {
            throw std::runtime_error("Could not find signature for network game hud message event");
        }

        try {
            std::function<bool()> before_function = network_game_multiplayer_hud_message_event_before_asm;
            auto *hook = Memory::hook_function(network_game_multiplayer_hud_message_event_before_sig->data(), before_function, network_game_multiplayer_hud_message_event_after_asm);
        }
        catch(const std::runtime_error &e) {
            throw std::runtime_error("Could not hook network game hud message event: " + std::string(e.what()));
        }
    }
}
