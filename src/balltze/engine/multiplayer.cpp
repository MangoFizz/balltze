// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/memory.hpp>
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
}
