// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__CONFIG__CONFIG_HPP
#define BALLTZE__CONFIG__CONFIG_HPP

#include <nlohmann/json.hpp>
#include <string>
#include <optional>
#include <filesystem>

namespace Balltze::Config {
    /**
     * Gets the Balltze directory.
     * @return  Path to the Balltze directory.
     */
    std::filesystem::path get_balltze_directory() noexcept;
}

#endif
