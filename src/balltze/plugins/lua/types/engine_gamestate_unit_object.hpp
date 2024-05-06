// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__PLUGINS__LUA__TYPES__ENGINE_GAMESTATE_UNIT_OBJECT_HPP
#define BALLTZE__PLUGINS__LUA__TYPES__ENGINE_GAMESTATE_UNIT_OBJECT_HPP

#include <lua.hpp>
#include <balltze/engine.hpp>
#include "../helpers/luacstruct.hpp"

namespace Balltze::Plugins::Lua {
    using EngineUnitRecentDamager = Engine::UnitRecentDamager;
    using EngineUnitObjectFlags = Engine::UnitFlags;
    using EngineUnitControlFlags = Engine::UnitControlFlags;
    using EngineUnitThrowingGrenadeState = Engine::UnitThrowingGrenadeState;
    using EngineUnitAnimationState = Engine::UnitAnimationState;
    using EngineUnitReplacementAnimationState = Engine::UnitReplacementAnimationState;
    using EngineUnitOverlayAnimationState = Engine::UnitOverlayAnimationState;
    using EngineUnitBaseSeat = Engine::UnitBaseSeat;
    using EngineUnitAnimationStateData = Engine::UnitAnimationStateData;
    using EngineUnitAnimationFlags = Engine::UnitAnimationFlags;
    using EngineUnitAnimationData = Engine::UnitAnimationData;
    using EngineUnitSpeechPriority = Engine::UnitSpeechPriority;
    using EngineUnitScreamType = Engine::UnitScreamType;
    using EngineAiCommunicationType = Engine::AiCommunicationType;
    using EngineAiCommunicationPacket = Engine::AiCommunicationPacket;
    using EngineUnitSpeech = Engine::UnitSpeech;
    using EngineUnitSpeechData = Engine::UnitSpeechData;
    using EngineUnitControlData = Engine::UnitControlData;
    using EngineUnitObject = Engine::UnitObject;
    using EngineBipedObjectFlags = Engine::BipedFlags;
    using EngineBipedMovementState = Engine::BipedMovementState;
    using EngineBipedNetworkDelta = Engine::BipedNetworkDelta;
    using EngineBipedNetwork = Engine::BipedNetwork;
    using EngineBipedObject = Engine::BipedObject;
    using EngineVehicleObjectFlags = Engine::VehicleFlags;
    using EngineVehicleNetworkData = Engine::VehicleNetworkData;
    using EngineVehicleNetwork = Engine::VehicleNetwork;
    using EngineVehicleObject = Engine::VehicleObject;

    void lua_define_engine_unit_throwing_grenade_state_enum(lua_State *state) noexcept;
    void lua_define_engine_unit_animation_state_enum(lua_State *state) noexcept;
    void lua_define_engine_unit_replacement_animation_state_enum(lua_State *state) noexcept;
    void lua_define_engine_unit_overlay_animation_state_enum(lua_State *state) noexcept;
    void lua_define_engine_unit_base_seat_enum(lua_State *state) noexcept;
    void lua_define_engine_unit_speech_priority_enum(lua_State *state) noexcept;
    void lua_define_engine_unit_scream_type_enum(lua_State *state) noexcept;
    void lua_define_engine_ai_communication_type_enum(lua_State *state) noexcept;
    void lua_define_engine_biped_movement_state_enum(lua_State *state) noexcept;

    void lua_define_engine_unit_recent_damager_struct(lua_State *state) noexcept;
    void lua_push_engine_unit_recent_damager(lua_State *state, EngineUnitRecentDamager &damager) noexcept;

    void lua_define_engine_unit_flags_struct(lua_State *state) noexcept;
    void lua_push_engine_unit_flags(lua_State *state, EngineUnitObjectFlags &flags) noexcept;

    void lua_define_engine_unit_control_flags_struct(lua_State *state) noexcept;
    void lua_push_engine_unit_control_flags(lua_State *state, EngineUnitControlFlags &flags) noexcept;

    void lua_define_engine_unit_animation_state_data_struct(lua_State *state) noexcept;
    void lua_push_engine_unit_animation_state_data(lua_State *state, EngineUnitAnimationStateData &data) noexcept;

    void lua_define_engine_unit_animation_flags_struct(lua_State *state) noexcept;
    void lua_push_engine_unit_animation_flags(lua_State *state, EngineUnitAnimationFlags &data) noexcept;

    void lua_define_engine_unit_animation_data_struct(lua_State *state) noexcept;
    void lua_push_engine_unit_animation_data(lua_State *state, EngineUnitAnimationData &data) noexcept;

    void lua_define_engine_ai_communication_packet_struct(lua_State *state) noexcept;
    void lua_push_engine_ai_communication_packet(lua_State *state, EngineAiCommunicationPacket &packet) noexcept;

    void lua_define_engine_unit_speech_struct(lua_State *state) noexcept;
    void lua_push_engine_unit_speech(lua_State *state, EngineUnitSpeech &speech) noexcept;

    void lua_define_engine_unit_speech_data_struct(lua_State *state) noexcept;
    void lua_push_engine_unit_speech_data(lua_State *state, EngineUnitSpeechData &speech) noexcept;

    void lua_define_engine_unit_control_data_struct(lua_State *state) noexcept;
    void lua_push_engine_unit_control_data(lua_State *state, EngineUnitControlData &data) noexcept;

    void lua_define_engine_unit_object_struct(lua_State *state) noexcept;
    void lua_push_engine_unit_object(lua_State *state, EngineUnitObject &object) noexcept;

    void lua_define_engine_biped_flags_struct(lua_State *state) noexcept;
    void lua_push_engine_biped_flags(lua_State *state, EngineBipedObjectFlags &flags) noexcept;

    void lua_define_engine_biped_network_delta_struct(lua_State *state) noexcept;
    void lua_push_engine_biped_network_delta(lua_State *state, EngineBipedNetworkDelta &delta) noexcept;

    void lua_define_engine_biped_network_struct(lua_State *state) noexcept;
    void lua_push_engine_biped_network(lua_State *state, EngineBipedNetwork &network) noexcept;

    void lua_define_engine_biped_object_struct(lua_State *state) noexcept;
    void lua_push_engine_biped_object(lua_State *state, EngineBipedObject &object) noexcept;

    void lua_define_engine_vehicle_flags_struct(lua_State *state) noexcept;
    void lua_push_engine_vehicle_flags(lua_State *state, EngineVehicleObjectFlags &flags) noexcept;

    void lua_define_engine_vehicle_network_data_struct(lua_State *state) noexcept;
    void lua_push_engine_vehicle_network_data(lua_State *state, EngineVehicleNetworkData &data) noexcept;

    void lua_define_engine_vehicle_network_struct(lua_State *state) noexcept;
    void lua_push_engine_vehicle_network(lua_State *state, EngineVehicleNetwork &network) noexcept;

    void lua_define_engine_vehicle_object_struct(lua_State *state) noexcept;
    void lua_push_engine_vehicle_object(lua_State *state, EngineVehicleObject &object) noexcept;

    inline void lua_define_engine_gamestate_unit_object_types(lua_State *state) noexcept {
        luacs_newenum(state, EngineUnitThrowingGrenadeState);
        lua_pop(state, 1);
        luacs_newenum(state, EngineUnitAnimationState);
        lua_pop(state, 1);
        luacs_newenum(state, EngineUnitReplacementAnimationState);
        lua_pop(state, 1);
        luacs_newenum(state, EngineUnitOverlayAnimationState);
        lua_pop(state, 1);
        luacs_newenum(state, EngineUnitBaseSeat);
        lua_pop(state, 1);
        luacs_newenum(state, EngineUnitSpeechPriority);
        lua_pop(state, 1);
        luacs_newenum(state, EngineUnitScreamType);
        lua_pop(state, 1);
        luacs_newenum(state, EngineAiCommunicationType);
        lua_pop(state, 1);
        luacs_newenum(state, EngineBipedMovementState);
        lua_pop(state, 1);
        luacs_newstruct(state, EngineUnitRecentDamager);
        lua_pop(state, 1);
        luacs_newstruct(state, EngineUnitObjectFlags); 
        lua_pop(state, 1);
        luacs_newstruct(state, EngineUnitControlFlags); 
        lua_pop(state, 1);
        luacs_newstruct(state, EngineUnitAnimationStateData);
        lua_pop(state, 1);
        luacs_newstruct(state, EngineUnitAnimationFlags); 
        lua_pop(state, 1);
        luacs_newstruct(state, EngineUnitAnimationData);
        lua_pop(state, 1);
        luacs_newstruct(state, EngineAiCommunicationPacket);
        lua_pop(state, 1);
        luacs_newstruct(state, EngineUnitSpeech);
        lua_pop(state, 1);
        luacs_newstruct(state, EngineUnitSpeechData);
        lua_pop(state, 1);
        luacs_newstruct(state, EngineUnitControlData);
        lua_pop(state, 1);
        luacs_newstruct(state, EngineUnitObject);
        lua_pop(state, 1);
        luacs_newstruct(state, EngineBipedObjectFlags); 
        lua_pop(state, 1);
        luacs_newstruct(state, EngineBipedNetworkDelta);
        lua_pop(state, 1);
        luacs_newstruct(state, EngineBipedNetwork);
        lua_pop(state, 1);
        luacs_newstruct(state, EngineVehicleObjectFlags); 
        lua_pop(state, 1);
        luacs_newstruct(state, EngineVehicleNetworkData);
        lua_pop(state, 1);
        luacs_newstruct(state, EngineVehicleNetwork);
        lua_pop(state, 1);
        luacs_newstruct(state, EngineVehicleObject);
        lua_pop(state, 1);
        luacs_newderivedstruct(state, EngineBipedObject, EngineUnitObject);
        lua_pop(state, 1);

        lua_define_engine_unit_throwing_grenade_state_enum(state);
        lua_define_engine_unit_animation_state_enum(state);
        lua_define_engine_unit_replacement_animation_state_enum(state);
        lua_define_engine_unit_overlay_animation_state_enum(state);
        lua_define_engine_unit_base_seat_enum(state);
        lua_define_engine_unit_speech_priority_enum(state);
        lua_define_engine_unit_scream_type_enum(state);
        lua_define_engine_ai_communication_type_enum(state);
        lua_define_engine_biped_movement_state_enum(state);

        lua_define_engine_unit_recent_damager_struct(state);
        lua_define_engine_unit_flags_struct(state);
        lua_define_engine_unit_control_flags_struct(state);
        lua_define_engine_unit_animation_state_data_struct(state);
        lua_define_engine_unit_animation_flags_struct(state);
        lua_define_engine_unit_animation_data_struct(state);
        lua_define_engine_ai_communication_packet_struct(state);
        lua_define_engine_unit_speech_struct(state);
        lua_define_engine_unit_speech_data_struct(state);
        lua_define_engine_unit_control_data_struct(state);
        lua_define_engine_unit_object_struct(state);

        lua_define_engine_biped_flags_struct(state);
        lua_define_engine_biped_network_delta_struct(state);
        lua_define_engine_biped_network_struct(state);
        lua_define_engine_biped_object_struct(state);

        lua_define_engine_vehicle_flags_struct(state);
        lua_define_engine_vehicle_network_data_struct(state);
        lua_define_engine_vehicle_network_struct(state);
        lua_define_engine_vehicle_object_struct(state);
    }
}

#endif
