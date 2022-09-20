// SPDX-License-Identifier: GPL-3.0-only

#include <windows.h>
#include <balltze/engine/window.hpp>
#include <balltze/output/message_box.hpp>

namespace Balltze {
    void message_box(const char *message) noexcept {
        HWND window_handle;
        try {
            auto *window_globals = Engine::WindowGlobals::get();
            window_handle = window_globals->hWnd;
        }
        catch(...) {
            window_handle = nullptr;
        }
        MessageBoxA(window_handle, message, "Balltze", MB_OK);
    }
}