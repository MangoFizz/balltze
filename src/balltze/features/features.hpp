// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__FEATURES__FEATURES_HPP
#define BALLTZE__FEATURES__FEATURES_HPP

#include <balltze/api.hpp>
#include <balltze/features.hpp>

namespace Balltze::Features {
    void set_up_hud_button_prompts();
    void set_up_gamepad_pause_button();
    void set_up_tag_data_importing();
    void set_up_loading_screen() noexcept;
    void set_up_map_data_read_warden() noexcept;
    void set_up_map_textures_preloading() noexcept;
    void set_up_extended_limits();
    void set_up_set_console_key_binding_command() noexcept;

    inline void set_up_features() {
        try {
            switch(get_balltze_side()) {
                case BALLTZE_SIDE_CLIENT: {
                    set_up_gamepad_pause_button();
                    set_up_hud_button_prompts();
                    set_up_map_data_read_warden();
                    set_up_extended_limits();
                    set_up_tag_data_importing();
                    set_up_loading_screen();
                    set_up_map_textures_preloading();
                    set_up_set_console_key_binding_command();
                    break;
                }
            }
        }
        catch(...) {
            throw;
        }
    }
}

#endif
