// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/engine.hpp>
#include <lua.hpp>
#include "../helpers.hpp"
#include "../metatables.hpp"

namespace Balltze::Plugins {
    static int lua_engine_player_multiplayer_statistics__index(lua_State *state) {
        auto *stats = lua_from_meta_object<Engine::PlayerMultiplayerStatistics>(state, 1); 
        auto *key = lua_tostring(state, 2);
        
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        }
        
        std::string field = key;
        if(field == "ctfFlagGrabs") {
            lua_pushinteger(state, stats->ctf.flag_grabs);
        }
        else if(field == "ctfFlagReturns") {
            lua_pushinteger(state, stats->ctf.flag_returns);
        }
        else if(field == "ctfFlagScores") {
            lua_pushinteger(state, stats->ctf.flag_scores);
        }
        else if(field == "oddballTargetKills") {
            lua_pushinteger(state, stats->oddball.target_kills);
        }
        else if(field == "oddballKills") {
            lua_pushinteger(state, stats->oddball.kills);
        }
        else if(field == "oddballUnknown") {
            lua_pushinteger(state, stats->oddball.unknown);
        }
        else if(field == "kingHillScore") {
            lua_pushinteger(state, stats->king.hill_score);
        }
        else if(field == "raceTime") {
            lua_pushinteger(state, stats->race.time);
        }
        else if(field == "raceLaps") {
            lua_pushinteger(state, stats->race.laps);
        }
        else if(field == "raceBestTime") {
            lua_pushinteger(state, stats->race.best_time);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_player_multiplayer_statistics__newindex(lua_State *state) {
        auto *stats = lua_from_meta_object<Engine::PlayerMultiplayerStatistics>(state, 1); 
        auto *key = lua_tostring(state, 2);
        
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        }
        
        std::string field = key;
        if(field == "ctfFlagGrabs") {
            stats->ctf.flag_grabs = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "ctfFlagReturns") {
            stats->ctf.flag_returns = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "ctfFlagScores") {
            stats->ctf.flag_scores = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "oddballTargetKills") {
            stats->oddball.target_kills = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "oddballKills") {
            stats->oddball.kills = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "oddballUnknown") {
            stats->oddball.unknown = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "kingHillScore") {
            stats->king.hill_score = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "raceTime") {
            stats->race.time = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "raceLaps") {
            stats->race.laps = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "raceBestTime") {
            stats->race.best_time = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_player_multiplayer_statistics(lua_State *state, Engine::PlayerMultiplayerStatistics &stats, bool read_only) noexcept {
        lua_push_meta_object(state, &stats, lua_engine_player_multiplayer_statistics__index, lua_engine_player_multiplayer_statistics__newindex, read_only);
    }

    static int lua_engine_player__index(lua_State *state) noexcept {
        auto *player = lua_from_meta_object<Engine::Player>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        }
        
        std::string field = key;
        if(field == "playerId") {
            lua_pushinteger(state, player->player_id);
        }
        else if(field == "localHandle") {
            lua_pushinteger(state, player->local_handle);
        }
        else if(field == "name") {
            lua_pushstring(state, reinterpret_cast<const char *>(player->name));
        }
        else if(field == "unknownHandle") {
            lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&player->unknown_handle));
        }
        else if(field == "team") {
            lua_pushinteger(state, player->team);
        }
        else if(field == "interactionObjectHandle") {
            lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&player->interaction_object_handle));
        }
        else if(field == "interactionObjectType") {
            lua_pushinteger(state, player->interaction_object_type);
        }
        else if(field == "interactionObjectSeat") {
            lua_pushinteger(state, player->interaction_object_seat);
        }
        else if(field == "respawnTime") {
            lua_pushinteger(state, player->respawn_time);
        }
        else if(field == "respawnTimeGrowth") {
            lua_pushinteger(state, player->respawn_time_growth);
        }
        else if(field == "objectHandle") {
            lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&player->object_handle));
        }
        else if(field == "prevObjectHandle") {
            lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&player->prev_object_handle));
        }
        else if(field == "bspClusterId") {
            lua_pushinteger(state, player->bsp_cluster_id);
        }
        else if(field == "weaponSwapResult") {
            lua_pushboolean(state, player->weapon_swap_result);
        }
        else if(field == "autoAimTargetObject") {
            lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&player->auto_aim_target_object));
        }
        else if(field == "lastFireTime") {
            lua_pushinteger(state, player->last_fire_time);
        }
        else if(field == "name2") {
            lua_pushstring(state, reinterpret_cast<const char *>(player->name2));
        }
        else if(field == "color") {
            lua_pushstring(state, network_color_to_string(player->color).c_str());
        }
        else if(field == "iconIndex") {
            lua_pushinteger(state, player->icon_index);
        }
        else if(field == "machineIndex") {
            lua_pushinteger(state, player->machine_index);
        }
        else if(field == "controllerIndex") {
            lua_pushinteger(state, player->controller_index);
        }
        else if(field == "team2") {
            lua_pushinteger(state, player->team2);
        }
        else if(field == "index") {
            lua_pushinteger(state, player->index);
        }
        else if(field == "invisibilityTime") {
            lua_pushinteger(state, player->invisibility_time);
        }
        else if(field == "otherPowerupTimeLeft") {
            lua_pushinteger(state, player->other_powerup_time_left);
        }
        else if(field == "speed") {
            lua_pushnumber(state, player->speed);
        }
        else if(field == "teleporterFlagHandle") {
            lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&player->teleporter_flag_handle));
        }
        else if(field == "objectiveMode") {
            lua_pushstring(state, player_objective_mode_to_string(player->objective_mode).c_str());
        }
        else if(field == "objectivePlayerHandle") {
            lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&player->objective_player_handle));
        }
        else if(field == "targetPlayer") {
            lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&player->target_player));
        }
        else if(field == "targetTime") {
            lua_pushinteger(state, player->target_time);
        }
        else if(field == "lastDeathTime") {
            lua_pushinteger(state, player->last_death_time);
        }
        else if(field == "slayerTarget") {
            lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&player->slayer_target));
        }
        else if(field == "oddManOut") {
            lua_pushboolean(state, player->odd_man_out);
        }
        else if(field == "killStreak") {
            lua_pushinteger(state, player->kill_streak);
        }
        else if(field == "multikill") {
            lua_pushinteger(state, player->multikill);
        }
        else if(field == "lastKillTime") {
            lua_pushinteger(state, player->last_kill_time);
        }
        else if(field == "kills") {
            lua_newtable(state);
            for(std::size_t i = 0; i < sizeof(player->kills) / sizeof(player->kills[0]); i++) {
                lua_pushinteger(state, player->kills[i]);
                lua_rawseti(state, -2, i + 1);
            }
        }
        else if(field == "assists") {
            lua_newtable(state);
            for(std::size_t i = 0; i < sizeof(player->assists) / sizeof(player->assists[0]); i++) {
                lua_pushinteger(state, player->assists[i]);
                lua_rawseti(state, -2, i + 1);
            }
        }
        else if(field == "betrays") {
            lua_pushinteger(state, player->betrays);
        }
        else if(field == "deaths") {
            lua_pushinteger(state, player->deaths);
        }
        else if(field == "suicides") {
            lua_pushinteger(state, player->suicides);
        }
        else if(field == "teamKills") {
            lua_pushinteger(state, player->team_kills);
        }
        else if(field == "multiplayerStatistics") {
            lua_push_meta_engine_player_multiplayer_statistics(state, player->multiplayer_statistics);
        }
        else if(field == "telefragTimer") {
            lua_pushinteger(state, player->telefrag_timer);
        }
        else if(field == "quitTime") {
            lua_pushinteger(state, player->quit_time);
        }
        else if(field == "telefragDanger") {
            lua_pushboolean(state, player->telefrag_danger);
        }
        else if(field == "quit") {
            lua_pushboolean(state, player->quit);
        }
        else if(field == "ping") {
            lua_pushinteger(state, player->ping);
        }
        else if(field == "teamKillCount") {
            lua_pushinteger(state, player->team_kill_count);
        }
        else if(field == "teamKillTicksSinceLast") {
            lua_pushinteger(state, player->team_kill_ticks_since_last);
        }
        else if(field == "position") {
            lua_push_meta_engine_point3_d(state, player->position);
        }
        else if(field == "melee") {
            lua_pushboolean(state, player->melee == 1);
        }
        else if(field == "action") {
            lua_pushboolean(state, player->action == 1);
        }
        else if(field == "flashlight") {
            lua_pushboolean(state, player->flashlight == 1);
        }
        else if(field == "reload") {
            lua_pushboolean(state, player->reload == 1);
        }
        else if(field == "baselineUpdateXYAim") {
            lua_pushnumber(state, player->baseline_update_xy_aim);
        }
        else if(field == "baselineUpdateZAim") {
            lua_pushnumber(state, player->baseline_update_z_aim);
        }
        else if(field == "baselineUpdateForward") {
            lua_pushnumber(state, player->baseline_update_forward);
        }
        else if(field == "baselineUpdateLeft") {
            lua_pushnumber(state, player->baseline_update_left);
        }
        else if(field == "baselineUpdateRateOfFire") {
            lua_pushnumber(state, player->baseline_update_rate_of_fire);
        }
        else if(field == "baselineUpdateWeaponSlot") {
            lua_pushinteger(state, player->baseline_update_weapon_slot);
        }
        else if(field == "baselineUpdateGrenadeSlot") {
            lua_pushinteger(state, player->baseline_update_grenade_slot);
        }
        else if(field == "updateAiming") {
            lua_push_meta_engine_point3_d(state, player->update_aiming);
        }
        else if(field == "updatePosition") {
            lua_push_meta_engine_point3_d(state, player->update_position);
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 1;
    }

    static int lua_engine_player__newindex(lua_State *state) noexcept {
        auto *player = lua_from_meta_object<Engine::Player>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr || !lua_istable(state, 3)) {  
            return luaL_error(state, "Invalid key type"); 
        }
        
        std::string field = key;
        if(field == "playerId") {
            player->player_id = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "localHandle") {
            player->local_handle = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "name") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "unknownHandle") {
            player->unknown_handle.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "team") {
            player->team = static_cast<std::uint8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "interactionObjectHandle") {
            player->interaction_object_handle.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "interactionObjectType") {
            player->interaction_object_type = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "interactionObjectSeat") {
            player->interaction_object_seat = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "respawnTime") {
            player->respawn_time = static_cast<std::uint32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "respawnTimeGrowth") {
            player->respawn_time_growth = static_cast<std::uint32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "objectHandle") {
            player->object_handle.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "prevObjectHandle") {
            player->prev_object_handle.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "bspClusterId") {
            player->bsp_cluster_id = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "weaponSwapResult") {
            if(lua_isboolean(state, 3)) {
                player->weapon_swap_result = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "autoAimTargetObject") {
            player->auto_aim_target_object.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "lastFireTime") {
            player->last_fire_time = static_cast<std::uint32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "name2") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "color") {
            try {
                player->color = network_color_from_string(luaL_checkstring(state, 3));
            }
            catch(const std::exception &exception) {
                return luaL_error(state, exception.what());
            }
        }
        else if(field == "iconIndex") {
            player->icon_index = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "machineIndex") {
            player->machine_index = static_cast<std::uint8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "controllerIndex") {
            player->controller_index = static_cast<std::uint8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "team2") {
            player->team2 = static_cast<std::uint8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "index") {
            player->index = static_cast<std::uint8_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "invisibilityTime") {
            player->invisibility_time = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "otherPowerupTimeLeft") {
            player->other_powerup_time_left = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "speed") {
            player->speed = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "teleporterFlagHandle") {
            player->teleporter_flag_handle.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "objectiveMode") {
            try {
                player->objective_mode = player_objective_mode_from_string(luaL_checkstring(state, 3));
            }
            catch(const std::exception &exception) {
                return luaL_error(state, exception.what());
            }
        }
        else if(field == "objectivePlayerHandle") {
            player->objective_player_handle.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "targetPlayer") {
            player->target_player.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "targetTime") {
            player->target_time = static_cast<std::uint32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "lastDeathTime") {
            player->last_death_time = static_cast<std::uint32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "slayerTarget") {
            player->slayer_target.handle = luaL_checkinteger(state, 3);
        }
        else if(field == "oddManOut") {
            if(lua_isboolean(state, 3)) {
                player->odd_man_out = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "killStreak") {
            player->kill_streak = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "multikill") {
            player->multikill = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "lastKillTime") {
            player->last_kill_time = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "kills") {
            for(std::size_t i = 0; i < sizeof(player->kills) / sizeof(player->kills[0]); i++) {
                lua_rawgeti(state, 3, i + 1);
                if(!lua_isnil(state, -1)) {
                    player->kills[i] = static_cast<std::int16_t>(luaL_checkinteger(state, -1));
                }
                lua_pop(state, 1);
            }
        }
        else if(field == "assists") {
            for(std::size_t i = 0; i < sizeof(player->assists) / sizeof(player->assists[0]); i++) {
                lua_rawgeti(state, 3, i + 1);
                if(!lua_isnil(state, -1)) {
                    player->assists[i] = static_cast<std::int16_t>(luaL_checkinteger(state, -1));
                }
                lua_pop(state, 1);
            }
        }
        else if(field == "betrays") {
            player->betrays = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "deaths") {
            player->deaths = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "suicides") {
            player->suicides = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "teamKills") {
            player->team_kills = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "multiplayerStatistics") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "telefragTimer") {
            player->telefrag_timer = static_cast<std::uint32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "quitTime") {
            player->quit_time = static_cast<std::uint32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "telefragDanger") {
            if(lua_isboolean(state, 3)) {
                player->telefrag_danger = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "quit") {
            if(lua_isboolean(state, 3)) {
                player->quit = lua_toboolean(state, 3);
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "ping") {
            player->ping = static_cast<std::uint32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "teamKillCount") {
            player->team_kill_count = static_cast<std::uint32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "teamKillTicksSinceLast") {
            player->team_kill_ticks_since_last = static_cast<std::uint32_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "position") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "melee") {
            if(lua_isboolean(state, 3)) {
                player->melee = lua_toboolean(state, 3) ? 1 : 0;
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "action") {
            if(lua_isboolean(state, 3)) {
                player->action = lua_toboolean(state, 3) ? 1 : 0;
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "flashlight") {
            if(lua_isboolean(state, 3)) {
                player->flashlight = lua_toboolean(state, 3) ? 1 : 0;
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "reload") {
            if(lua_isboolean(state, 3)) {
                player->reload = lua_toboolean(state, 3) ? 1 : 0;
            }
            else {
                return luaL_error(state, "Invalid value type");
            }
        }
        else if(field == "baselineUpdateXYAim") {
            player->baseline_update_xy_aim = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "baselineUpdateZAim") {
            player->baseline_update_z_aim = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "baselineUpdateForward") {
            player->baseline_update_forward = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "baselineUpdateLeft") {
            player->baseline_update_left = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "baselineUpdateRateOfFire") {
            player->baseline_update_rate_of_fire = static_cast<float>(luaL_checknumber(state, 3));
        }
        else if(field == "baselineUpdateWeaponSlot") {
            player->baseline_update_weapon_slot = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "baselineUpdateGrenadeSlot") {
            player->baseline_update_grenade_slot = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "updateAiming") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "updatePosition") {
            return luaL_error(state, "Invalid operation");
        }
        else {
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_player(lua_State *state, Engine::Player &player, bool read_only) noexcept {
        lua_push_meta_object(state, &player, lua_engine_player__index, lua_engine_player__newindex, read_only);
    }
}
