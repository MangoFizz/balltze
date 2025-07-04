// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/api.hpp>
#include <balltze/legacy_api/event.hpp>
#include <balltze/hook.hpp>
#include "../../logger.hpp"

namespace Balltze::LegacyApi::Event {
    static void frame_event_before_dispatcher() {
        FrameEvent frame_event(EVENT_TIME_BEFORE);
        frame_event.dispatch();
    }

    static void frame_event_after_dispatcher() {
        FrameEvent frame_event(EVENT_TIME_AFTER);
        frame_event.dispatch();
    }

    template<>
    void EventHandler<FrameEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        if(get_balltze_side() != BALLTZE_SIDE_CLIENT) {
            return;
        }

        auto *frame_event_sig = Memory::get_signature("on_frame");
        if(!frame_event_sig) {
            throw std::runtime_error("Could not find signature for frame event");
        }

        // Workaround for Chimera hook (NEEDS TO BE FIXED)
        std::byte *ptr = Memory::follow_32bit_jump(frame_event_sig->data()) + 23;
        auto *tick_event_after_chimera_hook = Memory::hook_function(ptr, frame_event_after_dispatcher);

        try {
            auto *tick_event_hook = Memory::hook_function(frame_event_sig->data(), frame_event_before_dispatcher);
        }
        catch(std::runtime_error &e) {
            throw std::runtime_error("Could not hook frame event: " + std::string(e.what()));
        }
    }
}