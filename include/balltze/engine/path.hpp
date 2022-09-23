// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__ENGINE__PATH_HPP
#define BALLTZE__ENGINE__PATH_HPP

#include <filesystem>

namespace Balltze::Engine {
    /**
     * Get the Halo profile path
     * @return Halo profile path
     */
    std::filesystem::path get_path() noexcept;
}

#endif
