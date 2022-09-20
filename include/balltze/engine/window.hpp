// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__ENGINE__WINDOW_HPP
#define BALLTZE__ENGINE__WINDOW_HPP

#include <windows.h>

namespace Balltze::Engine {
    /**
     * Basic application window information.
     * Struct from: https://github.com/surrealwaffle/chimera/tree/dearimgui_integration
     */
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
        
        /**
         * Get the window globals.
         * @return      Reference to the window globals.
         * @exception   If window globals signature was not found... yet
         */
        static WindowGlobals *get();
    };
    static_assert(sizeof(WindowGlobals) == 0x18);
}

#endif