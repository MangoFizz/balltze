// SPDX-License-Identifier: GPL-3.0-only

#include <vector>
#include <balltze/config.hpp>
#include "../../../helpers/plugin.hpp"

namespace Balltze::Lua::Api::V1 {
    static int save_config_file(lua_State *state) {
        int args = lua_gettop(state);
        if(args != 1) {
            return luaL_error(state, "Invalid number of arguments in function Balltze.config.saveConfigFile.");
        }

        if(!lua_istable(state, 1)) {
            return luaL_error(state, "Invalid argument in function Balltze.config.saveConfigFile.");
        }

        lua_getfield(state, 1, "data");
        auto *config = reinterpret_cast<Config::Config *>(lua_touserdata(state, -1));
        lua_pop(state, 1);

        try {
            config->save();
        }
        catch(const std::runtime_error &e) {
            return luaL_error(state, "Could not save config file in function Balltze.config.saveConfigFile: %s", e.what());
        }

        return 0;
    }

    static int load_config_file(lua_State *state) {
        int args = lua_gettop(state);
        if(args != 1) {
            return luaL_error(state, "Invalid number of arguments in function Balltze.config.loadConfigFile.");
        }

        if(!lua_istable(state, 1)) {
            return luaL_error(state, "Invalid argument in function Balltze.config.loadConfigFile.");
        }

        lua_getfield(state, 1, "data");
        auto *config = reinterpret_cast<Config::Config *>(lua_touserdata(state, -1));
        lua_pop(state, 1);

        try {
            config->load();
        }
        catch(const std::runtime_error &e) {
            return luaL_error(state, "Could not load config file in function Balltze.config.loadConfigFile: %s", e.what());
        }

        return 0;
    }

    static int config_key_exists(lua_State *state) {
        int args = lua_gettop(state);
        if(args != 2) {
            return luaL_error(state, "Invalid number of arguments in function Balltze.config.keyExists.");
        }

        if(!lua_istable(state, 1)) {
            return luaL_error(state, "Invalid argument in function Balltze.config.keyExists.");
        }

        lua_getfield(state, 1, "data");
        auto *config = reinterpret_cast<Config::Config *>(lua_touserdata(state, -1));
        lua_pop(state, 1);

        auto *key = luaL_checkstring(state, 2);
        if(std::strlen(key) == 0) {
            return luaL_error(state, "Invalid key in function Balltze.config.keyExists.");
        }

        lua_pushboolean(state, config->exists(key));
        return 1;
    }

    static int config_remove_key(lua_State *state) {
        int args = lua_gettop(state);
        if(args != 2) {
            return luaL_error(state, "Invalid number of arguments in function Balltze.config.removeKey.");
        }

        if(!lua_istable(state, 1)) {
            return luaL_error(state, "Invalid argument in function Balltze.config.removeKey.");
        }

        lua_getfield(state, 1, "data");
        auto *config = reinterpret_cast<Config::Config *>(lua_touserdata(state, -1));
        lua_pop(state, 1);

        auto *key = luaL_checkstring(state, 2);
        if(std::strlen(key) == 0) {
            return luaL_error(state, "Invalid key in function Balltze.config.removeKey.");
        }

        config->remove(key);
        return 0;
    }

    static int config_get_key_integer(lua_State *state) {
        int args = lua_gettop(state);
        if(args != 2) {
            return luaL_error(state, "Invalid number of arguments in function Balltze.config.getInteger.");
        }

        if(!lua_istable(state, 1)) {
            return luaL_error(state, "Invalid argument in function Balltze.config.getInteger.");
        }

        lua_getfield(state, 1, "data");
        auto *config = reinterpret_cast<Config::Config *>(lua_touserdata(state, -1));
        lua_pop(state, 1);

        auto *key = luaL_checkstring(state, 2);
        if(std::strlen(key) == 0) {
            return luaL_error(state, "Invalid key in function Balltze.config.getInteger.");
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
            return luaL_error(state, "Could not get integer value in function Balltze.config.getInteger: %s", e.what());
        }
    }

    static int config_get_key_number(lua_State *state) {
        int args = lua_gettop(state);
        if(args != 2) {
            return luaL_error(state, "Invalid number of arguments in function Balltze.config.getNumber.");
        }

        if(!lua_istable(state, 1)) {
            return luaL_error(state, "Invalid argument in function Balltze.config.getNumber.");
        }

        lua_getfield(state, 1, "data");
        auto *config = reinterpret_cast<Config::Config *>(lua_touserdata(state, -1));
        lua_pop(state, 1);

        auto *key = luaL_checkstring(state, 2);
        if(std::strlen(key) == 0) {
            return luaL_error(state, "Invalid key in function Balltze.config.getNumber.");
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
            return luaL_error(state, "Could not get number value in function Balltze.config.getNumber: %s", e.what());
        }
    }

    static int config_get_key_string(lua_State *state) {
        int args = lua_gettop(state);
        if(args != 2) {
            return luaL_error(state, "Invalid number of arguments in function Balltze.config.getString.");
        }

        if(!lua_istable(state, 1)) {
            return luaL_error(state, "Invalid argument in function Balltze.config.getString.");
        }

        lua_getfield(state, 1, "data");
        auto *config = reinterpret_cast<Config::Config *>(lua_touserdata(state, -1));
        lua_pop(state, 1);

        auto *key = luaL_checkstring(state, 2);
        if(std::strlen(key) == 0) {
            return luaL_error(state, "Invalid key in function Balltze.config.getString.");
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
            return luaL_error(state, "Could not get string value in function Balltze.config.getString: %s", e.what());
        }
    }

    static int config_get_key_boolean(lua_State *state) {
        int args = lua_gettop(state);
        if(args != 2) {
            return luaL_error(state, "Invalid number of arguments in function Balltze.config.getBoolean.");
        }

        if(!lua_istable(state, 1)) {
            return luaL_error(state, "Invalid argument in function Balltze.config.getBoolean.");
        }

        lua_getfield(state, 1, "data");
        auto *config = reinterpret_cast<Config::Config *>(lua_touserdata(state, -1));
        lua_pop(state, 1);

        auto *key = luaL_checkstring(state, 2);
        if(std::strlen(key) == 0) {
            return luaL_error(state, "Invalid key in function Balltze.config.getBoolean.");
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
            return luaL_error(state, "Could not get boolean value in function Balltze.config.getBoolean: %s", e.what());
        }
    }

    static int config_set_key(lua_State *state) {
        int args = lua_gettop(state);
        if(args != 3) {
            return luaL_error(state, "Invalid number of arguments in function Balltze.config.setKeyInteger.");
        }

        if(!lua_istable(state, 1)) {
            return luaL_error(state, "Invalid argument in function Balltze.config.setKeyInteger.");
        }

        lua_getfield(state, 1, "data");
        auto *config = reinterpret_cast<Config::Config *>(lua_touserdata(state, -1));
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

    static int config__gc(lua_State *state) {
        auto *config = reinterpret_cast<Config::Config *>(lua_touserdata(state, lua_upvalueindex(1)));
        delete config;
        return 0;
    }

    static const luaL_Reg functions[] = {
        {"save", save_config_file},
        {"load", load_config_file},
        {"exists", config_key_exists},
        {"remove", config_remove_key},
        {"getInteger", config_get_key_integer},
        {"getNumber", config_get_key_number},
        {"getString", config_get_key_string},
        {"getBoolean", config_get_key_boolean},
        {"set", config_set_key},
        {nullptr, nullptr}
    };

    static int open_config_file(lua_State *state) {
        auto *filepath = luaL_checkstring(state, 1);
        if(std::strlen(filepath) == 0) {
            return luaL_error(state, "Invalid config filepath in function Balltze.config.openConfigFile.");
        }

        lua_newtable(state);

        try {
            auto *config = new Config::Config(filepath);
            lua_pushlightuserdata(state, config);
            lua_setfield(state, -2, "data");
            luaL_setfuncs(state, functions, 0);

            lua_newtable(state);
            lua_pushlightuserdata(state, config);
            lua_pushcclosure(state, config__gc, 1);
            lua_setfield(state, -2, "__gc");
            lua_setmetatable(state, -2);
        
            return 1;
        }
        catch(const std::runtime_error &e) {
            lua_pop(state, 2);
            return luaL_error(state, "Could not open config file in function Balltze.config.openConfigfile: %s", e.what());
        }
    }

    static const luaL_Reg config_functions[] = {
        {"open", open_config_file},
        {"save", save_config_file},
        {"load", load_config_file},
        {"exists", config_key_exists},
        {"remove", config_remove_key},
        {"getInteger", config_get_key_integer},
        {"getNumber", config_get_key_number},
        {"getString", config_get_key_string},
        {"getBoolean", config_get_key_boolean},
        {"set", config_set_key},
        {nullptr, nullptr}
    };

    void set_config_table(lua_State *state) noexcept {
        push_plugin_functions_table(state, "config", -1, config_functions);
    }
}
