// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__LUA__API__TYPES__ENGINE_GAMESTATE_OBJECT_HPP
#define BALLTZE__LUA__API__TYPES__ENGINE_GAMESTATE_OBJECT_HPP

#include <lua.hpp>
#include <balltze/legacy_api/engine.hpp>

namespace Balltze::Lua::Api::V1 {
    using EngineObjectNetworkRole = LegacyApi::Engine::ObjectNetworkRole;
    using EngineGameStateObjectFlags = LegacyApi::Engine::BaseObjectFlags;
    using EngineBaseObjectNetwork = LegacyApi::Engine::BaseObjectNetwork;
    using EngineScenarioLocation = LegacyApi::Engine::ScenarioLocation;
    using EngineBaseObjectVitalsFlags = LegacyApi::Engine::BaseObjectVitalsFlags;
    using EngineBaseObjectVitals = LegacyApi::Engine::BaseObjectVitals;
    using EngineBaseObjectAttachmentsData = LegacyApi::Engine::BaseObjectAttachmentsData;
    using EngineBaseObjectAttachmentType = LegacyApi::Engine::BaseObjectAttachmentType;
    using EngineBaseObjectRegionDestroyeds = LegacyApi::Engine::BaseObjectRegionDestroyeds;
    using EngineBaseObjectBlockReference = LegacyApi::Engine::BaseObjectBlockReference;
    using EngineObjectValidOutGoingFunctions = LegacyApi::Engine::ObjectValidOutGoingFunctions;
    using EngineBaseObject = LegacyApi::Engine::BaseObject;

    LegacyApi::Engine::ObjectType get_object_type(lua_State *state, int index) noexcept;

    void push_engine_object_flags(lua_State *state, LegacyApi::Engine::BaseObjectFlags *flags) noexcept;

    void push_engine_object_network(lua_State *state, LegacyApi::Engine::BaseObjectNetwork *network) noexcept;

    void push_engine_scenario_location(lua_State *state, LegacyApi::Engine::ScenarioLocation *location) noexcept;

    void push_engine_object_vitals_flags(lua_State *state, LegacyApi::Engine::BaseObjectVitalsFlags *flags) noexcept;

    void push_engine_object_vitals(lua_State *state, LegacyApi::Engine::BaseObjectVitals *vitals) noexcept;

    void push_engine_object_attachments_data(lua_State *state, LegacyApi::Engine::BaseObjectAttachmentsData *data) noexcept;

    void push_engine_object_region_destroyeds(lua_State *state, LegacyApi::Engine::BaseObjectRegionDestroyeds *destroyeds) noexcept;

    void push_engine_object_block_reference(lua_State *state, LegacyApi::Engine::BaseObjectBlockReference *reference) noexcept;

    void push_engine_object_valid_out_going_functions(lua_State *state, LegacyApi::Engine::ObjectValidOutGoingFunctions *functions) noexcept;

    void push_engine_object(lua_State *state, LegacyApi::Engine::BaseObject *object) noexcept;

    void define_engine_gamestate_object_types(lua_State *state) noexcept;
}

#endif
