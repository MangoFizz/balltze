// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/engine/game_state.hpp>
#include "../../../helpers/bitfield.hpp"
#include "../../../libraries/luacstruct.hpp"
#include "engine_types.hpp"
#include "engine_tag_data.hpp"
#include "engine_gamestate_player.hpp"

namespace Balltze::Lua::Api::V1 {
    static void define_engine_player_multiplayer_statistics_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EnginePlayerMultiplayerStatistics);
        luacs_declare_field(state, LUACS_TINT16, NULL, "ctfFlagGrabs", sizeof(((EnginePlayerMultiplayerStatistics *)0)->ctf.flag_grabs), OFFSET_OF(EnginePlayerMultiplayerStatistics, ctf.flag_grabs), 0, 0);
        luacs_declare_field(state, LUACS_TINT16, NULL, "ctfFlagReturns", sizeof(((EnginePlayerMultiplayerStatistics *)0)->ctf.flag_returns), OFFSET_OF(EnginePlayerMultiplayerStatistics, ctf.flag_returns), 0, 0);
        luacs_declare_field(state, LUACS_TINT16, NULL, "ctfFlagScores", sizeof(((EnginePlayerMultiplayerStatistics *)0)->ctf.flag_scores), OFFSET_OF(EnginePlayerMultiplayerStatistics, ctf.flag_scores), 0, 0);
        luacs_declare_field(state, LUACS_TINT16, NULL, "oddballTargetKills", sizeof(((EnginePlayerMultiplayerStatistics *)0)->oddball.target_kills), OFFSET_OF(EnginePlayerMultiplayerStatistics, oddball.target_kills), 0, 0);
        luacs_declare_field(state, LUACS_TINT16, NULL, "oddballKills", sizeof(((EnginePlayerMultiplayerStatistics *)0)->oddball.kills), OFFSET_OF(EnginePlayerMultiplayerStatistics, oddball.kills), 0, 0);
        luacs_declare_field(state, LUACS_TINT16, NULL, "kothHillScore", sizeof(((EnginePlayerMultiplayerStatistics *)0)->king.hill_score), OFFSET_OF(EnginePlayerMultiplayerStatistics, king.hill_score), 0, 0);
        luacs_declare_field(state, LUACS_TINT16, NULL, "raceTime", sizeof(((EnginePlayerMultiplayerStatistics *)0)->race.time), OFFSET_OF(EnginePlayerMultiplayerStatistics, race.time), 0, 0);
        luacs_declare_field(state, LUACS_TINT16, NULL, "raceLaps", sizeof(((EnginePlayerMultiplayerStatistics *)0)->race.laps), OFFSET_OF(EnginePlayerMultiplayerStatistics, race.laps), 0, 0);
        luacs_declare_field(state, LUACS_TINT16, NULL, "raceBestTime", sizeof(((EnginePlayerMultiplayerStatistics *)0)->race.best_time), OFFSET_OF(EnginePlayerMultiplayerStatistics, race.best_time), 0, 0);
        lua_pop(state, 1);
    }

    void push_meta_engine_player_multiplayer_statistics(lua_State *state, Engine::PlayerMultiplayerStatistics *statistics) noexcept {
        luacs_newobject(state, EnginePlayerMultiplayerStatistics, statistics);
    }

    void define_engine_network_player_color_enum(lua_State *state) noexcept {
        luacs_newenum(state, EngineNetworkPlayerColor);
        luacs_enum_declare_value(state, "WHITE", Engine::NETWORK_COLOR_WHITE);
        luacs_enum_declare_value(state, "BLACK", Engine::NETWORK_COLOR_BLACK);
        luacs_enum_declare_value(state, "RED", Engine::NETWORK_COLOR_RED);
        luacs_enum_declare_value(state, "BLUE", Engine::NETWORK_COLOR_BLUE);
        luacs_enum_declare_value(state, "GRAY", Engine::NETWORK_COLOR_GRAY);
        luacs_enum_declare_value(state, "YELLOW", Engine::NETWORK_COLOR_YELLOW);
        luacs_enum_declare_value(state, "GREEN", Engine::NETWORK_COLOR_GREEN);
        luacs_enum_declare_value(state, "PINK", Engine::NETWORK_COLOR_PINK);
        luacs_enum_declare_value(state, "PURPLE", Engine::NETWORK_COLOR_PURPLE);
        luacs_enum_declare_value(state, "CYAN", Engine::NETWORK_COLOR_CYAN);
        luacs_enum_declare_value(state, "COBALT", Engine::NETWORK_COLOR_COBALT);
        luacs_enum_declare_value(state, "ORANGE", Engine::NETWORK_COLOR_ORANGE);
        luacs_enum_declare_value(state, "TEAL", Engine::NETWORK_COLOR_TEAL);
        luacs_enum_declare_value(state, "SAGE", Engine::NETWORK_COLOR_SAGE);
        luacs_enum_declare_value(state, "BROWN", Engine::NETWORK_COLOR_BROWN);
        luacs_enum_declare_value(state, "TAN", Engine::NETWORK_COLOR_TAN);
        luacs_enum_declare_value(state, "MAROON", Engine::NETWORK_COLOR_MAROON);
        luacs_enum_declare_value(state, "SALMON", Engine::NETWORK_COLOR_SALMON);
        lua_pop(state, 1);
    }

    void lua_define_player_objective_mode_enum(lua_State *state) noexcept {
        luacs_newenum(state, EnginePlayerObjectiveMode);
        luacs_enum_declare_value(state, "RACE", Engine::PLAYER_OBJECTIVE_MODE_RACE);
        luacs_enum_declare_value(state, "HILL", Engine::PLAYER_OBJECTIVE_MODE_HILL);
        luacs_enum_declare_value(state, "JUGGERNAUT", Engine::PLAYER_OBJECTIVE_MODE_JUGGERNAUT);
        luacs_enum_declare_value(state, "BALL", Engine::PLAYER_OBJECTIVE_MODE_BALL);
        luacs_enum_declare_value(state, "NONE", Engine::PLAYER_OBJECTIVE_MODE_NONE);
        lua_pop(state, 1);
    }

    void lua_define_player_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EnginePlayer);
        luacs_unsigned_field(state, EnginePlayer, player_id, 0);
        luacs_unsigned_field(state, EnginePlayer, local_handle, 0);
        luacs_string_field(state, EnginePlayer, name, 0);
        luacs_nested_field(state, EnginePlayer, EngineResourceHandle, unknown_handle, 0);
        luacs_unsigned_field(state, EnginePlayer, team, 0);
        luacs_nested_field(state, EnginePlayer, EngineResourceHandle, interaction_object_handle, 0);
        luacs_unsigned_field(state, EnginePlayer, interaction_object_type, 0);
        luacs_unsigned_field(state, EnginePlayer, interaction_object_seat, 0);
        luacs_unsigned_field(state, EnginePlayer, respawn_time, 0);
        luacs_unsigned_field(state, EnginePlayer, respawn_time_growth, 0);
        luacs_nested_field(state, EnginePlayer, EngineResourceHandle, object_handle, 0);
        luacs_nested_field(state, EnginePlayer, EngineResourceHandle, prev_object_handle, 0);
        luacs_int_field(state, EnginePlayer, bsp_cluster_id, 0);
        luacs_bool_field(state, EnginePlayer, weapon_swap_result, 0);
        luacs_nested_field(state, EnginePlayer, EngineResourceHandle, auto_aim_target_object, 0);
        luacs_unsigned_field(state, EnginePlayer, last_fire_time, 0);
        luacs_string_field(state, EnginePlayer, name2, 0);
        luacs_enum_field(state, EnginePlayer, EngineNetworkPlayerColor, color, 0);
        luacs_int_field(state, EnginePlayer, icon_index, 0);
        luacs_unsigned_field(state, EnginePlayer, machine_index, 0);
        luacs_unsigned_field(state, EnginePlayer, controller_index, 0);
        luacs_unsigned_field(state, EnginePlayer, team2, 0);
        luacs_unsigned_field(state, EnginePlayer, index, 0);
        luacs_unsigned_field(state, EnginePlayer, invisibility_time, 0);
        luacs_int_field(state, EnginePlayer, other_powerup_time_left, 0);
        luacs_float_field(state, EnginePlayer, speed, 0);
        luacs_nested_field(state, EnginePlayer, EngineResourceHandle, teleporter_flag_handle, 0);
        luacs_enum_field(state, EnginePlayer, EnginePlayerObjectiveMode, objective_mode, 0);
        luacs_nested_field(state, EnginePlayer, EngineResourceHandle, objective_player_handle, 0);
        luacs_nested_field(state, EnginePlayer, EngineResourceHandle, target_player, 0);
        luacs_unsigned_field(state, EnginePlayer, target_time, 0);
        luacs_unsigned_field(state, EnginePlayer, last_death_time, 0);
        luacs_nested_field(state, EnginePlayer, EngineResourceHandle, slayer_target, 0);
        luacs_bool_field(state, EnginePlayer, odd_man_out, 0);
        luacs_unsigned_field(state, EnginePlayer, kill_streak, 0);
        luacs_unsigned_field(state, EnginePlayer, multikill, 0);
        luacs_unsigned_field(state, EnginePlayer, last_kill_time, 0);
        luacs_int_array_field(state, EnginePlayer, kills, 0);
        luacs_int_array_field(state, EnginePlayer, assists, 0);
        luacs_unsigned_field(state, EnginePlayer, betrays, 0);
        luacs_unsigned_field(state, EnginePlayer, deaths, 0);
        luacs_unsigned_field(state, EnginePlayer, suicides, 0);
        luacs_unsigned_field(state, EnginePlayer, team_kills, 0);
        luacs_nested_field(state, EnginePlayer, EnginePlayerMultiplayerStatistics, multiplayer_statistics, 0);
        luacs_unsigned_field(state, EnginePlayer, telefrag_timer, 0);
        luacs_unsigned_field(state, EnginePlayer, quit_time, 0);
        luacs_bool_field(state, EnginePlayer, telefrag_danger, 0);
        luacs_bool_field(state, EnginePlayer, quit, 0);
        luacs_unsigned_field(state, EnginePlayer, ping, 0);
        luacs_unsigned_field(state, EnginePlayer, team_kill_count, 0);
        luacs_unsigned_field(state, EnginePlayer, team_kill_ticks_since_last, 0);
        luacs_nested_field(state, EnginePlayer, EnginePoint3D, position, 0);
        luacs_declare_method(state, "melee", lua_bitfield_struct_method(state, EnginePlayer, melee));
        luacs_declare_method(state, "action", lua_bitfield_struct_method(state, EnginePlayer, action));
        luacs_declare_method(state, "flashlight", lua_bitfield_struct_method(state, EnginePlayer, flashlight));
        luacs_declare_method(state, "reload", lua_bitfield_struct_method(state, EnginePlayer, reload));
        luacs_float_field(state, EnginePlayer, baseline_update_xy_aim, 0);
        luacs_float_field(state, EnginePlayer, baseline_update_z_aim, 0);
        luacs_float_field(state, EnginePlayer, baseline_update_forward, 0);
        luacs_float_field(state, EnginePlayer, baseline_update_left, 0);
        luacs_float_field(state, EnginePlayer, baseline_update_rate_of_fire, 0);
        luacs_unsigned_field(state, EnginePlayer, baseline_update_weapon_slot, 0);
        luacs_unsigned_field(state, EnginePlayer, baseline_update_grenade_slot, 0);
        luacs_nested_field(state, EnginePlayer, EnginePoint3D, update_aiming, 0);
        luacs_nested_field(state, EnginePlayer, EnginePoint3D, update_position, 0);
        lua_pop(state, 1); 
    }

    void push_meta_engine_player(lua_State *state, Engine::Player *player) noexcept {
        luacs_newobject(state, EnginePlayer, player);
    }

    void declare_engine_gamestate_player_types(lua_State *state) noexcept {
        define_engine_network_player_color_enum(state);
        lua_define_player_objective_mode_enum(state);
        define_engine_player_multiplayer_statistics_struct(state);
        lua_define_player_struct(state);
    }
}
