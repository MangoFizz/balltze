// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__TAG_DATA_IMPORTING__MAP_HPP
#define BALLTZE__TAG_DATA_IMPORTING__MAP_HPP

#include <cstdint>
#include <optional>
#include <filesystem>
#include <balltze/legacy_api/engine/map.hpp>

namespace Balltze::Features {
    struct MapEntry {
        bool multiplayer;
        std::optional<std::uint32_t> crc32;
        std::optional<LegacyApi::Engine::CacheFileEngine> engine;
        std::string name;
        std::optional<std::uint32_t> index;
        
        std::filesystem::path get_file_path();
    };

    const std::filesystem::path get_map_path() noexcept;
    const std::filesystem::path get_download_map_path() noexcept;
    std::filesystem::path path_for_map_local(const char *map_name);
    std::string name_tmp_map(std::string map_name) noexcept;
    std::filesystem::path path_for_tmp(std::size_t tmp) noexcept;
    bool same_string_case_insensitive(const char *a, const char *b) noexcept;
    std::optional<MapEntry> get_map_entry(const char *map_name);
}

#endif
