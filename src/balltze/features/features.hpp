// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__FEATURES__FEATURES_HPP
#define BALLTZE__FEATURES__FEATURES_HPP

namespace Balltze {
    namespace Features {
        /**
         * Set up the HUD button prompts
         */
        void set_up_hud_button_prompts();
    }

    /**
     * Set up all features.
     */
    inline void set_up_features() {
        try {
            Features::set_up_hud_button_prompts();
        }
        catch(...) {
            throw;
        }
    }
}

#endif
