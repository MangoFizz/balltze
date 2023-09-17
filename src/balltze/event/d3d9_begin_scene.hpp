// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__EVENTS__D3D9_BEGIN_SCENE_HPP
#define BALLTZE__EVENTS__D3D9_BEGIN_SCENE_HPP

#include <d3d9.h>
#include <balltze/event.hpp>

namespace Balltze::Event {
    struct D3D9BeginSceneEventArguments {
        IDirect3DDevice9 *const device;

        D3D9BeginSceneEventArguments(IDirect3DDevice9 *const device) : device(device) {}
    };

    class D3D9BeginSceneEvent : public EventData<D3D9BeginSceneEvent> {
    public:
        D3D9BeginSceneEventArguments args;

        bool cancellable() const {
            return false;
        }

        D3D9BeginSceneEvent(EventTime time, D3D9BeginSceneEventArguments args) : EventData(time), args(args) {}
    };
}

#endif
