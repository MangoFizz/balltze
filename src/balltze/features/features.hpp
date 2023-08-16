// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__FEATURES__FEATURES_HPP
#define BALLTZE__FEATURES__FEATURES_HPP

namespace Balltze::Features {
    /**
     * Set up the HUD button prompts
     */
    void set_up_hud_button_prompts();

    /**
     * Set up the gamepad pause button
     */
    void set_up_gamepad_pause_button();

    /**
     * Set up loading maps outside of the maps directory
     */
    void set_up_tag_data_importing();

    /**
     * Set up sound subtitles
     */
    void set_up_sound_subtitles() noexcept;

    /**
     * Set up the loading screen
     */
    void set_up_loading_screen() noexcept;

    /**
     * Set up all features.
     */
    inline void set_up_features() {
        try {
            set_up_gamepad_pause_button();
            set_up_hud_button_prompts();

#ifdef BALLTZE_ENABLE_EXPERIMENTAL
            set_up_sound_subtitles();
            set_up_tag_data_importing();
            set_up_loading_screen();
#endif
        }
        catch(...) {
            throw;
        }
    }
}

#endif
