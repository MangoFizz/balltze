// SPDX-License-Identifier: GPL-3.0-only

#include <windows.h>
#include <balltze/event.hpp>
#include <balltze/logger.hpp>
#include <balltze/utils.hpp>
#include "features/features.hpp"
#include "event/event.hpp"
#include "memory/memory.hpp"
#include "output/draw_text.hpp"
#include "plugins/loader.hpp"
#include "config/config.hpp"

namespace Balltze {
    using namespace Event;
    
    Logger logger("Balltze");
    
    static EventListenerHandle<TickEvent> firstTickListener;

    static void initialize_balltze() noexcept {
        logger.mute_ingame(true);
        try {
            logger.set_file(Config::get_balltze_directory() / "balltze.log");
        }
        catch(std::runtime_error &e) {
            logger.error("failed to set log file: {}", e.what());
        }
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

        firstTickListener = TickEvent::subscribe_const(+[](TickEvent const &context) {
            logger.debug("First tick");

            // Initialize stuff
            setup_text_hook();

            firstTickListener.remove();
        }, EVENT_PRIORITY_HIGHEST);
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
