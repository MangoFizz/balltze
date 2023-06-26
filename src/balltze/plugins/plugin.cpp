// SPDX-License-Identifier: GPL-3.0-only

#include "../logger.hpp"
#include "lua/api.hpp"
#include "lua/helpers.hpp"
#include "../config/config.hpp"
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

    std::string Plugin::name() const noexcept {
        return m_metadata.name;
    }

    std::string Plugin::author() const noexcept {
        return m_metadata.author;
    }

    VersionNumber Plugin::version() const noexcept {
        return m_metadata.version;
    }

    VersionNumber Plugin::balltze_version() const noexcept {
        return m_metadata.balltze_version;
    }

    bool Plugin::reloadable() const noexcept {
        return m_metadata.reloadable;
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

    void DLLPlugin::read_metadata() {
        auto metadata_proc = GetProcAddress(m_handle, "plugin_metadata");
        if(metadata_proc) {
            auto get_metadata = reinterpret_cast<plugin_metadata_proc_t>(metadata_proc);
            m_metadata = get_metadata();
        }
        else {
            logger.warning("Could not find plugin_metadata function in plugin {}.", m_filename);
            m_metadata.name = m_filepath.stem().string();
        }
    }

    void DLLPlugin::get_directory() {
        m_directory = get_plugins_path() / m_metadata.name;
    }

    HMODULE DLLPlugin::handle() {
        return m_handle;
    }

    PluginInitResult DLLPlugin::init() {
        logger.info("Initializing plugin {}...", m_filename);
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
                logger.error("Could not initialize plugin {}: {}", m_metadata.name, e.what());
            }
            return PluginInitResult::PLUGIN_INIT_FAILURE;
        }
        else {
            logger.warning("Could not find plugin_init function in plugin {}.", m_metadata.name);
            return PluginInitResult::PLUGIN_INIT_NOT_FOUND;
        }
    }

    void DLLPlugin::load() {
        logger.info("Loading plugin {}...", m_filename);
        auto load_proc = GetProcAddress(m_handle, "plugin_load");
        if(load_proc) {
            auto load_plugin = reinterpret_cast<plugin_load_proc_t>(load_proc);
            load_plugin();
        }
        else {
            logger.warning("Could not find plugin_load function in plugin {}.", m_filename);
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
        lua_getglobal(m_state, "plugin_metadata");
        if(lua_isfunction(m_state, -1)) {
            lua_pcall(m_state, 0, 1, 0);
            if(lua_istable(m_state, -1)) {
                lua_getfield(m_state, -1, "name");
                if(lua_isstring(m_state, -1)) {
                    m_metadata.name = lua_tostring(m_state, -1);
                }
                else {
                    logger.warning("Could not read name in plugin {}.", m_filename);
                    m_metadata.name = m_filepath.stem().string();
                }
                lua_pop(m_state, 1);
                
                lua_getfield(m_state, -1, "author");
                if(lua_isstring(m_state, -1)) {
                    m_metadata.author = lua_tostring(m_state, -1);
                }
                else {
                    logger.warning("Could not read author in plugin {}.", m_filename);
                    m_metadata.author = "Unknown";
                }
                lua_pop(m_state, 1);
                
                try {
                    lua_getfield(m_state, -1, "version");
                    if(lua_istable(m_state, -1)) {
                        m_metadata.version = lua_check_version_number(m_state, -1);
                    }
                    lua_pop(m_state, 1);
                    
                    lua_getfield(m_state, -1, "balltze_version");
                    if(lua_istable(m_state, -1)) {
                        m_metadata.balltze_version = lua_check_version_number(m_state, -1);
                    }
                    lua_pop(m_state, 1);
                }
                catch(std::runtime_error& e) {
                    Balltze::logger.warning("Could not read version number in plugin {}.", m_filename);
                }

                lua_getfield(m_state, -1, "reloadable");
                if(lua_isboolean(m_state, -1)) {
                    m_metadata.reloadable = lua_toboolean(m_state, -1);
                }
                else {
                    Balltze::logger.warning("Could not read reloadable in plugin {}.", m_filename);
                    m_metadata.reloadable = false;
                }
                lua_pop(m_state, 1);
            }
            else {
                Balltze::logger.warning("plugin_metadata function did not return a table in plugin {}.", m_filename);
                m_metadata.name = m_filepath.stem().string();
            }
            lua_pop(m_state, 1);
        }
        else {
            Balltze::logger.warning("Could not find plugin_metadata function in plugin {}.", m_filename);
            m_metadata.name = m_filepath.stem().string();
        }
    }

    void LuaPlugin::get_directory() {
        m_directory =  get_plugins_path() / ("lua_" + m_metadata.name);
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

    PluginInitResult LuaPlugin::init() {
        Balltze::logger.info("Initializing plugin {}...", m_filename);
        lua_getglobal(m_state, "plugin_init");
        if(lua_isfunction(m_state, -1)) {
            auto res = lua_pcall(m_state, 0, 1, 0);
            if(res == LUA_OK) {
                if(lua_isboolean(m_state, -1)) {
                    bool result = lua_toboolean(m_state, -1);
                    if(result) {
                        Balltze::logger.debug("Plugin {} initialized successfully.", m_filename);
                        return PluginInitResult::PLUGIN_INIT_SUCCESS;
                    }
                }
                else {
                    Balltze::logger.warning("plugin_init function did not return a boolean in plugin {}.", m_filename);
                }
            }
            else {
                const char *err = lua_tostring(m_state, -1);
                Balltze::logger.error("Error while initializing plugin {}: {}", m_filename, err);
                lua_pop(m_state, 1);
            }
            return PluginInitResult::PLUGIN_INIT_FAILURE;
        }
        else {
            Balltze::logger.warning("Could not find plugin_init function in plugin {}.", m_filename);
            return PluginInitResult::PLUGIN_INIT_NOT_FOUND;
        }
    }

    void LuaPlugin::load() {
        Balltze::logger.info("Loading plugin {}...", m_filename);
        lua_getglobal(m_state, "plugin_load");
        if(lua_isfunction(m_state, -1)) {
            auto res = lua_pcall(m_state, 0, 0, 0);
            if(res != LUA_OK) {
                const char *err = lua_tostring(m_state, -1);
                if(err == nullptr) {
                    err = "Unknown error.";
                }
                Balltze::logger.error("Error while loading plugin {}: {}", m_filename, err);
                lua_pop(m_state, 1);
            }
        }
        else {
            Balltze::logger.warning("Could not find plugin_load function in plugin {}.", m_filename);
        }
    }

    LuaPlugin::LuaPlugin(std::filesystem::path lua_file) {
        m_filename = lua_file.filename().string();
        m_filepath = lua_file;
        m_state = luaL_newstate();
        if(m_state) {
            luaL_openlibs(m_state);
            lua_open_balltze_api(m_state);
            if(luaL_loadfile(m_state, lua_file.string().c_str()) == LUA_OK) {
                int res = lua_pcall(m_state, 0, 0, 0);
                if(res != LUA_OK) {
                    const char *err = lua_tostring(m_state, -1);
                    Balltze::logger.error("Error while loading plugin {}: {}", m_filename, err);
                    lua_pop(m_state, 1);
                    lua_close(m_state);
                    throw std::runtime_error("Could not execute Lua plugin.");
                }
                read_metadata();
                get_directory();
            }
            else {
                lua_close(m_state);
                throw std::runtime_error("Could not load Lua plugin.");
            }
        }
        else {
            lua_close(m_state);
            throw std::runtime_error("Could not create Lua state.");
        }
    }

    std::filesystem::path get_plugins_path() noexcept {
        try {
            return Engine::get_path() / "plugins";
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
