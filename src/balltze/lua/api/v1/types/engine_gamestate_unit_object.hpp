// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__LUA__API__TYPES__ENGINE_GAMESTATE_UNIT_OBJECT_HPP
#define BALLTZE__LUA__API__TYPES__ENGINE_GAMESTATE_UNIT_OBJECT_HPP

#include <lua.hpp>
#include <balltze/engine.hpp>

namespace Balltze::Lua {
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
