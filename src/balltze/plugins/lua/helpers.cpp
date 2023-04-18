// SPDX-License-Identifier: GPL-3.0-only

#include "../../logger.hpp"
#include "../loader.hpp"
#include "helpers.hpp"

namespace Balltze::Plugins {
    void lua_create_functions_table(lua_State *state, const char *name, const luaL_Reg *functions) noexcept {
        lua_pushstring(state, name);
        luaL_newlibtable(state, functions);
        luaL_setfuncs(state, functions, 0);
        lua_settable(state, -3);
    }

    VersionNumber lua_check_version_number(lua_State* state, int index) {
        if(lua_istable(state, index)) {
            VersionNumber version;

            auto get_field = [&state, &index](const char *field, int &value) {
                lua_getfield(state, index, field);
                if(lua_isnumber(state, -1)) {
                    value = lua_tointeger(state, -1);
                }
                else {
                    auto *error = "Expected number for version number field.";
                    throw std::runtime_error(error);
                }
                lua_pop(state, 1);
            };

            try {
                get_field("major", version.major);
                get_field("minor", version.minor);
                get_field("patch", version.patch);
                get_field("build", version.build);
            }
            catch(std::runtime_error &e) {
                throw;
            }

            return version;
        }
        else {
            auto *error = "Expected table for version number.";
            throw std::runtime_error(error);
        }
    }

    Engine::ColorARGB lua_to_color_argb(lua_State *state, int index) {
        Engine::ColorARGB color;
        if(lua_istable(state, index)) {
            auto get_field = [&state, &index](const char *field, float &value) {
                lua_getfield(state, index, field);
                if(lua_isnumber(state, -1)) {
                    value = lua_tonumber(state, -1);
                }
                else {
                    auto *error = "Expected number for color field.";
                    luaL_error(state, error);
                    throw std::runtime_error(error);
                }
                lua_pop(state, 1);
            };

            try {
                get_field("red", color.red);
                get_field("green", color.green);
                get_field("blue", color.blue);
                get_field("alpha", color.alpha);
            }
            catch(std::runtime_error &e) {
                throw;
            }
        }
        else {
            if(lua_gettop(state) >= index + 3) {
                auto get_field = [&state, &index](float &value) {
                    if(lua_isnumber(state, index)) {
                        value = lua_tonumber(state, index);
                        index++;
                    }
                    else {
                        auto *error = "Expected number for color field.";
                        luaL_error(state, error);
                        throw std::runtime_error(error);
                    }
                };

                try {
                    get_field(color.red);
                    get_field(color.green);
                    get_field(color.blue);
                    get_field(color.alpha);
                }
                catch(std::runtime_error &e) {
                    throw;
                }
            }
            else {
                throw std::runtime_error("Could not get color from table or stack.");
            }
        }
        return color;
    }

    std::string engine_edition_to_string(Engine::EngineEdition edition) {
        switch(edition) {
            case Engine::ENGINE_TYPE_RETAIL:
                return "retail";
            case Engine::ENGINE_TYPE_DEMO:
                return "demo";
            case Engine::ENGINE_TYPE_CUSTOM_EDITION:
                return "custom";
            default:
                return "unknown";
        }
    }

    Engine::EngineEdition engine_edition_from_string(const std::string &edition) {
        if(edition == "retail") {
            return Engine::ENGINE_TYPE_RETAIL;
        }
        else if(edition == "demo") {
            return Engine::ENGINE_TYPE_DEMO;
        }
        else if(edition == "custom") {
            return Engine::ENGINE_TYPE_CUSTOM_EDITION;
        }
        else {
            throw std::runtime_error("Invalid engine edition.");
        }
    }

    std::string cache_file_engine_to_string(Engine::CacheFileEngine engine) {
        switch(engine) {
            case Engine::CACHE_FILE_XBOX:
                return "xbox";
            case Engine::CACHE_FILE_DEMO:
                return "demo";
            case Engine::CACHE_FILE_RETAIL:
                return "retail";
            case Engine::CACHE_FILE_CUSTOM_EDITION:
                return "custom";
            case Engine::CACHE_FILE_INVADER:
                return "invader";
            case Engine::CACHE_FILE_DEMO_COMPRESSED:
                return "demo compressed";
            case Engine::CACHE_FILE_RETAIL_COMPRESSED:
                return "retail compressed";
            case Engine::CACHE_FILE_CUSTOM_EDITION_COMPRESSED:
                return "custom compressed";
            default:
                return "unknown";
        }   
    }

    Engine::CacheFileEngine cache_file_engine_from_string(const std::string &engine) {
        if(engine == "xbox") {
            return Engine::CACHE_FILE_XBOX;
        }
        else if(engine == "demo") {
            return Engine::CACHE_FILE_DEMO;
        }
        else if(engine == "retail") {
            return Engine::CACHE_FILE_RETAIL;
        }
        else if(engine == "custom") {
            return Engine::CACHE_FILE_CUSTOM_EDITION;
        }
        else if(engine == "invader") {
            return Engine::CACHE_FILE_INVADER;
        }
        else if(engine == "demo compressed") {
            return Engine::CACHE_FILE_DEMO_COMPRESSED;
        }
        else if(engine == "retail compressed") {
            return Engine::CACHE_FILE_RETAIL_COMPRESSED;
        }
        else if(engine == "custom compressed") {
            return Engine::CACHE_FILE_CUSTOM_EDITION_COMPRESSED;
        }
        else {
            throw std::runtime_error("Invalid cache file engine.");
        }
    }

    std::string map_game_type_to_string(Engine::MapGameType type) {
        switch(type) {
            case Engine::MAP_SINGLE_PLAYER:
                return "single_player";
            case Engine::MAP_MULTIPLAYER:
                return "multiplayer";
            case Engine::MAP_USER_INTERFACE:
                return "user_interface";
            default:
                return "unknown";
        }
    }

    Engine::MapGameType map_game_type_from_string(const std::string &type) {
        if(type == "single_player") {
            return Engine::MAP_SINGLE_PLAYER;
        }
        else if(type == "multiplayer") {
            return Engine::MAP_MULTIPLAYER;
        }
        else if(type == "user_interface") {
            return Engine::MAP_USER_INTERFACE;
        }
        else {
            throw std::runtime_error("Invalid map game type.");
        }
    }

    std::string server_type_to_string(Engine::ServerType type) {
        switch(type) {
            case Engine::SERVER_NONE:
                return "none";
            case Engine::SERVER_DEDICATED:
                return "dedicated";
            case Engine::SERVER_LOCAL:
                return "local";
            default:
                return "unknown";
        }
    }

    Engine::ServerType server_type_from_string(const std::string &type) {
        if(type == "none") {
            return Engine::SERVER_NONE;
        }
        else if(type == "dedicated") {
            return Engine::SERVER_DEDICATED;
        }
        else if(type == "local") {
            return Engine::SERVER_LOCAL;
        }
        else {
            throw std::runtime_error("Invalid server type.");
        }
    }

    std::string server_game_type_to_string(Engine::Gametype gametype) {
        switch(gametype) {
            case Engine::GAMETYPE_CTF:
                return "ctf";
            case Engine::GAMETYPE_SLAYER:
                return "slayer";
            case Engine::GAMETYPE_ODDBALL:
                return "oddball";
            case Engine::GAMETYPE_KING:
                return "king";
            case Engine::GAMETYPE_RACE:
                return "race";
            default:
                return "none";
        }
    }

    Engine::Gametype server_game_type_from_string(std::string gametype) {
        if(gametype == "ctf") {
            return Engine::GAMETYPE_CTF;
        }
        else if(gametype == "slayer") {
            return Engine::GAMETYPE_SLAYER;
        }
        else if(gametype == "oddball") {
            return Engine::GAMETYPE_ODDBALL;
        }
        else if(gametype == "king") {
            return Engine::GAMETYPE_KING;
        }
        else if(gametype == "race") {
            return Engine::GAMETYPE_RACE;
        }
        else {
            throw std::runtime_error("Invalid server game type.");
        }
    }
}
