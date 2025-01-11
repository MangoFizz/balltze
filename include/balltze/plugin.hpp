// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__PLUGIN_HPP
#define BALLTZE_API__PLUGIN_HPP

#define BALLTZE_PLUGIN_API extern "C" __declspec(dllexport)

#include <string>
#include <vector>
#include <filesystem>
#include <semver.hpp>
#include "api.hpp"
#include "utils.hpp"

using PluginHandle = void *;

namespace Balltze {
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

    BALLTZE_API std::filesystem::path get_plugin_path(PluginHandle);

    inline std::filesystem::path get_plugin_path() {
        return get_plugin_path(get_current_module());
    }
}

#endif
