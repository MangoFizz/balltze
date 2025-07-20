// SPDX-License-Identifier: GPL-3.0-only

#include "../../config/config.hpp"
#include "../../logger.hpp"
#include "../../version.hpp"
#include "../../lua/api/v1/api.hpp"
#include "../../lua/api/v1/plugin/commands.hpp"
#include "../../lua/api/v2/api.hpp"
#include "../../lua/libraries/preloaded_libraries.hpp"
#include "../../plugins/loader.hpp"
#include "plugin.hpp"

namespace Balltze {
    extern std::vector<std::shared_ptr<Command>> commands;
}

namespace Balltze::LegacyApi::Plugins {
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

    std::vector<std::string> const &Plugin::maps() const noexcept {
        return m_metadata->maps;
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
        return initialized() && m_loaded;
    }

    void NativePlugin::update_metadata() {
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

    void NativePlugin::set_up_directory() {
        m_directory = Balltze::Plugins::get_plugins_path() / m_filepath.stem().string();
        init_data_directory();
    }

    HMODULE NativePlugin::handle() {
        if(!m_handle) {
            logger.warning("Trying to get handle of unloaded native plugin {}.", m_filename);
        }
        return m_handle;
    }

    bool NativePlugin::initialized() const noexcept {
        return m_handle != nullptr;
    }

    void NativePlugin::init() {
        if(m_handle) {
            throw std::runtime_error("plugin already initialized");
        }
        m_handle = LoadLibraryA(m_filepath.string().c_str());
        if(m_handle) {
            update_metadata();
            set_up_directory();
        }
        else {
            throw std::runtime_error("could not load plugin");
        }
    }

    PluginLoadResult NativePlugin::load() {
        logger.info("Loading plugin {}...", m_filename);

        if(m_loaded) {
            throw std::runtime_error("tried to load a plugin that is already loaded");
        }
        m_loaded = true;

        if(!m_handle) {
            try {
                init();
            }
            catch(std::runtime_error &e) {
                logger.error("Failed to initialize plugin {}: {}", m_filename, e.what());
                return PluginLoadResult::PLUGIN_LOAD_FAILURE;
            }
        }

        // check if plugin is compatible with the current version of Balltze
        if(m_metadata && m_metadata->target_api.major != Balltze::balltze_version.major) {
            return PluginLoadResult::PLUGIN_LOAD_INCOMPATIBLE;
        }

        auto load_proc = GetProcAddress(m_handle, "plugin_load");
        if(load_proc) {
            try {
                auto load_plugin = reinterpret_cast<plugin_load_proc_t>(load_proc);
                bool result = load_plugin();
                if(result) {
                    m_execute_first_tick = true;
                    return PluginLoadResult::PLUGIN_LOAD_SUCCESS;
                }
            }
            catch(std::runtime_error& e) {
                logger.error("Failed to load plugin {}: {}", m_metadata->name, e.what());
            }
            return PluginLoadResult::PLUGIN_LOAD_FAILURE;
        }
        else {
            return PluginLoadResult::PLUGIN_LOAD_NOT_FOUND;
        }
    }

    void NativePlugin::unload() {
        if(!m_handle) {
            throw std::runtime_error("tried to unload plugin that is not initialized");
        }
        if(!m_loaded) {
            throw std::runtime_error("tried to unload plugin that is not loaded");
        }

        logger.info("Unloading plugin {}...", m_filename);
        auto unload_proc = GetProcAddress(m_handle, "plugin_unload");
        if(unload_proc) {
            auto unload_plugin = reinterpret_cast<plugin_unload_proc_t>(unload_proc);
            unload_plugin();
        }

        remove_commands_from_plugin(this);
        
        dispose();
        m_loaded = false;
    }

    void NativePlugin::dispose() {
        if(!m_handle) {
            throw std::runtime_error("plugin not initialized");
        }
        logger.debug("Disposing plugin {}...", m_filename);
        FreeLibrary(m_handle);
        m_handle = nullptr;
    }

    void NativePlugin::first_tick() {
        if(!m_handle) {
            throw std::runtime_error("plugin not initialized");
        }
        if(!m_execute_first_tick) {
            return;
        }
        auto first_tick_proc = GetProcAddress(m_handle, "plugin_first_tick");
        if(first_tick_proc) {
            auto first_tick = reinterpret_cast<plugin_first_tick_proc_t>(first_tick_proc);
            first_tick();
        }
        m_execute_first_tick = false;
    }

    NativePlugin::NativePlugin(std::filesystem::path dll_file) {
        m_filename = dll_file.filename().string();
        m_filepath = dll_file;
        m_handle = nullptr;
        try {
            init();
        }
        catch(std::runtime_error& e) {
            logger.error("Failed to initialize plugin {}: {}", m_filename, e.what());
            throw;
        }
    }

    NativePlugin::~NativePlugin() {
        if(m_handle) {
            dispose();
        }
    }

    void LuaPlugin::update_metadata() {
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
                    logger.warning("Could not read plugin version/target api version in plugin {}.", m_filename);
                }

                lua_getfield(m_state, -1, "maps");
                if(lua_istable(m_state, -1)) {
                    lua_pushnil(m_state);
                    while(lua_next(m_state, -2) != 0) {
                        if(lua_isstring(m_state, -1)) {
                            m_metadata->maps.push_back(lua_tostring(m_state, -1));
                        }
                        lua_pop(m_state, 1);
                    }
                }
                lua_pop(m_state, 1);

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

    void LuaPlugin::set_up_directory() {
        m_directory = Balltze::Plugins::get_plugins_path() / ("lua_" + m_filepath.stem().string());
        init_data_directory();
    }

    lua_State* LuaPlugin::state() noexcept {
        if(!m_state) {
            logger.warning("Trying to get state of unloaded Lua plugin {}.", m_filename);
        }
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

    void LuaPlugin::add_tag_import(std::string map_name_or_path, std::string tag_path, LegacyApi::Engine::TagClassInt tag_class) {
        if(m_tag_imports.find(map_name_or_path) == m_tag_imports.end()) {
            m_tag_imports[map_name_or_path] = std::vector<std::pair<std::string, LegacyApi::Engine::TagClassInt>>();
        }
        m_tag_imports[map_name_or_path].push_back(std::make_pair(tag_path, tag_class));
    }

    void LuaPlugin::import_all_tags(std::string map_name_or_path) {
        if(m_tag_imports.find(map_name_or_path) == m_tag_imports.end()) {
            m_tag_imports[map_name_or_path] = std::vector<std::pair<std::string, LegacyApi::Engine::TagClassInt>>();
        }
    }

    void LuaPlugin::clear_tag_imports() noexcept {
        m_tag_imports.clear();
    }

    std::map<std::string, std::vector<std::pair<std::string, LegacyApi::Engine::TagClassInt>>> const &LuaPlugin::imported_tags() const noexcept {
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

    bool LuaPlugin::initialized() const noexcept {
        return m_state != nullptr;
    }

    void LuaPlugin::init() {
        if(m_state) {
            throw std::runtime_error("plugin already initialized");
        }
        m_state = luaL_newstate();
        if(m_state) {
            // Open standard libraries and Balltze API
            luaL_openlibs(m_state);
            Lua::Api::open_balltze_api_v1(m_state);

            // Set up preloaded libraries
            Lua::set_preloaded_libraries(m_state);

            // Remove os.exit, os.getenv and os.execute functions
            lua_getglobal(m_state, "os");
            lua_pushnil(m_state);
            lua_setfield(m_state, -2, "exit");
            lua_pushnil(m_state);
            lua_setfield(m_state, -2, "getenv");
            lua_pushnil(m_state);
            lua_setfield(m_state, -2, "execute");
            
            // Get plugin directory
            set_up_directory();
                 
            // Set package.path and package.cpath
            lua_getglobal(m_state, "package");
            auto new_lua_path = m_directory.string() + "\\modules\\?.lua";
            lua_pushstring(m_state, new_lua_path.c_str());
            lua_setfield(m_state, -2, "path");
            auto new_lua_cpath = Balltze::Plugins::get_plugins_path().string() + "\\?.dll;" + m_directory.string() + "\\modules\\?.dll";
            lua_pushstring(m_state, new_lua_cpath.c_str());
            lua_setfield(m_state, -2, "cpath");
            lua_pop(m_state, 1);

            if(luaL_loadfile(m_state, m_filepath.string().c_str()) == LUA_OK) {
                int res = lua_pcall(m_state, 0, 0, 0);
                if(res != LUA_OK) {
                    logger.error("Could not execute Lua plugin '{}': {}", m_filename, get_error_message());
                    print_traceback();
                    lua_close(m_state);
                    throw std::runtime_error("Could not execute Lua plugin '" + m_filename + "'.");
                }
                update_metadata();
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

    PluginLoadResult LuaPlugin::load() {
        logger.info("Loading plugin {}...", m_filename);

        if(m_loaded) {
            throw std::runtime_error("tried to load a plugin that is already loaded");
        }
        m_loaded = true;

        if(!m_state) {
            try {
                init();
            }
            catch(std::runtime_error &e) {
                logger.error("Failed to initialize plugin '{}': {}", m_filename, e.what());
                return PluginLoadResult::PLUGIN_LOAD_FAILURE;
            }
        }
        
        if(m_metadata && m_metadata->target_api.major != Balltze::balltze_version.major) {
            return PluginLoadResult::PLUGIN_LOAD_INCOMPATIBLE;
        }

        lua_getglobal(m_state, "PluginLoad");
        if(lua_isfunction(m_state, -1)) {
            auto res = lua_pcall(m_state, 0, 1, 0);
            if(res == LUA_OK) {
                if(lua_isboolean(m_state, -1)) {
                    bool result = lua_toboolean(m_state, -1);
                    lua_pop(m_state, 1);
                    if(result) {
                        m_execute_first_tick = true;
                        return PluginLoadResult::PLUGIN_LOAD_SUCCESS;
                    }
                }
                lua_pop(m_state, 1);
            }
            else {
                logger.error("Failed to load plugin '{}': {}", m_filename, get_error_message());
                print_traceback();
            }
            return PluginLoadResult::PLUGIN_LOAD_FAILURE;
        }
        else {
            return PluginLoadResult::PLUGIN_LOAD_NOT_FOUND;
        }
    }

    void LuaPlugin::unload() {
        if(!m_state) {
            throw std::runtime_error("plugin already disposed");
        }
        if(!m_loaded) {
            throw std::runtime_error("tried to unload plugin that is not loaded");
        }

        logger.debug("Unloading Lua plugin '{}'...", m_filename);
        lua_getglobal(m_state, "PluginUnload");
        if(lua_isfunction(m_state, -1)) {
            auto res = lua_pcall(m_state, 0, 0, 0);
            if(res != LUA_OK) {
                logger.error("Error while unloading Lua plugin '{}': {}", m_filename, get_error_message());
                print_traceback();
            }
        }

        remove_plugin_commands(this);

        dispose();
        m_loaded = false;
    }

    void LuaPlugin::dispose() {
        if(!m_state) {
            throw std::runtime_error("plugin already disposed");
        }
        logger.debug("Disposing Lua plugin '{}'...", m_filename);
        lua_close(m_state);
        m_state = nullptr;
    }

    void LuaPlugin::first_tick() {
        if(!m_state) {
            throw std::runtime_error("plugin not initialized");
        }
        if(!m_execute_first_tick) {
            return;
        }
        lua_getglobal(m_state, "PluginFirstTick");
        if(lua_isfunction(m_state, -1)) {
            auto res = lua_pcall(m_state, 0, 0, 0);
            if(res != LUA_OK) {
                logger.error("Error while calling PluginFirstTick in Lua plugin '{}': {}", m_filename, get_error_message());
                print_traceback();
            }
        }
        m_execute_first_tick = false;
    }

    LuaPlugin::LuaPlugin(std::filesystem::path lua_file) {
        m_filename = lua_file.filename().string();
        m_filepath = lua_file;
        m_state = nullptr;
        try {
            init();
        }
        catch(std::runtime_error &e) {
            logger.error("Failed to initialize plugin '{}': {}", m_filename, e.what());
            throw;
        }
    }

    LuaPlugin::~LuaPlugin() {
        if(m_state) {
            dispose();
        }
    }

    void remove_plugin_commands(LuaPlugin *plugin) noexcept {
        logger.debug("Removing commands from plugin {}", plugin->name());
        for(auto it = commands.begin(); it != commands.end();) {
            if((*it)->plugin() == reinterpret_cast<PluginHandle>(plugin)) {
                it = commands.erase(it);
            }
            else {
                it++;
            }
        }   
    }
}
