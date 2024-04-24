// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__PLUGINS__LUA__METATABLES__ENGINE_GAMESTATE_UNIT_OBJECT_HPP
#define BALLTZE__PLUGINS__LUA__METATABLES__ENGINE_GAMESTATE_UNIT_OBJECT_HPP

#include <string>
#include <balltze/engine.hpp>

namespace Balltze::Plugins {
    void lua_push_meta_engine_unit_recent_damager(lua_State *state, Engine::UnitRecentDamager &damager, bool read_only = false) noexcept;
    void lua_push_meta_engine_unit_flags(lua_State *state, Engine::UnitFlags &flags, bool read_only = false) noexcept;
    void lua_push_meta_engine_unit_control_flags(lua_State *state, Engine::UnitControlFlags &flags, bool read_only = false) noexcept;
    void lua_push_meta_engine_unit_animation_state_data(lua_State *state, Engine::UnitAnimationStateData &data, bool read_only = false) noexcept;
    void lua_push_meta_engine_unit_animation_flags(lua_State *state, Engine::UnitAnimationFlags &data, bool read_only = false) noexcept;
    void lua_push_meta_engine_unit_animation_data(lua_State *state, Engine::UnitAnimationData &data, bool read_only = false) noexcept;
    void lua_push_meta_engine_ai_communication_packet(lua_State *state, Engine::AiCommunicationPacket &packet, bool read_only = false) noexcept;
    void lua_push_meta_engine_unit_speech(lua_State *state, Engine::UnitSpeech &speech, bool read_only = false) noexcept;
    void lua_push_meta_engine_unit_speech_data(lua_State *state, Engine::UnitSpeech &speech, bool read_only = false) noexcept;
    void lua_push_meta_engine_unit_control_data(lua_State *state, Engine::UnitControlData &data, bool read_only = false) noexcept;
    void lua_push_meta_engine_unit_object(lua_State *state, Engine::UnitObject &object, bool read_only = false) noexcept;
    void lua_push_meta_engine_biped_flags(lua_State *state, Engine::BipedFlags &flags, bool read_only = false) noexcept;
    void lua_push_meta_engine_biped_network_delta(lua_State *state, Engine::BipedNetworkDelta &delta, bool read_only = false) noexcept;
    void lua_push_meta_engine_biped_network(lua_State *state, Engine::BipedNetwork &network, bool read_only = false) noexcept;
    void lua_push_meta_engine_biped_object(lua_State *state, Engine::BipedObject &object, bool read_only = false) noexcept;
    void lua_push_meta_engine_vehicle_flags(lua_State *state, Engine::VehicleFlags &flags, bool read_only = false) noexcept;
    void lua_push_meta_engine_vehicle_network_data(lua_State *state, Engine::VehicleNetworkData &data, bool read_only = false) noexcept;
    void lua_push_meta_engine_vehicle_network(lua_State *state, Engine::VehicleNetwork &network, bool read_only = false) noexcept;
    void lua_push_meta_engine_vehicle_object(lua_State *state, Engine::VehicleObject &object, bool read_only = false) noexcept;
}

#endif
