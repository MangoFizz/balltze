// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__ENGINE__CORE_HPP
#define BALLTZE_API__ENGINE__CORE_HPP

#include <string>
#include <cstdio>
#include <filesystem>
#include <windows.h>
#include "data_types.hpp"
#include "../api.hpp"

namespace Balltze::Engine {
    enum EngineEdition {
        ENGINE_TYPE_CUSTOM_EDITION,
        ENGINE_TYPE_RETAIL,
        ENGINE_TYPE_DEMO
    };

    #define CONSOLE_COLOR_ERROR Balltze::Engine::ColorARGB{1.0, 0.0, 0.0, 1.0}

    /**
     * Print a message of a color in the console
     * @param color   Color to use in the message
     * @param message Message to use
     */
    BALLTZE_API void console_print(std::string message, ColorARGB color = {1.0f, 1.0f, 1.0f, 1.0f}) noexcept;

    /**
     * Print a message of a color in the console
     * @param color  Color to use in the message
     * @param format String format (passed to snprintf)
     * @param args   Additional arguments to pass
     */
    template<typename... Args> void console_printf(const ColorARGB &color, const char *format, Args... args) noexcept {
        char message[256];
        std::snprintf(message, sizeof(message), format, args...);
        console_print(message, color);
    }

    /**
     * Print a message in the console
     * @param format String format (passed to snprintf)
     * @param args   Additional arguments to pass
     */
    template<typename... Args> void console_printf(const char *format, Args... args) noexcept {
        console_printf(ColorARGB{1.0, 1.0, 1.0, 1.0}, format, args...);
    }

    /**
     * Get the Halo profile path
     * @return ABSOLUTE path to the Halo profile folder
     */
    BALLTZE_API std::filesystem::path get_path();

    /**
     * Returns the number of ticks since the engine started.
     */
    BALLTZE_API std::size_t get_tick_count() noexcept;

    /**
     * Get the game engine used
     */
    BALLTZE_API EngineEdition get_engine_edition();
}

#endif
