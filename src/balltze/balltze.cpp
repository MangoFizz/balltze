// SPDX-License-Identifier: GPL-3.0-only

#include <windows.h>
#include <balltze/event.hpp>
#include <balltze/output.hpp>
#include "features/features.hpp"

namespace Balltze {
    namespace Memory {
        void find_signatures();
    }

    namespace Event {
        void set_up_events();
    }

    namespace PluginsLoader {
        void load_plugins();
    }

    using namespace Event;
    using namespace Memory;
    using namespace PluginsLoader;

    static EventListenerHandle<TickEvent> firstTickListener;

    static void first_tick(TickEvent const &context) noexcept {
        // show_message_box("[%s] Tick #%d in %d milliseconds", context.time == EVENT_TIME_BEFORE ? "Before" : "After", context.args.tick_count, context.args.delta_time_ms);
        if(context.time == EVENT_TIME_AFTER) {
            firstTickListener.remove();
        }
    }

    static void initialize_balltze() noexcept {
        try {
            find_signatures();
            set_up_events();
            set_up_features();
            load_plugins();
        }
        catch(std::runtime_error &e) {
            show_message_box("Balltze failed to initialize: %s", e.what());
            std::terminate();
        }
        firstTickListener = TickEvent::subscribe_const(first_tick, EVENT_PRIORITY_HIGHEST);
    }
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    switch(fdwReason) {
        case DLL_PROCESS_ATTACH:
            Balltze::initialize_balltze();
            break;

        default:
            break;
    }
    return TRUE;
}
