// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__PLUGINS__PLUGIN_HPP
#define BALLTZE__PLUGINS__PLUGIN_HPP

#include <string>
#include <filesystem>
#include <vector>
#include <map>
#include <utility>
#include <windows.h>
#include <lua.hpp>
#include <balltze/engine/tag.hpp>
#include <balltze/logger.hpp>
#include <balltze/plugin.hpp>

namespace Balltze::Plugins {
    enum PluginInitResult {
        PLUGIN_INIT_SUCCESS,
        PLUGIN_INIT_FAILURE,
        PLUGIN_INIT_INCOMPATIBLE,
        PLUGIN_INIT_NOT_FOUND
    };

    class Plugin {
    protected:
        std::optional<PluginMetadata> m_metadata;
        std::string m_filename;
        std::filesystem::path m_filepath;
        std::filesystem::path m_directory;
        bool m_loaded = false;

        virtual void get_directory() = 0;
        virtual void read_metadata() = 0;

    public:
        std::string filename() const noexcept;
        std::filesystem::path filepath() const noexcept;
        std::string name() const noexcept;
        std::string author() const noexcept;
        semver::version version() const noexcept;
        semver::version target_api() const noexcept;
        bool reloadable() const noexcept;
        std::filesystem::path directory() const noexcept;
        bool path_is_valid(std::filesystem::path path) const noexcept;
        void init_data_directory();
        bool loaded() const noexcept;

        virtual PluginInitResult init() = 0;
        virtual void load() = 0;
        virtual void unload() = 0;
    };

    class DLLPlugin : public Plugin {
    private:
        HMODULE m_handle;

        void get_directory();
        void read_metadata();

    public:
        HMODULE handle();
        PluginInitResult init();
        void load();
        void unload();
        DLLPlugin(std::filesystem::path dlL_file);
    };

    class LuaPlugin : public Plugin {
    private:
        lua_State *m_state;
        std::vector<std::unique_ptr<Logger>> m_loggers;
        std::map<std::string, std::vector<std::pair<std::string, Engine::TagClassInt>>> m_tag_imports;

        void get_directory();
        void read_metadata();

    public:
        lua_State *state() noexcept;
        void add_logger(std::string name);
        void remove_logger(std::string name);
        Logger *get_logger(std::string name) noexcept;
        void add_tag_import(std::string map_name_or_path, std::string tag_path, Engine::TagClassInt tag_class);
        void import_all_tags(std::string map_name_or_path);
        void clear_tag_imports() noexcept;
        std::map<std::string, std::vector<std::pair<std::string, Engine::TagClassInt>>> const &imported_tags() const noexcept;
        PluginInitResult init();
        void load();
        void unload();
        LuaPlugin(std::filesystem::path lua_file);
    };

    std::filesystem::path get_plugins_path() noexcept;
    void init_plugins_path();
}

#endif
