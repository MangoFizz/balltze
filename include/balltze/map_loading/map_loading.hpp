// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__MAP_LOADING__MAP_LOADING_HPP
#define BALLTZE__MAP_LOADING__MAP_LOADING_HPP

#include <cstdint>
#include <cstddef>
#include <optional>
#include <filesystem>

namespace Balltze {
    struct LoadedMap {
        std::string name;
        std::filesystem::path path;
        std::optional<std::byte *> memory_location;
        std::size_t buffer_size;
        std::size_t decompressed_size;
        std::size_t loaded_size;
        std::size_t file_size;
        std::filesystem::file_time_type timestamp; // time it was modified
        bool secondary;
        bool fixed_tags = false;
    };
    
    /**
     * Get the loaded map
     * @param  name name of map
     * @return      reference to map if found, or nullptr
     */
    LoadedMap *get_loaded_map(const char *name) noexcept;
    
    /**
     * Load the map
     * @param map_name map to load
     * @return         loaded map
     */
    LoadedMap *load_map(const char *map_name, bool secondary = false);

    /**
     * Set up loading maps outside of the maps directory
     */
    void set_up_map_loading();
}

#endif
