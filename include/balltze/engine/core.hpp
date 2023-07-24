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

    #define CONSOLE_COLOR_ERROR Balltze::Engine::ColorARGB{1.0, 0.0, 0.0, 1.0}

    /**
     * Display a message of a color
     * @param color   Color to use in the message
     * @param message Message to use
     */
    BALLTZE_API void console_print(std::string message, ColorARGB color = {1.0f, 1.0f, 1.0f, 1.0f}) noexcept;

    /**
     * Display a message of a color
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
     * Display a message (white)
     * @param format String format (passed to snprintf)
     * @param args   Additional arguments to pass
     */
    template<typename... Args> void console_printf(const char *format, Args... args) noexcept {
        console_printf(ColorARGB{1.0, 1.0, 1.0, 1.0}, format, args...);
    }

    /**
     * Get the Halo profile path
     * @return Halo profile path
     */
    BALLTZE_API std::filesystem::path get_path();

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

    /**
     * This is the state the camera is in.
     */
    enum CameraType : std::uint16_t {
        /** First person view; rendering the player's weapon */
        CAMERA_FIRST_PERSON = 0,

        /** Vehicle view (third person); can be controlled by the player */
        CAMERA_VEHICLE,

        /** Cinematic camera */
        CAMERA_CINEMATIC,

        /** Devcam or death cam */
        CAMERA_DEBUG
    };

    /**
     * Get the camera type.
     * @return Return the camera type.
     */
    BALLTZE_API CameraType get_camera_type() noexcept;

    struct CameraData {
        /** This is the position of the camera. */
        Point3D position;

        /** More stuff */
        std::uint32_t unknown[5];

        /** Orientation/rotation of the camera. */
        Point3D orientation[2];

        /** FOV (radians) */
        float fov;
    };
    static_assert(sizeof(CameraData) == 0x3C);

    /**
     * Get the camera data.
     * @return Return a reference to the camera data.
     */
    BALLTZE_API CameraData &get_camera_data() noexcept;
}

#endif
