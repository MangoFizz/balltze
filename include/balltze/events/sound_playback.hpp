// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__EVENTS__SOUND_PLAYBACK_HPP
#define BALLTZE_API__EVENTS__SOUND_PLAYBACK_HPP

#include "../engine/tag.hpp"
#include "../engine/tag_definitions/sound.hpp"
#include "../event.hpp"

namespace Balltze::Event {
    struct SoundPlaybackEventArgs {
        const Engine::TagHandle sound_tag;
        Engine::TagDefinitions::Sound *const sound;
        const std::uint16_t permutation_index;
        Engine::TagDefinitions::SoundPermutation *const permutation;

        SoundPlaybackEventArgs(Engine::TagHandle sound_tag, Engine::TagDefinitions::Sound *sound, std::uint16_t permutation_index, Engine::TagDefinitions::SoundPermutation *permutation) : sound_tag(sound_tag), sound(sound), permutation_index(permutation_index), permutation(permutation) {}
    };

    class SoundPlaybackEvent : public EventData<SoundPlaybackEvent> {
    public:
        SoundPlaybackEventArgs args;

        static bool cancellable() {
            return true;
        }

        SoundPlaybackEvent(EventTime time, SoundPlaybackEventArgs args) : EventData(time), args(args) {}
    };
}

#endif
