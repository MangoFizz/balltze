// SPDX-License-Identifier: GPL-3.0-only

#include <windows.h>
#include <balltze/engine/core.hpp>
#include <balltze/output.hpp>

namespace Balltze {
    void show_message_box_raw(unsigned int type, const char *message) noexcept {
        HWND window_handle;
        try {
            auto *window_globals = Engine::get_window_globals();
            window_handle = window_globals->hWnd;
        }
        catch(...) {
            window_handle = nullptr;
        }
        MessageBoxA(window_handle, message, "Balltze", type);
    }
}