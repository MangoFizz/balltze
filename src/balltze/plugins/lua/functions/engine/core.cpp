// SPDX-License-Identifier: GPL-3.0-only

#include <lua.hpp>
#include <balltze/engine.hpp>
#include "../../../../logger.hpp"
#include "../../../plugin.hpp"
#include "../../../loader.hpp"
#include "../../libraries.hpp"
#include "../../helpers.hpp"

namespace Balltze::Plugins {
    static int lua_engine_console_print(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args >= 1) {
                bool has_color = lua_istable(state, 1);

                Engine::ColorARGB color;
                if(has_color) {
                    try {
                        color = lua_to_color_argb(state, 1);
                    }
                    catch(const std::exception &e) {
                        return luaL_error(state, "Invalid color argument in function engine.console.printf.");
                    }
                }
                else {
                    color = {1.0, 1.0, 1.0, 1.0};
                }

                std::string message; 
                if(args > 1) { 
                    if(has_color) {
                        lua_remove(state, 1); 
                    }
                    Lformat(state); 
                    message = luaL_checkstring(state, -1); 
                    lua_pop(state, 1); 
                } 
                else { 
                    message = luaL_checkstring(state, 1); 
                }

                Engine::console_print(message, color);
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function engine.console.printf.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
        return 0;
    }

    static int lua_engine_get_tick_count(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 0) {
                lua_pushinteger(state, Engine::get_tick_count());
                return 1;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function engine.get_tick_count.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static int lua_engine_get_engine_edition(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 0) {
                auto edition = Engine::get_engine_edition();
                switch(edition) {
                    case Engine::ENGINE_TYPE_RETAIL:
                        lua_pushstring(state, "retail");
                        break;
                    case Engine::ENGINE_TYPE_DEMO:
                        lua_pushstring(state, "demo");
                        break;
                    case Engine::ENGINE_TYPE_CUSTOM_EDITION:
                        lua_pushstring(state, "custom");
                        break;
                    default:
                        lua_pushstring(state, "unknown");
                        break;
                }
                return 1;
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function engine.get_engine_edition.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin.");
        }
    }

    static const luaL_Reg engine_core_functions[] = {
        {"consolePrint", lua_engine_console_print},
        {"getTickCount", lua_engine_get_tick_count},
        {"getEngineEdition", lua_engine_get_engine_edition},
        {nullptr, nullptr}
    };

    void set_engine_core_functions(lua_State *state) noexcept {
        luaL_newlibtable(state, engine_core_functions);
        luaL_setfuncs(state, engine_core_functions, 0);
        lua_setfield(state, -2, "core");
    }
}
