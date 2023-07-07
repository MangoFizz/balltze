// SPDX-License-Identifier: GPL-3.0-only

#include <vector>
#include <memory>
#include <cstdint>
#include <vorbis/vorbisenc.h>

namespace Invader::SoundEncoder {
    void write_sample(std::int32_t sample, std::byte *pcm, std::size_t bits_per_sample) noexcept {
        std::size_t bytes_per_sample = bits_per_sample / 8;
        for(std::size_t b = 0; b < bytes_per_sample; b++) {
            pcm[b] = static_cast<std::byte>((sample >> b * 8) & 0xFF);
        }
    }

    std::vector<std::byte> convert_float_to_int(const std::vector<float> &pcm, std::size_t new_bits_per_sample) {
        std::size_t sample_count = pcm.size();
        std::size_t bytes_per_sample = new_bits_per_sample / 8;
        std::vector<std::byte> samples(sample_count * bytes_per_sample);
        auto *sample_data = samples.data();

        // Calculate what we multiply by
        std::int64_t multiply_by = (1 << new_bits_per_sample) / 2.0;
        std::int64_t multiply_by_minus_one = multiply_by - 1;
        std::int64_t multiply_by_arr[2] = { multiply_by_minus_one, multiply_by };

        for(std::size_t i = 0; i < sample_count; i++) {
            std::int64_t sample = pcm[i] * multiply_by_arr[pcm[i] < 0];

            // Clamp
            if(sample >= multiply_by_minus_one) {
                sample = multiply_by_minus_one;
            }
            else if(sample <= -multiply_by) {
                sample = -multiply_by;
            }

            write_sample(static_cast<std::int32_t>(sample), sample_data, new_bits_per_sample);
            sample_data += bytes_per_sample;
        }

        return samples;
    }
}
