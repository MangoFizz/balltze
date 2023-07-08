// SPDX-License-Identifier: GPL-3.0-only

#include <stack>
#include <utility>
#include <balltze/hook.hpp>
#include <balltze/events/sound_playback.hpp>
#include <balltze/engine/tag.hpp>
#include <balltze/engine/tag_definitions/sound.hpp>
#include "../logger.hpp"

namespace Balltze::Event {
    extern "C" {
        void sound_playback_event_before();
        void sound_playback_event_after();

        bool dispatch_sound_playback_event_before(Engine::TagDefinitions::SoundPermutation *permutation) {
            auto *tag = Engine::get_tag(permutation->tag_id_1);
            if(!tag) {
                logger.debug("Could not find tag for permutation {} in dispatch_sound_playback_event_before", permutation->name.string);
                return false;
            }
            auto *sound = reinterpret_cast<Engine::TagDefinitions::Sound *>(tag->data);
            SoundPlaybackEventArgs args(sound, permutation);
            SoundPlaybackEvent sound_playback_event(EVENT_TIME_BEFORE, args);
            sound_playback_event.dispatch();
            return sound_playback_event.cancelled();
        }

        void dispatch_sound_playback_event_after(Engine::TagDefinitions::SoundPermutation *permutation) {
            auto *tag = Engine::get_tag(permutation->tag_id_1);
            if(!tag) {
                logger.debug("Could not find tag for permutation {} in dispatch_sound_playback_event_before", permutation->name.string);
                return;
            }
            auto *sound = reinterpret_cast<Engine::TagDefinitions::Sound *>(tag->data);
            SoundPlaybackEventArgs args(sound, permutation);
            SoundPlaybackEvent sound_playback_event(EVENT_TIME_AFTER, args);
            sound_playback_event.dispatch();
        }
    }

    template <>
    void EventHandler<SoundPlaybackEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        auto *enqueue_sound_permutation_call_1_sig = Memory::get_signature("enqueue_sound_permutation_call_1");
        auto *enqueue_sound_permutation_call_2_sig = Memory::get_signature("enqueue_sound_permutation_call_2");
        auto *enqueue_sound_permutation_call_3_sig = Memory::get_signature("enqueue_sound_permutation_call_3");
        if(!enqueue_sound_permutation_call_1_sig || !enqueue_sound_permutation_call_2_sig || !enqueue_sound_permutation_call_3_sig) {
            throw std::runtime_error("Could not find signatures for sound playback event");
        }

        try {
            Memory::hook_function(enqueue_sound_permutation_call_1_sig->data(), sound_playback_event_before, sound_playback_event_after);
            Memory::hook_function(enqueue_sound_permutation_call_2_sig->data(), sound_playback_event_before, sound_playback_event_after);
            Memory::hook_function(enqueue_sound_permutation_call_3_sig->data(), sound_playback_event_before, sound_playback_event_after);
        }
        catch(const std::exception &e) {
            logger.error("Could not hook sound playback event: {}", e.what());
        }
    }
}
