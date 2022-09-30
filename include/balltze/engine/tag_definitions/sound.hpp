#ifndef BALLTZE__ENGINE__TAG_DEFINITIONS__SOUND_HPP
#define BALLTZE__ENGINE__TAG_DEFINITIONS__SOUND_HPP

#include <cstdint>
#include "types.hpp"

namespace Balltze::Engine { 
	struct SoundPermutation {
		char name[32];
		float skip_fraction;
		float gain;
		std::uint16_t format;
		std::uint16_t next_permutation_index;
		std::uint32_t samples_pointer;
		std::byte pad_7[4];
		std::uint32_t tag_id_0;
		std::uint32_t buffer_size;
		std::uint32_t tag_id_1;
		TagDataOffset samples;
		TagDataOffset mouth_data;
		TagDataOffset subtitle_data;
	};
	struct SoundPitchRange {
		char name[32];
		float natural_pitch;
		float bend_bounds[2];
		std::uint16_t actual_permutation_count;
		std::byte pad_5[2];
		float playback_rate;
		std::uint32_t unknown_ffffffff_0;
		std::uint32_t unknown_ffffffff_1;
		TagReflexive<SoundPermutation> permutations;
	};
	static_assert(sizeof(SoundPitchRange) == 72);

	struct Sound {
		std::uint32_t flags;
		std::uint16_t sound_class;
		std::uint16_t sample_rate;
		float minimum_distance;
		float maximum_distance;
		float skip_fraction;
		float random_pitch_bounds[2];
		float inner_cone_angle;
		float outer_cone_angle;
		float outer_cone_gain;
		float random_gain_modifier;
		float maximum_bend_per_second;
		std::byte pad_13[12];
		float zero_skip_fraction_modifier;
		float zero_gain_modifier;
		float zero_pitch_modifier;
		std::byte pad_17[12];
		float one_skip_fraction_modifier;
		float one_gain_modifier;
		float one_pitch_modifier;
		std::byte pad_21[12];
		std::uint16_t channel_count;
		std::uint16_t format;
		TagDependency promotion_sound;
		std::uint16_t promotion_count;
		std::byte pad_26[2];
		std::uint32_t longest_permutation_length;
		std::byte pad_28[8];
		std::uint32_t unknown_ffffffff_0;
		std::uint32_t unknown_ffffffff_1;
		TagReflexive<SoundPitchRange> pitch_ranges;
	};
	static_assert(sizeof(Sound) == 0xA4);
}

#endif
