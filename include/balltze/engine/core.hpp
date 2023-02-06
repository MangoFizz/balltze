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
    struct WindowGlobals {
        /** A handle to the application instance. */
        HINSTANCE hInstance;

        /** Halo's main window handle. */
        HWND hWnd;

        /** possibly used for error dialog menus? */
        HWND hWndUnknown;

        /** 
         * wShow from GetStartupInfo(), if the STARTF_USESHOWWINDOW flag is set. 
         * Otherwise, takes on the value `SW_SHOWDEFAULT`.
         */ 
        int nCmdShow;

        /** The WindowProc callback function as registered with the window class. */
        WNDPROC lpfnWndProc;
        
        /** Halo's small icon resource. */
        HICON hIconSm;
    };
    static_assert(sizeof(WindowGlobals) == 0x18);

    struct Resolution {
        std::uint16_t height;
        std::uint16_t width;
    };

    enum EngineEdition {
        ENGINE_TYPE_CUSTOM_EDITION,
        ENGINE_TYPE_RETAIL,
        ENGINE_TYPE_DEMO
    };

    /**
     * Display a message of a color
     * @param color   Color to use in the message
     * @param message Message to use
     */
    BALLTZE_API void console_print(std::string message, ColorARGB color = {1.0f, 1.0f, 1.0f, 1.0f}) noexcept;

    /**
     * Get the Halo profile path
     * @return Halo profile path
     */
    BALLTZE_API std::filesystem::path get_path() noexcept;

    /**
     * Get the current resolution of Halo.
     * @return Return the current resolution of Halo.
     */
    BALLTZE_API Resolution &get_resolution() noexcept;

    /**
     * Returns the number of ticks since the engine started.
     */
    BALLTZE_API std::size_t get_tick_count() noexcept;

    /**
     * Get the game engine used
     */
    BALLTZE_API EngineEdition get_engine_edition();

    /**
     * Get the window globals.
     * @return      Reference to the window globals.
     * @exception   If window globals signature was not found... yet
     */
    BALLTZE_API WindowGlobals *get_window_globals();
}

#endif
