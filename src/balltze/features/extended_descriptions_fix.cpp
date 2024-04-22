// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/memory.hpp>
#include <balltze/hook.hpp>
#include "../logger.hpp"

namespace Balltze::Features {
    extern "C" {
        void multiplayer_screen_extended_screen_pic_index_1_fix();
    }

    static void set_up_multiplayer_screen_extended_screen_pic_index_1_fix() {
        static auto *sig = Memory::get_signature("multiplayer_screen_extended_description_pic_index_1");
        if(!sig) {
            logger.error("Failed to find signature for fixing extended descriptions");
            return;
        }

        try {
            Memory::hook_function(sig->data(), std::nullopt, multiplayer_screen_extended_screen_pic_index_1_fix, false);
        }
        catch(const std::exception &e) {
            logger.error("Failed to hook function for fixing extended descriptions: {}", e.what());
        }
    }

    void set_up_extended_decriptions_fix() {
        set_up_multiplayer_screen_extended_screen_pic_index_1_fix();
    }
}
