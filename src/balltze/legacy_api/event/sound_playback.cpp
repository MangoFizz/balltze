// SPDX-License-Identifier: GPL-3.0-only

#include <stack>
#include <utility>
#include <balltze/api.hpp>
#include <balltze/hook.hpp>
#include <balltze/command.hpp>
#include <balltze/legacy_api/events/sound_playback.hpp>
#include <balltze/legacy_api/engine/tag.hpp>
#include <balltze/legacy_api/engine/tag_definitions/sound.hpp>
#include "../../logger.hpp"

namespace Balltze::LegacyApi::Event {
    extern "C" {
        void sound_playback_event();
        void *sound_playback_function = nullptr;

        bool dispatch_sound_playback_event_before(LegacyApi::Engine::TagDefinitions::SoundPermutation *permutation) {
            auto *tag = LegacyApi::Engine::get_tag(permutation->sound_tag_handle_0);
            if(!tag) {
                logger.debug("Could not find tag for permutation {} in dispatch_sound_playback_event_before", permutation->name.string);
                return false;
            }
            auto *sound = reinterpret_cast<LegacyApi::Engine::TagDefinitions::Sound *>(tag->data);
            SoundPlaybackEventContext args(sound, permutation);
            SoundPlaybackEvent sound_playback_event(EVENT_TIME_BEFORE, args);
            sound_playback_event.dispatch();
            return sound_playback_event.cancelled();
        }

        void dispatch_sound_playback_event_after(LegacyApi::Engine::TagDefinitions::SoundPermutation *permutation) {
            auto *tag = LegacyApi::Engine::get_tag(permutation->sound_tag_handle_0);
            if(!tag) {
                logger.debug("Could not find tag for permutation {} in dispatch_sound_playback_event_before", permutation->name.string);
                return;
            }
            auto *sound = reinterpret_cast<LegacyApi::Engine::TagDefinitions::Sound *>(tag->data);
            SoundPlaybackEventContext args(sound, permutation);
            SoundPlaybackEvent sound_playback_event(EVENT_TIME_AFTER, args);
            sound_playback_event.dispatch();
        }
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
    }
}
