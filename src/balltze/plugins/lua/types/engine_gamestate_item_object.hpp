// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__PLUGINS__LUA__TYPES__ENGINE_GAMESTATE_ITEM_OBJECT_HPP
#define BALLTZE__PLUGINS__LUA__TYPES__ENGINE_GAMESTATE_ITEM_OBJECT_HPP

#include <lua.hpp>
#include <balltze/engine.hpp>
#include "../helpers/luacstruct.hpp"

namespace Balltze::Plugins::Lua {
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

    void lua_define_weapon_state_enum(lua_State *state) noexcept;
    void lua_define_weapon_magazine_state_enum(lua_State *state) noexcept;
    
    void lua_define_engine_item_object_struct(lua_State *state) noexcept;
    void lua_push_engine_item_object(lua_State *state, Engine::ItemObject *item_object) noexcept;

    void lua_define_engine_garbage_object_struct(lua_State *state) noexcept;
    void lua_push_engine_garbage_object(lua_State *state, Engine::GarbageObject *garbage_object) noexcept;

    void lua_define_engine_weapon_trigger_struct(lua_State *state) noexcept;
    void lua_push_engine_weapon_trigger(lua_State *state, Engine::WeaponTrigger *weapon_trigger) noexcept;

    void lua_define_engine_weapon_magazine_struct(lua_State *state) noexcept;
    void lua_push_engine_weapon_magazine(lua_State *state, Engine::WeaponMagazine *weapon_magazine) noexcept;

    void lua_define_engine_weapon_reload_start_data_struct(lua_State *state) noexcept;
    void lua_push_engine_weapon_reload_start_data(lua_State *state, Engine::WeaponReloadStartData *weapon_reload_start_data) noexcept;

    void lua_define_engine_weapon_network_data_struct(lua_State *state) noexcept;
    void lua_push_engine_weapon_network_data(lua_State *state, Engine::WeaponNetworkData *weapon_network_data) noexcept;

    void lua_define_engine_weapon_network_struct(lua_State *state) noexcept;
    void lua_push_engine_weapon_network(lua_State *state, Engine::WeaponNetwork *weapon_network) noexcept;

    void lua_define_engine_weapon_object_struct(lua_State *state) noexcept;
    void lua_push_engine_weapon_object(lua_State *state, Engine::WeaponObject *weapon_object) noexcept;

    void lua_define_engine_equipment_network_data_struct(lua_State *state) noexcept;
    void lua_push_engine_equipment_network_data(lua_State *state, Engine::EquipmentNetworkData *equipment_network_data) noexcept;

    void lua_define_engine_equipment_network_struct(lua_State *state) noexcept;
    void lua_push_engine_equipment_network(lua_State *state, Engine::EquipmentNetwork *equipment_network) noexcept;

    void lua_define_engine_equipment_object_struct(lua_State *state) noexcept;
    void lua_push_engine_equipment_object(lua_State *state, Engine::EquipmentObject *equipment_object) noexcept;

    void lua_define_engine_projectile_object_flags_struct(lua_State *state) noexcept;
    void lua_push_engine_projectile_object_flags(lua_State *state, Engine::ProjectileObjectFlags *projectile_object_flags) noexcept;

    void lua_define_engine_projectile_network_data_struct(lua_State *state) noexcept;
    void lua_push_engine_projectile_network_data(lua_State *state, Engine::ProjectileNetworkData *projectile_network_data) noexcept;

    void lua_define_engine_projectile_network_struct(lua_State *state) noexcept;
    void lua_push_engine_projectile_network(lua_State *state, Engine::ProjectileNetwork *projectile_network) noexcept;

    void lua_define_engine_projectile_object_struct(lua_State *state) noexcept;
    void lua_push_engine_projectile_object(lua_State *state, Engine::ProjectileObject *projectile_object) noexcept;

    inline void lua_define_engine_gamestate_item_object_types(lua_State *state) noexcept {
        luacs_newenum(state, EngineWeaponState);
        lua_pop(state, 1);
        luacs_newenum(state, EngineGameStateWeaponMagazineState);
        lua_pop(state, 1);
        luacs_newstruct(state, EngineGameStateWeaponTrigger);
        lua_pop(state, 1);
        luacs_newstruct(state, EngineGameStateWeaponMagazine);
        lua_pop(state, 1);
        luacs_newstruct(state, EngineWeaponReloadStartData);
        lua_pop(state, 1);
        luacs_newstruct(state, EngineWeaponNetworkData);
        lua_pop(state, 1);
        luacs_newstruct(state, EngineWeaponNetwork);
        lua_pop(state, 1);
        luacs_newstruct(state, EngineEquipmentNetworkData);
        lua_pop(state, 1);
        luacs_newstruct(state, EngineEquipmentNetwork);
        lua_pop(state, 1);
        luacs_newstruct(state, EngineProjectileObjectFlags); 
        lua_pop(state, 1);
        luacs_newstruct(state, EngineProjectileNetworkData);
        lua_pop(state, 1);
        luacs_newstruct(state, EngineProjectileNetwork);
        lua_pop(state, 1);
        luacs_newderivedstruct(state, EngineItemObject, EngineBaseObject);
        lua_pop(state, 1);
        luacs_newderivedstruct(state, EngineGarbageObject, EngineItemObject);
        lua_pop(state, 1);
        luacs_newderivedstruct(state, EngineWeaponObject, EngineItemObject);
        lua_pop(state, 1);
        luacs_newderivedstruct(state, EngineEquipmentObject, EngineItemObject);
        lua_pop(state, 1);
        luacs_newderivedstruct(state, EngineProjectileObject, EngineItemObject);
        lua_pop(state, 1);

        lua_define_weapon_state_enum(state);
        lua_define_weapon_magazine_state_enum(state);

        lua_define_engine_item_object_struct(state);
        lua_define_engine_garbage_object_struct(state);
        lua_define_engine_weapon_trigger_struct(state);
        lua_define_engine_weapon_magazine_struct(state);
        lua_define_engine_weapon_reload_start_data_struct(state);
        lua_define_engine_weapon_network_data_struct(state);
        lua_define_engine_weapon_network_struct(state);
        lua_define_engine_weapon_object_struct(state);
        lua_define_engine_equipment_network_data_struct(state);
        lua_define_engine_equipment_network_struct(state);
        lua_define_engine_equipment_object_struct(state);
        lua_define_engine_projectile_object_flags_struct(state);
        lua_define_engine_projectile_network_data_struct(state);
        lua_define_engine_projectile_network_struct(state);
        lua_define_engine_projectile_object_struct(state);
    }
}

#endif
