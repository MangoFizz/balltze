// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/legacy_api/event.hpp>
#include <balltze/legacy_api/engine/game_state.hpp>
#include <balltze/hook.hpp>
#include <balltze/command.hpp>
#include "../../logger.hpp"

namespace Balltze::LegacyApi::Event {
    extern "C" {
        void object_damage_event();
        void *object_damage_function_address = nullptr;
        
        bool dispatch_object_damage_event_before(LegacyApi::Engine::ObjectHandle *object, LegacyApi::Engine::DamageObjectStructThing *damage_thing) {
            ObjectDamageEventContext args(*object, damage_thing->damage_tag_handle, damage_thing->multiplier, damage_thing->causer_player, damage_thing->causer_object);
            ObjectDamageEvent event(EVENT_TIME_BEFORE, args);
            event.dispatch();
            return event.cancelled();
        }

        void dispatch_object_damage_event_after(LegacyApi::Engine::ObjectHandle *object, LegacyApi::Engine::DamageObjectStructThing *damage_thing) {
            ObjectDamageEventContext args(*object, damage_thing->damage_tag_handle, damage_thing->multiplier, damage_thing->causer_player, damage_thing->causer_object);
            ObjectDamageEvent event(EVENT_TIME_AFTER, args);
            event.dispatch();
        }
    }

    template<>
    void EventHandler<ObjectDamageEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        auto *apply_damage_function_sig = Memory::get_signature("apply_damage_function");
        if(!apply_damage_function_sig) {
            throw std::runtime_error("Could not find signature for object damage event");
        }

        try {
            Memory::override_function(apply_damage_function_sig->data() + 6, object_damage_event, &object_damage_function_address);
        }
        catch(const std::runtime_error &e) {
            throw std::runtime_error("Could not hook object damage event: " + std::string(e.what()));
        }
    }
}
