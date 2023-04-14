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
            auto plugin_path = get_plugin_dll_path(m_handle);
            auto plugin_name = plugin_path.stem().string();
            logger.warning("Could not find plugin_metadata function in DLL plugin {}.", plugin_name);
            m_metadata.name = plugin_name;
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
            logger.warning("Could not find plugin_init function in DLL plugin {}.", m_metadata.name);
            return PluginInitResult::PLUGIN_INIT_NOT_FOUND;
        }
    }

    void DLLPlugin::load() {
        logger.info("Starting plugin {}...", m_filename);
        auto load_proc = GetProcAddress(m_handle, "plugin_load");
        if(load_proc) {
            auto load_plugin = reinterpret_cast<plugin_load_proc_t>(load_proc);
            load_plugin();
        }
        else {
            logger.warning("Could not find plugin_load function in DLL plugin {}.", m_filename);
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
