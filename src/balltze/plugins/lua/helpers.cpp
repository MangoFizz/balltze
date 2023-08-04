// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/engine/data_types.hpp>
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

    Engine::Gametype server_game_type_from_string(const std::string &gametype) {
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

    void lua_push_engine_matrix(lua_State *state, Engine::Matrix &matrix) noexcept {
        lua_newtable(state);
        for(std::size_t i = 0; i < 3; i++) {
            lua_newtable(state);
            for(std::size_t j = 0; j < 3; j++) {
                lua_pushnumber(state, matrix[i][j]);
                lua_rawseti(state, -2, j + 1);
            }
            lua_rawseti(state, -2, i + 1);
        }
    }

    std::shared_ptr<Engine::Matrix> lua_to_engine_matrix(lua_State *state, int index) noexcept {
        auto matrix_ptr = new Engine::Matrix();
        std::shared_ptr<Engine::Matrix> matrix(matrix_ptr);
        for(std::size_t i = 0; i < 3; i++) {
            lua_rawgeti(state, index, i + 1);
            for(std::size_t j = 0; j < 3; j++) {
                lua_rawgeti(state, -1, j + 1);
                matrix[i][j] = luaL_checknumber(state, -1);
                lua_pop(state, 1);
            }
            lua_pop(state, 1);
        }
        return matrix;
    }

    void lua_push_engine_resolution(lua_State *state, Engine::Resolution &resolution) noexcept {
        lua_newtable(state);
        lua_pushinteger(state, resolution.width);
        lua_setfield(state, -2, "width");
        lua_pushinteger(state, resolution.height);
        lua_setfield(state, -2, "height");
    }

    std::string object_type_to_string(Engine::ObjectType type) {
        switch(type) {
            case Engine::OBJECT_TYPE_BIPED: 
                return "biped";
            case Engine::OBJECT_TYPE_VEHICLE:
                return "vehicle";
            case Engine::OBJECT_TYPE_WEAPON:
                return "weapon";
            case Engine::OBJECT_TYPE_EQUIPMENT:
                return "equipment";
            case Engine::OBJECT_TYPE_GARBAGE:
                return "garbage";
            case Engine::OBJECT_TYPE_PROJECTILE:
                return "projectile";
            case Engine::OBJECT_TYPE_SCENERY:
                return "scenery";
            case Engine::OBJECT_TYPE_DEVICE_MACHINE:
                return "device_machine";
            case Engine::OBJECT_TYPE_DEVICE_CONTROL:
                return "device_control";
            case Engine::OBJECT_TYPE_DEVICE_LIGHT_FIXTURE:
                return "device_light_fixture";
            case Engine::OBJECT_TYPE_PLACEHOLDER:
                return "placeholder";
            case Engine::OBJECT_TYPE_SOUND_SCENERY:
                return "sound_scenery";
            default:
                return "unknown";
        }
    }

    Engine::ObjectType object_type_from_string(const std::string &type) {
        if(type == "biped") {
            return Engine::OBJECT_TYPE_BIPED;
        }
        else if(type == "vehicle") {
            return Engine::OBJECT_TYPE_VEHICLE;
        }
        else if(type == "weapon") {
            return Engine::OBJECT_TYPE_WEAPON;
        }
        else if(type == "equipment") {
            return Engine::OBJECT_TYPE_EQUIPMENT;
        }
        else if(type == "garbage") {
            return Engine::OBJECT_TYPE_GARBAGE;
        }
        else if(type == "projectile") {
            return Engine::OBJECT_TYPE_PROJECTILE;
        }
        else if(type == "scenery") {
            return Engine::OBJECT_TYPE_SCENERY;
        }
        else if(type == "device_machine") {
            return Engine::OBJECT_TYPE_DEVICE_MACHINE;
        }
        else if(type == "device_control") {
            return Engine::OBJECT_TYPE_DEVICE_CONTROL;
        }
        else if(type == "device_light_fixture") {
            return Engine::OBJECT_TYPE_DEVICE_LIGHT_FIXTURE;
        }
        else if(type == "placeholder") {
            return Engine::OBJECT_TYPE_PLACEHOLDER;
        }
        else if(type == "sound_scenery") {
            return Engine::OBJECT_TYPE_SOUND_SCENERY;
        }
        else {
            throw std::runtime_error("Invalid object type.");
        }
    }

    std::string object_network_role_to_string(Engine::ObjectNetworkRole role) {
        switch(role) {
            case Engine::OBJECT_NETWORK_ROLE_MASTER:
                return "master";
            case Engine::OBJECT_NETWORK_ROLE_PUPPET:
                return "puppet";
            case Engine::OBJECT_NETWORK_ROLE_LOCALLY_CONTROLLED_PUPPET:
                return "locally_controlled_puppet";
            case Engine::OBJECT_NETWORK_ROLE_LOCAL_ONLY:
                return "local_only";
            default:
                return "unknown";
        }
    }

    Engine::ObjectNetworkRole object_network_role_from_string(const std::string &role) {
        if(role == "master") {
            return Engine::OBJECT_NETWORK_ROLE_MASTER;
        }
        else if(role == "puppet") {
            return Engine::OBJECT_NETWORK_ROLE_PUPPET;
        }
        else if(role == "locally_controlled_puppet") {
            return Engine::OBJECT_NETWORK_ROLE_LOCALLY_CONTROLLED_PUPPET;
        }
        else if(role == "local_only") {
            return Engine::OBJECT_NETWORK_ROLE_LOCAL_ONLY;
        }
        else {
            throw std::runtime_error("Invalid object network role.");
        }
    }

    std::string dynamic_object_attachment_type_to_string(Engine::DynamicObjectAttachmentType type) {
        switch(type) {
            case Engine::OBJECT_ATTACHMENT_TYPE_INVALID: 
                return "invalid";
            case Engine::OBJECT_ATTACHMENT_TYPE_LIGHT:
                return "light";
            case Engine::OBJECT_ATTACHMENT_TYPE_LOOPING_SOUND:
                return "looping_sound";
            case Engine::OBJECT_ATTACHMENT_TYPE_EFFECT:
                return "effect";
            case Engine::OBJECT_ATTACHMENT_TYPE_CONTRAIL:
                return "contrail";
            case Engine::OBJECT_ATTACHMENT_TYPE_PARTICLE:
                return "particle";
            default:
                return "unknown";
        }
    }

    Engine::DynamicObjectAttachmentType dynamic_object_attachment_type_from_string(const std::string &type) {
        if(type == "invalid") {
            return Engine::OBJECT_ATTACHMENT_TYPE_INVALID;
        }
        else if(type == "light") {
            return Engine::OBJECT_ATTACHMENT_TYPE_LIGHT;
        }
        else if(type == "looping_sound") {
            return Engine::OBJECT_ATTACHMENT_TYPE_LOOPING_SOUND;
        }
        else if(type == "effect") {
            return Engine::OBJECT_ATTACHMENT_TYPE_EFFECT;
        }
        else if(type == "contrail") {
            return Engine::OBJECT_ATTACHMENT_TYPE_CONTRAIL;
        }
        else if(type == "particle") {
            return Engine::OBJECT_ATTACHMENT_TYPE_PARTICLE;
        }
        else {
            throw std::runtime_error("Invalid dynamic object attachment type.");
        }
    }

    std::string multiplayer_team_to_string(Engine::MultiplayerTeam team) {
        switch(team) {
            case Engine::MULTIPLAYER_TEAM_RED:
                return "red";
            case Engine::MULTIPLAYER_TEAM_BLUE:
                return "blue";
            default:
                return "none";
        }
    }
    
    Engine::MultiplayerTeam multiplayer_team_from_string(const std::string &team) {
        if(team == "red") {
            return Engine::MULTIPLAYER_TEAM_RED;
        }
        else if(team == "blue") {
            return Engine::MULTIPLAYER_TEAM_BLUE;
        }
        else {
            throw std::runtime_error("Invalid multiplayer team.");
        }
    }

    Engine::Point2DInt lua_to_point2_d_int(lua_State *state, int index) {
        Engine::Point2DInt point;
        if(lua_istable(state, index)) {
            auto get_field = [&state, &index](const char *field, std::int16_t &value) {
                lua_getfield(state, index, field);
                if(lua_isinteger(state, -1)) {
                    value = lua_tointeger(state, -1);
                }
                else {
                    auto *error = "Expected integer for Point2DInt field.";
                    throw std::runtime_error(error);
                }
                lua_pop(state, 1);
            };

            try {
                get_field("x", point.x);
                get_field("y", point.y);
            }
            catch(std::runtime_error &e) {
                throw;
            }
        }
        else {
            if(lua_gettop(state) >= index + 1) {
                auto get_field = [&state, &index](std::int16_t &value) {
                    if(lua_isinteger(state, index)) {
                        value = lua_tointeger(state, index);
                        index++;
                    }
                    else {
                        auto *error = "Expected integer for Point2DInt field.";
                        luaL_error(state, error);
                        throw std::runtime_error(error);
                    }
                };

                try {
                    get_field(point.x);
                    get_field(point.y);
                }
                catch(std::runtime_error &e) {
                    throw;
                }
            }
            else {
                throw std::runtime_error("Could not get Point2DInt from table or stack.");
            }
        }
        return point;
    }

    Engine::Point3D lua_to_point3_d(lua_State *state, int index) {
        Engine::Point3D point;
        if(lua_istable(state, index)) {
            auto get_field = [&state, &index](const char *field, float &value) {
                lua_getfield(state, index, field);
                if(lua_isnumber(state, -1)) {
                    value = lua_tonumber(state, -1);
                }
                else {
                    auto *error = "Expected number for Point3D field.";
                    throw std::runtime_error(error);
                }
                lua_pop(state, 1);
            };

            try {
                get_field("x", point.x);
                get_field("y", point.y);
                get_field("z", point.z);
            }
            catch(std::runtime_error &e) {
                throw;
            }
        }
        else {
            if(lua_gettop(state) >= index + 1) {
                auto get_field = [&state, &index](float &value) {
                    if(lua_isnumber(state, index)) {
                        value = lua_tonumber(state, index);
                        index++;
                    }
                    else {
                        auto *error = "Expected number for Point3D field.";
                        luaL_error(state, error);
                        throw std::runtime_error(error);
                    }
                };

                try {
                    get_field(point.x);
                    get_field(point.y);
                    get_field(point.z);
                }
                catch(std::runtime_error &e) {
                    throw;
                }
            }
            else {
                throw std::runtime_error("Could not get Point3D from table or stack.");
            }
        }
        return point;
    }

    Engine::ColorARGBInt lua_to_color_a_r_g_b_int(lua_State *state, int index) {
        Engine::ColorARGBInt color;
        if(lua_istable(state, index)) {
            auto get_field = [&state, &index](const char *field, std::uint8_t &value) {
                lua_getfield(state, index, field);
                if(lua_isinteger(state, -1)) {
                    value = lua_tointeger(state, -1);
                }
                else {
                    auto *error = "Expected integer for ColorARGBInt field.";
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
            if(lua_gettop(state) >= index + 1) {
                auto get_field = [&state, &index](std::uint8_t &value) {
                    if(lua_isinteger(state, index)) {
                        value = lua_tointeger(state, index);
                        index++;
                    }
                    else {
                        auto *error = "Expected integer for ColorARGBInt field.";
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
                throw std::runtime_error("Could not get ColorARGBInt from table or stack.");
            }
        }
        return color;
    }

    static int lua_engine_color_a_r_g_b_int__index(lua_State *state) {
        lua_getfield(state, 1, "_data"); 
        auto color = static_cast<Engine::ColorARGBInt *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            lua_pushnil(state); 
            return 1; 
        }
        else if(field == "red") { 
            lua_pushinteger(state, color->red); 
            return 1; 
        }
        else if(field == "green") { 
            lua_pushinteger(state, color->green); 
            return 1; 
        }
        else if(field == "blue") { 
            lua_pushinteger(state, color->blue); 
            return 1; 
        }
        else if(field == "alpha") { 
            lua_pushinteger(state, color->alpha); 
            return 1; 
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
    }

    static int lua_engine_color_a_r_g_b_int__newindex(lua_State *state) {
        lua_getfield(state, 1, "_data"); 
        auto color = static_cast<Engine::ColorARGBInt *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            return luaL_error(state, "Cannot set _data"); 
        }
        else if(field == "red") { 
            color->red = static_cast<std::uint8_t>(luaL_checkinteger(state, 3)); 
        }
        else if(field == "green") { 
            color->green = static_cast<std::uint8_t>(luaL_checkinteger(state, 3)); 
        }
        else if(field == "blue") { 
            color->blue = static_cast<std::uint8_t>(luaL_checkinteger(state, 3)); 
        }
        else if(field == "alpha") { 
            color->alpha = static_cast<std::uint8_t>(luaL_checkinteger(state, 3)); 
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_color_a_r_g_b_int(lua_State *state, Engine::ColorARGBInt &color) noexcept {
        lua_push_meta_object(state, color, lua_engine_color_a_r_g_b_int__index, lua_engine_color_a_r_g_b_int__newindex); 
    }

    static int lua_engine_tag_dependency__index(lua_State *state) {
        lua_getfield(state, 1, "_data"); 
        auto dependency = static_cast<Engine::TagDependency *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            lua_pushnil(state); 
            return 1; 
        }
        else if(field == "handle") { 
            lua_pushinteger(state, dependency->tag_handle.handle); 
            return 1; 
        }
        else if(field == "class") {
            auto tag_class = Engine::tag_class_to_string(*reinterpret_cast<Engine::TagClassInt *>(dependency->tag_fourcc)); 
            lua_pushstring(state, tag_class.c_str()); 
            return 1; 
        }
        else if(field == "path") { 
            char path[dependency->path_size + 1];
            std::memcpy(path, dependency->path, dependency->path_size);
            path[dependency->path_size] = '\0';
            lua_pushstring(state, path);
            return 1; 
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
    }

    static int lua_engine_tag_dependency__newindex(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto dependency = static_cast<Engine::TagDependency *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        }

        std::string field = key;
        if(field == "_data") { 
            return luaL_error(state, "Cannot set _data"); 
        }
        else if(field == "handle") { 
            dependency->tag_handle.handle = luaL_checkinteger(state, 3); 
        }
        else if(field == "class") {
            auto *tag_class = lua_tostring(state, 3);
            if(tag_class == nullptr) {
                return luaL_error(state, "Invalid tag class");
            }
            *reinterpret_cast<Engine::TagClassInt *>(dependency->tag_fourcc) = Engine::tag_class_from_string(tag_class);
        }
        else if(field == "path") { 
            auto *path = lua_tostring(state, 3);
            if(path == nullptr) {
                return luaL_error(state, "Invalid path");
            }
            if(dependency->path_size < std::strlen(path)) {
                return luaL_error(state, "Path is too long");
            }
            std::memcpy(dependency->path, path, std::strlen(path));
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_tag_dependency(lua_State *state, Engine::TagDependency &dependency) noexcept {
        lua_push_meta_object(state, dependency, lua_engine_tag_dependency__index, lua_engine_tag_dependency__newindex); 
    }

    static int lua_engine_point2_d__index(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto point = static_cast<Engine::Point2D *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            lua_pushnil(state); 
            return 1; 
        }
        else if(field == "x") { 
            lua_pushnumber(state, point->x); 
            return 1; 
        }
        else if(field == "y") { 
            lua_pushnumber(state, point->y); 
            return 1; 
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
    }

    static int lua_engine_point2_d__newindex(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto point = static_cast<Engine::Point2D *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            return luaL_error(state, "Cannot set _data"); 
        }
        else if(field == "x") { 
            point->x = luaL_checknumber(state, 3); 
        }
        else if(field == "y") { 
            point->y = luaL_checknumber(state, 3); 
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_point2_d(lua_State *state, Engine::Point2D &point) noexcept {
        lua_push_meta_object(state, point, lua_engine_point2_d__index, lua_engine_point2_d__newindex); 
    }

    static int lua_engine_point3_d__index(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto point = static_cast<Engine::Point3D *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            lua_pushnil(state); 
            return 1; 
        }
        else if(field == "x") { 
            lua_pushinteger(state, point->x); 
            return 1; 
        }
        else if(field == "y") { 
            lua_pushinteger(state, point->y); 
            return 1; 
        }
        else if(field == "z") { 
            lua_pushinteger(state, point->z); 
            return 1; 
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
    }

    static int lua_engine_point3_d__newindex(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto point = static_cast<Engine::Point3D *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            return luaL_error(state, "Cannot set _data"); 
        }
        else if(field == "x") { 
            point->x = luaL_checkinteger(state, 3); 
        }
        else if(field == "y") { 
            point->y = luaL_checkinteger(state, 3); 
        }
        else if(field == "z") { 
            point->z = luaL_checkinteger(state, 3); 
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_point3_d(lua_State *state, Engine::Point3D &point) noexcept {
        lua_push_meta_object(state, point, lua_engine_point3_d__index, lua_engine_point3_d__newindex); 
    }

    static int lua_engine_tag_data_offset__index(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto offset = static_cast<Engine::TagDataOffset *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            lua_pushnil(state); 
            return 1; 
        }
        else if(field == "size") { 
            lua_pushinteger(state, offset->size); 
            return 1; 
        }
        else if(field == "external") { 
            lua_pushboolean(state, offset->external);
            return 1; 
        }
        else if(field == "file_offset") { 
            auto file_offset = offset->file_offset;
            if(file_offset == 0) {
                lua_pushnil(state);
            }
            else {
                lua_pushinteger(state, file_offset);
            }
            return 1; 
        }
        else if(field == "pointer") {
            auto pointer = offset->pointer;
            if(pointer) {
                lua_pushinteger(state, reinterpret_cast<std::uintptr_t>(pointer));
            }
            else {
                lua_pushnil(state);
            }
            return 1; 
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
    }

    static int lua_engine_tag_data_offset__newindex(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto offset = static_cast<Engine::TagDataOffset *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            return luaL_error(state, "Cannot set _data"); 
        }
        else if(field == "size") { 
            offset->size = luaL_checkinteger(state, 3); 
        }
        else if(field == "external") { 
            offset->external = lua_toboolean(state, 3);
        }
        else if(field == "file_offset") { 
            offset->file_offset = luaL_checkinteger(state, 3);
        }
        else if(field == "pointer") {
            offset->pointer = reinterpret_cast<std::byte *>(luaL_checkinteger(state, 3));
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_tag_data_offset(lua_State *state, Engine::TagDataOffset &offset) noexcept {
        lua_push_meta_object(state, offset, lua_engine_tag_data_offset__index, lua_engine_tag_data_offset__newindex); 
    }

    static int lua_engine_color_a_r_g_b__index(lua_State *state) {
        lua_getfield(state, 1, "_data"); 
        auto color = static_cast<Engine::ColorARGB *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            lua_pushnil(state); 
            return 1; 
        }
        else if(field == "alpha") { 
            lua_pushnumber(state, color->alpha); 
            return 1; 
        }
        else if(field == "red") { 
            lua_pushnumber(state, color->red); 
            return 1; 
        }
        else if(field == "green") { 
            lua_pushnumber(state, color->green); 
            return 1; 
        }
        else if(field == "blue") { 
            lua_pushnumber(state, color->blue); 
            return 1; 
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
    }

    static int lua_engine_color_a_r_g_b__newindex(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto color = static_cast<Engine::ColorARGB *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            return luaL_error(state, "Cannot set _data"); 
        }
        else if(field == "alpha") { 
            color->alpha = luaL_checknumber(state, 3); 
        }
        else if(field == "red") { 
            color->red = luaL_checknumber(state, 3); 
        }
        else if(field == "green") { 
            color->green = luaL_checknumber(state, 3); 
        }
        else if(field == "blue") { 
            color->blue = luaL_checknumber(state, 3); 
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_color_a_r_g_b(lua_State *state, Engine::ColorARGB &color) noexcept {
        lua_push_meta_object(state, color, lua_engine_color_a_r_g_b__index, lua_engine_color_a_r_g_b__newindex); 
    }

    static int lua_engine_rectangle2_d__index(lua_State *state) {
        lua_getfield(state, 1, "_data"); 
        auto rectangle = static_cast<Engine::Rectangle2D *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            lua_pushnil(state); 
            return 1; 
        }
        else if(field == "top") { 
            lua_pushinteger(state, rectangle->top); 
            return 1; 
        }
        else if(field == "left") { 
            lua_pushinteger(state, rectangle->left); 
            return 1; 
        }
        else if(field == "bottom") { 
            lua_pushinteger(state, rectangle->bottom); 
            return 1; 
        }
        else if(field == "right") { 
            lua_pushinteger(state, rectangle->right); 
            return 1; 
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
    }

    static int lua_engine_rectangle2_d__newindex(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto rectangle = static_cast<Engine::Rectangle2D *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            return luaL_error(state, "Cannot set _data"); 
        }
        else if(field == "top") { 
            rectangle->top = luaL_checkinteger(state, 3); 
        }
        else if(field == "left") { 
            rectangle->left = luaL_checkinteger(state, 3); 
        }
        else if(field == "bottom") { 
            rectangle->bottom = luaL_checkinteger(state, 3); 
        }
        else if(field == "right") { 
            rectangle->right = luaL_checkinteger(state, 3); 
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_rectangle2_d(lua_State *state, Engine::Rectangle2D &rectangle) noexcept {
        lua_push_meta_object(state, rectangle, lua_engine_rectangle2_d__index, lua_engine_rectangle2_d__newindex); 
    }

    static int lua_engine_point2_d_int__index(lua_State *state) {
        lua_getfield(state, 1, "_data"); 
        auto point = static_cast<Engine::Point2DInt *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            lua_pushnil(state); 
            return 1; 
        }
        else if(field == "x") { 
            lua_pushinteger(state, point->x); 
            return 1; 
        }
        else if(field == "y") { 
            lua_pushinteger(state, point->y); 
            return 1; 
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
    }

    static int lua_engine_point2_d_int__newindex(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto point = static_cast<Engine::Point2DInt *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            return luaL_error(state, "Cannot set _data"); 
        }
        else if(field == "x") { 
            point->x = luaL_checkinteger(state, 3); 
        }
        else if(field == "y") { 
            point->y = luaL_checkinteger(state, 3); 
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_point2_d_int(lua_State *state, Engine::Point2DInt &point) noexcept {
        lua_push_meta_object(state, point, lua_engine_point2_d_int__index, lua_engine_point2_d_int__newindex); 
    }

    static int lua_engine_euler2_d__index(lua_State *state) {
        lua_getfield(state, 1, "_data"); 
        auto euler = static_cast<Engine::Euler2D *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            lua_pushnil(state); 
            return 1; 
        }
        else if(field == "yaw") { 
            lua_pushnumber(state, euler->yaw); 
            return 1; 
        }
        else if(field == "pitch") { 
            lua_pushnumber(state, euler->pitch); 
            return 1; 
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
    }

    static int lua_engine_euler2_d__newindex(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto euler = static_cast<Engine::Euler2D *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            return luaL_error(state, "Cannot set _data"); 
        }
        else if(field == "yaw") { 
            euler->yaw = luaL_checknumber(state, 3); 
        }
        else if(field == "pitch") { 
            euler->pitch = luaL_checknumber(state, 3); 
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_euler2_d(lua_State *state, Engine::Euler2D &euler) noexcept {
        lua_push_meta_object(state, euler, lua_engine_euler2_d__index, lua_engine_euler2_d__newindex); 
    }

    static int lua_engine_euler3_d__index(lua_State *state) {
        lua_getfield(state, 1, "_data"); 
        auto euler = static_cast<Engine::Euler3D *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            lua_pushnil(state); 
            return 1; 
        }
        else if(field == "yaw") { 
            lua_pushnumber(state, euler->yaw); 
            return 1; 
        }
        else if(field == "pitch") { 
            lua_pushnumber(state, euler->pitch); 
            return 1; 
        }
        else if(field == "roll") { 
            lua_pushnumber(state, euler->roll); 
            return 1; 
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
    }

    static int lua_engine_euler3_d__newindex(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto euler = static_cast<Engine::Euler3D *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            return luaL_error(state, "Cannot set _data"); 
        }
        else if(field == "yaw") { 
            euler->yaw = luaL_checknumber(state, 3); 
        }
        else if(field == "pitch") { 
            euler->pitch = luaL_checknumber(state, 3); 
        }
        else if(field == "roll") { 
            euler->roll = luaL_checknumber(state, 3); 
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_euler3_d(lua_State *state, Engine::Euler3D &euler) noexcept {
        lua_push_meta_object(state, euler, lua_engine_euler3_d__index, lua_engine_euler3_d__newindex); 
    }

    static int lua_engine_euler3_d_p_y_r__index(lua_State *state) {
        lua_getfield(state, 1, "_data"); 
        auto euler = static_cast<Engine::Euler3DPYR *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            lua_pushnil(state); 
            return 1; 
        }
        else if(field == "yaw") { 
            lua_pushnumber(state, euler->yaw); 
            return 1; 
        }
        else if(field == "pitch") { 
            lua_pushnumber(state, euler->pitch); 
            return 1; 
        }
        else if(field == "roll") { 
            lua_pushnumber(state, euler->roll); 
            return 1; 
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
    }

    static int lua_engine_euler3_d_p_y_r__newindex(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto euler = static_cast<Engine::Euler3DPYR *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            return luaL_error(state, "Cannot set _data"); 
        }
        else if(field == "yaw") { 
            euler->yaw = luaL_checknumber(state, 3); 
        }
        else if(field == "pitch") { 
            euler->pitch = luaL_checknumber(state, 3); 
        }
        else if(field == "roll") { 
            euler->roll = luaL_checknumber(state, 3); 
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_euler3_d_p_y_r(lua_State *state, Engine::Euler3DPYR &euler) noexcept {
        lua_push_meta_object(state, euler, lua_engine_euler3_d_p_y_r__index, lua_engine_euler3_d_p_y_r__newindex); 
    }

    static int lua_engine_vector2_d__index(lua_State *state) {
        lua_getfield(state, 1, "_data"); 
        auto vector = static_cast<Engine::Vector2D *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            lua_pushnil(state); 
            return 1; 
        }
        else if(field == "i") { 
            lua_pushnumber(state, vector->i); 
            return 1; 
        }
        else if(field == "j") { 
            lua_pushnumber(state, vector->j); 
            return 1; 
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
    }

    static int lua_engine_vector2_d__newindex(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto vector = static_cast<Engine::Vector2D *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            return luaL_error(state, "Cannot set _data"); 
        }
        else if(field == "i") { 
            vector->i = luaL_checknumber(state, 3); 
        }
        else if(field == "j") { 
            vector->j = luaL_checknumber(state, 3); 
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_vector2_d(lua_State *state, Engine::Vector2D &vector) noexcept {
        lua_push_meta_object(state, vector, lua_engine_vector2_d__index, lua_engine_vector2_d__newindex); 
    }

    static int lua_engine_vector3_d__index(lua_State *state) {
        lua_getfield(state, 1, "_data"); 
        auto vector = static_cast<Engine::Vector3D *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            lua_pushnil(state); 
            return 1; 
        }
        else if(field == "i") { 
            lua_pushnumber(state, vector->i); 
            return 1; 
        }
        else if(field == "j") { 
            lua_pushnumber(state, vector->j); 
            return 1; 
        }
        else if(field == "k") { 
            lua_pushnumber(state, vector->k); 
            return 1; 
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
    }

    static int lua_engine_vector3_d__newindex(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto vector = static_cast<Engine::Vector3D *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            return luaL_error(state, "Cannot set _data"); 
        }
        else if(field == "i") { 
            vector->i = luaL_checknumber(state, 3); 
        }
        else if(field == "j") { 
            vector->j = luaL_checknumber(state, 3); 
        }
        else if(field == "k") { 
            vector->k = luaL_checknumber(state, 3); 
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_vector3_d(lua_State *state, Engine::Vector3D &vector) noexcept {
        lua_push_meta_object(state, vector, lua_engine_vector3_d__index, lua_engine_vector3_d__newindex); 
    }

    static int lua_engine_color_r_g_b__index(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto color = static_cast<Engine::ColorRGB *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            lua_pushnil(state); 
            return 1; 
        }
        else if(field == "red") { 
            lua_pushnumber(state, color->red); 
            return 1; 
        }
        else if(field == "green") { 
            lua_pushnumber(state, color->green); 
            return 1; 
        }
        else if(field == "blue") { 
            lua_pushnumber(state, color->blue); 
            return 1; 
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
    }

    static int lua_engine_color_r_g_b__newindex(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto color = static_cast<Engine::ColorRGB *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            return luaL_error(state, "Cannot set _data"); 
        }
        else if(field == "red") { 
            color->red = luaL_checknumber(state, 3); 
        }
        else if(field == "green") { 
            color->green = luaL_checknumber(state, 3); 
        }
        else if(field == "blue") { 
            color->blue = luaL_checknumber(state, 3); 
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_color_r_g_b(lua_State *state, Engine::ColorRGB &color) noexcept {
        lua_push_meta_object(state, color, lua_engine_color_r_g_b__index, lua_engine_color_r_g_b__newindex); 
    }

    static int lua_engine_quaternion__index(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto quaternion = static_cast<Engine::Quaternion *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            lua_pushnil(state); 
            return 1; 
        }
        else if(field == "i") { 
            lua_pushnumber(state, quaternion->i); 
            return 1; 
        }
        else if(field == "j") { 
            lua_pushnumber(state, quaternion->j); 
            return 1; 
        }
        else if(field == "k") { 
            lua_pushnumber(state, quaternion->k); 
            return 1; 
        }
        else if(field == "w") { 
            lua_pushnumber(state, quaternion->w); 
            return 1; 
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
    }

    static int lua_engine_quaternion__newindex(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto quaternion = static_cast<Engine::Quaternion *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            return luaL_error(state, "Cannot set _data"); 
        }
        else if(field == "i") { 
            quaternion->i = luaL_checknumber(state, 3); 
        }
        else if(field == "j") { 
            quaternion->j = luaL_checknumber(state, 3); 
        }
        else if(field == "k") { 
            quaternion->k = luaL_checknumber(state, 3); 
        }
        else if(field == "w") { 
            quaternion->w = luaL_checknumber(state, 3); 
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_quaternion(lua_State *state, Engine::Quaternion &quaternion) noexcept {
        lua_push_meta_object(state, quaternion, lua_engine_quaternion__index, lua_engine_quaternion__newindex); 
    }

    static int lua_engine_plane3_d__index(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto plane = static_cast<Engine::Plane3D *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            lua_pushnil(state); 
            return 1; 
        }
        else if(field == "vector") { 
            lua_push_meta_engine_vector3_d(state, plane->vector);
            return 1;
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
    }

    static int lua_engine_plane3_d__newindex(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto plane = static_cast<Engine::Plane3D *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            return luaL_error(state, "Cannot set _data"); 
        }
        else if(field == "vector") { 
            return luaL_error(state, "Invalid operation");
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_plane3_d(lua_State *state, Engine::Plane3D &plane) noexcept {
        lua_push_meta_object(state, plane, lua_engine_plane3_d__index, lua_engine_plane3_d__newindex); 
    }

    static int lua_engine_plane2_d__index(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto plane = static_cast<Engine::Plane2D *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            lua_pushnil(state); 
            return 1; 
        }
        else if(field == "vector") { 
            lua_push_meta_engine_vector2_d(state, plane->vector);
            return 1;
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
    }

    static int lua_engine_plane2_d__newindex(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto plane = static_cast<Engine::Plane2D *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            return luaL_error(state, "Cannot set _data"); 
        }
        else if(field == "vector") { 
            return luaL_error(state, "Invalid operation");
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_plane2_d(lua_State *state, Engine::Plane2D &plane) noexcept {
        lua_push_meta_object(state, plane, lua_engine_plane2_d__index, lua_engine_plane2_d__newindex); 
    }

    extern std::string lua_engine_u_i_widget_type_to_string(Engine::TagDefinitions::UIWidgetType value) noexcept; 
    extern Engine::TagDefinitions::UIWidgetType lua_engine_u_i_widget_type_from_string(std::string str); 

    static int lua_engine_widget__index(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto widget = static_cast<Engine::Widget *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "definition_tag_handle") { 
            lua_pushinteger(state, widget->definition_tag_handle.handle);
            return 1;
        }
        else if(field == "name") {
            lua_pushstring(state, widget->name);
            return 1;
        }
        else if(field == "hidden") {
            if(widget->hidden == 1) {
                lua_pushboolean(state, true);
            }
            else {
                lua_pushboolean(state, false);
            }
            return 1;
        }
        else if(field == "left_bound") {
            lua_pushinteger(state, widget->left_bound);
            return 1;
        }
        else if(field == "top_bound") {
            lua_pushinteger(state, widget->top_bound);
            return 1;
        }
        else if(field == "type") {
            std::string type = lua_engine_u_i_widget_type_to_string(widget->type);
            lua_pushstring(state, type.c_str());
            return 1;
        }
        else if(field == "ms_to_close") {
            lua_pushinteger(state, widget->ms_to_close);
            return 1;
        }
        else if(field == "ms_to_close_fade_time") {
            lua_pushinteger(state, widget->ms_to_close_fade_time);
            return 1;
        }
        else if(field == "opacity") {
            lua_pushnumber(state, widget->opacity);
            return 1;
        }
        else if(field == "previous_widget") {
            if(widget->previous_widget == nullptr) {
                lua_pushnil(state);
            }
            else {
                lua_push_meta_engine_widget(state, *widget->previous_widget);
            }
            return 1;
        }
        else if(field == "next_widget") {
            if(widget->next_widget == nullptr) {
                lua_pushnil(state);
            }
            else {
                lua_push_meta_engine_widget(state, *widget->next_widget);
            }
            return 1;
        }
        else if(field == "parent_widget") {
            if(widget->parent_widget == nullptr) {
                lua_pushnil(state);
            }
            else {
                lua_push_meta_engine_widget(state, *widget->parent_widget);
            }
            return 1;
        }
        else if(field == "child_widget") {
            if(widget->child_widget == nullptr) {
                lua_pushnil(state);
            }
            else {
                lua_push_meta_engine_widget(state, *widget->child_widget);
            }
            return 1;
        }
        else if(field == "focused_child") {
            if(widget->focused_child == nullptr) {
                lua_pushnil(state);
            }
            else {
                lua_push_meta_engine_widget(state, *widget->focused_child);
            }
            return 1;
        }
        else if(field == "text") {
            lua_pushlightuserdata(state, reinterpret_cast<void *>(const_cast<wchar_t *>(widget->text)));
            return 1;
        }
        else if(field == "cursor_index") {
            lua_pushinteger(state, widget->cursor_index);
            return 1;
        }
        else if(field == "bitmap_index") {
            lua_pushinteger(state, widget->bitmap_index);
            return 1;
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
    }

    static int lua_engine_widget__newindex(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto widget = static_cast<Engine::Widget *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "definition_tag_handle") { 
            widget->definition_tag_handle.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "name") {
            return luaL_error(state, "Unsupported operation");
        }
        else if(field == "hidden") {
            widget->hidden = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "left_bound") {
            widget->left_bound = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "top_bound") {
            widget->top_bound = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "type") {
            try {
                std::string type = luaL_checkstring(state, 3);
                widget->type = lua_engine_u_i_widget_type_from_string(type);
            }
            catch(...) {
                return luaL_error(state, "Invalid value");
            }
        }
        else if(field == "ms_to_close") {
            widget->ms_to_close = luaL_checkinteger(state, 3);
        }
        else if(field == "ms_to_close_fade_time") {
            widget->ms_to_close_fade_time = luaL_checkinteger(state, 3);
        }
        else if(field == "opacity") {
            float opacity = luaL_checknumber(state, 3);
            if(opacity < 0.0f || opacity > 1.0f) {
                return luaL_error(state, "Invalid value");
            }
            widget->opacity = opacity;
        }
        else if(field == "previous_widget") {
            auto *previous_widget = lua_from_meta_object<Engine::Widget>(state, 3);
            if(previous_widget) {
                widget->previous_widget = previous_widget;
            }
            else {
                return luaL_error(state, "Invalid value");
            }
        }
        else if(field == "next_widget") {
            auto *next_widget = lua_from_meta_object<Engine::Widget>(state, 3);
            if(next_widget) {
                widget->next_widget = next_widget;
            }
            else {
                return luaL_error(state, "Invalid value");
            }
        }
        else if(field == "parent_widget") {
            auto *parent_widget = lua_from_meta_object<Engine::Widget>(state, 3);
            if(parent_widget) {
                widget->parent_widget = parent_widget;
            }
            else {
                return luaL_error(state, "Invalid value");
            }
        }
        else if(field == "child_widget") {
            auto *child_widget = lua_from_meta_object<Engine::Widget>(state, 3);
            if(child_widget) {
                widget->child_widget = child_widget;
            }
            else {
                return luaL_error(state, "Invalid value");
            }
        }
        else if(field == "focused_child") {
            auto *focused_child = lua_from_meta_object<Engine::Widget>(state, 3);
            if(focused_child) {
                widget->focused_child = focused_child;
            }
            else {
                return luaL_error(state, "Invalid value");
            }
        }
        else if(field == "text") {
            return luaL_error(state, "Unsupported operation");
        }
        else if(field == "cursor_index") {
            widget->cursor_index = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "bitmap_index") {
            widget->bitmap_index = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_widget(lua_State *state, Engine::Widget &widget) noexcept {
        lua_push_meta_object(state, widget, lua_engine_widget__index, lua_engine_widget__newindex); 
    }

    static int lua_engine_camera_data__index(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto camera = static_cast<Engine::CameraData *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "position") { 
            lua_push_meta_engine_point3_d(state, camera->position);
            return 1;
        }
        else if(field == "orientation") {
            lua_newtable(state);
            lua_push_meta_engine_point3_d(state, camera->orientation[0]);
            lua_rawseti(state, -2, 1);
            lua_push_meta_engine_point3_d(state, camera->orientation[1]);
            lua_rawseti(state, -2, 2);
            return 1;
        }
        else if(field == "fov") {
            lua_pushnumber(state, camera->fov);
            return 1;
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
    }

    static int lua_engine_camera_data__newindex(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto camera = static_cast<Engine::CameraData *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "position") { 
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "orientation") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "fov") {
            camera->fov = luaL_checknumber(state, 3);
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_camera_data(lua_State *state, Engine::CameraData &camera) noexcept {
        lua_push_meta_object(state, camera, lua_engine_camera_data__index, lua_engine_camera_data__newindex); 
    }

    static int lua_engine_rotation_matrix__index(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto rotation_matrix = static_cast<Engine::RotationMatrix *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto index = luaL_checkinteger(state, 2);

        if(index < 1 || index > 3) {  
            return luaL_error(state, "Index out of bounds"); 
        } 
        
        if(index == 1) { 
            lua_push_meta_engine_point3_d(state, rotation_matrix->v[0]);
        }
        else if(index == 2) {
            lua_push_meta_engine_point3_d(state, rotation_matrix->v[1]);
        }
        else if(index == 3) {
            lua_push_meta_engine_point3_d(state, rotation_matrix->v[2]);
        }
        return 1;
    }

    static int lua_engine_rotation_matrix__newindex(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto rotation_matrix = static_cast<Engine::RotationMatrix *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto index = luaL_checkinteger(state, 2);

        if(index < 1 || index > 3) {  
            return luaL_error(state, "Index out of bounds"); 
        } 
        
        if(index == 1) { 
            return luaL_error(state, "Invalid operation");
        }
        else if(index == 2) {
            return luaL_error(state, "Invalid operation");
        }
        else if(index == 3) {
            return luaL_error(state, "Invalid operation");
        }
        return 0;
    }

    void lua_push_meta_engine_rotation_matrix(lua_State *state, Engine::RotationMatrix &matrix) noexcept {
        lua_push_meta_object(state, matrix, lua_engine_rotation_matrix__index, lua_engine_rotation_matrix__newindex); 
    }

    static int lua_engine_model_node__index(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto model_node = static_cast<Engine::ModelNode *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "scale") { 
            lua_pushnumber(state, model_node->scale);
            return 1;
        }
        else if(field == "rotation_matrix") {
            lua_push_meta_engine_rotation_matrix(state, model_node->rotation);
            return 1;
        }
        else if(field == "position") {
            lua_push_meta_engine_point3_d(state, model_node->position);
            return 1;
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    static int lua_engine_model_node__newindex(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto model_node = static_cast<Engine::ModelNode *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "scale") { 
            model_node->scale = luaL_checknumber(state, 3);
        }
        else if(field == "rotation_matrix") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "position") {
            return luaL_error(state, "Invalid operation");
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_model_node(lua_State *state, Engine::ModelNode &node) noexcept {
        lua_push_meta_object(state, node, lua_engine_model_node__index, lua_engine_model_node__newindex); 
    }

    static int lua_engine_dynamic_object_flags__index(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto flags = static_cast<Engine::DynamicObjectFlags *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "no_collision") { 
            lua_pushboolean(state, flags->no_collision);
        }
        else if(field == "on_ground") {
            lua_pushboolean(state, flags->on_ground);
        }
        else if(field == "ignore_gravity") {
            lua_pushboolean(state, flags->ignore_gravity);
        }
        else if(field == "in_water") {
            lua_pushboolean(state, flags->in_water);
        }
        else if(field == "stationary") {
            lua_pushboolean(state, flags->stationary);
        }
        else if(field == "no_collision2") {
            lua_pushboolean(state, flags->no_collision2);
        }
        else if(field == "has_sound_looping_attachment") {
            lua_pushboolean(state, flags->has_sound_looping_attachment);
        }
        else if(field == "connected_to_map") {
            lua_pushboolean(state, flags->connected_to_map);
        }
        else if(field == "not_placed_automatically") {
            lua_pushboolean(state, flags->not_placed_automatically);
        }
        else if(field == "is_device_machine") {
            lua_pushboolean(state, flags->is_device_machine);
        }
        else if(field == "is_elevator") {
            lua_pushboolean(state, flags->is_elevator);
        }
        else if(field == "is_elevator_2") {
            lua_pushboolean(state, flags->is_elevator_2);
        }
        else if(field == "is_garbage") {
            lua_pushboolean(state, flags->is_garbage);
        }
        else if(field == "no_shadow") {
            lua_pushboolean(state, flags->no_shadow);
        }
        else if(field == "delete_at_deactivation") {
            lua_pushboolean(state, flags->delete_at_deactivation);
        }
        else if(field == "do_not_reactivate") {
            lua_pushboolean(state, flags->do_not_reactivate);
        }
        else if(field == "outside_of_map") {
            lua_pushboolean(state, flags->outside_of_map);
        }
        else if(field == "collidable") {
            lua_pushboolean(state, flags->collidable);
        }
        else if(field == "has_collision_model") {
            lua_pushboolean(state, flags->has_collision_model);
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_dynamic_object_flags__newindex(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto flags = static_cast<Engine::DynamicObjectFlags *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_isboolean(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        }
        
        std::string field = key;
        bool value = lua_toboolean(state, 3);
        if(field == "no_collision") { 
            flags->no_collision = value;
        }
        else if(field == "on_ground") {
            flags->on_ground = value;
        }
        else if(field == "ignore_gravity") {
            flags->ignore_gravity = value;
        }
        else if(field == "in_water") {
            flags->in_water = value;
        }
        else if(field == "stationary") {
            flags->stationary = value;
        }
        else if(field == "no_collision2") {
            flags->no_collision2 = value;
        }
        else if(field == "has_sound_looping_attachment") {
            flags->has_sound_looping_attachment = value;
        }
        else if(field == "connected_to_map") {
            flags->connected_to_map = value;
        }
        else if(field == "not_placed_automatically") {
            flags->not_placed_automatically = value;
        }
        else if(field == "is_device_machine") {
            flags->is_device_machine = value;
        }
        else if(field == "is_elevator") {
            flags->is_elevator = value;
        }
        else if(field == "is_elevator_2") {
            flags->is_elevator_2 = value;
        }
        else if(field == "is_garbage") {
            flags->is_garbage = value;
        }
        else if(field == "no_shadow") {
            flags->no_shadow = value;
        }
        else if(field == "delete_at_deactivation") {
            flags->delete_at_deactivation = value;
        }
        else if(field == "do_not_reactivate") {
            flags->do_not_reactivate = value;
        }
        else if(field == "outside_of_map") {
            flags->outside_of_map = value;
        }
        else if(field == "collidable") {
            flags->collidable = value;
        }
        else if(field == "has_collision_model") {
            flags->has_collision_model = value;
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_dynamic_object_flags(lua_State *state, Engine::DynamicObjectFlags &flags) noexcept {
        lua_push_meta_object(state, flags, lua_engine_dynamic_object_flags__index, lua_engine_dynamic_object_flags__newindex); 
    }

    static int lua_engine_dynamic_object_network__index(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto object_network = static_cast<Engine::DynamicObjectNetwork *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "valid_position") { 
            lua_pushboolean(state, object_network->valid_position);
        }
        else if(field == "position") {
            lua_push_meta_engine_vector3_d(state, object_network->position);
        }
        else if(field == "valid_forward_and_up") { 
            lua_pushboolean(state, object_network->valid_forward_and_up);
        }
        else if(field == "orientation") {
            lua_newtable(state);
            lua_push_meta_engine_point3_d(state, object_network->orientation[0]);
            lua_rawseti(state, -2, 1);
            lua_push_meta_engine_point3_d(state, object_network->orientation[1]);
            lua_rawseti(state, -2, 2);
        }
        else if(field == "valid_transitional_velocity") { 
            lua_pushboolean(state, object_network->valid_transitional_velocity);
        }
        else if(field == "transitional_velocity") {
            lua_push_meta_engine_vector3_d(state, object_network->transitional_velocity);
        }
        else if(field == "valid_timestamp") { 
            lua_pushboolean(state, object_network->valid_timestamp);
        }
        else if(field == "timestamp") { 
            lua_pushinteger(state, object_network->timestamp);
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_dynamic_object_network__newindex(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto object_network = static_cast<Engine::DynamicObjectNetwork *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key"); 
        } 
        
        std::string field = key;
        if(field == "valid_position") { 
            if(lua_isboolean(state, 3)) {
                object_network->valid_position = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value");
            }
        }
        else if(field == "position") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "valid_forward_and_up") { 
            if(lua_isboolean(state, 3)) {
                object_network->valid_forward_and_up = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value");
            }
        }
        else if(field == "orientation") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "valid_transitional_velocity") { 
            if(lua_isboolean(state, 3)) {
                object_network->valid_transitional_velocity = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value");
            }
        }
        else if(field == "transitional_velocity") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "valid_timestamp") { 
            if(lua_isboolean(state, 3)) {
                object_network->valid_timestamp = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value");
            }
        }
        else if(field == "timestamp") { 
            if(lua_isinteger(state, 3)) {
                object_network->timestamp = static_cast<Engine::TickCount>(lua_tointeger(state, 3));
            }
            else {
                return luaL_error(state, "Invalid value");
            }
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_dynamic_object_network(lua_State *state, Engine::DynamicObjectNetwork &network) noexcept {
        lua_push_meta_object(state, network, lua_engine_dynamic_object_network__index, lua_engine_dynamic_object_network__newindex); 
    }

    static int lua_engine_scenario_location__index(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto location = static_cast<Engine::ScenarioLocation *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "leaf_id") { 
            lua_pushinteger(state, location->leaf_id);
        }
        else if(field == "cluster_id") {
            lua_pushinteger(state, location->cluster_id);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_scenario_location__newindex(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto location = static_cast<Engine::ScenarioLocation *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key"); 
        } 
        
        std::string field = key;
        if(field == "leaf_id") { 
            location->leaf_id = static_cast<std::int32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "cluster_id") {
            location->cluster_id = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_scenario_location(lua_State *state, Engine::ScenarioLocation &location) noexcept {
        lua_push_meta_object(state, location, lua_engine_scenario_location__index, lua_engine_scenario_location__newindex); 
    }

    static int lua_engine_dynamic_object_vitals_flags__index(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto flags = static_cast<Engine::DynamicObjectVitalsFlags *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "health_damage_effect_applied") { 
            lua_pushboolean(state, flags->health_damage_effect_applied);
        }
        else if(field == "shield_damage_effect_applied") { 
            lua_pushboolean(state, flags->shield_damage_effect_applied);
        }
        else if(field == "health_depleted") { 
            lua_pushboolean(state, flags->health_depleted);
        }
        else if(field == "shield_depleted") { 
            lua_pushboolean(state, flags->shield_depleted);
        }
        else if(field == "killed") { 
            lua_pushboolean(state, flags->killed);
        }
        else if(field == "killed_silent") { 
            lua_pushboolean(state, flags->killed_silent);
        }
        else if(field == "cannot_melee_attack") { 
            lua_pushboolean(state, flags->cannot_melee_attack);
        }
        else if(field == "invulnerable") { 
            lua_pushboolean(state, flags->invulnerable);
        }
        else if(field == "shield_recharging") { 
            lua_pushboolean(state, flags->shield_recharging);
        }
        else if(field == "killed_no_stats") { 
            lua_pushboolean(state, flags->killed_no_stats);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_dynamic_object_vitals_flags__newindex(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto flags = static_cast<Engine::DynamicObjectVitalsFlags *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_isboolean(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        bool value = lua_toboolean(state, 3);
        if(field == "health_damage_effect_applied") { 
            flags->health_damage_effect_applied = value;
        }
        else if(field == "shield_damage_effect_applied") { 
            flags->shield_damage_effect_applied = value;
        }
        else if(field == "health_depleted") { 
            flags->health_depleted = value;
        }
        else if(field == "shield_depleted") { 
            flags->shield_depleted = value;
        }
        else if(field == "killed") { 
            flags->killed = value;
        }
        else if(field == "killed_silent") { 
            flags->killed_silent = value;
        }
        else if(field == "cannot_melee_attack") { 
            flags->cannot_melee_attack = value;
        }
        else if(field == "invulnerable") { 
            flags->invulnerable = value;
        }
        else if(field == "shield_recharging") { 
            flags->shield_recharging = value;
        }
        else if(field == "killed_no_stats") { 
            flags->killed_no_stats = value;
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_dynamic_object_vitals_flags(lua_State *state, Engine::DynamicObjectVitalsFlags &flags) noexcept {
        lua_push_meta_object(state, flags, lua_engine_dynamic_object_vitals_flags__index, lua_engine_dynamic_object_vitals_flags__newindex); 
    }

    static int lua_engine_dynamic_object_vitals__index(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto vitals = static_cast<Engine::DynamicObjectVitals *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "base_health") { 
            lua_pushnumber(state, vitals->base_health);
        }
        else if(field == "base_shield") {
            lua_pushnumber(state, vitals->base_shield);
        }
        else if(field == "health") {
            lua_pushnumber(state, vitals->health);
        }
        else if(field == "shield") {
            lua_pushnumber(state, vitals->shield);
        }
        else if(field == "current_shield_damage") {
            lua_pushnumber(state, vitals->current_shield_damage);
        }
        else if(field == "current_health_damage") {
            lua_pushnumber(state, vitals->current_health_damage);
        }
        else if(field == "entangled_object") {
            lua_pushinteger(state, vitals->entangled_object.handle);
        }
        else if(field == "recent_shield_damage") {
            lua_pushnumber(state, vitals->recent_shield_damage);
        }
        else if(field == "recent_health_damage") {
            lua_pushnumber(state, vitals->recent_health_damage);
        }
        else if(field == "recent_shield_damage_time") {
            lua_pushinteger(state, vitals->recent_shield_damage_time);
        }
        else if(field == "recent_health_damage_time") {
            lua_pushinteger(state, vitals->recent_health_damage_time);
        }
        else if(field == "shield_stun_time") {
            lua_pushinteger(state, vitals->shield_stun_time);
        }
        else if(field == "flags") {
            lua_push_meta_engine_dynamic_object_vitals_flags(state, vitals->flags);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_dynamic_object_vitals__newindex(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto vitals = static_cast<Engine::DynamicObjectVitals *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key"); 
        } 
        
        std::string field = key;
        if(field == "base_health") { 
            vitals->base_health = luaL_checknumber(state, 3);
        }
        else if(field == "base_shield") {
            vitals->base_shield = luaL_checknumber(state, 3);
        }
        else if(field == "health") {
            vitals->health = luaL_checknumber(state, 3);
        }
        else if(field == "shield") {
            vitals->shield = luaL_checknumber(state, 3);
        }
        else if(field == "current_shield_damage") {
            vitals->current_shield_damage = luaL_checknumber(state, 3);
        }
        else if(field == "current_health_damage") {
            vitals->current_health_damage = luaL_checknumber(state, 3);
        }
        else if(field == "entangled_object") {
            vitals->entangled_object.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "recent_shield_damage") {
            vitals->recent_shield_damage = luaL_checknumber(state, 3);
        }
        else if(field == "recent_health_damage") {
            vitals->recent_health_damage = luaL_checknumber(state, 3);
        }
        else if(field == "recent_shield_damage_time") {
            vitals->recent_shield_damage_time = static_cast<Engine::TickCount>(luaL_checkinteger(state, 3));
        }
        else if(field == "recent_health_damage_time") {
            vitals->recent_health_damage_time = static_cast<Engine::TickCount>(luaL_checkinteger(state, 3));
        }
        else if(field == "shield_stun_time") {
            vitals->shield_stun_time = static_cast<Engine::TickCount16>(luaL_checkinteger(state, 3));
        }
        else if(field == "flags") {
            return luaL_error(state, "Invalid operation");
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_dynamic_object_vitals(lua_State *state, Engine::DynamicObjectVitals &flags) noexcept {
        lua_push_meta_object(state, flags, lua_engine_dynamic_object_vitals__index, lua_engine_dynamic_object_vitals__newindex); 
    }

    static int lua_engine_dynamic_object_attachments_data__index(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto attachments_data = static_cast<Engine::DynamicObjectAttachmentsData *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "types") { 
            lua_newtable(state);
            for(std::size_t i = 0; i < sizeof(attachments_data->types) / sizeof(attachments_data->types[0]); i++) {
                lua_pushstring(state, dynamic_object_attachment_type_to_string(attachments_data->types[i]).c_str());
                lua_rawseti(state, -2, i + 1);
            }
        }
        else if(field == "attachments") {
            lua_newtable(state);
            for(std::size_t i = 0; i < sizeof(attachments_data->attachments) / sizeof(attachments_data->attachments[0]); i++) {
                lua_pushinteger(state, attachments_data->attachments[i].handle);
                lua_rawseti(state, -2, i + 1);
            }
        }
        else if(field == "first_widget") {
            lua_pushinteger(state, attachments_data->first_widget.handle);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_dynamic_object_attachments_data__newindex(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto attachments_data = static_cast<Engine::DynamicObjectAttachmentsData *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_istable(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "types") { 
            std::optional<Engine::DynamicObjectAttachmentType> types[sizeof(attachments_data->types) / sizeof(attachments_data->types[0])];
            try {
                for(std::size_t i = 0; i < sizeof(attachments_data->types) / sizeof(attachments_data->types[0]); i++) {
                    lua_rawgeti(state, 3, i + 1);
                    if(lua_isnil(state, -1)) {
                        types[i] = dynamic_object_attachment_type_from_string(luaL_checkstring(state, -1));
                        lua_pop(state, 1);
                    }
                }
                for(std::size_t i = 0; i < sizeof(attachments_data->types) / sizeof(attachments_data->types[0]); i++) {
                    if(types[i]) {
                        attachments_data->types[i] = *types[i];
                    }
                }
            }
            catch(std::runtime_error &e) {
                return luaL_error(state, e.what());
            }
        }
        else if(field == "attachments") {
            std::optional<Engine::ObjectHandle> attachments[sizeof(attachments_data->attachments) / sizeof(attachments_data->attachments[0])];
            try {
                for(std::size_t i = 0; i < sizeof(attachments_data->attachments) / sizeof(attachments_data->attachments[0]); i++) {
                    lua_rawgeti(state, 3, i + 1);
                    if(lua_isnil(state, -1)) {
                        attachments[i] = static_cast<Engine::ObjectHandle>(luaL_checkinteger(state, -1));
                        lua_pop(state, 1);
                    }
                }
                for(std::size_t i = 0; i < sizeof(attachments_data->attachments) / sizeof(attachments_data->attachments[0]); i++) {
                    if(attachments[i]) {
                        attachments_data->attachments[i] = *attachments[i];
                    }
                }
            }
            catch(std::runtime_error &e) {
                return luaL_error(state, e.what());
            }
        }
        else if(field == "first_widget") {
            attachments_data->first_widget.handle = luaL_checkinteger(state, 3);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_dynamic_object_attachments_data(lua_State *state, Engine::DynamicObjectAttachmentsData &attachments_data) noexcept {
        lua_push_meta_object(state, attachments_data, lua_engine_dynamic_object_attachments_data__index, lua_engine_dynamic_object_attachments_data__newindex); 
    }

    static int lua_engine_dynamic_object_region_destroyeds__index(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto regions = static_cast<Engine::DynamicObjectRegionDestroyeds *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "region_0") { 
            lua_pushboolean(state, regions->region_0);
        }
        else if(field == "region_1") {
            lua_pushboolean(state, regions->region_1);
        }
        else if(field == "region_2") {
            lua_pushboolean(state, regions->region_2);
        }
        else if(field == "region_3") {
            lua_pushboolean(state, regions->region_3);
        }
        else if(field == "region_4") {
            lua_pushboolean(state, regions->region_4);
        }
        else if(field == "region_5") {
            lua_pushboolean(state, regions->region_5);
        }
        else if(field == "region_6") {
            lua_pushboolean(state, regions->region_6);
        }
        else if(field == "region_7") {
            lua_pushboolean(state, regions->region_7);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_dynamic_object_region_destroyeds__newindex(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto regions = static_cast<Engine::DynamicObjectRegionDestroyeds *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_isboolean(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        bool value = lua_toboolean(state, 3);
        if(field == "region_0") { 
            regions->region_0 = value;
        }
        else if(field == "region_1") {
            regions->region_1 = value;
        }
        else if(field == "region_2") {
            regions->region_2 = value;
        }
        else if(field == "region_3") {
            regions->region_3 = value;
        }
        else if(field == "region_4") {
            regions->region_4 = value;
        }
        else if(field == "region_5") {
            regions->region_5 = value;
        }
        else if(field == "region_6") {
            regions->region_6 = value;
        }
        else if(field == "region_7") {
            regions->region_7 = value;
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_dynamic_object_region_destroyeds(lua_State *state, Engine::DynamicObjectRegionDestroyeds &regions) noexcept {
        lua_push_meta_object(state, regions, lua_engine_dynamic_object_region_destroyeds__index, lua_engine_dynamic_object_region_destroyeds__newindex); 
    }

    static int lua_engine_dynamic_object_block_reference__index(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto block_reference = static_cast<Engine::DynamicObjectBlockReference *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "size") { 
            lua_pushinteger(state, block_reference->size);
        }
        else if(field == "offset") {
            lua_pushinteger(state, block_reference->offset);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_dynamic_object_block_reference__newindex(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto block_reference = static_cast<Engine::DynamicObjectBlockReference *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_isinteger(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "size") { 
            block_reference->size = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "offset") {
            block_reference->offset = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_dynamic_object_block_reference(lua_State *state, Engine::DynamicObjectBlockReference &block_reference) noexcept {
        lua_push_meta_object(state, block_reference, lua_engine_dynamic_object_block_reference__index, lua_engine_dynamic_object_block_reference__newindex); 
    }

    extern std::string lua_engine_scenario_team_index_to_string(Engine::TagDefinitions::ScenarioTeamIndex value) noexcept; 
    extern Engine::TagDefinitions::ScenarioTeamIndex lua_engine_scenario_team_index_from_string(std::string str); 

    static int lua_engine_dynamic_object__index(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto object = static_cast<Engine::DynamicObject *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "tag_handle") { 
            lua_pushinteger(state, object->tag_handle.handle);
        }
        else if(field == "network_role") {
            lua_pushstring(state, object_network_role_to_string(object->network_role).c_str());
        }
        else if(field == "should_force_baseline_update") {
            lua_pushboolean(state, object->should_force_baseline_update == 1);
        }
        else if(field == "existence_time") {
            lua_pushinteger(state, object->existence_time);
        }
        else if(field == "flags") {
            lua_push_meta_engine_dynamic_object_flags(state, object->flags);
        }
        else if(field == "object_marker_id") {
            lua_pushinteger(state, object->object_marker_id);
        }
        else if(field == "network") {
            lua_push_meta_engine_dynamic_object_network(state, object->network);
        }
        else if(field == "position") {
            lua_push_meta_engine_point3_d(state, object->position);
        }
        else if(field == "velocity") {
            lua_push_meta_engine_point3_d(state, object->velocity);
        }
        else if(field == "orientation") {
            lua_newtable(state);
            lua_push_meta_engine_point3_d(state, object->orientation[0]);
            lua_rawseti(state, -2, 1);
            lua_push_meta_engine_point3_d(state, object->orientation[1]);
            lua_rawseti(state, -2, 2);
        }
        else if(field == "rotation_velocity") {
            lua_push_meta_engine_euler3_d_p_y_r(state, object->rotation_velocity);
        }
        else if(field == "scenario_location") {
            lua_push_meta_engine_scenario_location(state, object->scenario_location);
        }
        else if(field == "center_position") {
            lua_push_meta_engine_point3_d(state, object->center_position);
        }
        else if(field == "bounding_radius") {
            lua_pushnumber(state, object->bounding_radius);
        }
        else if(field == "scale") {
            lua_pushnumber(state, object->scale);
        }
        else if(field == "type") {
            lua_pushstring(state, object_type_to_string(object->type).c_str());
        }
        else if(field == "team_owner") {
            lua_pushstring(state, lua_engine_scenario_team_index_to_string(object->team_owner).c_str());
        }
        else if(field == "multiplayer_team_owner") {
            lua_pushstring(state, multiplayer_team_to_string(object->multiplayer_team_owner).c_str());
        }
        else if(field == "name_list_index") {
            lua_pushinteger(state, object->name_list_index);
        }
        else if(field == "moving_time") {
            lua_pushinteger(state, object->moving_time);
        }
        else if(field == "variant_index") {
            lua_pushinteger(state, object->variant_index);
        }
        else if(field == "player") {
            lua_pushinteger(state, object->player.handle);
        }
        else if(field == "owner_object") {
            lua_pushinteger(state, object->owner_object.handle);
        }
        else if(field == "animation_tag_handle") {
            lua_pushinteger(state, object->animation_tag_handle.handle);
        }
        else if(field == "animation_index") {
            lua_pushinteger(state, object->animation_index);
        }
        else if(field == "animation_frame") {
            lua_pushinteger(state, object->animation_frame);
        }
        else if(field == "animation_interpolation_frame") {
            lua_pushinteger(state, object->animation_interpolation_frame);
        }
        else if(field == "animation_interpolation_frame_count") {
            lua_pushinteger(state, object->animation_interpolation_frame_count);
        }
        else if(field == "vitals") {
            lua_push_meta_engine_dynamic_object_vitals(state, object->vitals);
        }
        else if(field == "cluster_partition") {
            lua_pushinteger(state, object->cluster_partition.handle);
        }
        else if(field == "unknown_object") {
            lua_pushinteger(state, object->unknown_object.handle);
        }
        else if(field == "next_object") {
            lua_pushinteger(state, object->next_object.handle);
        }
        else if(field == "first_object") {
            lua_pushinteger(state, object->first_object.handle);
        }
        else if(field == "parent_object") {
            lua_pushinteger(state, object->parent_object.handle);
        }
        else if(field == "parent_attachment_node") {
            lua_pushinteger(state, object->parent_attachment_node);
        }
        else if(field == "force_shield_update") {
            lua_pushboolean(state, object->force_shield_update);
        }
        else if(field == "attachment_data") {
            lua_push_meta_engine_dynamic_object_attachments_data(state, object->attachment_data);
        }
        else if(field == "cached_render_state") {
            lua_pushinteger(state, object->cached_render_state.handle);
        }
        else if(field == "region_destroyeds") {
            lua_push_meta_engine_dynamic_object_region_destroyeds(state, object->region_destroyeds);
        }
        else if(field == "shader_permutation") {
            lua_pushinteger(state, object->shader_permutation);
        }
        else if(field == "region_healths") {
            lua_newtable(state);
            for(std::size_t i = 0; i < sizeof(object->region_healths) / sizeof(object->region_healths[0]); i++) {
                lua_pushinteger(state, object->region_healths[i]);
                lua_rawseti(state, -2, i + 1);
            }
        }
        else if(field == "region_permutation_ids") {
            lua_newtable(state);
            for(std::size_t i = 0; i < sizeof(object->region_permutation_ids) / sizeof(object->region_permutation_ids[0]); i++) {
                lua_pushinteger(state, object->region_permutation_ids[i]);
                lua_rawseti(state, -2, i + 1);
            }
        }
        else if(field == "color_change") {
            lua_newtable(state);
            for(std::size_t i = 0; i < sizeof(object->color_change) / sizeof(object->color_change[0]); i++) {
                lua_push_meta_engine_color_r_g_b(state, object->color_change[i]);
                lua_rawseti(state, -2, i + 1);
            }
        }
        else if(field == "color_change_2") {
            lua_newtable(state);
            for(std::size_t i = 0; i < sizeof(object->color_change_2) / sizeof(object->color_change_2[0]); i++) {
                lua_push_meta_engine_color_r_g_b(state, object->color_change_2[i]);
                lua_rawseti(state, -2, i + 1);
            }
        }
        else if(field == "node_orientations") {
            lua_newtable(state);
            lua_push_meta_engine_dynamic_object_block_reference(state, object->node_orientations[0]);
            lua_rawseti(state, -2, 1);
            lua_push_meta_engine_dynamic_object_block_reference(state, object->node_orientations[1]);
            lua_rawseti(state, -2, 2);
        }
        else if(field == "node_matrices_block") {
            lua_push_meta_engine_dynamic_object_block_reference(state, object->node_matrices_block);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_dynamic_object__newindex(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto object = static_cast<Engine::DynamicObject *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key"); 
        } 
        
        std::string field = key;
        if(field == "tag_handle") { 
            object->tag_handle.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "network_role") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "should_force_baseline_update") {
            object->should_force_baseline_update = luaL_checkinteger(state, 3);
        }
        else if(field == "existence_time") {
            object->existence_time = static_cast<Engine::TickCount>(luaL_checkinteger(state, 3));
        }
        else if(field == "flags") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "object_marker_id") {
            object->object_marker_id = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "network") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "position") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "velocity") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "orientation") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "rotation_velocity") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "scenario_location") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "center_position") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "bounding_radius") {
            object->bounding_radius = luaL_checknumber(state, 3);
        }
        else if(field == "scale") {
            object->scale = luaL_checknumber(state, 3);
        }
        else if(field == "type") {
            try {
                object->type = object_type_from_string(luaL_checkstring(state, 3));
            }
            catch(std::runtime_error &e) {
                return luaL_error(state, e.what());
            }
        }
        else if(field == "team_owner") {
            try {
                object->team_owner = lua_engine_scenario_team_index_from_string(luaL_checkstring(state, 3));
            }
            catch(std::runtime_error &e) {
                return luaL_error(state, e.what());
            }
        }
        else if(field == "multiplayer_team_owner") {
            try {
                object->multiplayer_team_owner = multiplayer_team_from_string(luaL_checkstring(state, 3));
            }
            catch(std::runtime_error &e) {
                return luaL_error(state, e.what());
            }
        }
        else if(field == "name_list_index") {
            object->name_list_index = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "moving_time") {
            object->moving_time = static_cast<Engine::TickCount16>(luaL_checkinteger(state, 3));
        }
        else if(field == "variant_index") {
            object->variant_index = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "player") {
            object->player.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "owner_object") {
            object->owner_object.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "animation_tag_handle") {
            object->animation_tag_handle.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "animation_index") {
            object->animation_index = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "animation_frame") {
            object->animation_frame = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "animation_interpolation_frame") {
            object->animation_interpolation_frame = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "animation_interpolation_frame_count") {
            object->animation_interpolation_frame_count = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "vitals") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "cluster_partition") {
            object->cluster_partition.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "unknown_object") {
            object->unknown_object.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "next_object") {
            object->next_object.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "first_object") {
            object->first_object.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "parent_object") {
            object->parent_object.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "parent_attachment_node") {
            object->parent_attachment_node = static_cast<std::uint8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "force_shield_update") {
            if(lua_isboolean(state, 3)) {
                object->force_shield_update = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "attachment_data") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "cached_render_state") {
            object->cached_render_state.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "region_destroyeds") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "shader_permutation") {
            object->shader_permutation = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "region_healths") {
            std::optional<std::uint8_t> region_healths[sizeof(object->region_healths) / sizeof(object->region_healths[0])];
            for(std::size_t i = 0; i < sizeof(object->region_healths) / sizeof(object->region_healths[0]); i++) {
                lua_rawgeti(state, 3, i + 1);
                if(lua_isnil(state, -1)) {
                    region_healths[i] = static_cast<std::uint8_t>(luaL_checkinteger(state, -1));
                    lua_pop(state, 1);
                }
            }
            for(std::size_t i = 0; i < sizeof(object->region_healths) / sizeof(object->region_healths[0]); i++) {
                if(region_healths[i]) {
                    object->region_healths[i] = *region_healths[i];
                }
            }
        }
        else if(field == "region_permutation_ids") {
            std::optional<std::int8_t> region_permutation_ids[sizeof(object->region_permutation_ids) / sizeof(object->region_permutation_ids[0])];
            for(std::size_t i = 0; i < sizeof(object->region_permutation_ids) / sizeof(object->region_permutation_ids[0]); i++) {
                lua_rawgeti(state, 3, i + 1);
                if(lua_isnil(state, -1)) {
                    region_permutation_ids[i] = static_cast<std::int8_t>(luaL_checkinteger(state, -1));
                    lua_pop(state, 1);
                }
            }
            for(std::size_t i = 0; i < sizeof(object->region_permutation_ids) / sizeof(object->region_permutation_ids[0]); i++) {
                if(region_permutation_ids[i]) {
                    object->region_permutation_ids[i] = *region_permutation_ids[i];
                }
            }
        }
        else if(field == "color_change") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "color_change_2") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "node_orientations") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "node_matrices_block") {
            return luaL_error(state, "Invalid operation");
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_dynamic_object(lua_State *state, Engine::DynamicObject &object) noexcept {
        lua_push_meta_object(state, object, lua_engine_dynamic_object__index, lua_engine_dynamic_object__newindex); 
    }
}
