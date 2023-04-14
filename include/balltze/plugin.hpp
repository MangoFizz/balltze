// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__PLUGIN_HPP
#define BALLTZE_API__PLUGIN_HPP

#define BALLTZE_PLUGIN_API extern "C" __declspec(dllexport)

#include <string>

namespace Balltze {
    struct VersionNumber {
        int major;
        int minor;
        int patch;
        int build;
    };

    struct PluginMetadata {
        std::string name;
        std::string author;
        VersionNumber version;
        VersionNumber balltze_version;
        bool reloadable;
    };

    using plugin_metadata_proc_t = PluginMetadata (*)();
    using plugin_init_proc_t = bool (*)();
    using plugin_load_proc_t = void (*)();
    using plugin_unload_proc_t = void (*)();
}

#endif
