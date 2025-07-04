// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__EVENTS__D3D9_HPP
#define BALLTZE_API__EVENTS__D3D9_HPP

#include <d3d9.h>
#include "../event.hpp"

namespace Balltze::LegacyApi::Event {
    struct D3D9BeginSceneEventContext {
        IDirect3DDevice9 *const device;

        D3D9BeginSceneEventContext(IDirect3DDevice9 *const device) : device(device) {}
    };

    class D3D9BeginSceneEvent : public EventData<D3D9BeginSceneEvent> {
    public:
        D3D9BeginSceneEventContext context;

        bool cancellable() const {
            return false;
        }

        D3D9BeginSceneEvent(EventTime time, D3D9BeginSceneEventContext context) : EventData(time), context(context) {}
    };
    
    struct D3D9EndSceneEventContext {
        IDirect3DDevice9 *const device;

        D3D9EndSceneEventContext(IDirect3DDevice9 *const device) : device(device) {}
    };

    class D3D9EndSceneEvent : public EventData<D3D9EndSceneEvent> {
    public:
        D3D9EndSceneEventContext context;

        bool cancellable() const {
            return false;
        }

        D3D9EndSceneEvent(EventTime time, D3D9EndSceneEventContext context) : EventData(time), context(context) {}
    };

    struct D3D9DeviceResetEventContext {
        IDirect3DDevice9 *const device;
        D3DPRESENT_PARAMETERS *present_parameters;


        D3D9DeviceResetEventContext(IDirect3DDevice9 *const device, D3DPRESENT_PARAMETERS *params) : device(device), present_parameters(params) {}
    };

    class D3D9DeviceResetEvent : public EventData<D3D9DeviceResetEvent> {
    public:
        D3D9DeviceResetEventContext context;

        bool cancellable() const {
            return false;
        }

        D3D9DeviceResetEvent(EventTime time, D3D9DeviceResetEventContext context) : EventData(time), context(context) {}
    };
}

#endif
