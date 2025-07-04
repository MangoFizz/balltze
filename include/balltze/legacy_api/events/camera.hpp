// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__EVENTS__CAMERA_HPP
#define BALLTZE_API__EVENTS__CAMERA_HPP

#include "../engine/game_state.hpp"
#include "../event.hpp"

namespace Balltze::LegacyApi::Event {
    struct CameraEventContext {
        LegacyApi::Engine::CameraData *const camera;
        const LegacyApi::Engine::CameraType type;

        CameraEventContext(LegacyApi::Engine::CameraData *camera, const LegacyApi::Engine::CameraType type) : camera(camera), type(type) {}
    };

    class CameraEvent : public EventData<CameraEvent> {
    public:
        CameraEventContext context;

        bool cancellable() const {
            return false;
        }

        CameraEvent(EventTime time, const CameraEventContext &context) : EventData(time), context(context) {}
    };
}

#endif
