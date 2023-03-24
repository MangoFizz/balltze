// SPDX-License-Identifier: GPL-3.0-only

#include <windows.h>
#include <vector>
#include <filesystem>
#include <iostream>
#include <balltze/event.hpp>
#include "../config/config.hpp"

namespace Balltze::PluginsLoader {
    using namespace Event;

    enum PluginInitResult {
        PLUGIN_INIT_SUCCESS,
        PLUGIN_INIT_FAILURE,
        PLUGIN_INIT_NOT_FOUND
    };

    static EventListenerHandle<TickEvent> firstTickListener;

    static std::filesystem::path get_plugins_path() {
        auto balltze_path = Config::get_balltze_directory();
        auto plugins_path = balltze_path / "plugins";
        if(!std::filesystem::exists(plugins_path)) {
            std::filesystem::create_directory(plugins_path);
        }
        return plugins_path;
    }

    static std::filesystem::path get_dll_name(HMODULE dll = NULL) {
        char filename[MAX_PATH];
        GetModuleFileNameA(dll, filename, MAX_PATH);
        return std::filesystem::path(filename);
    }

    static std::vector<std::filesystem::path> get_plugins_dlls() {
        std::vector<std::filesystem::path> dlls;
        auto balltze_filename = get_dll_name();
        for(auto &dll : std::filesystem::directory_iterator(get_plugins_path())) {
            if(dll.path().extension() == ".dll" && dll.path() != balltze_filename) {
                dlls.push_back(dll.path());
            }
        }
        return dlls;
    } 


    static HMODULE get_dll_handle(std::filesystem::path const &dll_path) {
        return LoadLibraryA(dll_path.string().c_str());
    }

    static PluginInitResult call_plugin_dll_init(HMODULE dll) {
        auto plugin_init_proc = GetProcAddress(dll, "plugin_init");
        if(plugin_init_proc != NULL) {
            auto plugin_init = reinterpret_cast<bool(*)()>(plugin_init_proc);
            if(plugin_init()) {
                return PLUGIN_INIT_SUCCESS;
            }
            else {
                return PLUGIN_INIT_FAILURE;
            }
        }
        return PLUGIN_INIT_NOT_FOUND;
    }

    static bool call_plugin_dll_start(HMODULE dll) {
        auto plugin_start_proc = GetProcAddress(dll, "plugin_start");
        if(plugin_start_proc != NULL) {
            auto plugin_start = reinterpret_cast<void(*)()>(plugin_start_proc);
            plugin_start();
            return true;
        }
        return false;
    }

    static void start_plugins_first_tick(TickEvent const &context) noexcept {
        auto plugins_dlls = get_plugins_dlls();
        for(auto &dll : plugins_dlls) {
            auto handle = get_dll_handle(dll);
            if(handle != NULL) {
                auto result = call_plugin_dll_start(handle);
                auto plugin_name = get_dll_name(handle);
                if(result) {
                    std::cout << "Plugin " << plugin_name << " started successfully" << std::endl;
                }
                else {
                    std::cout << "Plugin " << plugin_name << " failed to start" << std::endl;
                }
            }
        }
        firstTickListener.remove();
    }

    void load_plugins() {
        std::cout << "Loading plugins..." << std::endl;
        auto plugins_dlls = get_plugins_dlls();
        for(auto &dll : plugins_dlls) {
            std::cout << "Loading " << dll << std::endl;
            auto handle = get_dll_handle(dll);
            if(handle != NULL) {
                auto result = call_plugin_dll_init(handle);
                if(result == PLUGIN_INIT_SUCCESS) {
                    std::cout << "Plugin " << dll << " initialized successfully" << std::endl;
                }
                else if(result == PLUGIN_INIT_FAILURE) {
                    std::cout << "Plugin " << dll << " failed to initialize" << std::endl;
                }
                else if(result == PLUGIN_INIT_NOT_FOUND) {
                    std::cout << "Plugin " << dll << " does not have a plugin_init function" << std::endl;
                }
            }
            else {
                std::cout << "Failed to load " << dll << std::endl;
            }
        }

        // Start plugins on first tick
        firstTickListener = TickEvent::subscribe_const(start_plugins_first_tick, EVENT_PRIORITY_HIGHEST);
    }
}
