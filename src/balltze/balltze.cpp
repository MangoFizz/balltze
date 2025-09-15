// SPDX-License-Identifier: GPL-3.0-only

#include <windows.h>
#include <filesystem>
#include <balltze/api.hpp>
#include <balltze/logger.hpp>
#include <impl/debug/debug_symbols.h>
#include <impl/debug/log.h>
#include <impl/exception/exception.h>
#include <impl/terminal/terminal.h>
#include <ringworld.h>
#include "events/events.hpp"
#include "features/features.hpp"
#include "legacy_api/event/event.hpp"
#include "memory/memory.hpp"
#include "plugins/loader.hpp"
#include "command/command.hpp"
#include "command/ringworld_commands.hpp"
#include "config/config.hpp"
#include "version.hpp"

extern "C" LogLevel current_log_level;

namespace Balltze {
    using namespace LegacyApi::Event;

    Logger logger("Balltze");
    
    static BalltzeSide balltze_side;

    static void print_stacktrace_on_unhandled_exception() {
        CONTEXT context;
        RtlCaptureContext(&context);
        exception_print_stack_trace(&context);
    }

    static void initialize_balltze() noexcept {
        std::set_terminate(print_stacktrace_on_unhandled_exception);

        logger.mute_ingame(true); // mute this because ringworld terminal is not initialized yet
        logger.mute_debug(false);
        try {
            logger.set_file(Config::get_balltze_directory() / "balltze.log", false);
            logger.info << logger.endl;
        }
        catch(std::runtime_error &e) {
            logger.error("failed to set log file: {}", e.what());
        }

#ifdef DEBUG
        debug_symbols_initialize();
#else
        if(std::filesystem::exists("mods\\balltze.dll.debug")) {
            logger.debug("Debugging symbols found, enabling debug logs");
            current_log_level = LOG_DEBUG;
            logger.mute_debug(false);
            debug_symbols_initialize();
        }
#endif

        try {
            balltze_side = Memory::find_signatures();
            if(balltze_side == BALLTZE_SIDE_CLIENT) {
                logger.info("loading client...");
                set_up_ringworld_hooks(RW_PLATFORM_GAME);
            }
            else if(balltze_side == BALLTZE_SIDE_DEDICATED_SERVER) {
                logger.info("loading dedicated server...");
                set_up_ringworld_hooks(RW_PLATFORM_DEDICATED_SERVER);
            }
            else {
                logger.fatal("failed to detect engine type");
                std::exit(EXIT_FAILURE);
            }

            LegacyApi::Event::set_up_events();
            Events::set_up_events_handlers();
            set_up_commands();
            register_ringworld_commands();
            Features::set_up_features();
            Plugins::set_up_plugins_loader();

            logger.info("initialized successfully!");
        }
        catch(std::runtime_error &e) {
            logger.fatal("failed to initialize: {}", e.what());
            std::terminate();
        }

        CommandBuilder()
            .name("version")
            .category("misc")
            .help("Prints the current version of Balltze")
            .function([](const std::vector<std::string> &args) -> bool {
                auto version = balltze_version.to_string();
                terminal_info_printf("Balltze version %s", version.c_str());
                return true;
            })
            .can_call_from_console()
            .create();

        CommandBuilder()
            .name("terminal_debug")
            .category("debug")
            .help("Sets whether to print log messages to the in-game terminal.")
            .param(HSC_DATA_TYPE_BOOLEAN, "enable")
            .function([](const std::vector<std::string> &args) -> bool {
                bool new_setting = STR_TO_BOOL(args[0].c_str());
                logger.mute_ingame(!new_setting);
                return true;
            })
            .autosave()
            .can_call_from_console()
            .create();

        Config::load_commands_settings();
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
