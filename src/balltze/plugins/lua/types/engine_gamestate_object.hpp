// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__PLUGINS__LUA__TYPES__ENGINE_GAMESTATE_OBJECT_HPP
#define BALLTZE__PLUGINS__LUA__TYPES__ENGINE_GAMESTATE_OBJECT_HPP

#include <lua.hpp>
#include <balltze/engine.hpp>
#include "../helpers/luacstruct.hpp"

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

    void lua_define_engine_base_object_attachment_type_enum(lua_State *state) noexcept;
    void lua_define_engine_object_network_role_enum(lua_State *state) noexcept;

    void lua_define_engine_object_flags_struct(lua_State *state) noexcept;
    void lua_push_engine_object_flags(lua_State *state, Engine::BaseObjectFlags *flags) noexcept;

    void lua_define_engine_object_network_struct(lua_State *state) noexcept;
    void lua_push_engine_object_network(lua_State *state, Engine::BaseObjectNetwork *network) noexcept;

    void lua_define_engine_scenario_location_struct(lua_State *state) noexcept;
    void lua_push_engine_scenario_location(lua_State *state, Engine::ScenarioLocation *location) noexcept;

    void lua_define_engine_object_vitals_flags_struct(lua_State *state) noexcept;
    void lua_push_engine_object_vitals_flags(lua_State *state, Engine::BaseObjectVitalsFlags *flags) noexcept;

    void lua_define_engine_object_vitals_struct(lua_State *state) noexcept;
    void lua_push_engine_object_vitals(lua_State *state, Engine::BaseObjectVitals *vitals) noexcept;

    void lua_define_engine_object_attachments_data_struct(lua_State *state) noexcept;
    void lua_push_engine_object_attachments_data(lua_State *state, Engine::BaseObjectAttachmentsData *data) noexcept;

    void lua_define_engine_object_region_destroyeds_struct(lua_State *state) noexcept;
    void lua_push_engine_object_region_destroyeds(lua_State *state, Engine::BaseObjectRegionDestroyeds *destroyeds) noexcept;

    void lua_define_engine_object_block_reference_struct(lua_State *state) noexcept;
    void lua_push_engine_object_block_reference(lua_State *state, Engine::BaseObjectBlockReference *reference) noexcept;

    void lua_define_engine_object_valid_out_going_functions_struct(lua_State *state) noexcept;
    void lua_push_engine_object_valid_out_going_functions(lua_State *state, Engine::ObjectValidOutGoingFunctions *functions) noexcept;

    void lua_define_engine_object_struct(lua_State *state) noexcept;
    void lua_push_engine_object(lua_State *state, Engine::BaseObject *object) noexcept;

    inline void lua_define_engine_gamestate_object_types(lua_State *state) noexcept {
        luacs_newenum(state, EngineBaseObjectAttachmentType);
        lua_pop(state, 1);
        luacs_newenum(state, EngineObjectNetworkRole);
        lua_pop(state, 1);
        luacs_newstruct(state, EngineGameStateObjectFlags); 
        lua_pop(state, 1);
        luacs_newstruct(state, EngineBaseObjectNetwork); 
        lua_pop(state, 1);
        luacs_newstruct(state, EngineScenarioLocation);
        lua_pop(state, 1);
        luacs_newstruct(state, EngineBaseObjectVitalsFlags); 
        lua_pop(state, 1);
        luacs_newstruct(state, EngineBaseObjectVitals);
        lua_pop(state, 1);
        luacs_newstruct(state, EngineBaseObjectAttachmentsData);
        lua_pop(state, 1);
        luacs_newstruct(state, EngineBaseObjectRegionDestroyeds); 
        lua_pop(state, 1);
        luacs_newstruct(state, EngineBaseObjectBlockReference);
        lua_pop(state, 1);
        luacs_newstruct(state, EngineObjectValidOutGoingFunctions); 
        lua_pop(state, 1);
        luacs_newstruct(state, EngineBaseObject);
        lua_pop(state, 1);

        lua_define_engine_base_object_attachment_type_enum(state);
        lua_define_engine_object_network_role_enum(state);
        lua_define_engine_object_flags_struct(state);
        lua_define_engine_object_network_struct(state);
        lua_define_engine_scenario_location_struct(state);
        lua_define_engine_object_vitals_flags_struct(state);
        lua_define_engine_object_vitals_struct(state);
        lua_define_engine_object_attachments_data_struct(state);
        lua_define_engine_object_region_destroyeds_struct(state);
        lua_define_engine_object_block_reference_struct(state);
        lua_define_engine_object_valid_out_going_functions_struct(state);
        lua_define_engine_object_struct(state);
    }
}

#endif
