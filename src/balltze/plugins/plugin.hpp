// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__PLUGINS__PLUGIN_HPP
#define BALLTZE__PLUGINS__PLUGIN_HPP

#include <string>
#include <filesystem>
#include <windows.h>
#include <balltze/plugin.hpp>

namespace Balltze::Plugins {
    enum PluginInitResult {
        PLUGIN_INIT_SUCCESS,
        PLUGIN_INIT_FAILURE,
        PLUGIN_INIT_NOT_FOUND
    };

    class Plugin {
    protected:
        PluginMetadata m_metadata;
        std::string m_filename;
        std::filesystem::path m_filepath;
        std::filesystem::path m_directory;

        void get_directory();
        virtual void read_metadata() = 0;

    public:
        std::string filename() const;
        std::string name() const;
        std::string author() const;
        VersionNumber version() const;
        VersionNumber balltze_version() const;
        std::filesystem::path directory() const;

        virtual PluginInitResult init() = 0;
        virtual void load() = 0;
    };

    class DLLPlugin : public Plugin {
    private:
        HMODULE m_handle;

        void read_metadata();

    public:
        HMODULE handle();
        PluginInitResult init();
        void load();
        DLLPlugin(std::filesystem::path dlL_file);
    };

    std::filesystem::path get_plugins_path() noexcept;
    void init_plugins_path();
}

#endif
