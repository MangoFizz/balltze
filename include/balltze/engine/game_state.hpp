// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__ENGINE__GAME_STATE_HPP
#define BALLTZE_API__ENGINE__GAME_STATE_HPP

#include <cstdint>
#include "../memory.hpp"
#include "data_types.hpp"

namespace Balltze::Engine {
    /**
     * This is a table type that is used throughout Halo for many things.
     */
    template<typename T> struct GenericTable {
        /** Name of the table */
        char name[0x20];

        /** Maximum number of elements */
        std::uint16_t max_elements;

        /** Size of an element */
        std::uint16_t element_size;

        PADDING(0x8);
        PADDING(0x2);

        /** Current size of the table, even including gaps */
        std::uint16_t current_size;

        /** Number of valid elements in the table */
        std::uint16_t count;

        /** handle of the next element to be added */
        std::uint16_t next_id;

        /** Pointer to the first element */
        T *first_element;

        /**
         * Get the element from an index
         * @param  index index of the element
         * @return       pointer to the element or nullptr if out of bounds
         */
        T *get_element(std::size_t index) {
            if(index >= this->current_size) {
                return nullptr;
            }
            else {
                return this->first_element + index;
            }
        }
    };
    static_assert(sizeof(GenericTable<int>) == 0x38);

    struct BaseDynamicObject;

    /**
     * This represents and points to an object in loopobjects.
     */
    struct ObjectTableIndexHeader {
        /** Object handle index */
        std::uint16_t id;

        PADDING(0x4);

        /** Address of the object in loopobjects */
        BaseDynamicObject *object;
    };
    static_assert(sizeof(ObjectTableIndexHeader) == 0xC);

    struct ObjectTable : GenericTable<ObjectTableIndexHeader> {
        /**
         * Get the object by its handle, returning nullptr if the handle is invalid.
         * @param  object_handle This is the handle of the object.
         * @return           Return a pointer to the object or nullptr if the handle is invalid.
         */
        BaseDynamicObject *get_dynamic_object(const ObjectHandle &object_handle) noexcept;

        /**
         * Get the object by an index, returning nullptr if the index is invalid.
         * @param  index This is the index of the object.
         * @return       Return a pointer to the object or nullptr if the index is invalid.
         */
        BaseDynamicObject *get_dynamic_object(std::uint32_t index) noexcept;

        /**
         * Spawn an object with an tag id.
         * @param  tag_handle   Tag handle of the object.
         * @param  offset       Offset of the object.
         * @param  parent       Parent object.
         * @return              Handle of the spawned object.
         */
        ObjectHandle create_object(const TagHandle &tag_handle, Point3D offset, const ObjectHandle &parent) noexcept;

        /**
         * Delete an object with an object handle.
         * @param  object_handle The object handle of the object
         */
        void delete_object(const ObjectHandle &object_handle) noexcept;

        /**
         * Delete an object with an index.
         * @param  object_handle The object handle of the object
         */
        void delete_object(std::uint32_t index) noexcept;
    };

    /**
     * Get the object table
     * @return object table
     */
    ObjectTable &get_object_table() noexcept;

    /** This is the type of object an object is. */
    enum ObjectType : std::uint16_t {
        /** Object -> Unit -> Biped */
        OBJECT_TYPE_BIPED = 0,

        /** Object -> Unit -> Vehicle */
        OBJECT_TYPE_VEHICLE,

        /** Object -> Item -> Weapon */
        OBJECT_TYPE_WEAPON,

        /** Object -> Item -> Equipment */
        OBJECT_TYPE_EQUIPMENT,

        /** Object -> Item -> Garbage */
        OBJECT_TYPE_GARBAGE,

        /** Object -> Projectile */
        OBJECT_TYPE_PROJECTILE,

        /** Object -> Scenery */
        OBJECT_TYPE_SCENERY,

        /** Object -> Device -> Device Machine */
        OBJECT_TYPE_DEVICE_MACHINE,

        /** Object -> Device -> Device Control */
        OBJECT_TYPE_DEVICE_CONTROL,

        /** Object -> Device -> Device Light Fixture */
        OBJECT_TYPE_DEVICE_LIGHT_FIXTURE,

        /** Object -> Placeholder */
        OBJECT_TYPE_PLACEHOLDER,

        /** Object -> Sound Scenery */
        OBJECT_TYPE_SOUND_SCENERY
    };

    /** A model node is a part of a model which can have its own position, rotation, and scale. */
    struct ModelNode {
        /** Scale of this part of the model */
        float scale;

        /** Rotation of the model node */
        RotationMatrix rotation;

        /** Position of the model node relative to the world */
        Point3D position;
    };

    /** As of Halo 1.10, 64 nodes is the maximum count. */
    #define MAX_NODES 64

    /**
     * These are objects that are present in an instance of Halo rather than in tag data and have parameters such as location and health.
     */
    struct BaseDynamicObject {
        /** This is the tag handle of the object. */
        TagHandle tag_handle;

        PADDING(0x4);
        PADDING_BIT(std::uint32_t, 8);

        /** If true, force baseline update. */
        std::uint32_t should_force_baseline_update : 1;

        PADDING_BIT(std::uint32_t, 23);

        /** This is the number of ticks the object has existed. */
        TickCount existence_time;

        /** No collision - this is used for backpack weapons. */
        std::uint32_t no_collision : 1;

        /** On the ground */
        std::uint32_t on_ground : 1;

        /** Ignore gravity */
        std::uint32_t ignore_gravity : 1;

        PADDING_BIT(std::uint32_t, 2);

        /** Not moving */
        std::uint32_t stationary : 1;

        PADDING_BIT(std::uint32_t, 1);

        /** No collision? */
        std::uint32_t no_collision2 : 1;

        PADDING_BIT(std::uint32_t, 10);

        /** Cast a shadow if this is off. This is usually off for units, but it can be forced off for other objects. */
        std::uint32_t no_shadow : 1;

        PADDING_BIT(std::uint32_t, 2);

        /** Object is in the void. */
        std::uint32_t outside_of_map : 1;

        PADDING_BIT(std::uint32_t, 10);
        PADDING(0x48);

        /** Coordinates of the object relative to the world or to its parent */
        Point3D position;

        /** Velocity in world units per tick */
        Point3D velocity;

        /** Orientation/rotation */
        Point3D orientation[2];

        /** Rotational velocity of the object in world units per tick */
        Euler3DPYR rotation_velocity;

        /** Unknown */
        std::uint32_t location_id;

        PADDING(0x4);

        /** Position of the object's center used for things such as lens flares, triggers, teleporters, etc. This is always relative to the world. */
        Point3D center_position;

        PADDING(0x4);

        /** Scale of the object */
        float scale;

        /** Type of object */
        ObjectType type;

        PADDING(0x2);
        PADDING(0x14);

        /** Tag handle of the animation tag */
        std::uint32_t animation_tag_id;

        /** Current animation index */
        std::uint16_t animation;

        /** Frame of the current animation */
        std::uint16_t animation_frame;

        PADDING(0x4);

        /** Base health */
        float base_health;

        /** Base shield */
        float base_shield;

        /** Ratio of health remaining */
        float health;

        /** Ratio of shield remaining */
        float shield;

        /** Current shield damage? */
        float current_shield_damage;

        /** Current health damage? */
        float current_health_damage;

        PADDING(0x4);

        /** Recent shield damage taken? */
        float recent_shield_damage;

        /** Recent health damage taken? */
        float recent_health_damage;

        /** Amount of time since shield damage was taken? */
        std::uint32_t recent_shield_damage_time;

        /** Amount of time since health damage was taken? */
        std::uint32_t recent_health_damage_time;

        /** Time in ticks before shields recharge */
        std::uint16_t shield_stun_time;

        PADDING_BIT(std::uint16_t, 11);

        /** Immune to damage and damage side effects */
        std::uint16_t invulnerable : 1;

        /** Shields are recharging */
        std::uint16_t shield_recharging : 1;

        PADDING_BIT(std::uint16_t, 3);

        PADDING(0x10);

        /** Current object handle of the object's current weapon; this may also be the object handle of the object's grenade if throwing */
        ObjectHandle weapon;

        /** Object handle of the object's parent; if set, then the object's position is relative to the parent */
        ObjectHandle parent;

        /** Unknown */
        std::uint16_t parent_seat_index;

        PADDING(0x2);
        PADDING(0x64);

        /** Colors used for things like armor color */
        ColorRGB color_change[4];

        /** Colors used for things like armor color */
        ColorRGB color_change_2[4];

        PADDING(0xC);

        /**
         * Get the full object handle of the object.
         * @return full object handle of the object or a null handle if not available
         */
        ObjectHandle full_object_id() noexcept {
            auto &table = get_object_table();

            ObjectHandle returned_id;
            returned_id.whole_id = 0xFFFFFFFF;

            for(std::size_t i = 0; i < table.current_size; i++) {
                auto &object = table.first_element[i];
                if(object.object == this) {
                    returned_id.whole_id = i + 0x10000 * object.id;
                    return returned_id;
                }
            }

            return returned_id;
        }


        /**
         * Get the object's model nodes or nullptr if this is unknown for this object type.
         * @return a pointer to the object model nodes or nullptr if unknown or not available
         */
        ModelNode *nodes() noexcept {
            static const std::size_t model_node_offset[] = {
                0x550,
                0x5C0,
                0x340,
                0x294,
                0x244,
                0x2B0,
                0x1F8,
                0x228,
                0x21C
            };

            if(this->type >= sizeof(model_node_offset) / sizeof(model_node_offset[0])) {
                return nullptr;
            }

            return reinterpret_cast<ModelNode *>(reinterpret_cast<std::byte *>(this) + model_node_offset[this->type]);
        }
    };
    static_assert(sizeof(BaseDynamicObject) == 0x1F4);

    struct UnitRecentDamager {
        /** Last tick this object damaged this unit */
        TickCount last_damage_time;

        /** Total damage done by this object */
        float total_damage;

        /** ObjectHandle of this object */
        ObjectHandle object;

        /** Owning player handle of this object */
        PlayerHandle player;
    };
    static_assert(sizeof(UnitRecentDamager) == 0x10);

    struct UnitDynamicObject : BaseDynamicObject {
        PADDING(0x10);

        PADDING_BIT(std::uint32_t, 4);

        /** Invisible (camo) */
        std::uint32_t invisible : 1;

        PADDING_BIT(std::uint32_t, 14);

        /** Flashlight is on */
        std::uint32_t flashlight : 1;

        /** Do not drop weapons on death */
        std::uint32_t does_not_drop_items : 1;

        PADDING_BIT(std::uint32_t, 3);

        /** Frozen. burr */
        std::uint32_t suspended : 1;

        PADDING_BIT(std::uint32_t, 7);

        /** Crouching button is pressed */
        std::uint16_t crouch_button : 1;

        /** Jumping button is pressed */
        std::uint16_t jump_button : 1;

        PADDING_BIT(std::uint16_t, 2);

        /** Flashlight button is pressed */
        std::uint16_t flashlight_button : 1;

        PADDING_BIT(std::uint16_t, 1);

        /** Action button is pressed */
        std::uint16_t action_button : 1;

        /** Melee button is pressed */
        std::uint16_t melee_button : 1;

        PADDING_BIT(std::uint16_t, 2);

        /** Is there a meaning of our existence if we only live and die seemingly with no purpose? */
        std::uint16_t reload_button : 1;

        /** Primary fire button is pressed */
        std::uint16_t primary_fire_button : 1;

        /** Secondary fire button is pressed */
        std::uint16_t secondary_fire_button : 1;

        /** Grenade button is pressed */
        std::uint16_t grenade_button : 1;

        /** Exchange weapon button or action button is pressed */
        std::uint16_t exchange_weapon_button : 1;

        PADDING_BIT(std::uint16_t, 1);

        PADDING(0x2);
        PADDING(0xC);

        /** handle of the player owner */
        PlayerHandle player_handle;

        PADDING(0x4);

        /** Last tick the unit fired a round */
        TickCount last_bullet_time;

        /** Direction the unit is facing? */
        Point3D facing;

        /** Where the unit wants to aim? */
        Point3D desired_aim;

        /** The aim? */
        Point3D aim;

        /** What is this? */
        Point3D aim_velocity;

        /** More aim stuff? What? */
        Point3D aim2;

        /** Is this it? */
        Point3D aim3;

        PADDING(0xC);

        /** Analog running, from -1 (full speed backward) to 1 (full speed upward). */
        float run;

        /** Analog strafing, from -1 (full speed to the right) to 1 (full speed to the left) */
        float strafe;

        /** Analog ascension, from -1 (full speed downward) to 1 (full speed upward). */
        float ascend;

        /** Apparently 1 when firing? */
        float shooting;

        PADDING(0xC);

        /** Object handle of the thrown grenade */
        ObjectHandle thrown_grenade_id;

        PADDING(0x8);

        /** 4 when standing, 3 when crouching, and 0 when in a vehicle */
        std::uint8_t crouch_stand_thing;

        /** Current weapon slot */
        std::uint8_t weapon_slot2;

        /** Current weapon type? */
        std::uint8_t weapon_type;

        /** Current animation state of the weapon */
        std::uint8_t animation_state;

        /** 5 when reloading and 7 when meleeing? */
        std::uint8_t reload_melee;

        /** Firing */
        std::uint8_t shooting2;

        /** Animation state of the player's weapon again? */
        std::uint8_t animation_state2;

        /** Crouching... again */
        std::uint8_t crouch2;

        PADDING(0x10);

        /** Top-most aim possible? */
        float aim_rectangle_top_x;

        /** Bottom-most aim possible? */
        float aim_rectangle_bottom_x;

        /** Left-most aim possible? */
        float aim_rectangle_left_y;

        /** Right-most aim possible? */
        float aim_rectangle_right_y;

        /** Top-most aim possible again? */
        float look_rectangle_top_x;

        /** Bottom-most aim possible again? */
        float look_rectangle_bottom_x;

        /** Left-most aim possible again? */
        float look_rectangle_left_y;

        /** Right-most aim possible again? */
        float look_rectangle_right_y;

        PADDING(0x18);

        /** Current seat (0xFFFF if no seat) */
        std::uint16_t vehicle_seat;

        /** Current weapon slot */
        std::uint16_t weapon_slot;

        /** Weapon slot being changed to */
        std::uint16_t next_weapon_slot;

        PADDING(0x2);

        /** Weapons the unit owns in slot order */
        ObjectHandle weapons[4];

        /** Last tick a weapon was used */
        TickCount weapon_last_use[4];

        PADDING(0x4);

        /** Grenade type (0 = fragmentation; 1 = plasma) */
        std::uint8_t current_grenade_type;

        /** Next grenade type */
        std::uint8_t next_grenade_type;

        /** Primary grenade count */
        std::uint8_t primary_grenade_count;

        /** Secondary grenades count */
        std::uint8_t secondary_grenade_count;

        /** Zoom level */
        std::uint8_t zoom_level;

        /** Zoom level being changed to */
        std::uint8_t desired_zoom_level;

        PADDING(2);

        /** Object handle of the controller of this unit */
        ObjectHandle controller;

        /** Object handle of the gunner of this unit */
        ObjectHandle gunner;

        PADDING(0x14);

        /** Intensity of the flashlight */
        float flashlight_intensity;

        /** Remaining battery life of the flashlight */
        float flashlight_battery;

        /* Intensity of the night vision */
        float night_vision_intensity;

        PADDING(0x30);

        /* Invisibility of the unit (1 = full camo; 0 = no camo) */
        float invisibility;

        PADDING(0xB0);

        /** Most recent damaging objects */
        UnitRecentDamager recent_damager[4];

        PADDING(0x5C);
    };
    static_assert(sizeof(UnitDynamicObject) == 0x4CC);

    struct WeaponTriggerState {
        PADDING(0x1);

        /** Unknown */
        std::uint8_t trigger_state;

        /** Unknown */
        std::uint8_t trigger_time;

        PADDING(1);

        /** Unknown */
        std::uint32_t not_firing;

        /** Unknown */
        std::uint32_t auto_reload;

        PADDING(0x2);

        /** Number of rounds since the last tracer */
        std::uint16_t rounds_since_last_tracer;

        /** Rare of fire */
        float rate_of_fire;

        /** Ejection port recovery time */
        float ejection_port_recovery_time;

        /** Illumination recovery time */
        float illumination_recovery_time;

        /** Unknown */
        float error_angle_thingy;

        PADDING(0x8);
    };
    static_assert(sizeof(WeaponTriggerState) == 0x28);

    struct WeaponMagazineState {
        /** Unknown */
        std::uint16_t magazine_state;

        /** Unknown */
        std::uint16_t chambering_time;

        PADDING(0x2);

        /** Total unloaded ammo */
        std::uint16_t ammo;

        /** Total loaded ammo */
        std::uint16_t loaded_ammo;

        PADDING(0x2);

        PADDING(0x4);
    };
    static_assert(sizeof(WeaponMagazineState) == 0x10);

    struct WeaponDynamicObject : BaseDynamicObject {
        PADDING(0x40);

        /** Unknown */
        float primary_trigger;

        PADDING(0x4);

        /** Heat */
        float heat;

        /** Age; 1 = unusable */
        float age;

        /** Illumination */
        float illumination;

        /** Unknown */
        float light_power;

        PADDING(0x14);

        /** Trigger states */
        WeaponTriggerState triggers[2];

        /** Magazine states */
        WeaponMagazineState magazines[2];

    };
    static_assert(sizeof(WeaponDynamicObject) == 0x2D0);

    struct DamageObjectStructThing {
        TagHandle damage_tag_handle;
        std::uint32_t flags = 0;
        PlayerHandle causer_player;
        ObjectHandle causer_object;

        std::uint32_t unknown1 = 0;
        std::uint32_t unknown2 = 0x0000FFFF;
        std::uint32_t unknown3 = 0;
        std::uint32_t unknown4 = 0x0000FFFF;
        std::uint32_t unknown5 = 0x0000FFFF;
        std::byte padding1[0x1C] = {};

        float modifier = 1.0F;
        float multiplier = 1.0F;

        std::byte padding2[0x8] = {};
    };
    static_assert(sizeof(DamageObjectStructThing) == 0x50);

    /**
     * Apply damage to the given object
     * @param object        object to damage
     * @param damage_effect damage effect to apply
     * @param multiplier    multiply damage by this
     * @param causer_player player responsible for the damage
     * @param causer_object object responsible for the damage
     */
    void apply_damage(ObjectHandle object, TagHandle damage_effect, float multiplier = 1.0F, PlayerHandle causer_player = PlayerHandle::null(), ObjectHandle causer_object = ObjectHandle::null()) noexcept;

    struct AntennaVertex {
        Point3D position;
        Point3D velocity;
        float scale;
        std::uint32_t counter;
    };
    static_assert(sizeof(AntennaVertex) == 0x20);

    /**
     * These are players.
     */
    struct Player {
        /** This is some sort of an index of the player. If it's 0xFFFF, the slot is not used. */
        std::uint16_t player_id;

        /** Unknown - I am guessing that this is the local client handle of the player (if valid - otherwise 0xFFFF). */
        std::uint16_t local_handle;

        /** Name of the player */
        wchar_t name[12];

        PADDING(0x4);

        /** Team of the player 0 = red; 1 = blue; etc. */
        std::uint8_t team;

        PADDING(0x3);

        /** Object handle for whatever is being interacted with right now */
        ObjectHandle interaction_object_handle;

        /** Type of object being interacted with right now */
        std::uint16_t interaction_object_type;

        /** If interacting with a vehicle, this is the seat */
        std::uint16_t interaction_object_seat;

        /** Ticks remaining for the player to respawn */
        TickCount respawn_time;

        /** Number of ticks being added to the player's next respawn time */
        TickCount respawn_time_growth;

        /** Player's current object handle. */
        ObjectHandle object_handle;

        /** Player's object handle. Does this have to do with death cam or something? */
        ObjectHandle last_object_handle;

        PADDING(0x4);
        PADDING(0x4);

        /** Last tick the player fired */
        TickCount last_fire_time;

        /** Name of the player (again) */
        wchar_t name2[12];

        /** Color of the player in respect to FFA */
        std::uint16_t color;

        PADDING(0x2);

        /** Machine index of the player. This + 1 = rcon index */
        std::uint8_t machine_index;

        /** Unknown - Is this the controller input being used or something? */
        std::uint8_t controller_index;

        /** Team again? 0 = red; 1 = blue; etc. */
        std::uint8_t team2;

        /** Index of the player */
        std::uint8_t index;

        /** Number of ticks remaining for the player to be invisible. */
        std::uint16_t invisibility_time;

        PADDING(0x2);

        /** Speed multiplier of the player. */
        float speed;

        /** Unknown */
        ElementHandle teleporter_flag_handle;

        /** Unknown */
        std::uint32_t objective_mode;

        /** Unknown */
        PlayerHandle objective_player_handle;

        /** Unknown - Player handle the player is looking at? */
        PlayerHandle target_player;

        /** Unknown - Some timer for fading in the name of the player being looked at? */
        std::uint32_t target_time;

        /** Tick the player last died */
        TickCount last_death_time;

        /** Current slayer target */
        PlayerHandle slayer_target;

        /** Player is odd man out */
        std::uint32_t odd_man_out;

        PADDING(0x6);

        /** Player's current kill streak */
        std::uint16_t kill_streak;

        /** Unknown */
        std::uint16_t multikill;

        /** Last tick a player killed someone */
        std::uint16_t last_kill_time;

        /** Number of kills made by the player */
        std::uint16_t kills;

        PADDING(0x6);

        /** Number of assists made by the player */
        std::uint16_t assists;

        PADDING(0x6);

        /** Number of times player killed a member on the team (including themself) */
        std::uint16_t betrays;

        /** Number of times the player has died. If 0, instantly respawn player. */
        std::uint16_t deaths;

        /** Number of times the player killed themself */
        std::uint16_t suicides;

        PADDING(0xE);

        /** Unknown - Number of times the player betrayed? */
        std::uint16_t team_kills;

        PADDING(0x2);

        /** Unknown - Possibly number of flag steals, hill time (ticks), race time (ticks)? */
        std::uint16_t stat1;

        /** Unknown - Possibly number of flag returns or oddball target kills? */
        std::uint16_t stat2;

        /** Unknown - Possibly number of scores, oddball kills, or race best time (ticks)? */
        std::uint16_t stat3;

        PADDING(0x2);

        /** Time being telefragged in ticks */
        TickCount telefrag_timer;

        /** Tick the player left */
        TickCount quit_time;

        PADDING(0x8);

        /** Current ping of the player in milliseconds */
        std::uint32_t ping;

        /** Number of betrayals */
        std::uint32_t team_kill_count;

        /** Number of ticks since the player's last betrayal */
        TickCount team_kill_timer;

        PADDING(0x10);

        /** Current position */
        Point3D position;

        PADDING(0x18);

        /** Player is meleeing */
        std::uint16_t melee : 1;

        /** Player is actioning */
        std::uint16_t action : 1;

        PADDING_BIT(std::uint16_t, 1);

        /** Player is using a flashlight */
        std::uint16_t flashlight : 1;

        PADDING_BIT(std::uint16_t, 9);

        /** Player is pressing the reload key */
        std::uint16_t reload : 1;

        PADDING_BIT(std::uint16_t, 2);

        PADDING(0x2);
        PADDING(0x18);

        /** XY aim; updated every remote_player_position_baseliine_update_rate ticks */
        float baseline_update_xy_aim;

        /** Aiming; updated every remote_player_position_baseliine_update_rate ticks */
        float baseline_update_z_aim;

        /** Player is moving forward (analog, -1 to 1); updated every remote_player_position_baseliine_update_rate ticks */
        float baseline_update_forward;

        /** Player is moving to the left (analog, -1 to 1); updated every remote_player_position_baseliine_update_rate ticks */
        float baseline_update_left;

        /** Rate of fire (0 = min, 1 = max); updated every remote_player_position_baseliine_update_rate ticks */
        float baseline_update_rate_of_fire;

        /** Current weapon slot; updated every remote_player_position_baseliine_update_rate ticks */
        std::uint16_t baseline_update_weapon_slot;

        /** Current grenade slot; updated every remote_player_position_baseliine_update_rate ticks */
        std::uint16_t baseline_update_grenade_slot;

        PADDING(0x4);

        /** Player aiming thingy; updated every remote_player_position_baseline_update_rate ticks */
        Point3D update_aiming;

        PADDING(0x10);

        /** Position; updated every remote_player_position_baseliine_update_rate ticks */
        Point3D update_position;

        PADDING(0x84);

        /**
         * Get the handle of the player
         * @return the handle of the player
         */
        PlayerHandle get_handle() const noexcept;
    };
    static_assert(sizeof(Player) == 0x200);

    /**
     * Get the handle of the client player
     * @return handle of the client player
     */
    PlayerHandle get_client_player_handle() noexcept;

    struct PlayerTable : GenericTable<Player> {
        /**
         * Get the player
         * @param  player_handle handle of the player to get
         * @return           the player if found, or nullptr if not
         */
        Player *get_player(PlayerHandle player_handle) noexcept;

        /**
         * Get a player by rcon handle (0-15)
         * @return pointer to the player if found
         */
        Player *get_player_by_rcon_handle(std::size_t rcon_handle) noexcept;

        /**
         * Get the client player
         * @return pointer to the client player
         */
        Player *get_client_player() noexcept;
    };

    /**
     * Get the player table
     * @return reference to the player table
     */
    PlayerTable &get_player_table() noexcept;

    struct Antenna {
        std::uint32_t unknown_0;
        std::uint32_t unknown_1;
        std::uint32_t tag_handle;
        std::uint32_t parent_object_id;
        Point3D position;
        AntennaVertex vertices[0x15];
    };
    static_assert(sizeof(Antenna) == 0x2BC);

    using AntennaTable = GenericTable<Antenna>;

    /**
     * Get the antenna table
     * @return reference to the antenna table
     */
    AntennaTable &get_antenna_table() noexcept;

    using DecalTable = GenericTable<void>;
    
    /**
     * Get the decal table
     * @return reference to the decal table
     */
    DecalTable &get_decal_table() noexcept;

    using EffectTable = GenericTable<void>;

    /**
     * Get the effect table
     * @return reference to the effect table
     */
    EffectTable &get_effect_table() noexcept;

    /** This is a part of a flag. */
    struct FlagPart {
        /** Coordinates relative to the world */
        Point3D position;

        /** World units per tick */
        Point3D velocity;
    };
    static_assert(sizeof(FlagPart) == 0x18);

    /** This is a flag cloth object. */
    struct Flag {
        ElementHandle some_handle;
        std::uint32_t unknown0;
        ObjectHandle parent_object_handle;
        std::uint32_t some_handle1;
        Point3D position;
        FlagPart parts[241];
        char padding[8];
    };
    static_assert(sizeof(Flag) == 0x16BC);

    using FlagTable = GenericTable<Flag>;

    /**
     * Get the flag table
     * @return reference to the flag table
     */
    FlagTable &get_flag_table() noexcept;

    struct Light {
        // 0x0
        std::uint32_t unknown0;
        ElementHandle some_handle;
        std::uint32_t unknown1;
        std::uint32_t some_counter;
        // 0x10
        std::uint32_t unknown2;
        float red;
        float green;
        float blue;
        // 0x20
        std::uint32_t unknown3;
        std::uint32_t unknown4;
        std::uint32_t unknown5;
        ObjectHandle parent_object_handle;
        // 0x30
        Point3D position;
        Point3D orientation[2];
        // 0x54
        char i_stopped_caring_at_this_point[0x28]; // TODO: Figure out what this is 
    };
    static_assert(sizeof(Light) == 0x7C);

    using LightTable = GenericTable<Light>;

    /**
     * Get the light table
     * @return reference to the light table
     */
    LightTable &get_light_table() noexcept;

    struct Particle {
        // 0x0
        std::uint32_t unknown0;
        TagHandle tag_handle;
        std::uint32_t unknown1 = 0xFFFFFFFF;
        std::uint32_t unknown2;
        // 0x10
        std::uint32_t frames_alive;
        float a0; // 0.0 = begin to fade out
        float a;
        float b;
        // 0x20
        float c; // idk, don't set this to anything less than or equal to 0 or the game will freeze with max CPU usage somehow
        std::uint32_t unknown3;
        std::uint32_t unknown4;
        ElementHandle some_handle;
        // 0x30
        Point3D position;
        float unknownx0;
        // 0x40
        float unknowny0;
        float unknownz0;
        float unknownx1;
        float unknowny1;
        // 0x50
        float unknownz1;
        float radius_x;
        float radius_y;
        float radius_z;
        // 0x60
        std::uint32_t unknown5;
        float red;
        float green;
        float blue;
    };
    static_assert(sizeof(Particle) == 0x70);

    using ParticleTable = GenericTable<Particle>;

    /**
     * Get the particle table
     * @return reference to the particle table
     */
    ParticleTable &get_particle_table() noexcept;

    /**
     * Get whether the game is paused
     */
    bool game_paused() noexcept;
}

#endif
