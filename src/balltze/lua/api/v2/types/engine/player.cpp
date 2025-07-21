// SPDX-License-Identifier: GPL-3.0-only

#include "../../../../libraries/luastruct.hpp"
#include "types.hpp"
#include "player.hpp"

namespace Balltze::Lua::Api::V2 {
    static void define_player_handle_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, PlayerHandle);
        LUAS_PRIMITIVE_FIELD(state, PlayerHandle, value, LUAST_UINT32, 0);
        LUAS_PRIMITIVE_FIELD(state, PlayerHandle, id, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, PlayerHandle, index, LUAST_INT16, 0);
        LUAS_METHOD_FIELD(state, PlayerHandle, "isNull", [](lua_State *state) noexcept {
            auto handle = get_table_resource_handle(state, 1);
            if(!handle.has_value()) {
                return luaL_error(state, "Invalid TableResourceHandle object.");
            }
            lua_pushboolean(state, HANDLE_IS_NULL(handle.value()));
            return 1;
        });
        lua_pop(state, 1);
    }

    void push_player_handle(lua_State *state, const PlayerHandle &handle, bool readonly) noexcept {
        LUAS_NEW_OBJECT(state, PlayerHandle, readonly);
        PlayerHandle *player_handle = LUAS_CHECK_OBJECT(state, -1, PlayerHandle);
        player_handle->value = handle.value;
    }

    std::optional<PlayerHandle> get_player_handle(lua_State *state, int idx) noexcept {
        std::optional<PlayerHandle> handle;
        if(lua_istable(state, idx) || lua_isuserdata(state, idx)) {
            lua_getfield(state, idx, "value");
            if(lua_isinteger(state, -1)) {
                handle = PlayerHandle();
                handle->value = luaL_checkinteger(state, -1);
            }
            lua_pop(state, 1);
        } 
        else if(lua_isinteger(state, idx)) {
            handle = PlayerHandle();
            handle->value = luaL_checkinteger(state, idx);
        }
        return handle;
    }

    static void define_engine_player_globals_type(lua_State *state) {
        LUAS_STRUCT(state, PlayersGlobals);
        LUAS_PRIMITIVE_FIELD(state, PlayersGlobals, local_player_network_id, LUAST_INT32, 0);
        LUAS_OBJREF_ARRAY_FIELD(state, PlayersGlobals, local_players, PlayerHandle, 0);
        LUAS_OBJREF_ARRAY_FIELD(state, PlayersGlobals, local_player_dead_units, ObjectHandle, 0);
        LUAS_PRIMITIVE_FIELD(state, PlayersGlobals, local_player_count, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, PlayersGlobals, double_speed_ticks_remaining, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, PlayersGlobals, are_all_dead, LUAST_BOOL, 0);
        LUAS_PRIMITIVE_FIELD(state, PlayersGlobals, input_disabled, LUAST_BOOL, 0);
        LUAS_PRIMITIVE_FIELD(state, PlayersGlobals, bsp_switch_trigger_index, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, PlayersGlobals, respawn_failure, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, PlayersGlobals, was_teleported, LUAST_BOOL, 0);
        lua_pop(state, 1);
    }

    static void define_engine_player_control_type(lua_State *state) {
        LUAS_STRUCT(state, PlayerControl);
        LUAS_OBJREF_FIELD(state, PlayerControl, unit_handle, ObjectHandle, 0);
        LUAS_PRIMITIVE_FIELD(state, PlayerControl, control_flags, LUAST_UINT32, 0);
        LUAS_OBJREF_FIELD(state, PlayerControl, desired_angles, VectorPY, 0);
        LUAS_OBJREF_FIELD(state, PlayerControl, throttle, VectorIJ, 0);
        LUAS_PRIMITIVE_FIELD(state, PlayerControl, primary_trigger, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, PlayerControl, weapon_index, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, PlayerControl, grenade_index, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, PlayerControl, zoom_level, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, PlayerControl, weapon_swap_ticks, LUAST_UINT8, 0);
        LUAS_OBJREF_FIELD(state, PlayerControl, target_object_index, ObjectHandle, 0);
        LUAS_PRIMITIVE_FIELD(state, PlayerControl, autoaim_level, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, PlayerControl, magnetism_level, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, PlayerControl, look_acceleration_time, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, PlayerControl, pitch_minimum, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, PlayerControl, pitch_maximum, LUAST_FLOAT, 0);
        lua_pop(state, 1);
    }

    static void define_engine_player_control_globals_type(lua_State *state) {
        LUAS_STRUCT(state, PlayerControlGlobals);
        LUAS_PRIMITIVE_FIELD(state, PlayerControlGlobals, action_test_flags, LUAST_UINT32, 0);
        LUAS_PRIMITIVE_FIELD(state, PlayerControlGlobals, testing_for_action_flags, LUAST_UINT32, 0);
        LUAS_PRIMITIVE_FIELD(state, PlayerControlGlobals, disabled_button_flags, LUAST_UINT32, 0);
        LUAS_PRIMITIVE_FIELD(state, PlayerControlGlobals, control_flags, LUAST_UINT32, 0);
        LUAS_OBJREF_ARRAY_FIELD(state, PlayerControlGlobals, local_players, PlayerControl, 0);
        lua_pop(state, 1);
    }

    static void define_engine_player_multiplayer_statistics_type(lua_State *state) {
        LUAS_STRUCT(state, PlayerMultiplayerStatistics);
        luastruct_new_struct_field(state, "ctf_flag_grabs", LUAST_INT16, NULL, offsetof(PlayerMultiplayerStatistics, ctf.flag_grabs), false, false);
        luastruct_new_struct_field(state, "ctf_flag_returns", LUAST_INT16, NULL, offsetof(PlayerMultiplayerStatistics, ctf.flag_returns), false, false);
        luastruct_new_struct_field(state, "ctf_flag_scores", LUAST_INT16, NULL, offsetof(PlayerMultiplayerStatistics, ctf.flag_scores), false, false);
        luastruct_new_struct_field(state, "oddball_time", LUAST_INT16, NULL, offsetof(PlayerMultiplayerStatistics, oddball.time), false, false);
        luastruct_new_struct_field(state, "oddball_carrier_kills", LUAST_INT16, NULL, offsetof(PlayerMultiplayerStatistics, oddball.carrier_kills), false, false);
        luastruct_new_struct_field(state, "oddball_kills", LUAST_INT16, NULL, offsetof(PlayerMultiplayerStatistics, oddball.kills), false, false);
        luastruct_new_struct_field(state, "king_hill_score", LUAST_INT16, NULL, offsetof(PlayerMultiplayerStatistics, king.hill_score), false, false);
        luastruct_new_struct_field(state, "race_time", LUAST_INT16, NULL, offsetof(PlayerMultiplayerStatistics, race.time), false, false);
        luastruct_new_struct_field(state, "race_laps", LUAST_INT16, NULL, offsetof(PlayerMultiplayerStatistics, race.laps), false, false);
        luastruct_new_struct_field(state, "race_best_time", LUAST_INT16, NULL, offsetof(PlayerMultiplayerStatistics, race.best_time), false, false);
        lua_pop(state, 1);
    }

    static void define_engine_network_player_type(lua_State *state) {
        LUAS_STRUCT(state, NetworkPlayer);
        LUAS_PRIMITIVE_ARRAY_FIELD(state, NetworkPlayer, name, LUAST_CHAR, LUAS_FIELD_READONLY);
        LUAS_PRIMITIVE_FIELD(state, NetworkPlayer, color_index, LUAST_UINT16, 0);
        LUAS_PRIMITIVE_FIELD(state, NetworkPlayer, icon_index, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, NetworkPlayer, machine_index, LUAST_UINT8, 0);
        LUAS_PRIMITIVE_FIELD(state, NetworkPlayer, controller_index, LUAST_UINT8, 0);
        LUAS_PRIMITIVE_FIELD(state, NetworkPlayer, team_index, LUAST_UINT8, 0);
        LUAS_PRIMITIVE_FIELD(state, NetworkPlayer, index, LUAST_UINT8, 0);
        lua_pop(state, 1);
    }

    static void define_engine_multiplayer_player_info_type(lua_State *state) {
        LUAS_STRUCT(state, MultiplayerPlayerInfo);
        LUAS_PRIMITIVE_FIELD(state, MultiplayerPlayerInfo, speed_multiplier, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, MultiplayerPlayerInfo, teleporter_index, LUAST_INT32, 0);
        LUAS_PRIMITIVE_FIELD(state, MultiplayerPlayerInfo, objective_mode, LUAST_INT32, 0);
        LUAS_OBJREF_FIELD(state, MultiplayerPlayerInfo, objective_player_handle, TableResourceHandle, 0);
        LUAS_PRIMITIVE_FIELD(state, MultiplayerPlayerInfo, target_player, LUAST_INT32, 0);
        LUAS_PRIMITIVE_FIELD(state, MultiplayerPlayerInfo, target_time, LUAST_UINT32, 0);
        LUAS_PRIMITIVE_FIELD(state, MultiplayerPlayerInfo, last_death_time, LUAST_UINT32, 0);
        LUAS_PRIMITIVE_FIELD(state, MultiplayerPlayerInfo, slayer_target, LUAST_UINT32, 0);
        LUAS_PRIMITIVE_FIELD(state, MultiplayerPlayerInfo, odd_man_out, LUAST_BOOL, 0);
        lua_pop(state, 1);
    }

    static void define_engine_player_actions_type(lua_State *state) {
        LUAS_STRUCT(state, PlayerActions);
        luastruct_new_struct_bit_field(state, "melee", LUAST_UINT16, 0, 0, false, false);
        luastruct_new_struct_bit_field(state, "action", LUAST_UINT16, 0, 1, false, false);
        luastruct_new_struct_bit_field(state, "flashlight", LUAST_UINT16, 0, 3, false, false);
        luastruct_new_struct_bit_field(state, "reload", LUAST_UINT16, 0, 13, false, false);
        lua_pop(state, 1);
    }

    static void define_engine_player_type(lua_State *state) {
        LUAS_STRUCT(state, Player);
        LUAS_PRIMITIVE_FIELD(state, Player, player_id, LUAST_UINT16, 0);
        LUAS_PRIMITIVE_FIELD(state, Player, local_player_index, LUAST_INT16, 0);
        LUAS_PRIMITIVE_ARRAY_FIELD(state, Player, name, LUAST_CHAR, LUAS_FIELD_READONLY);
        LUAS_PRIMITIVE_FIELD(state, Player, squad_index, LUAST_UINT32, 0);
        LUAS_PRIMITIVE_FIELD(state, Player, team_index, LUAST_UINT32, 0);
        LUAS_OBJREF_FIELD(state, Player, interaction_object_handle, TableResourceHandle, 0);
        LUAS_PRIMITIVE_FIELD(state, Player, interaction_object_type, LUAST_UINT16, 0);
        LUAS_PRIMITIVE_FIELD(state, Player, interaction_object_seat, LUAST_UINT16, 0);
        LUAS_PRIMITIVE_FIELD(state, Player, respawn_time, LUAST_UINT32, 0);
        LUAS_PRIMITIVE_FIELD(state, Player, respawn_time_penalty, LUAST_UINT32, 0);
        LUAS_OBJREF_FIELD(state, Player, unit_handle, TableResourceHandle, 0);
        LUAS_OBJREF_FIELD(state, Player, dead_unit_handle, TableResourceHandle, 0);
        LUAS_PRIMITIVE_FIELD(state, Player, bsp_cluster_index, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, Player, weapon_swap_result, LUAST_BOOL, 0);
        LUAS_OBJREF_FIELD(state, Player, auto_aim_target_object, TableResourceHandle, 0);
        LUAS_PRIMITIVE_FIELD(state, Player, last_fire_time, LUAST_UINT32, 0);
        LUAS_OBJREF_FIELD(state, Player, network_player_data, NetworkPlayer, 0);
        LUAS_PRIMITIVE_ARRAY_FIELD(state, Player, power_up_durations, LUAST_UINT16, 2);
        LUAS_OBJREF_FIELD(state, Player, multiplayer_info, MultiplayerPlayerInfo, 0);
        LUAS_PRIMITIVE_FIELD(state, Player, kill_streak, LUAST_UINT32, 0);
        LUAS_PRIMITIVE_FIELD(state, Player, multikill, LUAST_UINT32, 0);
        LUAS_PRIMITIVE_FIELD(state, Player, last_kill_time, LUAST_UINT32, 0);
        LUAS_PRIMITIVE_ARRAY_FIELD(state, Player, kills, LUAST_INT16, 4);
        LUAS_PRIMITIVE_ARRAY_FIELD(state, Player, assists, LUAST_INT16, 4);
        LUAS_PRIMITIVE_FIELD(state, Player, betrays, LUAST_UINT16, 0);
        LUAS_PRIMITIVE_FIELD(state, Player, deaths, LUAST_UINT16, 0);
        LUAS_PRIMITIVE_FIELD(state, Player, suicides, LUAST_UINT16, 0);
        LUAS_PRIMITIVE_FIELD(state, Player, team_kills, LUAST_UINT16, 0);
        LUAS_OBJREF_FIELD(state, Player, multiplayer_statistics, PlayerMultiplayerStatistics, 0);
        LUAS_PRIMITIVE_FIELD(state, Player, telefrag_timer, LUAST_UINT32, 0);
        LUAS_PRIMITIVE_FIELD(state, Player, quit_time, LUAST_UINT32, 0);
        LUAS_PRIMITIVE_FIELD(state, Player, telefrag_danger, LUAST_BOOL, 0);
        LUAS_PRIMITIVE_FIELD(state, Player, quit, LUAST_BOOL, 0);
        LUAS_PRIMITIVE_FIELD(state, Player, ping, LUAST_UINT32, 0);
        LUAS_PRIMITIVE_FIELD(state, Player, team_kill_count, LUAST_UINT32, 0);
        LUAS_PRIMITIVE_FIELD(state, Player, team_kill_ticks_since_last, LUAST_UINT32, 0);
        LUAS_OBJREF_FIELD(state, Player, position, VectorXYZ, 0);
        LUAS_OBJREF_FIELD(state, Player, actions, PlayerActions, 0);
        LUAS_PRIMITIVE_FIELD(state, Player, baseline_update_xy_aim, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, Player, baseline_update_z_aim, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, Player, baseline_update_forward, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, Player, baseline_update_left, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, Player, baseline_update_rate_of_fire, LUAST_FLOAT, 0);
        LUAS_PRIMITIVE_FIELD(state, Player, baseline_update_weapon_slot, LUAST_UINT16, 0);
        LUAS_PRIMITIVE_FIELD(state, Player, baseline_update_grenade_slot, LUAST_UINT16, 0);
        LUAS_OBJREF_FIELD(state, Player, update_aiming, VectorXYZ, 0);
        LUAS_OBJREF_FIELD(state, Player, update_position, VectorXYZ, 0);
        lua_pop(state, 1);
    }

    void push_player(lua_State *state, const Player &player, bool readonly) noexcept {
        LUAS_NEW_OBJECT(state, Player, readonly);
    }

    void define_engine_player_types(lua_State *state) noexcept {
        define_player_handle_type(state);
        define_engine_player_globals_type(state);
        define_engine_player_control_type(state);
        define_engine_player_control_globals_type(state);
        define_engine_player_multiplayer_statistics_type(state);
        define_engine_network_player_type(state);
        define_engine_multiplayer_player_info_type(state);
        define_engine_player_actions_type(state);
        define_engine_player_type(state);
    }
}
