// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__LUA__API__TYPES__ENGINE_GAMESTATE_ITEM_OBJECT_HPP
#define BALLTZE__LUA__API__TYPES__ENGINE_GAMESTATE_ITEM_OBJECT_HPP

#include <lua.hpp>
#include <balltze/engine.hpp>

namespace Balltze::Lua {
    using EngineItemObject = Engine::ItemObject;
    using EngineGarbageObject = Engine::GarbageObject;
    using EngineWeaponState = Engine::WeaponState;
    using EngineGameStateWeaponTrigger = Engine::WeaponTrigger;
    using EngineGameStateWeaponMagazineState = Engine::WeaponMagazineState;
    using EngineGameStateWeaponMagazine = Engine::WeaponMagazine;
    using EngineWeaponReloadStartData = Engine::WeaponReloadStartData;
    using EngineWeaponNetworkData = Engine::WeaponNetworkData;
    using EngineWeaponNetwork = Engine::WeaponNetwork;
    using EngineWeaponObject = Engine::WeaponObject;
    using EngineEquipmentNetworkData = Engine::EquipmentNetworkData;
    using EngineEquipmentNetwork = Engine::EquipmentNetwork;
    using EngineEquipmentObject = Engine::EquipmentObject;
    using EngineProjectileObjectFlags = Engine::ProjectileObjectFlags;
    using EngineProjectileNetworkData = Engine::ProjectileNetworkData;
    using EngineProjectileNetwork = Engine::ProjectileNetwork;
    using EngineProjectileObject = Engine::ProjectileObject;
    
    void push_engine_item_object(lua_State *state, Engine::ItemObject *item_object) noexcept;

    void push_engine_garbage_object(lua_State *state, Engine::GarbageObject *garbage_object) noexcept;

    void push_engine_weapon_trigger(lua_State *state, Engine::WeaponTrigger *weapon_trigger) noexcept;

    void push_engine_weapon_magazine(lua_State *state, Engine::WeaponMagazine *weapon_magazine) noexcept;

    void push_engine_weapon_reload_start_data(lua_State *state, Engine::WeaponReloadStartData *weapon_reload_start_data) noexcept;

    void push_engine_weapon_network_data(lua_State *state, Engine::WeaponNetworkData *weapon_network_data) noexcept;

    void push_engine_weapon_network(lua_State *state, Engine::WeaponNetwork *weapon_network) noexcept;

    void push_engine_weapon_object(lua_State *state, Engine::WeaponObject *weapon_object) noexcept;

    void push_engine_equipment_network_data(lua_State *state, Engine::EquipmentNetworkData *equipment_network_data) noexcept;

    void push_engine_equipment_network(lua_State *state, Engine::EquipmentNetwork *equipment_network) noexcept;

    void push_engine_equipment_object(lua_State *state, Engine::EquipmentObject *equipment_object) noexcept;

    void push_engine_projectile_object_flags(lua_State *state, Engine::ProjectileObjectFlags *projectile_object_flags) noexcept;

    void push_engine_projectile_network_data(lua_State *state, Engine::ProjectileNetworkData *projectile_network_data) noexcept;

    void push_engine_projectile_network(lua_State *state, Engine::ProjectileNetwork *projectile_network) noexcept;

    void push_engine_projectile_object(lua_State *state, Engine::ProjectileObject *projectile_object) noexcept;

    void define_engine_gamestate_item_object_types(lua_State *state) noexcept;
}

#endif
