// SPDX-License-Identifier: GPL-3.0-only

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <lua.hpp>
#include <balltze/api.hpp>
#include <balltze/engine.hpp>
#include <balltze/event.hpp>
#include <balltze/features.hpp>
#include <balltze/utils.hpp>
#include "../version.hpp"
#include "../logger.hpp"

namespace Balltze::Plugins {
    extern int lua_populate_chimera_table(lua_State *state) noexcept;
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
    static std::optional<Event::EventListenerHandle<Event::MapFileLoadEvent>> map_file_load_listener_handle;

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
            auto *unit = reinterpret_cast<Engine::UnitObject *>(object_table.get_dynamic_object(unit_handle));
            auto *vehicle = reinterpret_cast<Engine::UnitObject *>(object_table.get_dynamic_object(vehicle_handle));
            if(!unit) {
                return luaL_error(state, "invalid unit handle in balltze unit_enter_vehicle");
            }
            if(!unit->type == Engine::OBJECT_TYPE_BIPED) {
                return luaL_error(state, "invalid object type in balltze unit_enter_vehicle, expected biped");
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
            unit->enter_vehicle(vehicle_handle, vehicle_tag_data->seats.offset[seat_index].label.string);
        }
        else {
            return luaL_error(state, "invalid number of arguments in balltze unit_enter_vehicle");
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

    static void on_map_file_load(Event::MapFileLoadEvent const &event) noexcept {
        allow_tag_data_import = true;
        for(auto &script : scripts) {
            auto *state = script->state;
            auto it = script->callbacks.find(CallbackTargetedEvent::MAP_FILE_LOAD);
            if(it != script->callbacks.end()) {
                lua_getglobal(state, it->second.c_str());
                lua_pushstring(state, event.args.map_name.c_str());
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
                lua_register(state, "load_balltze_chimera_table", Plugins::lua_populate_chimera_table);
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
                Plugins::lua_populate_chimera_table(state);
                return 0;
            }
        }

        lua_newtable(state);

        // Set balltze version
        lua_pushstring(state, "version");
        lua_pushstring(state, BALLTZE_VERSION);
        lua_settable(state, -3);

        // Set balltze functions
        register_function(state, "set_callback", lua_set_callback);
        register_function(state, "import_tag_data", lua_import_tag_data);
        register_function(state, "unit_enter_vehicle", lua_unit_enter_vehicle);

        /**
         * Set __gc metamethod
         * The Lua garbage collector will call the unload function before collect the library table.
         */
        lua_createtable(state, 0, 1);
        lua_pushcfunction(state, lua_script_unload);
        lua_setfield(state, -2, "__gc");
        lua_setmetatable(state, -2);

        // Subscribe to events
        if(!map_file_load_listener_handle) {
            map_file_load_listener_handle = Event::MapFileLoadEvent::subscribe_const(on_map_file_load);
        }

        auto *script = scripts.emplace_back(std::make_unique<Script>()).get();
        script->state = state;

        return 1;
    }
}
