// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/event.hpp>
#include <balltze/hook.hpp>

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

    template <>
    void EventHandler<MapFileLoadEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        auto *load_map_path_sig = Memory::get_signature("map_load_path");
        auto *load_map_path_hook = Memory::hook_function(load_map_path_sig->data(), map_loading_asm);
    }
}
