// SPDX-License-Identifier: GPL-3.0-only

#include <vector>
#include <queue>
#include <utility>
#include <cstring>
#include <balltze/events/sound_playback.hpp>
#include <balltze/engine/tag.hpp>
#include <balltze/engine/tag_definitions/sound.hpp>
#include <balltze/engine/core.hpp>
#include <balltze/memory.hpp>
#include "../output/subtitles.hpp"
#include "../logger.hpp"

namespace Balltze::Features {
    static std::vector<std::pair<Engine::TagDefinitions::Sound *, Engine::TagDefinitions::SoundPermutation *>> unloaded_permutations;
    static std::queue<std::pair<Engine::TagDefinitions::Sound *, Engine::TagDefinitions::SoundPermutation *>> waiting_list;

    static const wchar_t *get_sound_subtitles(Engine::TagDefinitions::SoundPermutation *sound_permutation) noexcept {
        if(sound_permutation && sound_permutation->subtitle_data.size > 0 && sound_permutation->subtitle_data.pointer) {
            return reinterpret_cast<const wchar_t *>(sound_permutation->subtitle_data.pointer);
        }
        return nullptr;
    }

    void sound_permutation_samples_loaded(std::byte *buffer, std::size_t offset, std::size_t size) {
        auto it = unloaded_permutations.begin();
        while(it != unloaded_permutations.end()) {
            auto &pair = *it;
            auto *sound = pair.first;
            auto *permutation = pair.second;
            if(permutation->samples.file_offset == offset && permutation->samples.size == size) {
                waiting_list.push(pair);
                logger.debug("Loaded sound permutation samples for {}", permutation->name.string);
                logger.debug("Output buffer: {}", reinterpret_cast<std::uint32_t>(buffer));
                it = unloaded_permutations.erase(it);
            }
            else {
                it++;
            }
        }
    }

    static void worker(Event::TickEvent &) {
        if(!waiting_list.empty()) {
            auto &pair = waiting_list.front();
            auto *sound = pair.first;
            auto *permutation = pair.second;
            auto duration = Engine::get_sound_permutation_samples_duration(sound, permutation);
            logger.debug("Showing subtitles for sound {} ({} ms)", permutation->name.string, duration.count());
            auto *subtitles = get_sound_subtitles(permutation);
            if(subtitles) {
                std::wstring wstr = subtitles;
                add_subtitle(wstr, {1.0f, 1.0f, 1.0f, 1.0f}, duration);
            }
            waiting_list.pop();
        }
    }

    static bool show_subtitles(Event::SoundPlaybackEvent const &event) noexcept {
        auto &args = event.args;
        auto *subtitles = get_sound_subtitles(args.permutation);
        if(subtitles) {
            std::wstring wstr = subtitles;
            if(!args.permutation->samples_pointer) {
                logger.debug("Waiting for sound permutation samples to be loaded for {}", args.permutation->name.string);
                unloaded_permutations.push_back({args.sound, args.permutation});
            }
            else {
                auto duration = Engine::get_sound_permutation_samples_duration(args.sound, args.permutation);
                logger.debug("Showing subtitles for sound {} ({} ms)", args.permutation->name.string, duration.count());
                add_subtitle(wstr, {1.0f, 1.0f, 1.0f, 1.0f}, duration);
            }
        }
        return true;
    }

    void set_up_sound_subtitles() noexcept {
        Event::CancellableConstEventDelegate<Event::SoundPlaybackEvent> show_subtitles_delegate(show_subtitles);
        Event::SoundPlaybackEvent::subscribe_const(show_subtitles_delegate);
        Event::TickEvent::subscribe(worker);
    }
}

