// SPDX-License-Identifier: GPL-3.0-only

#ifndef INVADER__SOUND__ENCODER_HPP
#define INVADER__SOUND__ENCODER_HPP

#include <vector>
#include <cstdint>

namespace Invader::SoundEncoder {
    /**
     * Encode from one PCM size to another. This is lossy unless the PCM data was originally integer PCM of the same bitness or smaller.
     * @param pcm                 PCM data
     * @param new_bits_per_sample new bits per sample
     */
    std::vector<std::byte> convert_float_to_int(const std::vector<float> &pcm, std::size_t new_bits_per_sample);
}

#endif
