// SPDX-License-Identifier: GPL-3.0-only

#include <vector>
#include <balltze/config.hpp>
#include "../../../helpers/function_table.hpp"

namespace Balltze::Lua::Api::V2 {
    static auto LUA_CONFIG_OBJECT_METATABLE = "balltze_config_object";

    static Config::Config *get_config_object(lua_State *state, int index) {
        if(!lua_isuserdata(state, index)) {
            luaL_error(state, "Expected a userdata object at index %d.", index);
        }
        auto *config = static_cast<Config::Config *>(luaL_checkudata(state, index, LUA_CONFIG_OBJECT_METATABLE));
        if(!config) {
            luaL_error(state, "Invalid config object at index %d.", index);
        }
        return config;
    }

    static int save_config_file(lua_State *state) {
        int args = lua_gettop(state);
        if(args != 1) {
            return luaL_error(state, "Invalid number of arguments in function Config:saveConfigFile.");
        }

        auto *config = get_config_object(state, 1);
        try {
            config->save();
        }
        catch(const std::runtime_error &e) {
            return luaL_error(state, "Could not save config file in function Config:saveConfigFile: %s", e.what());
        }

        return 0;
    }

    static int load_config_file(lua_State *state) {
        int args = lua_gettop(state);
        if(args != 1) {
            return luaL_error(state, "Invalid number of arguments in function Config:loadConfigFile.");
        }

        auto *config = get_config_object(state, 1);
        try {
            config->load();
        }
        catch(const std::runtime_error &e) {
            return luaL_error(state, "Could not load config file in function Config:loadConfigFile: %s", e.what());
        }

        return 0;
    }

    static int config_key_exists(lua_State *state) {
        int args = lua_gettop(state);
        if(args != 2) {
            return luaL_error(state, "Invalid number of arguments in function Config:keyExists.");
        }

        auto *config = get_config_object(state, 1);
        auto *key = luaL_checkstring(state, 2);
        if(std::strlen(key) == 0) {
            return luaL_error(state, "Invalid key in function Config:keyExists.");
        }

        lua_pushboolean(state, config->exists(key));
        return 1;
    }

    static int config_remove_key(lua_State *state) {
        int args = lua_gettop(state);
        if(args != 2) {
            return luaL_error(state, "Invalid number of arguments in function Config:removeKey.");
        }

        auto *config = get_config_object(state, 1);
        auto *key = luaL_checkstring(state, 2);
        if(std::strlen(key) == 0) {
            return luaL_error(state, "Invalid key in function Config:removeKey.");
        }

        config->remove(key);
        return 0;
    }

    static int config_get_key_integer(lua_State *state) {
        int args = lua_gettop(state);
        if(args != 2) {
            return luaL_error(state, "Invalid number of arguments in function Config:getInteger.");
        }

        auto *config = get_config_object(state, 1);
        auto *key = luaL_checkstring(state, 2);
        if(std::strlen(key) == 0) {
            return luaL_error(state, "Invalid key in function Config:getInteger.");
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
            return luaL_error(state, "Could not get integer value in function Config:getInteger: %s", e.what());
        }
    }

    static int config_get_key_number(lua_State *state) {
        int args = lua_gettop(state);
        if(args != 2) {
            return luaL_error(state, "Invalid number of arguments in function Config:getNumber.");
        }

        auto *config = get_config_object(state, 1);
        auto *key = luaL_checkstring(state, 2);
        if(std::strlen(key) == 0) {
            return luaL_error(state, "Invalid key in function Config:getNumber.");
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
            return luaL_error(state, "Could not get number value in function Config:getNumber: %s", e.what());
        }
    }

    static int config_get_key_string(lua_State *state) {
        int args = lua_gettop(state);
        if(args != 2) {
            return luaL_error(state, "Invalid number of arguments in function Config:getString.");
        }

        auto *config = get_config_object(state, 1);
        auto *key = luaL_checkstring(state, 2);
        if(std::strlen(key) == 0) {
            return luaL_error(state, "Invalid key in function Config:getString.");
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
            return luaL_error(state, "Could not get string value in function Config:getString: %s", e.what());
        }
    }

    static int config_get_key_boolean(lua_State *state) {
        int args = lua_gettop(state);
        if(args != 2) {
            return luaL_error(state, "Invalid number of arguments in function Config:getBoolean.");
        }

        auto *config = get_config_object(state, 1);
        auto *key = luaL_checkstring(state, 2);
        if(std::strlen(key) == 0) {
            return luaL_error(state, "Invalid key in function Config:getBoolean.");
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
            return luaL_error(state, "Could not get boolean value in function Config:getBoolean: %s", e.what());
        }
    }

    static int config_set_key(lua_State *state) {
        int args = lua_gettop(state);
        if(args != 3) {
            return luaL_error(state, "Invalid number of arguments in function Config:setKeyInteger.");
        }

        auto *config = get_config_object(state, 1);
        auto *key = luaL_checkstring(state, 2);
        if(std::strlen(key) == 0) {
            return luaL_error(state, "Invalid key in function Config:setKeyInteger.");
        }

        if(lua_isinteger(state, 3)) {
            auto value = static_cast<int>(lua_tointeger(state, 3));
            try {
                config->set(key, value);
            }
            catch(const std::runtime_error &e) {
                return luaL_error(state, "Could not set integer value in function Config:setKeyInteger: %s", e.what());
            }
        }
        else if(lua_isnumber(state, 3)) {
            auto value = lua_tonumber(state, 3);
            try {
                config->set(key, value);
            }
            catch(const std::runtime_error &e) {
                return luaL_error(state, "Could not set number value in function Config:setKeyInteger: %s", e.what());
            }
        }
        else if(lua_isstring(state, 3)) {
            auto *value = luaL_checkstring(state, 3);
            try {
                config->set<std::string>(key, value);
            }
            catch(const std::runtime_error &e) {
                return luaL_error(state, "Could not set string value in function Config:setKeyInteger: %s", e.what());
            }
        }
        else if(lua_isboolean(state, 3)) {
            auto value = lua_toboolean(state, 3);
            try {
                config->set(key, value);
            }
            catch(const std::runtime_error &e) {
                return luaL_error(state, "Could not set boolean value in function Config:setKeyInteger: %s", e.what());
            }
        }
        else {
            return luaL_error(state, "Invalid value in function Config:setKeyInteger.");
        }
        return 0;
    }

    static int config__gc(lua_State *state) {
        auto *config = reinterpret_cast<Config::Config *>(lua_touserdata(state, 1));
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
            return luaL_error(state, "Invalid config filepath in function Config:openConfigFile.");
        }

        Config::Config *config = nullptr;
        try {
            config = new Config::Config(filepath);
        }
        catch(const std::runtime_error &e) {
            lua_pop(state, 1);
            return luaL_error(state, "Could not open config file in function Config:openConfigfile: %s", e.what());
        }

        lua_pushlightuserdata(state, config);

        if(luaL_newmetatable(state, LUA_CONFIG_OBJECT_METATABLE) != 0) {
            luaL_newlib(state, functions);
            lua_setfield(state, -2, "__index");
            lua_pushcfunction(state, config__gc);
            lua_setfield(state, -2, "__gc");
        }
        lua_setmetatable(state, -2);

        return 1;
    }

    void set_config_functions(lua_State *state, int table_idx) noexcept {
        int table_abs_idx = lua_absindex(state, table_idx);
        lua_pushcfunction(state, open_config_file);
        lua_setfield(state, table_abs_idx, "openConfigFile");
    }
}
