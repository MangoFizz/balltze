// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_LEGACY_API__EVENTS__OBJECT_DAMAGE_HPP
#define BALLTZE_LEGACY_API__EVENTS__OBJECT_DAMAGE_HPP

#include <cstdint>
#include <string>
#include "../event.hpp"

namespace Balltze::LegacyApi::Event {
    struct ObjectDamageEventContext {
        const LegacyApi::Engine::ObjectHandle object;
        const LegacyApi::Engine::TagHandle damage_effect;
        const float multiplier;
        const LegacyApi::Engine::PlayerHandle causer_player;
        const LegacyApi::Engine::ObjectHandle causer_object;

        ObjectDamageEventContext(LegacyApi::Engine::ObjectHandle object, LegacyApi::Engine::TagHandle damage_effect, float multiplier, LegacyApi::Engine::PlayerHandle causer_player, LegacyApi::Engine::ObjectHandle causer_object) : object(object), damage_effect(damage_effect), multiplier(multiplier), causer_player(causer_player), causer_object(causer_object) {}
    };

    class ObjectDamageEvent : public EventData<ObjectDamageEvent> {
    public:
        ObjectDamageEventContext context;

        bool cancellable() const {
            return true;
        }

        ObjectDamageEvent(EventTime time, const ObjectDamageEventContext &context) : EventData(time), context(context) {}
    };
}

#endif
