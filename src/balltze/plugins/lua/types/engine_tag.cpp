// SPDX-License-Identifier: GPL-3.0-only

#include <luacstruct/luacstruct.h>
#include "engine_tag.hpp"

namespace Balltze::Plugins::Lua {
    using namespace Engine;

    void lua_define_engine_tag_class_enum(lua_State *state) noexcept {
        luacs_newenum(state, EngineTagClass);
        luacs_enum_declare_value(state, "ACTOR", TAG_CLASS_ACTOR);
        luacs_enum_declare_value(state, "ACTOR_VARIANT", TAG_CLASS_ACTOR_VARIANT);
        luacs_enum_declare_value(state, "ANTENNA", TAG_CLASS_ANTENNA);
        luacs_enum_declare_value(state, "MODEL_ANIMATIONS", TAG_CLASS_MODEL_ANIMATIONS);
        luacs_enum_declare_value(state, "BIPED", TAG_CLASS_BIPED);
        luacs_enum_declare_value(state, "BITMAP", TAG_CLASS_BITMAP);
        luacs_enum_declare_value(state, "SPHEROID", TAG_CLASS_SPHEROID);
        luacs_enum_declare_value(state, "CONTINUOUS_DAMAGE_EFFECT", TAG_CLASS_CONTINUOUS_DAMAGE_EFFECT);
        luacs_enum_declare_value(state, "MODEL_COLLISION_GEOMETRY", TAG_CLASS_MODEL_COLLISION_GEOMETRY);
        luacs_enum_declare_value(state, "COLOR_TABLE", TAG_CLASS_COLOR_TABLE);
        luacs_enum_declare_value(state, "CONTRAIL", TAG_CLASS_CONTRAIL);
        luacs_enum_declare_value(state, "DEVICE_CONTROL", TAG_CLASS_DEVICE_CONTROL);
        luacs_enum_declare_value(state, "DECAL", TAG_CLASS_DECAL);
        luacs_enum_declare_value(state, "UI_WIDGET_DEFINITION", TAG_CLASS_UI_WIDGET_DEFINITION);
        luacs_enum_declare_value(state, "INPUT_DEVICE_DEFAULTS", TAG_CLASS_INPUT_DEVICE_DEFAULTS);
        luacs_enum_declare_value(state, "DEVICE", TAG_CLASS_DEVICE);
        luacs_enum_declare_value(state, "DETAIL_OBJECT_COLLECTION", TAG_CLASS_DETAIL_OBJECT_COLLECTION);
        luacs_enum_declare_value(state, "EFFECT", TAG_CLASS_EFFECT);
        luacs_enum_declare_value(state, "EQUIPMENT", TAG_CLASS_EQUIPMENT);
        luacs_enum_declare_value(state, "FLAG", TAG_CLASS_FLAG);
        luacs_enum_declare_value(state, "FOG", TAG_CLASS_FOG);
        luacs_enum_declare_value(state, "FONT", TAG_CLASS_FONT);
        luacs_enum_declare_value(state, "MATERIAL_EFFECTS", TAG_CLASS_MATERIAL_EFFECTS);
        luacs_enum_declare_value(state, "GARBAGE", TAG_CLASS_GARBAGE);
        luacs_enum_declare_value(state, "GLOW", TAG_CLASS_GLOW);
        luacs_enum_declare_value(state, "GRENADE_HUD_INTERFACE", TAG_CLASS_GRENADE_HUD_INTERFACE);
        luacs_enum_declare_value(state, "HUD_MESSAGE_TEXT", TAG_CLASS_HUD_MESSAGE_TEXT);
        luacs_enum_declare_value(state, "HUD_NUMBER", TAG_CLASS_HUD_NUMBER);
        luacs_enum_declare_value(state, "HUD_GLOBALS", TAG_CLASS_HUD_GLOBALS);
        luacs_enum_declare_value(state, "ITEM", TAG_CLASS_ITEM);
        luacs_enum_declare_value(state, "ITEM_COLLECTION", TAG_CLASS_ITEM_COLLECTION);
        luacs_enum_declare_value(state, "DAMAGE_EFFECT", TAG_CLASS_DAMAGE_EFFECT);
        luacs_enum_declare_value(state, "LENS_FLARE", TAG_CLASS_LENS_FLARE);
        luacs_enum_declare_value(state, "LIGHTNING", TAG_CLASS_LIGHTNING);
        luacs_enum_declare_value(state, "DEVICE_LIGHT_FIXTURE", TAG_CLASS_DEVICE_LIGHT_FIXTURE);
        luacs_enum_declare_value(state, "LIGHT", TAG_CLASS_LIGHT);
        luacs_enum_declare_value(state, "SOUND_LOOPING", TAG_CLASS_SOUND_LOOPING);
        luacs_enum_declare_value(state, "DEVICE_MACHINE", TAG_CLASS_DEVICE_MACHINE);
        luacs_enum_declare_value(state, "GLOBALS", TAG_CLASS_GLOBALS);
        luacs_enum_declare_value(state, "METER", TAG_CLASS_METER);
        luacs_enum_declare_value(state, "LIGHT_VOLUME", TAG_CLASS_LIGHT_VOLUME);
        luacs_enum_declare_value(state, "GBXMODEL", TAG_CLASS_GBXMODEL);
        luacs_enum_declare_value(state, "MODEL", TAG_CLASS_MODEL);
        luacs_enum_declare_value(state, "MULTIPLAYER_SCENARIO_DESCRIPTION", TAG_CLASS_MULTIPLAYER_SCENARIO_DESCRIPTION);
        luacs_enum_declare_value(state, "NULL", TAG_CLASS_NULL);
        luacs_enum_declare_value(state, "PREFERENCES_NETWORK_GAME", TAG_CLASS_PREFERENCES_NETWORK_GAME);
        luacs_enum_declare_value(state, "OBJECT", TAG_CLASS_OBJECT);
        luacs_enum_declare_value(state, "PARTICLE", TAG_CLASS_PARTICLE);
        luacs_enum_declare_value(state, "PARTICLE_SYSTEM", TAG_CLASS_PARTICLE_SYSTEM);
        luacs_enum_declare_value(state, "PHYSICS", TAG_CLASS_PHYSICS);
        luacs_enum_declare_value(state, "PLACEHOLDER", TAG_CLASS_PLACEHOLDER);
        luacs_enum_declare_value(state, "POINT_PHYSICS", TAG_CLASS_POINT_PHYSICS);
        luacs_enum_declare_value(state, "PROJECTILE", TAG_CLASS_PROJECTILE);
        luacs_enum_declare_value(state, "WEATHER_PARTICLE_SYSTEM", TAG_CLASS_WEATHER_PARTICLE_SYSTEM);
        luacs_enum_declare_value(state, "SCENARIO_STRUCTURE_BSP", TAG_CLASS_SCENARIO_STRUCTURE_BSP);
        luacs_enum_declare_value(state, "SCENERY", TAG_CLASS_SCENERY);
        luacs_enum_declare_value(state, "SHADER_TRANSPARENT_CHICAGO_EXTENDED", TAG_CLASS_SHADER_TRANSPARENT_CHICAGO_EXTENDED);
        luacs_enum_declare_value(state, "SHADER_TRANSPARENT_CHICAGO", TAG_CLASS_SHADER_TRANSPARENT_CHICAGO);
        luacs_enum_declare_value(state, "SCENARIO", TAG_CLASS_SCENARIO);
        luacs_enum_declare_value(state, "SHADER_ENVIRONMENT", TAG_CLASS_SHADER_ENVIRONMENT);
        luacs_enum_declare_value(state, "SHADER_TRANSPARENT_GLASS", TAG_CLASS_SHADER_TRANSPARENT_GLASS);
        luacs_enum_declare_value(state, "SHADER", TAG_CLASS_SHADER);
        luacs_enum_declare_value(state, "SKY", TAG_CLASS_SKY);
        luacs_enum_declare_value(state, "SHADER_TRANSPARENT_METER", TAG_CLASS_SHADER_TRANSPARENT_METER);
        luacs_enum_declare_value(state, "SOUND", TAG_CLASS_SOUND);
        luacs_enum_declare_value(state, "SOUND_ENVIRONMENT", TAG_CLASS_SOUND_ENVIRONMENT);
        luacs_enum_declare_value(state, "SHADER_MODEL", TAG_CLASS_SHADER_MODEL);
        luacs_enum_declare_value(state, "SHADER_TRANSPARENT_GENERIC", TAG_CLASS_SHADER_TRANSPARENT_GENERIC);
        luacs_enum_declare_value(state, "UI_WIDGET_COLLECTION", TAG_CLASS_UI_WIDGET_COLLECTION);
        luacs_enum_declare_value(state, "SHADER_TRANSPARENT_PLASMA", TAG_CLASS_SHADER_TRANSPARENT_PLASMA);
        luacs_enum_declare_value(state, "SOUND_SCENERY", TAG_CLASS_SOUND_SCENERY);
        luacs_enum_declare_value(state, "STRING_LIST", TAG_CLASS_STRING_LIST);
        luacs_enum_declare_value(state, "SHADER_TRANSPARENT_WATER", TAG_CLASS_SHADER_TRANSPARENT_WATER);
        luacs_enum_declare_value(state, "TAG_COLLECTION", TAG_CLASS_TAG_COLLECTION);
        luacs_enum_declare_value(state, "CAMERA_TRACK", TAG_CLASS_CAMERA_TRACK);
        luacs_enum_declare_value(state, "DIALOGUE", TAG_CLASS_DIALOGUE);
        luacs_enum_declare_value(state, "UNIT_HUD_INTERFACE", TAG_CLASS_UNIT_HUD_INTERFACE);
        luacs_enum_declare_value(state, "UNIT", TAG_CLASS_UNIT);
        luacs_enum_declare_value(state, "UNICODE_STRING_LIST", TAG_CLASS_UNICODE_STRING_LIST);
        luacs_enum_declare_value(state, "VIRTUAL_KEYBOARD", TAG_CLASS_VIRTUAL_KEYBOARD);
        luacs_enum_declare_value(state, "VEHICLE", TAG_CLASS_VEHICLE);
        luacs_enum_declare_value(state, "WEAPON", TAG_CLASS_WEAPON);
        luacs_enum_declare_value(state, "WIND", TAG_CLASS_WIND);
        luacs_enum_declare_value(state, "WEAPON_HUD_INTERFACE", TAG_CLASS_WEAPON_HUD_INTERFACE);
        lua_pop(state, 1);
    }

    void lua_define_engine_tag_base_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineTag);
        luacs_enum_field(state, EngineTag, EngineTagClass, primary_class, 0);
        luacs_enum_field(state, EngineTag, EngineTagClass, secondary_class, 0);
        luacs_enum_field(state, EngineTag, EngineTagClass, tertiary_class, 0);
        luacs_objref_field(state, EngineTag, EngineResourceHandle, handle, LUACS_FREADONLY);
        luacs_strptr_field(state, EngineTag, path, LUACS_FREADONLY);
        luacs_pseudo_field(state, EngineTag, dataAddress, 0);
        lua_pop(state, 1);
    }

    void lua_define_engine_tag_dependency_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineTagDependency);
        luacs_enum_field(state, EngineTagDependency, EngineTagClass, tag_class, 0);
        luacs_strptr_field(state, EngineTagDependency, path, LUACS_FREADONLY);
        luacs_unsigned_field(state, EngineTagDependency, path_size, LUACS_FREADONLY);
        luacs_nested_field(state, EngineTagDependency, EngineResourceHandle, tag_handle, 0);
        lua_pop(state, 1);
    }

    void lua_push_engine_tag_dependency(lua_State *state, Engine::TagDependency *tag_dependency) noexcept {
        luacs_newobject(state, EngineTagDependency, tag_dependency);
    }

    void lua_define_engine_tag_data_offset_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineTagDataOffset);
        luacs_unsigned_field(state, EngineTagDataOffset, size, LUACS_FREADONLY);
        luacs_unsigned_field(state, EngineTagDataOffset, external, LUACS_FREADONLY);
        luacs_unsigned_field(state, EngineTagDataOffset, file_offset, LUACS_FREADONLY);
        luacs_extref_field(state, EngineTagDataOffset, pointer, LUACS_FREADONLY);
        lua_pop(state, 1);
    }

    void lua_define_engine_tag_string_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineTagString);
        luacs_string_field(state, EngineTagString, string, 0);
        lua_pop(state, 1);
    }

    void lua_push_engine_tag_string(lua_State *state, Engine::TagString *tag_string) noexcept {
        luacs_newobject(state, EngineTagString, tag_string);
    }
}
