// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__MAP_LOADING__COMPRESSION_HPP
#define BALLTZE__MAP_LOADING__COMPRESSION_HPP

#include <cstddef>

namespace Balltze {
    std::size_t decompress_map_file(const char *input, const char *output);
    std::size_t decompress_map_file(const char *input, std::byte *output, std::size_t output_size);
}

#endif