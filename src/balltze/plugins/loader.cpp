// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/events.hpp>
#include <balltze/command.hpp>
#include "../logger.hpp"
#include "plugin.hpp"
#include "loader.hpp"

using namespace Balltze::Events;

namespace Balltze::LegacyApi::Plugins {
    extern bool reinit_plugins_on_next_tick;
}

namespace Balltze::Plugins {
    static bool reload_plugins_on_next_tick = false;
    static std::vector<std::unique_ptr<Plugin>> plugins;

    static bool plugin_is_global(Plugin *plugin) noexcept {
        return plugin->maps().empty();
    }

    static bool plugin_should_load_on_map(Plugin *plugin, const std::string &map) noexcept {
        if(plugin->maps().empty()) {
            return true;
        }
        for(auto &m : plugin->maps()) {
            if(m == map) {
                return true;
            }
        }
        return false;
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
        void *upvalue_state = reinterpret_cast<lua_State *>(lua_touserdata(state, lua_upvalueindex(1)));
        for(auto &plugin : plugins) {
            if(auto lua_plugin = dynamic_cast<LuaPlugin *>(plugin.get())) {
                if(lua_plugin->lua_state() == upvalue_state) {
                    return lua_plugin;
                }
            }
        }
        return nullptr;
    }

    static void load_plugin(Plugin *plugin) {
        logger.debug("Loading plugin {}...", plugin->name());
        try {
            if(!plugin->loaded()) {
                plugin->load();
            }
        }
        catch(std::runtime_error &e) {
            logger.error("Failed to load plugin {}: {}", plugin->name(), e.what());
        }
    }

    static void unload_plugin(Plugin *plugin) {
        logger.debug("Unloading plugin {}...", plugin->name());
        try {
            plugin->unload();
        }
        catch(std::runtime_error &e) {
            logger.error("Failed to unload plugin {}: {}", plugin->name(), e.what());
        }
    }

    static void load_global_plugins() {
        for(auto &plugin : plugins) {
            if(plugin_is_global(plugin.get()) && !plugin->loaded()) {
                load_plugin(plugin.get());
            }
        }
    }

    static void load_map_plugins(std::string map) {
        for(auto &plugin : plugins) {
            if(!plugin_is_global(plugin.get()) && plugin_should_load_on_map(plugin.get(), map)) {
                load_plugin(plugin.get());
            }
        }
    }

    static void unload_map_plugins() {
        for(auto &plugin : plugins) {
            if(!plugin_is_global(plugin.get()) && plugin->loaded()) {
                unload_plugin(plugin.get());
            }
        }
    }

    static void unload_all_plugins() {
        for(auto &plugin : plugins) {
            if(plugin->reloadable() && plugin->loaded()) {
                plugin->unload();
            }
        }
    }

    static void plugins_tick(TickEvent const &context) noexcept {
        if(reload_plugins_on_next_tick) {
            logger.info("Reloading plugins...");
            for(auto &plugin : plugins) {
                if(plugin->reloadable() && plugin->loaded()) {
                    unload_plugin(plugin.get());
                    load_plugin(plugin.get());
                }
            }
            reload_plugins_on_next_tick = false;
        }

        for(auto &plugin : plugins) {
            if(plugin->loaded()) {
                plugin->call_on_game_start();
            }
        }
    }

    static void lua_plugins_collect_garbage(FrameEndEvent const &context) noexcept {
        auto lua_plugins = get_lua_plugins();
        for(auto &plugin : lua_plugins) {
            auto *state = plugin->lua_state();
            if(state) {
                lua_gc(state, LUA_GCCOLLECT, 0);
            }
        }
    }

    static void plugins_map_load(const MapLoadEvent &context) noexcept {
        auto map_name = context.map_name();
        load_global_plugins();
        unload_map_plugins();
        load_map_plugins(map_name);
    }

    static void initialize_plugins() {
        logger.info("Initializing plugins...");
        auto plugins_path = get_plugins_path();
        for(auto &directory_entry : std::filesystem::directory_iterator(plugins_path)) {
            if(directory_entry.is_directory()) {
                auto path = directory_entry.path();
                try {
                    auto metadata = Plugin::read_manifest(path / "manifest.json");
                    if(metadata.plugin_main.empty()) {
                        logger.error("Plugin {} has no main file specified in manifest.", metadata.name);
                        continue;
                    }
                    std::filesystem::path main_file = path.stem() / metadata.plugin_main;
                    logger.debug("Initializing plugin {}...", main_file.string());
                    if(metadata.plugin_main.ends_with(".dll")) {
                        plugins.emplace_back(std::make_unique<NativePlugin>(path, metadata));
                    } 
                    else if(metadata.plugin_main.ends_with(".lua")) {
                        plugins.emplace_back(std::make_unique<LuaPlugin>(path, metadata));
                    } 
                    else {
                        logger.error("Plugin {} has an unsupported main file type: {}", metadata.name, metadata.plugin_main);
                        continue;
                    }
                }
                catch(std::runtime_error &e) {
                    logger.debug("Failed to initialize plugin {}: {}", directory_entry.path().filename().string(), e.what());
                }
                catch(std::exception &e) {
                    logger.debug("Failed to initialize plugin {}: {}", directory_entry.path().filename().string(), e.what());
                }
            }
        }
    }

    void set_up_plugins_loader() noexcept {       
        init_plugins_path();
        initialize_plugins();
        
        TickEvent::subscribe(plugins_tick, EVENT_PRIORITY_HIGHEST);
        FrameEvent::subscribe(lua_plugins_collect_garbage, EVENT_PRIORITY_LOWEST);
        MapLoadEvent::subscribe(plugins_map_load, EVENT_PRIORITY_HIGHEST);

        register_command("reload_plugins", "plugins", "Reloads all loaded reloadable plugins.", std::nullopt, [](int arg_count, const char **args) -> bool {
            reload_plugins_on_next_tick = true;
            LegacyApi::Plugins::reinit_plugins_on_next_tick = true;
            logger.info("Plugins reloading scheduled for next tick.");
            return true;
        }, false, 0, 0);
    }
}
