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

    static bool debug_object_damage_event(int arg_count, const char **args) {
        static std::optional<LegacyApi::Event::EventListenerHandle<ObjectDamageEvent>> handle;
        if(arg_count == 1) {
            bool new_setting = STR_TO_BOOL(args[0]);
            if(new_setting) {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
                handle = LegacyApi::Event::ObjectDamageEvent::subscribe_const([](ObjectDamageEvent const &event) {
                    auto &context = event.context;
                    auto time = event_time_to_string(event.time);
                    auto tag = LegacyApi::Engine::get_tag(context.damage_effect);
                    logger.debug("Object damage event ({}): object: {}, damage tag: {}, multiplier: {}, causer player: {}, causer object: {}", time, context.object.value, tag->path, context.multiplier, context.causer_player.value, context.causer_object.value);
                });
            }
            else {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
            }
        }
        logger.info("debug_object_damage_event: {}", handle.has_value());
        return true;
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

        // Register debug command
        register_command("debug_object_damage_event", "debug", "Sets whenever to log object damage event.", "[enable: boolean]", debug_object_damage_event, true, 0, 1);
    }
}
