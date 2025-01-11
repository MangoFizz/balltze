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
    static bool reinit_plugins_on_next_tick;
    static std::optional<std::string> last_map;

    static bool plugin_loaded(std::filesystem::path path) noexcept {
        for(auto &plugin : plugins) {
            if(plugin->filepath() == path) {
                return true;
            }
        }
        return false;
    }

    static bool plugin_should_load_on_map(Plugin *plugin, std::string map) noexcept {
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

    static bool plugin_is_global(Plugin *plugin) noexcept {
        return plugin->maps().empty();
    }

    static void init_plugins() noexcept {
        logger.info("Initializing plugins...");
        init_plugins_path();
        auto plugins_path = get_plugins_path();

        // Load native plugins first so that they can be used by Lua plugins
        for(auto &plugin_file : std::filesystem::directory_iterator(plugins_path)) {
            if(plugin_file.path().extension() == ".dll") {
                if(plugin_loaded(plugin_file.path())) {
                    continue;
                }
                try {
                    plugins.emplace_back(std::make_unique<NativePlugin>(plugin_file.path()));
                }
                catch(std::runtime_error &) {
                    logger.error("Failed to initialize plugin {}", plugin_file.path().filename().string());
                }
            }
        }

        for(auto &plugin_file : std::filesystem::directory_iterator(plugins_path)) {
            if(plugin_file.path().extension() == ".lua") {
                if(plugin_loaded(plugin_file.path())) {
                    continue;
                }
                try {
                    plugins.emplace_back(std::make_unique<LuaPlugin>(plugin_file.path()));
                }
                catch(std::runtime_error &) {
                    logger.error("Failed to initialize plugin {}", plugin_file.path().filename().string());
                }
            }
        }
    }

    static void load_plugin(Plugin *plugin) {
        auto load_result = plugin->load();
        switch(load_result) {
            case PLUGIN_LOAD_SUCCESS:
                logger.debug("Plugin {} loaded successfully.", plugin->name());
                break;
            case PLUGIN_LOAD_FAILURE:
                logger.warning("Plugin {} failed to load.", plugin->name());
                break;
            case PLUGIN_LOAD_INCOMPATIBLE:
                logger.warning("Failed to load plugin {}: plugin is not compatible with this version of Balltze.", plugin->name());
                break;
            case PLUGIN_LOAD_NOT_FOUND:
                logger.warning("Could not find load function in plugin {}.", plugin->name());
                break;
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
                plugin->unload();
            }
        }
    }

    static void load_global_plugins() {
        for(auto &plugin : plugins) {
            if(plugin_is_global(plugin.get()) && !plugin->loaded()) {
                load_plugin(plugin.get());
            }
        }
    }

    static void reinitialize_all_plugins() {
        auto it = plugins.begin();
        while(it != plugins.end()) {
            auto *plugin = it->get();
            if(plugin->reloadable()) {
                if(plugin->loaded()) {
                    plugin->unload();
                }
                it = plugins.erase(it);
            }
            else {
                it++;
            }
        }
        init_plugins();
    }

    static bool first_load = true;

    static void plugins_tick(TickEvent const &context) noexcept {
        if(context.time == EVENT_TIME_AFTER) {
            return;
        }

        if(reinit_plugins_on_next_tick) {
            logger.info("Reloading plugins...");
            reinitialize_all_plugins();
            load_global_plugins();
            if(last_map) {
                load_map_plugins(*last_map);
            }
            else {
                logger.error("FIXME: No map loaded, skipping map plugins loading.");
            }
            reinit_plugins_on_next_tick = false;
        }

        for(auto &plugin : plugins) {
            if(plugin->loaded()) {
                plugin->first_tick();
            }
        }
    }

    static void plugins_frame(FrameEvent const &context) noexcept {
        if(context.time == EVENT_TIME_AFTER) {
            auto lua_plugins = get_lua_plugins();
            for(auto &plugin : lua_plugins) {
                if(plugin->loaded()) {
                    auto *state = plugin->state();
                    lua_gc(state, LUA_GCCOLLECT, 0);
                }
            }
        }
    }

    static void plugins_map_load(MapLoadEvent const &ev) noexcept {
        static bool first_load = true;
        if(ev.time == EVENT_TIME_BEFORE) {
            if(first_load) {
                load_global_plugins();
                first_load = false;
            }
            else {
                unload_map_plugins();
            }
            load_map_plugins(ev.context.name);
            last_map = ev.context.name;
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
        void *upvalue_state = reinterpret_cast<lua_State *>(lua_touserdata(state, lua_upvalueindex(1)));
        for(auto &plugin : plugins) {
            if(auto lua_plugin = dynamic_cast<LuaPlugin *>(plugin.get())) {
                if(lua_plugin->state() == upvalue_state) {
                    return lua_plugin;
                }
            }
        }
        return nullptr;
    }

    NativePlugin *get_dll_plugin(HMODULE handle) noexcept {
        for(auto &plugin : plugins) {
            if(auto dll_plugin = dynamic_cast<NativePlugin *>(plugin.get())) {
                if(dll_plugin->handle() == handle) {
                    return dll_plugin;
                }
            }
        }
        return nullptr;
    }

    void set_up_plugins() noexcept {
        TickEvent::subscribe_const(plugins_tick, EVENT_PRIORITY_HIGHEST);
        FrameEvent::subscribe_const(plugins_frame, EVENT_PRIORITY_HIGHEST);
        MapLoadEvent::subscribe_const(plugins_map_load, EVENT_PRIORITY_HIGHEST);

        init_plugins();
        reinit_plugins_on_next_tick = false;

        register_command("reload_plugins", "plugins", "Reloads all loaded reloadable plugins.", std::nullopt, [](int arg_count, const char **args) -> bool {
            reinit_plugins_on_next_tick = true;
            return true;
        }, false, 0, 0);
    }
}

namespace Balltze {
    std::filesystem::path get_plugin_path(PluginHandle handle) {
        auto *plugin = Plugins::get_dll_plugin(reinterpret_cast<HMODULE>(handle));
        return plugin->directory();
    }
}
