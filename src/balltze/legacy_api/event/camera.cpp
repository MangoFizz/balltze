// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/legacy_api/event.hpp>
#include <balltze/hook.hpp>
#include <balltze/command.hpp>
#include <balltze/api.hpp>
#include "../../logger.hpp"

namespace Balltze::LegacyApi::Event {
    static void camera_event_before_dispatcher() {
        CameraEventContext context(&LegacyApi::Engine::get_camera_data(), LegacyApi::Engine::get_camera_type());
        CameraEvent camera_event(EVENT_TIME_BEFORE, context);
        camera_event.dispatch();
    }

    static void camera_event_after_dispatcher() {
        CameraEventContext context(&LegacyApi::Engine::get_camera_data(), LegacyApi::Engine::get_camera_type());
        CameraEvent camera_event(EVENT_TIME_AFTER, context);
        camera_event.dispatch();
    }

    template<>
    void EventHandler<CameraEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        if(get_balltze_side() != BALLTZE_SIDE_CLIENT) {
            return;
        }

        auto *camera_data_read_sig = Memory::get_signature("camera_data_read");
        if(!camera_data_read_sig) {
            throw std::runtime_error("Could not find signature for camera event");
        }

        try {
            // Workaround for Chimera hook (NEEDS TO BE FIXED)
            std::byte *ptr = Memory::follow_32bit_jump(camera_data_read_sig->data()) + 9;
            auto *camera_data_read_chimera_hook = Memory::hook_function(ptr, camera_event_before_dispatcher, camera_event_after_dispatcher);
        }
        catch(const std::runtime_error &e) {
            throw std::runtime_error("Could not hook camera event: " + std::string(e.what()));
        }
    }
}
