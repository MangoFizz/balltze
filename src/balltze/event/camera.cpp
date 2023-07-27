// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/event.hpp>
#include <balltze/hook.hpp>
#include <balltze/command.hpp>
#include "../logger.hpp"

namespace Balltze::Event {
    static void camera_event_before_dispatcher() {
        CameraEventArgs args(&Engine::get_camera_data(), Engine::get_camera_type());
        CameraEvent camera_event(EVENT_TIME_BEFORE, args);
        camera_event.dispatch();
    }

    static void camera_event_after_dispatcher() {
        CameraEventArgs args(&Engine::get_camera_data(), Engine::get_camera_type());
        CameraEvent camera_event(EVENT_TIME_AFTER, args);
        camera_event.dispatch();
    }

    static bool debug_camera_event(int arg_count, const char **args) {
        static std::optional<Event::EventListenerHandle<CameraEvent>> handle;
        if(arg_count == 1) {
            bool new_setting = STR_TO_BOOL(args[0]);
            if(new_setting) {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
                handle = Event::CameraEvent::subscribe_const([](CameraEvent const &event) {
                    auto &arguments = event.args;
                    auto time = event_time_to_string(event.time);
                    auto type = std::to_string(static_cast<int>(arguments.type));
                    auto fov = std::to_string(arguments.camera->fov);
                    auto position = fmt::format("({}, {}, {})", arguments.camera->position.x, arguments.camera->position.y, arguments.camera->position.z);
                    auto orientation_1 = fmt::format("({}, {}, {})", arguments.camera->orientation[0].x, arguments.camera->orientation[0].y, arguments.camera->orientation[0].z);
                    auto orientation_2 = fmt::format("({}, {}, {})", arguments.camera->orientation[1].x, arguments.camera->orientation[1].y, arguments.camera->orientation[1].z);
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

    template <>
    void EventHandler<CameraEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

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
        register_command("debug_camera_event", "debug", "Debug camera event", "[enable]", debug_camera_event, true, 0, 1);
    }
}
