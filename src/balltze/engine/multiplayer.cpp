// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/memory.hpp>
#include <balltze/engine/game_state.hpp>
#include <balltze/engine/multiplayer.hpp>

namespace Balltze::Engine {
    ServerType get_server_type() {
        static auto *server_type = *reinterpret_cast<ServerType **>(Memory::get_signature("server_type")->data() + 3);
        return *server_type;
    }

    Gametype get_current_game_type() {
        static auto *gametype = *reinterpret_cast<Gametype **>(Memory::get_signature("current_gametype")->data() + 2);
        return *gametype;
    }

    bool current_game_is_team() {
        static auto *is_team = *reinterpret_cast<std::uint8_t **>(Memory::get_signature("current_gametype")->data() + 2) + 4;
        return *is_team;
    }

    extern "C" {
        void chat_out_asm(int channel, const wchar_t *message);

        // std::uint32_t chat_get_local_rcon_id() noexcept {
        //     auto *list = ServerInfoPlayerList::get_server_info_player_list();
        //     auto *player = PlayerTable::get_player_table().get_client_player();
        //     if(!list || !player) {
        //         return 0xFFFFFFFF;
        //     }
        //     auto *local_player = list->get_player(player->get_full_id());
        //     if(local_player) {
        //         return local_player - list->players;
        //     }
        //     else {
        //         return 0xFFFFFFFF;
        //     }
        // }
    }

    void chat_out(int channel, const wchar_t *message) {
        if(!message) {
            return;
        }
        chat_out_asm(channel, message);
    }
    
    void chat_out(int channel, const char *message) {
        if(!message) {
            return;
        }

        wchar_t message_to_send[256] = {};
        MultiByteToWideChar(CP_UTF8, 0, message, -1, message_to_send, sizeof(message_to_send) / sizeof(*message_to_send));
        chat_out_asm(channel, message_to_send);
    }

    Player *ServerInfoPlayer::get_player_table_player() const noexcept {
        auto &player_table = get_player_table();
        if(this->player_id >= player_table.current_size) {
            return nullptr;
        }
        return player_table.first_element + this->player_id;
    }

    ServerInfoPlayer *ServerInfoPlayerList::get_player(PlayerHandle player) noexcept {
        for(auto &p : this->players) {
            if(p.player_id == player.index) {
                return &p;
            }
        }
        return nullptr;
    }

    ServerInfoPlayerList *ServerInfoPlayerList::get_server_info_player_list() noexcept {
        auto *info = ServerInfo::get_server_info();
        if(!info) {
            return nullptr;
        }

        static std::uint32_t offset = *reinterpret_cast<std::uint32_t *>(Memory::get_signature("server_info_player_list_offset")->data()) - 1;
        return reinterpret_cast<ServerInfoPlayerList *>(reinterpret_cast<std::byte *>(info) + offset);
    }

    ServerInfo *ServerInfo::get_server_info() noexcept {
        #define RETURN_TABLE_FOR_SIGNATURE(SIGNATURE) { \
            static ServerInfo *table = nullptr; \
            if(!table) { \
                table = reinterpret_cast<ServerInfo *>(*reinterpret_cast<std::byte **>(Memory::get_signature(SIGNATURE)->data()) - 8); \
            } \
            return table; \
        }

        switch(get_server_type()) {
            // In a server (hosting)
            case ServerType::SERVER_LOCAL:
                RETURN_TABLE_FOR_SIGNATURE("server_info_host")

            // In a server (as client)
            case ServerType::SERVER_DEDICATED:
                RETURN_TABLE_FOR_SIGNATURE("server_info_client")

            // Not in a server
            default:
                return nullptr;
        }
    }
}
