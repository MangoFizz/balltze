// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__ENGINE___SAVED_GAMES_HPP
#define BALLTZE__ENGINE___SAVED_GAMES_HPP

#include "../api.hpp"

namespace Balltze::Engine {
    /**
     * Apply the current player profile settings to the game.
     */
    BALLTZE_API void saved_games_reload_player_profile();

    /**
     * Read the player profile data from the saved games directory.
     */
    BALLTZE_API void saved_games_read_player_profile();

    /**
     * Write the player profile data to the saved games directory.
     */
    BALLTZE_API void saved_games_save_player_profile();
}


#endif
