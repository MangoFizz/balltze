// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__ENGINE__NETGAME_HPP
#define BALLTZE_API__ENGINE__NETGAME_HPP

#include <cstdint>
#include "../api.hpp"
#include "netgame_messages.hpp"

namespace Balltze::Engine {
    enum NetworkGameServerType : std::uint16_t {
        NETWORK_GAME_SERVER_NONE,
        NETWORK_GAME_SERVER_DEDICATED,
        NETWORK_GAME_SERVER_LOCAL
    };

    enum NetworkGameType : std::uint16_t {
        NETWORK_GAMETYPE_CTF = 1,
        NETWORK_GAMETYPE_SLAYER,
        NETWORK_GAMETYPE_ODDBALL,
        NETWORK_GAMETYPE_KING,
        NETWORK_GAMETYPE_RACE
    };

    enum NetworkGameMultiplayerTeam : std::uint16_t {
        NETWORK_GAME_TEAM_RED = 0,
        NETWORK_GAME_TEAM_BLUE,
    };

    struct Player;

    using PlayerHandle = ResourceHandle;

    /**
     * This is a player in the server info player table. This information is used in chat and sv_players.
     */
    struct NetworkGameServerPlayer {
        /** Name of the player */
        wchar_t name[0xC];

        /** Armor color of the player; unused? */
        std::uint8_t armor_color;

        PADDING(3);

        /** Index of the player in the machine table; 0xFF if not present */
        std::uint8_t machine_index;

        /** Status of the player; 0xFF if not present */
        std::uint8_t status;

        /** Team of the player; 0xFF if not present */
        std::uint8_t team;

        /** Index of the player in the player table; 0xFF if not present */
        std::uint8_t player_id;

        /**
         * Get the player structure from the players table
         * @return pointer to the player structure, or nullptr if not found
         */
        Player *get_player() const noexcept;
    };
    static_assert(sizeof(NetworkGameServerPlayer) == 0x20);

    /**
     * Machine connection info
     */
    struct NetworkGameServerMachineConnectionInfo {
        /** IP address */
        std::uint32_t ip_address;

        /** Port */
        std::uint16_t port;

        PADDING(0x2);

        /** Socket (???) */
        SOCKET *socket;

        /** Map this out later? */
        PADDING(0x144);
    };

    /**
     * Machine connection state
     */
    struct NetworkGameServerMachineConnection {
        /** Information about the machine's connection */
        NetworkGameServerMachineConnectionInfo **connection_info;

        /** Last packet timestamp */
        LARGE_INTEGER last_packet_time;
    };

    /**
     * Machine info - Retail server & client/Custom Edition client
     */
    struct NetworkGameServerMachine {
        /** State of machine connection */
        NetworkGameServerMachineConnection *connection_state;

        PADDING(0x8);

        /** Index in player table I think. Phasor says this is used for rcon. */
        std::uint16_t player_index;

        /** Machine status; 0 if unused, 1 is found during the handshake, 7 if currently playing the game */
        std::uint16_t unknown;

        PADDING(0x42);

        /** Key used in handshake */
        char key[10];

        /** Increases by 1 for each connection */
        std::uint32_t player_number;

        /** This seems to be set to the player name when the player leaves */
        wchar_t last_player_name[12];

        PADDING(0x8);

        /** IP:port of the player in ascii format */
        char ip_address[0x20];

        /** CD key hash in ascii format */
        char cd_key_hash[0x20];

        PADDING(0x2C);
    };
    static_assert(sizeof(NetworkGameServerMachine) == 0xEC);

    enum NetworkGameServerState: std::uint16_t {
        NETWORK_GAME_SERVER_STATE_INACTIVE,
        NETWORK_GAME_SERVER_STATE_IN_GAME,
        NETWORK_GAME_SERVER_STATE_GAME_RESULTS
    };

    /**
     * This is server information (players, machines, etc.)
     */
    struct NetworkGameServerInfo {
        PADDING(4); // unknown pointer

        /** State of the server */
        NetworkGameServerState state;

        PADDING(2); // some bit flags

        /** Name of the server */
        wchar_t server_name[66];

        /** Name of the currently loaded map */
        char map_name[128];

        /** Name of the gametype */
        wchar_t gametype[24];

        PADDING(0xA9); // 64 bytes extra bcuz custom edition

        PADDING(0x1);

        /** Maximum players in the server */
        std::uint8_t max_players;

        PADDING(0x2);

        /** Number of players */
        std::uint8_t player_count;

        PADDING(0x1);

        /** Stuff used with sv_players */
        NetworkGameServerPlayer players[16];

        PADDING(6);

        /** Number of games played */
        std::size_t games_played;

        PADDING(4);

        /** Machine list */
        NetworkGameServerMachine machines[16];

        PADDING(0x44);

        /** Password of the server */
        wchar_t password[16];

        PADDING(4);

        /**
         * Get the player struct by its handle
         * @param  player handle of the player
         * @return        pointer to the player or nullptr if not valid
         */
        NetworkGameServerPlayer *get_player(PlayerHandle player) noexcept;
    };

    /**
     * Get the network game server struct
     * 
     * @return pointer to the server info struct or nullptr if not possible
     */
    BALLTZE_API NetworkGameServerInfo *network_game_get_server() noexcept;

    /**
     * Return the server type.
     * 
     * @return Return the server type.
     */
    BALLTZE_API NetworkGameServerType network_game_get_server_type();

    /**
     * Return if we are in a network game as a client.
     * 
     * @return Return if the game is a client.
     */
    BALLTZE_API bool network_game_is_client();

    /**
     * Return if we are in a network game as a server.
     * 
     * @return Return if the game is a server.
     */
    BALLTZE_API bool network_game_is_server();

    /**
     * Return the gametype.
     * 
     * @return Return the gametype.
     */
    BALLTZE_API NetworkGameType network_game_get_current_game_type();

    /**
     * Get if team game
     * 
     * @return true if team
     */
    BALLTZE_API bool network_game_current_game_is_team();

    /**
     * Encodes a stateless message.
     * 
     * @param output_buffer The buffer to write the message to.
     * @param arg1 
     * @param type The type of message.
     * @param arg3
     * @param message The message to encode.
     * @param arg5
     * @param arg6
     * @param arg7
     * 
     * @return The size of the encoded message (to be confirmed).
     */
    BALLTZE_API uint32_t network_game_encode_message(void *output_buffer, int32_t arg1, NetworkGameMessageType type, uint32_t arg3, void* message, uint32_t arg5, uint32_t arg6, uint32_t arg7);

    /**
     * Encodes a stateless message.
     * 
     * @param output_buffer The buffer to write the message to.
     * @param type The type of message.
     * @param message The message to encode.
     * 
     * @return The size of the encoded message (to be confirmed).
     */
    BALLTZE_API uint32_t network_game_encode_message(void *output_buffer, NetworkGameMessageType type, void *message);

    /**
     * Decodes a stateless message.
     * 
     * @param destination The buffer to write the message to.
     * @param message_header The message header.
     * 
     * @return True if the message was decoded successfully.
     */
    BALLTZE_API bool network_game_decode_message(void *destination, NetworkGameMessages::MessageHeader *message_header);

    /**
     * As server, sends a network game message to all machines.
     * 
     * @param message The message to send.
     * @param message_size The size of the message.
     * @param ingame_only If the message should only be sent to players in game.
     * @param write_to_local_connection If the message should be written to the local connection.
     * @param flush_queue If the message should be flushed from the queue.
     * @param unbuffered 
     * @param buffer_priority 
     */
    BALLTZE_API void network_game_server_send_message_to_all_machines(void *message, std::uint32_t message_size, bool ingame_only, bool write_to_local_connection, bool flush_queue, bool unbuffered, std::int32_t buffer_priority);

    /**
     * As server, sends a network game message to a given client machine.
     * 
     * @param machine_id The id of the client machine to send the message to.
     * @param message The message to send.
     * @param message_size The size of the message.
     * @param ingame_only If the message should only be sent to players in game.
     * @param write_to_local_connection If the message should be written to the local connection.
     * @param flush_queue If the message should be flushed from the queue.
     * @param unbuffered
     * @param buffer_priority
     */
    BALLTZE_API void network_game_server_send_message_to_machine(std::int8_t machine_id, void *message, std::uint32_t message_size, bool ingame_only, bool write_to_local_connection, bool flush_queue, bool unbuffered, std::int32_t buffer_priority);

    /**
     * As server, sends a RCON response message to a given client machine.
     * 
     * @param machine_id    The id of the client machine to send the message to.
     * @param message       The message to send.
     */
    BALLTZE_API void network_game_server_send_rcon_message(std::int8_t machine_id, const char *message);

    /**
     * As server, sends a RCON response message to a given client machine.
     * 
     * @param machine_id    The id of the client machine to send the message to.
     * @param format        String format (passed to snprintf).
     * @param args          Additional arguments to pass.
     */
    template<typename... Args> void network_game_server_send_rcon_message(std::int8_t machine_id, const char *format, Args... args) {
        char message[128];
        std::snprintf(message, sizeof(message), format, args...);
        network_game_server_send_rcon_message(machine_id, message);
    }

    /**
     * Send a chat message
     * 
     * @param channel channel to send on
     * @param message message to send
     */
    BALLTZE_API void network_game_client_send_chat_message(int channel, const wchar_t *message);

    /**
     * Send a chat message
     * 
     * @param channel channel to send on
     * @param message message to send
     */
    BALLTZE_API void network_game_client_send_chat_message(int channel, const char *message);

    /**
     * As client, send a message to the server 
     * 
     * @param message message to send
     * @param message_size size of the message
     */
    BALLTZE_API void network_game_client_send_message(void *message, size_t message_size);

    /**
     * Get the local rcon id
     * 
     * @return the local rcon id
     */
    BALLTZE_API std::uint32_t network_game_get_local_rcon_id() noexcept;
}

#endif
