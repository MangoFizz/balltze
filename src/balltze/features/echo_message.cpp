// SPDX-License-Identifier: GPL-3.0-only

#include <optional>
#include <balltze/command.hpp>
#include <balltze/legacy_api/engine/netgame.hpp>
#include <balltze/legacy_api/engine/netgame_messages.hpp>
#include <balltze/legacy_api/event.hpp>
#include "../logger.hpp"

namespace Balltze::Features {
    static void send_ping_message() {
        const wchar_t *ping = L"PING";
        switch(get_balltze_side()) {
            case BALLTZE_SIDE_CLIENT: {
                auto message = LegacyApi::Engine::NetworkGameMessages::HudChat(LegacyApi::Engine::NetworkGameMessages::HudChatType::CUSTOM, LegacyApi::Engine::network_game_get_local_rcon_id(), const_cast<wchar_t *>(ping));
                char buffer[sizeof(message) + 16];
                uint32_t size = LegacyApi::Engine::network_game_encode_message(buffer, LegacyApi::Engine::NETWORK_GAME_MESSAGE_TYPE_HUD_CHAT, &message);
                LegacyApi::Engine::network_game_client_send_message(buffer, size);
                break;
            }
            case BALLTZE_SIDE_DEDICATED_SERVER: {
                auto message = LegacyApi::Engine::NetworkGameMessages::HudChat(LegacyApi::Engine::NetworkGameMessages::HudChatType::CUSTOM, -1, const_cast<wchar_t *>(ping));
                char buffer[sizeof(message) + 16];
                uint32_t size = LegacyApi::Engine::network_game_encode_message(buffer, LegacyApi::Engine::NETWORK_GAME_MESSAGE_TYPE_HUD_CHAT, &message);
                LegacyApi::Engine::network_game_server_send_message_to_all_machines(buffer, size, true, true, false, true, 2);
                break;
            }
        }
        logger.info("Ping sent!");
    }

    static void send_pong_message(std::int8_t machine_id) {
        const wchar_t *pong = L"PONG";
        switch(get_balltze_side()) {
            case BALLTZE_SIDE_CLIENT: {
                auto message = LegacyApi::Engine::NetworkGameMessages::HudChat(LegacyApi::Engine::NetworkGameMessages::HudChatType::CUSTOM, LegacyApi::Engine::network_game_get_local_rcon_id(), const_cast<wchar_t *>(pong));
                char buffer[sizeof(message) + 16];
                uint32_t size = LegacyApi::Engine::network_game_encode_message(buffer, LegacyApi::Engine::NETWORK_GAME_MESSAGE_TYPE_HUD_CHAT, &message);
                LegacyApi::Engine::network_game_client_send_message(buffer, size);
                break;
            }
            case BALLTZE_SIDE_DEDICATED_SERVER: {
                auto message = LegacyApi::Engine::NetworkGameMessages::HudChat(LegacyApi::Engine::NetworkGameMessages::HudChatType::CUSTOM, -1, const_cast<wchar_t *>(pong));
                char buffer[sizeof(message) + 16];
                uint32_t size = LegacyApi::Engine::network_game_encode_message(buffer, LegacyApi::Engine::NETWORK_GAME_MESSAGE_TYPE_HUD_CHAT, &message);
                LegacyApi::Engine::network_game_server_send_message_to_machine(machine_id, buffer, size, true, true, false, true, 2);
                break;
            }
        }
    }

    static bool ping_packet_command(int argc, const char **argv) {
        send_ping_message();
        return true;
    }

    void set_up_echo_message_command() {
        LegacyApi::Event::NetworkGameChatMessageEvent::subscribe([](LegacyApi::Event::NetworkGameChatMessageEvent &event) {
            auto &context = event.context;
            if(event.time == LegacyApi::Event::EVENT_TIME_BEFORE) {
                if(context.chat_message->msg_type == LegacyApi::Engine::NetworkGameMessages::HudChatType::CUSTOM) {
                    if(std::wcsncmp(context.chat_message->message, L"PING", 4) == 0) {
                        std::string msg_str;
                        size_t wstr_len = std::wcslen(context.chat_message->message);
                        for(size_t i = 0; i < wstr_len; i++) {
                            msg_str.push_back(static_cast<char>(context.chat_message->message[i] & 0xFF));
                        }
                        logger.info("Ball Ball says {}. Sending pong...", msg_str);

                        send_pong_message(context.chat_message->player_id);
                    }
                    if(std::wcsncmp(context.chat_message->message, L"PONG", 4) == 0) {
                        std::string msg_str;
                        size_t wstr_len = std::wcslen(context.chat_message->message);
                        for(size_t i = 0; i < wstr_len; i++) {
                            msg_str.push_back(static_cast<char>(context.chat_message->message[i] & 0xFF));
                        }
                        logger.info("Ball Ball says {}.", msg_str);
                    }
                }
            }
        }, LegacyApi::Event::EVENT_PRIORITY_HIGHEST);

        register_command("send_ping_message", "debug", "Send a ping message through the chat.", std::nullopt, ping_packet_command, false, 0, 0);
    }
}
