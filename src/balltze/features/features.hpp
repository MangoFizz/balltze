// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__FEATURES__FEATURES_HPP
#define BALLTZE__FEATURES__FEATURES_HPP

namespace Balltze::Features {
    void set_up_hud_button_prompts();
    void set_up_gamepad_pause_button();
    void set_up_tag_data_importing();
    void set_up_sound_subtitles() noexcept;
    void set_up_loading_screen() noexcept;
    void set_up_map_data_read_warden() noexcept;
    void set_up_map_textures_preloading() noexcept;
    void set_up_extended_limits();

    inline void set_up_features() {
        try {
            set_up_gamepad_pause_button();
            set_up_hud_button_prompts();
            set_up_map_data_read_warden();
            set_up_extended_limits();

#ifdef BALLTZE_ENABLE_EXPERIMENTAL
            set_up_sound_subtitles();
            set_up_tag_data_importing();
            set_up_loading_screen();
            set_up_map_textures_preloading();
#endif
        }
        catch(...) {
            throw;
        }
    }
}

#endif
