// SPDX-License-Identifier: GPL-3.0-only

#include <stack>
#include <utility>
#include <balltze/api.hpp>
#include <balltze/hook.hpp>
#include <balltze/command.hpp>
#include <balltze/events/sound_playback.hpp>
#include <balltze/engine/tag.hpp>
#include <balltze/engine/tag_definitions/sound.hpp>
#include "../logger.hpp"

namespace Balltze::Event {
    extern "C" {
        void sound_playback_event();
        void *sound_playback_function = nullptr;

        bool dispatch_sound_playback_event_before(Engine::TagDefinitions::SoundPermutation *permutation) {
            auto *tag = Engine::get_tag(permutation->sound_tag_handle_0);
            if(!tag) {
                logger.debug("Could not find tag for permutation {} in dispatch_sound_playback_event_before", permutation->name.string);
                return false;
            }
            auto *sound = reinterpret_cast<Engine::TagDefinitions::Sound *>(tag->data);
            SoundPlaybackEventContext args(sound, permutation);
            SoundPlaybackEvent sound_playback_event(EVENT_TIME_BEFORE, args);
            sound_playback_event.dispatch();
            return sound_playback_event.cancelled();
        }

        void dispatch_sound_playback_event_after(Engine::TagDefinitions::SoundPermutation *permutation) {
            auto *tag = Engine::get_tag(permutation->sound_tag_handle_0);
            if(!tag) {
                logger.debug("Could not find tag for permutation {} in dispatch_sound_playback_event_before", permutation->name.string);
                return;
            }
            auto *sound = reinterpret_cast<Engine::TagDefinitions::Sound *>(tag->data);
            SoundPlaybackEventContext args(sound, permutation);
            SoundPlaybackEvent sound_playback_event(EVENT_TIME_AFTER, args);
            sound_playback_event.dispatch();
        }
    }

    static bool debug_sound_playback_event(int arg_count, const char **args) {
        static std::optional<Event::EventListenerHandle<SoundPlaybackEvent>> handle;
        if(arg_count == 1) {
            bool new_setting = STR_TO_BOOL(args[0]);
            if(new_setting) {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
                handle = Event::SoundPlaybackEvent::subscribe_const([](SoundPlaybackEvent const &event) {
                    auto &context = event.context;
                    auto time = event_time_to_string(event.time);
                    auto tag = Engine::get_tag(context.permutation->sound_tag_handle_0);
                    logger.debug("Sound playback event ({}): sound name: {}, permutation name: {}", time, tag->path, context.permutation->name.string);
                });
            }
            else {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
            }
        }
        logger.info("debug_sound_playback_event: {}", handle.has_value());
        return true;
    }

    template<>
    void EventHandler<SoundPlaybackEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        if(get_balltze_side() != BALLTZE_SIDE_CLIENT) {
            return;
        }

        auto *enqueue_sound_permutation_function_sig = Memory::get_signature("enqueue_sound_function");
        if(!enqueue_sound_permutation_function_sig) {
            throw std::runtime_error("Could not find signatures for sound playback event");
        }

        try {
            Memory::override_function(enqueue_sound_permutation_function_sig->data(), sound_playback_event, &sound_playback_function);
        }
        catch(const std::runtime_error &e) {
            throw std::runtime_error("Could not hook sound playback event: " + std::string(e.what()));
        }

        // Register debug command
        register_command("debug_sound_playback_event", "debug", "Sets whenever to log sound playback event.", "[enable: boolean]", debug_sound_playback_event, true, 0, 1);
    }
}
