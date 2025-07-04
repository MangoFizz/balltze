// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__LUA__API__TYPES__ENGINE_GAMESTATE_ITEM_OBJECT_HPP
#define BALLTZE__LUA__API__TYPES__ENGINE_GAMESTATE_ITEM_OBJECT_HPP

#include <lua.hpp>
#include <balltze/legacy_api/engine.hpp>

namespace Balltze::Lua::Api::V1 {
    using EngineItemObject = LegacyApi::Engine::ItemObject;
    using EngineGarbageObject = LegacyApi::Engine::GarbageObject;
    using EngineWeaponState = LegacyApi::Engine::WeaponState;
    using EngineGameStateWeaponTrigger = LegacyApi::Engine::WeaponTrigger;
    using EngineGameStateWeaponMagazineState = LegacyApi::Engine::WeaponMagazineState;
    using EngineGameStateWeaponMagazine = LegacyApi::Engine::WeaponMagazine;
    using EngineWeaponReloadStartData = LegacyApi::Engine::WeaponReloadStartData;
    using EngineWeaponNetworkData = LegacyApi::Engine::WeaponNetworkData;
    using EngineWeaponNetwork = LegacyApi::Engine::WeaponNetwork;
    using EngineWeaponObject = LegacyApi::Engine::WeaponObject;
    using EngineEquipmentNetworkData = LegacyApi::Engine::EquipmentNetworkData;
    using EngineEquipmentNetwork = LegacyApi::Engine::EquipmentNetwork;
    using EngineEquipmentObject = LegacyApi::Engine::EquipmentObject;
    using EngineProjectileObjectFlags = LegacyApi::Engine::ProjectileObjectFlags;
    using EngineProjectileNetworkData = LegacyApi::Engine::ProjectileNetworkData;
    using EngineProjectileNetwork = LegacyApi::Engine::ProjectileNetwork;
    using EngineProjectileObject = LegacyApi::Engine::ProjectileObject;
    
    void push_engine_item_object(lua_State *state, LegacyApi::Engine::ItemObject *item_object) noexcept;

    void push_engine_garbage_object(lua_State *state, LegacyApi::Engine::GarbageObject *garbage_object) noexcept;

    void push_engine_weapon_trigger(lua_State *state, LegacyApi::Engine::WeaponTrigger *weapon_trigger) noexcept;

    void push_engine_weapon_magazine(lua_State *state, LegacyApi::Engine::WeaponMagazine *weapon_magazine) noexcept;

    void push_engine_weapon_reload_start_data(lua_State *state, LegacyApi::Engine::WeaponReloadStartData *weapon_reload_start_data) noexcept;

    void push_engine_weapon_network_data(lua_State *state, LegacyApi::Engine::WeaponNetworkData *weapon_network_data) noexcept;

    void push_engine_weapon_network(lua_State *state, LegacyApi::Engine::WeaponNetwork *weapon_network) noexcept;

    void push_engine_weapon_object(lua_State *state, LegacyApi::Engine::WeaponObject *weapon_object) noexcept;

    void push_engine_equipment_network_data(lua_State *state, LegacyApi::Engine::EquipmentNetworkData *equipment_network_data) noexcept;

    void push_engine_equipment_network(lua_State *state, LegacyApi::Engine::EquipmentNetwork *equipment_network) noexcept;

    void push_engine_equipment_object(lua_State *state, LegacyApi::Engine::EquipmentObject *equipment_object) noexcept;

    void push_engine_projectile_object_flags(lua_State *state, LegacyApi::Engine::ProjectileObjectFlags *projectile_object_flags) noexcept;

    void push_engine_projectile_network_data(lua_State *state, LegacyApi::Engine::ProjectileNetworkData *projectile_network_data) noexcept;

    void push_engine_projectile_network(lua_State *state, LegacyApi::Engine::ProjectileNetwork *projectile_network) noexcept;

    void push_engine_projectile_object(lua_State *state, LegacyApi::Engine::ProjectileObject *projectile_object) noexcept;

    void define_engine_gamestate_item_object_types(lua_State *state) noexcept;
}

#endif
