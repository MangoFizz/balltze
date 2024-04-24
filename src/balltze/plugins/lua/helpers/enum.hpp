// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__PLUGINS__LUA__HELPERS__ENUM_HPP
#define BALLTZE__PLUGINS__LUA__HELPERS__ENUM_HPP

#include <string>
#include <balltze/engine.hpp>

namespace Balltze::Plugins {
    std::string engine_edition_to_string(Engine::EngineEdition edition);
    Engine::EngineEdition engine_edition_from_string(const std::string &edition);
    
    std::string cache_file_engine_to_string(Engine::CacheFileEngine engine);
    Engine::CacheFileEngine cache_file_engine_from_string(const std::string &engine);
    
    std::string map_game_type_to_string(Engine::MapGameType type);
    Engine::MapGameType map_game_type_from_string(const std::string &type);
    
    std::string network_game_server_type_to_string(Engine::NetworkGameServerType type);
    Engine::NetworkGameServerType network_game_server_type_from_string(const std::string &type);

    std::string network_game_server_game_type_to_string(Engine::NetworkGameType gametype);
    Engine::NetworkGameType network_game_server_game_type_from_string(const std::string &gametype);

    std::string object_type_to_string(Engine::ObjectType type);
    Engine::ObjectType object_type_from_string(const std::string &type);

    std::string object_network_role_to_string(Engine::ObjectNetworkRole role);
    Engine::ObjectNetworkRole object_network_role_from_string(const std::string &role);

    std::string object_attachment_type_to_string(Engine::BaseObjectAttachmentType type);
    Engine::BaseObjectAttachmentType object_attachment_type_from_string(const std::string &type);

    std::string network_game_multiplayer_team_to_string(Engine::NetworkGameMultiplayerTeam team);
    Engine::NetworkGameMultiplayerTeam network_game_multiplayer_team_from_string(const std::string &team);

    std::string camera_type_to_string(Engine::CameraType camera_type);
    Engine::CameraType camera_type_from_string(const std::string &camera_type);

    std::string unit_throwing_grenade_state_to_string(Engine::UnitThrowingGrenadeState state);
    Engine::UnitThrowingGrenadeState unit_throwing_grenade_state_from_string(const std::string &state);

    std::string unit_animation_state_to_string(Engine::UnitAnimationState state);
    Engine::UnitAnimationState unit_animation_state_from_string(const std::string &state);

    std::string unit_replacement_animation_state_to_string(Engine::UnitReplacementAnimationState state);
    Engine::UnitReplacementAnimationState unit_replacement_animation_state_from_string(const std::string &state);
    
    std::string unit_overlay_animation_state_to_string(Engine::UnitOverlayAnimationState state);
    Engine::UnitOverlayAnimationState unit_overlay_animation_state_from_string(const std::string &state);
    
    std::string unit_base_seat_to_string(Engine::UnitBaseSeat seat);
    Engine::UnitBaseSeat unit_base_seat_from_string(const std::string &seat);

    std::string unit_speech_priority_to_string(Engine::UnitSpeechPriority priority);
    Engine::UnitSpeechPriority unit_speech_priority_from_string(const std::string &priority);

    std::string unit_scream_type_to_string(Engine::UnitScreamType type);
    Engine::UnitScreamType unit_scream_type_from_string(const std::string &type);

    std::string ai_communication_type_to_string(Engine::AiCommunicationType type);
    Engine::AiCommunicationType ai_communication_type_from_string(const std::string &type);

    std::string biped_movement_state_to_string(Engine::BipedMovementState state);
    Engine::BipedMovementState biped_movement_state_from_string(const std::string &state);

    std::string weapon_state_to_string(Engine::WeaponState state);
    Engine::WeaponState weapon_state_from_string(const std::string &state);

    std::string weapon_magazine_state_to_string(Engine::WeaponMagazineState state);
    Engine::WeaponMagazineState weapon_magazine_state_from_string(const std::string &state);

    std::string network_color_to_string(Engine::NetworkPlayerColor color);
    Engine::NetworkPlayerColor network_color_from_string(const std::string &color);

    std::string player_objective_mode_to_string(Engine::PlayerObjectiveMode mode);
    Engine::PlayerObjectiveMode player_objective_mode_from_string(const std::string &mode);

    std::string input_device_to_string(Engine::InputDevice device);
    Engine::InputDevice input_device_from_string(const std::string &device);
}

#endif
