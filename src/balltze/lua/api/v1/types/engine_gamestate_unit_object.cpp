// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/legacy_api/engine/game_state.hpp>
#include "../../../helpers/bitfield.hpp"
#include "../../../libraries/luacstruct.hpp"
#include "engine_types.hpp"
#include "engine_tag_data.hpp"
#include "engine_gamestate_object.hpp"
#include "engine_gamestate_unit_object.hpp"

namespace Balltze::Lua::Api::V1 {
    static void define_engine_unit_recent_damager_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineUnitRecentDamager);
        luacs_unsigned_field(state, EngineUnitRecentDamager, last_damage_time, 0);
        luacs_float_field(state, EngineUnitRecentDamager, total_damage, 0);
        luacs_nested_field(state, EngineUnitRecentDamager, EngineResourceHandle, object, 0);
        luacs_nested_field(state, EngineUnitRecentDamager, EngineResourceHandle,player, 0);
        lua_pop(state, 1); 
    }

    void push_engine_unit_recent_damager(lua_State *state, EngineUnitRecentDamager *recent_damager) noexcept {
        luacs_newobject(state, EngineUnitRecentDamager, recent_damager);
    }

    static void define_engine_unit_flags_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineUnitObjectFlags); 
        luacs_declare_field(state, LUACS_TINT32, NULL, "flags", sizeof(std::uint32_t), 0, 0, 0); 
        luacs_declare_method(state, "unknownBipedSpeechRelated", lua_bitfield_struct_method(state, EngineUnitObjectFlags, unknown_biped_speech_related)); 
        luacs_declare_method(state, "powerUp", lua_bitfield_struct_method(state, EngineUnitObjectFlags, power_up));
        luacs_declare_method(state, "powerUpAddition", lua_bitfield_struct_method(state, EngineUnitObjectFlags, power_up_addition));
        luacs_declare_method(state, "controllable", lua_bitfield_struct_method(state, EngineUnitObjectFlags, controllable));
        luacs_declare_method(state, "berserking", lua_bitfield_struct_method(state, EngineUnitObjectFlags, berserking));
        luacs_declare_method(state, "unknownIntegratedLightRelated", lua_bitfield_struct_method(state, EngineUnitObjectFlags, unknown_integrated_light_related));
        luacs_declare_method(state, "willNotDropItems", lua_bitfield_struct_method(state, EngineUnitObjectFlags, will_not_drop_items));
        luacs_declare_method(state, "unknownFlag", lua_bitfield_struct_method(state, EngineUnitObjectFlags, unknown));
        luacs_declare_method(state, "canBlink", lua_bitfield_struct_method(state, EngineUnitObjectFlags, can_blink));
        luacs_declare_method(state, "impervious", lua_bitfield_struct_method(state, EngineUnitObjectFlags, impervious));
        luacs_declare_method(state, "suspended", lua_bitfield_struct_method(state, EngineUnitObjectFlags, suspended));
        luacs_declare_method(state, "blind", lua_bitfield_struct_method(state, EngineUnitObjectFlags, blind));
        luacs_declare_method(state, "unknownNVRelated", lua_bitfield_struct_method(state, EngineUnitObjectFlags, unknown_nv_related));
        luacs_declare_method(state, "possessed", lua_bitfield_struct_method(state, EngineUnitObjectFlags, possessed));
        luacs_declare_method(state, "desiresFlashlightOn", lua_bitfield_struct_method(state, EngineUnitObjectFlags, desires_flashlight_on));
        luacs_declare_method(state, "desiresFlashlightOff", lua_bitfield_struct_method(state, EngineUnitObjectFlags, desires_flashlight_off));
        lua_pop(state, 1); 
    }

    void push_engine_unit_flags(lua_State *state, EngineUnitObjectFlags *flags) noexcept {
        luacs_newobject(state, EngineUnitObjectFlags, flags);
    }

    static void define_engine_unit_control_flags_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineUnitControlFlags); 
        luacs_declare_field(state, LUACS_TINT16, NULL, "flags", sizeof(std::uint16_t), 0, 0, 0); 
        luacs_declare_method(state, "crouch", lua_bitfield_struct_method(state, EngineUnitControlFlags, crouch)); 
        luacs_declare_method(state, "jump", lua_bitfield_struct_method(state, EngineUnitControlFlags, jump));
        luacs_declare_method(state, "user1", lua_bitfield_struct_method(state, EngineUnitControlFlags, user1));
        luacs_declare_method(state, "user2", lua_bitfield_struct_method(state, EngineUnitControlFlags, user2));
        luacs_declare_method(state, "light", lua_bitfield_struct_method(state, EngineUnitControlFlags, light));
        luacs_declare_method(state, "exactFacing", lua_bitfield_struct_method(state, EngineUnitControlFlags, exact_facing));
        luacs_declare_method(state, "action", lua_bitfield_struct_method(state, EngineUnitControlFlags, action));
        luacs_declare_method(state, "melee", lua_bitfield_struct_method(state, EngineUnitControlFlags, melee));
        luacs_declare_method(state, "lookDontTurn", lua_bitfield_struct_method(state, EngineUnitControlFlags, look_dont_turn));
        luacs_declare_method(state, "forceAlert", lua_bitfield_struct_method(state, EngineUnitControlFlags, force_alert));
        luacs_declare_method(state, "reload", lua_bitfield_struct_method(state, EngineUnitControlFlags, reload));
        luacs_declare_method(state, "primaryTrigger", lua_bitfield_struct_method(state, EngineUnitControlFlags, primary_trigger));
        luacs_declare_method(state, "secondaryTrigger", lua_bitfield_struct_method(state, EngineUnitControlFlags, secondary_trigger));
        luacs_declare_method(state, "grenade", lua_bitfield_struct_method(state, EngineUnitControlFlags, grenade));
        luacs_declare_method(state, "exchangeWeapon", lua_bitfield_struct_method(state, EngineUnitControlFlags, exchange_weapon));
        lua_pop(state, 1); 
    }

    void push_engine_unit_control_flags(lua_State *state, EngineUnitControlFlags *flags) noexcept {
        luacs_newobject(state, EngineUnitControlFlags, flags);
    }

    static void define_engine_unit_throwing_grenade_state_enum(lua_State *state) noexcept {
        luacs_newenum(state, EngineUnitThrowingGrenadeState);
        luacs_enum_declare_value(state, "NONE", LegacyApi::Engine::UNIT_THROWING_GRENADE_STATE_NONE);
        luacs_enum_declare_value(state, "BEGIN", LegacyApi::Engine::UNIT_THROWING_GRENADE_STATE_BEGIN);
        luacs_enum_declare_value(state, "IN_HAND", LegacyApi::Engine::UNIT_THROWING_GRENADE_STATE_IN_HAND);
        luacs_enum_declare_value(state, "RELEASED", LegacyApi::Engine::UNIT_THROWING_GRENADE_STATE_RELEASED);
        lua_pop(state, 1);
    }

    static void define_engine_unit_animation_state_enum(lua_State *state) noexcept {
        luacs_newenum(state, EngineUnitAnimationState);
        luacs_enum_declare_value(state, "INVALID", LegacyApi::Engine::UNI_ANIMATION_STATE_INVALID);
        luacs_enum_declare_value(state, "IDLE", LegacyApi::Engine::UNI_ANIMATION_STATE_IDLE);
        luacs_enum_declare_value(state, "GESTURE", LegacyApi::Engine::UNI_ANIMATION_STATE_GESTURE);
        luacs_enum_declare_value(state, "TURN_LEFT", LegacyApi::Engine::UNI_ANIMATION_STATE_TURN_LEFT);
        luacs_enum_declare_value(state, "TURN_RIGHT", LegacyApi::Engine::UNI_ANIMATION_STATE_TURN_RIGHT);
        luacs_enum_declare_value(state, "MOVE_FRONT", LegacyApi::Engine::UNI_ANIMATION_STATE_MOVE_FRONT);
        luacs_enum_declare_value(state, "MOVE_BACK", LegacyApi::Engine::UNI_ANIMATION_STATE_MOVE_BACK);
        luacs_enum_declare_value(state, "MOVE_LEFT", LegacyApi::Engine::UNI_ANIMATION_STATE_MOVE_LEFT);
        luacs_enum_declare_value(state, "MOVE_RIGHT", LegacyApi::Engine::UNI_ANIMATION_STATE_MOVE_RIGHT);
        luacs_enum_declare_value(state, "STUNNED_FRONT", LegacyApi::Engine::UNI_ANIMATION_STATE_STUNNED_FRONT);
        luacs_enum_declare_value(state, "STUNNED_BACK", LegacyApi::Engine::UNI_ANIMATION_STATE_STUNNED_BACK);
        luacs_enum_declare_value(state, "STUNNED_LEFT", LegacyApi::Engine::UNI_ANIMATION_STATE_STUNNED_LEFT);
        luacs_enum_declare_value(state, "STUNNED_RIGHT", LegacyApi::Engine::UNI_ANIMATION_STATE_STUNNED_RIGHT);
        luacs_enum_declare_value(state, "SLIDE_FRONT", LegacyApi::Engine::UNI_ANIMATION_STATE_SLIDE_FRONT);
        luacs_enum_declare_value(state, "SLIDE_BACK", LegacyApi::Engine::UNI_ANIMATION_STATE_SLIDE_BACK);
        luacs_enum_declare_value(state, "SLIDE_LEFT", LegacyApi::Engine::UNI_ANIMATION_STATE_SLIDE_LEFT);
        luacs_enum_declare_value(state, "SLIDE_RIGHT", LegacyApi::Engine::UNI_ANIMATION_STATE_SLIDE_RIGHT);
        luacs_enum_declare_value(state, "READY", LegacyApi::Engine::UNI_ANIMATION_STATE_READY);
        luacs_enum_declare_value(state, "PUT_AWAY", LegacyApi::Engine::UNI_ANIMATION_STATE_PUT_AWAY);
        luacs_enum_declare_value(state, "AIM_STILL", LegacyApi::Engine::UNI_ANIMATION_STATE_AIM_STILL);
        luacs_enum_declare_value(state, "AIM_MOVE", LegacyApi::Engine::UNI_ANIMATION_STATE_AIM_MOVE);
        luacs_enum_declare_value(state, "AIRBORNE", LegacyApi::Engine::UNI_ANIMATION_STATE_AIRBORNE);
        luacs_enum_declare_value(state, "LAND_SOFT", LegacyApi::Engine::UNI_ANIMATION_STATE_LAND_SOFT);
        luacs_enum_declare_value(state, "LAND_HARD", LegacyApi::Engine::UNI_ANIMATION_STATE_LAND_HARD);
        luacs_enum_declare_value(state, "UNKNOWN23", LegacyApi::Engine::UNI_ANIMATION_STATE_UNKNOWN23);
        luacs_enum_declare_value(state, "AIRBORNE_DEAD", LegacyApi::Engine::UNI_ANIMATION_STATE_AIRBORNE_DEAD);
        luacs_enum_declare_value(state, "LANDING_DEAD", LegacyApi::Engine::UNI_ANIMATION_STATE_LANDING_DEAD);
        luacs_enum_declare_value(state, "SEAT_ENTER", LegacyApi::Engine::UNI_ANIMATION_STATE_SEAT_ENTER);
        luacs_enum_declare_value(state, "SEAT_EXIT", LegacyApi::Engine::UNI_ANIMATION_STATE_SEAT_EXIT);
        luacs_enum_declare_value(state, "CUSTOM_ANIMATION", LegacyApi::Engine::UNI_ANIMATION_STATE_CUSTOM_ANIMATION);
        luacs_enum_declare_value(state, "IMPULSE", LegacyApi::Engine::UNI_ANIMATION_STATE_IMPULSE);
        luacs_enum_declare_value(state, "MELEE", LegacyApi::Engine::UNI_ANIMATION_STATE_MELEE);
        luacs_enum_declare_value(state, "MELEE_AIRBORNE", LegacyApi::Engine::UNI_ANIMATION_STATE_MELEE_AIRBORNE);
        luacs_enum_declare_value(state, "MELEE_CONTINUOUS", LegacyApi::Engine::UNI_ANIMATION_STATE_MELEE_CONTINUOUS);
        luacs_enum_declare_value(state, "THROW_GRENADE", LegacyApi::Engine::UNI_ANIMATION_STATE_THROW_GRENADE);
        luacs_enum_declare_value(state, "RESSURECT_FRONT", LegacyApi::Engine::UNI_ANIMATION_STATE_RESSURECT_FRONT);
        luacs_enum_declare_value(state, "RESSURECT_BACK", LegacyApi::Engine::UNI_ANIMATION_STATE_RESSURECT_BACK);
        luacs_enum_declare_value(state, "FEEDING", LegacyApi::Engine::UNI_ANIMATION_STATE_FEEDING);
        luacs_enum_declare_value(state, "SURPRISE_FRONT", LegacyApi::Engine::UNI_ANIMATION_STATE_SURPRISE_FRONT);
        luacs_enum_declare_value(state, "SURPRISE_BACK", LegacyApi::Engine::UNI_ANIMATION_STATE_SURPRISE_BACK);
        luacs_enum_declare_value(state, "LEAP_START", LegacyApi::Engine::UNI_ANIMATION_STATE_LEAP_START);
        luacs_enum_declare_value(state, "LEAP_AIRBORNE", LegacyApi::Engine::UNI_ANIMATION_STATE_LEAP_AIRBORNE);
        luacs_enum_declare_value(state, "LEAP_MELEE", LegacyApi::Engine::UNI_ANIMATION_STATE_LEAP_MELEE);
        luacs_enum_declare_value(state, "UNKNOWN42", LegacyApi::Engine::UNI_ANIMATION_STATE_UNKNOWN42);
        luacs_enum_declare_value(state, "BERSERK", LegacyApi::Engine::UNI_ANIMATION_STATE_BERSERK);
        luacs_enum_declare_value(state, "YELO_SEAT_BOARDING", LegacyApi::Engine::UNI_ANIMATION_STATE_YELO_SEAT_BOARDING);
        luacs_enum_declare_value(state, "YELO_SEAT_EJECTING", LegacyApi::Engine::UNI_ANIMATION_STATE_YELO_SEAT_EJECTING);
        luacs_enum_declare_value(state, "YELO_MOUNTING", LegacyApi::Engine::UNI_ANIMATION_STATE_YELO_MOUNTING);
        luacs_enum_declare_value(state, "YELO_TRANSFORMING", LegacyApi::Engine::UNI_ANIMATION_STATE_YELO_TRANSFORMING);
        lua_pop(state, 1);
    }

    static void define_engine_unit_replacement_animation_state_enum(lua_State *state) noexcept {
        luacs_newenum(state, EngineUnitReplacementAnimationState);
        luacs_enum_declare_value(state, "NONE", LegacyApi::Engine::UNIT_REPLACEMENT_ANIMATION_STATE_NONE);
        luacs_enum_declare_value(state, "DISARM", LegacyApi::Engine::UNIT_REPLACEMENT_ANIMATION_STATE_DISARM);
        luacs_enum_declare_value(state, "WEAPON_DROP", LegacyApi::Engine::UNIT_REPLACEMENT_ANIMATION_STATE_WEAPON_DROP);
        luacs_enum_declare_value(state, "WEAPON_READY", LegacyApi::Engine::UNIT_REPLACEMENT_ANIMATION_STATE_WEAPON_READY);
        luacs_enum_declare_value(state, "WEAPON_PUT_AWAY", LegacyApi::Engine::UNIT_REPLACEMENT_ANIMATION_STATE_WEAPON_PUT_AWAY);
        luacs_enum_declare_value(state, "WEAPON_RELOAD1", LegacyApi::Engine::UNIT_REPLACEMENT_ANIMATION_STATE_WEAPON_RELOAD1);
        luacs_enum_declare_value(state, "WEAPON_RELOAD2", LegacyApi::Engine::UNIT_REPLACEMENT_ANIMATION_STATE_WEAPON_RELOAD2);
        luacs_enum_declare_value(state, "MELEE", LegacyApi::Engine::UNIT_REPLACEMENT_ANIMATION_STATE_MELEE);
        luacs_enum_declare_value(state, "THROW_GRENADE", LegacyApi::Engine::UNIT_REPLACEMENT_ANIMATION_STATE_THROW_GRENADE);
        lua_pop(state, 1);
    }

    static void define_engine_unit_overlay_animation_state_enum(lua_State *state) noexcept {
        luacs_newenum(state, EngineUnitOverlayAnimationState);
        luacs_enum_declare_value(state, "NONE", LegacyApi::Engine::UNIT_OVERLAY_ANIMATION_STATE_NONE);
        luacs_enum_declare_value(state, "FIRE1", LegacyApi::Engine::UNIT_OVERLAY_ANIMATION_STATE_FIRE1);
        luacs_enum_declare_value(state, "FIRE2", LegacyApi::Engine::UNIT_OVERLAY_ANIMATION_STATE_FIRE2);
        luacs_enum_declare_value(state, "CHARGED1", LegacyApi::Engine::UNIT_OVERLAY_ANIMATION_STATE_CHARGED1);
        luacs_enum_declare_value(state, "CHARGED2", LegacyApi::Engine::UNIT_OVERLAY_ANIMATION_STATE_CHARGED2);
        luacs_enum_declare_value(state, "CHAMBER1", LegacyApi::Engine::UNIT_OVERLAY_ANIMATION_STATE_CHAMBER1);
        luacs_enum_declare_value(state, "CHAMBER2", LegacyApi::Engine::UNIT_OVERLAY_ANIMATION_STATE_CHAMBER2);
        lua_pop(state, 1);
    }

    static void define_engine_unit_base_seat_enum(lua_State *state) noexcept {
        luacs_newenum(state, EngineUnitBaseSeat);
        luacs_enum_declare_value(state, "ASLEEP", LegacyApi::Engine::UNIT_BASE_SEAT_ASLEEP);
        luacs_enum_declare_value(state, "ALERT", LegacyApi::Engine::UNIT_BASE_SEAT_ALERT);
        luacs_enum_declare_value(state, "STAND", LegacyApi::Engine::UNIT_BASE_SEAT_STAND);
        luacs_enum_declare_value(state, "CROUCH", LegacyApi::Engine::UNIT_BASE_SEAT_CROUCH);
        luacs_enum_declare_value(state, "FLEE", LegacyApi::Engine::UNIT_BASE_SEAT_FLEE);
        luacs_enum_declare_value(state, "FLAMING", LegacyApi::Engine::UNIT_BASE_SEAT_FLAMING);
        lua_pop(state, 1);
    }

    static void define_engine_unit_animation_state_data_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineUnitAnimationStateData);
        luacs_int_field(state, EngineUnitAnimationStateData, animation_index, 0);
        luacs_int_field(state, EngineUnitAnimationStateData, frame_index, 0);
        lua_pop(state, 1); 
    }

    void push_engine_unit_animation_state_data(lua_State *state, EngineUnitAnimationStateData *data) noexcept {
        luacs_newobject(state, EngineUnitAnimationStateData, data);
    }

    static void define_engine_unit_animation_flags_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineUnitAnimationFlags); 
        luacs_declare_field(state, LUACS_TINT8, NULL, "flags", sizeof(std::uint8_t), 0, 0, 0); 
        luacs_declare_method(state, "animationBit0Unknown", lua_bitfield_struct_method(state, EngineUnitAnimationFlags, animation_bit0_unknown)); 
        luacs_declare_method(state, "animationBit1Unknown", lua_bitfield_struct_method(state, EngineUnitAnimationFlags, animation_bit1_unknown));
        luacs_declare_method(state, "animationBit2Unknown", lua_bitfield_struct_method(state, EngineUnitAnimationFlags, animation_bit2_unknown));
        luacs_declare_method(state, "animationBit3Unknown", lua_bitfield_struct_method(state, EngineUnitAnimationFlags, animation_bit3_unknown));
        lua_pop(state, 1); 
    }

    void push_engine_unit_animation_flags(lua_State *state, EngineUnitAnimationFlags *flags) noexcept {
        luacs_newobject(state, EngineUnitAnimationFlags, flags);
    }

    static void define_engine_unit_animation_data_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineUnitAnimationData);
        luacs_nested_field(state, EngineUnitAnimationData, EngineUnitAnimationFlags, flags, 0);
        luacs_int_field(state, EngineUnitAnimationData, unknown_some_animation_index_maybe, 0);
        luacs_int_field(state, EngineUnitAnimationData, unknown_some_animation_index, 0);
        luacs_unsigned_field(state, EngineUnitAnimationData, seat_index, 0);
        luacs_unsigned_field(state, EngineUnitAnimationData, seat_weapon_index, 0);
        luacs_unsigned_field(state, EngineUnitAnimationData, weapon_type_index, 0);
        luacs_enum_field(state, EngineUnitAnimationData, EngineUnitAnimationState, state, 0);
        luacs_enum_field(state, EngineUnitAnimationData, EngineUnitReplacementAnimationState, replacement_state, 0);
        luacs_enum_field(state, EngineUnitAnimationData, EngineUnitOverlayAnimationState, overlay_state, 0);
        luacs_enum_field(state, EngineUnitAnimationData, EngineUnitAnimationState, desired_state, 0);
        luacs_enum_field(state, EngineUnitAnimationData, EngineUnitBaseSeat, base_seat, 0);
        luacs_int_field(state, EngineUnitAnimationData, emotion, 0);
        luacs_nested_field(state, EngineUnitAnimationData, EngineUnitAnimationStateData, replacement_animation, 0);
        luacs_nested_field(state, EngineUnitAnimationData, EngineUnitAnimationStateData, overlay_state_animation, 0);
        luacs_nested_field(state, EngineUnitAnimationData, EngineUnitAnimationStateData, weapon_ik, 0);
        luacs_bool_field(state, EngineUnitAnimationData, update_look, 0);
        luacs_bool_field(state, EngineUnitAnimationData, update_aim, 0);
        luacs_nested_field(state, EngineUnitAnimationData, EngineRectangle2DF, looking_bounds, 0);
        luacs_nested_field(state, EngineUnitAnimationData, EngineRectangle2DF, aiming_bounds, 0);
        lua_pop(state, 1);
    }

    void push_engine_unit_animation_data(lua_State *state, EngineUnitAnimationData *data) noexcept {
        luacs_newobject(state, EngineUnitAnimationData, data);
    }

    static void define_engine_unit_speech_priority_enum(lua_State *state) noexcept {
        luacs_newenum(state, EngineUnitSpeechPriority);
        luacs_enum_declare_value(state, "NONE", LegacyApi::Engine::UNIT_SPEECH_PRIORITY_NONE);
        luacs_enum_declare_value(state, "IDLE", LegacyApi::Engine::UNIT_SPEECH_PRIORITY_IDLE);
        luacs_enum_declare_value(state, "PAIN", LegacyApi::Engine::UNIT_SPEECH_PRIORITY_PAIN);
        luacs_enum_declare_value(state, "TALK", LegacyApi::Engine::UNIT_SPEECH_PRIORITY_TALK);
        luacs_enum_declare_value(state, "COMMUNICATE", LegacyApi::Engine::UNIT_SPEECH_PRIORITY_COMMUNICATE);
        luacs_enum_declare_value(state, "SHOUT", LegacyApi::Engine::UNIT_SPEECH_PRIORITY_SHOUT);
        luacs_enum_declare_value(state, "SCRIPT", LegacyApi::Engine::UNIT_SPEECH_PRIORITY_SCRIPT);
        luacs_enum_declare_value(state, "INVOLUNTARY", LegacyApi::Engine::UNIT_SPEECH_PRIORITY_INVOLUNTARY);
        luacs_enum_declare_value(state, "EXLAIM", LegacyApi::Engine::UNIT_SPEECH_PRIORITY_EXLAIM);
        luacs_enum_declare_value(state, "SCREAM", LegacyApi::Engine::UNIT_SPEECH_PRIORITY_SCREAM);
        luacs_enum_declare_value(state, "DEATH", LegacyApi::Engine::UNIT_SPEECH_PRIORITY_DEATH);
        lua_pop(state, 1);
    }

    static void define_engine_unit_scream_type_enum(lua_State *state) noexcept {
        luacs_newenum(state, EngineUnitScreamType);
        luacs_enum_declare_value(state, "FEAR", LegacyApi::Engine::UNIT_SCREAM_TYPE_FEAR);
        luacs_enum_declare_value(state, "ENEMY_GRENADE", LegacyApi::Engine::UNIT_SCREAM_TYPE_ENEMY_GRENADE);
        luacs_enum_declare_value(state, "PAIN", LegacyApi::Engine::UNIT_SCREAM_TYPE_PAIN);
        luacs_enum_declare_value(state, "MAIMED_LIMB", LegacyApi::Engine::UNIT_SCREAM_TYPE_MAIMED_LIMB);
        luacs_enum_declare_value(state, "MAIMED_HEAD", LegacyApi::Engine::UNIT_SCREAM_TYPE_MAIMED_HEAD);
        luacs_enum_declare_value(state, "RESSURECTION", LegacyApi::Engine::UNIT_SCREAM_TYPE_RESSURECTION);
        lua_pop(state, 1);
    }

    static void define_engine_ai_communication_type_enum(lua_State *state) noexcept {
        luacs_newenum(state, EngineAiCommunicationType);
        luacs_enum_declare_value(state, "DEATH", LegacyApi::Engine::AI_COMMUNICATION_TYPE_DEATH);
        luacs_enum_declare_value(state, "SPREE", LegacyApi::Engine::AI_COMMUNICATION_TYPE_SPREE);
        luacs_enum_declare_value(state, "HURT", LegacyApi::Engine::AI_COMMUNICATION_TYPE_HURT);
        luacs_enum_declare_value(state, "DAMAGE", LegacyApi::Engine::AI_COMMUNICATION_TYPE_DAMAGE);
        luacs_enum_declare_value(state, "SIGHTED_ENEMY", LegacyApi::Engine::AI_COMMUNICATION_TYPE_SIGHTED_ENEMY);
        luacs_enum_declare_value(state, "FOUND_ENEMY", LegacyApi::Engine::AI_COMMUNICATION_TYPE_FOUND_ENEMY);
        luacs_enum_declare_value(state, "UNEXPECTED_ENEMY", LegacyApi::Engine::AI_COMMUNICATION_TYPE_UNEXPECTED_ENEMY);
        luacs_enum_declare_value(state, "FOUND_DEAD_FRIEND", LegacyApi::Engine::AI_COMMUNICATION_TYPE_FOUND_DEAD_FRIEND);
        luacs_enum_declare_value(state, "ALLEGIANCE_CHANGED", LegacyApi::Engine::AI_COMMUNICATION_TYPE_ALLEGIANCE_CHANGED);
        luacs_enum_declare_value(state, "GRENADE_THROWING", LegacyApi::Engine::AI_COMMUNICATION_TYPE_GRENADE_THROWING);
        luacs_enum_declare_value(state, "GRENADE_STARTLE", LegacyApi::Engine::AI_COMMUNICATION_TYPE_GRENADE_STARTLE);
        luacs_enum_declare_value(state, "GRENADE_SIGHTED", LegacyApi::Engine::AI_COMMUNICATION_TYPE_GRENADE_SIGHTED);
        luacs_enum_declare_value(state, "GRENADE_DANGER", LegacyApi::Engine::AI_COMMUNICATION_TYPE_GRENADE_DANGER);
        luacs_enum_declare_value(state, "LOST_CONTACT", LegacyApi::Engine::AI_COMMUNICATION_TYPE_LOST_CONTACT);
        luacs_enum_declare_value(state, "BLOCKED", LegacyApi::Engine::AI_COMMUNICATION_TYPE_BLOCKED);
        luacs_enum_declare_value(state, "ALERT_NONCOMBAT", LegacyApi::Engine::AI_COMMUNICATION_TYPE_ALERT_NONCOMBAT);
        luacs_enum_declare_value(state, "SEARCH_START", LegacyApi::Engine::AI_COMMUNICATION_TYPE_SEARCH_START);
        luacs_enum_declare_value(state, "SEARCH_QUERY", LegacyApi::Engine::AI_COMMUNICATION_TYPE_SEARCH_QUERY);
        luacs_enum_declare_value(state, "SEARCH_REPORT", LegacyApi::Engine::AI_COMMUNICATION_TYPE_SEARCH_REPORT);
        luacs_enum_declare_value(state, "SEARCH_ABANDON", LegacyApi::Engine::AI_COMMUNICATION_TYPE_SEARCH_ABANDON);
        luacs_enum_declare_value(state, "SEARCH_GROUP_ABANDON", LegacyApi::Engine::AI_COMMUNICATION_TYPE_SEARCH_GROUP_ABANDON);
        luacs_enum_declare_value(state, "UNCOVER_START", LegacyApi::Engine::AI_COMMUNICATION_TYPE_UNCOVER_START);
        luacs_enum_declare_value(state, "ADVANCE", LegacyApi::Engine::AI_COMMUNICATION_TYPE_ADVANCE);
        luacs_enum_declare_value(state, "RETREAT", LegacyApi::Engine::AI_COMMUNICATION_TYPE_RETREAT);
        luacs_enum_declare_value(state, "COVER", LegacyApi::Engine::AI_COMMUNICATION_TYPE_COVER);
        luacs_enum_declare_value(state, "SIGHTED_FRIEND_PLAYER", LegacyApi::Engine::AI_COMMUNICATION_TYPE_SIGHTED_FRIEND_PLAYER);
        luacs_enum_declare_value(state, "SHOOTING", LegacyApi::Engine::AI_COMMUNICATION_TYPE_SHOOTING);
        luacs_enum_declare_value(state, "SHOOTING_VEHICLE", LegacyApi::Engine::AI_COMMUNICATION_TYPE_SHOOTING_VEHICLE);
        luacs_enum_declare_value(state, "SHOOTING_BERSERK", LegacyApi::Engine::AI_COMMUNICATION_TYPE_SHOOTING_BERSERK);
        luacs_enum_declare_value(state, "SHOOTING_GROUP", LegacyApi::Engine::AI_COMMUNICATION_TYPE_SHOOTING_GROUP);
        luacs_enum_declare_value(state, "SHOOTING_TRAITOR", LegacyApi::Engine::AI_COMMUNICATION_TYPE_SHOOTING_TRAITOR);
        luacs_enum_declare_value(state, "FLEE", LegacyApi::Engine::AI_COMMUNICATION_TYPE_FLEE);
        luacs_enum_declare_value(state, "FLEE_LEADER_DIED", LegacyApi::Engine::AI_COMMUNICATION_TYPE_FLEE_LEADER_DIED);
        luacs_enum_declare_value(state, "FLEE_IDLE", LegacyApi::Engine::AI_COMMUNICATION_TYPE_FLEE_IDLE);
        luacs_enum_declare_value(state, "ATTEMPTED_FLEE", LegacyApi::Engine::AI_COMMUNICATION_TYPE_ATTEMPTED_FLEE);
        luacs_enum_declare_value(state, "HIDING_FINISHED", LegacyApi::Engine::AI_COMMUNICATION_TYPE_HIDING_FINISHED);
        luacs_enum_declare_value(state, "VEHICLE_ENTRY", LegacyApi::Engine::AI_COMMUNICATION_TYPE_VEHICLE_ENTRY);
        luacs_enum_declare_value(state, "VEHICLE_EXIT", LegacyApi::Engine::AI_COMMUNICATION_TYPE_VEHICLE_EXIT);
        luacs_enum_declare_value(state, "VEHICLE_WOOHOO", LegacyApi::Engine::AI_COMMUNICATION_TYPE_VEHICLE_WOOHOO);
        luacs_enum_declare_value(state, "VEHICLE_SCARED", LegacyApi::Engine::AI_COMMUNICATION_TYPE_VEHICLE_SCARED);
        luacs_enum_declare_value(state, "VEHICLE_FALLING", LegacyApi::Engine::AI_COMMUNICATION_TYPE_VEHICLE_FALLING);
        luacs_enum_declare_value(state, "SURPRISE", LegacyApi::Engine::AI_COMMUNICATION_TYPE_SURPRISE);
        luacs_enum_declare_value(state, "BERSERK", LegacyApi::Engine::AI_COMMUNICATION_TYPE_BERSERK);
        luacs_enum_declare_value(state, "MELEE", LegacyApi::Engine::AI_COMMUNICATION_TYPE_MELEE);
        luacs_enum_declare_value(state, "DIVE", LegacyApi::Engine::AI_COMMUNICATION_TYPE_DIVE);
        luacs_enum_declare_value(state, "UNCONVER_EXCLAMATION", LegacyApi::Engine::AI_COMMUNICATION_TYPE_UNCONVER_EXCLAMATION);
        luacs_enum_declare_value(state, "FALLING", LegacyApi::Engine::AI_COMMUNICATION_TYPE_FALLING);
        luacs_enum_declare_value(state, "LEAP", LegacyApi::Engine::AI_COMMUNICATION_TYPE_LEAP);
        luacs_enum_declare_value(state, "POSTCOMBAT_ALONE", LegacyApi::Engine::AI_COMMUNICATION_TYPE_POSTCOMBAT_ALONE);
        luacs_enum_declare_value(state, "POSTCOMBAT_UNSCATHED", LegacyApi::Engine::AI_COMMUNICATION_TYPE_POSTCOMBAT_UNSCATHED);
        luacs_enum_declare_value(state, "POSTCOMBAT_WOUNDED", LegacyApi::Engine::AI_COMMUNICATION_TYPE_POSTCOMBAT_WOUNDED);
        luacs_enum_declare_value(state, "POSTCOMBAT_MASSACRE", LegacyApi::Engine::AI_COMMUNICATION_TYPE_POSTCOMBAT_MASSACRE);
        luacs_enum_declare_value(state, "POSTCOMBAT_TRIUMPH", LegacyApi::Engine::AI_COMMUNICATION_TYPE_POSTCOMBAT_TRIUMPH);
        luacs_enum_declare_value(state, "POSTCOMBAT_CHECK_ENEMY", LegacyApi::Engine::AI_COMMUNICATION_TYPE_POSTCOMBAT_CHECK_ENEMY);
        luacs_enum_declare_value(state, "POSTCOMBAT_CHECK_FRIEND", LegacyApi::Engine::AI_COMMUNICATION_TYPE_POSTCOMBAT_CHECK_FRIEND);
        luacs_enum_declare_value(state, "POSTCOMBAT_SHOOT_CORPSE", LegacyApi::Engine::AI_COMMUNICATION_TYPE_POSTCOMBAT_SHOOT_CORPSE);
        luacs_enum_declare_value(state, "POSTCOMBAT_CELEBRATE", LegacyApi::Engine::AI_COMMUNICATION_TYPE_POSTCOMBAT_CELEBRATE);
        lua_pop(state, 1);
    }

    static void define_engine_ai_communication_packet_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineAiCommunicationPacket);
        luacs_enum_field(state, EngineAiCommunicationPacket, EngineAiCommunicationType, type, 0);
        luacs_bool_field(state, EngineAiCommunicationPacket, broken, 0);
        lua_pop(state, 1); 
    }

    void push_engine_ai_communication_packet(lua_State *state, EngineAiCommunicationPacket *packet) noexcept {
        luacs_newobject(state, EngineAiCommunicationPacket, packet);
    }

    static void define_engine_unit_speech_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineUnitSpeech);
        luacs_enum_field(state, EngineUnitSpeech, EngineUnitSpeechPriority, priority, 0);
        luacs_enum_field(state, EngineUnitSpeech, EngineUnitScreamType, scream_type, 0);
        luacs_nested_field(state, EngineUnitSpeech, EngineResourceHandle, sound_tag, 0);
        luacs_unsigned_field(state, EngineUnitSpeech, ticks, 0);
        luacs_int_field(state, EngineUnitSpeech, unknown2, 0);
        luacs_int_field(state, EngineUnitSpeech, unknown, 0);
        luacs_nested_field(state, EngineUnitSpeech, EngineAiCommunicationPacket, ai_communication_info, 0);
        lua_pop(state, 1); 
    }

    void push_engine_unit_speech(lua_State *state, EngineUnitSpeech *speech) noexcept {
        luacs_newobject(state, EngineUnitSpeech, speech);
    }

    static void define_engine_unit_speech_data_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineUnitSpeechData);
        luacs_nested_field(state, EngineUnitSpeechData, EngineUnitSpeech, current, 0);
        luacs_nested_field(state, EngineUnitSpeechData, EngineUnitSpeech, next, 0);
        luacs_int_field(state, EngineUnitSpeechData, unknown0, 0);
        luacs_int_field(state, EngineUnitSpeechData, unknown1, 0);
        luacs_int_field(state, EngineUnitSpeechData, unknown2, 0);
        luacs_int_field(state, EngineUnitSpeechData, unknown3, 0);
        luacs_int_field(state, EngineUnitSpeechData, unknown4, 0);
        luacs_bool_field(state, EngineUnitSpeechData, unknown6, 0);
        luacs_bool_field(state, EngineUnitSpeechData, unknown7, 0);
        luacs_bool_field(state, EngineUnitSpeechData, unknown8, 0);
        luacs_int_field(state, EngineUnitSpeechData, unknown9, 0);
        luacs_int_field(state, EngineUnitSpeechData, unknown10, 0);
        luacs_int_field(state, EngineUnitSpeechData, unknown11, 0);
        luacs_int_field(state, EngineUnitSpeechData, unknown12, 0);
        luacs_int_field(state, EngineUnitSpeechData, unknown13, 0);
        lua_pop(state, 1); 
    }

    void push_engine_unit_speech_data(lua_State *state, EngineUnitSpeechData *data) noexcept {
        luacs_newobject(state, EngineUnitSpeechData, data);
    }

    static void define_engine_unit_control_data_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineUnitControlData);
        luacs_int_field(state, EngineUnitControlData, animation_state, 0);
        luacs_int_field(state, EngineUnitControlData, aiming_speed, 0);
        luacs_nested_field(state, EngineUnitControlData, EngineUnitControlFlags, control_flags, 0);
        luacs_int_field(state, EngineUnitControlData, weapon_index, 0);
        luacs_int_field(state, EngineUnitControlData, grenade_index, 0);
        luacs_int_field(state, EngineUnitControlData, zoom_index, 0);
        luacs_nested_field(state, EngineUnitControlData, EngineVector3D, throttle, 0);
        luacs_float_field(state, EngineUnitControlData, primary_trigger, 0);
        luacs_nested_field(state, EngineUnitControlData, EngineVector3D, facing_vector, 0);
        luacs_nested_field(state, EngineUnitControlData, EngineVector3D, aiming_vector, 0);
        luacs_nested_field(state, EngineUnitControlData, EngineVector3D, looking_vector, 0);
        lua_pop(state, 1); 
    }

    void push_engine_unit_control_data(lua_State *state, EngineUnitControlData *data) noexcept {
        luacs_newobject(state, EngineUnitControlData, data);
    }

    static void define_engine_unit_object_struct(lua_State *state) noexcept {
        luacs_newderivedstruct(state, EngineUnitObject, EngineBaseObject);
        luacs_nested_field(state, EngineUnitObject, EngineResourceHandle, actor, 0);
        luacs_nested_field(state, EngineUnitObject, EngineResourceHandle, swarm_actor, 0);
        luacs_nested_field(state, EngineUnitObject, EngineResourceHandle, swarm_next_unit, 0);
        luacs_nested_field(state, EngineUnitObject, EngineResourceHandle, swarm_previous_unit, 0);
        luacs_nested_field(state, EngineUnitObject, EngineUnitObjectFlags, unit_flags, 0);
        luacs_nested_field(state, EngineUnitObject, EngineUnitControlFlags, unit_control_flags, 0);
        luacs_int_field(state, EngineUnitObject, shield_snapping, 0);
        luacs_int_field(state, EngineUnitObject, base_seat_index, 0);
        luacs_declare_field(state, LUACS_TINT32, NULL, "persistentControlTicksRemaining", sizeof(((EngineUnitObject *)0)->persistent_control.ticks_remaining), OFFSET_OF(EngineUnitObject, persistent_control.ticks_remaining), 0, 0);
        luacs_declare_field(state, LUACS_TOBJENT, "EngineUnitControlFlags", "persistentControlFlags", sizeof(((EngineUnitObject *)0)->persistent_control.control_flags), OFFSET_OF(EngineUnitObject, persistent_control.control_flags), 0, 0);
        luacs_nested_field(state, EngineUnitObject, EngineResourceHandle, controlling_player, 0);
        luacs_int_field(state, EngineUnitObject, ai_effect_type, 0);
        luacs_int_field(state, EngineUnitObject, emotion_animation_index, 0);
        luacs_unsigned_field(state, EngineUnitObject, next_ai_effect_tick, 0);
        luacs_nested_field(state, EngineUnitObject, EngineVector3D, desired_facing_vector, 0);
        luacs_nested_field(state, EngineUnitObject, EngineVector3D, desired_aiming_vector, 0);
        luacs_nested_field(state, EngineUnitObject, EngineVector3D, aiming_vector, 0);
        luacs_nested_field(state, EngineUnitObject, EngineVector3D, aiming_velocity, 0);
        luacs_nested_field(state, EngineUnitObject, EngineEuler3D, looking_angles, 0);
        luacs_nested_field(state, EngineUnitObject, EngineVector3D, looking_vector, 0);
        luacs_nested_field(state, EngineUnitObject, EngineVector3D, looking_velocity, 0);
        luacs_nested_field(state, EngineUnitObject, EngineVector3D, throttle, 0);
        luacs_float_field(state, EngineUnitObject, primary_trigger, 0);
        luacs_int_field(state, EngineUnitObject, aiming_speed, 0);
        luacs_int_field(state, EngineUnitObject, melee_state, 0);
        luacs_int_field(state, EngineUnitObject, melee_timer, 0);
        luacs_int_field(state, EngineUnitObject, ticks_until_flame_to_death, 0);
        luacs_int_field(state, EngineUnitObject, ping_animation_ticks_left, 0);
        luacs_enum_field(state, EngineUnitObject, EngineUnitThrowingGrenadeState, grenade_state, 0);
        luacs_int_field(state, EngineUnitObject, unknown_725, 0);
        luacs_int_field(state, EngineUnitObject, unknown_726, 0);
        luacs_nested_field(state, EngineUnitObject, EngineResourceHandle, grenade_projectile, 0);
        luacs_nested_field(state, EngineUnitObject, EngineUnitAnimationData, animation, 0);
        luacs_float_field(state, EngineUnitObject, ambient, 0);
        luacs_float_field(state, EngineUnitObject, illumination, 0);
        luacs_float_field(state, EngineUnitObject, mouth_factor, 0);
        luacs_int_field(state, EngineUnitObject, vehicle_seat_id, 0);
        luacs_int_field(state, EngineUnitObject, current_weapon_id, 0);
        luacs_int_field(state, EngineUnitObject, next_weapon_id, 0);
        luacs_nested_array_field(state, EngineUnitObject, EngineResourceHandle, weapons, 0);
        luacs_int_array_field(state, EngineUnitObject, weapon_ready_ticks, 0);
        luacs_nested_field(state, EngineUnitObject, EngineResourceHandle, equipment_handle, 0);
        luacs_int_field(state, EngineUnitObject, current_grenade_index, 0);
        luacs_int_field(state, EngineUnitObject, next_grenade_index, 0);
        luacs_int_array_field(state, EngineUnitObject, grenade_counts, 0);
        luacs_unsigned_field(state, EngineUnitObject, zoom_level, 0);
        luacs_unsigned_field(state, EngineUnitObject, desired_zoom_level, 0);
        luacs_int_field(state, EngineUnitObject, ticks_since_last_vehicle_speech, 0);
        luacs_unsigned_field(state, EngineUnitObject, aiming_change, 0);
        luacs_nested_array_field(state, EngineUnitObject, EngineResourceHandle, powered_seats_riders, 0);
        luacs_nested_field(state, EngineUnitObject, EngineResourceHandle, _unknown22, 0);
        luacs_int_field(state, EngineUnitObject, _some_tick_time, 0);
        luacs_int_field(state, EngineUnitObject, encounter_id, 0);
        luacs_int_field(state, EngineUnitObject, squad_id, 0);
        luacs_float_array_field(state, EngineUnitObject, powered_seats_power, 0);
        luacs_float_field(state, EngineUnitObject, integrated_light_power, 0);
        luacs_float_field(state, EngineUnitObject, integrated_light_toggle_power, 0);
        luacs_float_field(state, EngineUnitObject, integrated_night_vision_toggle_power, 0);
        luacs_nested_array_field(state, EngineUnitObject, EngineVector3D, seat_related, 0);
        luacs_float_field(state, EngineUnitObject, camo_power, 0);
        luacs_float_field(state, EngineUnitObject, full_spectrum_vision_power, 0);
        luacs_nested_field(state, EngineUnitObject, EngineResourceHandle, dialogue_definition, 0);
        luacs_nested_field(state, EngineUnitObject, EngineUnitSpeechData, speech, 0);
        luacs_declare_field(state, LUACS_TENUM, "EngineDamageEffectCategory", "damageResultCategory", sizeof(((EngineUnitObject *)0)->damage_result.category), OFFSET_OF(EngineUnitObject, damage_result.category), 0, 0);
        luacs_declare_field(state, LUACS_TUINT16, NULL, "aiTicksUntilHandle", sizeof(((EngineUnitObject *)0)->damage_result.ai_ticks_until_handle), OFFSET_OF(EngineUnitObject, damage_result.ai_ticks_until_handle), 0, 0);
        luacs_declare_field(state, LUACS_FLOAT, NULL, "amount", sizeof(((EngineUnitObject *)0)->damage_result.amount), OFFSET_OF(EngineUnitObject, damage_result.amount), 0, 0);
        luacs_declare_field(state, LUACS_TOBJENT, "EngineResourceHandle", "responsibleUnit", sizeof(((EngineUnitObject *)0)->damage_result.responsible_unit), OFFSET_OF(EngineUnitObject, damage_result.responsible_unit), 0, 0);
        luacs_nested_field(state, EngineUnitObject, EngineResourceHandle, object_flame_causer, 0);
        luacs_float_field(state, EngineUnitObject, _unknown23, 0);
        luacs_unsigned_field(state, EngineUnitObject, died_at_tick, 0);
        luacs_unsigned_field(state, EngineUnitObject, feign_death_timer, 0);
        luacs_bool_field(state, EngineUnitObject, camo_regrowth, 0);
        luacs_float_field(state, EngineUnitObject, stun, 0);
        luacs_unsigned_field(state, EngineUnitObject, stun_ticks, 0);
        luacs_int_field(state, EngineUnitObject, spree_count, 0);
        luacs_int_field(state, EngineUnitObject, spree_starting_time, 0);
        luacs_nested_array_field(state, EngineUnitObject, EngineUnitRecentDamager, recent_damage, 0);
        luacs_unsigned_field(state, EngineUnitObject, opensauce_zoom_level, 0);
        luacs_unsigned_field(state, EngineUnitObject, opensauce_desired_zoom_level, 0);
        luacs_nested_field(state, EngineUnitObject, EngineUnitControlData, control_data, 0);
        luacs_bool_field(state, EngineUnitObject, last_completed_client_update_valid, 0);
        luacs_int_field(state, EngineUnitObject, last_completed_client_update_id, 0);
        lua_pop(state, 1); 
    }

    void push_engine_unit_object(lua_State *state, EngineUnitObject *object) noexcept {
        luacs_newobject(state, EngineUnitObject, object);
    }

    static void define_engine_biped_flags_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineBipedObjectFlags); 
        luacs_declare_field(state, LUACS_TINT32, NULL, "flags", sizeof(std::uint32_t), 0, 0, 0); 
        luacs_declare_method(state, "airborne", lua_bitfield_struct_method(state, EngineBipedObjectFlags, airborne)); 
        luacs_declare_method(state, "slipping", lua_bitfield_struct_method(state, EngineBipedObjectFlags, slipping));
        luacs_declare_method(state, "absoluteMovement", lua_bitfield_struct_method(state, EngineBipedObjectFlags, absolute_movement));
        luacs_declare_method(state, "noCollision", lua_bitfield_struct_method(state, EngineBipedObjectFlags, no_collision));
        luacs_declare_method(state, "passesThroughOtherBipeds", lua_bitfield_struct_method(state, EngineBipedObjectFlags, passes_through_other_bipeds));
        luacs_declare_method(state, "limping2", lua_bitfield_struct_method(state, EngineBipedObjectFlags, limping2));
        lua_pop(state, 1); 
    }

    void push_engine_biped_flags(lua_State *state, EngineBipedObjectFlags *flags) noexcept {
        luacs_newobject(state, EngineBipedObjectFlags, flags);
    }

    static void define_engine_biped_movement_state_enum(lua_State *state) noexcept {
        luacs_newenum(state, EngineBipedMovementState);
        luacs_enum_declare_value(state, "MOVING", LegacyApi::Engine::BIPED_MOVEMENT_STATE_MOVING);
        luacs_enum_declare_value(state, "IDLE", LegacyApi::Engine::BIPED_MOVEMENT_STATE_IDLE);
        luacs_enum_declare_value(state, "GESTURING", LegacyApi::Engine::BIPED_MOVEMENT_STATE_GESTURING);
        lua_pop(state, 1);
    }

    static void define_engine_biped_network_delta_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineBipedNetworkDelta);
        luacs_int_array_field(state, EngineBipedNetworkDelta, grenade_counts, 0);
        luacs_float_field(state, EngineBipedNetworkDelta, body_vitality, 0);
        luacs_float_field(state, EngineBipedNetworkDelta, shield_vitality, 0);
        luacs_bool_field(state, EngineBipedNetworkDelta, shield_stun_ticks_greater_than_zero, 0);
        lua_pop(state, 1);
    }

    void push_engine_biped_network_delta(lua_State *state, EngineBipedNetworkDelta *delta) noexcept {
        luacs_newobject(state, EngineBipedNetworkDelta, delta);
    }

    static void define_engine_biped_network_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineBipedNetwork);
        luacs_bool_field(state, EngineBipedNetwork, baseline_valid, 0);
        luacs_int_field(state, EngineBipedNetwork, baseline_id, 0);
        luacs_int_field(state, EngineBipedNetwork, message_id, 0);
        luacs_nested_field(state, EngineBipedNetwork, EngineBipedNetworkDelta, update_baseline, 0);
        luacs_bool_field(state, EngineBipedNetwork, delta_valid, 0);
        luacs_nested_field(state, EngineBipedNetwork, EngineBipedNetworkDelta, update_delta, 0);
        lua_pop(state, 1); 
    }

    void push_engine_biped_network(lua_State *state, EngineBipedNetwork *network) noexcept {
        luacs_newobject(state, EngineBipedNetwork, network);
    }

    static void define_engine_biped_object_struct(lua_State *state) noexcept {
        luacs_newderivedstruct(state, EngineBipedObject, EngineUnitObject);
        luacs_nested_field(state, EngineBipedObject, EngineBipedFlags, biped_flags, 0);
        luacs_int_field(state, EngineBipedObject, landing_timer, 0);
        luacs_int_field(state, EngineBipedObject, landing_force, 0);
        luacs_enum_field(state, EngineBipedObject, EngineBipedMovementState, movement_state, 0);
        luacs_int_field(state, EngineBipedObject, _biped_unknown3, 0);
        luacs_unsigned_field(state, EngineBipedObject, action_flags, 0);
        luacs_int_field(state, EngineBipedObject, _biped_unknown4, 0);
        luacs_nested_field(state, EngineBipedObject, EngineVector3D, biped_position, 0);
        luacs_int_field(state, EngineBipedObject, walking_counter, 0);
        luacs_nested_field(state, EngineBipedObject, EngineResourceHandle, bump_object, 0);
        luacs_int_field(state, EngineBipedObject, ticks_since_last_bump, 0);
        luacs_int_field(state, EngineBipedObject, airborne_ticks, 0);
        luacs_int_field(state, EngineBipedObject, slipping_ticks, 0);
        luacs_int_field(state, EngineBipedObject, digital_throttle, 0);
        luacs_int_field(state, EngineBipedObject, jump_ticks, 0);
        luacs_int_field(state, EngineBipedObject, melee_ticks, 0);
        luacs_int_field(state, EngineBipedObject, melee_inflict_ticks, 0);
        luacs_int_field(state, EngineBipedObject, unknown_biped2, 0);
        luacs_float_field(state, EngineBipedObject, crouch_scale, 0);
        luacs_float_field(state, EngineBipedObject, unknown_biped1, 0);
        luacs_nested_field(state, EngineBipedObject, EnginePlane3D, unknown_biped_physics_related, 0);
        luacs_nested_field(state, EngineBipedObject, EngineBipedNetwork, network, 0);
        lua_pop(state, 1); 
    }

    void push_engine_biped_object(lua_State *state, EngineBipedObject *object) noexcept {
        luacs_newobject(state, EngineBipedObject, object);
    }

    static void define_engine_vehicle_flags_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineVehicleObjectFlags); 
        luacs_declare_field(state, LUACS_TINT8, NULL, "flags", sizeof(std::uint8_t), 0, 0, 0); 
        luacs_declare_method(state, "vehicleUnknown0", lua_bitfield_struct_method(state, EngineVehicleObjectFlags, vehicle_unknown0)); 
        luacs_declare_method(state, "hovering", lua_bitfield_struct_method(state, EngineVehicleObjectFlags, hovering));
        luacs_declare_method(state, "crouched", lua_bitfield_struct_method(state, EngineVehicleObjectFlags, crouched));
        luacs_declare_method(state, "jumping", lua_bitfield_struct_method(state, EngineVehicleObjectFlags, jumping));
        luacs_declare_method(state, "unknownVehicle1", lua_bitfield_struct_method(state, EngineVehicleObjectFlags, unknown_vehicle1));
        luacs_declare_method(state, "unknownVehicle2", lua_bitfield_struct_method(state, EngineVehicleObjectFlags, unknown_vehicle2));
        lua_pop(state, 1); 
    }

    void push_engine_vehicle_flags(lua_State *state, EngineVehicleObjectFlags *flags) noexcept {
        luacs_newobject(state, EngineVehicleObjectFlags, flags);
    }

    static void define_engine_vehicle_network_data_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineVehicleNetworkData);
        luacs_bool_field(state, EngineVehicleNetworkData, at_rest, 0);
        luacs_nested_field(state, EngineVehicleNetworkData, EngineVector3D, position, 0);
        luacs_nested_field(state, EngineVehicleNetworkData, EngineVector3D, transitional_velocity, 0);
        luacs_nested_field(state, EngineVehicleNetworkData, EngineVector3D, angular_velocity, 0);
        luacs_nested_field(state, EngineVehicleNetworkData, EngineVector3D, forward, 0);
        luacs_nested_field(state, EngineVehicleNetworkData, EngineVector3D, up, 0);
        lua_pop(state, 1); 
    }

    void push_engine_vehicle_network_data(lua_State *state, EngineVehicleNetworkData *data) noexcept {
        luacs_newobject(state, EngineVehicleNetworkData, data);
    }

    static void define_engine_vehicle_network_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineVehicleNetwork);
        luacs_bool_field(state, EngineVehicleNetwork, time_valid, 0);
        luacs_bool_field(state, EngineVehicleNetwork, baseline_valid, 0);
        luacs_int_field(state, EngineVehicleNetwork, baseline_id, 0);
        luacs_int_field(state, EngineVehicleNetwork, message_id, 0);
        luacs_nested_field(state, EngineVehicleNetwork, EngineVehicleNetworkData, update_baseline, 0);
        luacs_bool_field(state, EngineVehicleNetwork, delta_valid, 0);
        luacs_nested_field(state, EngineVehicleNetwork, EngineVehicleNetworkData, update_delta, 0);
        luacs_unsigned_field(state, EngineVehicleNetwork, last_moved_at_tick, 0);
        luacs_int_field(state, EngineVehicleNetwork, scenario_respawn_id, 0);
        luacs_nested_field(state, EngineVehicleNetwork, EngineVector3D, respawn_position, 0);
        lua_pop(state, 1);
    }

    void push_engine_vehicle_network(lua_State *state, EngineVehicleNetwork *network) noexcept {
        luacs_newobject(state, EngineVehicleNetwork, network);
    }

    static void define_engine_vehicle_object_struct(lua_State *state) noexcept {
        luacs_newderivedstruct(state, EngineVehicleObject, EngineUnitObject);
        luacs_nested_field(state, EngineVehicleObject, EngineVehicleObjectFlags, vehicle_flags, 0);
        luacs_float_field(state, EngineVehicleObject, speed, 0);
        luacs_float_field(state, EngineVehicleObject, slide, 0);
        luacs_float_field(state, EngineVehicleObject, turn, 0);
        luacs_float_field(state, EngineVehicleObject, tire_position, 0);
        luacs_float_field(state, EngineVehicleObject, thread_position_left, 0);
        luacs_float_field(state, EngineVehicleObject, thread_position_right, 0);
        luacs_float_field(state, EngineVehicleObject, hover, 0);
        luacs_float_field(state, EngineVehicleObject, thrust, 0);
        luacs_int_array_field(state, EngineVehicleObject, suspension_states, 0);
        luacs_nested_field(state, EngineVehicleObject, EngineVector3D, hover_position, 0);
        luacs_nested_field(state, EngineVehicleObject, EngineVector3D, unknown_vehicle3, 0);
        luacs_nested_field(state, EngineVehicleObject, EngineVector3D, unknown_vehicle4, 0);
        luacs_int_field(state, EngineVehicleObject, unknown_vehicle5, 0);
        luacs_nested_field(state, EngineVehicleObject, EngineVehicleNetwork, network, 0);
        lua_pop(state, 1); 
    }

    void push_engine_vehicle_object(lua_State *state, EngineVehicleObject *object) noexcept {
        luacs_newobject(state, EngineVehicleObject, object);
    }

    void define_engine_gamestate_unit_object_types(lua_State *state) noexcept {
        define_engine_unit_throwing_grenade_state_enum(state);
        define_engine_unit_animation_state_enum(state);
        define_engine_unit_replacement_animation_state_enum(state);
        define_engine_unit_overlay_animation_state_enum(state);
        define_engine_unit_base_seat_enum(state);
        define_engine_unit_speech_priority_enum(state);
        define_engine_unit_scream_type_enum(state);
        define_engine_ai_communication_type_enum(state);
        define_engine_biped_movement_state_enum(state);
        define_engine_unit_recent_damager_struct(state);
        define_engine_unit_flags_struct(state);
        define_engine_unit_control_flags_struct(state);
        define_engine_unit_animation_state_data_struct(state);
        define_engine_unit_animation_flags_struct(state);
        define_engine_unit_animation_data_struct(state);
        define_engine_ai_communication_packet_struct(state);
        define_engine_unit_speech_struct(state);
        define_engine_unit_speech_data_struct(state);
        define_engine_unit_control_data_struct(state);
        define_engine_unit_object_struct(state);
        define_engine_biped_flags_struct(state);
        define_engine_biped_network_delta_struct(state);
        define_engine_biped_network_struct(state);
        define_engine_biped_object_struct(state);
        define_engine_vehicle_flags_struct(state);
        define_engine_vehicle_network_data_struct(state);
        define_engine_vehicle_network_struct(state);
        define_engine_vehicle_object_struct(state);
    }
}
