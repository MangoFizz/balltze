// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__PLUGIN_HPP
#define BALLTZE_API__PLUGIN_HPP

#define BALLTZE_PLUGIN_API extern "C" __declspec(dllexport)

#include "api.hpp"
#include "utils.hpp"

using PluginHandle = void *;

namespace Balltze {
    BALLTZE_API std::filesystem::path get_plugin_path(PluginHandle);

    inline std::filesystem::path get_plugin_path() {
        return get_plugin_path(get_current_module());
    }
}

#endif
