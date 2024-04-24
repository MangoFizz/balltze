// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__PLUGINS__LUA__METATABLES__ENGINE_GAMESTATE_OBJECT_HPP
#define BALLTZE__PLUGINS__LUA__METATABLES__ENGINE_GAMESTATE_OBJECT_HPP

#include <string>
#include <balltze/engine.hpp>

namespace Balltze::Plugins {
    void lua_push_meta_engine_object_flags(lua_State *state, Engine::BaseObjectFlags &flags, bool read_only = false) noexcept;
    void lua_push_meta_engine_object_network(lua_State *state, Engine::BaseObjectNetwork &network, bool read_only = false) noexcept;
    void lua_push_meta_engine_scenario_location(lua_State *state, Engine::ScenarioLocation &location, bool read_only = false) noexcept;
    void lua_push_meta_engine_object_vitals_flags(lua_State *state, Engine::BaseObjectVitalsFlags &flags, bool read_only = false) noexcept;
    void lua_push_meta_engine_object_vitals(lua_State *state, Engine::BaseObjectVitals &flags, bool read_only = false) noexcept;
    void lua_push_meta_engine_object_attachments_data(lua_State *state, Engine::BaseObjectAttachmentsData &data, bool read_only = false) noexcept;
    void lua_push_meta_engine_object_region_destroyeds(lua_State *state, Engine::BaseObjectRegionDestroyeds &destroyeds, bool read_only = false) noexcept;
    void lua_push_meta_engine_object_block_reference(lua_State *state, Engine::BaseObjectBlockReference &reference, bool read_only = false) noexcept;
    void lua_push_meta_engine_object(lua_State *state, Engine::BaseObject &object, bool read_only = false) noexcept;
}

#endif
