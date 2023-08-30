// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/event.hpp>
#include <balltze/hook.hpp>
#include <balltze/command.hpp>
#include "../logger.hpp"

namespace Balltze::Event {
    extern "C" {
        void map_loading_asm();

        bool dispatch_map_file_load_event_before(char *map_path, const char *map_name) {
            MapFileLoadEventArgs args;
            args.map_name = map_name;
            args.map_path = map_path;
            MapFileLoadEvent event(EVENT_TIME_BEFORE, args);
            event.dispatch();
            return false;
        }
    }

    static bool debug_map_file_load_event(int arg_count, const char **args) {
        static std::optional<Event::EventListenerHandle<MapFileLoadEvent>> handle;
        if(arg_count == 1) {
            bool new_setting = STR_TO_BOOL(args[0]);
            if(new_setting) {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
                handle = Event::MapFileLoadEvent::subscribe_const([](MapFileLoadEvent const &event) {
                    auto &arguments = event.args;
                    auto time = event_time_to_string(event.time);
                    logger.debug("Map file load event ({}): map name: {}, map path: {}", time, arguments.map_name, arguments.map_path);
                });
            }
            else {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
            }
        }
        logger.info("debug_map_file_load_event: {}", handle.has_value());
        return true;
    }

    template <>
    void EventHandler<MapFileLoadEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        auto *load_map_path_sig = Memory::get_signature("map_load_path");
        auto *load_map_path_addr = Memory::follow_32bit_jump(load_map_path_sig->data()) + 14; // let Chimera overwrite the map path
        auto *load_map_path_hook = Memory::hook_function(load_map_path_addr, map_loading_asm);

        // Register debug command
        register_command("debug_map_file_load_event", "debug", "Debug map file load event", "[enable]", debug_map_file_load_event, true, 0, 1);
    }
}
