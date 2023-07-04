// SPDX-License-Identifier: GPL-3.0-only

#include <cstring>
#include <balltze/events/sound_playback.hpp>
#include <balltze/engine/tag.hpp>
#include <balltze/engine/tag_definitions/sound.hpp>
#include <balltze/engine/core.hpp>
#include <balltze/memory.hpp>

#include "../logger.hpp"

namespace Balltze::Features {
    static const wchar_t *get_sound_subtitles(Engine::TagDefinitions::SoundPermutation *sound_permutation) noexcept {
        if(sound_permutation && sound_permutation->subtitle_data.size > 0 && sound_permutation->subtitle_data.pointer) {
            return reinterpret_cast<const wchar_t *>(sound_permutation->subtitle_data.pointer);
        }
        return nullptr;
    }

    static bool show_subtitles(Event::SoundPlaybackEvent const &event) noexcept {
        auto &args = event.args;
        auto *subtitles = get_sound_subtitles(args.permutation);
        if(subtitles) {
            std::wstring wstr = subtitles;
            std::string str(wstr.begin(), wstr.end());
            Engine::console_print(str);
        }
        return true;
    }

    void set_up_sound_subtitles() noexcept {
        Event::CancellableConstEventDelegate<Event::SoundPlaybackEvent> show_subtitles_delegate(show_subtitles);
        Event::SoundPlaybackEvent::subscribe_const(show_subtitles_delegate);
    }
}

