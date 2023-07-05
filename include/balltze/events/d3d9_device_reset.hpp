// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__EVENTS__D3D9_DEVICE_RESET_HPP
#define BALLTZE_API__EVENTS__D3D9_DEVICE_RESET_HPP

#include <d3d9.h>
#include "../event.hpp"

namespace Balltze::Event {
    struct D3D9DeviceResetEventArguments {
        IDirect3DDevice9 *const device;
        D3DPRESENT_PARAMETERS *present_parameters;


        D3D9DeviceResetEventArguments(IDirect3DDevice9 *const device, D3DPRESENT_PARAMETERS *params) : device(device), present_parameters(params) {}
    };

    class D3D9DeviceResetEvent : public EventData<D3D9DeviceResetEvent> {
    public:
        D3D9DeviceResetEventArguments args;

        static bool cancellable() {
            return false;
        }

        D3D9DeviceResetEvent(EventTime time, D3D9DeviceResetEventArguments args) : EventData(time), args(args) {}
    };
}

#endif
