// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__EVENTS__D3D9_END_SCENE_HPP
#define BALLTZE_API__EVENTS__D3D9_END_SCENE_HPP

#include <d3d9.h>
#include "../event.hpp"

namespace Balltze::Event {
    struct D3D9EndSceneEventArguments {
        IDirect3DDevice9 *const device;

        D3D9EndSceneEventArguments(IDirect3DDevice9 *const device) : device(device) {}
    };

    class D3D9EndSceneEvent : public EventData<D3D9EndSceneEvent> {
    public:
        D3D9EndSceneEventArguments args;

        static bool cancellable() {
            return false;
        }

        D3D9EndSceneEvent(EventTime time, D3D9EndSceneEventArguments args) : EventData(time), args(args) {}
    };
}

#endif
