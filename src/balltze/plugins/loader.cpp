// SPDX-License-Identifier: GPL-3.0-only

#include <windows.h>
#include <vector>
#include <filesystem>
#include <memory>
#include <balltze/command.hpp>
#include <balltze/event.hpp>
#include "../logger.hpp"
#include "loader.hpp"

namespace Balltze::Plugins {
    using namespace Event;

    static std::vector<std::unique_ptr<Plugin>> plugins;
    static EventListenerHandle<TickEvent> firstTickListener;

    static bool plugin_loaded(std::filesystem::path path) noexcept {
        for(auto &plugin : plugins) {
            if(plugin->filepath() == path) {
                return true;
            }
        }
        return false;
    }

    static void load_plugins_dlls() {
        auto plugins_path = get_plugins_path();
        for(auto &dll : std::filesystem::directory_iterator(plugins_path)) {
            if(plugin_loaded(dll.path())) {
                continue;
            }

            if(dll.path().extension() == ".dll") {
                try {
                    auto &plugin = plugins.emplace_back(std::make_unique<DLLPlugin>(dll.path()));
                    auto init_result = plugin->init();
                    switch(init_result) {
                        case PLUGIN_INIT_SUCCESS:
                            logger.debug("Plugin {} initialized successfully.", plugin->name());
                            break;
                        case PLUGIN_INIT_FAILURE:
                            logger.error("Plugin {} failed to initialize.", plugin->name());
                            break;
                        case PLUGIN_INIT_NOT_FOUND:
                            logger.warning("Plugin {} does not implement plugin_init.", plugin->name());
                            break;
                    }
                }
                catch(std::runtime_error &) {
                    logger.error("Failed to load module {}", dll.path().filename().string());
                }
            }
        }
    }

    static void load_plugins_lua() {
        auto plugins_path = get_plugins_path();
        for(auto &lua : std::filesystem::directory_iterator(plugins_path)) {
            if(plugin_loaded(lua.path())) {
                continue;
            }

            if(lua.path().extension() == ".lua") {
                try {
                    auto &plugin = plugins.emplace_back(std::make_unique<LuaPlugin>(lua.path()));
                    auto init_result = plugin->init();
                    switch(init_result) {
                        case PLUGIN_INIT_SUCCESS:
                            logger.debug("Plugin {} initialized successfully.", plugin->name());
                            break;
                        case PLUGIN_INIT_FAILURE:
                            logger.error("Plugin {} failed to initialize.", plugin->name());
                            break;
                        case PLUGIN_INIT_NOT_FOUND:
                            logger.warning("Plugin {} does not implement plugin_init.", plugin->name());
                            break;
                    }
                }
                catch(std::runtime_error &e) {
                    logger.error("Failed to read Lua script {}: {}", lua.path().filename().string(), e.what());
                }
            }
        }
    }

    static void load_plugins_first_tick(TickEvent const &context) noexcept {
        for(auto &plugin : plugins) {
            if(plugin->loaded()) {
                continue;
            }
            plugin->load();
        }
        firstTickListener.remove();
    }

    void init_plugins() noexcept {
        init_plugins_path();
        logger.info("Initializing plugins...");
        load_plugins_dlls(); // Load DLL plugins first, so that Lua plugins can use them
        load_plugins_lua();
    }

    void unload_plugins() noexcept {
        logger.info("Unloading plugins...");
        auto it = plugins.begin();
        while(it != plugins.end()) {
            auto *plugin = it->get();
            if(plugin->loaded() && plugin->reloadable()) {
                plugin->unload();
                it = plugins.erase(it);
            }
            else {
                it++;
            }
        }
    }

    std::vector<LuaPlugin *> get_lua_plugins() noexcept {
        std::vector<LuaPlugin *> lua_plugins;
        for(auto &plugin : plugins) {
            if(auto lua_plugin = dynamic_cast<LuaPlugin *>(plugin.get())) {
                lua_plugins.push_back(lua_plugin);
            }
        }
        return lua_plugins;
    }

    LuaPlugin *get_lua_plugin(lua_State *state) noexcept {
        for(auto &plugin : plugins) {
            if(auto lua_plugin = dynamic_cast<LuaPlugin *>(plugin.get())) {
                if(lua_plugin->state() == state) {
                    return lua_plugin;
                }
            }
        }
        return nullptr;
    }

    DLLPlugin *get_dll_plugin(HMODULE handle) noexcept {
        for(auto &plugin : plugins) {
            if(auto dll_plugin = dynamic_cast<DLLPlugin *>(plugin.get())) {
                if(dll_plugin->handle() == handle) {
                    return dll_plugin;
                }
            }
        }
        return nullptr;
    }

    void set_up_plugins() noexcept {
        init_plugins();
        firstTickListener = TickEvent::subscribe_const(load_plugins_first_tick, EVENT_PRIORITY_HIGHEST);

        register_command("reload_plugins", "plugins", "Reloads all loaded plugins.", std::nullopt, [](int arg_count, const char **args) -> bool {
            unload_plugins();
            init_plugins();
            for(auto &plugin : plugins) {
                if(plugin->loaded()) {
                    continue;
                }
                plugin->load();
            }
            return true;
        }, false, 0, 0);
    }
}
