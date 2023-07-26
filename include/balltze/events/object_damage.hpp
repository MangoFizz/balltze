// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__EVENTS__OBJECT_DAMAGE_HPP
#define BALLTZE_API__EVENTS__OBJECT_DAMAGE_HPP

#include <cstdint>
#include <string>
#include "../event.hpp"

namespace Balltze::Event {
    struct ObjectDamageEventArgs {
        Engine::ObjectHandle object;
        Engine::TagHandle damage_effect;
        float multiplier;
        Engine::PlayerHandle causer_player;
        Engine::ObjectHandle causer_object;

        ObjectDamageEventArgs(Engine::ObjectHandle object, Engine::TagHandle damage_effect, float multiplier, Engine::PlayerHandle causer_player, Engine::ObjectHandle causer_object) : object(object), damage_effect(damage_effect), multiplier(multiplier), causer_player(causer_player), causer_object(causer_object) {}
    };

    class ObjectDamageEvent : public EventData<ObjectDamageEvent> {
    public:
        ObjectDamageEventArgs args;

        bool cancellable() const {
            return true;
        }

        ObjectDamageEvent(EventTime time, const ObjectDamageEventArgs &args) : EventData(time), args(args) {}
    };
}

#endif
