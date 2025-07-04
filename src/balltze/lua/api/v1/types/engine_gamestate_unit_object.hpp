// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__LUA__API__TYPES__ENGINE_GAMESTATE_UNIT_OBJECT_HPP
#define BALLTZE__LUA__API__TYPES__ENGINE_GAMESTATE_UNIT_OBJECT_HPP

#include <lua.hpp>
#include <balltze/legacy_api/engine.hpp>

namespace Balltze::Lua::Api::V1 {
    using EngineUnitRecentDamager = LegacyApi::Engine::UnitRecentDamager;
    using EngineUnitObjectFlags = LegacyApi::Engine::UnitFlags;
    using EngineUnitControlFlags = LegacyApi::Engine::UnitControlFlags;
    using EngineUnitThrowingGrenadeState = LegacyApi::Engine::UnitThrowingGrenadeState;
    using EngineUnitAnimationState = LegacyApi::Engine::UnitAnimationState;
    using EngineUnitReplacementAnimationState = LegacyApi::Engine::UnitReplacementAnimationState;
    using EngineUnitOverlayAnimationState = LegacyApi::Engine::UnitOverlayAnimationState;
    using EngineUnitBaseSeat = LegacyApi::Engine::UnitBaseSeat;
    using EngineUnitAnimationStateData = LegacyApi::Engine::UnitAnimationStateData;
    using EngineUnitAnimationFlags = LegacyApi::Engine::UnitAnimationFlags;
    using EngineUnitAnimationData = LegacyApi::Engine::UnitAnimationData;
    using EngineUnitSpeechPriority = LegacyApi::Engine::UnitSpeechPriority;
    using EngineUnitScreamType = LegacyApi::Engine::UnitScreamType;
    using EngineAiCommunicationType = LegacyApi::Engine::AiCommunicationType;
    using EngineAiCommunicationPacket = LegacyApi::Engine::AiCommunicationPacket;
    using EngineUnitSpeech = LegacyApi::Engine::UnitSpeech;
    using EngineUnitSpeechData = LegacyApi::Engine::UnitSpeechData;
    using EngineUnitControlData = LegacyApi::Engine::UnitControlData;
    using EngineUnitObject = LegacyApi::Engine::UnitObject;
    using EngineBipedObjectFlags = LegacyApi::Engine::BipedFlags;
    using EngineBipedMovementState = LegacyApi::Engine::BipedMovementState;
    using EngineBipedNetworkDelta = LegacyApi::Engine::BipedNetworkDelta;
    using EngineBipedNetwork = LegacyApi::Engine::BipedNetwork;
    using EngineBipedObject = LegacyApi::Engine::BipedObject;
    using EngineVehicleObjectFlags = LegacyApi::Engine::VehicleFlags;
    using EngineVehicleNetworkData = LegacyApi::Engine::VehicleNetworkData;
    using EngineVehicleNetwork = LegacyApi::Engine::VehicleNetwork;
    using EngineVehicleObject = LegacyApi::Engine::VehicleObject;

    void push_engine_unit_recent_damager(lua_State *state, EngineUnitRecentDamager *damager) noexcept;

    void push_engine_unit_flags(lua_State *state, EngineUnitObjectFlags *flags) noexcept;

    void push_engine_unit_control_flags(lua_State *state, EngineUnitControlFlags *flags) noexcept;

    void push_engine_unit_animation_state_data(lua_State *state, EngineUnitAnimationStateData *data) noexcept;

    void push_engine_unit_animation_flags(lua_State *state, EngineUnitAnimationFlags *data) noexcept;

    void push_engine_unit_animation_data(lua_State *state, EngineUnitAnimationData *data) noexcept;

    void push_engine_ai_communication_packet(lua_State *state, EngineAiCommunicationPacket *packet) noexcept;

    void push_engine_unit_speech(lua_State *state, EngineUnitSpeech *speech) noexcept;

    void push_engine_unit_speech_data(lua_State *state, EngineUnitSpeechData *speech) noexcept;

    void push_engine_unit_control_data(lua_State *state, EngineUnitControlData *data) noexcept;

    void push_engine_unit_object(lua_State *state, EngineUnitObject *object) noexcept;

    void push_engine_biped_flags(lua_State *state, EngineBipedObjectFlags *flags) noexcept;

    void push_engine_biped_network_delta(lua_State *state, EngineBipedNetworkDelta *delta) noexcept;

    void push_engine_biped_network(lua_State *state, EngineBipedNetwork *network) noexcept;

    void push_engine_biped_object(lua_State *state, EngineBipedObject *object) noexcept;

    void push_engine_vehicle_flags(lua_State *state, EngineVehicleObjectFlags *flags) noexcept;

    void push_engine_vehicle_network_data(lua_State *state, EngineVehicleNetworkData *data) noexcept;

    void push_engine_vehicle_network(lua_State *state, EngineVehicleNetwork *network) noexcept;

    void push_engine_vehicle_object(lua_State *state, EngineVehicleObject *object) noexcept;

    void define_engine_gamestate_unit_object_types(lua_State *state) noexcept;
}

#endif
