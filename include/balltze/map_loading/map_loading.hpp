// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__MAP_LOADING__MAP_LOADING_HPP
#define BALLTZE__MAP_LOADING__MAP_LOADING_HPP

#include <cstdint>
#include <cstddef>
#include <optional>
#include <filesystem>

namespace Balltze {    
    /**
     * Load the map
     * @param map_name map to load
     */
    void append_map(const char *map_name);

    /**
     * Set up loading maps outside of the maps directory
     */
    void set_up_map_loading();
}

#endif
