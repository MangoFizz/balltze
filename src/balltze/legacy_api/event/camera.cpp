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

    static bool debug_camera_event(int arg_count, const char **args) {
        static std::optional<LegacyApi::Event::EventListenerHandle<CameraEvent>> handle;
        if(arg_count == 1) {
            bool new_setting = STR_TO_BOOL(args[0]);
            if(new_setting) {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
                handle = LegacyApi::Event::CameraEvent::subscribe_const([](CameraEvent const &event) {
                    auto &context = event.context;
                    auto time = event_time_to_string(event.time);
                    auto type = std::to_string(static_cast<int>(context.type));
                    auto fov = std::to_string(context.camera->fov);
                    auto position = fmt::format("({}, {}, {})", context.camera->position.x, context.camera->position.y, context.camera->position.z);
                    auto orientation_1 = fmt::format("({}, {}, {})", context.camera->orientation[0].x, context.camera->orientation[0].y, context.camera->orientation[0].z);
                    auto orientation_2 = fmt::format("({}, {}, {})", context.camera->orientation[1].x, context.camera->orientation[1].y, context.camera->orientation[1].z);
                    logger.debug("Camera event ({}): camera type: {}, camera fov: {}, camera position: ({}), camera orientation: ({}) ({})", time, type, fov, position, orientation_1, orientation_2);
                });
            }
            else {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
            }
        }
        logger.info("debug_camera_event: {}", handle.has_value());
        return true;
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

        // Register debug command
        register_command("debug_camera_event", "debug", "Sets whenever to log camera event.", "[enable: boolean]", debug_camera_event, true, 0, 1);
    }
}
