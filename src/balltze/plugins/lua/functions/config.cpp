// SPDX-License-Identifier: GPL-3.0-only

#include <vector>
#include <balltze/config.hpp>
#include "../../../logger.hpp"
#include "../../loader.hpp"
#include "../helpers.hpp"
#include "command.hpp"

namespace Balltze::Plugins {
    static int lua_save_config_file(lua_State *state) {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args != 1) {
                return luaL_error(state, "Invalid number of arguments in function Balltze.config.saveConfigFile.");
            }

            if(!lua_istable(state, 1)) {
                return luaL_error(state, "Invalid argument in function Balltze.config.saveConfigFile.");
            }

            lua_getfield(state, 1, "_config");
            auto *config = reinterpret_cast<Config::Config *>(luaL_checkudata(state, -1, "Config"));
            lua_pop(state, 1);

            try {
                config->save();
            }
            catch(const std::runtime_error &e) {
                return luaL_error(state, "Could not save config file in function Balltze.config.saveConfigFile: %s", e.what());
            }

            return 0;
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin in function Balltze.config.saveConfigFile.");
        }
    }

    static int lua_load_config_file(lua_State *state) {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args != 1) {
                return luaL_error(state, "Invalid number of arguments in function Balltze.config.loadConfigFile.");
            }

            if(!lua_istable(state, 1)) {
                return luaL_error(state, "Invalid argument in function Balltze.config.loadConfigFile.");
            }

            lua_getfield(state, 1, "_config");
            auto *config = reinterpret_cast<Config::Config *>(luaL_checkudata(state, -1, "Config"));
            lua_pop(state, 1);

            try {
                config->load();
            }
            catch(const std::runtime_error &e) {
                return luaL_error(state, "Could not load config file in function Balltze.config.loadConfigFile: %s", e.what());
            }

            return 0;
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin in function Balltze.config.loadConfigFile.");
        }
    }

    static int lua_config_key_exists(lua_State *state) {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args != 2) {
                return luaL_error(state, "Invalid number of arguments in function Balltze.config.keyExists.");
            }

            if(!lua_istable(state, 1)) {
                return luaL_error(state, "Invalid argument in function Balltze.config.keyExists.");
            }

            lua_getfield(state, 1, "_config");
            auto *config = reinterpret_cast<Config::Config *>(luaL_checkudata(state, -1, "Config"));
            lua_pop(state, 1);

            auto *key = luaL_checkstring(state, 2);
            if(std::strlen(key) == 0) {
                return luaL_error(state, "Invalid key in function Balltze.config.keyExists.");
            }

            lua_pushboolean(state, config->exists(key));
            return 1;
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin in function Balltze.config.keyExists.");
        }
    }

    static int lua_config_remove_key(lua_State *state) {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args != 2) {
                return luaL_error(state, "Invalid number of arguments in function Balltze.config.removeKey.");
            }

            if(!lua_istable(state, 1)) {
                return luaL_error(state, "Invalid argument in function Balltze.config.removeKey.");
            }

            lua_getfield(state, 1, "_config");
            auto *config = reinterpret_cast<Config::Config *>(luaL_checkudata(state, -1, "Config"));
            lua_pop(state, 1);

            auto *key = luaL_checkstring(state, 2);
            if(std::strlen(key) == 0) {
                return luaL_error(state, "Invalid key in function Balltze.config.removeKey.");
            }

            config->remove(key);
            return 0;
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin in function Balltze.config.removeKey.");
        }
    }

    static int lua_config_get_key_integer(lua_State *state) {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args != 2) {
                return luaL_error(state, "Invalid number of arguments in function Balltze.config.getKeyInteger.");
            }

            if(!lua_istable(state, 1)) {
                return luaL_error(state, "Invalid argument in function Balltze.config.getKeyInteger.");
            }

            lua_getfield(state, 1, "_config");
            auto *config = reinterpret_cast<Config::Config *>(luaL_checkudata(state, -1, "Config"));
            lua_pop(state, 1);

            auto *key = luaL_checkstring(state, 2);
            if(std::strlen(key) == 0) {
                return luaL_error(state, "Invalid key in function Balltze.config.getKeyInteger.");
            }

            try {
                auto value = config->get<int>(key);
                if(value.has_value()) {
                    lua_pushinteger(state, value.value());
                }
                else {
                    lua_pushnil(state);
                }
                return 1;
            }
            catch(const std::runtime_error &e) {
                return luaL_error(state, "Could not get integer value in function Balltze.config.getKeyInteger: %s", e.what());
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin in function Balltze.config.getKeyInteger.");
        }
    }

    static int lua_config_get_key_number(lua_State *state) {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args != 2) {
                return luaL_error(state, "Invalid number of arguments in function Balltze.config.getKeyNumber.");
            }

            if(!lua_istable(state, 1)) {
                return luaL_error(state, "Invalid argument in function Balltze.config.getKeyNumber.");
            }

            lua_getfield(state, 1, "_config");
            auto *config = reinterpret_cast<Config::Config *>(luaL_checkudata(state, -1, "Config"));
            lua_pop(state, 1);

            auto *key = luaL_checkstring(state, 2);
            if(std::strlen(key) == 0) {
                return luaL_error(state, "Invalid key in function Balltze.config.getKeyNumber.");
            }

            try {
                auto value = config->get<double>(key);
                if(value.has_value()) {
                    lua_pushnumber(state, value.value());
                }
                else {
                    lua_pushnil(state);
                }
                return 1;
            }
            catch(const std::runtime_error &e) {
                return luaL_error(state, "Could not get number value in function Balltze.config.getKeyNumber: %s", e.what());
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin in function Balltze.config.getKeyNumber.");
        }
    }

    static int lua_config_get_key_string(lua_State *state) {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args != 2) {
                return luaL_error(state, "Invalid number of arguments in function Balltze.config.getKeyString.");
            }

            if(!lua_istable(state, 1)) {
                return luaL_error(state, "Invalid argument in function Balltze.config.getKeyString.");
            }

            lua_getfield(state, 1, "_config");
            auto *config = reinterpret_cast<Config::Config *>(luaL_checkudata(state, -1, "Config"));
            lua_pop(state, 1);

            auto *key = luaL_checkstring(state, 2);
            if(std::strlen(key) == 0) {
                return luaL_error(state, "Invalid key in function Balltze.config.getKeyString.");
            }

            try {
                auto value = config->get<std::string>(key);
                if(value.has_value()) {
                    lua_pushstring(state, value->c_str());
                }
                else {
                    lua_pushnil(state);
                }
                return 1;
            }
            catch(const std::runtime_error &e) {
                return luaL_error(state, "Could not get string value in function Balltze.config.getKeyString: %s", e.what());
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin in function Balltze.config.getKeyString.");
        }
    }

    static int lua_config_get_key_boolean(lua_State *state) {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args != 2) {
                return luaL_error(state, "Invalid number of arguments in function Balltze.config.getKeyBoolean.");
            }

            if(!lua_istable(state, 1)) {
                return luaL_error(state, "Invalid argument in function Balltze.config.getKeyBoolean.");
            }

            lua_getfield(state, 1, "_config");
            auto *config = reinterpret_cast<Config::Config *>(luaL_checkudata(state, -1, "Config"));
            lua_pop(state, 1);

            auto *key = luaL_checkstring(state, 2);
            if(std::strlen(key) == 0) {
                return luaL_error(state, "Invalid key in function Balltze.config.getKeyBoolean.");
            }

            try {
                auto value = config->get<bool>(key);
                if(value.has_value()) {
                    lua_pushboolean(state, value.value());
                }
                else {
                    lua_pushnil(state);
                }
                return 1;
            }
            catch(const std::runtime_error &e) {
                return luaL_error(state, "Could not get boolean value in function Balltze.config.getKeyBoolean: %s", e.what());
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin in function Balltze.config.getKeyBoolean.");
        }
    }

    static int lua_config_set_key(lua_State *state) {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args != 3) {
                return luaL_error(state, "Invalid number of arguments in function Balltze.config.setKeyInteger.");
            }

            if(!lua_istable(state, 1)) {
                return luaL_error(state, "Invalid argument in function Balltze.config.setKeyInteger.");
            }

            lua_getfield(state, 1, "_config");
            auto *config = reinterpret_cast<Config::Config *>(luaL_checkudata(state, -1, "Config"));
            lua_pop(state, 1);

            auto *key = luaL_checkstring(state, 2);
            if(std::strlen(key) == 0) {
                return luaL_error(state, "Invalid key in function Balltze.config.setKeyInteger.");
            }

            if(lua_isinteger(state, 3)) {
                auto value = static_cast<int>(lua_tointeger(state, 3));
                try {
                    config->set(key, value);
                }
                catch(const std::runtime_error &e) {
                    return luaL_error(state, "Could not set integer value in function Balltze.config.setKeyInteger: %s", e.what());
                }
            }
            else if(lua_isnumber(state, 3)) {
                auto value = lua_tonumber(state, 3);
                try {
                    config->set(key, value);
                }
                catch(const std::runtime_error &e) {
                    return luaL_error(state, "Could not set number value in function Balltze.config.setKeyInteger: %s", e.what());
                }
            }
            else if(lua_isstring(state, 3)) {
                auto *value = luaL_checkstring(state, 3);
                try {
                    config->set<std::string>(key, value);
                }
                catch(const std::runtime_error &e) {
                    return luaL_error(state, "Could not set string value in function Balltze.config.setKeyInteger: %s", e.what());
                }
            }
            else if(lua_isboolean(state, 3)) {
                auto value = lua_toboolean(state, 3);
                try {
                    config->set(key, value);
                }
                catch(const std::runtime_error &e) {
                    return luaL_error(state, "Could not set boolean value in function Balltze.config.setKeyInteger: %s", e.what());
                }
            }
            else {
                return luaL_error(state, "Invalid value in function Balltze.config.setKeyInteger.");
            }
            return 0;
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin in function Balltze.config.setKeyInteger.");
        }
    }

    static int lua_open_config_file(lua_State *state) {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            auto *filepath = luaL_checkstring(state, 1);
            if(std::strlen(filepath) == 0) {
                return luaL_error(state, "Invalid config filepath in function Balltze.config.openConfigFile.");
            }

            lua_newtable(state);
            auto *config = reinterpret_cast<Config::Config *>(lua_newuserdata(state, sizeof(Config::Config)));

            try {
                *config = Config::Config(filepath);
            }
            catch(const std::runtime_error &e) {
                lua_pop(state, 2);
                return luaL_error(state, "Could not open config file in function Balltze.config.openConfigfile: %s", e.what());
            }
            lua_setfield(state, -2, "_config");

            lua_pushcfunction(state, lua_save_config_file);
            lua_setfield(state, -2, "save");

            lua_pushcfunction(state, lua_load_config_file);
            lua_setfield(state, -2, "load");

            lua_pushcfunction(state, lua_config_key_exists);
            lua_setfield(state, -2, "exists");

            lua_pushcfunction(state, lua_config_remove_key);
            lua_setfield(state, -2, "remove");

            lua_pushcfunction(state, lua_config_get_key_integer);
            lua_setfield(state, -2, "getInteger");

            lua_pushcfunction(state, lua_config_get_key_number);
            lua_setfield(state, -2, "getNumber");

            lua_pushcfunction(state, lua_config_get_key_string);
            lua_setfield(state, -2, "getString");

            lua_pushcfunction(state, lua_config_get_key_boolean);
            lua_setfield(state, -2, "getBoolean");

            lua_pushcfunction(state, lua_config_set_key);
            lua_setfield(state, -2, "set");

            return 1;
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin in function Balltze.config.openConfigFile.");
        }
    }

    static const luaL_Reg config_functions[] = {
        {"openConfigFile", lua_open_config_file},
        {"saveConfigFile", lua_save_config_file},
        {"loadConfigFile", lua_load_config_file},
        {"keyExists", lua_config_key_exists},
        {"removeKey", lua_config_remove_key},
        {"getKeyInteger", lua_config_get_key_integer},
        {"getKeyNumber", lua_config_get_key_number},
        {"getKeyString", lua_config_get_key_string},
        {"getKeyBoolean", lua_config_get_key_boolean},
        {"setKey", lua_config_set_key},
        {nullptr, nullptr}
    };

    void lua_set_config_table(lua_State *state) noexcept {
        lua_create_functions_table(state, "config", config_functions);
    }
}
