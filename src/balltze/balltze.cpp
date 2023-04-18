// SPDX-License-Identifier: GPL-3.0-only

#include <windows.h>
#include <balltze/event.hpp>
#include <balltze/logger.hpp>
#include <balltze/utils.hpp>
#include "features/features.hpp"
#include "event/event.hpp"
#include "memory/memory.hpp"
#include "plugins/loader.hpp"

namespace Balltze {
    using namespace Event;
    
    Logger logger("Balltze");
    
    static EventListenerHandle<TickEvent> firstTickListener;

    static void first_tick(TickEvent const &context) noexcept {
        if(context.time == EVENT_TIME_AFTER) {
            logger.debug("First tick");
            firstTickListener.remove();
        }
    }

    static void initialize_balltze() noexcept {
        logger.mute_ingame(true);
        logger.info << logger.endl;

        try {
            Memory::find_signatures();
            Event::set_up_events();
            Features::set_up_features();
            Plugins::load_plugins();

            logger.info("initialized successfully!");
        }
        catch(std::runtime_error &e) {
            logger.fatal("failed to initialize: {}", e.what());
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
