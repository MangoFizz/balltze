// SPDX-License-Identifier: GPL-3.0-only

#include <cstring>
#include <balltze/events/sound_playback.hpp>
#include <balltze/engine/tag.hpp>
#include <balltze/engine/tag_definitions/sound.hpp>
#include <balltze/engine/core.hpp>

#include "../logger.hpp"

namespace Balltze::Features {
    static const wchar_t *get_sound_subtitles(Engine::TagHandle sound_tag_handle) noexcept {
        auto tag = Engine::get_tag(sound_tag_handle);
        if(tag) {
            auto *sound_tag = reinterpret_cast<Engine::TagDefinitions::Sound *>(tag->data);
            if(sound_tag->pitch_ranges.count > 0) {
                auto &pitch_range = sound_tag->pitch_ranges.offset[0];
                if(pitch_range.permutations.count > 0) {
                    auto &permutation = pitch_range.permutations.offset[0];
                    return reinterpret_cast<const wchar_t *>(permutation.subtitle_data.pointer);
                }
            }
        }
        return nullptr;
    }

    static bool show_subtitles(Event::SoundPlaybackEvent const &event) noexcept {
        auto &args = event.args;
        auto *subtitles = get_sound_subtitles(args.sound_tag);
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

