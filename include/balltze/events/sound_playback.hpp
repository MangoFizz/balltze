// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__EVENTS__SOUND_PLAYBACK_HPP
#define BALLTZE_API__EVENTS__SOUND_PLAYBACK_HPP

#include "../engine/tag.hpp"
#include "../engine/tag_definitions/sound.hpp"
#include "../event.hpp"

namespace Balltze::Event {
    struct SoundPlaybackEventContext {
        Engine::TagDefinitions::Sound *const sound;
        Engine::TagDefinitions::SoundPermutation *const permutation;

        SoundPlaybackEventContext(Engine::TagDefinitions::Sound *sound, Engine::TagDefinitions::SoundPermutation *permutation) : sound(sound), permutation(permutation) {}
    };

    class SoundPlaybackEvent : public EventData<SoundPlaybackEvent> {
    public:
        SoundPlaybackEventContext context;

        bool cancellable() const {
            return true;
        }

        SoundPlaybackEvent(EventTime time, SoundPlaybackEventContext context) : EventData(time), context(context) {}
    };
}

#endif
