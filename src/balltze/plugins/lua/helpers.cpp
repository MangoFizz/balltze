// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/engine/data_types.hpp>
#include <balltze/engine/tag_definitions.hpp>
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

    std::string object_attachment_type_to_string(Engine::BaseObjectAttachmentType type) {
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

    Engine::BaseObjectAttachmentType object_attachment_type_from_string(const std::string &type) {
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

    std::string camera_type_to_string(Engine::CameraType camera_type) {
        switch(camera_type) {
            case Engine::CAMERA_FIRST_PERSON: 
                return "first_person";
            case Engine::CAMERA_VEHICLE:
                return "vehicle";
            case Engine::CAMERA_CINEMATIC:
                return "cinematic";
            case Engine::CAMERA_DEBUG:
                return "debug";
            default:
                return "unknown";
        }
    }

    Engine::CameraType camera_type_from_string(const std::string &camera_type) {
        if(camera_type == "first_person") {
            return Engine::CAMERA_FIRST_PERSON;
        }
        else if(camera_type == "vehicle") {
            return Engine::CAMERA_VEHICLE;
        }
        else if(camera_type == "cinematic") {
            return Engine::CAMERA_CINEMATIC;
        }
        else if(camera_type == "debug") {
            return Engine::CAMERA_DEBUG;
        }
        else {
            throw std::runtime_error("Invalid camera type.");
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

    void lua_push_engine_tag(lua_State *state, Engine::Tag *tag) noexcept {
        lua_newtable(state);

        auto primary_class = Engine::tag_class_to_string(tag->primary_class);
        lua_pushstring(state, primary_class.c_str());
        lua_setfield(state, -2, "primary_class");

        auto secondary_class = Engine::tag_class_to_string(tag->secondary_class);
        lua_pushstring(state, secondary_class.c_str());
        lua_setfield(state, -2, "secondary_class");

        auto tertiary_class = Engine::tag_class_to_string(tag->tertiary_class);
        lua_pushstring(state, tertiary_class.c_str());
        lua_setfield(state, -2, "tertiary_class");

        lua_pushinteger(state, tag->handle.handle);
        lua_setfield(state, -2, "handle");

        lua_pushstring(state, tag->path);
        lua_setfield(state, -2, "path");

        lua_pushinteger(state, reinterpret_cast<std::uint32_t>(tag->data));
        lua_setfield(state, -2, "data");

        lua_pushboolean(state, tag->indexed);
        lua_setfield(state, -2, "indexed");
    }

    std::string unit_throwing_grenade_state_to_string(Engine::UnitThrowingGrenadeState state) {
        switch(state) {
            case Engine::UNIT_THROWING_GRENADE_STATE_NONE:
                return "none";
            case Engine::UNIT_THROWING_GRENADE_STATE_BEGIN:
                return "begin";
            case Engine::UNIT_THROWING_GRENADE_STATE_IN_HAND:
                return "in_hand";
            case Engine::UNIT_THROWING_GRENADE_STATE_RELEASED:
                return "released";
            default:
                return "unknown";
        }
    }

    Engine::UnitThrowingGrenadeState unit_throwing_grenade_state_from_string(const std::string &state) {
        if(state == "none") {
            return Engine::UNIT_THROWING_GRENADE_STATE_NONE;
        }
        else if(state == "begin") {
            return Engine::UNIT_THROWING_GRENADE_STATE_BEGIN;
        }
        else if(state == "in_hand") {
            return Engine::UNIT_THROWING_GRENADE_STATE_IN_HAND;
        }
        else if(state == "released") {
            return Engine::UNIT_THROWING_GRENADE_STATE_RELEASED;
        }
        else {
            return Engine::UNIT_THROWING_GRENADE_STATE_NONE;
        }
    }

    std::string unit_animation_state_to_string(Engine::UnitAnimationState state) {
        switch(state) {
            case Engine::UNI_ANIMATION_STATE_INVALID:
                return "invalid";
            case Engine::UNI_ANIMATION_STATE_IDLE:
                return "idle";
            case Engine::UNI_ANIMATION_STATE_GESTURE:
                return "gesture";
            case Engine::UNI_ANIMATION_STATE_TURN_LEFT:
                return "turn_left";
            case Engine::UNI_ANIMATION_STATE_TURN_RIGHT:
                return "turn_right";
            case Engine::UNI_ANIMATION_STATE_MOVE_FRONT:
                return "move_front";
            case Engine::UNI_ANIMATION_STATE_MOVE_BACK:
                return "move_back";
            case Engine::UNI_ANIMATION_STATE_MOVE_LEFT:
                return "move_left";
            case Engine::UNI_ANIMATION_STATE_MOVE_RIGHT:
                return "move_right";
            case Engine::UNI_ANIMATION_STATE_STUNNED_FRONT:
                return "stunned_front";
            case Engine::UNI_ANIMATION_STATE_STUNNED_BACK:
                return "stunned_back";
            case Engine::UNI_ANIMATION_STATE_STUNNED_LEFT:
                return "stunned_left";
            case Engine::UNI_ANIMATION_STATE_STUNNED_RIGHT:
                return "stunned_right";
            case Engine::UNI_ANIMATION_STATE_SLIDE_FRONT:
                return "slide_front";
            case Engine::UNI_ANIMATION_STATE_SLIDE_BACK:
                return "slide_back";
            case Engine::UNI_ANIMATION_STATE_SLIDE_LEFT:
                return "slide_left";
            case Engine::UNI_ANIMATION_STATE_SLIDE_RIGHT:
                return "slide_right";
            case Engine::UNI_ANIMATION_STATE_READY:
                return "ready";
            case Engine::UNI_ANIMATION_STATE_PUT_AWAY:
                return "put_away";
            case Engine::UNI_ANIMATION_STATE_AIM_STILL:
                return "aim_still";
            case Engine::UNI_ANIMATION_STATE_AIM_MOVE:
                return "aim_move";
            case Engine::UNI_ANIMATION_STATE_AIRBORNE:
                return "airborne";
            case Engine::UNI_ANIMATION_STATE_LAND_SOFT:
                return "land_soft";
            case Engine::UNI_ANIMATION_STATE_LAND_HARD:
                return "land_hard";
            case Engine::UNI_ANIMATION_STATE_UNKNOWN23:
                return "unknown23";
            case Engine::UNI_ANIMATION_STATE_AIRBORNE_DEAD:
                return "airborne_dead";
            case Engine::UNI_ANIMATION_STATE_LANDING_DEAD:
                return "landing_dead";
            case Engine::UNI_ANIMATION_STATE_SEAT_ENTER:
                return "seat_enter";
            case Engine::UNI_ANIMATION_STATE_SEAT_EXIT:
                return "seat_exit";
            case Engine::UNI_ANIMATION_STATE_CUSTOM_ANIMATION:
                return "custom_animation";
            case Engine::UNI_ANIMATION_STATE_IMPULSE:
                return "impulse";
            case Engine::UNI_ANIMATION_STATE_MELEE:
                return "melee";
            case Engine::UNI_ANIMATION_STATE_MELEE_AIRBORNE:
                return "melee_airborne";
            case Engine::UNI_ANIMATION_STATE_MELEE_CONTINUOUS:
                return "melee_continuous";
            case Engine::UNI_ANIMATION_STATE_THROW_GRENADE:
                return "throw_grenade";
            case Engine::UNI_ANIMATION_STATE_RESSURECT_FRONT:
                return "ressurect_front";
            case Engine::UNI_ANIMATION_STATE_RESSURECT_BACK:
                return "ressurect_back";
            case Engine::UNI_ANIMATION_STATE_FEEDING:
                return "feeding";
            case Engine::UNI_ANIMATION_STATE_SURPRISE_FRONT:
                return "surprise_front";
            case Engine::UNI_ANIMATION_STATE_SURPRISE_BACK:
                return "surprise_back";
            case Engine::UNI_ANIMATION_STATE_LEAP_START:
                return "leap_start";
            case Engine::UNI_ANIMATION_STATE_LEAP_AIRBORNE:
                return "leap_airborne";
            case Engine::UNI_ANIMATION_STATE_LEAP_MELEE:
                return "leap_melee";
            case Engine::UNI_ANIMATION_STATE_UNKNOWN42:
                return "unknown42";
            case Engine::UNI_ANIMATION_STATE_BERSERK:
                return "berserk";
            case Engine::UNI_ANIMATION_STATE_YELO_SEAT_BOARDING:
                return "yelo_seat_boarding";
            case Engine::UNI_ANIMATION_STATE_YELO_SEAT_EJECTING:
                return "yelo_seat_ejecting";
            case Engine::UNI_ANIMATION_STATE_YELO_MOUNTING:
                return "yelo_mounting";
            case Engine::UNI_ANIMATION_STATE_YELO_TRANSFORMING:
                return "yelo_transforming";
            default:
                return "unknown";
        }
    }

    Engine::UnitAnimationState unit_animation_state_from_string(const std::string &state) {
        if(state == "invalid") {
            return Engine::UNI_ANIMATION_STATE_INVALID;
        }
        else if(state == "idle") {
            return Engine::UNI_ANIMATION_STATE_IDLE;
        }
        else if(state == "gesture") {
            return Engine::UNI_ANIMATION_STATE_GESTURE;
        }
        else if(state == "turn_left") {
            return Engine::UNI_ANIMATION_STATE_TURN_LEFT;
        }
        else if(state == "turn_right") {
            return Engine::UNI_ANIMATION_STATE_TURN_RIGHT;
        }
        else if(state == "move_front") {
            return Engine::UNI_ANIMATION_STATE_MOVE_FRONT;
        }
        else if(state == "move_back") {
            return Engine::UNI_ANIMATION_STATE_MOVE_BACK;
        }
        else if(state == "move_left") {
            return Engine::UNI_ANIMATION_STATE_MOVE_LEFT;
        }
        else if(state == "move_right") {
            return Engine::UNI_ANIMATION_STATE_MOVE_RIGHT;
        }
        else if(state == "stunned_front") {
            return Engine::UNI_ANIMATION_STATE_STUNNED_FRONT;
        }
        else if(state == "stunned_back") {
            return Engine::UNI_ANIMATION_STATE_STUNNED_BACK;
        }
        else if(state == "stunned_left") {
            return Engine::UNI_ANIMATION_STATE_STUNNED_LEFT;
        }
        else if(state == "stunned_right") {
            return Engine::UNI_ANIMATION_STATE_STUNNED_RIGHT;
        }
        else if(state == "slide_front") {
            return Engine::UNI_ANIMATION_STATE_SLIDE_FRONT;
        }
        else if(state == "slide_back") {
            return Engine::UNI_ANIMATION_STATE_SLIDE_BACK;
        }
        else if(state == "slide_left") {
            return Engine::UNI_ANIMATION_STATE_SLIDE_LEFT;
        }
        else if(state == "slide_right") {
            return Engine::UNI_ANIMATION_STATE_SLIDE_RIGHT;
        }
        else if(state == "ready") {
            return Engine::UNI_ANIMATION_STATE_READY;
        }
        else if(state == "put_away") {
            return Engine::UNI_ANIMATION_STATE_PUT_AWAY;
        }
        else if(state == "aim_still") {
            return Engine::UNI_ANIMATION_STATE_AIM_STILL;
        }
        else if(state == "aim_move") {
            return Engine::UNI_ANIMATION_STATE_AIM_MOVE;
        }
        else if(state == "airborne") {
            return Engine::UNI_ANIMATION_STATE_AIRBORNE;
        }
        else if(state == "land_soft") {
            return Engine::UNI_ANIMATION_STATE_LAND_SOFT;
        }
        else if(state == "land_hard") {
            return Engine::UNI_ANIMATION_STATE_LAND_HARD;
        }
        else if(state == "unknown23") {
            return Engine::UNI_ANIMATION_STATE_UNKNOWN23;
        }
        else if(state == "airborne_dead") {
            return Engine::UNI_ANIMATION_STATE_AIRBORNE_DEAD;
        }
        else if(state == "landing_dead") {
            return Engine::UNI_ANIMATION_STATE_LANDING_DEAD;
        }
        else if(state == "seat_enter") {
            return Engine::UNI_ANIMATION_STATE_SEAT_ENTER;
        }
        else if(state == "seat_exit") {
            return Engine::UNI_ANIMATION_STATE_SEAT_EXIT;
        }
        else if(state == "custom_animation") {
            return Engine::UNI_ANIMATION_STATE_CUSTOM_ANIMATION;
        }
        else if(state == "impulse") {
            return Engine::UNI_ANIMATION_STATE_IMPULSE;
        }
        else if(state == "melee") {
            return Engine::UNI_ANIMATION_STATE_MELEE;
        }
        else if(state == "melee_airborne") {
            return Engine::UNI_ANIMATION_STATE_MELEE_AIRBORNE;
        }
        else if(state == "melee_continuous") {
            return Engine::UNI_ANIMATION_STATE_MELEE_CONTINUOUS;
        }
        else if(state == "throw_grenade") {
            return Engine::UNI_ANIMATION_STATE_THROW_GRENADE;
        }
        else if(state == "ressurect_front") {
            return Engine::UNI_ANIMATION_STATE_RESSURECT_FRONT;
        }
        else if(state == "ressurect_back") {
            return Engine::UNI_ANIMATION_STATE_RESSURECT_BACK;
        }
        else if(state == "feeding") {
            return Engine::UNI_ANIMATION_STATE_FEEDING;
        }
        else if(state == "surprise_front") {
            return Engine::UNI_ANIMATION_STATE_SURPRISE_FRONT;
        }
        else if(state == "surprise_back") {
            return Engine::UNI_ANIMATION_STATE_SURPRISE_BACK;
        }
        else if(state == "leap_start") {
            return Engine::UNI_ANIMATION_STATE_LEAP_START;
        }
        else if(state == "leap_airborne") {
            return Engine::UNI_ANIMATION_STATE_LEAP_AIRBORNE;
        }
        else if(state == "leap_melee") {
            return Engine::UNI_ANIMATION_STATE_LEAP_MELEE;
        }
        else if(state == "unknown42") {
            return Engine::UNI_ANIMATION_STATE_UNKNOWN42;
        }
        else if(state == "berserk") {
            return Engine::UNI_ANIMATION_STATE_BERSERK;
        }
        else if(state == "yelo_seat_boarding") {
            return Engine::UNI_ANIMATION_STATE_YELO_SEAT_BOARDING;
        }
        else if(state == "yelo_seat_ejecting") {
            return Engine::UNI_ANIMATION_STATE_YELO_SEAT_EJECTING;
        }
        else if(state == "yelo_mounting") {
            return Engine::UNI_ANIMATION_STATE_YELO_MOUNTING;
        }
        else if(state == "yelo_transforming") {
            return Engine::UNI_ANIMATION_STATE_YELO_TRANSFORMING;
        }
        else {
            throw std::runtime_error("Invalid unit animation state.");
        }
    }

    std::string unit_replacement_animation_state_to_string(Engine::UnitReplacementAnimationState state) {
        switch(state) {
            case Engine::UNIT_REPLACEMENT_ANIMATION_STATE_NONE:
                return "none";
            case Engine::UNIT_REPLACEMENT_ANIMATION_STATE_DISARM:
                return "disarm";
            case Engine::UNIT_REPLACEMENT_ANIMATION_STATE_WEAPON_DROP:
                return "weapon_drop";
            case Engine::UNIT_REPLACEMENT_ANIMATION_STATE_WEAPON_READY:
                return "weapon_ready";
            case Engine::UNIT_REPLACEMENT_ANIMATION_STATE_WEAPON_PUT_AWAY:
                return "weapon_put_away";
            case Engine::UNIT_REPLACEMENT_ANIMATION_STATE_WEAPON_RELOAD1:
                return "weapon_reload1";
            case Engine::UNIT_REPLACEMENT_ANIMATION_STATE_WEAPON_RELOAD2:
                return "weapon_reload2";
            case Engine::UNIT_REPLACEMENT_ANIMATION_STATE_MELEE:
                return "melee";
            case Engine::UNIT_REPLACEMENT_ANIMATION_STATE_THROW_GRENADE:
                return "throw_grenade";
            default:
                return "unknown";
        }
    }

    Engine::UnitReplacementAnimationState unit_replacement_animation_state_from_string(const std::string &state) {
        if(state == "none") {
            return Engine::UNIT_REPLACEMENT_ANIMATION_STATE_NONE;
        }
        else if(state == "disarm") {
            return Engine::UNIT_REPLACEMENT_ANIMATION_STATE_DISARM;
        }
        else if(state == "weapon_drop") {
            return Engine::UNIT_REPLACEMENT_ANIMATION_STATE_WEAPON_DROP;
        }
        else if(state == "weapon_ready") {
            return Engine::UNIT_REPLACEMENT_ANIMATION_STATE_WEAPON_READY;
        }
        else if(state == "weapon_put_away") {
            return Engine::UNIT_REPLACEMENT_ANIMATION_STATE_WEAPON_PUT_AWAY;
        }
        else if(state == "weapon_reload1") {
            return Engine::UNIT_REPLACEMENT_ANIMATION_STATE_WEAPON_RELOAD1;
        }
        else if(state == "weapon_reload2") {
            return Engine::UNIT_REPLACEMENT_ANIMATION_STATE_WEAPON_RELOAD2;
        }
        else if(state == "melee") {
            return Engine::UNIT_REPLACEMENT_ANIMATION_STATE_MELEE;
        }
        else if(state == "throw_grenade") {
            return Engine::UNIT_REPLACEMENT_ANIMATION_STATE_THROW_GRENADE;
        }
        else {
            throw std::runtime_error("Invalid unit replacement animation state.");
        }
    }

    std::string unit_overlay_animation_state_to_string(Engine::UnitOverlayAnimationState state) {
        switch(state) {
            case Engine::UNIT_OVERLAY_ANIMATION_STATE_NONE:
                return "none";
            case Engine::UNIT_OVERLAY_ANIMATION_STATE_FIRE1:
                return "fire1";
            case Engine::UNIT_OVERLAY_ANIMATION_STATE_FIRE2:
                return "fire2";
            case Engine::UNIT_OVERLAY_ANIMATION_STATE_CHARGED1:
                return "charged1";
            case Engine::UNIT_OVERLAY_ANIMATION_STATE_CHARGED2:
                return "charged2";
            case Engine::UNIT_OVERLAY_ANIMATION_STATE_CHAMBER1:
                return "chamber1";
            case Engine::UNIT_OVERLAY_ANIMATION_STATE_CHAMBER2:
                return "chamber2";
            default:
                return "unknown";
        }
    }

    Engine::UnitOverlayAnimationState unit_overlay_animation_state_from_string(const std::string &state) {
        if(state == "none") {
            return Engine::UNIT_OVERLAY_ANIMATION_STATE_NONE;
        }
        else if(state == "fire1") {
            return Engine::UNIT_OVERLAY_ANIMATION_STATE_FIRE1;
        }
        else if(state == "fire2") {
            return Engine::UNIT_OVERLAY_ANIMATION_STATE_FIRE2;
        }
        else if(state == "charged1") {
            return Engine::UNIT_OVERLAY_ANIMATION_STATE_CHARGED1;
        }
        else if(state == "charged2") {
            return Engine::UNIT_OVERLAY_ANIMATION_STATE_CHARGED2;
        }
        else if(state == "chamber1") {
            return Engine::UNIT_OVERLAY_ANIMATION_STATE_CHAMBER1;
        }
        else if(state == "chamber2") {
            return Engine::UNIT_OVERLAY_ANIMATION_STATE_CHAMBER2;
        }
        else {
            throw std::runtime_error("Invalid unit overlay animation state.");
        }
    }

    std::string unit_base_seat_to_string(Engine::UnitBaseSeat seat) {
        switch(seat) {
            case Engine::UNIT_BASE_SEAT_ASLEEP:
                return "asleep";
            case Engine::UNIT_BASE_SEAT_ALERT:
                return "alert";
            case Engine::UNIT_BASE_SEAT_STAND:
                return "stand";
            case Engine::UNIT_BASE_SEAT_CROUCH:
                return "crouch";
            case Engine::UNIT_BASE_SEAT_FLEE:
                return "flee";
            case Engine::UNIT_BASE_SEAT_FLAMING:
                return "flaming";
            default:
                return "unknown";
        }
    }

    Engine::UnitBaseSeat unit_base_seat_from_string(const std::string &seat) {
        if(seat == "asleep") {
            return Engine::UNIT_BASE_SEAT_ASLEEP;
        }
        else if(seat == "alert") {
            return Engine::UNIT_BASE_SEAT_ALERT;
        }
        else if(seat == "stand") {
            return Engine::UNIT_BASE_SEAT_STAND;
        }
        else if(seat == "crouch") {
            return Engine::UNIT_BASE_SEAT_CROUCH;
        }
        else if(seat == "flee") {
            return Engine::UNIT_BASE_SEAT_FLEE;
        }
        else if(seat == "flaming") {
            return Engine::UNIT_BASE_SEAT_FLAMING;
        }
        else {
            throw std::runtime_error("Invalid unit base seat.");
        }
    }

    std::string unit_speech_priority_to_string(Engine::UnitSpeechPriority priority) {
        switch(priority) {
            case Engine::UNIT_SPEECH_PRIORITY_NONE:
                return "none";
            case Engine::UNIT_SPEECH_PRIORITY_IDLE:
                return "idle";
            case Engine::UNIT_SPEECH_PRIORITY_PAIN:
                return "pain";
            case Engine::UNIT_SPEECH_PRIORITY_TALK:
                return "talk";
            case Engine::UNIT_SPEECH_PRIORITY_COMMUNICATE:
                return "communicate";
            case Engine::UNIT_SPEECH_PRIORITY_SHOUT:
                return "shout";
            case Engine::UNIT_SPEECH_PRIORITY_SCRIPT:
                return "script";
            case Engine::UNIT_SPEECH_PRIORITY_INVOLUNTARY:
                return "involuntary";
            case Engine::UNIT_SPEECH_PRIORITY_EXLAIM:
                return "exclaim";
            case Engine::UNIT_SPEECH_PRIORITY_SCREAM:
                return "scream";
            case Engine::UNIT_SPEECH_PRIORITY_DEATH:
                return "death";
            default:
                return "unknown";
        }
    }

    Engine::UnitSpeechPriority unit_speech_priority_from_string(const std::string &priority) {
        if(priority == "none") {
            return Engine::UNIT_SPEECH_PRIORITY_NONE;
        }
        else if(priority == "idle") {
            return Engine::UNIT_SPEECH_PRIORITY_IDLE;
        }
        else if(priority == "pain") {
            return Engine::UNIT_SPEECH_PRIORITY_PAIN;
        }
        else if(priority == "talk") {
            return Engine::UNIT_SPEECH_PRIORITY_TALK;
        }
        else if(priority == "communicate") {
            return Engine::UNIT_SPEECH_PRIORITY_COMMUNICATE;
        }
        else if(priority == "shout") {
            return Engine::UNIT_SPEECH_PRIORITY_SHOUT;
        }
        else if(priority == "script") {
            return Engine::UNIT_SPEECH_PRIORITY_SCRIPT;
        }
        else if(priority == "involuntary") {
            return Engine::UNIT_SPEECH_PRIORITY_INVOLUNTARY;
        }
        else if(priority == "exclaim") {
            return Engine::UNIT_SPEECH_PRIORITY_EXLAIM;
        }
        else if(priority == "scream") {
            return Engine::UNIT_SPEECH_PRIORITY_SCREAM;
        }
        else if(priority == "death") {
            return Engine::UNIT_SPEECH_PRIORITY_DEATH;
        }
        else {
            throw std::runtime_error("Invalid unit speech priority.");
        }
    }

    std::string unit_scream_type_to_string(Engine::UnitScreamType type) {
        switch(type) {
            case Engine::UNIT_SCREAM_TYPE_FEAR:
                return "fear";
            case Engine::UNIT_SCREAM_TYPE_ENEMY_GRENADE:
                return "enemy_grenade";
            case Engine::UNIT_SCREAM_TYPE_PAIN:
                return "pain";
            case Engine::UNIT_SCREAM_TYPE_MAIMED_LIMB:
                return "maimed_limb";
            case Engine::UNIT_SCREAM_TYPE_MAIMED_HEAD:
                return "maimed_head";
            case Engine::UNIT_SCREAM_TYPE_RESSURECTION:
                return "ressurection";
            default:
                return "unknown";
        }
    }

    Engine::UnitScreamType unit_scream_type_from_string(const std::string &type) {
        if(type == "fear") {
            return Engine::UNIT_SCREAM_TYPE_FEAR;
        }
        else if(type == "enemy_grenade") {
            return Engine::UNIT_SCREAM_TYPE_ENEMY_GRENADE;
        }
        else if(type == "pain") {
            return Engine::UNIT_SCREAM_TYPE_PAIN;
        }
        else if(type == "maimed_limb") {
            return Engine::UNIT_SCREAM_TYPE_MAIMED_LIMB;
        }
        else if(type == "maimed_head") {
            return Engine::UNIT_SCREAM_TYPE_MAIMED_HEAD;
        }
        else if(type == "ressurection") {
            return Engine::UNIT_SCREAM_TYPE_RESSURECTION;
        }
        else {
            throw std::runtime_error("Invalid unit scream type.");
        }
    }

    std::string ai_communication_type_to_string(Engine::AiCommunicationType type) {
        switch(type) {
            case Engine::AI_COMMUNICATION_TYPE_DEATH:
                return "death";
            case Engine::AI_COMMUNICATION_TYPE_SPREE:
                return "spree";
            case Engine::AI_COMMUNICATION_TYPE_HURT:
                return "hurt";
            case Engine::AI_COMMUNICATION_TYPE_DAMAGE:
                return "damage";
            case Engine::AI_COMMUNICATION_TYPE_SIGHTED_ENEMY:
                return "sighted_enemy";
            case Engine::AI_COMMUNICATION_TYPE_FOUND_ENEMY:
                return "found_enemy";
            case Engine::AI_COMMUNICATION_TYPE_UNEXPECTED_ENEMY:
                return "unexpected_enemy";
            case Engine::AI_COMMUNICATION_TYPE_FOUND_DEAD_FRIEND:
                return "found_dead_friend";
            case Engine::AI_COMMUNICATION_TYPE_ALLEGIANCE_CHANGED:
                return "allegiance_changed";
            case Engine::AI_COMMUNICATION_TYPE_GRENADE_THROWING:
                return "grenade_throwing";
            case Engine::AI_COMMUNICATION_TYPE_GRENADE_STARTLE:
                return "grenade_startle";
            case Engine::AI_COMMUNICATION_TYPE_GRENADE_SIGHTED:
                return "grenade_sighted";
            case Engine::AI_COMMUNICATION_TYPE_GRENADE_DANGER:
                return "grenade_danger";
            case Engine::AI_COMMUNICATION_TYPE_LOST_CONTACT:
                return "lost_contact";
            case Engine::AI_COMMUNICATION_TYPE_BLOCKED:
                return "blocked";
            case Engine::AI_COMMUNICATION_TYPE_ALERT_NONCOMBAT:
                return "alert_noncombat";
            case Engine::AI_COMMUNICATION_TYPE_SEARCH_START:
                return "search_start";
            case Engine::AI_COMMUNICATION_TYPE_SEARCH_QUERY:
                return "search_query";
            case Engine::AI_COMMUNICATION_TYPE_SEARCH_REPORT:
                return "search_report";
            case Engine::AI_COMMUNICATION_TYPE_SEARCH_ABANDON:
                return "search_abandon";
            case Engine::AI_COMMUNICATION_TYPE_SEARCH_GROUP_ABANDON:
                return "search_group_abandon";
            case Engine::AI_COMMUNICATION_TYPE_UNCOVER_START:
                return "uncover_start";
            case Engine::AI_COMMUNICATION_TYPE_ADVANCE:
                return "advance";
            case Engine::AI_COMMUNICATION_TYPE_RETREAT:
                return "retreat";
            case Engine::AI_COMMUNICATION_TYPE_COVER:
                return "cover";
            case Engine::AI_COMMUNICATION_TYPE_SIGHTED_FRIEND_PLAYER:
                return "sighted_friend_player";
            case Engine::AI_COMMUNICATION_TYPE_SHOOTING:
                return "shooting";
            case Engine::AI_COMMUNICATION_TYPE_SHOOTING_VEHICLE:
                return "shooting_vehicle";
            case Engine::AI_COMMUNICATION_TYPE_SHOOTING_BERSERK:
                return "shooting_berserk";
            case Engine::AI_COMMUNICATION_TYPE_SHOOTING_GROUP:
                return "shooting_group";
            case Engine::AI_COMMUNICATION_TYPE_SHOOTING_TRAITOR:
                return "shooting_traitor";
            case Engine::AI_COMMUNICATION_TYPE_FLEE:
                return "flee";
            case Engine::AI_COMMUNICATION_TYPE_FLEE_LEADER_DIED:
                return "flee_leader_died";
            case Engine::AI_COMMUNICATION_TYPE_FLEE_IDLE:
                return "flee_idle";
            case Engine::AI_COMMUNICATION_TYPE_ATTEMPTED_FLEE:
                return "attempted_flee";
            case Engine::AI_COMMUNICATION_TYPE_HIDING_FINISHED:
                return "hiding_finished";
            case Engine::AI_COMMUNICATION_TYPE_VEHICLE_ENTRY:
                return "vehicle_entry";
            case Engine::AI_COMMUNICATION_TYPE_VEHICLE_EXIT:
                return "vehicle_exit";
            case Engine::AI_COMMUNICATION_TYPE_VEHICLE_WOOHOO:
                return "vehicle_woohoo";
            case Engine::AI_COMMUNICATION_TYPE_VEHICLE_SCARED:
                return "vehicle_scared";
            case Engine::AI_COMMUNICATION_TYPE_VEHICLE_FALLING:
                return "vehicle_falling";
            case Engine::AI_COMMUNICATION_TYPE_SURPRISE:
                return "surprise";
            case Engine::AI_COMMUNICATION_TYPE_BERSERK:
                return "berserk";
            case Engine::AI_COMMUNICATION_TYPE_MELEE:
                return "melee";
            case Engine::AI_COMMUNICATION_TYPE_DIVE:
                return "dive";
            case Engine::AI_COMMUNICATION_TYPE_UNCONVER_EXCLAMATION:
                return "uncover_exclamation";
            case Engine::AI_COMMUNICATION_TYPE_FALLING:
                return "falling";
            case Engine::AI_COMMUNICATION_TYPE_LEAP:
                return "leap";
            case Engine::AI_COMMUNICATION_TYPE_POSTCOMBAT_ALONE:
                return "postcombat_alone";
            case Engine::AI_COMMUNICATION_TYPE_POSTCOMBAT_UNSCATHED:
                return "postcombat_unscathed";
            case Engine::AI_COMMUNICATION_TYPE_POSTCOMBAT_WOUNDED:
                return "postcombat_wounded";
            case Engine::AI_COMMUNICATION_TYPE_POSTCOMBAT_MASSACRE:
                return "postcombat_massacre";
            case Engine::AI_COMMUNICATION_TYPE_POSTCOMBAT_TRIUMPH:
                return "postcombat_triumph";
            case Engine::AI_COMMUNICATION_TYPE_POSTCOMBAT_CHECK_ENEMY:
                return "postcombat_check_enemy";
            case Engine::AI_COMMUNICATION_TYPE_POSTCOMBAT_CHECK_FRIEND:
                return "postcombat_check_friend";
            case Engine::AI_COMMUNICATION_TYPE_POSTCOMBAT_SHOOT_CORPSE:
                return "postcombat_shoot_corpse";
            case Engine::AI_COMMUNICATION_TYPE_POSTCOMBAT_CELEBRATE:
                return "postcombat_celebrate";
            default:
                return "unknown";
        }
    }

    Engine::AiCommunicationType ai_communication_type_from_string(const std::string &type) {
        if(type == "death") {
            return Engine::AI_COMMUNICATION_TYPE_DEATH;
        }
        else if(type == "spree") {
            return Engine::AI_COMMUNICATION_TYPE_SPREE;
        }
        else if(type == "hurt") {
            return Engine::AI_COMMUNICATION_TYPE_HURT;
        }
        else if(type == "damage") {
            return Engine::AI_COMMUNICATION_TYPE_DAMAGE;
        }
        else if(type == "sighted_enemy") {
            return Engine::AI_COMMUNICATION_TYPE_SIGHTED_ENEMY;
        }
        else if(type == "found_enemy") {
            return Engine::AI_COMMUNICATION_TYPE_FOUND_ENEMY;
        }
        else if(type == "unexpected_enemy") {
            return Engine::AI_COMMUNICATION_TYPE_UNEXPECTED_ENEMY;
        }
        else if(type == "found_dead_friend") {
            return Engine::AI_COMMUNICATION_TYPE_FOUND_DEAD_FRIEND;
        }
        else if(type == "allegiance_changed") {
            return Engine::AI_COMMUNICATION_TYPE_ALLEGIANCE_CHANGED;
        }
        else if(type == "grenade_throwing") {
            return Engine::AI_COMMUNICATION_TYPE_GRENADE_THROWING;
        }
        else if(type == "grenade_startle") {
            return Engine::AI_COMMUNICATION_TYPE_GRENADE_STARTLE;
        }
        else if(type == "grenade_sighted") {
            return Engine::AI_COMMUNICATION_TYPE_GRENADE_SIGHTED;
        }
        else if(type == "grenade_danger") {
            return Engine::AI_COMMUNICATION_TYPE_GRENADE_DANGER;
        }
        else if(type == "lost_contact") {
            return Engine::AI_COMMUNICATION_TYPE_LOST_CONTACT;
        }
        else if(type == "blocked") {
            return Engine::AI_COMMUNICATION_TYPE_BLOCKED;
        }
        else if(type == "alert_noncombat") {
            return Engine::AI_COMMUNICATION_TYPE_ALERT_NONCOMBAT;
        }
        else if(type == "search_start") {
            return Engine::AI_COMMUNICATION_TYPE_SEARCH_START;
        }
        else if(type == "search_query") {
            return Engine::AI_COMMUNICATION_TYPE_SEARCH_QUERY;
        }
        else if(type == "search_report") {
            return Engine::AI_COMMUNICATION_TYPE_SEARCH_REPORT;
        }
        else if(type == "search_abandon") {
            return Engine::AI_COMMUNICATION_TYPE_SEARCH_ABANDON;
        }
        else if(type == "search_group_abandon") {
            return Engine::AI_COMMUNICATION_TYPE_SEARCH_GROUP_ABANDON;
        }
        else if(type == "uncover_start") {
            return Engine::AI_COMMUNICATION_TYPE_UNCOVER_START;
        }
        else if(type == "advance") {
            return Engine::AI_COMMUNICATION_TYPE_ADVANCE;
        }
        else if(type == "retreat") {
            return Engine::AI_COMMUNICATION_TYPE_RETREAT;
        }
        else if(type == "cover") {
            return Engine::AI_COMMUNICATION_TYPE_COVER;
        }
        else if(type == "sighted_friend_player") {
            return Engine::AI_COMMUNICATION_TYPE_SIGHTED_FRIEND_PLAYER;
        }
        else if(type == "shooting") {
            return Engine::AI_COMMUNICATION_TYPE_SHOOTING;
        }
        else if(type == "shooting_vehicle") {
            return Engine::AI_COMMUNICATION_TYPE_SHOOTING_VEHICLE;
        }
        else if(type == "shooting_berserk") {
            return Engine::AI_COMMUNICATION_TYPE_SHOOTING_BERSERK;
        }
        else if(type == "shooting_group") {
            return Engine::AI_COMMUNICATION_TYPE_SHOOTING_GROUP;
        }
        else if(type == "shooting_traitor") {
            return Engine::AI_COMMUNICATION_TYPE_SHOOTING_TRAITOR;
        }
        else if(type == "flee") {
            return Engine::AI_COMMUNICATION_TYPE_FLEE;
        }
        else if(type == "flee_leader_died") {
            return Engine::AI_COMMUNICATION_TYPE_FLEE_LEADER_DIED;
        }
        else if(type == "flee_idle") {
            return Engine::AI_COMMUNICATION_TYPE_FLEE_IDLE;
        }
        else if(type == "attempted_flee") {
            return Engine::AI_COMMUNICATION_TYPE_ATTEMPTED_FLEE;
        }
        else if(type == "hiding_finished") {
            return Engine::AI_COMMUNICATION_TYPE_HIDING_FINISHED;
        }
        else if(type == "vehicle_entry") {
            return Engine::AI_COMMUNICATION_TYPE_VEHICLE_ENTRY;
        }
        else if(type == "vehicle_exit") {
            return Engine::AI_COMMUNICATION_TYPE_VEHICLE_EXIT;
        }
        else if(type == "vehicle_woohoo") {
            return Engine::AI_COMMUNICATION_TYPE_VEHICLE_WOOHOO;
        }
        else if(type == "vehicle_scared") {
            return Engine::AI_COMMUNICATION_TYPE_VEHICLE_SCARED;
        }
        else if(type == "vehicle_falling") {
            return Engine::AI_COMMUNICATION_TYPE_VEHICLE_FALLING;
        }
        else if(type == "surprise") {
            return Engine::AI_COMMUNICATION_TYPE_SURPRISE;
        }
        else if(type == "berserk") {
            return Engine::AI_COMMUNICATION_TYPE_BERSERK;
        }
        else if(type == "melee") {
            return Engine::AI_COMMUNICATION_TYPE_MELEE;
        }
        else if(type == "dive") {
            return Engine::AI_COMMUNICATION_TYPE_DIVE;
        }
        else if(type == "uncover_exclamation") {
            return Engine::AI_COMMUNICATION_TYPE_UNCONVER_EXCLAMATION;
        }
        else if(type == "falling") {
            return Engine::AI_COMMUNICATION_TYPE_FALLING;
        }
        else if(type == "leap") {
            return Engine::AI_COMMUNICATION_TYPE_LEAP;
        }
        else if(type == "postcombat_alone") {
            return Engine::AI_COMMUNICATION_TYPE_POSTCOMBAT_ALONE;
        }
        else if(type == "postcombat_unscathed") {
            return Engine::AI_COMMUNICATION_TYPE_POSTCOMBAT_UNSCATHED;
        }
        else if(type == "postcombat_wounded") {
            return Engine::AI_COMMUNICATION_TYPE_POSTCOMBAT_WOUNDED;
        }
        else if(type == "postcombat_massacre") {
            return Engine::AI_COMMUNICATION_TYPE_POSTCOMBAT_MASSACRE;
        }
        else if(type == "postcombat_triumph") {
            return Engine::AI_COMMUNICATION_TYPE_POSTCOMBAT_TRIUMPH;
        }
        else if(type == "postcombat_check_enemy") {
            return Engine::AI_COMMUNICATION_TYPE_POSTCOMBAT_CHECK_ENEMY;
        }
        else if(type == "postcombat_check_friend") {
            return Engine::AI_COMMUNICATION_TYPE_POSTCOMBAT_CHECK_FRIEND;
        }
        else if(type == "postcombat_shoot_corpse") {
            return Engine::AI_COMMUNICATION_TYPE_POSTCOMBAT_SHOOT_CORPSE;
        }
        else if(type == "postcombat_celebrate") {
            return Engine::AI_COMMUNICATION_TYPE_POSTCOMBAT_CELEBRATE;
        }
        else {
            throw std::runtime_error("Invalid ai communication type.");
        }
    }

    std::string biped_movement_state_to_string(Engine::BipedMovementState state) {
        switch(state) {
            case Engine::BIPED_MOVEMENT_STATE_MOVING:
                return "moving";
            case Engine::BIPED_MOVEMENT_STATE_IDLE:
                return "idle";
            case Engine::BIPED_MOVEMENT_STATE_GESTURING:
                return "gesturing";
            default:
                return "unknown";
        }
    }

    Engine::BipedMovementState biped_movement_state_from_string(const std::string &state) {
        if(state == "moving") {
            return Engine::BIPED_MOVEMENT_STATE_MOVING;
        }
        else if(state == "idle") {
            return Engine::BIPED_MOVEMENT_STATE_IDLE;
        }
        else if(state == "gesturing") {
            return Engine::BIPED_MOVEMENT_STATE_GESTURING;
        }
        else {
            throw std::runtime_error("Invalid biped movement state.");
        }
    }

    std::string weapon_state_to_string(Engine::WeaponState state) {
        switch(state) {
            case Engine::WEAPON_STATE_IDLE:
                return "idle";
            case Engine::WEAPON_STATE_FIRE1:
                return "fire1";
            case Engine::WEAPON_STATE_FIRE2:
                return "fire2";
            case Engine::WEAPON_STATE_CHAMBER1:
                return "chamber1";
            case Engine::WEAPON_STATE_CHAMBER2:
                return "chamber2";
            case Engine::WEAPON_STATE_RELOAD1:
                return "reload1";
            case Engine::WEAPON_STATE_RELOAD2:
                return "reload2";
            case Engine::WEAPON_STATE_CHARGED1:
                return "charged1";
            case Engine::WEAPON_STATE_CHARGED2:
                return "charged2";
            case Engine::WEAPON_STATE_READY:
                return "ready";
            case Engine::WEAPON_STATE_PUT_AWAY:
                return "put_away";
            default:
                return "unknown";
        }
    }

    Engine::WeaponState weapon_state_from_string(const std::string &state) {
        if(state == "idle") {
            return Engine::WEAPON_STATE_IDLE;
        }
        else if(state == "fire1") {
            return Engine::WEAPON_STATE_FIRE1;
        }
        else if(state == "fire2") {
            return Engine::WEAPON_STATE_FIRE2;
        }
        else if(state == "chamber1") {
            return Engine::WEAPON_STATE_CHAMBER1;
        }
        else if(state == "chamber2") {
            return Engine::WEAPON_STATE_CHAMBER2;
        }
        else if(state == "reload1") {
            return Engine::WEAPON_STATE_RELOAD1;
        }
        else if(state == "reload2") {
            return Engine::WEAPON_STATE_RELOAD2;
        }
        else if(state == "charged1") {
            return Engine::WEAPON_STATE_CHARGED1;
        }
        else if(state == "charged2") {
            return Engine::WEAPON_STATE_CHARGED2;
        }
        else if(state == "ready") {
            return Engine::WEAPON_STATE_READY;
        }
        else if(state == "put_away") {
            return Engine::WEAPON_STATE_PUT_AWAY;
        }
        else {
            throw std::runtime_error("Invalid weapon state.");
        }
    }

    std::string weapon_magazine_state_to_string(Engine::WeaponMagazineState state) {
        switch(state) {
            case Engine::WEAPON_MAGAZINE_STATE_IDLE:
                return "idle";
            case Engine::WEAPON_MAGAZINE_STATE_CHAMBERING_START:
                return "chambering_start";
            case Engine::WEAPON_MAGAZINE_STATE_CHAMBERING_FINISH:
                return "chambering_finish";
            case Engine::WEAPON_MAGAZINE_STATE_CHAMBERING:
                return "chambering";
            default:
                return "unknown";
        }
    }

    Engine::WeaponMagazineState weapon_magazine_state_from_string(const std::string &state) {
        if(state == "idle") {
            return Engine::WEAPON_MAGAZINE_STATE_IDLE;
        }
        else if(state == "chambering_start") {
            return Engine::WEAPON_MAGAZINE_STATE_CHAMBERING_START;
        }
        else if(state == "chambering_finish") {
            return Engine::WEAPON_MAGAZINE_STATE_CHAMBERING_FINISH;
        }
        else if(state == "chambering") {
            return Engine::WEAPON_MAGAZINE_STATE_CHAMBERING;
        }
        else {
            throw std::runtime_error("Invalid weapon magazine state.");
        }
    }

    std::string network_color_to_string(Engine::NetworkColor color) {
        switch(color) {
            case Engine::NETWORK_COLOR_WHITE:
                return "white";
            case Engine::NETWORK_COLOR_BLACK:
                return "black";
            case Engine::NETWORK_COLOR_RED:
                return "red";
            case Engine::NETWORK_COLOR_BLUE:
                return "blue";
            case Engine::NETWORK_COLOR_GRAY:
                return "gray";
            case Engine::NETWORK_COLOR_YELLOW:
                return "yellow";
            case Engine::NETWORK_COLOR_GREEN:
                return "green";
            case Engine::NETWORK_COLOR_PINK:
                return "pink";
            case Engine::NETWORK_COLOR_PURPLE:
                return "purple";
            case Engine::NETWORK_COLOR_CYAN:
                return "cyan";
            case Engine::NETWORK_COLOR_COBALT:
                return "cobalt";
            case Engine::NETWORK_COLOR_ORANGE:
                return "orange";
            case Engine::NETWORK_COLOR_TEAL:
                return "teal";
            case Engine::NETWORK_COLOR_SAGE:
                return "sage";
            case Engine::NETWORK_COLOR_BROWN:
                return "brown";
            case Engine::NETWORK_COLOR_TAN:
                return "tan";
            case Engine::NETWORK_COLOR_MAROON:
                return "maroon";
            case Engine::NETWORK_COLOR_SALMON:
                return "salmon";
            default:
                return "unknown";
        }
    }

    Engine::NetworkColor network_color_from_string(const std::string &color) {
        if(color == "white") {
            return Engine::NETWORK_COLOR_WHITE;
        }
        else if(color == "black") {
            return Engine::NETWORK_COLOR_BLACK;
        }
        else if(color == "red") {
            return Engine::NETWORK_COLOR_RED;
        }
        else if(color == "blue") {
            return Engine::NETWORK_COLOR_BLUE;
        }
        else if(color == "gray") {
            return Engine::NETWORK_COLOR_GRAY;
        }
        else if(color == "yellow") {
            return Engine::NETWORK_COLOR_YELLOW;
        }
        else if(color == "green") {
            return Engine::NETWORK_COLOR_GREEN;
        }
        else if(color == "pink") {
            return Engine::NETWORK_COLOR_PINK;
        }
        else if(color == "purple") {
            return Engine::NETWORK_COLOR_PURPLE;
        }
        else if(color == "cyan") {
            return Engine::NETWORK_COLOR_CYAN;
        }
        else if(color == "cobalt") {
            return Engine::NETWORK_COLOR_COBALT;
        }
        else if(color == "orange") {
            return Engine::NETWORK_COLOR_ORANGE;
        }
        else if(color == "teal") {
            return Engine::NETWORK_COLOR_TEAL;
        }
        else if(color == "sage") {
            return Engine::NETWORK_COLOR_SAGE;
        }
        else if(color == "brown") {
            return Engine::NETWORK_COLOR_BROWN;
        }
        else if(color == "tan") {
            return Engine::NETWORK_COLOR_TAN;
        }
        else if(color == "maroon") {
            return Engine::NETWORK_COLOR_MAROON;
        }
        else if(color == "salmon") {
            return Engine::NETWORK_COLOR_SALMON;
        }
        else {
            throw std::runtime_error("Invalid network color.");
        }
    }

    std::string player_objective_mode_to_string(Engine::PlayerObjectiveMode mode) {
        switch(mode) {
            case Engine::PLAYER_OBJECTIVE_MODE_RACE:
                return "race";
            case Engine::PLAYER_OBJECTIVE_MODE_HILL:
                return "hill";
            case Engine::PLAYER_OBJECTIVE_MODE_JUGGERNAUT:
                return "juggernaut";
            case Engine::PLAYER_OBJECTIVE_MODE_BALL:
                return "ball";
            case Engine::PLAYER_OBJECTIVE_MODE_NONE:
                return "none";
            default:
                return "unknown";
        }
    }

    Engine::PlayerObjectiveMode player_objective_mode_from_string(const std::string &objective) {
        if(objective == "race") {
            return Engine::PLAYER_OBJECTIVE_MODE_RACE;
        }
        else if(objective == "hill") {
            return Engine::PLAYER_OBJECTIVE_MODE_HILL;
        }
        else if(objective == "juggernaut") {
            return Engine::PLAYER_OBJECTIVE_MODE_JUGGERNAUT;
        }
        else if(objective == "ball") {
            return Engine::PLAYER_OBJECTIVE_MODE_BALL;
        }
        else if(objective == "none") {
            return Engine::PLAYER_OBJECTIVE_MODE_NONE;
        }
        else {
            throw std::runtime_error("Invalid player objective mode.");
        }
    }

    static int lua_engine_color_a_r_g_b_int__index(lua_State *state) {
        auto *color = lua_from_meta_object<Engine::ColorARGBInt>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "red") { 
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
        auto *color = lua_from_meta_object<Engine::ColorARGBInt>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "red") { 
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
        auto *dependency = lua_from_meta_object<Engine::TagDependency>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "handle") { 
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
        auto *dependency = lua_from_meta_object<Engine::TagDependency>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        }

        std::string field = key;
        if(field == "handle") { 
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
        auto *point = lua_from_meta_object<Engine::Point2D>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "x") { 
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
        auto *point = lua_from_meta_object<Engine::Point2D>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "x") { 
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
        auto *point = lua_from_meta_object<Engine::Point3D>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "x") { 
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
        auto *point = lua_from_meta_object<Engine::Point3D>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "x") { 
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
        auto *offset = lua_from_meta_object<Engine::TagDataOffset>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "size") { 
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
        auto *offset = lua_from_meta_object<Engine::TagDataOffset>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "size") { 
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
        auto *color = lua_from_meta_object<Engine::ColorARGB>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "alpha") { 
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
        auto *color = lua_from_meta_object<Engine::ColorARGB>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "alpha") { 
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
        auto *rectangle = lua_from_meta_object<Engine::Rectangle2D>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "top") { 
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
        auto *rectangle = lua_from_meta_object<Engine::Rectangle2D>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "top") { 
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

    static int lua_engine_rectangle2_d_f__index(lua_State *state) {
        auto *rectangle = lua_from_meta_object<Engine::Rectangle2DF>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2); 
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "top") { 
            lua_pushnumber(state, rectangle->top); 
            return 1; 
        }
        else if(field == "left") { 
            lua_pushnumber(state, rectangle->left); 
            return 1; 
        }
        else if(field == "bottom") { 
            lua_pushnumber(state, rectangle->bottom); 
            return 1; 
        }
        else if(field == "right") { 
            lua_pushnumber(state, rectangle->right); 
            return 1; 
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
    }

    static int lua_engine_rectangle2_d_f__newindex(lua_State *state) noexcept {
        auto *rectangle = lua_from_meta_object<Engine::Rectangle2DF>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2); 
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "top") { 
            rectangle->top = luaL_checknumber(state, 3); 
        }
        else if(field == "left") { 
            rectangle->left = luaL_checknumber(state, 3); 
        }
        else if(field == "bottom") { 
            rectangle->bottom = luaL_checknumber(state, 3); 
        }
        else if(field == "right") { 
            rectangle->right = luaL_checknumber(state, 3); 
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_rectangle2_d_f(lua_State *state, Engine::Rectangle2DF &rectangle) noexcept {
        lua_push_meta_object(state, rectangle, lua_engine_rectangle2_d_f__index, lua_engine_rectangle2_d_f__newindex); 
    }

    static int lua_engine_point2_d_int__index(lua_State *state) {
        auto *point = lua_from_meta_object<Engine::Point2DInt>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "x") { 
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
        auto *point = lua_from_meta_object<Engine::Point2DInt>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "x") { 
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
        auto *euler = lua_from_meta_object<Engine::Euler2D>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "yaw") { 
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
        auto *euler = lua_from_meta_object<Engine::Euler2D>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "yaw") { 
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
        auto *euler = lua_from_meta_object<Engine::Euler3D>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "yaw") { 
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
        auto *euler = lua_from_meta_object<Engine::Euler3D>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "yaw") { 
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
        auto *euler = lua_from_meta_object<Engine::Euler3DPYR>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "yaw") { 
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
        auto *euler = lua_from_meta_object<Engine::Euler3DPYR>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "yaw") { 
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
        auto *vector = lua_from_meta_object<Engine::Vector2D>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "i") { 
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
        auto *vector = lua_from_meta_object<Engine::Vector2D>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "i") { 
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
        auto *vector = lua_from_meta_object<Engine::Vector3D>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "i") { 
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
        auto *vector = lua_from_meta_object<Engine::Vector3D>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "i") { 
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
        auto *color = lua_from_meta_object<Engine::ColorRGB>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "red") { 
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
        auto *color = lua_from_meta_object<Engine::ColorRGB>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "red") { 
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
        auto *quaternion = lua_from_meta_object<Engine::Quaternion>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "i") { 
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
        auto *quaternion = lua_from_meta_object<Engine::Quaternion>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "i") { 
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
        auto *plane = lua_from_meta_object<Engine::Plane3D>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "vector") { 
            lua_push_meta_engine_vector3_d(state, plane->vector);
            return 1;
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
    }

    static int lua_engine_plane3_d__newindex(lua_State *state) noexcept {
        auto *plane = lua_from_meta_object<Engine::Plane3D>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "vector") { 
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
        auto *plane = lua_from_meta_object<Engine::Plane2D>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "vector") { 
            lua_push_meta_engine_vector2_d(state, plane->vector);
            return 1;
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
    }

    static int lua_engine_plane2_d__newindex(lua_State *state) noexcept {
        auto *plane = lua_from_meta_object<Engine::Plane2D>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "vector") { 
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
        auto *widget = lua_from_meta_object<Engine::Widget>(state, 1); 
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
        auto *widget = lua_from_meta_object<Engine::Widget>(state, 1); 
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
        auto *camera = lua_from_meta_object<Engine::CameraData>(state, 1); 
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
        auto *camera = lua_from_meta_object<Engine::CameraData>(state, 1); 
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
        auto *rotation_matrix = lua_from_meta_object<Engine::RotationMatrix>(state, 1); 
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
        auto *rotation_matrix = lua_from_meta_object<Engine::RotationMatrix>(state, 1); 
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
        auto *model_node = lua_from_meta_object<Engine::ModelNode>(state, 1); 
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
        auto *model_node = lua_from_meta_object<Engine::ModelNode>(state, 1); 
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

    static int lua_engine_object_flags__index(lua_State *state) noexcept {
        auto *flags = lua_from_meta_object<Engine::BaseObjectFlags>(state, 1); 
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

    static int lua_engine_object_flags__newindex(lua_State *state) noexcept {
        auto *flags = lua_from_meta_object<Engine::BaseObjectFlags>(state, 1); 
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

    void lua_push_meta_engine_object_flags(lua_State *state, Engine::BaseObjectFlags &flags) noexcept {
        lua_push_meta_object(state, flags, lua_engine_object_flags__index, lua_engine_object_flags__newindex); 
    }

    static int lua_engine_object_network__index(lua_State *state) noexcept {
        auto *object_network = lua_from_meta_object<Engine::BaseObjectNetwork>(state, 1); 
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

    static int lua_engine_object_network__newindex(lua_State *state) noexcept {
        auto *object_network = lua_from_meta_object<Engine::BaseObjectNetwork>(state, 1); 
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
                object_network->timestamp = static_cast<Engine::TickCount32>(lua_tointeger(state, 3));
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

    void lua_push_meta_engine_object_network(lua_State *state, Engine::BaseObjectNetwork &network) noexcept {
        lua_push_meta_object(state, network, lua_engine_object_network__index, lua_engine_object_network__newindex); 
    }

    static int lua_engine_scenario_location__index(lua_State *state) noexcept {
        auto *location = lua_from_meta_object<Engine::ScenarioLocation>(state, 1); 
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
        auto *location = lua_from_meta_object<Engine::ScenarioLocation>(state, 1); 
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

    static int lua_engine_object_vitals_flags__index(lua_State *state) noexcept {
        auto *flags = lua_from_meta_object<Engine::BaseObjectVitalsFlags>(state, 1); 
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

    static int lua_engine_object_vitals_flags__newindex(lua_State *state) noexcept {
        auto *flags = lua_from_meta_object<Engine::BaseObjectVitalsFlags>(state, 1); 
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

    void lua_push_meta_engine_object_vitals_flags(lua_State *state, Engine::BaseObjectVitalsFlags &flags) noexcept {
        lua_push_meta_object(state, flags, lua_engine_object_vitals_flags__index, lua_engine_object_vitals_flags__newindex); 
    }

    static int lua_engine_object_vitals__index(lua_State *state) noexcept {
        auto *vitals = lua_from_meta_object<Engine::BaseObjectVitals>(state, 1); 
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
            lua_push_meta_engine_object_vitals_flags(state, vitals->flags);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_object_vitals__newindex(lua_State *state) noexcept {
        auto *vitals = lua_from_meta_object<Engine::BaseObjectVitals>(state, 1); 
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
            vitals->recent_shield_damage_time = static_cast<Engine::TickCount32>(luaL_checkinteger(state, 3));
        }
        else if(field == "recent_health_damage_time") {
            vitals->recent_health_damage_time = static_cast<Engine::TickCount32>(luaL_checkinteger(state, 3));
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

    void lua_push_meta_engine_object_vitals(lua_State *state, Engine::BaseObjectVitals &flags) noexcept {
        lua_push_meta_object(state, flags, lua_engine_object_vitals__index, lua_engine_object_vitals__newindex); 
    }

    static int lua_engine_object_attachments_data__index(lua_State *state) noexcept {
        auto *attachments_data = lua_from_meta_object<Engine::BaseObjectAttachmentsData>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "types") { 
            lua_newtable(state);
            for(std::size_t i = 0; i < sizeof(attachments_data->types) / sizeof(attachments_data->types[0]); i++) {
                lua_pushstring(state, object_attachment_type_to_string(attachments_data->types[i]).c_str());
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

    static int lua_engine_object_attachments_data__newindex(lua_State *state) noexcept {
        auto *attachments_data = lua_from_meta_object<Engine::BaseObjectAttachmentsData>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_istable(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "types") { 
            try {
                for(std::size_t i = 0; i < sizeof(attachments_data->types) / sizeof(attachments_data->types[0]); i++) {
                    lua_rawgeti(state, 3, i + 1);
                    if(!lua_isnil(state, -1)) {
                        attachments_data->types[i] = object_attachment_type_from_string(luaL_checkstring(state, -1));
                    }
                    lua_pop(state, 1);
                }
            }
            catch(std::runtime_error &e) {
                return luaL_error(state, e.what());
            }
        }
        else if(field == "attachments") {
            try {
                for(std::size_t i = 0; i < sizeof(attachments_data->attachments) / sizeof(attachments_data->attachments[0]); i++) {
                    lua_rawgeti(state, 3, i + 1);
                    if(!lua_isnil(state, -1)) {
                        attachments_data->attachments[i] = static_cast<Engine::ObjectHandle>(luaL_checkinteger(state, -1));
                    }
                    lua_pop(state, 1);
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

    void lua_push_meta_engine_object_attachments_data(lua_State *state, Engine::BaseObjectAttachmentsData &attachments_data) noexcept {
        lua_push_meta_object(state, attachments_data, lua_engine_object_attachments_data__index, lua_engine_object_attachments_data__newindex); 
    }

    static int lua_engine_object_region_destroyeds__index(lua_State *state) noexcept {
        auto *regions = lua_from_meta_object<Engine::BaseObjectRegionDestroyeds>(state, 1); 
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

    static int lua_engine_object_region_destroyeds__newindex(lua_State *state) noexcept {
        auto *regions = lua_from_meta_object<Engine::BaseObjectRegionDestroyeds>(state, 1); 
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

    void lua_push_meta_engine_object_region_destroyeds(lua_State *state, Engine::BaseObjectRegionDestroyeds &regions) noexcept {
        lua_push_meta_object(state, regions, lua_engine_object_region_destroyeds__index, lua_engine_object_region_destroyeds__newindex); 
    }

    static int lua_engine_object_block_reference__index(lua_State *state) noexcept {
        auto *block_reference = lua_from_meta_object<Engine::BaseObjectBlockReference>(state, 1); 
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

    static int lua_engine_object_block_reference__newindex(lua_State *state) noexcept {
        auto *block_reference = lua_from_meta_object<Engine::BaseObjectBlockReference>(state, 1); 
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

    void lua_push_meta_engine_object_block_reference(lua_State *state, Engine::BaseObjectBlockReference &block_reference) noexcept {
        lua_push_meta_object(state, block_reference, lua_engine_object_block_reference__index, lua_engine_object_block_reference__newindex); 
    }

    extern std::string lua_engine_scenario_team_index_to_string(Engine::TagDefinitions::ScenarioTeamIndex value) noexcept; 
    extern Engine::TagDefinitions::ScenarioTeamIndex lua_engine_scenario_team_index_from_string(std::string str); 

    static int lua_engine_object__index(lua_State *state) noexcept {
        auto *object = lua_from_meta_object<Engine::BaseObject>(state, 1); 
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
            lua_push_meta_engine_object_flags(state, object->flags);
        }
        else if(field == "object_marker_id") {
            lua_pushinteger(state, object->object_marker_id);
        }
        else if(field == "network") {
            lua_push_meta_engine_object_network(state, object->network);
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
            lua_push_meta_engine_object_vitals(state, object->vitals);
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
            lua_push_meta_engine_object_attachments_data(state, object->attachment_data);
        }
        else if(field == "cached_render_state") {
            lua_pushinteger(state, object->cached_render_state.handle);
        }
        else if(field == "region_destroyeds") {
            lua_push_meta_engine_object_region_destroyeds(state, object->region_destroyeds);
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
            lua_push_meta_engine_object_block_reference(state, object->node_orientations[0]);
            lua_rawseti(state, -2, 1);
            lua_push_meta_engine_object_block_reference(state, object->node_orientations[1]);
            lua_rawseti(state, -2, 2);
        }
        else if(field == "node_matrices_block") {
            lua_push_meta_engine_object_block_reference(state, object->node_matrices_block);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_object__newindex(lua_State *state) noexcept {
        auto *object = lua_from_meta_object<Engine::BaseObject>(state, 1); 
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
            object->existence_time = static_cast<Engine::TickCount32>(luaL_checkinteger(state, 3));
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
            for(std::size_t i = 0; i < sizeof(object->region_healths) / sizeof(object->region_healths[0]); i++) {
                lua_rawgeti(state, 3, i + 1);
                if(!lua_isnil(state, -1)) {
                    object->region_healths[i] = static_cast<std::uint8_t>(luaL_checkinteger(state, -1));
                }
                lua_pop(state, 1);
            }
        }
        else if(field == "region_permutation_ids") {
            for(std::size_t i = 0; i < sizeof(object->region_permutation_ids) / sizeof(object->region_permutation_ids[0]); i++) {
                lua_rawgeti(state, 3, i + 1);
                if(!lua_isnil(state, -1)) {
                    object->region_permutation_ids[i] = static_cast<std::int8_t>(luaL_checkinteger(state, -1));
                }
                lua_pop(state, 1);
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

    void lua_push_meta_engine_object(lua_State *state, Engine::BaseObject &object) noexcept {
        lua_push_meta_object(state, object, lua_engine_object__index, lua_engine_object__newindex); 
    }

    static int lua_engine_unit_recent_damager__index(lua_State *state) noexcept {
        auto *object = lua_from_meta_object<Engine::UnitRecentDamager>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "last_damage_time") {
            lua_pushinteger(state, object->last_damage_time);
        }
        else if(field == "total_damage") {
            lua_pushnumber(state, object->total_damage);
        }
        else if(field == "object") {
            lua_pushinteger(state, object->object.handle);
        }
        else if(field == "player") {
            lua_pushinteger(state, object->player.handle);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_unit_recent_damager__newindex(lua_State *state) noexcept {
        auto *object = lua_from_meta_object<Engine::UnitRecentDamager>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key"); 
        } 
        
        std::string field = key;
        if(field == "last_damage_time") {
            object->last_damage_time = static_cast<Engine::TickCount32>(luaL_checkinteger(state, 3));
        }
        else if(field == "total_damage") {
            object->total_damage = luaL_checknumber(state, 3);
        }
        else if(field == "object") {
            object->object.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "player") {
            object->player.handle = luaL_checkinteger(state, 3);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_unit_recent_damager(lua_State *state, Engine::UnitRecentDamager &object) noexcept {
        lua_push_meta_object(state, object, lua_engine_unit_recent_damager__index, lua_engine_unit_recent_damager__newindex); 
    }

    static int lua_engine_unit_flags__index(lua_State *state) {
        auto *flags = lua_from_meta_object<Engine::UnitFlags>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "unknown_biped_speech_related") {
            lua_pushboolean(state, flags->unknown_biped_speech_related);
        }
        else if(field == "power_up") {
            lua_pushboolean(state, flags->power_up);
        }
        else if(field == "power_up_addition") {
            lua_pushboolean(state, flags->power_up_addition);
        }
        else if(field == "controllable") {
            lua_pushboolean(state, flags->controllable);
        }
        else if(field == "berserking") {
            lua_pushboolean(state, flags->berserking);
        }
        else if(field == "unknown_integrated_light_related") {
            lua_pushboolean(state, flags->unknown_integrated_light_related);
        }
        else if(field == "will_not_drop_items") {
            lua_pushboolean(state, flags->will_not_drop_items);
        }
        else if(field == "unknown") {
            lua_pushboolean(state, flags->unknown);
        }
        else if(field == "can_blink") {
            lua_pushboolean(state, flags->can_blink);
        }
        else if(field == "impervious") {
            lua_pushboolean(state, flags->impervious);
        }
        else if(field == "suspended") {
            lua_pushboolean(state, flags->suspended);
        }
        else if(field == "blind") {
            lua_pushboolean(state, flags->blind);
        }
        else if(field == "unknown_nv_related") {
            lua_pushboolean(state, flags->unknown_nv_related);
        }
        else if(field == "possessed") {
            lua_pushboolean(state, flags->possessed);
        }
        else if(field == "desires_flashlight_on") {
            lua_pushboolean(state, flags->desires_flashlight_on);
        }
        else if(field == "desires_flashlight_off") {
            lua_pushboolean(state, flags->desires_flashlight_off);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_unit_flags__newindex(lua_State *state) {
        auto *flags = lua_from_meta_object<Engine::UnitFlags>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_isboolean(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        bool value = lua_toboolean(state, 3);
        if(field == "unknown_biped_speech_related") {
            flags->unknown_biped_speech_related = value;
        }
        else if(field == "power_up") {
            flags->power_up = value;
        }
        else if(field == "power_up_addition") {
            flags->power_up_addition = value;
        }
        else if(field == "controllable") {
            flags->controllable = value;
        }
        else if(field == "berserking") {
            flags->berserking = value;
        }
        else if(field == "unknown_integrated_light_related") {
            flags->unknown_integrated_light_related = value;
        }
        else if(field == "will_not_drop_items") {
            flags->will_not_drop_items = value;
        }
        else if(field == "unknown") {
            flags->unknown = value;
        }
        else if(field == "can_blink") {
            flags->can_blink = value;
        }
        else if(field == "impervious") {
            flags->impervious = value;
        }
        else if(field == "suspended") {
            flags->suspended = value;
        }
        else if(field == "blind") {
            flags->blind = value;
        }
        else if(field == "unknown_nv_related") {
            flags->unknown_nv_related = value;
        }
        else if(field == "possessed") {
            flags->possessed = value;
        }
        else if(field == "desires_flashlight_on") {
            flags->desires_flashlight_on = value;
        }
        else if(field == "desires_flashlight_off") {
            flags->desires_flashlight_off = value;
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_unit_flags(lua_State *state, Engine::UnitFlags &flags) noexcept {
        lua_push_meta_object(state, flags, lua_engine_unit_flags__index, lua_engine_unit_flags__newindex); 
    }

    static int lua_engine_unit_control_flags__index(lua_State *state) noexcept {
        auto *flags = lua_from_meta_object<Engine::UnitControlFlags>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "crouch") {
            lua_pushboolean(state, flags->crouch);
        }
        else if(field == "jump") {
            lua_pushboolean(state, flags->jump);
        }
        else if(field == "user1") {
            lua_pushboolean(state, flags->user1);
        }
        else if(field == "user2") {
            lua_pushboolean(state, flags->user2);
        }
        else if(field == "light") {
            lua_pushboolean(state, flags->light);
        }
        else if(field == "exact_facing") {
            lua_pushboolean(state, flags->exact_facing);
        }
        else if(field == "action") {
            lua_pushboolean(state, flags->action);
        }
        else if(field == "melee") {
            lua_pushboolean(state, flags->melee);
        }
        else if(field == "look_dont_turn") {
            lua_pushboolean(state, flags->look_dont_turn);
        }
        else if(field == "force_alert") {
            lua_pushboolean(state, flags->force_alert);
        }
        else if(field == "reload") {
            lua_pushboolean(state, flags->reload);
        }
        else if(field == "primary_trigger") {
            lua_pushboolean(state, flags->primary_trigger);
        }
        else if(field == "secondary_trigger") {
            lua_pushboolean(state, flags->secondary_trigger);
        }
        else if(field == "grenade") {
            lua_pushboolean(state, flags->grenade);
        }
        else if(field == "exchange_weapon") {
            lua_pushboolean(state, flags->exchange_weapon);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_unit_control_flags__newindex(lua_State *state) noexcept {
        auto *flags = lua_from_meta_object<Engine::UnitControlFlags>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_isboolean(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        bool value = lua_toboolean(state, 3);
        if(field == "crouch") {
            flags->crouch = value;
        }
        else if(field == "jump") {
            flags->jump = value;
        }
        else if(field == "user1") {
            flags->user1 = value;
        }
        else if(field == "user2") {
            flags->user2 = value;
        }
        else if(field == "light") {
            flags->light = value;
        }
        else if(field == "exact_facing") {
            flags->exact_facing = value;
        }
        else if(field == "action") {
            flags->action = value;
        }
        else if(field == "melee") {
            flags->melee = value;
        }
        else if(field == "look_dont_turn") {
            flags->look_dont_turn = value;
        }
        else if(field == "force_alert") {
            flags->force_alert = value;
        }
        else if(field == "reload") {
            flags->reload = value;
        }
        else if(field == "primary_trigger") {
            flags->primary_trigger = value;
        }
        else if(field == "secondary_trigger") {
            flags->secondary_trigger = value;
        }
        else if(field == "grenade") {
            flags->grenade = value;
        }
        else if(field == "exchange_weapon") {
            flags->exchange_weapon = value;
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_unit_control_flags(lua_State *state, Engine::UnitControlFlags &flags) noexcept {
        lua_push_meta_object(state, flags, lua_engine_unit_control_flags__index, lua_engine_unit_control_flags__newindex); 
    }

    static int lua_engine_unit_animation_state_data__index(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<Engine::UnitAnimationStateData>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "animation_index") {
            lua_pushinteger(state, data->animation_index);
        }
        else if(field == "frame_index") {
            lua_pushinteger(state, data->frame_index);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_unit_animation_state_data__newindex(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<Engine::UnitAnimationStateData>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_isinteger(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        std::int16_t value = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        if(field == "animation_index") {
            data->animation_index = value;
        }
        else if(field == "frame_index") {
            data->frame_index = value;
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_unit_animation_state_data(lua_State *state, Engine::UnitAnimationStateData &data) noexcept {
        lua_push_meta_object(state, data, lua_engine_unit_animation_state_data__index, lua_engine_unit_animation_state_data__newindex); 
    }

    static int lua_engine_unit_animation_flags__index(lua_State *state) noexcept {
        auto *flags = lua_from_meta_object<Engine::UnitAnimationFlags>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "animation_bit0_unknown") {
            lua_pushboolean(state, flags->animation_bit0_unknown);
        }
        else if(field == "animation_bit1_unknown") {
            lua_pushboolean(state, flags->animation_bit1_unknown);
        }
        else if(field == "animation_bit2_unknown") {
            lua_pushboolean(state, flags->animation_bit2_unknown);
        }
        else if(field == "animation_bit3_unknown") {
            lua_pushboolean(state, flags->animation_bit3_unknown);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_unit_animation_flags__newindex(lua_State *state) noexcept {
        auto *flags = lua_from_meta_object<Engine::UnitAnimationFlags>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_isboolean(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        bool value = lua_toboolean(state, 3);
        if(field == "animation_bit0_unknown") {
            flags->animation_bit0_unknown = value;
        }
        else if(field == "animation_bit1_unknown") {
            flags->animation_bit1_unknown = value;
        }
        else if(field == "animation_bit2_unknown") {
            flags->animation_bit2_unknown = value;
        }
        else if(field == "animation_bit3_unknown") {
            flags->animation_bit3_unknown = value;
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_unit_animation_flags(lua_State *state, Engine::UnitAnimationFlags &flags) noexcept {
        lua_push_meta_object(state, flags, lua_engine_unit_animation_flags__index, lua_engine_unit_animation_flags__newindex); 
    }

    static int lua_engine_unit_animation_data__index(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<Engine::UnitAnimationData>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "flags") {
            lua_push_meta_engine_unit_animation_flags(state, data->flags);
        }
        else if(field == "unknown_some_animation_index_maybe") {
            lua_pushinteger(state, data->unknown_some_animation_index_maybe);
        }
        else if(field == "unknown_some_animation_index") {
            lua_pushinteger(state, data->unknown_some_animation_index);
        }
        else if(field == "seat_index") {
            lua_pushinteger(state, data->seat_index);
        }
        else if(field == "seat_weapon_index") {
            lua_pushinteger(state, data->seat_weapon_index);
        }
        else if(field == "weapon_type_index") {
            lua_pushinteger(state, data->weapon_type_index);
        }
        else if(field == "state") {
            lua_pushstring(state, unit_animation_state_to_string(data->state).c_str());
        }
        else if(field == "replacement_state") {
            lua_pushstring(state, unit_replacement_animation_state_to_string(data->replacement_state).c_str());
        }
        else if(field == "overlay_state") {
            lua_pushstring(state, unit_overlay_animation_state_to_string(data->overlay_state).c_str());
        }
        else if(field == "desired_state") {
            lua_pushstring(state, unit_animation_state_to_string(data->desired_state).c_str());
        }
        else if(field == "base_seat") {
            lua_pushstring(state, unit_base_seat_to_string(data->base_seat).c_str());
        }
        else if(field == "emotion") {
            lua_pushinteger(state, data->emotion);
        }
        else if(field == "replacement_animation") {
            lua_push_meta_engine_unit_animation_state_data(state, data->replacement_animation);
        }
        else if(field == "overlay_state_animation") {
            lua_push_meta_engine_unit_animation_state_data(state, data->overlay_state_animation);
        }
        else if(field == "weapon_ik") {
            lua_push_meta_engine_unit_animation_state_data(state, data->weapon_ik);
        }
        else if(field == "update_look") {
            lua_pushboolean(state, data->update_look);
        }
        else if(field == "update_aim") {
            lua_pushboolean(state, data->update_aim);
        }
        else if(field == "looking_bounds") {
            lua_push_meta_engine_rectangle2_d_f(state, data->looking_bounds);
        }
        else if(field == "aiming_bounds") {
            lua_push_meta_engine_rectangle2_d_f(state, data->aiming_bounds);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_unit_animation_data__newindex(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<Engine::UnitAnimationData>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "flags") {
            lua_push_meta_engine_unit_animation_flags(state, data->flags);
        }
        else if(field == "unknown_some_animation_index_maybe") {
            data->unknown_some_animation_index_maybe = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "unknown_some_animation_index") {
            data->unknown_some_animation_index = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "seat_index") {
            data->seat_index = static_cast<std::uint8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "seat_weapon_index") {
            data->seat_weapon_index = static_cast<std::uint8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "weapon_type_index") {
            data->weapon_type_index = static_cast<std::uint8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "state") {
            try {
                data->state = unit_animation_state_from_string(luaL_checkstring(state, 3));
            }
            catch(std::runtime_error &e) {
                return luaL_error(state, e.what());
            }
        }
        else if(field == "replacement_state") {
            try {
                data->replacement_state = unit_replacement_animation_state_from_string(luaL_checkstring(state, 3));
            }
            catch(std::runtime_error &e) {
                return luaL_error(state, e.what());
            }
        }
        else if(field == "overlay_state") {
            try {
                data->overlay_state = unit_overlay_animation_state_from_string(luaL_checkstring(state, 3));
            }
            catch(std::runtime_error &e) {
                return luaL_error(state, e.what());
            }
        }
        else if(field == "desired_state") {
            try {
                data->desired_state = unit_animation_state_from_string(luaL_checkstring(state, 3));
            }
            catch(std::runtime_error &e) {
                return luaL_error(state, e.what());
            }
        }
        else if(field == "base_seat") {
            try {
                data->base_seat = unit_base_seat_from_string(luaL_checkstring(state, 3));
            }
            catch(std::runtime_error &e) {
                return luaL_error(state, e.what());
            }
        }
        else if(field == "emotion") {
            data->emotion = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "replacement_animation") {
            lua_push_meta_engine_unit_animation_state_data(state, data->replacement_animation);
        }
        else if(field == "overlay_state_animation") {
            lua_push_meta_engine_unit_animation_state_data(state, data->overlay_state_animation);
        }
        else if(field == "weapon_ik") {
            lua_push_meta_engine_unit_animation_state_data(state, data->weapon_ik);
        }
        else if(field == "update_look") {
            if(lua_isboolean(state, 3)) {
                data->update_look = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "update_aim") {
            if(lua_isboolean(state, 3)) {
                data->update_aim = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "looking_bounds") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "aiming_bounds") {
            return luaL_error(state, "Invalid operation");
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_unit_animation_data(lua_State *state, Engine::UnitAnimationData &data) noexcept {
        lua_push_meta_object(state, data, lua_engine_unit_animation_data__index, lua_engine_unit_animation_data__newindex); 
    }

    static int lua_engine_ai_communication_packet__index(lua_State *state) noexcept {
        auto *packet = lua_from_meta_object<Engine::AiCommunicationPacket>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "type") {
            lua_pushstring(state, ai_communication_type_to_string(packet->type).c_str());
        }
        else if(field == "broken") {
            lua_pushboolean(state, packet->broken);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_ai_communication_packet__newindex(lua_State *state) noexcept {
        auto *packet = lua_from_meta_object<Engine::AiCommunicationPacket>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key"); 
        } 
        
        std::string field = key;
        if(field == "type") {
            try {
                packet->type = ai_communication_type_from_string(luaL_checkstring(state, 3));
            }
            catch(std::runtime_error &e) {
                return luaL_error(state, e.what());
            }
        }
        else if(field == "broken") {
            if(lua_isboolean(state, 3)) {
                packet->broken = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_ai_communication_packet(lua_State *state, Engine::AiCommunicationPacket &packet) noexcept {
        lua_push_meta_object(state, packet, lua_engine_ai_communication_packet__index, lua_engine_ai_communication_packet__newindex); 
    }

    static int lua_engine_unit_speech__index(lua_State *state) noexcept {
        auto *speech = lua_from_meta_object<Engine::UnitSpeech>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "priority") {
            lua_pushstring(state, unit_speech_priority_to_string(speech->priority).c_str());
        }
        else if(field == "scream_type") {
            lua_pushstring(state, unit_scream_type_to_string(speech->scream_type).c_str());
        }
        else if(field == "sound_tag") {
            lua_pushinteger(state, speech->sound_tag.handle);
        }
        else if(field == "ticks") {
            lua_pushinteger(state, speech->ticks);
        }
        else if(field == "unknown2") {
            lua_pushinteger(state, speech->unknown2);
        }
        else if(field == "unknown") {
            lua_pushinteger(state, speech->unknown);
        }
        else if(field == "ai_communication_info") {
            lua_push_meta_engine_ai_communication_packet(state, speech->ai_communication_info);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_unit_speech__newindex(lua_State *state) noexcept {
        auto *speech = lua_from_meta_object<Engine::UnitSpeech>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key"); 
        } 
        
        std::string field = key;
        if(field == "priority") {
            try {
                speech->priority = unit_speech_priority_from_string(luaL_checkstring(state, 3));
            }
            catch(std::runtime_error &e) {
                return luaL_error(state, e.what());
            }
        }
        else if(field == "scream_type") {
            try {
                speech->scream_type = unit_scream_type_from_string(luaL_checkstring(state, 3));
            }
            catch(std::runtime_error &e) {
                return luaL_error(state, e.what());
            }
        }
        else if(field == "sound_tag") {
            speech->sound_tag.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "ticks") {
            speech->ticks = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "unknown2") {
            speech->unknown2 = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "unknown") {
            speech->unknown = static_cast<std::int32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "ai_communication_info") {
            return luaL_error(state, "Invalid operation");
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_unit_speech(lua_State *state, Engine::UnitSpeech &speech) noexcept {
        lua_push_meta_object(state, speech, lua_engine_unit_speech__index, lua_engine_unit_speech__newindex); 
    }

    static int lua_engine_unit_speech_data__index(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<Engine::UnitSpeechData>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "current") {
            lua_push_meta_engine_unit_speech(state, data->current);
        }
        else if(field == "next") {
            lua_push_meta_engine_unit_speech(state, data->next);
        }
        else if(field == "unknown0") {
            lua_pushinteger(state, data->unknown0);
        }
        else if(field == "unknown1") {
            lua_pushinteger(state, data->unknown1);
        }
        else if(field == "unknown2") {
            lua_pushinteger(state, data->unknown2);
        }
        else if(field == "unknown3") {
            lua_pushinteger(state, data->unknown3);
        }
        else if(field == "unknown4") {
            lua_pushinteger(state, data->unknown4);
        }
        else if(field == "unknown6") {
            lua_pushboolean(state, data->unknown6);
        }
        else if(field == "unknown7") {
            lua_pushboolean(state, data->unknown7);
        }
        else if(field == "unknown8") {
            lua_pushboolean(state, data->unknown8);
        }
        else if(field == "unknown9") {
            lua_pushinteger(state, data->unknown9);
        }
        else if(field == "unknown10") {
            lua_pushinteger(state, data->unknown10);
        }
        else if(field == "unknown11") {
            lua_pushinteger(state, data->unknown11);
        }
        else if(field == "unknown12") {
            lua_pushinteger(state, data->unknown12);
        }
        else if(field == "unknown13") {
            lua_pushinteger(state, data->unknown13);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_unit_speech_data__newindex(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<Engine::UnitSpeechData>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key"); 
        } 
        
        std::string field = key;
        if(field == "current") {
            lua_push_meta_engine_unit_speech(state, data->current);
        }
        else if(field == "next") {
            lua_push_meta_engine_unit_speech(state, data->next);
        }
        else if(field == "unknown0") {
            data->unknown0 = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "unknown1") {
            data->unknown1 = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "unknown2") {
            data->unknown2 = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "unknown3") {
            data->unknown3 = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "unknown4") {
            data->unknown4 = static_cast<std::int32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "unknown6") {
            if(lua_isboolean(state, 3)) {
                data->unknown6 = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "unknown7") {
            if(lua_isboolean(state, 3)) {
                data->unknown7 = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "unknown8") {
            if(lua_isboolean(state, 3)) {
                data->unknown8 = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "unknown9") {
            data->unknown9 = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "unknown10") {
            data->unknown10 = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "unknown11") {
            data->unknown11 = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "unknown12") {
            data->unknown12 = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "unknown13") {
            data->unknown13 = static_cast<std::int32_t>(luaL_checkinteger(state, 3));
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_unit_speech_data(lua_State *state, Engine::UnitSpeechData &data) noexcept {
        lua_push_meta_object(state, data, lua_engine_unit_speech_data__index, lua_engine_unit_speech_data__newindex); 
    }

    static int lua_engine_unit_control_data__index(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<Engine::UnitControlData>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "animation_state") {
            lua_pushinteger(state, data->animation_state);
        }
        else if(field == "aiming_speed") {
            lua_pushinteger(state, data->aiming_speed);
        }
        else if(field == "control_flags") {
            lua_push_meta_engine_unit_control_flags(state, data->control_flags);
        }
        else if(field == "weapon_index") {
            lua_pushinteger(state, data->weapon_index);
        }
        else if(field == "grenade_index") {
            lua_pushinteger(state, data->grenade_index);
        }
        else if(field == "zoom_index") {
            lua_pushinteger(state, data->zoom_index);
        }
        else if(field == "throttle") {
            lua_push_meta_engine_vector3_d(state, data->throttle);
        }
        else if(field == "primary_trigger") {
            lua_pushnumber(state, data->primary_trigger);
        }
        else if(field == "facing_vector") {
            lua_push_meta_engine_vector3_d(state, data->facing_vector);
        }
        else if(field == "aiming_vector") {
            lua_push_meta_engine_vector3_d(state, data->aiming_vector);
        }
        else if(field == "looking_vector") {
            lua_push_meta_engine_vector3_d(state, data->looking_vector);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_unit_control_data__newindex(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<Engine::UnitControlData>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_isnumber(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "animation_state") {
            data->animation_state = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "aiming_speed") {
            data->aiming_speed = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "control_flags") {
            lua_push_meta_engine_unit_control_flags(state, data->control_flags);
        }
        else if(field == "weapon_index") {
            data->weapon_index = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "grenade_index") {
            data->grenade_index = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "zoom_index") {
            data->zoom_index = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "throttle") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "primary_trigger") {
            data->primary_trigger = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "facing_vector") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "aiming_vector") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "looking_vector") {
            return luaL_error(state, "Invalid operation");
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_unit_control_data(lua_State *state, Engine::UnitControlData &data) noexcept {
        lua_push_meta_object(state, data, lua_engine_unit_control_data__index, lua_engine_unit_control_data__newindex); 
    }

    extern std::string lua_engine_damage_effect_category_to_string(Engine::TagDefinitions::DamageEffectCategory value) noexcept; 
    extern Engine::TagDefinitions::DamageEffectCategory lua_engine_damage_effect_category_from_string(std::string str); 

    static int lua_engine_unit_object__index(lua_State *state) noexcept {
        auto *unit = lua_from_meta_object<Engine::UnitObject>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "actor") {
            lua_pushinteger(state, unit->actor.handle);
        }
        else if(field == "swarm_actor") {
            lua_pushinteger(state, unit->swarm.actor.handle);
        }
        else if(field == "swarm_next_unir") {
            lua_pushinteger(state, unit->swarm.next_unit.handle);
        }
        else if(field == "swarm_previous_unit") {
            lua_pushinteger(state, unit->swarm.previous_unit.handle);
        }
        else if(field == "unit_flags") {
            lua_push_meta_engine_unit_flags(state, unit->unit_flags);
        }
        else if(field == "unit_control_flags") {
            lua_push_meta_engine_unit_control_flags(state, unit->unit_control_flags);
        }
        else if(field == "shield_snapping") {
            lua_pushinteger(state, unit->shield_snapping);
        }
        else if(field == "base_seat_index") {
            lua_pushinteger(state, unit->base_seat_index);
        }
        else if(field == "persistent_control_ticks_remaining") {
            lua_pushinteger(state, unit->persistent_control.ticks_remaining);
        }
        else if(field == "persistent_control_control_flags") {
            lua_push_meta_engine_unit_control_flags(state, unit->persistent_control.control_flags);
        }
        else if(field == "controlling_player") {
            lua_pushinteger(state, unit->controlling_player.handle);
        }
        else if(field == "ai_effect_type") {
            lua_pushinteger(state, unit->ai_effect_type);
        }
        else if(field == "emotion_animation_index") {
            lua_pushinteger(state, unit->emotion_animation_index);
        }
        else if(field == "next_ai_effect_tick") {
            lua_pushinteger(state, unit->next_ai_effect_tick);
        }
        else if(field == "desired_facing_vector") {
            lua_push_meta_engine_vector3_d(state, unit->desired_facing_vector);
        }
        else if(field == "desired_aiming_vector") {
            lua_push_meta_engine_vector3_d(state, unit->desired_aiming_vector);
        }
        else if(field == "aiming_vector") {
            lua_push_meta_engine_vector3_d(state, unit->aiming_vector);
        }
        else if(field == "aiming_velocity") {
            lua_push_meta_engine_vector3_d(state, unit->aiming_velocity);
        }
        else if(field == "looking_angles") {
            lua_push_meta_engine_euler3_d(state, unit->looking_angles);
        }
        else if(field == "looking_vector") {
            lua_push_meta_engine_vector3_d(state, unit->looking_vector);
        }
        else if(field == "looking_velocity") {
            lua_push_meta_engine_vector3_d(state, unit->looking_velocity);
        }
        else if(field == "throttle") {
            lua_push_meta_engine_vector3_d(state, unit->throttle);
        }
        else if(field == "primary_trigger") {
            lua_pushnumber(state, unit->primary_trigger);
        }
        else if(field == "aiming_speed") {
            lua_pushinteger(state, unit->aiming_speed);
        }
        else if(field == "melee_state") {
            lua_pushinteger(state, unit->melee_state);
        }
        else if(field == "melee_timer") {
            lua_pushinteger(state, unit->melee_timer);
        }
        else if(field == "ticks_until_flame_to_death") {
            lua_pushinteger(state, unit->ticks_until_flame_to_death);
        }
        else if(field == "ping_animation_ticks_left") {
            lua_pushinteger(state, unit->ping_animation_ticks_left);
        }
        else if(field == "grenade_state") {
            lua_pushstring(state, unit_throwing_grenade_state_to_string(unit->grenade_state).c_str());
        }
        else if(field == "unknown_725") {
            lua_pushinteger(state, unit->unknown_725);
        }
        else if(field == "unknown_726") {
            lua_pushinteger(state, unit->unknown_726);
        }
        else if(field == "grenade_projectile") {
            lua_pushinteger(state, unit->grenade_projectile.handle);
        }
        else if(field == "animation") {
            lua_push_meta_engine_unit_animation_data(state, unit->animation);
        }
        else if(field == "ambient") {
            lua_pushnumber(state, unit->ambient);
        }
        else if(field == "illumination") {
            lua_pushnumber(state, unit->illumination);
        }
        else if(field == "mouth_factor") {
            lua_pushnumber(state, unit->mouth_factor);
        }
        else if(field == "vehicle_seat_id") {
            lua_pushinteger(state, unit->vehicle_seat_id);
        }
        else if(field == "current_weapon_id") {
            lua_pushinteger(state, unit->current_weapon_id);
        }
        else if(field == "next_weapon_id") {
            lua_pushinteger(state, unit->next_weapon_id);
        }
        else if(field == "weapons") {
            lua_newtable(state);
            for(std::size_t i = 0; i < 4; ++i) {
                lua_pushinteger(state, unit->weapons[i].handle);
                lua_rawseti(state, -2, i + 1);
            }
        }
        else if(field == "weapon_ready_ticks") {
            lua_newtable(state);
            for(std::size_t i = 0; i < 4; ++i) {
                lua_pushinteger(state, unit->weapon_ready_ticks[i]);
                lua_rawseti(state, -2, i + 1);
            }
        }
        else if(field == "equipment_handle") {
            lua_pushinteger(state, unit->equipment_handle.handle);
        }
        else if(field == "current_grenade_index") {
            lua_pushinteger(state, unit->current_grenade_index);
        }
        else if(field == "next_grenade_index") {
            lua_pushinteger(state, unit->next_grenade_index);
        }
        else if(field == "grenade_counts") {
            lua_newtable(state);
            for(std::size_t i = 0; i < 2; ++i) {
                lua_pushinteger(state, unit->grenade_counts[i]);
                lua_rawseti(state, -2, i + 1);
            }
        }
        else if(field == "zoom_level") {
            lua_pushinteger(state, unit->zoom_level);
        }
        else if(field == "desired_zoom_level") {
            lua_pushinteger(state, unit->desired_zoom_level);
        }
        else if(field == "ticks_since_last_vehicle_speech") {
            lua_pushinteger(state, unit->ticks_since_last_vehicle_speech);
        }
        else if(field == "aiming_change") {
            lua_pushinteger(state, unit->aiming_change);
        }
        else if(field == "powered_seats_riders") {
            lua_newtable(state);
            for(std::size_t i = 0; i < 2; ++i) {
                lua_pushinteger(state, unit->powered_seats_riders[i].handle);
                lua_rawseti(state, -2, i + 1);
            }
        }
        else if(field == "_unknown22") {
            lua_pushinteger(state, unit->_unknown22.handle);
        }
        else if(field == "_some_tick_time") {
            lua_pushinteger(state, unit->_some_tick_time);
        }
        else if(field == "encounter_id") {
            lua_pushinteger(state, unit->encounter_id);
        }
        else if(field == "squad_id") {
            lua_pushinteger(state, unit->squad_id);
        }
        else if(field == "powered_seats_power") {
            lua_newtable(state);
            for(std::size_t i = 0; i < 2; ++i) {
                lua_pushnumber(state, unit->powered_seats_power[i]);
                lua_rawseti(state, -2, i + 1);
            }
        }
        else if(field == "integrated_light_power") {
            lua_pushnumber(state, unit->integrated_light_power);
        }
        else if(field == "integrated_light_toggle_power") {
            lua_pushnumber(state, unit->integrated_light_toggle_power);
        }
        else if(field == "integrated_night_vision_toggle_power") {
            lua_pushnumber(state, unit->integrated_night_vision_toggle_power);
        }
        else if(field == "seat_related") {
            lua_newtable(state);
            for(std::size_t i = 0; i < 4; ++i) {
                lua_push_meta_engine_vector3_d(state, unit->seat_related[i]);
                lua_rawseti(state, -2, i + 1);
            }
        }
        else if(field == "camo_power") {
            lua_pushnumber(state, unit->camo_power);
        }
        else if(field == "full_spectrum_vision_power") {
            lua_pushnumber(state, unit->full_spectrum_vision_power);
        }
        else if(field == "dialogue_definition") {
            lua_pushinteger(state, unit->dialogue_definition.handle);
        }
        else if(field == "speech") {
            lua_push_meta_engine_unit_speech_data(state, unit->speech);
        }
        else if(field == "damage_result_category") {
            lua_pushstring(state, lua_engine_damage_effect_category_to_string(unit->damage_result.category).c_str());
        }
        else if(field == "damage_result_ai_ticks_until_handle") {
            lua_pushinteger(state, unit->damage_result.ai_ticks_until_handle);
        }
        else if(field == "damage_result_amount") {
            lua_pushnumber(state, unit->damage_result.amount);
        }
        else if(field == "damage_result_responsible_unit") {
            lua_pushinteger(state, unit->damage_result.responsible_unit.handle);
        }
        else if(field == "object_flame_causer") {
            lua_pushinteger(state, unit->object_flame_causer.handle);
        }
        else if(field == "_unknown23") {
            lua_pushnumber(state, unit->_unknown23);
        }
        else if(field == "died_at_tick") {
            lua_pushinteger(state, unit->died_at_tick);
        }
        else if(field == "feign_death_timer") {
            lua_pushinteger(state, unit->feign_death_timer);
        }
        else if(field == "camo_regrowth") {
            lua_pushboolean(state, unit->camo_regrowth);
        }
        else if(field == "stun") {
            lua_pushnumber(state, unit->stun);
        }
        else if(field == "stun_ticks") {
            lua_pushinteger(state, unit->stun_ticks);
        }
        else if(field == "spree_count") {
            lua_pushinteger(state, unit->spree_count);
        }
        else if(field == "spree_starting_time") {
            lua_pushinteger(state, unit->spree_starting_time);
        }
        else if(field == "recent_damage") {
            lua_newtable(state);
            for(std::size_t i = 0; i < 4; ++i) {
                lua_push_meta_engine_unit_recent_damager(state, unit->recent_damage[i]);
                lua_rawseti(state, -2, i + 1);
            }
        }
        else if(field == "opensauce_zoom_level") {
            lua_pushinteger(state, unit->opensauce_zoom_level);
        }
        else if(field == "opensauce_desired_zoom_level") {
            lua_pushinteger(state, unit->opensauce_desired_zoom_level);
        }
        else if(field == "control_data") {
            lua_push_meta_engine_unit_control_data(state, unit->control_data);
        }
        else if(field == "last_completed_client_update_valid") {
            lua_pushboolean(state, unit->last_completed_client_update_valid);
        }
        else if(field == "last_completed_client_update_id") {
            lua_pushinteger(state, unit->last_completed_client_update_id);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_unit_object__newindex(lua_State *state) noexcept {
        auto *unit = lua_from_meta_object<Engine::UnitObject>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "actor") {
            unit->actor.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "swarm_actor") {
            unit->swarm.actor.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "swarm_next_unir") {
            unit->swarm.next_unit.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "swarm_previous_unit") {
            unit->swarm.previous_unit.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "unit_flags") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "unit_control_flags") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "shield_snapping") {
            unit->shield_snapping = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "base_seat_index") {
            unit->base_seat_index = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "persistent_control_ticks_remaining") {
            unit->persistent_control.ticks_remaining = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "persistent_control_control_flags") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "controlling_player") {
            unit->controlling_player.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "ai_effect_type") {
            unit->ai_effect_type = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "emotion_animation_index") {
            unit->emotion_animation_index = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "next_ai_effect_tick") {
            unit->next_ai_effect_tick = static_cast<std::uint32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "desired_facing_vector") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "desired_aiming_vector") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "aiming_vector") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "aiming_velocity") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "looking_angles") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "looking_vector") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "looking_velocity") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "throttle") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "primary_trigger") {
            unit->primary_trigger = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "aiming_speed") {
            unit->aiming_speed = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "melee_state") {
            unit->melee_state = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "melee_timer") {
            unit->melee_timer = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "ticks_until_flame_to_death") {
            unit->ticks_until_flame_to_death = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "ping_animation_ticks_left") {
            unit->ping_animation_ticks_left = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "grenade_state") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "unknown_725") {
            unit->unknown_725 = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "unknown_726") {
            unit->unknown_726 = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "grenade_projectile") {
            unit->grenade_projectile.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "animation") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "ambient") {
            unit->ambient = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "illumination") {
            unit->illumination = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "mouth_factor") {
            unit->mouth_factor = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "vehicle_seat_id") {
            unit->vehicle_seat_id = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "current_weapon_id") {
            unit->current_weapon_id = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "next_weapon_id") {
            unit->next_weapon_id = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "weapons") {
            for(std::size_t i = 0; i < sizeof(unit->weapons) / sizeof(unit->weapons[0]); i++) {
                lua_rawgeti(state, 3, i + 1);
                if(!lua_isnil(state, -1)) {
                    unit->weapons[i] = Engine::ObjectHandle(luaL_checkinteger(state, -1));
                }
                lua_pop(state, 1);
            }
        }
        else if(field == "weapon_ready_ticks") {
            for(std::size_t i = 0; i < sizeof(unit->weapon_ready_ticks) / sizeof(unit->weapon_ready_ticks[0]); i++) {
                lua_rawgeti(state, 3, i + 1);
                if(!lua_isnil(state, -1)) {
                    unit->weapon_ready_ticks[i] = static_cast<std::int16_t>(luaL_checkinteger(state, -1));
                }
                lua_pop(state, 1);
            }
        }
        else if(field == "equipment_handle") {
            unit->equipment_handle.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "current_grenade_index") {
            unit->current_grenade_index = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "next_grenade_index") {
            unit->next_grenade_index = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "grenade_counts") {
            for(std::size_t i = 0; i < sizeof(unit->grenade_counts) / sizeof(unit->grenade_counts[0]); i++) {
                lua_rawgeti(state, 3, i + 1);
                if(!lua_isnil(state, -1)) {
                    unit->grenade_counts[i] = static_cast<std::uint8_t>(luaL_checkinteger(state, -1));
                }
                lua_pop(state, 1);
            }
        }
        else if(field == "zoom_level") {
            unit->zoom_level = static_cast<std::uint8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "desired_zoom_level") {
            unit->desired_zoom_level = static_cast<std::uint8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "ticks_since_last_vehicle_speech") {
            unit->ticks_since_last_vehicle_speech = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "aiming_change") {
            unit->aiming_change = static_cast<std::uint8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "powered_seats_riders") {
            for(std::size_t i = 0; i < sizeof(unit->powered_seats_riders) / sizeof(unit->powered_seats_riders[0]); i++) {
                lua_rawgeti(state, 3, i + 1);
                if(!lua_isnil(state, -1)) {
                    unit->powered_seats_riders[i] = Engine::ObjectHandle(luaL_checkinteger(state, -1));
                }
                lua_pop(state, 1);
            }
        }
        else if(field == "_unknown22") {
            unit->_unknown22.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "_some_tick_time") {
            unit->_some_tick_time = static_cast<std::int32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "encounter_id") {
            unit->encounter_id = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "squad_id") {
            unit->squad_id = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "powered_seats_power") {
            for(std::size_t i = 0; i < sizeof(unit->powered_seats_power) / sizeof(unit->powered_seats_power[0]); i++) {
                lua_rawgeti(state, 3, i + 1);
                if(!lua_isnil(state, -1)) {
                    unit->powered_seats_power[i] = static_cast<float>(luaL_checknumber(state, -1));
                }
                lua_pop(state, 1);
            }
        }
        else if(field == "integrated_light_power") {
            unit->integrated_light_power = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "integrated_light_toggle_power") {
            unit->integrated_light_toggle_power = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "integrated_night_vision_toggle_power") {
            unit->integrated_night_vision_toggle_power = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "seat_related") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "camo_power") {
            unit->camo_power = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "full_spectrum_vision_power") {
            unit->full_spectrum_vision_power = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "dialogue_definition") {
            unit->dialogue_definition.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "speech") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "damage_result_catagory") {
            try {
                unit->damage_result.category = lua_engine_damage_effect_category_from_string(luaL_checkstring(state, 3));
            }
            catch(const std::exception &e) {
                return luaL_error(state, e.what());
            }
        }
        else if(field == "damage_result_ai_ticks_until_handle") {
            unit->damage_result.ai_ticks_until_handle = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "damage_result_amount") {
            unit->damage_result.amount = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "damage_result_responsible_unit") {
            unit->damage_result.responsible_unit.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "object_flame_causer") {
            unit->object_flame_causer.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "_unknown23") {
            unit->_unknown23 = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "died_at_tick") {
            unit->died_at_tick = static_cast<std::int32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "feign_death_timer") {
            unit->feign_death_timer = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "camo_regrowth") {
            if(lua_isboolean(state, 3)) {
                unit->camo_regrowth = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "stun") {
            unit->stun = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "stun_ticks") {
            unit->stun_ticks = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "spree_count") {
            unit->spree_count = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "spree_starting_time") {
            unit->spree_starting_time = static_cast<std::int32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "recent_damage") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "opensauce_zoom_level") {
            unit->opensauce_zoom_level = static_cast<std::uint8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "opensauce_desired_zoom_level") {
            unit->opensauce_desired_zoom_level = static_cast<std::uint8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "control_data") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "last_completed_client_update_valid") {
            if(lua_isboolean(state, 3)) {
                unit->last_completed_client_update_valid = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "last_completed_client_update_id") {
            unit->last_completed_client_update_id = static_cast<std::int32_t>(luaL_checkinteger(state, 3));
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_unit_object(lua_State *state, Engine::UnitObject &object) noexcept {
        lua_push_meta_object(state, object, lua_engine_unit_object__index, lua_engine_unit_object__newindex);
    }

    static int lua_engine_biped_flags__index(lua_State *state) noexcept {
        auto *flags = lua_from_meta_object<Engine::BipedFlags>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "airborne") {
            lua_pushboolean(state, flags->airborne);
        }
        else if(field == "slipping") {
            lua_pushboolean(state, flags->slipping);
        }
        else if(field == "absolute_movement") {
            lua_pushboolean(state, flags->absolute_movement);
        }
        else if(field == "no_collision") {
            lua_pushboolean(state, flags->no_collision);
        }
        else if(field == "passes_through_other_bipeds") {
            lua_pushboolean(state, flags->passes_through_other_bipeds);
        }
        else if(field == "limping2") {
            lua_pushboolean(state, flags->limping2);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_biped_flags__newindex(lua_State *state) noexcept {
        auto *flags = lua_from_meta_object<Engine::BipedFlags>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_isboolean(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        }

        std::string field = key;
        if(field == "airborne") {
            flags->airborne = lua_toboolean(state, 3);
        }
        else if(field == "slipping") {
            flags->slipping = lua_toboolean(state, 3);
        }
        else if(field == "absolute_movement") {
            flags->absolute_movement = lua_toboolean(state, 3);
        }
        else if(field == "no_collision") {
            flags->no_collision = lua_toboolean(state, 3);
        }
        else if(field == "passes_through_other_bipeds") {
            flags->passes_through_other_bipeds = lua_toboolean(state, 3);
        }
        else if(field == "limping2") {
            flags->limping2 = lua_toboolean(state, 3);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_biped_flags(lua_State *state, Engine::BipedFlags &flags) noexcept {
        lua_push_meta_object(state, flags, lua_engine_biped_flags__index, lua_engine_biped_flags__newindex);
    }

    static int lua_engine_biped_network_delta__index(lua_State *state) noexcept {
        auto *delta = lua_from_meta_object<Engine::BipedNetworkDelta>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "grenade_counts") {
            lua_newtable(state);
            for(std::size_t i = 0; i < 2; ++i) {
                lua_pushinteger(state, delta->grenade_counts[i]);
                lua_rawseti(state, -2, i + 1);
            }
        }
        else if(field == "body_vitality") {
            lua_pushnumber(state, delta->body_vitality);
        }
        else if(field == "shield_vitality") {
            lua_pushnumber(state, delta->shield_vitality);
        }
        else if(field == "shield_stun_ticks_greater_than_zero") {
            lua_pushboolean(state, delta->shield_stun_ticks_greater_than_zero);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_biped_network_delta__newindex(lua_State *state) noexcept {
        auto *delta = lua_from_meta_object<Engine::BipedNetworkDelta>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "grenade_counts") {
            for(std::size_t i = 0; i < sizeof(delta->grenade_counts) / sizeof(delta->grenade_counts[0]); i++) {
                lua_rawgeti(state, 3, i + 1);
                if(!lua_isnil(state, -1)) {
                    delta->grenade_counts[i] = static_cast<std::int8_t>(luaL_checkinteger(state, -1));
                }
                lua_pop(state, 1);
            }
        }
        else if(field == "body_vitality") {
            delta->body_vitality = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "shield_vitality") {
            delta->shield_vitality = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "shield_stun_ticks_greater_than_zero") {
            if(lua_isboolean(state, 3)) {
                delta->shield_stun_ticks_greater_than_zero = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_biped_network_delta(lua_State *state, Engine::BipedNetworkDelta &delta) noexcept {
        lua_push_meta_object(state, delta, lua_engine_biped_network_delta__index, lua_engine_biped_network_delta__newindex);
    }

    static int lua_engine_biped_network__index(lua_State *state) noexcept {
        auto *network = lua_from_meta_object<Engine::BipedNetwork>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "baseline_valid") {
            lua_pushboolean(state, network->baseline_valid);
        }
        else if(field == "baseline_id") {
            lua_pushinteger(state, network->baseline_id);
        }
        else if(field == "message_id") {
            lua_pushinteger(state, network->message_id);
        }
        else if(field == "update_baseline") {
            lua_push_meta_engine_biped_network_delta(state, network->update_baseline);
        }
        else if(field == "delta_valid") {
            lua_pushboolean(state, network->delta_valid);
        }
        else if(field == "update_delta") {
            lua_push_meta_engine_biped_network_delta(state, network->update_delta);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_biped_network__newindex(lua_State *state) noexcept {
        auto *network = lua_from_meta_object<Engine::BipedNetwork>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_istable(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "baseline_valid") {
            if(lua_isboolean(state, 3)) {
                network->baseline_valid = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "baseline_id") {
            network->baseline_id = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "message_id") {
            network->message_id = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "update_baseline") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "delta_valid") {
            if(lua_isboolean(state, 3)) {
                network->delta_valid = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "update_delta") {
            return luaL_error(state, "Invalid operation");
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_biped_network(lua_State *state, Engine::BipedNetwork &network) noexcept {
        lua_push_meta_object(state, network, lua_engine_biped_network__index, lua_engine_biped_network__newindex);
    }

    static int lua_engine_biped_object__index(lua_State *state) noexcept {
        auto *biped = lua_from_meta_object<Engine::BipedObject>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "biped_flags") {
            lua_push_meta_engine_biped_flags(state, biped->biped_flags);
        }
        else if(field == "landing_timer") {
            lua_pushinteger(state, biped->landing_timer);
        }
        else if(field == "landing_force") {
            lua_pushinteger(state, biped->landing_force);
        }
        else if(field == "movement_state") {
            lua_pushstring(state, biped_movement_state_to_string(biped->movement_state).c_str());
        }
        else if(field == "_biped_unknown3") {
            lua_pushinteger(state, biped->_biped_unknown3);
        }
        else if(field == "action_flags") {
            lua_pushinteger(state, biped->action_flags);
        }
        else if(field == "_biped_unknown4") {
            lua_pushinteger(state, biped->_biped_unknown4);
        }
        else if(field == "biped_position") {
            lua_push_meta_engine_vector3_d(state, biped->biped_position);
        }
        else if(field == "walking_counter") {
            lua_pushinteger(state, biped->walking_counter);
        }
        else if(field == "bump_object") {
            lua_pushinteger(state, biped->bump_object.handle);
        }
        else if(field == "ticks_since_last_bump") {
            lua_pushinteger(state, biped->ticks_since_last_bump);
        }
        else if(field == "airborne_ticks") {
            lua_pushinteger(state, biped->airborne_ticks);
        }
        else if(field == "slipping_ticks") {
            lua_pushinteger(state, biped->slipping_ticks);
        }
        else if(field == "digital_throttle") {
            lua_pushinteger(state, biped->digital_throttle);
        }
        else if(field == "jump_ticks") {
            lua_pushinteger(state, biped->jump_ticks);
        }
        else if(field == "melee_ticks") {
            lua_pushinteger(state, biped->melee_ticks);
        }
        else if(field == "melee_inflict_ticks") {
            lua_pushinteger(state, biped->melee_inflict_ticks);
        }
        else if(field == "unknown_biped2") {
            lua_pushinteger(state, biped->unknown_biped2);
        }
        else if(field == "crouch_scale") {
            lua_pushnumber(state, biped->crouch_scale);
        }
        else if(field == "unknown_biped1") {
            lua_pushnumber(state, biped->unknown_biped1);
        }
        else if(field == "unknown_biped_physics_related") {
            lua_push_meta_engine_plane3_d(state, biped->unknown_biped_physics_related);
        }
        else if(field == "network") {
            lua_push_meta_engine_biped_network(state, biped->network);
        }
        else {
            return lua_engine_unit_object__index(state);
        }
        return 1;
    }

    static int lua_engine_biped_object__newindex(lua_State *state) noexcept {
        auto *biped = lua_from_meta_object<Engine::BipedObject>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "biped_flags") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "landing_timer") {
            biped->landing_timer = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "landing_force") {
            biped->landing_force = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "movement_state") {
            try {
                biped->movement_state = biped_movement_state_from_string(luaL_checkstring(state, 3));
            }
            catch(const std::exception &e) {
                return luaL_error(state, e.what());
            }
        }
        else if(field == "_biped_unknown3") {
            biped->_biped_unknown3 = static_cast<std::int32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "action_flags") {
            biped->action_flags = static_cast<std::uint32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "_biped_unknown4") {
            biped->_biped_unknown4 = static_cast<std::int32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "biped_position") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "walking_counter") {
            biped->walking_counter = static_cast<std::int32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "bump_object") {
            biped->bump_object.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "ticks_since_last_bump") {
            biped->ticks_since_last_bump = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "airborne_ticks") {
            biped->airborne_ticks = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "slipping_ticks") {
            biped->slipping_ticks = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "digital_throttle") {
            biped->digital_throttle = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "jump_ticks") {
            biped->jump_ticks = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "melee_ticks") {
            biped->melee_ticks = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "melee_inflict_ticks") {
            biped->melee_inflict_ticks = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "unknown_biped2") {
            biped->unknown_biped2 = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "crouch_scale") {
            biped->crouch_scale = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "unknown_biped1") {
            biped->unknown_biped1 = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "unknown_biped_physics_related") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "network") {
            return luaL_error(state, "Invalid operation");
        }
        else {
            return lua_engine_unit_object__newindex(state);
        }
        return 0;
    }

    void lua_push_meta_engine_biped_object(lua_State *state, Engine::BipedObject &object) noexcept {
        lua_push_meta_object(state, object, lua_engine_biped_object__index, lua_engine_biped_object__newindex);
    }

    static int lua_engine_vehicle_flags__index(lua_State *state) noexcept {
        auto *flags = lua_from_meta_object<Engine::VehicleFlags>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "vehicle_unknown0") {
            lua_pushboolean(state, flags->vehicle_unknown0);
        }
        else if(field == "hovering") {
            lua_pushboolean(state, flags->hovering);
        }
        else if(field == "crouched") {
            lua_pushboolean(state, flags->crouched);
        }
        else if(field == "jumping") {
            lua_pushboolean(state, flags->jumping);
        }
        else if(field == "unknown_vehicle1") {
            lua_pushboolean(state, flags->unknown_vehicle1);
        }
        else if(field == "unknown_vehicle2") {
            lua_pushinteger(state, flags->unknown_vehicle2);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_vehicle_flags__newindex(lua_State *state) noexcept {
        auto *flags = lua_from_meta_object<Engine::VehicleFlags>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_isboolean(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        }

        std::string field = key;
        if(field == "vehicle_unknown0") {
            flags->vehicle_unknown0 = lua_toboolean(state, 3);
        }
        else if(field == "hovering") {
            flags->hovering = lua_toboolean(state, 3);
        }
        else if(field == "crouched") {
            flags->crouched = lua_toboolean(state, 3);
        }
        else if(field == "jumping") {
            flags->jumping = lua_toboolean(state, 3);
        }
        else if(field == "unknown_vehicle1") {
            flags->unknown_vehicle1 = lua_toboolean(state, 3);
        }
        else if(field == "unknown_vehicle2") {
            flags->unknown_vehicle2 = static_cast<std::uint8_t>(luaL_checkinteger(state, 3));
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_vehicle_flags(lua_State *state, Engine::VehicleFlags &flags) noexcept {
        lua_push_meta_object(state, flags, lua_engine_vehicle_flags__index, lua_engine_vehicle_flags__newindex);
    }

    static int lua_engine_vehicle_network_data__index(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<Engine::VehicleNetworkData>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "_unknown1") {
            lua_pushboolean(state, data->at_rest);
        }
        else if(field == "_unknown2") {
            lua_push_meta_engine_vector3_d(state, data->position);
        }
        else if(field == "_unknown3") {
            lua_push_meta_engine_vector3_d(state, data->transitional_velocity);
        }
        else if(field == "_unknown4") {
            lua_push_meta_engine_vector3_d(state, data->angular_velocity);
        }
        else if(field == "_unknown5") {
            lua_push_meta_engine_vector3_d(state, data->forward);
        }
        else if(field == "_unknown6") {
            lua_push_meta_engine_vector3_d(state, data->up);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_vehicle_network_data__newindex(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<Engine::VehicleNetworkData>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        }

        std::string field = key;
        if(field == "_unknown1") {
            if(lua_isboolean(state, 3)) {
                data->at_rest = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "_unknown2") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "_unknown3") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "_unknown4") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "_unknown5") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "_unknown6") {
            return luaL_error(state, "Invalid operation");
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_vehicle_network_data(lua_State *state, Engine::VehicleNetworkData &data) noexcept {
        lua_push_meta_object(state, data, lua_engine_vehicle_network_data__index, lua_engine_vehicle_network_data__newindex);
    }

    static int lua_engine_vehicle_network__index(lua_State *state) noexcept {
        auto *network = lua_from_meta_object<Engine::VehicleNetwork>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        }

        std::string field = key;
        if(field == "time_valid") {
            lua_pushboolean(state, network->time_valid);
        }
        else if(field == "baseline_valid") {
            lua_pushboolean(state, network->baseline_valid);
        }
        else if(field == "baseline_id") {
            lua_pushinteger(state, network->baseline_id);
        }
        else if(field == "message_id") {
            lua_pushinteger(state, network->message_id);
        }
        else if(field == "update_baseline") {
            lua_push_meta_engine_vehicle_network_data(state, network->update_baseline);
        }
        else if(field == "delta_valid") {
            lua_pushboolean(state, network->delta_valid);
        }
        else if(field == "update_delta") {
            lua_push_meta_engine_vehicle_network_data(state, network->update_delta);
        }
        else if(field == "last_moved_at_tick") {
            lua_pushinteger(state, network->last_moved_at_tick);
        }
        else if(field == "scenario_respawn_id") {
            lua_pushinteger(state, network->scenario_respawn_id);
        }
        else if(field == "respawn_position") {
            lua_push_meta_engine_vector3_d(state, network->respawn_position);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_vehicle_network__newindex(lua_State *state) noexcept {
        auto *network = lua_from_meta_object<Engine::VehicleNetwork>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_istable(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        }

        std::string field = key;
        if(field == "time_valid") {
            if(lua_isboolean(state, 3)) {
                network->time_valid = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "baseline_valid") {
            if(lua_isboolean(state, 3)) {
                network->baseline_valid = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "baseline_id") {
            network->baseline_id = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "message_id") {
            network->message_id = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "update_baseline") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "delta_valid") {
            if(lua_isboolean(state, 3)) {
                network->delta_valid = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "update_delta") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "last_moved_at_tick") {
            network->last_moved_at_tick = static_cast<std::uint32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "scenario_respawn_id") {
            network->scenario_respawn_id = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "respawn_position") {
            return luaL_error(state, "Invalid operation");
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_vehicle_network(lua_State *state, Engine::VehicleNetwork &network) noexcept {
        lua_push_meta_object(state, network, lua_engine_vehicle_network__index, lua_engine_vehicle_network__newindex);
    }

    static int lua_engine_vehicle_object__index(lua_State *state) noexcept {
        auto *vehicle = lua_from_meta_object<Engine::VehicleObject>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        }

        std::string field = key;
        if(field == "vehicle_flags") {
            lua_push_meta_engine_vehicle_flags(state, vehicle->vehicle_flags);
        }
        else if(field == "speed") {
            lua_pushnumber(state, vehicle->speed);
        }
        else if(field == "slide") {
            lua_pushnumber(state, vehicle->slide);
        }
        else if(field == "turn") {
            lua_pushnumber(state, vehicle->turn);
        }
        else if(field == "tire_position") {
            lua_pushnumber(state, vehicle->tire_position);
        }
        else if(field == "thread_position_left") {
            lua_pushnumber(state, vehicle->thread_position_left);
        }
        else if(field == "thread_position_right") {
            lua_pushnumber(state, vehicle->thread_position_right);
        }
        else if(field == "hover") {
            lua_pushnumber(state, vehicle->hover);
        }
        else if(field == "thrust") {
            lua_pushnumber(state, vehicle->thrust);
        }
        else if(field == "suspension_states") {
            lua_newtable(state);
            for(std::size_t i = 0; i < sizeof(vehicle->suspension_states) / sizeof(vehicle->suspension_states[0]); i++) {
                lua_pushinteger(state, vehicle->suspension_states[i]);
                lua_rawseti(state, -2, i + 1);
            }
        }
        else if(field == "hover_position") {
            lua_push_meta_engine_vector3_d(state, vehicle->hover_position);
        }
        else if(field == "unknown_vehicle3") {
            lua_push_meta_engine_vector3_d(state, vehicle->unknown_vehicle3);
        }
        else if(field == "unknown_vehicle4") {
            lua_push_meta_engine_vector3_d(state, vehicle->unknown_vehicle4);
        }
        else if(field == "unknown_vehicle5") {
            lua_pushinteger(state, vehicle->unknown_vehicle5);
        }
        else if(field == "network") {
            lua_push_meta_engine_vehicle_network(state, vehicle->network);
        }
        else {
            return lua_engine_unit_object__index(state);
        }
        return 1;
    }

    static int lua_engine_vehicle_object__newindex(lua_State *state) noexcept {
        auto *vehicle = lua_from_meta_object<Engine::VehicleObject>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_istable(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        }

        std::string field = key;
        if(field == "vehicle_flags") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "speed") {
            vehicle->speed = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "slide") {
            vehicle->slide = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "turn") {
            vehicle->turn = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "tire_position") {
            vehicle->tire_position = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "thread_position_left") {
            vehicle->thread_position_left = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "thread_position_right") {
            vehicle->thread_position_right = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "hover") {
            vehicle->hover = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "thrust") {
            vehicle->thrust = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "suspension_states") {
            for(std::size_t i = 0; i < sizeof(vehicle->suspension_states) / sizeof(vehicle->suspension_states[0]); i++) {
                lua_rawgeti(state, 3, i + 1);
                if(!lua_isnil(state, -1)) {
                    vehicle->suspension_states[i] = static_cast<std::int8_t>(luaL_checkinteger(state, -1));
                }
                lua_pop(state, 1);
            }
        }
        else if(field == "hover_position") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "unknown_vehicle3") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "unknown_vehicle4") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "unknown_vehicle5") {
            vehicle->unknown_vehicle5 = static_cast<std::int32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "network") {
            return luaL_error(state, "Invalid operation");
        }
        else {
            return lua_engine_unit_object__newindex(state);
        }
        return 0;
    }

    void lua_push_meta_engine_vehicle_object(lua_State *state, Engine::VehicleObject &object) noexcept {
        lua_push_meta_object(state, object, lua_engine_vehicle_object__index, lua_engine_vehicle_object__newindex);
    }

    static int lua_engine_item_object__index(lua_State *state) noexcept {
        auto *item = lua_from_meta_object<Engine::ItemObject>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        }

        std::string field = key;
        if(field == "flags") {
            lua_pushinteger(state, item->flags);
        }
        else if(field == "ticks_until_detonation") {
            lua_pushinteger(state, item->ticks_until_detonation);
        }
        else if(field == "bsp_collision_surface_id") {
            lua_pushinteger(state, item->bsp_collision.surface_id);
        }
        else if(field == "bsp_collision_reference_id") {
            lua_pushinteger(state, item->bsp_collision.reference_id);
        }
        else if(field == "dropped_by_unit") {
            lua_pushinteger(state, item->dropped_by_unit.handle);
        }
        else if(field == "last_update_tick") {
            lua_pushinteger(state, item->last_update_tick);
        }
        else if(field == "object_collision_object") {
            lua_pushinteger(state, item->object_collision.object.handle);
        }
        else if(field == "object_collision_object_position") {
            lua_push_meta_engine_vector3_d(state, item->object_collision.object_position);
        }
        else if(field == "unknown_collision_position") {
            lua_push_meta_engine_vector3_d(state, item->unknown_collision_position);
        }
        else if(field == "unknown_collision_angle") {
            lua_push_meta_engine_euler2_d(state, item->unknown_collision_angle);
        }
        else {
            return lua_engine_unit_object__index(state);
        }
        return 1;
    }

    static int lua_engine_item_object__newindex(lua_State *state) noexcept {
        auto *item = lua_from_meta_object<Engine::ItemObject>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_istable(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        }

        std::string field = key;
        if(field == "flags") {
            item->flags = static_cast<std::uint32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "ticks_until_detonation") {
            item->ticks_until_detonation = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "bsp_collision_surface_id") {
            item->bsp_collision.surface_id = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "bsp_collision_reference_id") {
            item->bsp_collision.reference_id = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "dropped_by_unit") {
            item->dropped_by_unit.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "last_update_tick") {
            item->last_update_tick = static_cast<std::int32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "object_collision_object") {
            item->object_collision.object.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "object_collision_object_position") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "unknown_collision_position") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "unknown_collision_angle") {
            return luaL_error(state, "Invalid operation");
        }
        else {
            return lua_engine_unit_object__newindex(state);
        }
        return 0;
    }

    void lua_push_meta_engine_item_object(lua_State *state, Engine::ItemObject &object) noexcept {
        lua_push_meta_object(state, object, lua_engine_item_object__index, lua_engine_item_object__newindex);
    }

    static int lua_engine_garbage_object__index(lua_State *state) noexcept {
        auto *garbage = lua_from_meta_object<Engine::GarbageObject>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        }

        std::string field = key;
        if(field == "ticks_until_garbage_collection") {
            lua_pushinteger(state, garbage->ticks_until_garbage_collection);
        }
        else {
            return lua_engine_item_object__index(state);
        }
        return 1;
    }

    static int lua_engine_garbage_object__newindex(lua_State *state) noexcept {
        auto *garbage = lua_from_meta_object<Engine::GarbageObject>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_istable(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        }

        std::string field = key;
        if(field == "ticks_until_garbage_collection") {
            garbage->ticks_until_garbage_collection = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else {
            return lua_engine_item_object__newindex(state);
        }
        return 0;
    }

    void lua_push_meta_engine_garbage_object(lua_State *state, Engine::GarbageObject &object) noexcept {
        lua_push_meta_object(state, object, lua_engine_garbage_object__index, lua_engine_garbage_object__newindex);
    }

    static int lua_engine_weapon_trigger__index(lua_State *state) noexcept {
        auto *trigger = lua_from_meta_object<Engine::WeaponTrigger>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        }

        std::string field = key;
        if(field == "idle_time") {
            lua_pushinteger(state, trigger->idle_time);
        }
        else if(field == "state") {
            lua_pushstring(state, weapon_state_to_string(trigger->state).c_str());
        }
        else if(field == "trigger_time") {
            lua_pushinteger(state, trigger->trigger_time);
        }
        else if(field == "not_firing") {
            lua_pushinteger(state, trigger->not_firing);
        }
        else if(field == "auto_reload") {
            lua_pushinteger(state, trigger->auto_reload);
        }
        else if(field == "rounds_since_last_tracer") {
            lua_pushinteger(state, trigger->rounds_since_last_tracer);
        }
        else if(field == "rate_of_fire") {
            lua_pushnumber(state, trigger->rate_of_fire);
        }
        else if(field == "ejection_port_recovery_time") {
            lua_pushnumber(state, trigger->ejection_port_recovery_time);
        }
        else if(field == "illumination_recovery_time") {
            lua_pushnumber(state, trigger->illumination_recovery_time);
        }
        else if(field == "projectile_error_related") {
            lua_pushnumber(state, trigger->projectile_error_related);
        }
        else if(field == "charing_effect") {
            lua_pushinteger(state, trigger->charing_effect.handle);
        }
        else if(field == "network_delay_ticks") {
            lua_pushinteger(state, trigger->network_delay_ticks);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_weapon_trigger__newindex(lua_State *state) noexcept {
        auto *trigger = lua_from_meta_object<Engine::WeaponTrigger>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_istable(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        }

        std::string field = key;
        if(field == "idle_time") {
            trigger->idle_time = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "state") {
            trigger->state = weapon_state_from_string(luaL_checkstring(state, 3));
        }
        else if(field == "trigger_time") {
            trigger->trigger_time = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "not_firing") {
            trigger->not_firing = static_cast<std::uint32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "auto_reload") {
            trigger->auto_reload = static_cast<std::uint32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "rounds_since_last_tracer") {
            trigger->rounds_since_last_tracer = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "rate_of_fire") {
            trigger->rate_of_fire = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "ejection_port_recovery_time") {
            trigger->ejection_port_recovery_time = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "illumination_recovery_time") {
            trigger->illumination_recovery_time = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "projectile_error_related") {
            trigger->projectile_error_related = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "charing_effect") {
            trigger->charing_effect.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "network_delay_ticks") {
            trigger->network_delay_ticks = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_weapon_trigger(lua_State *state, Engine::WeaponTrigger &trigger) noexcept {
        lua_push_meta_object(state, trigger, lua_engine_weapon_trigger__index, lua_engine_weapon_trigger__newindex);
    }

    static int lua_engine_weapon_magazine__index(lua_State *state) noexcept {
        auto *magazine = lua_from_meta_object<Engine::WeaponMagazine>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        }

        std::string field = key;
        if(field == "state") {
            lua_pushstring(state, weapon_magazine_state_to_string(magazine->state).c_str());
        }
        else if(field == "reload_ticks_remaining") {
            lua_pushinteger(state, magazine->reload_ticks_remaining);
        }
        else if(field == "reload_ticks") {
            lua_pushinteger(state, magazine->reload_ticks);
        }
        else if(field == "rounds_unloaded") {
            lua_pushinteger(state, magazine->rounds_unloaded);
        }
        else if(field == "rounds_loaded") {
            lua_pushinteger(state, magazine->rounds_loaded);
        }
        else if(field == "rounds_left_to_recharge") {
            lua_pushinteger(state, magazine->rounds_left_to_recharge);
        }
        else if(field == "unknown") {
            lua_pushinteger(state, magazine->unknown);
        }
        else if(field == "unknown2") {
            lua_pushinteger(state, magazine->unknown2);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_weapon_magazine__newindex(lua_State *state) noexcept {
        auto *magazine = lua_from_meta_object<Engine::WeaponMagazine>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_istable(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        }

        std::string field = key;
        if(field == "state") {
            magazine->state = weapon_magazine_state_from_string(luaL_checkstring(state, 3));
        }
        else if(field == "reload_ticks_remaining") {
            magazine->reload_ticks_remaining = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "reload_ticks") {
            magazine->reload_ticks = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "rounds_unloaded") {
            magazine->rounds_unloaded = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "rounds_loaded") {
            magazine->rounds_loaded = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "rounds_left_to_recharge") {
            magazine->rounds_left_to_recharge = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "unknown") {
            magazine->unknown = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "unknown2") {
            magazine->unknown2 = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_weapon_magazine(lua_State *state, Engine::WeaponMagazine &magazine) noexcept {
        lua_push_meta_object(state, magazine, lua_engine_weapon_magazine__index, lua_engine_weapon_magazine__newindex);
    }

    static int lua_engine_weapon_reload_start_data__index(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<Engine::WeaponReloadStartData>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "total_rounds") {
            lua_newtable(state);
            for(std::size_t i = 0; i < sizeof(data->total_rounds) / sizeof(data->total_rounds[0]); i++) {
                lua_pushinteger(state, data->total_rounds[i]);
                lua_rawseti(state, -2, i + 1);
            }
        }
        else if(field == "loaded_rounds") {
            lua_newtable(state);
            for(std::size_t i = 0; i < sizeof(data->loaded_rounds) / sizeof(data->loaded_rounds[0]); i++) {
                lua_pushinteger(state, data->loaded_rounds[i]);
                lua_rawseti(state, -2, i + 1);
            }
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_weapon_reload_start_data__newindex(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<Engine::WeaponReloadStartData>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_istable(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "total_rounds") {
            for(std::size_t i = 0; i < sizeof(data->total_rounds) / sizeof(data->total_rounds[0]); i++) {
                lua_rawgeti(state, 3, i + 1);
                if(!lua_isnil(state, -1)) {
                    data->total_rounds[i] = static_cast<std::int16_t>(luaL_checkinteger(state, -1));
                }
                lua_pop(state, 1);
            }
        }
        else if(field == "loaded_rounds") {
            for(std::size_t i = 0; i < sizeof(data->loaded_rounds) / sizeof(data->loaded_rounds[0]); i++) {
                lua_rawgeti(state, 3, i + 1);
                if(!lua_isnil(state, -1)) {
                    data->loaded_rounds[i] = static_cast<std::int16_t>(luaL_checkinteger(state, -1));
                }
                lua_pop(state, 1);
            }
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_weapon_reload_start_data(lua_State *state, Engine::WeaponReloadStartData &data) noexcept {
        lua_push_meta_object(state, data, lua_engine_weapon_reload_start_data__index, lua_engine_weapon_reload_start_data__newindex);
    }

    static int lua_engine_weapon_network_data__index(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<Engine::WeaponNetworkData>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "position") {
            lua_push_meta_engine_vector3_d(state, data->position);
        } 
        else if(field == "transitional_velocity") {
            lua_push_meta_engine_vector3_d(state, data->transitional_velocity);
        }
        else if(field == "angular_velocity") {
            lua_push_meta_engine_vector3_d(state, data->angular_velocity);
        }
        else if(field == "magazine_rounds_total") {
            lua_newtable(state);
            for(std::size_t i = 0; i < sizeof(data->magazine_rounds_total) / sizeof(data->magazine_rounds_total[0]); i++) {
                lua_pushinteger(state, data->magazine_rounds_total[i]);
                lua_rawseti(state, -2, i + 1);
            }
        }
        else if(field == "age") {
            lua_pushnumber(state, data->age);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_weapon_network_data__newindex(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<Engine::WeaponNetworkData>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 

        std::string field = key;
        if(field == "position") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "transitional_velocity") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "angular_velocity") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "magazine_rounds_total") {
            for(std::size_t i = 0; i < sizeof(data->magazine_rounds_total) / sizeof(data->magazine_rounds_total[0]); i++) {
                lua_rawgeti(state, 3, i + 1);
                if(!lua_isnil(state, -1)) {
                    data->magazine_rounds_total[i] = static_cast<std::int16_t>(luaL_checkinteger(state, -1));
                }
                lua_pop(state, 1);
            }
        }
        else if(field == "age") {
            data->age = static_cast<float>(luaL_checknumber(state, 3));
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_weapon_network_data(lua_State *state, Engine::WeaponNetworkData &data) noexcept {
        lua_push_meta_object(state, data, lua_engine_weapon_network_data__index, lua_engine_weapon_network_data__newindex);
    }

    static int lua_engine_weapon_network__index(lua_State *state) noexcept {
        auto *network = lua_from_meta_object<Engine::WeaponNetwork>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "baseline_valid") {
            lua_pushboolean(state, network->baseline_valid);
        }
        else if(field == "baseline_index") {
            lua_pushinteger(state, network->baseline_index);
        }
        else if(field == "message_index") {
            lua_pushinteger(state, network->message_index);
        }
        else if(field == "update_baseline") {
            lua_push_meta_engine_weapon_network_data(state, network->update_baseline);
        }
        else if(field == "delta_valid") {
            lua_pushboolean(state, network->delta_valid);
        }
        else if(field == "update_delta") {
            lua_push_meta_engine_weapon_network_data(state, network->update_delta);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_weapon_network__newindex(lua_State *state) noexcept {
        auto *network = lua_from_meta_object<Engine::WeaponNetwork>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 

        std::string field = key;
        if(field == "baseline_valid") {
            if(lua_isboolean(state, 3)) {
                network->baseline_valid = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "baseline_index") {
            network->baseline_index = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "message_index") {
            network->message_index = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "update_baseline") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "delta_valid") {
            if(lua_isboolean(state, 3)) {
                network->delta_valid = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "update_delta") {
            return luaL_error(state, "Invalid operation");
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_weapon_network(lua_State *state, Engine::WeaponNetwork &network) noexcept {
        lua_push_meta_object(state, network, lua_engine_weapon_network__index, lua_engine_weapon_network__newindex);
    }

    static int lua_engine_weapon_object__index(lua_State *state) noexcept {
        auto *weapon = lua_from_meta_object<Engine::WeaponObject>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "flags") {
            lua_pushinteger(state, weapon->flags);
        }
        else if(field == "owner_unit_flags") {
            lua_pushinteger(state, weapon->owner_unit_flags);
        }
        else if(field == "primary_trigger") {
            lua_pushnumber(state, weapon->primary_trigger);
        }
        else if(field == "weapon_state") {
            lua_pushstring(state, weapon_state_to_string(weapon->weapon_state).c_str());
        }
        else if(field == "ready_ticks") {
            lua_pushinteger(state, weapon->ready_ticks);
        }
        else if(field == "heat") {
            lua_pushnumber(state, weapon->heat);
        }
        else if(field == "age") {
            lua_pushnumber(state, weapon->age);
        }
        else if(field == "illumination_fraction") {
            lua_pushnumber(state, weapon->illumination_fraction);
        }
        else if(field == "integrated_light_power") {
            lua_pushnumber(state, weapon->integrated_light_power);
        }
        else if(field == "tracked_object") {
            lua_pushinteger(state, weapon->tracked_object.handle);
        }
        else if(field == "alt_shots_loaded") {
            lua_pushinteger(state, weapon->alt_shots_loaded);
        }
        else if(field == "triggers") {
            lua_newtable(state);
            for(std::size_t i = 0; i < sizeof(weapon->triggers) / sizeof(weapon->triggers[0]); i++) {
                lua_push_meta_engine_weapon_trigger(state, weapon->triggers[i]);
                lua_rawseti(state, -2, i + 1);
            }
        }
        else if(field == "magazines") {
            lua_newtable(state);
            for(std::size_t i = 0; i < sizeof(weapon->magazines) / sizeof(weapon->magazines[0]); i++) {
                lua_push_meta_engine_weapon_magazine(state, weapon->magazines[i]);
                lua_rawseti(state, -2, i + 1);
            }
        }
        else if(field == "last_trigger_fire_tick") {
            lua_pushinteger(state, weapon->last_trigger_fire_tick);
        }
        else if(field == "reload_starting_point") {
            lua_push_meta_engine_weapon_reload_start_data(state, weapon->reload_starting_point);
        }
        else if(field == "network") {
            lua_push_meta_engine_weapon_network(state, weapon->network);
        }
        else {
            return lua_engine_item_object__index(state);
        }
        return 1;
    }

    static int lua_engine_weapon_object__newindex(lua_State *state) noexcept {
        auto *weapon = lua_from_meta_object<Engine::WeaponObject>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_istable(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "flags") {
            weapon->flags = static_cast<std::uint32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "owner_unit_flags") {
            weapon->owner_unit_flags = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "primary_trigger") {
            weapon->primary_trigger = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "weapon_state") {
            weapon->weapon_state = weapon_state_from_string(luaL_checkstring(state, 3));
        }
        else if(field == "ready_ticks") {
            weapon->ready_ticks = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "heat") {
            weapon->heat = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "age") {
            weapon->age = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "illumination_fraction") {
            weapon->illumination_fraction = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "integrated_light_power") {
            weapon->integrated_light_power = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "tracked_object") {
            weapon->tracked_object.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "alt_shots_loaded") {
            weapon->alt_shots_loaded = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "triggers") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "magazines") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "last_trigger_fire_tick") {
            weapon->last_trigger_fire_tick = static_cast<std::uint32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "reload_starting_point") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "network") {
            return luaL_error(state, "Invalid operation");
        }
        else {
            return lua_engine_item_object__newindex(state);
        }
        return 0;
    }

    void lua_push_meta_engine_weapon_object(lua_State *state, Engine::WeaponObject &object) noexcept {
        lua_push_meta_object(state, object, lua_engine_weapon_object__index, lua_engine_weapon_object__newindex);
    }

    static int lua_engine_equipment_network_data__index(lua_State *state) noexcept {
        auto *equipment_network_data = lua_from_meta_object<Engine::EquipmentNetworkData>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "position") {
            lua_push_meta_engine_vector3_d(state, equipment_network_data->position);
        }
        else if(field == "transitional_velocity") {
            lua_push_meta_engine_vector3_d(state, equipment_network_data->transitional_velocity);
        }
        else if(field == "angular_velocity") {
            lua_push_meta_engine_vector3_d(state, equipment_network_data->angular_velocity);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

     static int lua_engine_equipment_network_data__newindex(lua_State *state) noexcept {
        auto *equipment_network_data = lua_from_meta_object<Engine::EquipmentNetworkData>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "position") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "transitional_velocity") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "angular_velocity") {
            return luaL_error(state, "Invalid operation");
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_equipment_network_data(lua_State *state, Engine::EquipmentNetworkData &data) noexcept {
        lua_push_meta_object(state, data, lua_engine_equipment_network_data__index, lua_engine_equipment_network_data__newindex);
    }

    static int lua_engine_equipment_network__index(lua_State *state) noexcept {
        auto *equipment_network = lua_from_meta_object<Engine::EquipmentNetwork>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "baseline_valid") {
            lua_pushboolean(state, equipment_network->baseline_valid);
        }
        else if(field == "baseline_index") {
            lua_pushinteger(state, equipment_network->baseline_index);
        }
        else if(field == "message_index") {
            lua_pushinteger(state, equipment_network->message_index);
        }
        else if(field == "update_baseline") {
            lua_push_meta_engine_equipment_network_data(state, equipment_network->update_baseline);
        }
        else if(field == "delta_valid") {
            lua_pushboolean(state, equipment_network->delta_valid);
        }
        else if(field == "update_delta") {
            lua_push_meta_engine_equipment_network_data(state, equipment_network->update_delta);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_equipment_network__newindex(lua_State *state) noexcept {
        auto *equipment_network = lua_from_meta_object<Engine::EquipmentNetwork>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_istable(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "baseline_valid") {
            if(lua_isboolean(state, 3)) {
                equipment_network->baseline_valid = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "baseline_index") {
            equipment_network->baseline_index = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "message_index") {
            equipment_network->message_index = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "update_baseline") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "delta_valid") {
            if(lua_isboolean(state, 3)) {
                equipment_network->delta_valid = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "update_delta") {
            return luaL_error(state, "Invalid operation");
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_equipment_network(lua_State *state, Engine::EquipmentNetwork &network) noexcept {
        lua_push_meta_object(state, network, lua_engine_equipment_network__index, lua_engine_equipment_network__newindex);
    }

    static int lua_engine_equipment_object__index(lua_State *state) noexcept {
        auto *equipment = lua_from_meta_object<Engine::EquipmentObject>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return lua_engine_item_object__index(state);
        } 
        
        std::string field = key;
        if(field == "network") {
            lua_push_meta_engine_equipment_network(state, equipment->network);
        }
        else {
            return lua_engine_item_object__index(state);
        }
        return 1;
    }

    static int lua_engine_equipment_object__newindex(lua_State *state) noexcept {
        auto *equipment = lua_from_meta_object<Engine::EquipmentObject>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_istable(state, 3)) {  
            return lua_engine_item_object__newindex(state);
        } 
        
        std::string field = key;
        if(field == "network") {
            return luaL_error(state, "Invalid operation");
        }
        else {
            return lua_engine_item_object__newindex(state);
        }
        return 0;
    }

    void lua_push_meta_engine_equipment_object(lua_State *state, Engine::EquipmentObject &object) noexcept {
        lua_push_meta_object(state, object, lua_engine_equipment_object__index, lua_engine_equipment_object__newindex);
    }

    static int lua_engine_projectile_object_flags__index(lua_State *state) noexcept {
        auto *flags = lua_from_meta_object<Engine::ProjectileObjectFlags>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        }
        
        std::string field = key;
        if(field == "tracer") {
            lua_pushboolean(state, flags->tracer);
        }
        else if(field == "projectile_unknown_bit") {
            lua_pushboolean(state, flags->projectile_unknown_bit);
        }
        else if(field == "attached") {
            lua_pushboolean(state, flags->attached);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_projectile_object_flags__newindex(lua_State *state) noexcept {
        auto *flags = lua_from_meta_object<Engine::ProjectileObjectFlags>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_isboolean(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        }
        
        std::string field = key;
        if(field == "tracer") {
            flags->tracer = lua_toboolean(state, 3);
        }
        else if(field == "projectile_unknown_bit") {
            flags->projectile_unknown_bit = lua_toboolean(state, 3);
        }
        else if(field == "attached") {
            flags->attached = lua_toboolean(state, 3);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_projectile_object_flags(lua_State *state, Engine::ProjectileObjectFlags &flags) noexcept {
        lua_push_meta_object(state, flags, lua_engine_projectile_object_flags__index, lua_engine_projectile_object_flags__newindex);
    }

    static int lua_engine_projectile_network_data__index(lua_State *state) noexcept {
        auto *flags = lua_from_meta_object<Engine::ProjectileNetworkData>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        }
        
        std::string field = key;
        if(field == "position") {
            lua_push_meta_engine_vector3_d(state, flags->position);
        }
        else if(field == "transitional_velocity") {
            lua_push_meta_engine_vector3_d(state, flags->transitional_velocity);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_projectile_network_data__newindex(lua_State *state) noexcept {
        auto *flags = lua_from_meta_object<Engine::ProjectileNetworkData>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        }
        
        std::string field = key;
        if(field == "position") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "transitional_velocity") {
            return luaL_error(state, "Invalid operation");
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_projectile_network_data(lua_State *state, Engine::ProjectileNetworkData &data) noexcept {
        lua_push_meta_object(state, data, lua_engine_projectile_network_data__index, lua_engine_projectile_network_data__newindex);
    }

    static int lua_engine_projectile_network__index(lua_State *state) noexcept {
        auto *network = lua_from_meta_object<Engine::ProjectileNetwork>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        }
        
        std::string field = key;
        if(field == "unknown") {
            lua_pushboolean(state, network->unknown);
        }
        else if(field == "baseline_valid") {
            lua_pushboolean(state, network->baseline_valid);
        }
        else if(field == "baseline_index") {
            lua_pushinteger(state, network->baseline_index);
        }
        else if(field == "message_index") {
            lua_pushinteger(state, network->message_index);
        }
        else if(field == "update_baseline") {
            lua_push_meta_engine_projectile_network_data(state, network->update_baseline);
        }
        else if(field == "delta_valid") {
            lua_pushboolean(state, network->delta_valid);
        }
        else if(field == "update_delta") {
            lua_push_meta_engine_projectile_network_data(state, network->update_delta);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_projectile_network__newindex(lua_State *state) noexcept {
        auto *network = lua_from_meta_object<Engine::ProjectileNetwork>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_istable(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        }
        
        std::string field = key;
        if(field == "unknown") {
            if(lua_isboolean(state, 3)) {
                network->unknown = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "baseline_valid") {
            if(lua_isboolean(state, 3)) {
                network->baseline_valid = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "baseline_index") {
            network->baseline_index = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "message_index") {
            network->message_index = static_cast<std::int8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "update_baseline") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "delta_valid") {
            if(lua_isboolean(state, 3)) {
                network->delta_valid = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "update_delta") {
            return luaL_error(state, "Invalid operation");
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_projectile_network(lua_State *state, Engine::ProjectileNetwork &network) noexcept {
        lua_push_meta_object(state, network, lua_engine_projectile_network__index, lua_engine_projectile_network__newindex);
    }

    static int lua_engine_projectile_object__index(lua_State *state) noexcept {
        auto *projectile = lua_from_meta_object<Engine::ProjectileObject>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        
        if(key == nullptr) {  
            return lua_engine_item_object__index(state);
        }
        
        std::string field = key;
        if(field == "projectile_flags") {
            lua_push_meta_engine_projectile_object_flags(state, projectile->projectile_flags);
        }
        else if(field == "action_enum") {
            lua_pushinteger(state, projectile->action_enum);
        }
        else if(field == "material_id") {
            lua_pushinteger(state, projectile->material_id);
        }
        else if(field == "source_unit") {
            lua_pushinteger(state, projectile->source_unit.handle);
        }
        else if(field == "target_object") {
            lua_pushinteger(state, projectile->target_object.handle);
        }
        else if(field == "contrail_attachment_block_id") {
            lua_pushinteger(state, projectile->contrail_attachment_block_id);
        }
        else if(field == "time_remaining") {
            lua_pushnumber(state, projectile->time_remaining);
        }
        else if(field == "arming_rate") {
            lua_pushnumber(state, projectile->arming_rate);
        }
        else if(field == "unknown_proj_float1") {
            lua_pushnumber(state, projectile->unknown_proj_float1);
        }
        else if(field == "unknown_proj_float2") {
            lua_pushnumber(state, projectile->unknown_proj_float2);
        }
        else if(field == "distance_travelled") {
            lua_pushnumber(state, projectile->distance_travelled);
        }
        else if(field == "transitional_velocity") {
            lua_push_meta_engine_vector3_d(state, projectile->transitional_velocity);
        }
        else if(field == "water_damage_upper_bound") {
            lua_pushnumber(state, projectile->water_damage_upper_bound);
        }
        else if(field == "angular_velocity") {
            lua_push_meta_engine_vector3_d(state, projectile->angular_velocity);
        }
        else if(field == "unknown_euler") {
            lua_push_meta_engine_euler2_d(state, projectile->unknown_euler);
        }
        else if(field == "network") {
            lua_push_meta_engine_projectile_network(state, projectile->network);
        }
        else {
            return lua_engine_item_object__index(state);
        }
        return 1;
    }

    static int lua_engine_projectile_object__newindex(lua_State *state) noexcept {
        auto *projectile = lua_from_meta_object<Engine::ProjectileObject>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        
        if(key == nullptr || !lua_istable(state, 3)) {  
            return lua_engine_item_object__newindex(state);
        }
        
        std::string field = key;
        if(field == "projectile_flags") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "action_enum") {
            projectile->action_enum = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "material_id") {
            projectile->material_id = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "source_unit") {
            projectile->source_unit.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "target_object") {
            projectile->target_object.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "contrail_attachment_block_id") {
            projectile->contrail_attachment_block_id = static_cast<std::int32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "time_remaining") {
            projectile->time_remaining = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "arming_rate") {
            projectile->arming_rate = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "unknown_proj_float1") {
            projectile->unknown_proj_float1 = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "unknown_proj_float2") {
            projectile->unknown_proj_float2 = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "distance_travelled") {
            projectile->distance_travelled = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "transitional_velocity") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "water_damage_upper_bound") {
            projectile->water_damage_upper_bound = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "angular_velocity") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "unknown_euler") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "network") {
            return luaL_error(state, "Invalid operation");
        }
        else {
            return lua_engine_item_object__newindex(state);
        }
        return 0;
    }

    void lua_push_meta_engine_projectile_object(lua_State *state, Engine::ProjectileObject &object) noexcept {
        lua_push_meta_object(state, object, lua_engine_projectile_object__index, lua_engine_projectile_object__newindex);
    }

    static int lua_engine_device_object_state__index(lua_State *state) noexcept {
        auto *state_data = lua_from_meta_object<Engine::DeviceObjectState>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        }
        
        std::string field = key;
        if(field == "device_group_id") {
            lua_pushinteger(state, state_data->device_group_id);
        }
        else if(field == "value") {
            lua_pushnumber(state, state_data->value);
        }
        else if(field == "change") {
            lua_pushnumber(state, state_data->change);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_device_object_state__newindex(lua_State *state) noexcept {
        auto *state_data = lua_from_meta_object<Engine::DeviceObjectState>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_isnumber(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        }
        
        std::string field = key;
        if(field == "device_group_id") {
            state_data->device_group_id = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "value") {
            state_data->value = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "change") {
            state_data->change = static_cast<float>(luaL_checknumber(state, 3));
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_device_object_state(lua_State *state, Engine::DeviceObjectState &state_data) noexcept {
        lua_push_meta_object(state, state_data, lua_engine_device_object_state__index, lua_engine_device_object_state__newindex);
    }

    static int lua_engine_device_object__index(lua_State *state) noexcept {
        auto *device = lua_from_meta_object<Engine::DeviceObject>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        
        if(key == nullptr) {  
            return lua_engine_item_object__index(state);
        }
        
        std::string field = key;
        if(field == "position_reversed") {
            lua_pushboolean(state, device->position_reversed);
        }
        else if(field == "not_usable_from_any_side") {
            lua_pushboolean(state, device->not_usable_from_any_side);
        }
        else if(field == "power") {
            lua_push_meta_engine_device_object_state(state, device->power);
        }
        else if(field == "position") {
            lua_push_meta_engine_device_object_state(state, device->position);
        }
        else if(field == "one_sided") {
            lua_pushboolean(state, device->one_sided);
        }
        else if(field == "operates_automatically") {
            lua_pushboolean(state, device->operates_automatically);
        }
        else {
            return lua_engine_item_object__index(state);
        }
        return 1;        
    }

    static int lua_engine_device_object__newindex(lua_State *state) noexcept {
        auto *device = lua_from_meta_object<Engine::DeviceObject>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        
        if(key == nullptr || !lua_istable(state, 3)) {  
            return lua_engine_item_object__newindex(state);
        }
        
        std::string field = key;
        if(field == "position_reversed") {
            if(lua_isboolean(state, 3)) {
                device->position_reversed = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "not_usable_from_any_side") {
            if(lua_isboolean(state, 3)) {
                device->not_usable_from_any_side = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "power") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "position") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "one_sided") {
            if(lua_isboolean(state, 3)) {
                device->one_sided = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "operates_automatically") {
            if(lua_isboolean(state, 3)) {
                device->operates_automatically = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else {
            return lua_engine_item_object__newindex(state);
        }
        return 0;        
    }

    void lua_push_meta_engine_device_object(lua_State *state, Engine::DeviceObject &object) noexcept {
        lua_push_meta_object(state, object, lua_engine_device_object__index, lua_engine_device_object__newindex);
    }

    static int lua_engine_device_machine_object_flags__index(lua_State *state) noexcept {
        auto *flags = lua_from_meta_object<Engine::DeviceMachineObjectFlags>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        }
        
        std::string field = key;
        if(field == "does_not_operate_automatically") {
            lua_pushboolean(state, flags->does_not_operate_automatically);
        }
        else if(field == "machine_one_sided") {
            lua_pushboolean(state, flags->machine_one_sided);
        }
        else if(field == "never_appears_locked") {
            lua_pushboolean(state, flags->never_appears_locked);
        }
        else if(field == "opened_by_melee_attack") {
            lua_pushboolean(state, flags->opened_by_melee_attack);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_device_machine_object_flags__newindex(lua_State *state) noexcept {
        auto *flags = lua_from_meta_object<Engine::DeviceMachineObjectFlags>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_isboolean(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        }
        
        std::string field = key;
        if(field == "does_not_operate_automatically") {
            flags->does_not_operate_automatically = lua_toboolean(state, 3);
        }
        else if(field == "machine_one_sided") {
            flags->machine_one_sided = lua_toboolean(state, 3);
        }
        else if(field == "never_appears_locked") {
            flags->never_appears_locked = lua_toboolean(state, 3);
        }
        else if(field == "opened_by_melee_attack") {
            flags->opened_by_melee_attack = lua_toboolean(state, 3);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_device_machine_object_flags(lua_State *state, Engine::DeviceMachineObjectFlags &flags) noexcept {
        lua_push_meta_object(state, flags, lua_engine_device_machine_object_flags__index, lua_engine_device_machine_object_flags__newindex);
    }

    static int lua_engine_device_machine_object__index(lua_State *state) noexcept {
        auto *device = lua_from_meta_object<Engine::DeviceMachineObject>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        
        if(key == nullptr) {  
            return lua_engine_device_object__index(state);
        }
        
        std::string field = key;
        if(field == "device_flags") {
            lua_push_meta_engine_device_machine_object_flags(state, device->device_flags);
        }
        else if(field == "ticks_since_started_opening") {
            lua_pushinteger(state, device->ticks_since_started_opening);
        }
        else if(field == "elevator_position") {
            lua_push_meta_engine_vector3_d(state, device->elevator_position);
        }
        else {
            return lua_engine_device_object__index(state);
        }
        return 1;
    }

    static int lua_engine_device_machine_object__newindex(lua_State *state) noexcept {
        auto *device = lua_from_meta_object<Engine::DeviceMachineObject>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        
        if(key == nullptr || !lua_istable(state, 3)) {  
            return lua_engine_device_object__newindex(state);
        }
        
        std::string field = key;
        if(field == "device_flags") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "ticks_since_started_opening") {
            device->ticks_since_started_opening = static_cast<std::uint32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "elevator_position") {
            return luaL_error(state, "Invalid operation");
        }
        else {
            return lua_engine_device_object__newindex(state);
        }
        return 0;
    }

    void lua_push_meta_engine_device_machine_object(lua_State *state, Engine::DeviceMachineObject &object) noexcept {
        lua_push_meta_object(state, object, lua_engine_device_machine_object__index, lua_engine_device_machine_object__newindex);
    }

    static int lua_engine_device_control_object_flags__index(lua_State *state) noexcept {
        auto *flags = lua_from_meta_object<Engine::DeviceControlObjectFlags>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        }
        
        std::string field = key;
        if(field == "usable_from_both_sides") {
            lua_pushboolean(state, flags->usable_from_both_sides);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }
    
    static int lua_engine_device_control_object_flags__newindex(lua_State *state) noexcept {
        auto *flags = lua_from_meta_object<Engine::DeviceControlObjectFlags>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_isboolean(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        }
        
        std::string field = key;
        if(field == "usable_from_both_sides") {
            flags->usable_from_both_sides = lua_toboolean(state, 3);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_device_control_object_flags(lua_State *state, Engine::DeviceControlObjectFlags &flags) noexcept {
        lua_push_meta_object(state, flags, lua_engine_device_control_object_flags__index, lua_engine_device_control_object_flags__newindex);
    }

    static int lua_engine_device_control_object__index(lua_State *state) noexcept {
        auto *device = lua_from_meta_object<Engine::DeviceControlObject>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        
        if(key == nullptr) {  
            return lua_engine_device_object__index(state);
        }
        
        std::string field = key;
        if(field == "device_control_flags") {
            lua_push_meta_engine_device_control_object_flags(state, device->device_control_flags);
        }
        else if(field == "custom_name_id") {
            lua_pushinteger(state, device->custom_name_id);
        }
        else {
            return lua_engine_device_object__index(state);
        }
        return 1;
    }

    static int lua_engine_device_control_object__newindex(lua_State *state) noexcept {
        auto *device = lua_from_meta_object<Engine::DeviceControlObject>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        
        if(key == nullptr || !lua_istable(state, 3)) {  
            return lua_engine_device_object__newindex(state);
        }
        
        std::string field = key;
        if(field == "device_control_flags") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "custom_name_id") {
            device->custom_name_id = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else {
            return lua_engine_device_object__newindex(state);
        }
        return 0;
    }

    void lua_push_meta_engine_device_control_object(lua_State *state, Engine::DeviceControlObject &object) noexcept {
        lua_push_meta_object(state, object, lua_engine_device_control_object__index, lua_engine_device_control_object__newindex);
    }

    static int lua_engine_device_light_fixture_object__index(lua_State *state) noexcept {
        auto *device = lua_from_meta_object<Engine::DeviceLightFixtureObject>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        
        if(key == nullptr) {  
            return lua_engine_device_object__index(state);
        }
        
        std::string field = key;
        if(field == "light_color") {
            lua_push_meta_engine_color_r_g_b(state, device->light_color);
        }
        else if(field == "light_intensity") {
            lua_pushnumber(state, device->light_intensity);
        }
        else if(field == "light_falloff_angle") {
            lua_pushnumber(state, device->light_falloff_angle);
        }
        else if(field == "light_cutoff_angle") {
            lua_pushnumber(state, device->light_cutoff_angle);
        }
        else {
            return lua_engine_device_object__index(state);
        }
        return 1;
    }

    static int lua_engine_device_light_fixture_object__newindex(lua_State *state) noexcept {
        auto *device = lua_from_meta_object<Engine::DeviceLightFixtureObject>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        
        if(key == nullptr || !lua_istable(state, 3)) {  
            return lua_engine_device_object__newindex(state);
        }
        
        std::string field = key;
        if(field == "light_color") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "light_intensity") {
            device->light_intensity = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "light_falloff_angle") {
            device->light_falloff_angle = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "light_cutoff_angle") {
            device->light_cutoff_angle = static_cast<float>(luaL_checknumber(state, 3));
        }
        else {
            return lua_engine_device_object__newindex(state);
        }
        return 0;
    }

    void lua_push_meta_engine_device_light_fixture_object(lua_State *state, Engine::DeviceLightFixtureObject &object) noexcept {
        lua_push_meta_object(state, object, lua_engine_device_light_fixture_object__index, lua_engine_device_light_fixture_object__newindex);
    }

    static int lua_engine_player_multiplayer_statistics__index(lua_State *state) {
        auto *stats = lua_from_meta_object<Engine::PlayerMultiplayerStatistics>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        
        if(key == nullptr) {  
            return lua_engine_device_object__index(state);
        }
        
        std::string field = key;
        if(field == "ctf_flag_grabs") {
            lua_pushinteger(state, stats->ctf.flag_grabs);
        }
        else if(field == "ctf_flag_returns") {
            lua_pushinteger(state, stats->ctf.flag_returns);
        }
        else if(field == "ctf_flag_scores") {
            lua_pushinteger(state, stats->ctf.flag_scores);
        }
        else if(field == "oddball_target_kills") {
            lua_pushinteger(state, stats->oddball.target_kills);
        }
        else if(field == "oddball_kills") {
            lua_pushinteger(state, stats->oddball.kills);
        }
        else if(field == "oddball_unknown") {
            lua_pushinteger(state, stats->oddball.unknown);
        }
        else if(field == "king_hill_score") {
            lua_pushinteger(state, stats->king.hill_score);
        }
        else if(field == "race_time") {
            lua_pushinteger(state, stats->race.time);
        }
        else if(field == "race_laps") {
            lua_pushinteger(state, stats->race.laps);
        }
        else if(field == "race_best_time") {
            lua_pushinteger(state, stats->race.best_time);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_player_multiplayer_statistics__newindex(lua_State *state) {
        auto *stats = lua_from_meta_object<Engine::PlayerMultiplayerStatistics>(state, 1); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        
        if(key == nullptr) {  
            return lua_engine_device_object__index(state);
        }
        
        std::string field = key;
        if(field == "ctf_flag_grabs") {
            stats->ctf.flag_grabs = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "ctf_flag_returns") {
            stats->ctf.flag_returns = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "ctf_flag_scores") {
            stats->ctf.flag_scores = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "oddball_target_kills") {
            stats->oddball.target_kills = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "oddball_kills") {
            stats->oddball.kills = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "oddball_unknown") {
            stats->oddball.unknown = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "king_hill_score") {
            stats->king.hill_score = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "race_time") {
            stats->race.time = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "race_laps") {
            stats->race.laps = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "race_best_time") {
            stats->race.best_time = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_player_multiplayer_statistics(lua_State *state, Engine::PlayerMultiplayerStatistics &stats) noexcept {
        lua_push_meta_object(state, stats, lua_engine_player_multiplayer_statistics__index, lua_engine_player_multiplayer_statistics__newindex);
    }

    static int lua_engine_player__index(lua_State *state) noexcept {
        auto *player = lua_from_meta_object<Engine::Player>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        }
        
        std::string field = key;
        if(field == "player_id") {
            lua_pushinteger(state, player->player_id);
        }
        else if(field == "local_handle") {
            lua_pushinteger(state, player->local_handle);
        }
        else if(field == "name") {
            lua_pushstring(state, reinterpret_cast<const char *>(player->name));
        }
        else if(field == "unknown_handle") {
            lua_pushinteger(state, player->unknown_handle.handle);
        }
        else if(field == "team") {
            lua_pushinteger(state, player->team);
        }
        else if(field == "interaction_object_handle") {
            lua_pushinteger(state, player->interaction_object_handle.handle);
        }
        else if(field == "interaction_object_type") {
            lua_pushinteger(state, player->interaction_object_type);
        }
        else if(field == "interaction_object_seat") {
            lua_pushinteger(state, player->interaction_object_seat);
        }
        else if(field == "respawn_time") {
            lua_pushinteger(state, player->respawn_time);
        }
        else if(field == "respawn_time_growth") {
            lua_pushinteger(state, player->respawn_time_growth);
        }
        else if(field == "object_handle") {
            lua_pushinteger(state, player->object_handle.handle);
        }
        else if(field == "prev_object_handle") {
            lua_pushinteger(state, player->prev_object_handle.handle);
        }
        else if(field == "bsp_cluster_id") {
            lua_pushinteger(state, player->bsp_cluster_id);
        }
        else if(field == "weapon_swap_result") {
            lua_pushboolean(state, player->weapon_swap_result);
        }
        else if(field == "auto_aim_target_object") {
            lua_pushinteger(state, player->auto_aim_target_object.handle);
        }
        else if(field == "last_fire_time") {
            lua_pushinteger(state, player->last_fire_time);
        }
        else if(field == "name2") {
            lua_pushstring(state, reinterpret_cast<const char *>(player->name2));
        }
        else if(field == "color") {
            lua_pushstring(state, network_color_to_string(player->color).c_str());
        }
        else if(field == "icon_index") {
            lua_pushinteger(state, player->icon_index);
        }
        else if(field == "machine_index") {
            lua_pushinteger(state, player->machine_index);
        }
        else if(field == "controller_index") {
            lua_pushinteger(state, player->controller_index);
        }
        else if(field == "team2") {
            lua_pushinteger(state, player->team2);
        }
        else if(field == "index") {
            lua_pushinteger(state, player->index);
        }
        else if(field == "invisibility_time") {
            lua_pushinteger(state, player->invisibility_time);
        }
        else if(field == "other_powerup_time_left") {
            lua_pushinteger(state, player->other_powerup_time_left);
        }
        else if(field == "speed") {
            lua_pushnumber(state, player->speed);
        }
        else if(field == "teleporter_flag_handle") {
            lua_pushinteger(state, player->teleporter_flag_handle.handle);
        }
        else if(field == "objective_mode") {
            lua_pushstring(state, player_objective_mode_to_string(player->objective_mode).c_str());
        }
        else if(field == "objective_player_handle") {
            lua_pushinteger(state, player->objective_player_handle.handle);
        }
        else if(field == "target_player") {
            lua_pushinteger(state, player->target_player.handle);
        }
        else if(field == "target_time") {
            lua_pushinteger(state, player->target_time);
        }
        else if(field == "last_death_time") {
            lua_pushinteger(state, player->last_death_time);
        }
        else if(field == "slayer_target") {
            lua_pushinteger(state, player->slayer_target.handle);
        }
        else if(field == "odd_man_out") {
            lua_pushboolean(state, player->odd_man_out);
        }
        else if(field == "kill_streak") {
            lua_pushinteger(state, player->kill_streak);
        }
        else if(field == "multikill") {
            lua_pushinteger(state, player->multikill);
        }
        else if(field == "last_kill_time") {
            lua_pushinteger(state, player->last_kill_time);
        }
        else if(field == "kills") {
            lua_newtable(state);
            for(std::size_t i = 0; i < sizeof(player->kills) / sizeof(player->kills[0]); i++) {
                lua_pushinteger(state, player->kills[i]);
                lua_rawseti(state, -2, i + 1);
            }
        }
        else if(field == "assists") {
            lua_newtable(state);
            for(std::size_t i = 0; i < sizeof(player->assists) / sizeof(player->assists[0]); i++) {
                lua_pushinteger(state, player->assists[i]);
                lua_rawseti(state, -2, i + 1);
            }
        }
        else if(field == "betrays") {
            lua_pushinteger(state, player->betrays);
        }
        else if(field == "deaths") {
            lua_pushinteger(state, player->deaths);
        }
        else if(field == "suicides") {
            lua_pushinteger(state, player->suicides);
        }
        else if(field == "team_kills") {
            lua_pushinteger(state, player->team_kills);
        }
        else if(field == "multiplayer_statistics") {
            lua_push_meta_engine_player_multiplayer_statistics(state, player->multiplayer_statistics);
        }
        else if(field == "telefrag_timer") {
            lua_pushinteger(state, player->telefrag_timer);
        }
        else if(field == "quit_time") {
            lua_pushinteger(state, player->quit_time);
        }
        else if(field == "telefrag_danger") {
            lua_pushboolean(state, player->telefrag_danger);
        }
        else if(field == "quit") {
            lua_pushboolean(state, player->quit);
        }
        else if(field == "ping") {
            lua_pushinteger(state, player->ping);
        }
        else if(field == "team_kill_count") {
            lua_pushinteger(state, player->team_kill_count);
        }
        else if(field == "team_kill_ticks_since_last") {
            lua_pushinteger(state, player->team_kill_ticks_since_last);
        }
        else if(field == "position") {
            lua_push_meta_engine_point3_d(state, player->position);
        }
        else if(field == "melee") {
            lua_pushboolean(state, player->melee == 1);
        }
        else if(field == "action") {
            lua_pushboolean(state, player->action == 1);
        }
        else if(field == "flashlight") {
            lua_pushboolean(state, player->flashlight == 1);
        }
        else if(field == "reload") {
            lua_pushboolean(state, player->reload == 1);
        }
        else if(field == "baseline_update_xy_aim") {
            lua_pushnumber(state, player->baseline_update_xy_aim);
        }
        else if(field == "baseline_update_z_aim") {
            lua_pushnumber(state, player->baseline_update_z_aim);
        }
        else if(field == "baseline_update_forward") {
            lua_pushnumber(state, player->baseline_update_forward);
        }
        else if(field == "baseline_update_left") {
            lua_pushnumber(state, player->baseline_update_left);
        }
        else if(field == "baseline_update_rate_of_fire") {
            lua_pushnumber(state, player->baseline_update_rate_of_fire);
        }
        else if(field == "baseline_update_weapon_slot") {
            lua_pushinteger(state, player->baseline_update_weapon_slot);
        }
        else if(field == "baseline_update_grenade_slot") {
            lua_pushinteger(state, player->baseline_update_grenade_slot);
        }
        else if(field == "update_aiming") {
            lua_push_meta_engine_point3_d(state, player->update_aiming);
        }
        else if(field == "update_position") {
            lua_push_meta_engine_point3_d(state, player->update_position);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_player__newindex(lua_State *state) noexcept {
        auto *player = lua_from_meta_object<Engine::Player>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_istable(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        }
        
        std::string field = key;
        if(field == "player_id") {
            player->player_id = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "local_handle") {
            player->local_handle = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "name") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "unknown_handle") {
            player->unknown_handle.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "team") {
            player->team = static_cast<std::uint8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "interaction_object_handle") {
            player->interaction_object_handle.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "interaction_object_type") {
            player->interaction_object_type = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "interaction_object_seat") {
            player->interaction_object_seat = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "respawn_time") {
            player->respawn_time = static_cast<std::uint32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "respawn_time_growth") {
            player->respawn_time_growth = static_cast<std::uint32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "object_handle") {
            player->object_handle.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "prev_object_handle") {
            player->prev_object_handle.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "bsp_cluster_id") {
            player->bsp_cluster_id = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "weapon_swap_result") {
            if(lua_isboolean(state, 3)) {
                player->weapon_swap_result = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "auto_aim_target_object") {
            player->auto_aim_target_object.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "last_fire_time") {
            player->last_fire_time = static_cast<std::uint32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "name2") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "color") {
            try {
                player->color = network_color_from_string(luaL_checkstring(state, 3));
            }
            catch(const std::exception &exception) {
                return luaL_error(state, exception.what());
            }
        }
        else if(field == "icon_index") {
            player->icon_index = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "machine_index") {
            player->machine_index = static_cast<std::uint8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "controller_index") {
            player->controller_index = static_cast<std::uint8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "team2") {
            player->team2 = static_cast<std::uint8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "index") {
            player->index = static_cast<std::uint8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "invisibility_time") {
            player->invisibility_time = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "other_powerup_time_left") {
            player->other_powerup_time_left = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "speed") {
            player->speed = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "teleporter_flag_handle") {
            player->teleporter_flag_handle.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "objective_mode") {
            try {
                player->objective_mode = player_objective_mode_from_string(luaL_checkstring(state, 3));
            }
            catch(const std::exception &exception) {
                return luaL_error(state, exception.what());
            }
        }
        else if(field == "objective_player_handle") {
            player->objective_player_handle.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "target_player") {
            player->target_player.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "target_time") {
            player->target_time = static_cast<std::uint32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "last_death_time") {
            player->last_death_time = static_cast<std::uint32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "slayer_target") {
            player->slayer_target.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "odd_man_out") {
            if(lua_isboolean(state, 3)) {
                player->odd_man_out = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "kill_streak") {
            player->kill_streak = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "multikill") {
            player->multikill = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "last_kill_time") {
            player->last_kill_time = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "kills") {
            for(std::size_t i = 0; i < sizeof(player->kills) / sizeof(player->kills[0]); i++) {
                lua_rawgeti(state, 3, i + 1);
                if(!lua_isnil(state, -1)) {
                    player->kills[i] = static_cast<std::int16_t>(luaL_checkinteger(state, -1));
                }
                lua_pop(state, 1);
            }
        }
        else if(field == "assists") {
            for(std::size_t i = 0; i < sizeof(player->assists) / sizeof(player->assists[0]); i++) {
                lua_rawgeti(state, 3, i + 1);
                if(!lua_isnil(state, -1)) {
                    player->assists[i] = static_cast<std::int16_t>(luaL_checkinteger(state, -1));
                }
                lua_pop(state, 1);
            }
        }
        else if(field == "betrays") {
            player->betrays = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "deaths") {
            player->deaths = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "suicides") {
            player->suicides = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "team_kills") {
            player->team_kills = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "multiplayer_statistics") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "telefrag_timer") {
            player->telefrag_timer = static_cast<std::uint32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "quit_time") {
            player->quit_time = static_cast<std::uint32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "telefrag_danger") {
            if(lua_isboolean(state, 3)) {
                player->telefrag_danger = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "quit") {
            if(lua_isboolean(state, 3)) {
                player->quit = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "ping") {
            player->ping = static_cast<std::uint32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "team_kill_count") {
            player->team_kill_count = static_cast<std::uint32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "team_kill_ticks_since_last") {
            player->team_kill_ticks_since_last = static_cast<std::uint32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "position") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "melee") {
            if(lua_isboolean(state, 3)) {
                player->melee = lua_toboolean(state, 3) ? 1 : 0;
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "action") {
            if(lua_isboolean(state, 3)) {
                player->action = lua_toboolean(state, 3) ? 1 : 0;
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "flashlight") {
            if(lua_isboolean(state, 3)) {
                player->flashlight = lua_toboolean(state, 3) ? 1 : 0;
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "reload") {
            if(lua_isboolean(state, 3)) {
                player->reload = lua_toboolean(state, 3) ? 1 : 0;
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "baseline_update_xy_aim") {
            player->baseline_update_xy_aim = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "baseline_update_z_aim") {
            player->baseline_update_z_aim = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "baseline_update_forward") {
            player->baseline_update_forward = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "baseline_update_left") {
            player->baseline_update_left = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "baseline_update_rate_of_fire") {
            player->baseline_update_rate_of_fire = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "baseline_update_weapon_slot") {
            player->baseline_update_weapon_slot = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "baseline_update_grenade_slot") {
            player->baseline_update_grenade_slot = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "update_aiming") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "update_position") {
            return luaL_error(state, "Invalid operation");
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_player(lua_State *state, Engine::Player &player) noexcept {
        lua_push_meta_object(state, player, lua_engine_player__index, lua_engine_player__newindex);
    }

    static int lua_event_widget_render_vertex__index(lua_State *state) noexcept {
        auto *vertex = lua_from_meta_object<Event::WidgetRenderVertices::Vertex>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {
            return luaL_error(state, "Invalid key type");
        }
        
        std::string field = key;
        if(field == "x") {
            lua_pushnumber(state, vertex->x);
        }
        else if(field == "y") {
            lua_pushnumber(state, vertex->y);
        }
        else if(field == "z") {
            lua_pushnumber(state, vertex->z);
        }
        else if(field == "rhw") {
            lua_pushnumber(state, vertex->rhw);
        }
        else if(field == "u") {
            lua_pushnumber(state, vertex->u);
        }
        else if(field == "v") {
            lua_pushnumber(state, vertex->v);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_event_widget_render_vertex__newindex(lua_State *state) noexcept {
        auto *vertex = lua_from_meta_object<Event::WidgetRenderVertices::Vertex>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {
            return luaL_error(state, "Invalid key type");
        }
        
        std::string field = key;
        if(field == "x") {
            vertex->x = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "y") {
            vertex->y = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "z") {
            vertex->z = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "rhw") {
            vertex->rhw = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "u") {
            vertex->u = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "v") {
            vertex->v = static_cast<float>(luaL_checknumber(state, 3));
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_event_widget_render_vertex(lua_State *state, Event::WidgetRenderVertices::Vertex &vertex) noexcept {
        lua_push_meta_object(state, vertex, lua_event_widget_render_vertex__index, lua_event_widget_render_vertex__newindex);
    }
    
    static int lua_event_widget_render_vertices__index(lua_State *state) noexcept {
        auto *vertices = lua_from_meta_object<Event::WidgetRenderVertices>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {
            return luaL_error(state, "Invalid key type");
        }
        
        std::string field = key;
        if(field == "top_left") {
            lua_push_meta_event_widget_render_vertex(state, vertices->top_left);
        }
        else if(field == "top_right") {
            lua_push_meta_event_widget_render_vertex(state, vertices->top_right);
        }
        else if(field == "bottom_left") {
            lua_push_meta_event_widget_render_vertex(state, vertices->bottom_left);
        }
        else if(field == "bottom_right") {
            lua_push_meta_event_widget_render_vertex(state, vertices->bottom_right);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_event_widget_render_vertices__newindex(lua_State *state) noexcept {
        auto *vertices = lua_from_meta_object<Event::WidgetRenderVertices>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {
            return luaL_error(state, "Invalid key type");
        }
        
        std::string field = key;
        if(field == "top_left") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "top_right") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "bottom_left") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "bottom_right") {
            return luaL_error(state, "Invalid operation");
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_event_widget_render_vertices(lua_State *state, Event::WidgetRenderVertices &vertices) noexcept {
        lua_push_meta_object(state, vertices, lua_event_widget_render_vertices__index, lua_event_widget_render_vertices__newindex);
    }
}
