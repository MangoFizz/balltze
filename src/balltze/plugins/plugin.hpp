// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__PLUGINS__PLUGIN_HPP
#define BALLTZE__PLUGINS__PLUGIN_HPP

#include <string>
#include <vector>
#include <filesystem>
#include <windows.h>
#include <lua.hpp>
#include <semver/semver.hpp>
#include <balltze/legacy_api/engine/tag.hpp>
#include <balltze/logger.hpp>
#include <balltze/plugin.hpp>

namespace Balltze::Plugins {
    struct PluginMetadata {
        std::string name;
        std::string author;
        std::string plugin_main;
        semver::version version;
        semver::version target_api;
        std::vector<std::string> maps;
        bool reloadable;
    };

    class Plugin {
    public:
        /**
         * Get the metadata of the plugin.
         * 
         * @return constant reference to the plugin metadata
         */
        const PluginMetadata &metadata() const noexcept;

        /**
         * Get the directory of the plugin.
         * 
         * @return the directory of the plugin
         */
        std::filesystem::path directory() const noexcept;

        /**
         * Get the name of the plugin.
         * 
         * @return the name of the plugin
         */
        std::string name() const noexcept;

        /**
         * Get the maps the plugin is associated with.
         * 
         * @return the maps the plugin is associated with
         */
        std::vector<std::string> const &maps() const noexcept;

        /**
         * Check if the plugin is reloadable.
         * 
         * @return true if the plugin is reloadable, false otherwise
         */
        bool reloadable() const noexcept;

        /**
         * Check if the plugin is loaded.
         * 
         * @return true if the plugin is loaded, false otherwise
         */
        bool loaded() const noexcept;

        /**
         * Load the plugin.
         * 
         * The reason this is not part of the constructor is to ensure 
         * that the plugin object exists before the plugin is loaded.
         * 
         * @throw std::runtime_error if the plugin could not be loaded
         */
        virtual void load() = 0;

        /**
         * Called when the game starts.
         * 
         * This is called after the plugin is loaded and the game is ready.
         */
        virtual void call_on_game_start() = 0;

        /**
         * Safely unload the plugin resources and call the plugin 
         * unload function if it exists.
         * 
         * @throw std::runtime_error if the plugin couldn't be unloaded
         */
        virtual void unload() = 0;

        /**
         * Constructor for the plugin.
         * 
         * @param directory the directory of the plugin
         * @param metadata the metadata of the plugin
         * @throw std::invalid_argument if the plugin directory is invalid
         */
        Plugin(std::filesystem::path directory, const PluginMetadata &metadata);

        /**
         * Default destructor for the plugin.
         */
        virtual ~Plugin() = default;

        /**
         * Read the plugin metadata from the manifest.json in the 
         * plugin directory.
         * 
         * @param manifest_path the path to the manifest file
         * @return the plugin metadata
         * @throw std::runtime_error if the manifest file could not be read
         */
        static PluginMetadata read_manifest(std::filesystem::path manifest_path);
    
    protected:
        const std::filesystem::path m_directory;
        const PluginMetadata m_metadata;
        bool m_loaded = false;
        bool m_game_start_called = false;
        bool m_load_failed = false;
    };

    class NativePlugin : public Plugin {
    public:
        /**
         * Get the DLL handle of the plugin.
         * 
         * @return the handle of the DLL
         */
        HMODULE handle() const noexcept;

        /**
         * Load the plugin DLL.
         * 
         * @throw std::runtime_error if the plugin could not be loaded
         */
        void load() override;

        /**
         * Called when the game starts.
         * 
         * This is called after the plugin is loaded and the game is ready.
         */
        void call_on_game_start() override;

        /**
         * Call the plugin's unload function if it exists and
         * free the DLL handle.
         */
        void unload() override;

        /**
         * Constructor for native plugin.
         * 
         * @param plugin_directory the directory of the plugin
         * @throw std::invalid_argument if the plugin directory is invalid
         * @throw std::runtime_error if the plugin could not be loaded
         */
        NativePlugin(std::filesystem::path plugin_directory, const PluginMetadata &metadata) : Plugin(plugin_directory, metadata) {}

        /**
         * Default destructor for native plugin.
         */
        ~NativePlugin() override;

    private:
        HMODULE m_handle = nullptr;
    };

    class LuaPlugin : public Plugin {
    public:
        struct TagImport {
            std::string map_path;
            std::string tag_path;
            LegacyApi::Engine::TagClassInt tag_class;
        };

        /**
         * Get the Lua state of the plugin.
         * 
         * @return the Lua state of the plugin
         */
        lua_State *lua_state() const noexcept;

        /**
         * Get the logger of the plugin.
         * 
         * @return the logger of the plugin
         */
        Logger *plugin_logger() noexcept;

        /**
         * Load the plugin Lua script.
         * 
         * @throw std::runtime_error if the plugin could not be loaded
         */
        void load() override;

        /**
         * Called when the game starts.
         * 
         * This is called after the plugin is loaded and the game is ready.
         */
        void call_on_game_start() override;

        /**
         * Call the plugin's unload function if it exists and
         * close the Lua state.
         */
        void unload() override;

        /**
         * Add a tag import for the plugin.
         * 
         * @param map_path the path to the map
         * @param tag_path the path to the tag
         * @param tag_class the class of the tag
         */
        void add_tag_import(std::string map_name_or_path, std::string tag_path, LegacyApi::Engine::TagClassInt tag_class);

        /**
         * Import all tags for the plugin.
         * 
         * @return tag imports of the plugin
         */
        const std::vector<TagImport> &imported_tags() const noexcept;

        /**
         * Get the error message from the Lua state.
         * 
         * @return the error message
         */
        std::string pop_error_message() const noexcept;

        /**
         * Constructor for Lua plugin.
         * 
         * @param plugin_directory the directory of the plugin
         * @throw std::invalid_argument if the plugin directory is invalid
         * @throw std::runtime_error if the plugin could not be loaded
         */
        LuaPlugin(std::filesystem::path plugin_directory, const PluginMetadata &metadata);

        /**
         * Default destructor for Lua plugin.
         */
        ~LuaPlugin() override;

        /**
         * Error handling function to print the Lua traceback.
         * 
         * @param state the Lua state
         */
        static int error_message_handler(lua_State *state) noexcept;

    private:
        lua_State *m_lua_state = nullptr;
        std::vector<TagImport> m_tag_imports;
        std::unique_ptr<Logger> m_plugin_logger;

        /**
         * Initialize the Lua state.
         */
        void init_lua_state();
    };

    /**
     * Remove the plugin commands from the Lua environment.
     * 
     * @param plugin the Lua plugin to remove commands from
     */
    void remove_plugin_commands(LuaPlugin *plugin) noexcept;
}

#endif
