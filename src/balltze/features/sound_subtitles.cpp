// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/events/sound_playback.hpp>
#include <balltze/engine/core.hpp>
#include <balltze/hook.hpp>
#include "../output/subtitles.hpp"
#include "../logger.hpp"

namespace Balltze::Features {
    static const wchar_t *get_sound_subtitles(Engine::TagDefinitions::SoundPermutation *sound_permutation) noexcept {
        if(sound_permutation && sound_permutation->subtitle_data.size > 0 && sound_permutation->subtitle_data.pointer) {
            return reinterpret_cast<const wchar_t *>(sound_permutation->subtitle_data.pointer);
        }
        return nullptr;
    }

    static bool show_subtitles(Event::SoundPlaybackEvent const &event) noexcept {
        if(event.time == Event::EVENT_TIME_AFTER && !event.cancelled()) {
            auto &args = event.args;
            auto *subtitles = get_sound_subtitles(args.permutation);
            if(subtitles) {
                std::wstring wstr = subtitles;
                if(!args.permutation->samples_pointer) {
                    logger.debug("Null samples pointer for sound {}", args.permutation->name.string);
                }
                else {
                    auto duration = Engine::get_sound_permutation_samples_duration(args.sound, args.permutation);
                    add_subtitle(wstr, {1.0f, 1.0f, 1.0f, 1.0f}, duration);
                }
            }
        }
        return true;
    }

    void set_up_sound_subtitles() noexcept {
        Event::CancellableConstEventDelegate<Event::SoundPlaybackEvent> show_subtitles_delegate(show_subtitles);
        Event::SoundPlaybackEvent::subscribe_const(show_subtitles_delegate);
    }
}

