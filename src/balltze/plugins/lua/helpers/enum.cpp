// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/engine.hpp>

namespace Balltze::Plugins {
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

    std::string network_game_server_type_to_string(Engine::NetworkGameServerType type) {
        switch(type) {
            case Engine::NETWORK_GAME_SERVER_NONE:
                return "none";
            case Engine::NETWORK_GAME_SERVER_DEDICATED:
                return "dedicated";
            case Engine::NETWORK_GAME_SERVER_LOCAL:
                return "local";
            default:
                return "unknown";
        }
    }

    Engine::NetworkGameServerType network_game_server_type_from_string(const std::string &type) {
        if(type == "none") {
            return Engine::NETWORK_GAME_SERVER_NONE;
        }
        else if(type == "dedicated") {
            return Engine::NETWORK_GAME_SERVER_DEDICATED;
        }
        else if(type == "local") {
            return Engine::NETWORK_GAME_SERVER_LOCAL;
        }
        else {
            throw std::runtime_error("Invalid server type.");
        }
    }

    std::string network_game_server_game_type_to_string(Engine::NetworkGameType gametype) {
        switch(gametype) {
            case Engine::NETWORK_GAMETYPE_CTF:
                return "ctf";
            case Engine::NETWORK_GAMETYPE_SLAYER:
                return "slayer";
            case Engine::NETWORK_GAMETYPE_ODDBALL:
                return "oddball";
            case Engine::NETWORK_GAMETYPE_KING:
                return "king";
            case Engine::NETWORK_GAMETYPE_RACE:
                return "race";
            default:
                return "none";
        }
    }

    Engine::NetworkGameType network_game_server_game_type_from_string(const std::string &gametype) {
        if(gametype == "ctf") {
            return Engine::NETWORK_GAMETYPE_CTF;
        }
        else if(gametype == "slayer") {
            return Engine::NETWORK_GAMETYPE_SLAYER;
        }
        else if(gametype == "oddball") {
            return Engine::NETWORK_GAMETYPE_ODDBALL;
        }
        else if(gametype == "king") {
            return Engine::NETWORK_GAMETYPE_KING;
        }
        else if(gametype == "race") {
            return Engine::NETWORK_GAMETYPE_RACE;
        }
        else {
            throw std::runtime_error("Invalid server game type.");
        }
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

    std::string network_game_multiplayer_team_to_string(Engine::NetworkGameMultiplayerTeam team) {
        switch(team) {
            case Engine::NETWORK_GAME_TEAM_RED:
                return "red";
            case Engine::NETWORK_GAME_TEAM_BLUE:
                return "blue";
            default:
                return "none";
        }
    }
    
    Engine::NetworkGameMultiplayerTeam network_game_multiplayer_team_from_string(const std::string &team) {
        if(team == "red") {
            return Engine::NETWORK_GAME_TEAM_RED;
        }
        else if(team == "blue") {
            return Engine::NETWORK_GAME_TEAM_BLUE;
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

    std::string input_device_to_string(Engine::InputDevice device) {
        switch(device) {
            case Engine::INPUT_DEVICE_KEYBOARD:
                return "keyboard";
            case Engine::INPUT_DEVICE_MOUSE:
                return "mouse";
            case Engine::INPUT_DEVICE_GAMEPAD:
                return "gamepad";
            default:
                return "unknown";
        }
    }

    Engine::InputDevice input_device_from_string(const std::string &device) {
        if(device == "keyboard") {
            return Engine::INPUT_DEVICE_KEYBOARD;
        }
        else if(device == "mouse") {
            return Engine::INPUT_DEVICE_MOUSE;
        }
        else if(device == "gamepad") {
            return Engine::INPUT_DEVICE_GAMEPAD;
        }
        else {
            throw std::runtime_error("Invalid input device.");
        }
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

    std::string network_color_to_string(Engine::NetworkPlayerColor color) {
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

    Engine::NetworkPlayerColor network_color_from_string(const std::string &color) {
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
}
