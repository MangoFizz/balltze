// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__OUTPUT_HPP
#define BALLTZE_API__OUTPUT_HPP

#include <filesystem>
#include <chrono>
#include <variant>
#include "legacy_api/engine/data_types.hpp"
#include "legacy_api/engine/tag.hpp"
#include "api.hpp"

namespace Balltze {
    /**
     * Play a BIK video.
     * @param path The path to the BIK video.
     */
    BALLTZE_API void play_bik_video(std::filesystem::path const &path);
}

#endif
