// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__CONFIG__CONFIG_HPP
#define BALLTZE__CONFIG__CONFIG_HPP

#include <nlohmann/json.hpp>
#include <string>
#include <optional>
#include <filesystem>
#include <balltze/config.hpp>
#include <balltze/engine/widget.hpp>

namespace Balltze::Config {
    /**
     * Get the Balltze directory.
     * @return  Path to the Balltze directory.
     */
    std::filesystem::path get_balltze_directory() noexcept;

    /**
     * Get the Balltze configs.
     */
    Config get_config();

    /**
     * Translate a key code to a xbox gamepad button
     * @param button_index  Index of the button
     * @return              Xbox gamepad button
     */
    Engine::GamepadButton get_gamepad_mapped_button(std::size_t button_index);
}

#endif
