// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_LEGACY_API__PLUGIN_HPP
#define BALLTZE_LEGACY_API__PLUGIN_HPP

#include <string>
#include <vector>
#include <filesystem>
#include <semver/semver.hpp>
#include "../plugin.hpp"

using PluginHandle = void *;

namespace Balltze::LegacyApi {
    struct PluginMetadata {
        std::string name;
        std::string author;
        semver::version version;
        semver::version target_api;
        std::vector<std::string> maps;
        bool reloadable;
    };

    using plugin_metadata_proc_t = PluginMetadata (*)();
    using plugin_load_proc_t = bool (*)();
    using plugin_unload_proc_t = void (*)();
    using plugin_first_tick_proc_t = void (*)();
}

#endif
