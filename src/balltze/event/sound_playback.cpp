// SPDX-License-Identifier: GPL-3.0-only

#include <stack>
#include <utility>
#include <balltze/hook.hpp>
#include <balltze/events/sound_playback.hpp>
#include <balltze/engine/tag.hpp>
#include <balltze/engine/tag_definitions/sound.hpp>
#include "../logger.hpp"

namespace Balltze::Event {
    static std::stack<std::pair<Engine::TagDefinitions::Sound *, std::uint16_t>> permutations_stack;

    extern "C" {
        void *original_play_sound_function = nullptr; 
        void sound_playback_event();
        void get_next_sound_permutation_index_hook();

        void push_sound_permutation(Engine::TagDefinitions::Sound *sound, std::uint16_t permutation_index) {
            permutations_stack.push(std::make_pair(sound, permutation_index));
        }

        std::uint16_t pop_sound_permutation(Engine::TagDefinitions::Sound *sound) {
            if(!permutations_stack.empty()) {
                auto &top = permutations_stack.top();
                if(top.first == sound) {
                    auto permutation_index = top.second;
                    permutations_stack.pop();
                    // logger.debug("Found permutation index for sound ({} == {})", reinterpret_cast<uint32_t>(top.first), reinterpret_cast<uint32_t>(sound));
                    return permutation_index;
                }
                logger.debug("Could not find permutation index for sound ({} != {})", reinterpret_cast<uint32_t>(top.first), reinterpret_cast<uint32_t>(sound));
            }
            return -1;
        }

        bool dispatch_sound_playback_event_before(Engine::TagHandle sound_tag, Engine::TagDefinitions::Sound *sound, std::uint16_t permutation_index) {
            if(permutation_index == -1 && sound->pitch_ranges.count == 0 || permutation_index >= sound->pitch_ranges.offset[0].actual_permutation_count) {
                return false;
            }

            auto *permutation = &sound->pitch_ranges.offset[0].permutations.offset[permutation_index]; 

            SoundPlaybackEventArgs args(sound_tag, sound, permutation_index, permutation);
            SoundPlaybackEvent sound_playback_event(EVENT_TIME_BEFORE, args);
            sound_playback_event.dispatch();
            return sound_playback_event.cancelled();
        }

        void dispatch_sound_playback_event_after(Engine::TagHandle sound_tag, Engine::TagDefinitions::Sound *sound, std::uint16_t permutation_index) {
            if(permutation_index == -1 && sound->pitch_ranges.count == 0 || sound->pitch_ranges.offset[0].actual_permutation_count >= permutation_index) {
                return;
            }

            auto *permutation = &sound->pitch_ranges.offset[0].permutations.offset[permutation_index]; 

            SoundPlaybackEventArgs args(sound_tag, sound, permutation_index, permutation);
            SoundPlaybackEvent sound_playback_event(EVENT_TIME_BEFORE, args);
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

        static auto *get_next_sound_permutation_index_sig = Memory::get_signature("get_next_sound_permutation_function_play_sound_call");
        if(!get_next_sound_permutation_index_sig) {
            throw std::runtime_error("Could not find signature for get_next_sound_permutation_index");
        }

        Memory::override_function(play_sound_function_sig->data(), sound_playback_event, original_play_sound_function);
        Memory::replace_function_call(get_next_sound_permutation_index_sig->data(), get_next_sound_permutation_index_hook);
    }
}
