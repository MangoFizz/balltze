// SPDX-License-Identifier: GPL-3.0-only

#include "../helpers/luacstruct.hpp"
#include "../helpers/enum.hpp"
#include "engine_tag.hpp"

namespace Balltze::Plugins::Lua {
    using namespace Engine;

    static void define_engine_tag_class_enum(lua_State *state) noexcept {
        luacs_newenum(state, EngineTagClass);
        luacs_enum_declare_value(state, "actor", TAG_CLASS_ACTOR);
        luacs_enum_declare_value(state, "actorVariant", TAG_CLASS_ACTOR_VARIANT);
        luacs_enum_declare_value(state, "antenna", TAG_CLASS_ANTENNA);
        luacs_enum_declare_value(state, "modelAnimations", TAG_CLASS_MODEL_ANIMATIONS);
        luacs_enum_declare_value(state, "biped", TAG_CLASS_BIPED);
        luacs_enum_declare_value(state, "bitmap", TAG_CLASS_BITMAP);
        luacs_enum_declare_value(state, "spheroid", TAG_CLASS_SPHEROID);
        luacs_enum_declare_value(state, "continuousDamageEffect", TAG_CLASS_CONTINUOUS_DAMAGE_EFFECT);
        luacs_enum_declare_value(state, "modelCollisionGeometry", TAG_CLASS_MODEL_COLLISION_GEOMETRY);
        luacs_enum_declare_value(state, "colorTable", TAG_CLASS_COLOR_TABLE);
        luacs_enum_declare_value(state, "contrail", TAG_CLASS_CONTRAIL);
        luacs_enum_declare_value(state, "deviceControl", TAG_CLASS_DEVICE_CONTROL);
        luacs_enum_declare_value(state, "decal", TAG_CLASS_DECAL);
        luacs_enum_declare_value(state, "uiWidgetDefinition", TAG_CLASS_UI_WIDGET_DEFINITION);
        luacs_enum_declare_value(state, "inputDeviceDefaults", TAG_CLASS_INPUT_DEVICE_DEFAULTS);
        luacs_enum_declare_value(state, "device", TAG_CLASS_DEVICE);
        luacs_enum_declare_value(state, "detailObjectCollection", TAG_CLASS_DETAIL_OBJECT_COLLECTION);
        luacs_enum_declare_value(state, "effect", TAG_CLASS_EFFECT);
        luacs_enum_declare_value(state, "equipment", TAG_CLASS_EQUIPMENT);
        luacs_enum_declare_value(state, "flag", TAG_CLASS_FLAG);
        luacs_enum_declare_value(state, "fog", TAG_CLASS_FOG);
        luacs_enum_declare_value(state, "font", TAG_CLASS_FONT);
        luacs_enum_declare_value(state, "materialEffects", TAG_CLASS_MATERIAL_EFFECTS);
        luacs_enum_declare_value(state, "garbage", TAG_CLASS_GARBAGE);
        luacs_enum_declare_value(state, "glow", TAG_CLASS_GLOW);
        luacs_enum_declare_value(state, "grenadeHudInterface", TAG_CLASS_GRENADE_HUD_INTERFACE);
        luacs_enum_declare_value(state, "hudMessageText", TAG_CLASS_HUD_MESSAGE_TEXT);
        luacs_enum_declare_value(state, "hudNumber", TAG_CLASS_HUD_NUMBER);
        luacs_enum_declare_value(state, "hudGlobals", TAG_CLASS_HUD_GLOBALS);
        luacs_enum_declare_value(state, "item", TAG_CLASS_ITEM);
        luacs_enum_declare_value(state, "itemCollection", TAG_CLASS_ITEM_COLLECTION);
        luacs_enum_declare_value(state, "damageEffect", TAG_CLASS_DAMAGE_EFFECT);
        luacs_enum_declare_value(state, "lensFlare", TAG_CLASS_LENS_FLARE);
        luacs_enum_declare_value(state, "lightning", TAG_CLASS_LIGHTNING);
        luacs_enum_declare_value(state, "deviceLightFixture", TAG_CLASS_DEVICE_LIGHT_FIXTURE);
        luacs_enum_declare_value(state, "light", TAG_CLASS_LIGHT);
        luacs_enum_declare_value(state, "soundLooping", TAG_CLASS_SOUND_LOOPING);
        luacs_enum_declare_value(state, "deviceMachine", TAG_CLASS_DEVICE_MACHINE);
        luacs_enum_declare_value(state, "globals", TAG_CLASS_GLOBALS);
        luacs_enum_declare_value(state, "meter", TAG_CLASS_METER);
        luacs_enum_declare_value(state, "lightVolume", TAG_CLASS_LIGHT_VOLUME);
        luacs_enum_declare_value(state, "gbxmodel", TAG_CLASS_GBXMODEL);
        luacs_enum_declare_value(state, "model", TAG_CLASS_MODEL);
        luacs_enum_declare_value(state, "multiplayerScenarioDescription", TAG_CLASS_MULTIPLAYER_SCENARIO_DESCRIPTION);
        luacs_enum_declare_value(state, "null", TAG_CLASS_NULL);
        luacs_enum_declare_value(state, "preferencesNetworkGame", TAG_CLASS_PREFERENCES_NETWORK_GAME);
        luacs_enum_declare_value(state, "object", TAG_CLASS_OBJECT);
        luacs_enum_declare_value(state, "particle", TAG_CLASS_PARTICLE);
        luacs_enum_declare_value(state, "particleSystem", TAG_CLASS_PARTICLE_SYSTEM);
        luacs_enum_declare_value(state, "physics", TAG_CLASS_PHYSICS);
        luacs_enum_declare_value(state, "placeholder", TAG_CLASS_PLACEHOLDER);
        luacs_enum_declare_value(state, "pointPhysics", TAG_CLASS_POINT_PHYSICS);
        luacs_enum_declare_value(state, "projectile", TAG_CLASS_PROJECTILE);
        luacs_enum_declare_value(state, "weatherParticleSystem", TAG_CLASS_WEATHER_PARTICLE_SYSTEM);
        luacs_enum_declare_value(state, "scenarioStructureBsp", TAG_CLASS_SCENARIO_STRUCTURE_BSP);
        luacs_enum_declare_value(state, "scenery", TAG_CLASS_SCENERY);
        luacs_enum_declare_value(state, "shaderTransparentChicagoExtended", TAG_CLASS_SHADER_TRANSPARENT_CHICAGO_EXTENDED);
        luacs_enum_declare_value(state, "shaderTransparentChicago", TAG_CLASS_SHADER_TRANSPARENT_CHICAGO);
        luacs_enum_declare_value(state, "scenario", TAG_CLASS_SCENARIO);
        luacs_enum_declare_value(state, "shaderEnvironment", TAG_CLASS_SHADER_ENVIRONMENT);
        luacs_enum_declare_value(state, "shaderTransparentGlass", TAG_CLASS_SHADER_TRANSPARENT_GLASS);
        luacs_enum_declare_value(state, "shader", TAG_CLASS_SHADER);
        luacs_enum_declare_value(state, "sky", TAG_CLASS_SKY);
        luacs_enum_declare_value(state, "shaderTransparentMeter", TAG_CLASS_SHADER_TRANSPARENT_METER);
        luacs_enum_declare_value(state, "sound", TAG_CLASS_SOUND);
        luacs_enum_declare_value(state, "soundEnvironment", TAG_CLASS_SOUND_ENVIRONMENT);
        luacs_enum_declare_value(state, "shaderModel", TAG_CLASS_SHADER_MODEL);
        luacs_enum_declare_value(state, "shaderTransparentGeneric", TAG_CLASS_SHADER_TRANSPARENT_GENERIC);
        luacs_enum_declare_value(state, "uiWidgetCollection", TAG_CLASS_UI_WIDGET_COLLECTION);
        luacs_enum_declare_value(state, "shaderTransparentPlasma", TAG_CLASS_SHADER_TRANSPARENT_PLASMA);
        luacs_enum_declare_value(state, "soundScenery", TAG_CLASS_SOUND_SCENERY);
        luacs_enum_declare_value(state, "stringList", TAG_CLASS_STRING_LIST);
        luacs_enum_declare_value(state, "shaderTransparentWater", TAG_CLASS_SHADER_TRANSPARENT_WATER);
        luacs_enum_declare_value(state, "tagCollection", TAG_CLASS_TAG_COLLECTION);
        luacs_enum_declare_value(state, "cameraTrack", TAG_CLASS_CAMERA_TRACK);
        luacs_enum_declare_value(state, "dialogue", TAG_CLASS_DIALOGUE);
        luacs_enum_declare_value(state, "unitHudInterface", TAG_CLASS_UNIT_HUD_INTERFACE);
        luacs_enum_declare_value(state, "unit", TAG_CLASS_UNIT);
        luacs_enum_declare_value(state, "unicodeStringList", TAG_CLASS_UNICODE_STRING_LIST);
        luacs_enum_declare_value(state, "virtualKeyboard", TAG_CLASS_VIRTUAL_KEYBOARD);
        luacs_enum_declare_value(state, "vehicle", TAG_CLASS_VEHICLE);
        luacs_enum_declare_value(state, "weapon", TAG_CLASS_WEAPON);
        luacs_enum_declare_value(state, "wind", TAG_CLASS_WIND);
        luacs_enum_declare_value(state, "weaponHudInterface", TAG_CLASS_WEAPON_HUD_INTERFACE);
        publish_enum(state, "Engine", "tag", "classes", -1);
        lua_pop(state, 1);
    }

    Engine::TagClassInt get_tag_class(lua_State *state, int index) noexcept {
        return luacs_checkenumvalue(state, index, EngineTagClass);
    }

    static void define_engine_tag_base_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineTag);
        luacs_enum_field(state, EngineTag, EngineTagClass, primary_class, 0);
        luacs_enum_field(state, EngineTag, EngineTagClass, secondary_class, 0);
        luacs_enum_field(state, EngineTag, EngineTagClass, tertiary_class, 0);
        luacs_nested_field(state, EngineTag, EngineResourceHandle, handle, LUACS_FREADONLY);
        luacs_strptr_field(state, EngineTag, path, LUACS_FREADONLY);
        lua_pop(state, 1);
    }

    static void define_engine_tag_dependency_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineTagDependency);
        luacs_enum_field(state, EngineTagDependency, EngineTagClass, tag_class, 0);
        luacs_strptr_field(state, EngineTagDependency, path, LUACS_FREADONLY);
        luacs_unsigned_field(state, EngineTagDependency, path_size, LUACS_FREADONLY);
        luacs_nested_field(state, EngineTagDependency, EngineResourceHandle, tag_handle, 0);
        lua_pop(state, 1);
    }

    void push_engine_tag_dependency(lua_State *state, Engine::TagDependency *tag_dependency) noexcept {
        luacs_newobject(state, EngineTagDependency, tag_dependency);
    }

    static void define_engine_tag_data_offset_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineTagDataOffset);
        luacs_unsigned_field(state, EngineTagDataOffset, size, LUACS_FREADONLY);
        luacs_unsigned_field(state, EngineTagDataOffset, external, LUACS_FREADONLY);
        luacs_unsigned_field(state, EngineTagDataOffset, file_offset, LUACS_FREADONLY);
        luacs_unsigned_field(state, EngineTagDataOffset, pointer, LUACS_FREADONLY);
        lua_pop(state, 1);
    }

    void define_engine_tag_types(lua_State *state) noexcept {
        define_engine_tag_class_enum(state);
        define_engine_tag_base_struct(state);
        define_engine_tag_dependency_struct(state);
        define_engine_tag_data_offset_struct(state);
    }
}
