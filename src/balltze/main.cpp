// SPDX-License-Identifier: GPL-3.0-only

#include <windows.h>
#include "balltze.hpp"

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    switch(fdwReason) {
        case DLL_PROCESS_ATTACH:
            new Balltze::Balltze();
            break;

        default:
            break;
    }
    return TRUE;
}
