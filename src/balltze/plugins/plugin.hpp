// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__PLUGINS__PLUGIN_HPP
#define BALLTZE__PLUGINS__PLUGIN_HPP

#include <string>
#include <filesystem>
#include <windows.h>
#include <lua.hpp>
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
        std::string filename() const noexcept;
        std::string name() const noexcept;
        std::string author() const noexcept;
        VersionNumber version() const noexcept;
        VersionNumber balltze_version() const noexcept;
        bool reloadable() const noexcept;
        std::filesystem::path directory() const noexcept;

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

    class LuaPlugin : public Plugin {
    private:
        lua_State *m_state;
        std::vector<std::unique_ptr<Logger>> m_loggers;

        void read_metadata();

    public:
        lua_State *state() noexcept;
        void add_logger(std::string name);
        void remove_logger(std::string name);
        Logger *get_logger(std::string name) noexcept;
        PluginInitResult init();
        void load();
        LuaPlugin(std::filesystem::path lua_file);
    };

    std::filesystem::path get_plugins_path() noexcept;
    void init_plugins_path();
}

#endif
