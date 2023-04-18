// SPDX-License-Identifier: GPL-3.0-only

#include <windows.h>
#include <vector>
#include <filesystem>
#include <memory>
#include <balltze/event.hpp>
#include "../logger.hpp"
#include "loader.hpp"

namespace Balltze::Plugins {
    using namespace Event;

    static std::vector<std::unique_ptr<Plugin>> plugins;
    static EventListenerHandle<TickEvent> firstTickListener;

    static void load_plugins_dlls() {
        auto plugins_path = get_plugins_path();
        for(auto &dll : std::filesystem::directory_iterator(plugins_path)) {
            if(dll.path().extension() == ".dll") {
                try {
                    plugins.emplace_back(std::make_unique<DLLPlugin>(dll.path()));
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
            if(lua.path().extension() == ".lua") {
                try {
                    plugins.emplace_back(std::make_unique<LuaPlugin>(lua.path()));
                }
                catch(std::runtime_error &) {
                    logger.error("Failed to read Lua script {}", lua.path().filename().string());
                }
            }
        }
    }

    static void load_plugins_first_tick(TickEvent const &context) noexcept {
        for(auto &plugin : plugins) {
            plugin->load();
        }
        firstTickListener.remove();
    }

    void load_plugins() noexcept {
        init_plugins_path();
        logger.info("Initializing plugins...");
        load_plugins_dlls();
        load_plugins_lua();
        for(auto &plugin : plugins) {
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
        firstTickListener = TickEvent::subscribe_const(load_plugins_first_tick, EVENT_PRIORITY_HIGHEST);
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
}
