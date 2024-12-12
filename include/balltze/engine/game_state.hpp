// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__ENGINE__GAME_STATE_HPP
#define BALLTZE_API__ENGINE__GAME_STATE_HPP

#include <cstdint>
#include <balltze/engine/tag_definitions/scenario.hpp>
#include <balltze/engine/tag_definitions/damage_effect.hpp>
#include <balltze/engine/netgame.hpp>
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
        T* first_element;

        /**
         * Get the element from an index
         * @param  index index of the element
         * @return       pointer to the element or nullptr if out of bounds
         */
        T* get_element(std::size_t index) {
            if (index >= this->current_size) {
                return nullptr;
            }
            else {
                return this->first_element + index;
            }
        }
    };
    static_assert(sizeof(GenericTable<int>) == 0x38);

    struct BaseObject;

    /**
     * This represents and points to an object in loopobjects.
     */
    struct ObjectTableEntry {
        /** Object handle index */
        std::uint16_t id;

        PADDING(0x4);

        /** Address of the object in loopobjects */
        BaseObject* object;
    };
    static_assert(sizeof(ObjectTableEntry) == 0xC);

    struct BALLTZE_API ObjectTable : GenericTable<ObjectTableEntry> {
        /**
         * Get the object by its handle, returning nullptr if the handle is invalid.
         * @param  object_handle This is the handle of the object.
         * @return           Return a pointer to the object or nullptr if the handle is invalid.
         */
        BaseObject* get_object(const ObjectHandle& object_handle) noexcept;

        /**
         * Get the object by an index, returning nullptr if the index is invalid.
         * @param  index This is the index of the object.
         * @return       Return a pointer to the object or nullptr if the index is invalid.
         */
        BaseObject* get_object(std::uint32_t index) noexcept;

        /**
         * Spawn an object.
         * @param  tag_handle   Tag handle of the object.
         * @param  offset       Offset of the object.
         * @param  parent       Parent object.
         * @return              Handle of the spawned object.
         */
        ObjectHandle create_object(const TagHandle& tag_handle, Point3D offset, const ObjectHandle& parent) noexcept;

        /**
         * Delete an object with an object handle.
         * @param  object_handle The object handle of the object
         */
        void delete_object(const ObjectHandle& object_handle) noexcept;

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
    BALLTZE_API ObjectTable& get_object_table() noexcept;

    /**
     * This is the type of object an object is.
     */
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

    /**
     * Roles of an object in the netgame.
     */
    enum ObjectNetworkRole : std::uint32_t {
        OBJECT_NETWORK_ROLE_MASTER = 0,
        OBJECT_NETWORK_ROLE_PUPPET,
        OBJECT_NETWORK_ROLE_LOCALLY_CONTROLLED_PUPPET,
        OBJECT_NETWORK_ROLE_LOCAL_ONLY
    };

    /**
     * A model node is a part of a model which can have its own position, rotation, and scale.
     */
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

    struct BaseObjectFlags {
        /** No collision - this is used for backpack weapons. */
        bool no_collision : 1;

        /** On the ground */
        bool on_ground : 1;

        /** Ignore gravity */
        bool ignore_gravity : 1;

        /** In water */
        bool in_water : 1;

        PADDING_BIT(bool, 1);

        /** Not moving */
        bool stationary : 1;

        PADDING_BIT(bool, 1);

        /** No collision? */
        bool no_collision2 : 1;

        PADDING_BIT(bool, 2);

        /** Has sound looping attachment */
        bool has_sound_looping_attachment : 1;

        /** Connected to map */
        bool connected_to_map : 1;

        /** Not placed automatically */
        bool not_placed_automatically : 1;

        /** Is a device machine */
        bool is_device_machine : 1;

        /** Is an elevator */
        bool is_elevator : 1;

        /** Is an elevator again */
        bool is_elevator_2 : 1;

        /** Is garbage */
        bool is_garbage : 1;

        PADDING_BIT(bool, 1);

        /** Cast a shadow if this is off. This is usually off for units, but it can be forced off for other objects. */
        bool no_shadow : 1;

        /** Deleted at deactivation */
        bool delete_at_deactivation : 1;

        /** Do not reactivate */
        bool do_not_reactivate : 1;

        /** Object is in the void. */
        bool outside_of_map : 1;

        PADDING_BIT(bool, 2);

        /** Is collidable */
        bool collidable : 1;

        /** Has collision model */
        bool has_collision_model : 1;

        /** Network related flags */
        bool network_message_unknown_1 : 1;
        bool network_message_unknown_2 : 1;

        /** Open sauce flags */
        bool opensauce_is_transforming_in : 1;
        bool opensauce_is_transforming_out : 1;

        PADDING_BIT(bool, 2);
    };
    static_assert(sizeof(BaseObjectFlags) == sizeof(std::uint32_t));

    struct BaseObjectNetwork {
        /** Object position is valid */
        bool valid_position;

        /** Object position */
        Vector3D position;

        /** Object orientation is valid */
        bool valid_forward_and_up;

        /** Object orientation/rotation */
        Point3D orientation[2];

        /** Object velocity is valid */
        bool valid_transitional_velocity;

        /** Object velocity */
        Vector3D transitional_velocity;

        /** Timestamp is valid */
        bool valid_timestamp;

        /** Timestamp */
        TickCount32 timestamp;
    };
    static_assert(sizeof(BaseObjectNetwork) == 0x44);

    struct ScenarioLocation {
        std::int32_t leaf_id;
        std::int16_t cluster_id;
        PADDING(0x2);
    };
    static_assert(sizeof(ScenarioLocation) == 0x8);

    struct BaseObjectVitalsFlags {
        /** Health damage effect applied */
        bool health_damage_effect_applied : 1;

        /** Shield damage effect applied */
        bool shield_damage_effect_applied : 1;

        /** Health depleted */
        bool health_depleted : 1;

        /** Shield depleted */
        bool shield_depleted : 1;

        PADDING_BIT(bool, 1); // something related to shield 

        /** Killed */
        bool killed : 1;

        /** Killed silent */
        bool killed_silent : 1;

        /** Cannot melee attack */
        bool cannot_melee_attack : 1;

        PADDING_BIT(bool, 3);

        /** Object is invulnerable */
        bool invulnerable : 1;

        /** Shield recharging */
        bool shield_recharging : 1;

        /** Killed no stats */
        bool killed_no_stats : 1;

        PADDING_BIT(bool, 2);
    };
    static_assert(sizeof(BaseObjectVitalsFlags) == sizeof(std::uint16_t));

    struct BaseObjectVitals {
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

        /**
         * Entangled object handle
         * When this object is damaged, the 'entangled' object will also get damaged
         * This is an immediate link, the entangled object's parent chain or 'entangled' reference isn't walked
         */
        ObjectHandle entangled_object;

        /** Recent shield damage taken? */
        float recent_shield_damage;

        /** Recent health damage taken? */
        float recent_health_damage;

        /** Amount of time since shield damage was taken? */
        TickCount32 recent_shield_damage_time;

        /** Amount of time since health damage was taken? */
        TickCount32 recent_health_damage_time;

        /** Time in ticks before shields recharge */
        TickCount16 shield_stun_time;

        /** Flags */
        BaseObjectVitalsFlags flags;
    };
    static_assert(sizeof(BaseObjectVitals) == 0x30);

    enum BaseObjectAttachmentType : std::int8_t {
        OBJECT_ATTACHMENT_TYPE_INVALID = -1,
        OBJECT_ATTACHMENT_TYPE_LIGHT = 0,
        OBJECT_ATTACHMENT_TYPE_LOOPING_SOUND,
        OBJECT_ATTACHMENT_TYPE_EFFECT,
        OBJECT_ATTACHMENT_TYPE_CONTRAIL,
        OBJECT_ATTACHMENT_TYPE_PARTICLE
    };

    struct BaseObjectAttachmentsData {
        BaseObjectAttachmentType types[8];
        ResourceHandle attachments[8];
        ResourceHandle first_widget;
    };
    static_assert(sizeof(BaseObjectAttachmentsData) == 0x2C);

    struct BaseObjectRegionDestroyeds {
        bool region_0 : 1;
        bool region_1 : 1;
        bool region_2 : 1;
        bool region_3 : 1;
        bool region_4 : 1;
        bool region_5 : 1;
        bool region_6 : 1;
        bool region_7 : 1;
        PADDING_BIT(bool, 0x8);
    };
    static_assert(sizeof(BaseObjectRegionDestroyeds) == sizeof(std::uint16_t));

    struct BaseObjectBlockReference {
        std::uint16_t size;
        std::uint16_t offset;
    };
    static_assert(sizeof(BaseObjectBlockReference) == 0x4);

    struct ObjectValidOutGoingFunctions {
        bool a : 1;
        bool b : 1;
        bool c : 1;
        bool d : 1;
        PADDING_BIT(bool, 4);
    };
    static_assert(sizeof(ObjectValidOutGoingFunctions) == 0x1);

    /**
     * These are objects that are present in an instance of Halo rather than in tag data and have parameters such as location and health.
     */
    struct BaseObject {
        /** This is the tag handle of the object. */
        TagHandle tag_handle;

        /** This is the object's network role */
        ObjectNetworkRole network_role;

        PADDING_BIT(std::uint32_t, 8);

        /** If true, force baseline update. */
        std::uint32_t should_force_baseline_update : 1;

        PADDING_BIT(std::uint32_t, 23);

        /** This is the number of ticks the object has existed. */
        TickCount32 existence_time;

        /** Object flags */
        BaseObjectFlags flags;

        /** Object marker id */
        std::uint32_t object_marker_id;

        /** Object network */
        BaseObjectNetwork network;

        /** Coordinates of the object relative to the world or to its parent */
        Point3D position;

        /** Velocity in world units per tick */
        Point3D velocity;

        /** Orientation/rotation */
        Point3D orientation[2];

        /** Rotational velocity of the object in world units per tick */
        Euler3DPYR rotation_velocity;

        /** Location of the object in the scenario */
        ScenarioLocation scenario_location;

        /** Position of the object's center used for things such as lens flares, triggers, teleporters, etc. This is always relative to the world. */
        Point3D center_position;

        /** Radius of the object's bounding sphere */
        float bounding_radius;

        /** Scale of the object */
        float scale;

        /** Type of object */
        ObjectType type;
        PADDING(0x2);

        union {
            /** Singleplayer team index */
            TagDefinitions::ScenarioTeamIndex team_owner;

            /** Multiplayer team index */
            NetworkGameMultiplayerTeam multiplayer_team_owner;
        };

        /** Name list index */
        std::uint16_t name_list_index;

        /** Ticks not spent at_rest, only biped updates this. */
        TickCount16 moving_time;

        /** Variant index for region permuations */
        std::uint16_t variant_index;

        /** Object player handle */
        PlayerHandle player;

        /** Object handle of the object's owner object */
        ObjectHandle owner_object;

        PADDING(0x4);

        /** Tag handle of the animation tag */
        TagHandle animation_tag_handle;

        /** Current animation index */
        std::uint16_t animation_index;

        /** Frame of the current animation */
        std::uint16_t animation_frame;

        /** Interpolation frame */
        std::uint16_t animation_interpolation_frame;

        /** Interpolation frame count */
        std::uint16_t animation_interpolation_frame_count;

        /** Object vitals */
        BaseObjectVitals vitals;

        PADDING(0x4);

        /** Cluster partition */
        ResourceHandle cluster_partition;

        /** Object handle of something */
        ObjectHandle unknown_object;

        /** Next object */
        ObjectHandle next_object;

        /** Current object handle of the object's current weapon; this may also be the object handle of the object's grenade if throwing */
        ObjectHandle first_object;

        /** Object handle of the object's parent; if set, then the object's position is relative to the parent */
        ObjectHandle parent_object;

        /** Parent attachment node */
        std::uint8_t parent_attachment_node;

        PADDING(0x1);

        /** Force shield update */
        bool force_shield_update;

        ObjectValidOutGoingFunctions valid_outgoing_functions; 
        float incoming_function_values[4];
        float outgoing_function_values[4];

        /** Attachment data */
        BaseObjectAttachmentsData attachment_data;

        /** Cached render state */
        ResourceHandle cached_render_state;

        /** Region destroyeds */
        BaseObjectRegionDestroyeds region_destroyeds;

        /** Shader permuations */
        std::int16_t shader_permutation;

        /** Region healths */
        std::uint8_t region_healths[8];

        /** Region permutation ids */
        std::int8_t region_permutation_ids[8];

        /** Colors used for things like armor color */
        ColorRGB color_change[4];

        /** Colors used for things like armor color */
        ColorRGB color_change_2[4];

        /** Node orientations blocks */
        BaseObjectBlockReference node_orientations[2];

        /** Node matrices block */
        BaseObjectBlockReference node_matrices_block;

        /**
         * Get the full object handle of the object.
         * @return full object handle of the object or a null handle if not available
         */
        ObjectHandle object_handle() noexcept {
            auto& table = get_object_table();

            ObjectHandle returned_id;
            returned_id.value = 0xFFFFFFFF;

            for (std::size_t i = 0; i < table.current_size; i++) {
                auto& object = table.first_element[i];
                if (object.object == this) {
                    returned_id.value = i + 0x10000 * object.id;
                    return returned_id;
                }
            }

            return returned_id;
        }


        /**
         * Get the object's model nodes or nullptr if this is unknown for this object type.
         * @return a pointer to the object model nodes or nullptr if unknown or not available
         */
        ModelNode* nodes() noexcept {
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

            if (this->type >= sizeof(model_node_offset) / sizeof(model_node_offset[0])) {
                return nullptr;
            }

            return reinterpret_cast<ModelNode*>(reinterpret_cast<std::byte*>(this) + model_node_offset[this->type]);
        }
    };
    static_assert(sizeof(BaseObject) == 0x1F4);

    struct UnitRecentDamager {
        /** Last tick this object damaged this unit */
        TickCount32 last_damage_time;

        /** Total damage done by this object */
        float total_damage;

        /** ObjectHandle of this object */
        ObjectHandle object;

        /** Owning player handle of this object */
        PlayerHandle player;
    };
    static_assert(sizeof(UnitRecentDamager) == 0x10);

    struct UnitFlags {
        bool unknown_biped_speech_related : 1;
        PADDING_BIT(bool, 3);
        bool power_up : 1;
        bool power_up_addition : 1; // invisible power up?
        bool controllable : 1;
        bool berserking : 1;
        PADDING_BIT(bool, 8);
        PADDING_BIT(bool, 3);
        bool unknown_integrated_light_related : 1;
        bool will_not_drop_items : 1;
        bool unknown : 1;
        bool can_blink : 1;
        bool impervious : 1;
        bool suspended : 1;
        bool blind : 1;
        bool unknown_nv_related : 1; // when this is on, the integrated NV power increases. rate is 2x the speed it leaks when on. Wtf is an NV?? I asked the same thing, maybe night vision?
        bool possessed : 1;
        bool desires_flashlight_on : 1;
        bool desires_flashlight_off : 1;
        PADDING_BIT(bool, 2);
    };
    static_assert(sizeof(UnitFlags) == 0x4);

    struct UnitControlFlags {
        bool crouch : 1;
        bool jump : 1;
        bool user1 : 1;
        bool user2 : 1;
        bool light : 1;
        bool exact_facing : 1;
        bool action : 1;
        bool melee : 1;
        bool look_dont_turn : 1;
        bool force_alert : 1;
        bool reload : 1;
        bool primary_trigger : 1;
        bool secondary_trigger : 1;
        bool grenade : 1;
        bool exchange_weapon : 1;
        PADDING_BIT(bool, 1);
    };

    enum UnitThrowingGrenadeState : std::int8_t {
        UNIT_THROWING_GRENADE_STATE_NONE = 0,
        UNIT_THROWING_GRENADE_STATE_BEGIN,
        UNIT_THROWING_GRENADE_STATE_IN_HAND,
        UNIT_THROWING_GRENADE_STATE_RELEASED
    };

    enum UnitAnimationState : std::int8_t {
        UNI_ANIMATION_STATE_INVALID = -1,
        UNI_ANIMATION_STATE_IDLE,
        UNI_ANIMATION_STATE_GESTURE,
        UNI_ANIMATION_STATE_TURN_LEFT,
        UNI_ANIMATION_STATE_TURN_RIGHT,
        UNI_ANIMATION_STATE_MOVE_FRONT,
        UNI_ANIMATION_STATE_MOVE_BACK,
        UNI_ANIMATION_STATE_MOVE_LEFT,
        UNI_ANIMATION_STATE_MOVE_RIGHT,
        UNI_ANIMATION_STATE_STUNNED_FRONT,
        UNI_ANIMATION_STATE_STUNNED_BACK,
        UNI_ANIMATION_STATE_STUNNED_LEFT,
        UNI_ANIMATION_STATE_STUNNED_RIGHT,
        UNI_ANIMATION_STATE_SLIDE_FRONT,
        UNI_ANIMATION_STATE_SLIDE_BACK,
        UNI_ANIMATION_STATE_SLIDE_LEFT,
        UNI_ANIMATION_STATE_SLIDE_RIGHT,
        UNI_ANIMATION_STATE_READY,
        UNI_ANIMATION_STATE_PUT_AWAY,
        UNI_ANIMATION_STATE_AIM_STILL,
        UNI_ANIMATION_STATE_AIM_MOVE,
        UNI_ANIMATION_STATE_AIRBORNE,
        UNI_ANIMATION_STATE_LAND_SOFT,
        UNI_ANIMATION_STATE_LAND_HARD,
        UNI_ANIMATION_STATE_UNKNOWN23,
        UNI_ANIMATION_STATE_AIRBORNE_DEAD,
        UNI_ANIMATION_STATE_LANDING_DEAD,
        UNI_ANIMATION_STATE_SEAT_ENTER,
        UNI_ANIMATION_STATE_SEAT_EXIT,
        UNI_ANIMATION_STATE_CUSTOM_ANIMATION,
        UNI_ANIMATION_STATE_IMPULSE,
        UNI_ANIMATION_STATE_MELEE,
        UNI_ANIMATION_STATE_MELEE_AIRBORNE,
        UNI_ANIMATION_STATE_MELEE_CONTINUOUS,
        UNI_ANIMATION_STATE_THROW_GRENADE,
        UNI_ANIMATION_STATE_RESSURECT_FRONT,
        UNI_ANIMATION_STATE_RESSURECT_BACK,
        UNI_ANIMATION_STATE_FEEDING,
        UNI_ANIMATION_STATE_SURPRISE_FRONT,
        UNI_ANIMATION_STATE_SURPRISE_BACK,
        UNI_ANIMATION_STATE_LEAP_START,
        UNI_ANIMATION_STATE_LEAP_AIRBORNE,
        UNI_ANIMATION_STATE_LEAP_MELEE,
        UNI_ANIMATION_STATE_UNKNOWN42,
        UNI_ANIMATION_STATE_BERSERK,
        UNI_ANIMATION_STATE_YELO_SEAT_BOARDING,
        UNI_ANIMATION_STATE_YELO_SEAT_EJECTING,
        UNI_ANIMATION_STATE_YELO_MOUNTING,
        UNI_ANIMATION_STATE_YELO_TRANSFORMING
    };

    enum UnitReplacementAnimationState : std::int8_t {
        UNIT_REPLACEMENT_ANIMATION_STATE_NONE = 0,
        UNIT_REPLACEMENT_ANIMATION_STATE_DISARM,
        UNIT_REPLACEMENT_ANIMATION_STATE_WEAPON_DROP,
        UNIT_REPLACEMENT_ANIMATION_STATE_WEAPON_READY,
        UNIT_REPLACEMENT_ANIMATION_STATE_WEAPON_PUT_AWAY,
        UNIT_REPLACEMENT_ANIMATION_STATE_WEAPON_RELOAD1,
        UNIT_REPLACEMENT_ANIMATION_STATE_WEAPON_RELOAD2,
        UNIT_REPLACEMENT_ANIMATION_STATE_MELEE,
        UNIT_REPLACEMENT_ANIMATION_STATE_THROW_GRENADE
    };

    enum UnitOverlayAnimationState : std::int8_t {
        UNIT_OVERLAY_ANIMATION_STATE_NONE = 0,
        UNIT_OVERLAY_ANIMATION_STATE_FIRE1,
        UNIT_OVERLAY_ANIMATION_STATE_FIRE2,
        UNIT_OVERLAY_ANIMATION_STATE_CHARGED1,
        UNIT_OVERLAY_ANIMATION_STATE_CHARGED2,
        UNIT_OVERLAY_ANIMATION_STATE_CHAMBER1,
        UNIT_OVERLAY_ANIMATION_STATE_CHAMBER2
    };

    enum UnitBaseSeat : std::int8_t {
        UNIT_BASE_SEAT_ASLEEP = 0,
        UNIT_BASE_SEAT_ALERT,
        UNIT_BASE_SEAT_STAND,
        UNIT_BASE_SEAT_CROUCH,
        UNIT_BASE_SEAT_FLEE,
        UNIT_BASE_SEAT_FLAMING
    };

    struct UnitAnimationStateData {
        std::int16_t animation_index;
        std::int16_t frame_index;
    };

    struct UnitAnimationFlags {
        bool animation_bit0_unknown : 1;
        bool animation_bit1_unknown : 1;
        bool animation_bit2_unknown : 1;
        bool animation_bit3_unknown : 1;
        PADDING_BIT(bool, 4);
        PADDING(1);
    };

    struct UnitAnimationData {
        UnitAnimationFlags flags;
        std::int16_t unknown_some_animation_index_maybe;
        std::int16_t unknown_some_animation_index;
        PADDING(2); // Only set on initialization, never read afterwards
        // animation graph unit indexes
        std::uint8_t seat_index;
        std::uint8_t seat_weapon_index;
        std::uint8_t weapon_type_index;
        UnitAnimationState state;
        UnitReplacementAnimationState replacement_state;
        UnitOverlayAnimationState overlay_state;
        UnitAnimationState desired_state;
        UnitBaseSeat base_seat;
        std::int8_t emotion;
        PADDING(1);
        UnitAnimationStateData replacement_animation;
        UnitAnimationStateData overlay_state_animation;
        UnitAnimationStateData weapon_ik;
        bool update_look;
        bool update_aim;
        Rectangle2DF looking_bounds;
        Rectangle2DF aiming_bounds;
        PADDING(8);
    };
    static_assert(sizeof(UnitAnimationData) == 0x48);

    enum UnitSpeechPriority : std::int16_t {
        UNIT_SPEECH_PRIORITY_NONE = 0,
        UNIT_SPEECH_PRIORITY_IDLE,
        UNIT_SPEECH_PRIORITY_PAIN,
        UNIT_SPEECH_PRIORITY_TALK,
        UNIT_SPEECH_PRIORITY_COMMUNICATE,
        UNIT_SPEECH_PRIORITY_SHOUT,
        UNIT_SPEECH_PRIORITY_SCRIPT,
        UNIT_SPEECH_PRIORITY_INVOLUNTARY,
        UNIT_SPEECH_PRIORITY_EXLAIM,
        UNIT_SPEECH_PRIORITY_SCREAM,
        UNIT_SPEECH_PRIORITY_DEATH
    };

    enum UnitScreamType : int16_t {
        UNIT_SCREAM_TYPE_FEAR = 0,
        UNIT_SCREAM_TYPE_ENEMY_GRENADE,
        UNIT_SCREAM_TYPE_PAIN,
        UNIT_SCREAM_TYPE_MAIMED_LIMB,
        UNIT_SCREAM_TYPE_MAIMED_HEAD,
        UNIT_SCREAM_TYPE_RESSURECTION
    };

    enum AiCommunicationType : int16_t {
        AI_COMMUNICATION_TYPE_DEATH = 0,
        AI_COMMUNICATION_TYPE_SPREE,
        AI_COMMUNICATION_TYPE_HURT,
        AI_COMMUNICATION_TYPE_DAMAGE,
        AI_COMMUNICATION_TYPE_SIGHTED_ENEMY,
        AI_COMMUNICATION_TYPE_FOUND_ENEMY,
        AI_COMMUNICATION_TYPE_UNEXPECTED_ENEMY,
        AI_COMMUNICATION_TYPE_FOUND_DEAD_FRIEND,
        AI_COMMUNICATION_TYPE_ALLEGIANCE_CHANGED,
        AI_COMMUNICATION_TYPE_GRENADE_THROWING,
        AI_COMMUNICATION_TYPE_GRENADE_STARTLE,
        AI_COMMUNICATION_TYPE_GRENADE_SIGHTED,
        AI_COMMUNICATION_TYPE_GRENADE_DANGER,
        AI_COMMUNICATION_TYPE_LOST_CONTACT,
        AI_COMMUNICATION_TYPE_BLOCKED,
        AI_COMMUNICATION_TYPE_ALERT_NONCOMBAT,
        AI_COMMUNICATION_TYPE_SEARCH_START,
        AI_COMMUNICATION_TYPE_SEARCH_QUERY,
        AI_COMMUNICATION_TYPE_SEARCH_REPORT,
        AI_COMMUNICATION_TYPE_SEARCH_ABANDON,
        AI_COMMUNICATION_TYPE_SEARCH_GROUP_ABANDON,
        AI_COMMUNICATION_TYPE_UNCOVER_START,
        AI_COMMUNICATION_TYPE_ADVANCE,
        AI_COMMUNICATION_TYPE_RETREAT,
        AI_COMMUNICATION_TYPE_COVER,
        AI_COMMUNICATION_TYPE_SIGHTED_FRIEND_PLAYER,
        AI_COMMUNICATION_TYPE_SHOOTING,
        AI_COMMUNICATION_TYPE_SHOOTING_VEHICLE,
        AI_COMMUNICATION_TYPE_SHOOTING_BERSERK,
        AI_COMMUNICATION_TYPE_SHOOTING_GROUP,
        AI_COMMUNICATION_TYPE_SHOOTING_TRAITOR,
        AI_COMMUNICATION_TYPE_FLEE,
        AI_COMMUNICATION_TYPE_FLEE_LEADER_DIED,
        AI_COMMUNICATION_TYPE_FLEE_IDLE,
        AI_COMMUNICATION_TYPE_ATTEMPTED_FLEE,
        AI_COMMUNICATION_TYPE_HIDING_FINISHED,
        AI_COMMUNICATION_TYPE_VEHICLE_ENTRY,
        AI_COMMUNICATION_TYPE_VEHICLE_EXIT,
        AI_COMMUNICATION_TYPE_VEHICLE_WOOHOO,
        AI_COMMUNICATION_TYPE_VEHICLE_SCARED,
        AI_COMMUNICATION_TYPE_VEHICLE_FALLING,
        AI_COMMUNICATION_TYPE_SURPRISE,
        AI_COMMUNICATION_TYPE_BERSERK,
        AI_COMMUNICATION_TYPE_MELEE,
        AI_COMMUNICATION_TYPE_DIVE,
        AI_COMMUNICATION_TYPE_UNCONVER_EXCLAMATION,
        AI_COMMUNICATION_TYPE_FALLING,
        AI_COMMUNICATION_TYPE_LEAP,
        AI_COMMUNICATION_TYPE_POSTCOMBAT_ALONE,
        AI_COMMUNICATION_TYPE_POSTCOMBAT_UNSCATHED,
        AI_COMMUNICATION_TYPE_POSTCOMBAT_WOUNDED,
        AI_COMMUNICATION_TYPE_POSTCOMBAT_MASSACRE,
        AI_COMMUNICATION_TYPE_POSTCOMBAT_TRIUMPH,
        AI_COMMUNICATION_TYPE_POSTCOMBAT_CHECK_ENEMY,
        AI_COMMUNICATION_TYPE_POSTCOMBAT_CHECK_FRIEND,
        AI_COMMUNICATION_TYPE_POSTCOMBAT_SHOOT_CORPSE,
        AI_COMMUNICATION_TYPE_POSTCOMBAT_CELEBRATE
    };

    struct AiCommunicationPacket {
        PADDING(6); //unknown
        AiCommunicationType type;
        PADDING(2); //unknown
        PADDING(2);
        PADDING(2); //unknown
        PADDING(6);
        PADDING(2); //unknown
        PADDING(2);
        PADDING(4); //unknown
        bool broken;
        PADDING(3);
    };
    static_assert(sizeof(AiCommunicationPacket) == 0x20);

    struct UnitSpeech {
        UnitSpeechPriority priority;
        UnitScreamType scream_type;
        TagHandle sound_tag;
        TickCount16 ticks;
        std::int16_t unknown2;
        std::int32_t unknown;
        AiCommunicationPacket ai_communication_info;
    };
    static_assert(sizeof(UnitSpeech) == 0x30);

    struct UnitSpeechData {
        UnitSpeech current;
        UnitSpeech next;
        std::int16_t unknown0; // speech_ticks_left?
        std::int16_t unknown1; // speech_ticks_left?
        std::int16_t unknown2; // speech_ticks_left?
        std::int16_t unknown3;
        std::int32_t unknown4;
        bool unknown6;
        bool unknown7;
        bool unknown8;
        PADDING(1);
        std::int16_t unknown9;
        std::int16_t unknown10;
        std::int16_t unknown11; // copied from unknown2
        std::int16_t unknown12;
        std::int32_t unknown13; // 400 set to -1 if unknown10 becomes 0
    };

    struct UnitControlData {
        std::int8_t animation_state;
        std::int8_t aiming_speed;
        UnitControlFlags control_flags;
        std::int16_t weapon_index;
        std::int16_t grenade_index;
        std::int16_t zoom_index;
        PADDING(2);
        Vector3D throttle;
        float primary_trigger;
        Vector3D facing_vector;
        Vector3D aiming_vector;
        Vector3D looking_vector;
    };
    static_assert(sizeof(UnitControlData) == 0x40);

    struct UnitObject : BaseObject {
        TagHandle actor;
        TagHandle swarm_actor;
        ObjectHandle swarm_next_unit;
        ObjectHandle swarm_previous_unit;
        UnitFlags unit_flags;
        UnitControlFlags unit_control_flags;
        PADDING(0x4);
        std::int8_t shield_snapping;
        std::int8_t base_seat_index;
        struct {
            TickCount32 ticks_remaining;
            UnitControlFlags control_flags;
            PADDING(0x2);
        } persistent_control;
        PlayerHandle controlling_player;
        std::int16_t ai_effect_type;
        std::int16_t emotion_animation_index;
        std::uint32_t next_ai_effect_tick;
        Vector3D desired_facing_vector;
        Vector3D desired_aiming_vector;
        Vector3D aiming_vector;
        Vector3D aiming_velocity;
        Euler3D looking_angles;
        Vector3D looking_vector;
        Vector3D looking_velocity;
        Vector3D throttle;
        float primary_trigger;
        std::int8_t aiming_speed;
        std::int8_t melee_state;
        std::int8_t melee_timer;
        std::int8_t ticks_until_flame_to_death;
        std::int8_t ping_animation_ticks_left; // also set to the same PersistentControlTicks value when an actor dies and they fire-wildely
        UnitThrowingGrenadeState grenade_state;
        std::int16_t unknown_725;
        std::int16_t unknown_726;
        PADDING(0x2);
        ObjectHandle grenade_projectile; 
        UnitAnimationData animation;
        float ambient;
        float illumination;
        float mouth_factor;
        PADDING(4);
        int16_t vehicle_seat_id;
        int16_t current_weapon_id;
        int16_t next_weapon_id;
        PADDING(2);
        ObjectHandle weapons[4];
        TickCount32 weapon_ready_ticks[4];
        ResourceHandle equipment_handle; // figure out what type of resource this is
        std::int8_t current_grenade_index;
        std::int8_t next_grenade_index;
        std::uint8_t grenade_counts[2];
        // Opensauce is weird and has this defined as a union
        // so it can be two extra grenade counts when unzoomed.
        std::uint8_t zoom_level;
        std::uint8_t desired_zoom_level;
        // End of opensauce shenanigans
        std::int8_t ticks_since_last_vehicle_speech;
        std::uint8_t aiming_change;
        ObjectHandle powered_seats_riders[2];
        // these are related
        ResourceHandle _unknown22;
        std::int32_t _some_tick_time;
        std::int16_t encounter_id;
        std::int16_t squad_id;
        float powered_seats_power[2];
        float integrated_light_power;
        float integrated_light_toggle_power; // minimum power for a toggle?
        float integrated_night_vision_toggle_power;
        Vector3D seat_related[4];
        float camo_power;
        float full_spectrum_vision_power; // gets updated, but not used.
        TagHandle dialogue_definition;
        UnitSpeechData speech;
        struct {
            TagDefinitions::DamageEffectCategory category;
            TickCount16 ai_ticks_until_handle;
            float amount;
            ObjectHandle responsible_unit;
        } damage_result;
        ObjectHandle object_flame_causer; // Object that caused flaming death
        float _unknown23;
        PADDING(4);
        TickCount32 died_at_tick;
        TickCount16 feign_death_timer;
        bool camo_regrowth;
        PADDING(1);
        float stun;
        TickCount16 stun_ticks;
        std::int16_t spree_count;
        std::int32_t spree_starting_time;
        UnitRecentDamager recent_damage[4];
        PADDING(4);
        PADDING(2);
        uint8_t opensauce_zoom_level;
        uint8_t opensauce_desired_zoom_level;
        UnitControlData control_data;
        bool last_completed_client_update_valid;
        PADDING(3);
        int32_t last_completed_client_update_id;
        PADDING(12);
    };
    static_assert(sizeof(UnitObject) == 0x4CC);

    struct BipedFlags {
        bool airborne : 1;
        bool slipping : 1;
        bool absolute_movement : 1;
        bool no_collision : 1;
        bool passes_through_other_bipeds : 1;
        bool limping2 : 1;
        PADDING_BIT(bool, 2);
        PADDING(3);
    };
    static_assert(sizeof(BipedFlags) == 0x4);

    enum BipedMovementState : std::int8_t {
        BIPED_MOVEMENT_STATE_MOVING = 0,
        BIPED_MOVEMENT_STATE_IDLE, // or turning
        BIPED_MOVEMENT_STATE_GESTURING
    };

    struct BipedNetworkDelta {
        std::int8_t grenade_counts[2];
        PADDING(2);
        float body_vitality;
        float shield_vitality; // Actual shield divided by 3
        bool shield_stun_ticks_greater_than_zero;
        PADDING(3);
    };
    static_assert(sizeof(BipedNetworkDelta) == 0x10);

    struct BipedNetwork {
        PADDING(2); // two unknown signed bytes
        bool baseline_valid;
        std::int8_t baseline_id;
        std::int8_t message_id;
        PADDING(3);
        BipedNetworkDelta update_baseline;
        bool delta_valid;
        PADDING(3);
        BipedNetworkDelta update_delta;
    };
    static_assert(sizeof(BipedNetwork) == 0x2C);

    struct BipedObject : public UnitObject {
        BipedFlags biped_flags;
        // I expect these to be stun related
        std::int8_t landing_timer; // counts up whe biped lands, gets higher depending on height.
        std::int8_t landing_force; // instantly changes when landing. Depends on how hard the fall was.
        BipedMovementState movement_state;
        PADDING(1);
        std::int32_t _biped_unknown3;
        std::uint32_t action_flags; // Something to do with walking and jumping
        // maybe another set of control flags
        std::int32_t _biped_unknown4;
        Vector3D biped_position;
        std::int32_t walking_counter; //? Counts up when moving
        PADDING(12); // unknown
        ObjectHandle bump_object; // references the object that this biped last bumped into.
        std::int8_t ticks_since_last_bump;
        std::int8_t airborne_ticks;
        std::int8_t slipping_ticks; // counts up when hit by nade
        std::int8_t digital_throttle;
        std::int8_t jump_ticks;
        std::int8_t melee_ticks;
        std::int8_t melee_inflict_ticks;
        PADDING(1);
        std::int16_t unknown_biped2;
        PADDING(2);
        float crouch_scale;
        float unknown_biped1;
        Plane3D unknown_biped_physics_related;
        BipedNetwork network;
    };
    static_assert(sizeof(BipedObject) == 0x84 + sizeof(UnitObject));

    struct VehicleFlags {
        bool vehicle_unknown0 : 1; 
        bool hovering : 1;
        bool crouched : 1;
        bool jumping : 1;
        bool unknown_vehicle1 : 1;
        bool unknown_vehicle2 : 3;
        PADDING(1);
    };

    struct VehicleNetworkData {
        bool at_rest;
        PADDING(3);
        Vector3D position;
        Vector3D transitional_velocity;
        Vector3D angular_velocity;
        Vector3D forward;
        Vector3D up;
    };
    static_assert(sizeof(VehicleNetworkData) == 0x40);

    struct VehicleNetwork {
        bool time_valid;
        bool baseline_valid;
        std::int8_t baseline_id;
        std::int8_t message_id;
        VehicleNetworkData update_baseline;
        bool delta_valid; //? A comment reads 'unused'
        PADDING(3);
        VehicleNetworkData update_delta;
        TickCount32 last_moved_at_tick; // -1 if the vehicle is occupied
        // For all gametypes except race this is the vehicle_block id
        // For race this is the netgame_flag
        std::int16_t scenario_respawn_id; 
        PADDING(2);
        // Only used to check if the vehicle should respawn it seems.
        Vector3D respawn_position; // unknown function.
    };

    struct VehicleObject : public UnitObject {
        VehicleFlags vehicle_flags;
        PADDING(2); // unknown int16
        PADDING(4); // unknown set of bytes
        float speed;
        float slide;
        float turn;
        float tire_position;
        float thread_position_left;
        float thread_position_right;
        float hover;
        float thrust;
        std::int8_t suspension_states[8];
        Vector3D hover_position;
        Vector3D unknown_vehicle3;
        Vector3D unknown_vehicle4;
        std::int32_t unknown_vehicle5;
        VehicleNetwork network;
    }; 
    static_assert(sizeof(VehicleObject) == 0xF4 + sizeof(UnitObject));

    struct ItemObject : BaseObject {
        std::uint32_t flags;
        std::int16_t ticks_until_detonation;
        std::int16_t bsp_collision_surface_id;
        std::int16_t bsp_collision_reference_id; // bsp_reference_id        
        PADDING(2);
        ObjectHandle dropped_by_unit; // Set when a unit that held this item drops it.
        std::int32_t last_update_tick;
        ObjectHandle collision_object_handle;
        Vector3D collision_object_position;
        Vector3D unknown_collision_position; // ? My guesses without checking it yet.
        Euler2D unknown_collision_angle; // ?
    };
    static_assert(sizeof(ItemObject) == 0x38 + sizeof(BaseObject));

    struct GarbageObject : ItemObject {
        TickCount16 ticks_until_garbage_collection;
        PADDING(2);
        PADDING(20);
    };
    static_assert(sizeof(GarbageObject) == 0x18 + sizeof(ItemObject));

    enum WeaponState : std::int8_t {
        WEAPON_STATE_IDLE,
        WEAPON_STATE_FIRE1,
        WEAPON_STATE_FIRE2,
        WEAPON_STATE_CHAMBER1,
        WEAPON_STATE_CHAMBER2,
        WEAPON_STATE_RELOAD1,
        WEAPON_STATE_RELOAD2,
        WEAPON_STATE_CHARGED1,
        WEAPON_STATE_CHARGED2,
        WEAPON_STATE_READY,
        WEAPON_STATE_PUT_AWAY
    };

    struct WeaponTrigger {
        std::int8_t idle_time;
        WeaponState state;
        TickCount16 trigger_time;
        std::uint32_t not_firing;
        std::uint32_t auto_reload;
        PADDING(0x2);
        std::uint16_t rounds_since_last_tracer;
        float rate_of_fire;
        float ejection_port_recovery_time;
        float illumination_recovery_time;
        float projectile_error_related;
        ResourceHandle charing_effect;
        std::int8_t network_delay_ticks;
        PADDING(3);
    };
    static_assert(sizeof(WeaponTrigger) == 0x28);

    enum WeaponMagazineState : std::int16_t {
        WEAPON_MAGAZINE_STATE_IDLE,
        WEAPON_MAGAZINE_STATE_CHAMBERING_START,
        WEAPON_MAGAZINE_STATE_CHAMBERING_FINISH,
        WEAPON_MAGAZINE_STATE_CHAMBERING
    };

    struct WeaponMagazine {
        WeaponMagazineState state;
        TickCount16 reload_ticks_remaining;
        TickCount16 reload_ticks;
        std::int16_t rounds_unloaded;
        std::int16_t rounds_loaded;
        std::int16_t rounds_left_to_recharge; // number of rounds left to apply to rounds_loaded (based on tag's rounds_recharged)
        std::int16_t unknown; // maybe an enum.
        std::int16_t unknown2; // possibly padding
    };
    static_assert(sizeof(WeaponMagazine) == 0x10);

    struct WeaponReloadStartData {
        std::int16_t total_rounds[2];
        std::int16_t loaded_rounds[2];
    };
    static_assert(sizeof(WeaponReloadStartData) == 8);

    struct WeaponNetworkData {
        Vector3D position;
        Vector3D transitional_velocity;
        Vector3D angular_velocity; //unused
        std::int16_t magazine_rounds_total[2];
        float age;
    }; 
    static_assert(sizeof(WeaponNetworkData) == 0x2C);

    struct WeaponNetwork {
        bool baseline_valid;
        std::int8_t baseline_index;
        std::int8_t message_index;
        PADDING(1);
        WeaponNetworkData update_baseline;
        bool delta_valid;
        PADDING(3);
        WeaponNetworkData update_delta;
    };

    struct WeaponObject : ItemObject {
        std::uint32_t flags;
        std::uint16_t owner_unit_flags;
        PADDING(2);
        float primary_trigger;
        WeaponState weapon_state;
        PADDING(1);
        std::int16_t ready_ticks;
        float heat;
        float age;
        float illumination_fraction;
        float integrated_light_power;
        PADDING(4);
        ObjectHandle tracked_object;
        PADDING(8);
        std::int16_t alt_shots_loaded;
        PADDING(2);
        WeaponTrigger triggers[2];
        WeaponMagazine magazines[2]; 
        TickCount32 last_trigger_fire_tick;
        WeaponReloadStartData reload_starting_point;
        PADDING(4);
        WeaponNetwork network;
    };
    static_assert(sizeof(WeaponObject) == 0x114 + sizeof(ItemObject));

    struct EquipmentNetworkData {
        Vector3D position;
        Vector3D transitional_velocity;
        Vector3D angular_velocity;
    }; 
    static_assert(sizeof(EquipmentNetworkData) == 0x24);

    struct EquipmentNetwork {
        bool baseline_valid;
        std::int8_t baseline_index;
        std::int8_t message_index;
        PADDING(1);
        EquipmentNetworkData update_baseline;
        bool delta_valid; // Unsure
        PADDING(3);
        EquipmentNetworkData update_delta;
    };

    struct EquipmentObject : public ItemObject {
        PADDING(24); //Opensauce uses these padding bytes
        EquipmentNetwork network;
    };
    static_assert(sizeof(EquipmentObject) == 0x68 + sizeof(ItemObject));

    struct ProjectileObjectFlags {
        bool tracer : 1;
        bool projectile_unknown_bit : 1;
        bool attached : 1;
        PADDING_BIT(bool, 5); //unknown
        PADDING(3); //possibly more bits
    };

    struct ProjectileNetworkData {
        Vector3D position;
        Vector3D transitional_velocity;
    }; static_assert(sizeof(ProjectileNetworkData) == 0x18);

    struct ProjectileNetwork {
        bool unknown;
        bool baseline_valid;
        std::int8_t baseline_index;
        std::int8_t message_index;
        ProjectileNetworkData update_baseline;
        bool delta_valid;
        PADDING(3);
        ProjectileNetworkData update_delta;
    };

    struct ProjectileObject : public ItemObject {
        ProjectileObjectFlags projectile_flags;
        std::int16_t action_enum;
        std::int16_t material_id; // unconfirmed.
        ObjectHandle source_unit;
        ObjectHandle target_object;
        std::int32_t contrail_attachment_block_id;
        float time_remaining; // To target or to 0.0? What?
        float arming_rate; // related to detonation coundown timer
        float unknown_proj_float1;
        float unknown_proj_float2; // related to arming time
        float distance_travelled;
        Vector3D transitional_velocity;
        float water_damage_upper_bound;
        Vector3D angular_velocity;
        Euler2D unknown_euler;
        ProjectileNetwork network;
    };
    static_assert(sizeof(ProjectileObject) == 0x84 + sizeof(ItemObject));

    struct DeviceObjectState {
        std::int16_t device_group_id;
        PADDING(2);
        float value;
        float change;
    };
    
    struct DeviceObject : public BaseObject {
        bool position_reversed : 1;
        bool not_usable_from_any_side : 1;
        PADDING_BIT(bool, 6);
        PADDING(3);
        DeviceObjectState power;
        DeviceObjectState position;
        bool one_sided : 1;
        bool operates_automatically : 1;
        PADDING_BIT(bool, 6);
        PADDING(3);
    };
    static_assert(sizeof(DeviceObject) == 0x20 + sizeof(BaseObject));

    struct DeviceMachineObjectFlags {
        bool does_not_operate_automatically : 1;
        bool machine_one_sided : 1;
        bool never_appears_locked : 1;
        bool opened_by_melee_attack : 1;
        PADDING_BIT(bool, 4);
        PADDING(3);
    };

    struct DeviceMachineObject : public DeviceObject {
        DeviceMachineObjectFlags device_flags;
        std::uint32_t ticks_since_started_opening;
        Vector3D elevator_position;
    };
    static_assert(sizeof(DeviceMachineObject) == 0x14 + sizeof(DeviceObject));

    struct DeviceControlObjectFlags {
        bool usable_from_both_sides : 1;
        PADDING_BIT(bool, 7);
        PADDING(3);
    };

    struct DeviceControlObject : public DeviceObject {
        DeviceControlObjectFlags device_control_flags;
        int16_t custom_name_id;
        PADDING(2);
    };
    static_assert(sizeof(DeviceControlObject) == 8 + sizeof(DeviceObject));

    struct DeviceLightFixtureObject : public DeviceObject {
        ColorRGB light_color;
        float light_intensity;
        float light_falloff_angle;
        float light_cutoff_angle;
    };
    static_assert(sizeof(DeviceLightFixtureObject) == 0x18 + sizeof(DeviceObject));

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
    BALLTZE_API void apply_damage(ObjectHandle object, TagHandle damage_effect, float multiplier = 1.0F, PlayerHandle causer_player = PlayerHandle::null(), ObjectHandle causer_object = ObjectHandle::null()) noexcept;

    struct AntennaVertex {
        Point3D position;
        Point3D velocity;
        float scale;
        std::uint32_t counter;
    };
    static_assert(sizeof(AntennaVertex) == 0x20);

    enum NetworkPlayerColor : std::uint16_t {
        NETWORK_COLOR_WHITE   = 0,
        NETWORK_COLOR_BLACK,
        NETWORK_COLOR_RED,
        NETWORK_COLOR_BLUE,
        NETWORK_COLOR_GRAY,
        NETWORK_COLOR_YELLOW,
        NETWORK_COLOR_GREEN,
        NETWORK_COLOR_PINK,
        NETWORK_COLOR_PURPLE,
        NETWORK_COLOR_CYAN,
        NETWORK_COLOR_COBALT,
        NETWORK_COLOR_ORANGE,
        NETWORK_COLOR_TEAL,
        NETWORK_COLOR_SAGE,
        NETWORK_COLOR_BROWN,
        NETWORK_COLOR_TAN,
        NETWORK_COLOR_MAROON,
        NETWORK_COLOR_SALMON
    };

    enum PlayerObjectiveMode : std::int32_t {
        PLAYER_OBJECTIVE_MODE_RACE = 22,
        PLAYER_OBJECTIVE_MODE_HILL = 34,
        PLAYER_OBJECTIVE_MODE_JUGGERNAUT = 35,
        PLAYER_OBJECTIVE_MODE_BALL = 41,
        PLAYER_OBJECTIVE_MODE_NONE = -1
    };

    union PlayerMultiplayerStatistics {
        std::uint32_t _pad[2];

        struct Ctf {
            std::int16_t flag_grabs;
            std::int16_t flag_returns;
            std::int16_t flag_scores;
        } ctf;

        struct Slayer {
        } slayer;

        struct Oddball {
            std::int16_t unknown;
            std::int16_t target_kills;
            std::int16_t kills;
        } oddball;

        struct King {
            std::int16_t hill_score;
        } king;

        struct Race {
            std::int16_t time;
            std::int16_t laps;
            std::int16_t best_time;
        } race;
    }; static_assert(sizeof(PlayerMultiplayerStatistics) == 8);

    enum PlayerInteractionType : std::uint16_t {
        PLAYER_INTERACTION_TYPE_NONE,
        PLAYER_INTERACTION_TYPE_PICKUP_POWERUP,
        PLAYER_INTERACTION_TYPE_SWAP_POWERUP,
        PLAYER_INTERACTION_TYPE_EXIT_SEAT,
        PLAYER_INTERACTION_TYPE_FOUR, // Unknown
        PLAYER_INTERACTION_TYPE_SWAP_EQUIPMENT,
        PLAYER_INTERACTION_TYPE_SWAP_WEAPON,
        PLAYER_INTERACTION_TYPE_PICKUP_WEAPON,
        PLAYER_INTERACTION_TYPE_ENTER_SEAT,
        PLAYER_INTERACTION_TYPE_FORCE_AI_TO_EXIT_SEAT,
        PLAYER_INTERACTION_TYPE_TOUCH_DEVICE,
        PLAYER_INTERACTION_TYPE_FLIP_VEHICLE
    };

    /**
     * These are players.
     */
    struct BALLTZE_API Player {
        /** This is some sort of an index of the player. If it's 0xFFFF, the slot is not used. */
        std::uint16_t player_id;

        /** Unknown - I am guessing that this is the local client handle of the player (if valid - otherwise 0xFFFF). */
        std::uint16_t local_handle;

        /** Name of the player */
        wchar_t name[12];

        ResourceHandle unknown_handle;

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
        TickCount32 respawn_time;

        /** Number of ticks being added to the player's next respawn time */
        TickCount32 respawn_time_growth;

        /** Player's current object handle. */
        ObjectHandle object_handle;

        /** Player's object handle. Does this have to do with death cam or something? */
        ObjectHandle prev_object_handle;

        std::int16_t bsp_cluster_id;
        bool weapon_swap_result;
        PADDING(1);
        ObjectHandle auto_aim_target_object;

        /** Last tick the player fired */
        TickCount32 last_fire_time;

        /** Name of the player (again) */
        wchar_t name2[12];

        /** Color of the player in respect to FFA */
        NetworkPlayerColor color;

        std::int16_t icon_index;

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

        std::int16_t other_powerup_time_left;

        /** Speed multiplier of the player. */
        float speed;

        /** Unknown */
        ResourceHandle teleporter_flag_handle;

        /** Unknown */
        PlayerObjectiveMode objective_mode;

        /** Unknown */
        PlayerHandle objective_player_handle;

        /** Unknown - Player handle the player is looking at? */
        PlayerHandle target_player;

        /** Unknown - Some timer for fading in the name of the player being looked at? */
        TickCount32 target_time;

        /** Tick the player last died */
        TickCount32 last_death_time;

        /** Current slayer target */
        PlayerHandle slayer_target;

        /** Player is odd man out */
        bool odd_man_out;

        PADDING(9);

        /** Player's current kill streak */
        std::uint16_t kill_streak;

        /** Unknown */
        std::uint16_t multikill;

        /** Last tick a player killed someone */
        std::uint16_t last_kill_time;

        std::int16_t kills[4];
        std::int16_t assists[4];

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

        PlayerMultiplayerStatistics multiplayer_statistics;

        /** Time being telefragged in ticks */
        TickCount32 telefrag_timer;

        /** Tick the player left */
        TickCount32 quit_time;

        bool telefrag_danger;
        bool quit;

        PADDING(0x6);

        /** Current ping of the player in milliseconds */
        std::uint32_t ping;

        /** Number of betrayals */
        std::uint32_t team_kill_count;

        /** Number of ticks since the player's last betrayal */
        TickCount32 team_kill_ticks_since_last;

        // Start of network stuff

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
    BALLTZE_API PlayerHandle get_client_player_handle() noexcept;

    struct BALLTZE_API PlayerTable : GenericTable<Player> {
        /**
         * Get the player
         * @param  player_handle handle of the player to get
         * @return           the player if found, or nullptr if not
         */
        Player* get_player(PlayerHandle player_handle) noexcept;

        /**
         * Get a player by rcon handle (0-15)
         * @return pointer to the player if found
         */
        Player* get_player_by_rcon_handle(std::size_t rcon_handle) noexcept;

        /**
         * Get the client player
         * @return pointer to the client player
         */
        Player* get_client_player() noexcept;
    };

    /**
     * Get the player table
     * @return reference to the player table
     */
    BALLTZE_API PlayerTable& get_player_table() noexcept;

    struct Antenna {
        std::uint32_t unknown_0;
        std::uint32_t unknown_1;
        TagHandle tag_handle;
        ObjectHandle parent_object_handle;
        Point3D position;
        AntennaVertex vertices[0x15];
    };
    static_assert(sizeof(Antenna) == 0x2BC);

    using AntennaTable = GenericTable<Antenna>;

    /**
     * Get the antenna table
     * @return reference to the antenna table
     */
    BALLTZE_API AntennaTable& get_antenna_table() noexcept;

    using DecalTable = GenericTable<void>;

    /**
     * Get the decal table
     * @return reference to the decal table
     */
    BALLTZE_API DecalTable& get_decal_table() noexcept;

    using EffectTable = GenericTable<void>;

    /**
     * Get the effect table
     * @return reference to the effect table
     */
    BALLTZE_API EffectTable& get_effect_table() noexcept;

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
        ResourceHandle some_handle;
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
    BALLTZE_API FlagTable& get_flag_table() noexcept;

    struct Light {
        // 0x0
        std::uint32_t unknown0;
        ResourceHandle some_handle;
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
    BALLTZE_API LightTable& get_light_table() noexcept;

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
        ResourceHandle some_handle;
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
    BALLTZE_API ParticleTable& get_particle_table() noexcept;

    /**
     * Get whether the game is paused
     */
    bool game_paused() noexcept;

    /**
     * This is the state the camera is in.
     */
    enum CameraType : std::uint16_t {
        /** First person view; rendering the player's weapon */
        CAMERA_FIRST_PERSON = 0,

        /** Vehicle view (third person); can be controlled by the player */
        CAMERA_VEHICLE,

        /** Cinematic camera */
        CAMERA_CINEMATIC,

        /** Devcam or death cam */
        CAMERA_DEBUG
    };

    /**
     * Get the camera type.
     * @return Return the camera type.
     */
    BALLTZE_API CameraType get_camera_type() noexcept;

    struct CameraData {
        /** This is the position of the camera. */
        Point3D position;

        /** More stuff */
        std::uint32_t unknown[5];

        /** Orientation/rotation of the camera. */
        Point3D orientation[2];

        /** FOV (radians) */
        float fov;
    };
    static_assert(sizeof(CameraData) == 0x3C);

    /**
     * Get the camera data.
     * @return Return a reference to the camera data.
     */
    BALLTZE_API CameraData &get_camera_data() noexcept;

    /**
     * Delete all weapons from a unit.
     * @param unit The handle of the unit to delete all weapons from.
     * @throws std::runtime_error if the object does not exist, or if the object is not a biped or vehicle.
     */
    BALLTZE_API void unit_delete_all_weapons(ObjectHandle unit);

    /**
     * Add a weapon to a unit.
     * @param unit The handle of the unit to add the weapon to.
     * @param weapon The handle of the weapon to add.
     * @throws std::runtime_error if the object does not exist, or if the object is not a biped or vehicle.
     */
    BALLTZE_API void unit_add_weapon_to_inventory(ObjectHandle unit, ObjectHandle weapon, uint16_t param_3);
}

#endif
