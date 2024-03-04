// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/memory.hpp>
#include <balltze/engine/network.hpp>
#include "../logger.hpp"

namespace Balltze::Engine {
    extern "C" {
        std::uint32_t mdp_encode_stateless_iterated_asm(void *output_buffer, std::uint32_t arg1, std::uint32_t type, std::uint32_t arg3, void **message, std::uint32_t arg5, std::uint32_t arg6, std::uint32_t arg7);
        bool mdp_decode_stateless_iterated_asm(void *destination, MessageDeltaHeader *message_header);
        void mdp_discard_iteration_body_asm(MessageDeltaHeader *message_header);
        void net_send_message_to_all_asm(void *message, std::uint32_t message_size, std::uint32_t ingame_only, std::uint32_t write_to_local_connection, std::uint32_t flush_queue, std::uint32_t unbuffered, std::uint32_t buffer_priority);
        void net_send_message_to_player_asm(void *message, std::uint32_t message_size, std::uint32_t ingame_only, std::uint32_t write_to_local_connection, std::uint32_t flush_queue, std::uint32_t unbuffered, std::uint32_t buffer_priority, std::uint32_t player_id);
    }

    uint32_t mdp_encode_stateless_iterated(void *output_buffer, int32_t arg1, MessageDeltaType type, uint32_t arg3, void* message, uint32_t arg5, uint32_t arg6, uint32_t arg7) {
        return mdp_encode_stateless_iterated_asm(output_buffer, arg1, type, arg3, &message, arg5, arg6, arg7);
    }

    uint32_t mdp_encode_stateless_iterated(void *output_buffer, MessageDeltaType type, void *message) {
        return mdp_encode_stateless_iterated_asm(output_buffer, 0, type, 0, &message, 0, 1, 0);
    }

    bool mdp_decode_stateless_iterated(void *destination, MessageDeltaHeader *message_header) {
        return mdp_decode_stateless_iterated_asm(destination, message_header);
    }

    void mdp_discard_iteration_body(MessageDeltaHeader *message_header) {
        mdp_discard_iteration_body_asm(message_header);
    }

    void send_delta_message_to_all(void* message, uint32_t message_size, bool ingame_only, bool write_to_local_connection, bool flush_queue, bool unbuffered, int32_t buffer_priority) {
        net_send_message_to_all_asm(message, message_size, ingame_only, write_to_local_connection, flush_queue, unbuffered, buffer_priority);
    }

    void send_delta_message_to_player(int32_t player_id, void *message, uint32_t message_size, bool ingame_only, bool write_to_local_connection, bool flush_queue, bool unbuffered, int32_t buffer_priority) {
        net_send_message_to_player_asm(message, message_size, ingame_only, write_to_local_connection, flush_queue, unbuffered, buffer_priority, player_id);
    }
}
