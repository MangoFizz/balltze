// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/event.hpp>
#include <balltze/hook.hpp>
#include <balltze/command.hpp>
#include "../logger.hpp"

namespace Balltze::Event {
    extern "C" {
        void map_load_before_event();
        void map_load_after_event();

        void dispatch_map_load_event_before(const char *map_name) {
            MapLoadEventArgs args;
            args.name = map_name;
            MapLoadEvent event(EVENT_TIME_BEFORE, args);
            event.dispatch();
        }

        void dispatch_map_load_event_after(const char *map_name) {
            MapLoadEventArgs args;
            args.name = map_name;
            MapLoadEvent event(EVENT_TIME_AFTER, args);
            event.dispatch();
        }
    }

    static bool debug_map_load_event(int arg_count, const char **args) {
        static std::optional<Event::EventListenerHandle<MapLoadEvent>> handle;
        if(arg_count == 1) {
            bool new_setting = STR_TO_BOOL(args[0]);
            if(new_setting) {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
                handle = Event::MapLoadEvent::subscribe_const([](MapLoadEvent const &event) {
                    auto &arguments = event.args;
                    auto time = event_time_to_string(event.time);
                    logger.debug("Map load event ({}): map name: {}", time, arguments.name);
                });
            }
            else {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
            }
        }
        logger.info("debug_map_load_event: {}", handle.has_value());
        return true;
    }

    template<>
    void EventHandler<MapLoadEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        // This is dispatched by the loading screen thread
        // auto *load_map_function_multiplayer_callload_map_path_sig = Memory::get_signature("load_map_function_multiplayer_call");
        // auto *load_map_function_singleplayer_call_sig = Memory::get_signature("load_map_function_singleplayer_call");
        // if(!load_map_function_multiplayer_callload_map_path_sig || !load_map_function_singleplayer_call_sig) {
        //     logger.error("Failed to find map load event signatures");
        //     return;
        // }

        // Memory::hook_function(load_map_function_multiplayer_callload_map_path_sig->data(), map_load_before_event, map_load_after_event);
        // Memory::hook_function(load_map_function_singleplayer_call_sig->data(), map_load_before_event, map_load_after_event);

        // Register debug command
        register_command("debug_map_load_event", "debug", "Debug map load event", "[enable]", debug_map_load_event, true, 0, 1);
    }
}
