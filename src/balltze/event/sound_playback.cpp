// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/hook.hpp>
#include <balltze/events/sound_playback.hpp>
#include <balltze/engine/tag.hpp>
#include "../logger.hpp"

namespace Balltze::Event {
    extern "C" {
        void *original_play_sound_function = nullptr; 
        void sound_playback_event();

        bool dispatch_sound_playback_event_before(Engine::TagHandle sound_tag) {
            SoundPlaybackEventArgs args;
            args.sound_tag = sound_tag;
            SoundPlaybackEvent sound_playback_event(EVENT_TIME_BEFORE, args);
            sound_playback_event.dispatch();
            return sound_playback_event.cancelled();
        }

        void dispatch_sound_playback_event_after(Engine::TagHandle sound_tag) {
            SoundPlaybackEventArgs args;
            args.sound_tag = sound_tag;
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

        static auto *play_sound_function_sig = Memory::get_signature("play_sound_function");
        if(!play_sound_function_sig) {
            throw std::runtime_error("Could not find signature for sound playback event");
        }

        auto *hook = Memory::override_function(play_sound_function_sig->data(), sound_playback_event, original_play_sound_function);
    }
}
