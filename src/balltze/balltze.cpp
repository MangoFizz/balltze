// SPDX-License-Identifier: GPL-3.0-only

#include <windows.h>
#include <balltze/legacy_api/event.hpp>
#include <balltze/legacy_api/engine/rasterizer.hpp>
#include <balltze/api.hpp>
#include <balltze/logger.hpp>
#include <balltze/utils.hpp>
#include <impl/debug/debug_symbols.h>
#include <ringworld.h>
#include "events/events.hpp"
#include "features/features.hpp"
#include "legacy_api/event/event.hpp"
#include "memory/memory.hpp"
#include "legacy_api/plugins/loader.hpp"
#include "plugins/loader.hpp"
#include "command/command.hpp"
#include "config/config.hpp"

namespace Balltze {
    using namespace LegacyApi::Event;

    Logger logger("Balltze");
    
    static BalltzeSide balltze_side;

    static void initialize_balltze() noexcept {
        logger.mute_ingame(true);
        try {
            logger.set_file(Config::get_balltze_directory() / "balltze.log", false);
        }
        catch(std::runtime_error &e) {
            logger.error("failed to set log file: {}", e.what());
        }
        logger.info << logger.endl;

        try {
            balltze_side = Memory::find_signatures();
            if(balltze_side == BALLTZE_SIDE_CLIENT) {
                logger.info("loading client...");

                debug_symbols_initialize();
                set_up_ringworld_hooks(RW_PLATFORM_GAME);

                LegacyApi::Event::set_up_events();
                LegacyApi::Plugins::set_up_plugins();
                
                Events::set_up_events_handlers();
                Features::set_up_features();
                Plugins::set_up_plugins_loader();
                
                set_up_commands();
                load_commands_settings();
            }
            else if(balltze_side == BALLTZE_SIDE_DEDICATED_SERVER) {
                logger.info("loading dedicated server...");

                set_up_ringworld_hooks(RW_PLATFORM_DEDICATED_SERVER);

                LegacyApi::Event::set_up_events();
                Features::set_up_features();
                LegacyApi::Plugins::set_up_plugins();
                set_up_commands();
                load_commands_settings();
            }
            else {
                logger.fatal("failed to detect engine type");
                std::exit(EXIT_FAILURE);
            }
            logger.info("initialized successfully!");
        }
        catch(std::runtime_error &e) {
            logger.fatal("failed to initialize: {}", e.what());
            std::terminate();
        }

        register_command("console_debug", "debug", "Sets whenever to print log messages to the in-game console.", "[enable: boolean]", [](int arg_count, const char **args) -> bool {
            if(arg_count == 1) {
                bool new_setting = STR_TO_BOOL(args[0]);
                logger.mute_ingame(!new_setting);
            }
            else {
                logger.info("console_debug: {}", logger.mute_ingame());
            }
            return true;
        }, true, 0, 1);
    }

    BalltzeSide get_balltze_side() noexcept {
        return balltze_side;
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
