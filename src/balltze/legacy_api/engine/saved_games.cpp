// SPDX-License-Identifier: GPL-3.0-only

#include <cstdint>
#include <impl/interface/player_ui.h>
#include <impl/saved_games/player_profile.h>

namespace Balltze::LegacyApi::Engine {
    void saved_games_reload_player_profile() {
        uint32_t profile_id = ::saved_games_get_player_profile_id(0);
        PlayerProfile *profile = ::saved_games_get_player_profile(0);
        ::player_ui_set_active_player_profile(profile_id, 0, profile);
    }

    void saved_games_read_player_profile() {
        PlayerProfile profile;
        uint32_t profile_id = ::saved_games_get_player_profile_id(0);
        ::saved_games_read_player_profile(profile_id, &profile);
        ::player_ui_set_active_player_profile(profile_id, 0, &profile);
    }

    void saved_games_save_player_profile() {
        uint32_t profile_id = ::saved_games_get_player_profile_id(0);
        PlayerProfile *profile = ::saved_games_get_player_profile(0);
        ::saved_games_write_player_profile(profile_id, profile);
    }
}
