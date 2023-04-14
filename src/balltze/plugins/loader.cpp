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
                    logger.error("Failed to load plugin {}", dll.path().filename().string());
                }
            }
        }
    }

    static void load_plugins_first_tick(TickEvent const &context) noexcept {
        for(auto &plugin : plugins) {
            logger.debug("Loading {}...", plugin->name());
            plugin->load();
        }
        firstTickListener.remove();
    }

    void load_plugins() noexcept {
        init_plugins_path();
        logger.info("Initializing plugins...");
        load_plugins_dlls();
        for(auto &plugin : plugins) {
            logger.debug("Initializing {}...", plugin->name());
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
}
