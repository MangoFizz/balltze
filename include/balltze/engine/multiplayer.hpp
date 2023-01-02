// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__ENGINE__MULTIPLAYER_HPP
#define BALLTZE_API__ENGINE__MULTIPLAYER_HPP

#include <cstdint>
#include "../api.hpp"

namespace Balltze::Engine {
    enum ServerType : std::uint16_t {
        SERVER_NONE,
        SERVER_DEDICATED,
        SERVER_LOCAL
    };

    enum Gametype : std::uint16_t {
        GAMETYPE_CTF = 1,
        GAMETYPE_SLAYER,
        GAMETYPE_ODDBALL,
        GAMETYPE_KING,
        GAMETYPE_RACE
    };

    /**
     * Return the server type.
     * @return Return the server type.
     */
    BALLTZE_API ServerType get_server_type();

    /**
     * Return the gametype.
     * @return Return the gametype.
     */
    BALLTZE_API Gametype get_current_game_type();

    /**
     * Get if team game
     * @return true if team
     */
    BALLTZE_API bool current_game_is_team();
}

#endif
