// SPDX-License-Identifier: GPL-3.0-only

#include "lua/api.hpp"
#include "../config/config.hpp"
#include "../logger.hpp"
#include "../version.hpp"
#include "lua/functions/command.hpp"
#include "plugin.hpp"

namespace Balltze::Plugins {
    static std::filesystem::path get_plugin_dll_path(HMODULE dll = NULL) {
        char filename[MAX_PATH];
        GetModuleFileNameA(dll, filename, MAX_PATH);
        return std::filesystem::path(filename);
    }

    std::string Plugin::filename() const noexcept {
        return m_filename;
    }

    std::filesystem::path Plugin::filepath() const noexcept {
        return m_filepath;
    }

    std::string Plugin::name() const noexcept {
        return m_metadata->name;
    }

    std::string Plugin::author() const noexcept {
        return m_metadata->author;
    }

    semver::version Plugin::version() const noexcept {
        return m_metadata->version;
    }

    semver::version Plugin::target_api() const noexcept {
        return m_metadata->target_api;
    }

    bool Plugin::reloadable() const noexcept {
        return m_metadata->reloadable;
    }

    std::filesystem::path Plugin::directory() const noexcept {
        return m_directory;
    }

    bool Plugin::path_is_valid(std::filesystem::path path) const noexcept {
        return std::filesystem::absolute(path).string().find(std::filesystem::absolute(m_directory).string()) != std::string::npos;
    }

    void Plugin::init_data_directory() {
        if(!std::filesystem::exists(m_directory)) {
            try {
                std::filesystem::create_directories(m_directory);
            }
            catch(std::filesystem::filesystem_error& e) {
                logger.error("Could not create plugin data directory: {}", e.what());
                throw;
            }
        }
    }

    bool Plugin::loaded() const noexcept {
        return m_loaded;
    }

    void DLLPlugin::read_metadata() {
        auto metadata_proc = GetProcAddress(m_handle, "plugin_metadata");
        if(metadata_proc) {
            auto get_metadata = reinterpret_cast<plugin_metadata_proc_t>(metadata_proc);
            m_metadata = get_metadata();
        }
        else {
            logger.warning("Could not find plugin_metadata function in plugin {}.", m_filename);
            m_metadata = PluginMetadata();
            m_metadata->name = m_filepath.stem().string();
            m_metadata->reloadable = false;
        }
    }

    void DLLPlugin::get_directory() {
        m_directory = get_plugins_path() / m_filepath.stem().string();
    }

    HMODULE DLLPlugin::handle() {
        return m_handle;
    }

    PluginInitResult DLLPlugin::init() {
        if(m_loaded) {
            throw std::runtime_error("Plugin cannot be initialized after loading.");
        }

        logger.info("Initializing plugin {}...", m_filename);

        // check if plugin is compatible with the current version of Balltze
        if(m_metadata && m_metadata->target_api.major != Balltze::balltze_version.major) {
            return PluginInitResult::PLUGIN_INIT_INCOMPATIBLE;
        }

        auto init_proc = GetProcAddress(m_handle, "plugin_init");
        if(init_proc) {
            try {
                auto init_plugin = reinterpret_cast<plugin_init_proc_t>(init_proc);
                bool result = init_plugin();
                if(result) {
                    return PluginInitResult::PLUGIN_INIT_SUCCESS;
                }
            }
            catch(std::runtime_error& e) {
                logger.error("Failed to initialize plugin {}: {}", m_metadata->name, e.what());
            }
            return PluginInitResult::PLUGIN_INIT_FAILURE;
        }
        else {
            return PluginInitResult::PLUGIN_INIT_NOT_FOUND;
        }
    }

    void DLLPlugin::load() {
        if(m_loaded) {
            throw std::runtime_error("Plugin already unloaded.");
        }

        logger.info("Loading plugin {}...", m_filename);
        auto load_proc = GetProcAddress(m_handle, "plugin_load");
        if(load_proc) {
            auto load_plugin = reinterpret_cast<plugin_load_proc_t>(load_proc);
            load_plugin();
        }
        else {
            logger.warning("Could not find plugin_load function in plugin {}.", m_filename);
        }

        m_loaded = true;
    }

    void DLLPlugin::unload() {
        if(!m_loaded) {
            throw std::runtime_error("Plugin not loaded.");
        }

        logger.info("Unloading plugin {}...", m_filename);
        auto unload_proc = GetProcAddress(m_handle, "plugin_unload");
        if(unload_proc) {
            auto unload_plugin = reinterpret_cast<plugin_unload_proc_t>(unload_proc);
            unload_plugin();
        }
        else {
            logger.warning("Could not find plugin_unload function in plugin {}.", m_filename);
        }
        
        if(reloadable()) {
            FreeLibrary(m_handle);
        }
    }

    DLLPlugin::DLLPlugin(std::filesystem::path dll_file) {
        m_filename = dll_file.filename().string();
        m_filepath = dll_file;
        m_handle = LoadLibraryA(dll_file.string().c_str());
        if(m_handle) {
            read_metadata();
            get_directory();
        }
        else {
            throw std::runtime_error("Could not load DLL plugin.");
        }
    }

    void LuaPlugin::read_metadata() {
        m_metadata = PluginMetadata();
        lua_getglobal(m_state, "PluginMetadata");
        if(lua_isfunction(m_state, -1)) {
            lua_pcall(m_state, 0, 1, 0);
            if(lua_istable(m_state, -1)) {
                lua_getfield(m_state, -1, "name");
                if(lua_isstring(m_state, -1)) {
                    m_metadata->name = lua_tostring(m_state, -1);
                }
                else {
                    logger.warning("Could not read name in plugin {}.", m_filename);
                    m_metadata->name = m_filepath.stem().string();
                }
                lua_pop(m_state, 1);
                
                lua_getfield(m_state, -1, "author");
                if(lua_isstring(m_state, -1)) {
                    m_metadata->author = lua_tostring(m_state, -1);
                }
                else {
                    logger.warning("Could not read author in plugin {}.", m_filename);
                    m_metadata->author = "Unknown";
                }
                lua_pop(m_state, 1);
                
                try {
                    lua_getfield(m_state, -1, "version");
                    if(lua_isstring(m_state, -1)) {
                        auto *version_str = luaL_checkstring(m_state, -1); 
                        m_metadata->version = semver::version{version_str};
                    }
                    lua_pop(m_state, 1);
                    
                    lua_getfield(m_state, -1, "targetApi");
                    if(lua_isstring(m_state, -1)) {
                        auto *version_str = luaL_checkstring(m_state, -1);
                        m_metadata->target_api = semver::version{version_str};
                    }
                    lua_pop(m_state, 1);
                }
                catch(std::runtime_error& e) {
                    logger.warning("Could not read version number in plugin {}.", m_filename);
                }

                lua_getfield(m_state, -1, "reloadable");
                if(lua_isboolean(m_state, -1)) {
                    m_metadata->reloadable = lua_toboolean(m_state, -1);
                }
                else {
                    logger.warning("Could not read reloadable in plugin {}.", m_filename);
                    m_metadata->reloadable = false;
                }
                lua_pop(m_state, 1);
            }
            else {
                logger.warning("PluginMetadata function did not return a table in plugin {}.", m_filename);
                m_metadata->name = m_filepath.stem().string();
            }
            lua_pop(m_state, 1);
        }
        else {
            logger.warning("Could not find PluginMetadata function in plugin {}.", m_filename);
            m_metadata->name = m_filepath.stem().string();
        }
    }

    void LuaPlugin::get_directory() {
        m_directory =  get_plugins_path() / ("lua_" + m_filepath.stem().string());
    }

    lua_State* LuaPlugin::state() noexcept {
        return m_state;
    }

    void LuaPlugin::add_logger(std::string name) {
        if(get_logger(name)) {
            throw std::runtime_error("Logger already exists.");
        }
        m_loggers.push_back(std::make_unique<Logger>(name));
    }

    void LuaPlugin::remove_logger(std::string name) {
        for(auto it = m_loggers.begin(); it != m_loggers.end(); ++it) {
            if((*it)->name() == name) {
                m_loggers.erase(it);
                return;
            }
        }
        throw std::runtime_error("Logger does not exist.");
    }

    Logger *LuaPlugin::get_logger(std::string name) noexcept {
        for(auto& logger : m_loggers) {
            if(logger->name() == name) {
                return logger.get();
            }
        }
        return nullptr;
    }

    void LuaPlugin::add_tag_import(std::string map_name_or_path, std::string tag_path, Engine::TagClassInt tag_class) {
        if(m_tag_imports.find(map_name_or_path) == m_tag_imports.end()) {
            m_tag_imports[map_name_or_path] = std::vector<std::pair<std::string, Engine::TagClassInt>>();
        }
        m_tag_imports[map_name_or_path].push_back(std::make_pair(tag_path, tag_class));
    }

    void LuaPlugin::import_all_tags(std::string map_name_or_path) {
        if(m_tag_imports.find(map_name_or_path) == m_tag_imports.end()) {
            m_tag_imports[map_name_or_path] = std::vector<std::pair<std::string, Engine::TagClassInt>>();
        }
    }

    void LuaPlugin::clear_tag_imports() noexcept {
        m_tag_imports.clear();
    }

    std::map<std::string, std::vector<std::pair<std::string, Engine::TagClassInt>>> const &LuaPlugin::imported_tags() const noexcept {
        return m_tag_imports;
    }

    void LuaPlugin::print_traceback() {
        luaL_traceback(m_state, m_state, NULL, 1);
        const char *traceback = lua_tostring(m_state, -1);
        lua_pop(m_state, 1);
        logger.debug("Traceback: {}", traceback);
    }

    std::string LuaPlugin::get_error_message() {
        const char *err = lua_tostring(m_state, -1);
        if(err == nullptr) {
            err = "Unknown error (no error message available).";
        }
        std::string message = err;
        lua_pop(m_state, 1);
        return err;
    }

    PluginInitResult LuaPlugin::init() {
        if(m_loaded) {
            throw std::runtime_error("Plugin cannot be initialized after loading.");
        }
        
        logger.info("Initializing plugin '{}'...", m_filename);

        // check if plugin is compatible with the current version of Balltze
        if(m_metadata && m_metadata->target_api.major != Balltze::balltze_version.major) {
            return PluginInitResult::PLUGIN_INIT_INCOMPATIBLE;
        }

        lua_getglobal(m_state, "PluginInit");
        if(lua_isfunction(m_state, -1)) {
            auto res = lua_pcall(m_state, 0, 1, 0);
            if(res == LUA_OK) {
                if(lua_isboolean(m_state, -1)) {
                    bool result = lua_toboolean(m_state, -1);
                    lua_pop(m_state, 1);
                    if(result) {
                        return PluginInitResult::PLUGIN_INIT_SUCCESS;
                    }
                }
                lua_pop(m_state, 1);
            }
            else {
                logger.error("Failed to initialize plugin '{}': {}", m_filename, get_error_message());
                print_traceback();
            }
            return PluginInitResult::PLUGIN_INIT_FAILURE;
        }
        else {
            return PluginInitResult::PLUGIN_INIT_NOT_FOUND;
        }
    }

    void LuaPlugin::load() {
        if(m_loaded) {
            throw std::runtime_error("Plugin already loaded.");
        }

        logger.info("Loading plugin {}...", m_filename);
        lua_getglobal(m_state, "PluginLoad");
        if(lua_isfunction(m_state, -1)) {
            auto res = lua_pcall(m_state, 0, 0, 0);
            if(res != LUA_OK) {
                logger.error("Error while loading plugin '{}': {}", m_filename, get_error_message());
                print_traceback();
            }
        }
        else {
            logger.warning("Could not find PluginLoad function in plugin '{}'.", m_filename);
        }
        m_loaded = true;
    }

    void LuaPlugin::unload() {
        if(!m_loaded) {
            throw std::runtime_error("Plugin not loaded.");
        }

        logger.debug("Unloading Lua plugin '{}'...", m_filename);
        lua_getglobal(m_state, "PluginUnload");
        if(lua_isfunction(m_state, -1)) {
            auto res = lua_pcall(m_state, 0, 0, 0);
            if(res != LUA_OK) {
                logger.error("Error while unloading plugin '{}': {}", m_filename, get_error_message());
                print_traceback();
            }
        }
        else {
            logger.warning("Could not find PluginUnload function in plugin '{}'.", m_filename);
        }

        Lua::remove_plugin_commands(this);
        
        if(reloadable()) {
            lua_close(m_state);
        }
    }

    LuaPlugin::LuaPlugin(std::filesystem::path lua_file) {
        m_filename = lua_file.filename().string();
        m_filepath = lua_file;
        m_state = luaL_newstate();
        if(m_state) {
            luaL_openlibs(m_state);
            Lua::open_balltze_api(m_state);
            get_directory();

            // Set package.path and package.cpath
            lua_getglobal(m_state, "package");
            auto new_lua_path = m_directory.string() + "\\modules\\?.lua";
            lua_pushstring(m_state, new_lua_path.c_str());
            lua_setfield(m_state, -2, "path");
            auto new_lua_cpath = get_plugins_path().string() + "\\?.dll;" + m_directory.string() + "\\modules\\?.dll";
            lua_pushstring(m_state, new_lua_cpath.c_str());
            lua_setfield(m_state, -2, "cpath");
            lua_pop(m_state, 1);

            if(luaL_loadfile(m_state, lua_file.string().c_str()) == LUA_OK) {
                int res = lua_pcall(m_state, 0, 0, 0);
                if(res != LUA_OK) {
                    logger.error("Could not execute Lua plugin '{}': {}", m_filename, get_error_message());
                    print_traceback();
                    lua_close(m_state);
                    throw std::runtime_error("Could not execute Lua plugin '" + m_filename + "'.");
                }
                read_metadata();
            }
            else {
                lua_close(m_state);
                throw std::runtime_error("could not read Lua plugin '" + m_filename + "'.");
            }
        }
        else {
            throw std::runtime_error("could not create Lua state for plugin '" + m_filename + "'.");
        }
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
