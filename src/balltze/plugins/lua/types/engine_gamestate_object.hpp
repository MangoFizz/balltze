// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__PLUGINS__LUA__TYPES__ENGINE_GAMESTATE_OBJECT_HPP
#define BALLTZE__PLUGINS__LUA__TYPES__ENGINE_GAMESTATE_OBJECT_HPP

#include <lua.hpp>
#include <balltze/engine.hpp>

namespace Balltze::Plugins::Lua {
    using EngineObjectNetworkRole = Engine::ObjectNetworkRole;
    using EngineGameStateObjectFlags = Engine::BaseObjectFlags;
    using EngineBaseObjectNetwork = Engine::BaseObjectNetwork;
    using EngineScenarioLocation = Engine::ScenarioLocation;
    using EngineBaseObjectVitalsFlags = Engine::BaseObjectVitalsFlags;
    using EngineBaseObjectVitals = Engine::BaseObjectVitals;
    using EngineBaseObjectAttachmentsData = Engine::BaseObjectAttachmentsData;
    using EngineBaseObjectAttachmentType = Engine::BaseObjectAttachmentType;
    using EngineBaseObjectRegionDestroyeds = Engine::BaseObjectRegionDestroyeds;
    using EngineBaseObjectBlockReference = Engine::BaseObjectBlockReference;
    using EngineObjectValidOutGoingFunctions = Engine::ObjectValidOutGoingFunctions;
    using EngineBaseObject = Engine::BaseObject;

    Engine::ObjectType get_object_type(lua_State *state, int index) noexcept;

    void push_engine_object_flags(lua_State *state, Engine::BaseObjectFlags *flags) noexcept;

    void push_engine_object_network(lua_State *state, Engine::BaseObjectNetwork *network) noexcept;

    void push_engine_scenario_location(lua_State *state, Engine::ScenarioLocation *location) noexcept;

    void push_engine_object_vitals_flags(lua_State *state, Engine::BaseObjectVitalsFlags *flags) noexcept;

    void push_engine_object_vitals(lua_State *state, Engine::BaseObjectVitals *vitals) noexcept;

    void push_engine_object_attachments_data(lua_State *state, Engine::BaseObjectAttachmentsData *data) noexcept;

    void push_engine_object_region_destroyeds(lua_State *state, Engine::BaseObjectRegionDestroyeds *destroyeds) noexcept;

    void push_engine_object_block_reference(lua_State *state, Engine::BaseObjectBlockReference *reference) noexcept;

    void push_engine_object_valid_out_going_functions(lua_State *state, Engine::ObjectValidOutGoingFunctions *functions) noexcept;

    void push_engine_object(lua_State *state, Engine::BaseObject *object) noexcept;

    void define_engine_gamestate_object_types(lua_State *state) noexcept;
}

#endif
