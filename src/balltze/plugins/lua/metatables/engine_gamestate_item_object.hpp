// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__PLUGINS__LUA__METATABLES__ENGINE_GAMESTATE_ITEM_OBJECT_HPP
#define BALLTZE__PLUGINS__LUA__METATABLES__ENGINE_GAMESTATE_ITEM_OBJECT_HPP

#include <string>
#include <balltze/engine.hpp>

namespace Balltze::Plugins {
    void lua_push_meta_engine_item_object(lua_State *state, Engine::ItemObject &object, bool read_only = false) noexcept;
    void lua_push_meta_engine_garbage_object(lua_State *state, Engine::GarbageObject &object, bool read_only = false) noexcept;
    void lua_push_meta_engine_weapon_trigger(lua_State *state, Engine::WeaponTrigger &trigger, bool read_only = false) noexcept;
    void lua_push_meta_engine_weapon_magazine(lua_State *state, Engine::WeaponMagazine &magazine, bool read_only = false) noexcept;
    void lua_push_meta_engine_weapon_reload_start_data(lua_State *state, Engine::WeaponReloadStartData &data, bool read_only = false) noexcept;
    void lua_push_meta_engine_weapon_network_data(lua_State *state, Engine::WeaponNetworkData &data, bool read_only = false) noexcept;
    void lua_push_meta_engine_weapon_network(lua_State *state, Engine::WeaponNetwork &network, bool read_only = false) noexcept;
    void lua_push_meta_engine_weapon_object(lua_State *state, Engine::WeaponObject &object, bool read_only = false) noexcept;
    void lua_push_meta_engine_equipment_network_data(lua_State *state, Engine::EquipmentNetworkData &data, bool read_only = false) noexcept;
    void lua_push_meta_engine_equipment_network(lua_State *state, Engine::EquipmentNetwork &network, bool read_only = false) noexcept;
    void lua_push_meta_engine_equipment_object(lua_State *state, Engine::EquipmentObject &object, bool read_only = false) noexcept;
    void lua_push_meta_engine_projectile_object_flags(lua_State *state, Engine::ProjectileObjectFlags &flags, bool read_only = false) noexcept;
    void lua_push_meta_engine_projectile_network_data(lua_State *state, Engine::ProjectileNetworkData &data, bool read_only = false) noexcept;
    void lua_push_meta_engine_projectile_network(lua_State *state, Engine::ProjectileNetwork &network, bool read_only = false) noexcept;
    void lua_push_meta_engine_projectile_object(lua_State *state, Engine::ProjectileObject &object, bool read_only = false) noexcept;
}

#endif
