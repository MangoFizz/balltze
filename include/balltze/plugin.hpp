// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__PLUGIN_HPP
#define BALLTZE_API__PLUGIN_HPP

#define BALLTZE_PLUGIN_API extern "C" __declspec(dllexport)

#include <string>
#include <semver.hpp>

namespace Balltze {
    struct PluginMetadata {
        std::string name;
        std::string author;
        semver::version version;
        semver::version target_api;
        bool reloadable;
    };

    using plugin_metadata_proc_t = PluginMetadata (*)();
    using plugin_init_proc_t = bool (*)();
    using plugin_load_proc_t = void (*)();
    using plugin_unload_proc_t = void (*)();
}

#endif
