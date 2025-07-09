// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_LEGACY_API__ENGINE__NETGAME_MESSAGES_HPP
#define BALLTZE_LEGACY_API__ENGINE__NETGAME_MESSAGES_HPP

#include "../../memory.hpp"
#include "data_types.hpp"

namespace Balltze::LegacyApi::Engine {
    enum NetworkGameMessageType : std::int8_t {
        NETWORK_GAME_MESSAGE_TYPE_NONE = -1,
        NETWORK_GAME_MESSAGE_TYPE_OBJECT_DELETION = 0,
        NETWORK_GAME_MESSAGE_TYPE_PROJECTILE_UPDATE,
        NETWORK_GAME_MESSAGE_TYPE_EQUIPMENT_UPDATE,
        NETWORK_GAME_MESSAGE_TYPE_WEAPON_UPDATE,
        NETWORK_GAME_MESSAGE_TYPE_BIPED_UPDATE,
        NETWORK_GAME_MESSAGE_TYPE_VEHICLE_UPDATE,
        NETWORK_GAME_MESSAGE_TYPE_HUD_ADD_ITEM,
        NETWORK_GAME_MESSAGE_TYPE_PLAYER_CREATE,
        NETWORK_GAME_MESSAGE_TYPE_PLAYER_SPAWN,
        NETWORK_GAME_MESSAGE_TYPE_PLAYER_EXIT_VEHICLE,
        NETWORK_GAME_MESSAGE_TYPE_PLAYER_SET_ACTION_RESULT,
        NETWORK_GAME_MESSAGE_TYPE_PLAYER_EFFECT_START,
        NETWORK_GAME_MESSAGE_TYPE_UNIT_KILL,
        NETWORK_GAME_MESSAGE_TYPE_CLIENT_GAME_UPDATE,
        NETWORK_GAME_MESSAGE_TYPE_PLAYER_HANDLE_POWERUP,
        NETWORK_GAME_MESSAGE_TYPE_HUD_CHAT,
        NETWORK_GAME_MESSAGE_TYPE_SLAYER_UPDATE,
        NETWORK_GAME_MESSAGE_TYPE_CTF_UPDATE,
        NETWORK_GAME_MESSAGE_TYPE_ODDBALL_UPDATE,
        NETWORK_GAME_MESSAGE_TYPE_KING_UPDATE,
        NETWORK_GAME_MESSAGE_TYPE_RACE_UPDATE,
        NETWORK_GAME_MESSAGE_TYPE_PLAYER_SCORE_UPDATE,
        NETWORK_GAME_MESSAGE_TYPE_GAME_ENGINE_CHANGE_MODE,
        NETWORK_GAME_MESSAGE_TYPE_GAME_ENGINE_MAP_RESET,
        NETWORK_GAME_MESSAGE_TYPE_MULTIPLAYER_HUD_MESSAGE,
        NETWORK_GAME_MESSAGE_TYPE_MULTIPLAYER_SOUND,
        NETWORK_GAME_MESSAGE_TYPE_TEAM_CHANGE,
        NETWORK_GAME_MESSAGE_TYPE_UNIT_DROP_CURRENT_WEAPON,
        NETWORK_GAME_MESSAGE_TYPE_VEHICLE_NEW,
        NETWORK_GAME_MESSAGE_TYPE_BIPED_NEW,
        NETWORK_GAME_MESSAGE_TYPE_PROJECTILE_NEW,
        NETWORK_GAME_MESSAGE_TYPE_EQUIPMENT_NEW,
        NETWORK_GAME_MESSAGE_TYPE_WEAPON_NEW,
        NETWORK_GAME_MESSAGE_TYPE_GAME_SETTINGS_UPDATE,
        NETWORK_GAME_MESSAGE_TYPE_PARAMETERS_PROTOCOL,
        NETWORK_GAME_MESSAGE_TYPE_LOCAL_PLAYER_UPDATE,
        NETWORK_GAME_MESSAGE_TYPE_LOCAL_PLAYER_VEHICLE_UPDATE,
        NETWORK_GAME_MESSAGE_TYPE_REMOTE_PLAYER_ACTION_UPDATE,
        NETWORK_GAME_MESSAGE_TYPE_SUPER_REMOTE_PLAYERS_ACTION_UPDATE,
        NETWORK_GAME_MESSAGE_TYPE_REMOTE_PLAYER_POSITION_UPDATE,
        NETWORK_GAME_MESSAGE_TYPE_REMOTE_PLAYER_VEHICLE_UPDATE,
        NETWORK_GAME_MESSAGE_TYPE_REMOTE_PLAYER_TOTAL_UPDATE_BIPED,
        NETWORK_GAME_MESSAGE_TYPE_REMOTE_PLAYER_TOTAL_UPDATE_VEHICLE,
        NETWORK_GAME_MESSAGE_TYPE_WEAPON_START_RELOAD,
        NETWORK_GAME_MESSAGE_TYPE_WEAPON_AMMO_PICKUP_MID_RELOAD,
        NETWORK_GAME_MESSAGE_TYPE_WEAPON_FINISH_RELOAD,
        NETWORK_GAME_MESSAGE_TYPE_WEAPON_CANCEL_RELOAD,
        NETWORK_GAME_MESSAGE_TYPE_NETGAME_EQUIPMENT_NEW,
        NETWORK_GAME_MESSAGE_TYPE_PROJECTILE_DETONATE,
        NETWORK_GAME_MESSAGE_TYPE_ITEM_ACCELERATE,
        NETWORK_GAME_MESSAGE_TYPE_DAMAGE_DEALT,
        NETWORK_GAME_MESSAGE_TYPE_PROJECTILE_ATTACH,
        NETWORK_GAME_MESSAGE_TYPE_CLIENT_TO_SERVER_PONG,
        NETWORK_GAME_MESSAGE_TYPE_SUPER_PING_UPDATE,
        NETWORK_GAME_MESSAGE_TYPE_SV_MOTD,
        NETWORK_GAME_MESSAGE_TYPE_RCON_REQUEST,
        NETWORK_GAME_MESSAGE_TYPE_RCON_RESPONSE,
        NETWORK_GAME_MESSAGE_TYPE_CUSTOM_MESSAGE
    };

    enum class NetworkGameMessageMode : std::int32_t {
        STATELESS,
        INCREMENTAL
    };

    namespace NetworkGameMessages {
        enum HudChatType : std::int8_t {
            NONE = -1,
            ALL,
            TEAM,
            VEHICLE,
            CUSTOM = 0x0A
        };
        
        struct PACKED_ATTR ObjectDeletion {
            std::uint32_t network_handle;
        };

        struct PACKED_ATTR ObjectUpdateHeader {
            std::uint32_t network_handle;
            std::int8_t baseline_id;
            std::int8_t message_index;
            bool update_baseline;
            PADDING(1);
        };

        struct PACKED_ATTR ObjectUpdateHeaderTimestamped : public ObjectUpdateHeader {
            std::uint16_t timestamp; // I'm not sure what type this is yet.
        };

        // Encode ObjectUpdateHeader before this
        struct PACKED_ATTR ProjectileUpdate {
            Vector3D position;
            Vector3D translational_velocity;
        };

        // Encode ObjectUpdateHeader before this
        struct PACKED_ATTR EquipmentUpdate {
            Vector3D locality_reference_position;
            Vector3D translational_velocity;
            Vector3D angular_velocity;
        };

        // Encode ObjectUpdateHeader before this
        struct PACKED_ATTR WeaponUpdate {
            Vector3D locality_reference_position;
            Vector3D translational_velocity;
            std::int16_t magazine_0_rounds_total;
            std::int16_t magazine_1_rounds_total;
            float age; // Check if actually a float.
        };

        struct PACKED_ATTR BipedUpdateHeader {
            std::int32_t network_id;
            std::int8_t baseline_id;
            std::int8_t message_index;
            bool update_baseline;
            bool force_shield_update;
        };

        // Encode BipedUpdateHeader before this
        struct PACKED_ATTR BipedUpdate {
            std::int16_t grenade_0_count;
            std::int16_t grenade_1_count;
            float body_vitality;
            float shield_vitality;
            bool shield_stun_ticks_greater_than_zero;
        };

        // Encode ObjectUpdateHeaderTimestamped before this
        struct PACKED_ATTR VehicleUpdate {
            bool at_rest;
            PADDING(3);
            Vector3D position;
            Vector3D translational_velocity;
            Vector3D angular_velocity;
            Vector3D forward;
            Vector3D up;
        };

        struct PACKED_ATTR HudAdditem {
            std::uint32_t item_definition_handle;
            std::int8_t message_offset;
            std::int16_t quantity;
        };

        struct PACKED_ATTR PlayerCreate {
            std::uint8_t player_id;
            PADDING(3);
            std::int32_t raw_player_index;//?
            std::int32_t translated_player_index;
            std::int32_t team_index;
        };

        struct PACKED_ATTR PlayerSpawn {
            std::int32_t translated_player_index;
            std::int32_t network_id; // Object player will control
            std::int32_t team_index;
            std::int32_t parent_vehicle_network_id;
            std::int16_t vehicle_seat_index;
            PADDING(2);
            std::int32_t weapon_object_network_ids[4];
            std::int16_t desired_weapon_index;
            std::int16_t powerup_durations[2];
        };

        // Seems like this could work for ai too.
        struct PACKED_ATTR PlayerExitVehicle {
            std::int32_t network_id;
            bool forced_exit;
        };

        struct PACKED_ATTR PlayerSetActionResult {
            std::uint32_t player_handle;
            std::int32_t action_result_type;
            std::uint32_t action_object_network_handle;
            std::int16_t action_seat_index;
            std::int32_t weapon_to_drop_as_result_of_swap;
        };

        // See what happens if we use the creation of this packet as the trigger to send our velocity override packet.
        struct PACKED_ATTR PlayerEffectStart {
            std::uint32_t damage_definition;
            std::uint32_t damage_owner_object_network_handle;
            std::int32_t damage_flags;
            Vector3D direction;
            float scale;
            float total_damage;
        };

        struct PACKED_ATTR UnitKill {
            std::uint32_t unit_network_handle;
            bool should_cause_ping;
            bool died;
            bool feign_death;
            bool died_flying;
            bool ignore_hard_pings;
            bool force_hard_pings;
            bool should_allignment_vector_be_null;
            PADDING(1);
            std::int16_t damage_part;
            PADDING(2);
            float angle;
            Vector2D allignment_vector;
            std::int32_t respawn_timer;
        };

        struct PACKED_ATTR ClientGameUpdateHeader {
            std::int32_t update_id;
        };

        // Encode ClientGameUpdateHeader first
        struct PACKED_ATTR ClientGameUpdate {
            std::uint32_t ticks_to_apply_update_to[2]; //?
            std::uint32_t control_flags; // figure out if these are the same
            Vector2D desired_facing;
            Vector2D digital_throttle;
            float primary_trigger;
            std::int16_t desired_weapon_id;
            std::int16_t desired_grenade_id;
            std::int16_t desired_zoom_level;
        };

        struct PACKED_ATTR PlayerHandlePowerup {
            std::int8_t player_id;
            PADDING(3);
            std::int16_t powerup_time;
            std::int16_t duration_in_ticks;
        };

        struct PACKED_ATTR HudChat {
            HudChatType msg_type;
            PADDING(3);
            std::int8_t player_id;
            PADDING(3);
            wchar_t *message;
            
            HudChat() = default;
            
            HudChat(HudChatType type, std::int8_t player, wchar_t *text) {
                msg_type = type; 
                player_id = player; 
                message = text;
            }
        };

        struct PACKED_ATTR SlayerScoreArray {
            struct PACKED_ATTR SlayerScore {
                std::int32_t score;
                std::int32_t kills;
                std::int32_t assists;
                std::int32_t deaths;
            } player[16]; 
        };

        struct PACKED_ATTR SlayerUpdate {
            SlayerScoreArray team_score;
            SlayerScoreArray individual_score;
        };

        struct PACKED_ATTR CtfUpdateHeader {
            std::uint32_t flag_swap_timer;
        };

        struct PACKED_ATTR CtfUpdate {
            std::int32_t team_score[2];
            std::int32_t team_with_flag;
        };

        // This one leaves me uncertain.
        struct PACKED_ATTR OddballUpdate {
            PADDING(4);
            std::int32_t team_score[16];
            std::int32_t player_score[16];
            PADDING(64);
            std::int32_t is_holding_ball[16];
        };

        struct PACKED_ATTR KingUpdate {
            PADDING(0x1A4); // king score array?
            std::int32_t hill_id;
        };

        struct PACKED_ATTR RaceUpdate {
            std::int32_t lap_completed_val;
            std::int32_t first_flag[16];
            std::int32_t lap_bit_vector[16];
            std::int32_t rally_flag;
            std::int32_t team_laps;
        };

        struct PACKED_ATTR PlayerScoreUpdateHeader {
            std::int32_t player_id;
        };

        struct PACKED_ATTR PlayerScoreUpdate {
            std::int16_t kills[4];
            std::int16_t assists[4];
            std::int16_t friendly_fire_kills;
            std::int16_t deaths;
            std::int16_t suicides;
            std::int16_t flag_grabs;
            std::int16_t flag_returns;
            std::int16_t flag_scores;
            std::int32_t special;
            bool is_odd_man_out;
            PADDING(3);
            float speed_multiplier;
        };

        struct PACKED_ATTR GameEngineChangeMode {
            std::int32_t mode; // Make an enum later.
        };

        struct PACKED_ATTR GameEngineMapReset {
            PADDING(1);
        };

        struct PACKED_ATTR MultiplayerHudMessage {
            std::int32_t player_id;
            std::int32_t message;
            std::int32_t message_data;
        };

        struct PACKED_ATTR MultiplayerSound {
            std::int32_t id;
        };

        struct PACKED_ATTR TeamChange {
            std::int8_t player_id;
            std::int8_t new_team_id;
        };

        struct PACKED_ATTR UnitDropCurrentWeapon {
            std::int32_t unit_id;
            std::int32_t weapon_id;
            bool immediate;
        };

        struct PACKED_ATTR VehicleNew {
            std::uint32_t tag_handle;
            std::int32_t server_object_id;
            std::int16_t owner_team_id;
            PADDING(2);
            std::int16_t owner_player_id;
            std::int32_t owner_object_id;
            std::int32_t weapon_object_id[4];
            Vector3D position;
            Vector3D forward; // might want to normalize these
            Vector3D up; // might want to normalize these
            Vector3D translational_velocity;
            Vector3D angular_velocity;
            std::int8_t current_baseline_id;
        };

        struct PACKED_ATTR BipedNew {
            std::uint32_t tag_handle;
            std::int32_t server_object_id;
            std::int16_t owner_team_id;
            PADDING(2);
            std::int16_t owner_player_id;
            PADDING(2);
            std::int32_t owner_object_id;
            Vector3D position;
            Vector3D forward; // might want to normalize these
            Vector3D up; // might want to normalize these
            Vector3D translational_velocity;
            ColorRGB change_color[4];
            bool flashlight;
            PADDING(3);
            float light_battery;
            std::int8_t current_baseline_id;
            PADDING(3);
            std::int16_t grenade_counts[2];
            float health;
            float shield;
            bool shield_stun_ticks_greater_than_zero;
        };

        struct PACKED_ATTR ProjectileNew {
            std::uint32_t tag_handle;
            std::int32_t server_object_id;
            std::int16_t owner_team_id;
            PADDING(2);
            std::int16_t owner_player_id;
            PADDING(2);
            std::int32_t owner_object_id;
            Vector3D position;
            Vector3D forward; // might want to normalize these
            Vector3D up; // might want to normalize these
            Vector3D translational_velocity;
            Vector3D angular_velocity;
            std::int8_t current_baseline_id;
        };

        struct PACKED_ATTR EquipmentNew : public ProjectileNew {
        };

        struct PACKED_ATTR WeaponNew {
            std::uint32_t tag_handle;
            std::int32_t server_object_id;
            std::int16_t owner_team_id;
            PADDING(2);
            std::int16_t owner_player_id;
            PADDING(2);
            std::int32_t owner_object_id;
            std::uint32_t object_flags;
            Vector3D position;
            Vector3D forward; // might want to normalize these
            Vector3D up; // might want to normalize these
            Vector3D translational_velocity;
            std::int8_t current_baseline_id;
            PADDING(1);
            std::int16_t magazine_totals[2];
            float age;
            std::int16_t magazine_loadeds[2];
        };

        struct PACKED_ATTR GameSettingsUpdate {
            char name[0x80];
            char map_name[0x84];
            char variant[0xD8];
            std::int8_t unused_game_engine;
            std::int8_t max_players;
            std::int16_t difficulty;
            std::int16_t player_count;
            char players[0x200];
            std::int32_t network_game_random_seed;
            std::int32_t number_of_games_played;
            std::int32_t local_data;
        };

        // struct ParametersControl

        struct PACKED_ATTR LocalPlayerUpdate {
            std::int8_t sequence_number;
            std::int8_t last_completed_update_id;
            PADDING(2);
            Vector3D position;
        };

        struct PACKED_ATTR LocalPlayerVehicleUpdate {
            std::int8_t sequence_number;
            std::int8_t last_completed_update_id;
            PADDING(2);
            std::int32_t vehicle_id;
            Vector3D position;
            Vector3D translational_velocity;
            Vector3D angular_velocity;
            Vector3D forward;
            Vector3D up;
        };

        struct PACKED_ATTR RemotePlayerActionUpdateHeader {
            std::int32_t player_id;
            std::int8_t update_id;
            std::int8_t baseline_id;
        };

        struct PACKED_ATTR RemotePlayerActionUpdate {
            std::uint32_t ticks_to_apply_update_to;
            std::uint32_t control_flags;
            PADDING(8);
            float forward_throttle;
            float sideways_throttle;
            float primary_trigger;
            std::int16_t desired_weapon_id;
            std::int16_t desired_grenade_id;
            PADDING(4);
            Vector3D facing;
        };

        struct PACKED_ATTR RemotePlayerPositionUpdateHeader {
            std::int32_t player_id;
            std::int8_t update_id;
            std::int8_t sequence_number;
            PADDING(2);
        };

        struct PACKED_ATTR RemotePlayerPositionUpdate {
            Vector3D locality_reference_position;
        };

        struct PACKED_ATTR RemotePlayerVehicleUpdate {
            std::int32_t vehicle_id;
            Vector3D position;
            Vector3D translational_velocity;
            Vector3D angular_velocity;
            Vector3D forward;
            Vector3D up;
        };

        struct PACKED_ATTR RemotePlayerTotalUpdateBipedHeader : public RemotePlayerPositionUpdateHeader {
            std::int8_t sequence_number;
        };

        struct PACKED_ATTR RemotePlayerTotalUpdateBiped : public RemotePlayerActionUpdate {
            Vector3D position;
        };

        struct PACKED_ATTR RemotePlayerTotalUpdateVehicle {
            RemotePlayerActionUpdate player;
            RemotePlayerVehicleUpdate vehicle;
        };

        struct PACKED_ATTR WeaponStartReload {
            std::int32_t weapon_id;
            std::int16_t magazine_id;
            std::int16_t total_rounds;
            std::int16_t loaded_rounds;
        };

        struct PACKED_ATTR WeaponAmmoPickupMidReload {
            std::int32_t weapon_id;
            std::int16_t magazine_id;
            std::int16_t rounds_picked_up;
        };

        struct PACKED_ATTR WeaponFinishReload : public WeaponStartReload {
        };

        struct PACKED_ATTR WeaponCancelReload : public WeaponStartReload {
        };

        struct PACKED_ATTR NetgameEquipmentNew {
            std::int32_t object_id;
            std::uint32_t tag_handle;
            std::uint32_t netgame_equipment_tag_handle;
        };

        struct PACKED_ATTR ProjectileDetonate {
            std::int32_t object_id;
            Vector3D position;
        };

        struct PACKED_ATTR ItemAccelerate {
            std::int32_t object_id;
            float magnitude;
            Vector3D direction;
        };

        struct PACKED_ATTR DamageDealt {
            std::int32_t damaged_object_id;
            float shield_damage;
            bool shields_depleted;
        };

        struct PACKED_ATTR ProjectileAttach {
            std::int32_t projectile_object_id;
            std::int32_t parent_object_id;
            std::int16_t parent_node_id;
        };

        struct PACKED_ATTR ClientToServerPong {
            std::int8_t player_id;
        };

        struct PACKED_ATTR SuperPingUpdate {
            std::int8_t player_id;
            PADDING(3);
            std::int32_t player_ping;
        };

        struct PACKED_ATTR SvMotd {
            char text[256]; // Confirm
        };

        struct PACKED_ATTR RconRequest {
            char password[9];
            char command[64];
        };

        struct PACKED_ATTR RconResponse {
            char text[81];
        };

        struct PACKED_ATTR MessageHeader {
            void *decoding_information;
            NetworkGameMessageType type;
        };
    }
}

#endif
