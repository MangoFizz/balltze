// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/memory.hpp>
#include <balltze/engine/game_state.hpp>
#include <balltze/engine/netgame.hpp>
#include "../logger.hpp"

namespace Balltze::Engine {
    extern "C" {
        std::uint32_t network_game_encode_message_asm(void *output_buffer, std::uint32_t arg1, std::uint32_t type, std::uint32_t arg3, void **message, std::uint32_t arg5, std::uint32_t arg6, std::uint32_t arg7);
        bool network_game_decode_message_asm(void *destination, NetworkGameMessages::MessageHeader *message_header);
        void network_game_server_send_message_to_all_machines_asm(void *message, std::uint32_t message_size, std::uint32_t ingame_only, std::uint32_t write_to_local_connection, std::uint32_t flush_queue, std::uint32_t unbuffered, std::uint32_t buffer_priority);
        void network_game_server_send_message_to_machine_asm(void *message, std::uint32_t message_size, std::uint32_t ingame_only, std::uint32_t write_to_local_connection, std::uint32_t flush_queue, std::uint32_t unbuffered, std::uint32_t buffer_priority, std::uint32_t machine_id);
        void network_game_client_send_chat_message_asm(int channel, const wchar_t *message);
    }

    NetworkGameServerType network_game_get_server_type() {
        static auto *server_type = *reinterpret_cast<NetworkGameServerType **>(Memory::get_signature("server_type")->data() + 3);
        return *server_type;
    }

    NetworkGameType network_game_get_current_game_type() {
        static auto *gametype = *reinterpret_cast<NetworkGameType **>(Memory::get_signature("current_gametype")->data() + 2);
        return *gametype;
    }

    bool network_game_current_game_is_team() {
        static auto *is_team = *reinterpret_cast<std::uint8_t **>(Memory::get_signature("current_gametype")->data() + 2) + 4;
        return *is_team;
    }

    void network_game_client_send_chat_message(int channel, const wchar_t *message) {
        if(!message) {
            return;
        }
        network_game_client_send_chat_message_asm(channel, message);
    }
    
    void network_game_client_send_chat_message(int channel, const char *message) {
        if(!message) {
            return;
        }

        wchar_t message_to_send[256] = {};
        MultiByteToWideChar(CP_UTF8, 0, message, -1, message_to_send, sizeof(message_to_send) / sizeof(*message_to_send));
        network_game_client_send_chat_message_asm(channel, message_to_send);
    }

    Player *NetworkGameServerPlayer::get_player() const noexcept {
        auto &player_table = get_player_table();
        if(this->player_id >= player_table.current_size) {
            return nullptr;
        }
        return player_table.first_element + this->player_id;
    }

    NetworkGameServer *network_game_get_server() noexcept {
        #define RETURN_TABLE_FOR_SIGNATURE(SIGNATURE) { \
            static NetworkGameServer *table = nullptr; \
            if(!table) { \
                table = reinterpret_cast<NetworkGameServer *>(*reinterpret_cast<std::byte **>(Memory::get_signature(SIGNATURE)->data()) - 8); \
            } \
            return table; \
        }

        switch(network_game_get_server_type()) {
            // In a server (hosting)
            case NETWORK_GAME_SERVER_LOCAL:
                RETURN_TABLE_FOR_SIGNATURE("server_info_host")

            // In a server (as client)
            case NETWORK_GAME_SERVER_DEDICATED:
                RETURN_TABLE_FOR_SIGNATURE("server_info_client")

            // Not in a server
            default:
                return nullptr;
        }
    }

    uint32_t network_game_encode_message(void *output_buffer, int32_t arg1, NetworkGameMessageType type, uint32_t arg3, void* message, uint32_t arg5, uint32_t arg6, uint32_t arg7) {
        return network_game_encode_message_asm(output_buffer, arg1, type, arg3, &message, arg5, arg6, arg7);
    }

    uint32_t network_game_encode_message(void *output_buffer, NetworkGameMessageType type, void *message) {
        return network_game_encode_message_asm(output_buffer, 0, type, 0, &message, 0, 1, 0);
    }

    bool network_game_decode_message(void *destination, NetworkGameMessages::MessageHeader *message_header) {
        return network_game_decode_message_asm(destination, message_header);
    }

    void network_game_server_send_message_to_all_machines(void *message, uint32_t message_size, bool ingame_only, bool write_to_local_connection, bool flush_queue, bool unbuffered, int32_t buffer_priority) {
        network_game_server_send_message_to_all_machines_asm(message, message_size, ingame_only, write_to_local_connection, flush_queue, unbuffered, buffer_priority);
    }

    void network_game_server_send_message_to_machine(std::int8_t machine_id, void *message, uint32_t message_size, bool ingame_only, bool write_to_local_connection, bool flush_queue, bool unbuffered, int32_t buffer_priority) {
        network_game_server_send_message_to_machine_asm(message, message_size, ingame_only, write_to_local_connection, flush_queue, unbuffered, buffer_priority, machine_id);
    }

    void network_game_server_send_rcon_message(std::int8_t machine_id, const char *message) {
        Engine::NetworkGameMessages::RconResponse rcon_response;
        memcpy(rcon_response.text, message, 80);
        char buffer[sizeof(rcon_response) + 16];
        uint32_t size = Engine::network_game_encode_message(buffer, Engine::NETWORK_GAME_MESSAGE_TYPE_RCON_RESPONSE, &rcon_response);
        if(machine_id < 0) {
            Engine::network_game_server_send_message_to_all_machines(&buffer, size, true, true, false, true, 2);
        }
        else if(machine_id < 16) {
            Engine::network_game_server_send_message_to_machine(machine_id, &buffer, size, true, true, false, true, 2);
        }
    }
}
