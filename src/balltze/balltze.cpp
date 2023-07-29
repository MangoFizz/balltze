// SPDX-License-Identifier: GPL-3.0-only

#include <windows.h>
#include <balltze/event.hpp>
#include <balltze/logger.hpp>
#include <balltze/utils.hpp>
#include "features/features.hpp"
#include "event/event.hpp"
#include "memory/memory.hpp"
#include "output/draw_text.hpp"
#include "output/subtitles.hpp"
#include "plugins/loader.hpp"
#include "command/command.hpp"
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
            set_up_commands();
            set_up_commands_tab_completion();
            load_commands_settings();

            logger.info("initialized successfully!");
        }
        catch(std::runtime_error &e) {
            logger.fatal("failed to initialize: {}", e.what());
            std::terminate();
        }

        firstTickListener = TickEvent::subscribe_const(+[](TickEvent const &context) {
            logger.debug("First tick");

            // Initialize stuff
            set_up_text_hook();
            set_up_subtitles();

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
