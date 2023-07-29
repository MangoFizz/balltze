// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__CONFIG__CONFIG_HPP
#define BALLTZE__CONFIG__CONFIG_HPP

#include <string>
#include <optional>
#include <filesystem>
#include <balltze/config.hpp>

namespace Balltze::Config {
    /**
     * Get the Balltze directory.
     * @return  Path to the Balltze directory.
     */
    std::filesystem::path get_balltze_directory() noexcept;

    /**
     * Get Chimera INI config.
     */
    Ini get_chimera_ini() noexcept;

    /**
     * Get the Balltze configs.
     */
    Config &get_config();

    /**
     * Get the Balltze gamepad configs.
     */
    std::optional<Config> get_gamepad_config();
}

#endif
