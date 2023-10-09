// SPDX-License-Identifier: GPL-3.0-only

#include <string>
#include <lua.hpp>
#include "../loader.hpp"
#include "../../logger.hpp"

namespace Balltze::LuaLibrary {
    extern lua_State *balltze_chimera_script;
}

namespace Balltze::Plugins {
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

    void lua_set_chimera_table(lua_State *state) noexcept {
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

    int lua_populate_chimera_table(lua_State *state) noexcept {
        if(LuaLibrary::balltze_chimera_script) {
            auto plugins = get_lua_plugins();
            for(auto *&plugin : plugins) {
                logger.debug("Populating chimera table on Lua plugin {}...", plugin->name());
                lua_getglobal(plugin->state(), "Balltze");
                lua_set_chimera_table(plugin->state());
                lua_pop(plugin->state(), 1);
            }
        }
        return 0;
    }
}
