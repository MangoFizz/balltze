// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__EVENTS__SOUND_PLAYBACK_HPP
#define BALLTZE_API__EVENTS__SOUND_PLAYBACK_HPP

#include "../engine/tag.hpp"
#include "../event.hpp"

namespace Balltze::Event {
    struct SoundPlaybackEventArgs {
        Engine::TagHandle sound_tag;
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
