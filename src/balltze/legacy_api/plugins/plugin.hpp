// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__LEGACY_API__PLUGINS__PLUGIN_HPP
#define BALLTZE__LEGACY_API__PLUGINS__PLUGIN_HPP

#include <string>
#include <filesystem>
#include <vector>
#include <map>
#include <utility>
#include <windows.h>
#include <lua.hpp>
#include <balltze/legacy_api/engine/tag.hpp>
#include <balltze/logger.hpp>
#include <balltze/legacy_api/plugin.hpp>

namespace Balltze::LegacyApi::Plugins {
    enum PluginLoadResult {
        PLUGIN_LOAD_SUCCESS,
        PLUGIN_LOAD_FAILURE,
        PLUGIN_LOAD_INCOMPATIBLE,
        PLUGIN_LOAD_NOT_FOUND
    };

    class Plugin {
    protected:
        std::optional<PluginMetadata> m_metadata;
        std::string m_filename;
        std::filesystem::path m_filepath;
        std::filesystem::path m_directory;
        bool m_loaded = false;
        bool m_execute_first_tick = false;

        virtual void set_up_directory() = 0;
        virtual void update_metadata() = 0;
        virtual void init() = 0;
        virtual void dispose() = 0;

    public:
        std::string filename() const noexcept;
        std::filesystem::path filepath() const noexcept;
        std::string name() const noexcept;
        std::string author() const noexcept;
        semver::version version() const noexcept;
        semver::version target_api() const noexcept;
        std::vector<std::string> const &maps() const noexcept;
        bool reloadable() const noexcept;
        std::filesystem::path directory() const noexcept;
        bool path_is_valid(std::filesystem::path path) const noexcept;
        void init_data_directory();
        bool loaded() const noexcept;

        virtual bool initialized() const noexcept = 0;
        virtual PluginLoadResult load() = 0;
        virtual void unload() = 0;
        virtual void first_tick() = 0;
        virtual ~Plugin() = default;
    };

    class NativePlugin : public Plugin {
    private:
        HMODULE m_handle;

        void set_up_directory();
        void update_metadata();
        void init();
        void dispose();

    public:
        HMODULE handle();
        bool initialized() const noexcept;
        PluginLoadResult load();
        void unload();
        void first_tick();
        NativePlugin(std::filesystem::path dlL_file);
        ~NativePlugin();
    };

    class LuaPlugin : public Plugin {
    private:
        lua_State *m_state;
        std::vector<std::unique_ptr<Logger>> m_loggers;
        std::map<std::string, std::vector<std::pair<std::string, LegacyApi::Engine::TagClassInt>>> m_tag_imports;

        void set_up_directory();
        void update_metadata();
        void init();
        void unload();

    public:
        lua_State *state() noexcept;
        void add_logger(std::string name);
        void remove_logger(std::string name);
        Logger *get_logger(std::string name) noexcept;
        void add_tag_import(std::string map_name_or_path, std::string tag_path, LegacyApi::Engine::TagClassInt tag_class);
        void import_all_tags(std::string map_name_or_path);
        void clear_tag_imports() noexcept;
        std::map<std::string, std::vector<std::pair<std::string, LegacyApi::Engine::TagClassInt>>> const &imported_tags() const noexcept;
        void print_traceback();
        std::string get_error_message();
        bool initialized() const noexcept;
        PluginLoadResult load();
        void dispose();
        void first_tick();
        LuaPlugin(std::filesystem::path lua_file);
        ~LuaPlugin();
    };

    void remove_plugin_commands(LuaPlugin *plugin) noexcept;
}

#endif
