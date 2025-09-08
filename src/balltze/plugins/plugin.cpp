// SPDX-License-Identifier: GPL-3.0-only

#include <nlohmann/json.hpp>
#include "../config/config.hpp"
#include "../lua/api/v2/plugin/commands.hpp"
#include "../lua/api/v2/api.hpp"
#include "../lua/helpers/plugin.hpp"
#include "../lua/libraries/preloaded_libraries.hpp"
#include "../logger.hpp"
#include "../version.hpp"
#include "plugin.hpp"

namespace Balltze::Plugins {
    const PluginMetadata &Plugin::metadata() const noexcept {
        return m_metadata;
    }

    std::filesystem::path Plugin::directory() const noexcept {
        return m_directory;
    }

    std::string Plugin::name() const noexcept {
        return m_metadata.name;
    }

    std::vector<std::string> const &Plugin::maps() const noexcept {
        return m_metadata.maps;
    }

    bool Plugin::reloadable() const noexcept {
        return m_metadata.reloadable;
    }

    bool Plugin::loaded() const noexcept {
        return m_loaded;
    }

    PluginHandle Plugin::handle() {
        return reinterpret_cast<PluginHandle>(this);
    }

    bool Plugin::path_is_valid(std::filesystem::path path) const noexcept {
        return std::filesystem::absolute(path).string().find(std::filesystem::absolute(m_directory).string()) != std::string::npos;
    }

    Plugin::Plugin(std::filesystem::path directory, const PluginMetadata &metadata) : m_directory(std::move(directory)), m_metadata(metadata) {
        if(!std::filesystem::exists(m_directory) || !std::filesystem::is_directory(m_directory)) {
            throw std::invalid_argument("Invalid plugin directory: " + m_directory.string());
        }
        auto main_file = m_directory / m_metadata.plugin_main;
        if(!std::filesystem::exists(main_file)) {
            throw std::runtime_error("Plugin main file not found: " + main_file.string());
        }
    }

    PluginMetadata Plugin::read_manifest(std::filesystem::path manifest_path) {
        PluginMetadata metadata;
        if(!std::filesystem::exists(manifest_path)) {
            throw std::runtime_error("Manifest file does not exist: " + manifest_path.string());
        }

        std::ifstream manifest_file(manifest_path);
        if(!manifest_file.is_open()) {
            throw std::runtime_error("Could not open plugin manifest: " + manifest_path.string());
        }

        try {
            nlohmann::json manifest_json;
            manifest_file >> manifest_json;

            metadata.name = manifest_json.at("name").get<std::string>();
            metadata.author = manifest_json.at("author").get<std::string>();
            metadata.plugin_main = manifest_json.at("plugin_main").get<std::string>();
            metadata.version = semver::version(manifest_json.at("version").get<std::string>());
            metadata.target_api = semver::version(manifest_json.at("target_api").get<std::string>());
            metadata.maps = manifest_json.value("maps", std::vector<std::string>{});
            metadata.reloadable = manifest_json.value("reloadable", false);

            return metadata;
        } 
        catch(const std::exception &e) {
            throw std::runtime_error("Error reading plugin manifest: " + std::string(e.what()));
        }
    }

    HMODULE NativePlugin::module_handle() const noexcept {
        return m_module_handle;
    }

    void NativePlugin::load() {
        if(m_load_failed) {
            return;
        }

        if(m_loaded) {
            throw std::runtime_error("Plugin already loaded: " + m_metadata.name);
        }

        m_module_handle = LoadLibraryA(m_directory.string().c_str());
        if(!m_module_handle) {
            m_load_failed = true;
            throw std::runtime_error("Could not load plugin DLL: " + m_directory.string());
        }

        m_loaded = true;
        logger.info("Plugin {} loaded successfully.", m_metadata.name);
    }

    void NativePlugin::call_on_game_start() {
        if(!m_loaded) {
            throw std::runtime_error("Plugin not loaded: " + m_metadata.name);
        }

        if(m_game_start_called) {
            return;
        }
        m_game_start_called = true;

        auto on_game_start_func = reinterpret_cast<void(*)()>(GetProcAddress(m_module_handle, "plugin_on_game_start"));
        if(on_game_start_func) {
            try {
                on_game_start_func();
            } 
            catch(const std::exception &e) {
                logger.error("Error calling plugin on_game_start function: {}", e.what());
            }
        } else {
            logger.warning("Plugin {} does not have an on_game_start function.", m_metadata.name);
        }
    }

    void NativePlugin::unload() {
        if(m_module_handle) {
            auto unload_func = reinterpret_cast<void(*)()>(GetProcAddress(m_module_handle, "plugin_unload"));
            if(unload_func) {
                try {
                    unload_func();
                } 
                catch(const std::exception &e) {
                    logger.error("Error calling plugin unload function: {}", e.what());
                }
            }
            FreeLibrary(m_module_handle);
            m_module_handle = nullptr;
        }
        m_game_start_called = false;
        m_load_failed = false;
        m_loaded = false;
    }

    NativePlugin::~NativePlugin() {
        if(m_loaded) {
            try {
                logger.debug("Disposing plugin {}...", m_metadata.name);
                unload();
            } 
            catch(const std::runtime_error &e) {
                logger.error("Failed to unload plugin {}: {}", m_metadata.name, e.what());
            }
        }
    }

    lua_State *LuaPlugin::lua_state() const noexcept {
        return m_lua_state;
    }

    Logger *LuaPlugin::plugin_logger() noexcept {
        return m_plugin_logger.get();
    }

    void LuaPlugin::load() {
        if(m_load_failed) {
            return;
        }

        if(m_loaded) {
            throw std::runtime_error("Plugin already loaded: " + m_metadata.name);
        }

        init_lua_state();

        auto script_path = m_directory / m_metadata.plugin_main;
        if(luaL_loadfile(m_lua_state, script_path.string().c_str())) {
            std::string error_msg = lua_tostring(m_lua_state, -1);
            lua_pop(m_lua_state, 1); 
            m_load_failed = true;
            throw std::runtime_error("Could not load Lua plugin main file: " + script_path.string() + " - " + error_msg);
        }

        lua_pushcfunction(m_lua_state, Lua::plugin_error_handler);
        lua_insert(m_lua_state, -2);

        if(lua_pcall(m_lua_state, 0, 0, -2)) {
            m_load_failed = true;
            throw std::runtime_error("Error executing Lua plugin main file: " + pop_error_message());
        }

        m_loaded = true;
    }

    void LuaPlugin::call_on_game_start() {
        if(!m_loaded) {
            throw std::runtime_error("Plugin not loaded: " + m_metadata.name);
        }

        if(m_game_start_called) {
            return;
        }
        m_game_start_called = true;

        lua_getglobal(m_lua_state, "PluginOnGameStart");
        if(lua_isfunction(m_lua_state, -1)) {
            if(lua_pcall(m_lua_state, 0, 0, 0)) {
                logger.error("Error calling PluginOnGameStart in Lua plugin '{}': {}", m_metadata.name, pop_error_message());
            }
        } 
        else {
            lua_pop(m_lua_state, 1); // Remove the non-function value
        }
    }

    void LuaPlugin::unload() {
        if(m_lua_state) {
            lua_pushcfunction(m_lua_state, Lua::plugin_error_handler);
            lua_getglobal(m_lua_state, "PluginUnload");
            if(lua_isfunction(m_lua_state, -1)) {
                if(lua_pcall(m_lua_state, 0, 0, -2)) {
                    logger.error("Error while calling PluginUnload in Lua plugin '{}': {}", m_metadata.name, pop_error_message());
                }
            }
            else {
                lua_pop(m_lua_state, 2);
            }
            lua_close(m_lua_state);
            m_lua_state = nullptr;
        }
        m_game_start_called = false;
        m_load_failed = false;
        m_loaded = false;
    }

    void LuaPlugin::add_tag_import(std::string map_name_or_path, std::string tag_path, LegacyApi::Engine::TagClassInt tag_class) {
        m_tag_imports.push_back({std::move(map_name_or_path), std::move(tag_path), tag_class});
    }

    const std::vector<LuaPlugin::TagImport> &LuaPlugin::imported_tags() const noexcept {
        return m_tag_imports;
    }

    LuaPlugin::LuaPlugin(std::filesystem::path plugin_directory, const PluginMetadata &metadata) : Plugin(std::move(plugin_directory), metadata) {
        m_plugin_logger = std::make_unique<Logger>(m_metadata.name);
    }

    LuaPlugin::~LuaPlugin() {
        if(m_loaded) {
            try {
                logger.debug("Disposing plugin {}...", m_metadata.name);
                unload();
            } 
            catch(const std::runtime_error &e) {
                logger.error("Failed to unload plugin {}: {}", m_metadata.name, e.what());
            }
        }
    }

    void LuaPlugin::init_lua_state() {
        m_lua_state = luaL_newstate();
        if(!m_lua_state) {
            throw std::runtime_error("Could not create Lua state for plugin: " + m_metadata.name);
        }
        
        auto *state = m_lua_state;
        luaL_openlibs(state);
        Lua::Api::open_balltze_api_v2(state);
        Lua::set_preloaded_libraries(state);

        // Remove os.exit, os.getenv and os.execute functions
        lua_getglobal(state, "os");
        lua_pushnil(state);
        lua_setfield(state, -2, "exit");
        lua_pushnil(state);
        lua_setfield(state, -2, "getenv");
        lua_pushnil(state);
        lua_setfield(state, -2, "execute");

        // Set package.path and package.cpath
        lua_getglobal(state, "package");
        auto new_lua_path = m_directory.string() + "\\modules\\?.lua";
        lua_pushstring(state, new_lua_path.c_str());
        lua_setfield(state, -2, "path");
        auto new_lua_cpath = m_directory.string() + "\\modules\\?.dll";
        lua_pushstring(state, new_lua_cpath.c_str());
        lua_setfield(state, -2, "cpath");
        lua_pop(state, 1);
    }

    std::string LuaPlugin::pop_error_message() const noexcept {
        const char *err = lua_tostring(m_lua_state, -1);
        if(err == nullptr) {
            err = "Unknown error (no error message available).";
        }
        lua_pop(m_lua_state, 1);
        return err;
    }

    std::filesystem::path get_plugins_path() noexcept {
        try {
            return Config::get_balltze_directory() / "plugins";
        }
        catch(std::runtime_error &e) {
            logger.warning("Could not get data directory path.");
            logger.warning("Using current path as plugins path (may not work due to permissions).");
            return std::filesystem::current_path() / "plugins";
        }
    }

    void init_plugins_path() {
        try {
            auto plugins_path = get_plugins_path();
            if(!std::filesystem::exists(plugins_path)) {
                logger.info("Creating plugins directory...");
                std::filesystem::create_directory(plugins_path);
            }
        }
        catch(std::filesystem::filesystem_error& e) {
            logger.error("Could not create plugins directory: {}", e.what());
            throw;
        }
    }
} 
