// SPDX-License-Identifier: GPL-3.0-only

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <lua.hpp>
#include <clipboardxx/clipboardxx.hpp>
#include <balltze/api.hpp>
#include <balltze/engine.hpp>
#include <balltze/event.hpp>
#include <balltze/features.hpp>
#include <balltze/utils.hpp>
#include "../../../../version.hpp"
#include "../../../../logger.hpp"
#include "../../../../plugins/loader.hpp"

namespace Balltze::Lua::Api::V1 {
    int populate_chimera_table(lua_State *state) noexcept;
}

namespace Balltze::LuaLibrary {
    lua_State *balltze_chimera_script = nullptr;

    enum struct CallbackTargetedEvent {
        MAP_FILE_LOAD
    };

    struct Script {
        std::string name;
        std::string type;
        std::map<CallbackTargetedEvent, std::string> callbacks;
        lua_State *state;
    };

    static std::vector<std::unique_ptr<Script>> scripts;
    static bool allow_tag_data_import = true;
    static std::optional<Event::EventListenerHandle<Event::MapLoadEvent>> map_load_listener_handle;

    static Script *get_script(lua_State *state) noexcept {
        for(auto &script : scripts) {
            if(script->state == state) {
                return script.get();
            }
        }
        return nullptr;
    }

    static int lua_set_callback(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 2) {
            std::string callback_name = luaL_checkstring(state, 1);
            std::string function = luaL_checkstring(state, 2);

            static constexpr const char *callback_names[] = {
                "map file load"
            };

            std::size_t i;
            std::size_t n = sizeof(callback_names) / sizeof(const char *);
            for(i = 0; i < n; i++) {
                if(callback_name == callback_names[i]) {
                    auto *script  = get_script(state);
                    script->callbacks[static_cast<CallbackTargetedEvent>(i)] = function;
                    break;
                }
            }

            if(i == n) {
                luaL_error(state, "invalid callback in balltze set_callback function");
            }
        }
        else {
            luaL_error(state, "invalid number of arguments in balltze set_callback function");
        }
        return 0;
    }

    static int lua_import_tag_data(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 3) {
            std::string map_name = luaL_checkstring(state, 1);
            std::string tag_path = luaL_checkstring(state, 2);
            const char *tag_class_name = luaL_checkstring(state, 3);
            auto tag_class = Engine::tag_class_from_string(tag_class_name);

            if(tag_class == Engine::TAG_CLASS_NULL) {
                luaL_error(state, "invalid tag class in balltze import_tag_data function");
            }

            if(!allow_tag_data_import) {
                luaL_error(state, "balltze import_tag_data function is not allowed in this context");
            }

            Features::import_tag_from_map(map_name, tag_path, tag_class);
        }
        else {
            luaL_error(state, "invalid number of arguments in balltze import_tag_data function");
        }
        return 0;
    }

    static int lua_unit_enter_vehicle(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 3) {
            Engine::ObjectHandle unit_handle(luaL_checkinteger(state, 1));
            Engine::ObjectHandle vehicle_handle(luaL_checkinteger(state, 2));
            // TODO Add support for using seat labels too instead of seat indices
            int seat_index = luaL_checkinteger(state, 3);
            auto &object_table = Engine::get_object_table();
            auto *unit = reinterpret_cast<Engine::UnitObject *>(object_table.get_object(unit_handle));
            auto *vehicle = reinterpret_cast<Engine::UnitObject *>(object_table.get_object(vehicle_handle));
            if(!unit) {
                return luaL_error(state, "invalid unit handle in balltze unit_enter_vehicle");
            }
            if(!unit->type == Engine::OBJECT_TYPE_BIPED && !unit->type == Engine::OBJECT_TYPE_VEHICLE) {
                return luaL_error(state, "invalid object type in balltze unit_enter_vehicle, expected biped or vehicle");
            }
            if(!vehicle) {
                return luaL_error(state, "invalid vehicle handle in balltze unit_enter_vehicle");
            }
            if(!vehicle->type == Engine::OBJECT_TYPE_VEHICLE) {
                return luaL_error(state, "invalid object type in balltze unit_enter_vehicle, expected vehicle");
            }
            auto vehicle_tag = Engine::get_tag(vehicle->tag_handle);
            auto vehicle_tag_data = reinterpret_cast<Engine::TagDefinitions::Vehicle *>(vehicle_tag->data);
            if(seat_index >= vehicle_tag_data->seats.count) {
                return luaL_error(state, "invalid seat index in ballte unit_enter_vehicle");
            }
            Engine::unit_scripting_enter_vehicle(unit_handle, vehicle_handle, vehicle_tag_data->seats.elements[seat_index].label.string);
        }
        else {
            return luaL_error(state, "invalid number of arguments in balltze unit_enter_vehicle");
        }
        return 0;
    }

    static int lua_unit_exit_vehicle(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 1) {
            Engine::ObjectHandle unit_handle(luaL_checkinteger(state, 1));
            Engine::unit_scripting_exit_vehicle(unit_handle);
        }
        else {
            return luaL_error(state, "invalid number of arguments in balltze unit_exit_vehicle");
        }
        return 0;
    }

    static int lua_get_clipboard(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 0) {
            clipboardxx::clipboard clipboard;
            lua_pushstring(state, clipboard.paste().c_str());
        }
        else {
            return luaL_error(state, "invalid number of arguments in balltze get_clipboard");
        }
        return 1;
    }

    static int lua_set_clipboard(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 1) {
            std::string text = luaL_checkstring(state, 1);
            clipboardxx::clipboard clipboard;
            clipboard.copy(text);
        }
        else {
            return luaL_error(state, "invalid number of arguments in balltze set_clipboard");
        }
        return 0;
    }

    static int lua_script_unload(lua_State *state) noexcept {
        for(auto it = scripts.begin(); it != scripts.end(); it++) {
            if((*it)->state == state) {
                scripts.erase(it);
                break;
            }
        }
        return 0;
    }

    static void on_map_load(Event::MapLoadEvent const &event) noexcept {
        if(event.time == Event::EVENT_TIME_AFTER) {
            return;
        }
        allow_tag_data_import = true;
        for(auto &script : scripts) {
            auto *state = script->state;
            auto it = script->callbacks.find(CallbackTargetedEvent::MAP_FILE_LOAD);
            if(it != script->callbacks.end()) {
                lua_getglobal(state, it->second.c_str());
                lua_pushstring(state, event.context.name.c_str());
                if(lua_pcall(state, 1, 0, 0) != LUA_OK) {
                    const char *err = lua_tostring(state, -1);
                    console_printf(CONSOLE_COLOR_ERROR, "%s", err);
                    lua_pop(state, 1);
                }
            }
        }
        allow_tag_data_import = false;
    }

    static void register_function(lua_State *state, const char *name, lua_CFunction function) noexcept {
        lua_pushstring(state, name);
        lua_pushcfunction(state, function);
        lua_settable(state, -3);
    }

    extern "C" BALLTZE_API int luaopen_mods_balltze(lua_State *state) noexcept {
        lua_getglobal(state, "script_name");
        if(lua_isstring(state, -1)) {
            std::string script_name = lua_tostring(state, -1);
            if(script_name == "balltze.lua") {
                balltze_chimera_script = state;
                lua_register(state, "load_balltze_chimera_table", Lua::Api::V1::populate_chimera_table);
                lua_getglobal(state, "set_callback");
                lua_pushstring(state, "map load");
                lua_pushstring(state, "load_balltze_chimera_table");
                int res = lua_pcall(state, 2, 0, 0);
                if(res != LUA_OK) {
                    const char *err = lua_tostring(state, -1);
                    logger.debug("Error loading Chimera Lua hijacker script: {}", err);
                    lua_pop(state, 1);
                }
                lua_pop(state, 1);
                Lua::Api::V1::populate_chimera_table(state);
                return 0;
            }
        }

        lua_newtable(state);

        // Set balltze version
        lua_pushstring(state, "version");
        lua_pushstring(state, balltze_version.to_string().c_str());
        lua_settable(state, -3);

        // Set balltze functions
        register_function(state, "set_callback", lua_set_callback);
        register_function(state, "import_tag_data", lua_import_tag_data);
        register_function(state, "unit_enter_vehicle", lua_unit_enter_vehicle);
        register_function(state, "unit_exit_vehicle", lua_unit_exit_vehicle);
        register_function(state, "get_clipboard", lua_get_clipboard);
        register_function(state, "set_clipboard", lua_set_clipboard);

        /**
         * Set __gc metamethod
         * The Lua garbage collector will call the unload function before collect the library table.
         */
        lua_createtable(state, 0, 1);
        lua_pushcfunction(state, lua_script_unload);
        lua_setfield(state, -2, "__gc");
        lua_setmetatable(state, -2);

        // Subscribe to events
        if(!map_load_listener_handle) {
            map_load_listener_handle = Event::MapLoadEvent::subscribe_const(on_map_load, Event::EVENT_PRIORITY_LOWEST);
        }

        auto *script = scripts.emplace_back(std::make_unique<Script>()).get();
        script->state = state;

        return 1;
    }
}

namespace Balltze::Lua::Api::V1 {
    static lua_CFunction get_chimera_function(const char *name) noexcept {
        if(LuaLibrary::balltze_chimera_script) {
            lua_getglobal(LuaLibrary::balltze_chimera_script, name);
            if(lua_isfunction(LuaLibrary::balltze_chimera_script, -1)) {
                auto func = lua_tocfunction(LuaLibrary::balltze_chimera_script, -1);
                lua_pop(LuaLibrary::balltze_chimera_script, 1);
                return func;
            }
            lua_pop(LuaLibrary::balltze_chimera_script, 1);
        }
        return nullptr;
    }

    static void refresh_lua_plugin_variables(lua_State *state) noexcept {
        if(!LuaLibrary::balltze_chimera_script) {
            return;
        }

        lua_getglobal(LuaLibrary::balltze_chimera_script, "local_player_index");
        if(lua_isnumber(LuaLibrary::balltze_chimera_script, -1)) {
            auto index = lua_tointeger(LuaLibrary::balltze_chimera_script, -1);
            lua_pushinteger(state, index);
            lua_setfield(state, -2, "local_player_index");
        }
        lua_pop(LuaLibrary::balltze_chimera_script, 1);

        lua_getglobal(LuaLibrary::balltze_chimera_script, "map");
        if(lua_isstring(LuaLibrary::balltze_chimera_script, -1)) {
            auto map = lua_tostring(LuaLibrary::balltze_chimera_script, -1);
            lua_pushstring(state, map);
            lua_setfield(state, -2, "map");
        }
        lua_pop(LuaLibrary::balltze_chimera_script, 1);

        lua_getglobal(LuaLibrary::balltze_chimera_script, "map_is_protected");
        if(lua_isboolean(LuaLibrary::balltze_chimera_script, -1)) {
            auto is_protected = lua_toboolean(LuaLibrary::balltze_chimera_script, -1);
            lua_pushboolean(state, is_protected);
            lua_setfield(state, -2, "map_is_protected");
        }
        lua_pop(LuaLibrary::balltze_chimera_script, 1);

        lua_getglobal(LuaLibrary::balltze_chimera_script, "server_type");
        if(lua_isstring(LuaLibrary::balltze_chimera_script, -1)) {
            auto server_type = lua_tostring(LuaLibrary::balltze_chimera_script, -1);
            lua_pushstring(state, server_type);
            lua_setfield(state, -2, "server_type");
        }
        lua_pop(LuaLibrary::balltze_chimera_script, 1);

        lua_getglobal(LuaLibrary::balltze_chimera_script, "gametype");
        if(lua_isstring(LuaLibrary::balltze_chimera_script, -1)) {
            auto gametype = lua_tostring(LuaLibrary::balltze_chimera_script, -1);
            lua_pushstring(state, gametype);
            lua_setfield(state, -2, "gametype");
        }
        lua_pop(LuaLibrary::balltze_chimera_script, 1);
    }

    void set_chimera_table(lua_State *state) noexcept {
        if(!LuaLibrary::balltze_chimera_script) {
            return;
        }

        lua_pushstring(state, "chimera");
        lua_newtable(state);
        
        auto push_chimera_function = [state](const char *name) {
            auto *func = get_chimera_function(name);
            if(func) {
                lua_pushcfunction(state, func);
                lua_setfield(state, -2, name);
            }
        };

        push_chimera_function("console_is_open");
        push_chimera_function("console_out");
        push_chimera_function("delete_object");
        push_chimera_function("draw_text");
        push_chimera_function("execute_script");
        push_chimera_function("get_dynamic_player");
        push_chimera_function("get_global");
        push_chimera_function("get_object");
        push_chimera_function("get_player");
        push_chimera_function("get_tag");
        push_chimera_function("hud_message");
        push_chimera_function("load_ui_widget");
        push_chimera_function("set_callback");
        push_chimera_function("set_global");
        push_chimera_function("set_timer");
        push_chimera_function("stop_timer");
        push_chimera_function("spawn_object");
        push_chimera_function("tick_rate");
        push_chimera_function("ticks");
        push_chimera_function("create_font_override");
        push_chimera_function("execute_chimera_command");

        push_chimera_function("read_bit");
        push_chimera_function("write_bit");
        push_chimera_function("read_byte");
        push_chimera_function("write_byte");
        push_chimera_function("read_short");
        push_chimera_function("write_short");
        push_chimera_function("read_word");
        push_chimera_function("write_word");
        push_chimera_function("read_int");
        push_chimera_function("write_int");
        push_chimera_function("read_dword");
        push_chimera_function("write_dword");
        push_chimera_function("read_float");
        push_chimera_function("write_float");
        push_chimera_function("read_char");
        push_chimera_function("write_char");
        push_chimera_function("read_string");
        push_chimera_function("write_string");

        refresh_lua_plugin_variables(state);

        lua_settable(state, -3);
    }

    int populate_chimera_table(lua_State *state) noexcept {
        if(LuaLibrary::balltze_chimera_script) {
            auto plugins = Plugins::get_lua_plugins();
            for(auto *&plugin : plugins) {
                if(plugin->initialized()) {
                    auto *plugin_state = plugin->state();
                    logger.debug("Populating chimera table on Lua plugin {}...", plugin->name());
                    lua_getglobal(plugin_state, "Balltze");
                    set_chimera_table(plugin_state);
                    lua_pop(plugin_state, 1);
                }
            }
        }
        return 0;
    }
}
