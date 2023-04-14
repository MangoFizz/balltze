// SPDX-License-Identifier: GPL-3.0-only

#include "../logger.hpp"
#include "plugin.hpp"

namespace Balltze::Plugins {
    static std::filesystem::path get_plugin_dll_path(HMODULE dll = NULL) {
        char filename[MAX_PATH];
        GetModuleFileNameA(dll, filename, MAX_PATH);
        return std::filesystem::path(filename);
    }

    void Plugin::get_directory() {
        m_directory = get_plugins_path() / m_metadata.name;
    }

    std::string Plugin::filename() const {
        return m_filename;
    }

    std::string Plugin::name() const {
        return m_metadata.name;
    }

    std::string Plugin::author() const {
        return m_metadata.author;
    }

    VersionNumber Plugin::version() const {
        return m_metadata.version;
    }

    VersionNumber Plugin::balltze_version() const {
        return m_metadata.balltze_version;
    }

    bool Plugin::reloadable() const {
        return m_metadata.reloadable;
    }

    std::filesystem::path Plugin::directory() const {
        return m_directory;
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

    static VersionNumber lua_to_version_number(lua_State* state, int index) {
        VersionNumber version;
        lua_getfield(state, index, "major");
        if(lua_isnumber(state, -1)) {
            version.major = lua_tointeger(state, -1);
        }
        lua_pop(state, 1);
        lua_getfield(state, index, "minor");
        if(lua_isnumber(state, -1)) {
            version.minor = lua_tointeger(state, -1);
        }
        lua_pop(state, 1);
        lua_getfield(state, index, "patch");
        if(lua_isnumber(state, -1)) {
            version.patch = lua_tointeger(state, -1);
        }
        lua_pop(state, 1);
        lua_getfield(state, index, "build");
        if(lua_isnumber(state, -1)) {
            version.build = lua_tointeger(state, -1);
        }
        lua_pop(state, 1);
        return version;
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
                lua_pop(m_state, 1);
                
                lua_getfield(m_state, -1, "author");
                if(lua_isstring(m_state, -1)) {
                    m_metadata.author = lua_tostring(m_state, -1);
                }
                lua_pop(m_state, 1);
                
                lua_getfield(m_state, -1, "version");
                if(lua_istable(m_state, -1)) {
                    m_metadata.version = lua_to_version_number(m_state, -1);
                }
                lua_pop(m_state, 1);
                
                lua_getfield(m_state, -1, "balltze_version");
                if(lua_istable(m_state, -1)) {
                    m_metadata.balltze_version = lua_to_version_number(m_state, -1);
                }
                lua_pop(m_state, 1);

                lua_getfield(m_state, -1, "reloadable");
                if(lua_isboolean(m_state, -1)) {
                    m_metadata.reloadable = lua_toboolean(m_state, -1);
                }
                lua_pop(m_state, 1);
            }
            else {
                logger.warning("plugin_metadata function did not return a table in plugin {}.", m_filename);
                m_metadata.name = m_filepath.stem().string();
            }
            lua_pop(m_state, 1);
        }
        else {
            logger.warning("Could not find plugin_metadata function in plugin {}.", m_filename);
            m_metadata.name = m_filepath.stem().string();
        }
    }

    lua_State* LuaPlugin::state() {
        return m_state;
    }

    PluginInitResult LuaPlugin::init() {
        logger.info("Initializing plugin {}...", m_filename);
        lua_getglobal(m_state, "plugin_init");
        if(lua_isfunction(m_state, -1)) {
            lua_pcall(m_state, 0, 1, 0);
            if(lua_isboolean(m_state, -1)) {
                bool result = lua_toboolean(m_state, -1);
                if(result) {
                    return PluginInitResult::PLUGIN_INIT_SUCCESS;
                }
            }
            else {
                logger.warning("plugin_init function did not return a boolean in plugin {}.", m_filename);
            }
            lua_pop(m_state, 1);
            return PluginInitResult::PLUGIN_INIT_FAILURE;
        }
        else {
            logger.warning("Could not find plugin_init function in plugin {}.", m_filename);
            return PluginInitResult::PLUGIN_INIT_NOT_FOUND;
        }
    }

    void LuaPlugin::load() {
        logger.info("Loading plugin {}...", m_filename);
        lua_getglobal(m_state, "plugin_load");
        if(lua_isfunction(m_state, -1)) {
            lua_pcall(m_state, 0, 0, 0);
        }
        else {
            logger.warning("Could not find plugin_load function in plugin {}.", m_filename);
        }
    }

    LuaPlugin::LuaPlugin(std::filesystem::path lua_file) {
        m_filename = lua_file.filename().string();
        m_filepath = lua_file;
        m_state = luaL_newstate();
        if(m_state) {
            luaL_openlibs(m_state);
            if(luaL_loadfile(m_state, lua_file.string().c_str()) == LUA_OK) {
                lua_pcall(m_state, 0, 0, 0);
                read_metadata();
                get_directory();
            }
            else {
                throw std::runtime_error("Could not load Lua plugin.");
            }
        }
        else {
            throw std::runtime_error("Could not create Lua state.");
        }
    }

    std::filesystem::path get_plugins_path() noexcept {
        return std::filesystem::current_path() / "plugins";
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
