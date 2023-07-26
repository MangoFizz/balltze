// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__EVENTS__CAMERA_HPP
#define BALLTZE_API__EVENTS__CAMERA_HPP

#include "../engine/core.hpp"
#include "../event.hpp"

namespace Balltze::Event {
    struct CameraEventArgs {
        const Engine::CameraData *camera;
        const Engine::CameraType type;

        CameraEventArgs(const Engine::CameraData *camera, const Engine::CameraType type) : camera(camera), type(type) {}
    };

    class CameraEvent : public EventData<CameraEvent> {
    public:
        CameraEventArgs args;

        bool cancellable() const {
            return false;
        }

        CameraEvent(EventTime time, const CameraEventArgs &args) : EventData(time), args(args) {}
    };
}

#endif
