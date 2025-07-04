// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__MESSAGE_BOX_HPP
#define BALLTZE_API__MESSAGE_BOX_HPP

#include <windows.h>
#include <intrin.h>
#include <cstdio>
#include "legacy_api/engine/core.hpp"
#include "api.hpp"

namespace Balltze {
    /**
     * Create a dialog with a given message
     * @param message   Message to display
     */
    BALLTZE_API void show_message_box_raw(unsigned int type, const char *message) noexcept;

    /**
     * Create a dialog with a given message
     * @param format    Message format
     * @param args      Format arguments
     */
    template<typename... Args> void show_message_box(const char *format, Args... args) noexcept {
        char message[256];
        std::snprintf(message, sizeof(message), format, args...);
        show_message_box_raw(MB_OK, message);
    }

    /**
     * Create a dialog with a given message
     * @param format    Message format
     * @param args      Format arguments
     */
    template<typename... Args> void show_error_box(const char *format, Args... args) noexcept {
        char message[256];
        std::snprintf(message, sizeof(message), format, args...);
        show_message_box_raw(MB_OK | MB_ICONERROR, message);
    }

    /**
     * Get the current module handle
     * https://stackoverflow.com/a/557774
     */
    inline HMODULE get_current_module() {
        HMODULE hModule = NULL;
        GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR)get_current_module, &hModule);
        return hModule;
    }

    /**
     * Get the module handle of the caller
     * https://stackoverflow.com/a/557774
     */
    inline HMODULE get_caller_module() {
        auto return_address = reinterpret_cast<std::uintptr_t>(_ReturnAddress());
        HMODULE hModule = NULL;
        GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCSTR)return_address, &hModule);
        return hModule;
    }
}

#endif
