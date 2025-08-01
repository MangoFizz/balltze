// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__ENGINE__TAG_CLASSES_HPP
#define BALLTZE__ENGINE__TAG_CLASSES_HPP

#include <cstdint>

namespace Balltze::LegacyApi::Engine {
    enum TagClassInt : std::uint32_t {
        TAG_CLASS_ACTOR = 0x61637472,
        TAG_CLASS_ACTOR_VARIANT = 0x61637476,
        TAG_CLASS_ANTENNA = 0x616E7421,
        TAG_CLASS_MODEL_ANIMATIONS = 0x616E7472,
        TAG_CLASS_BIPED = 0x62697064,
        TAG_CLASS_BITMAP = 0x6269746D,
        TAG_CLASS_SPHEROID = 0x626F6F6D,
        TAG_CLASS_CONTINUOUS_DAMAGE_EFFECT = 0x63646D67,
        TAG_CLASS_MODEL_COLLISION_GEOMETRY = 0x636F6C6C,
        TAG_CLASS_COLOR_TABLE = 0x636F6C6F,
        TAG_CLASS_CONTRAIL = 0x636F6E74,
        TAG_CLASS_DEVICE_CONTROL = 0x6374726C,
        TAG_CLASS_DECAL = 0x64656361,
        TAG_CLASS_UI_WIDGET_DEFINITION = 0x44654C61,
        TAG_CLASS_INPUT_DEVICE_DEFAULTS = 0x64657663,
        TAG_CLASS_DEVICE = 0x64657669,
        TAG_CLASS_DETAIL_OBJECT_COLLECTION = 0x646F6263,
        TAG_CLASS_EFFECT = 0x65666665,
        TAG_CLASS_EQUIPMENT = 0x65716970,
        TAG_CLASS_FLAG = 0x666C6167,
        TAG_CLASS_FOG = 0x666F6720,
        TAG_CLASS_FONT = 0x666F6E74,
        TAG_CLASS_MATERIAL_EFFECTS = 0x666F6F74,
        TAG_CLASS_GARBAGE = 0x67617262,
        TAG_CLASS_GLOW = 0x676C7721,
        TAG_CLASS_GRENADE_HUD_INTERFACE = 0x67726869,
        TAG_CLASS_HUD_MESSAGE_TEXT = 0x686D7420,
        TAG_CLASS_HUD_NUMBER = 0x68756423,
        TAG_CLASS_HUD_GLOBALS = 0x68756467,
        TAG_CLASS_ITEM = 0x6974656D,
        TAG_CLASS_ITEM_COLLECTION = 0x69746D63,
        TAG_CLASS_DAMAGE_EFFECT = 0x6A707421,
        TAG_CLASS_LENS_FLARE = 0x6C656E73,
        TAG_CLASS_LIGHTNING = 0x656C6563,
        TAG_CLASS_DEVICE_LIGHT_FIXTURE = 0x6C696669,
        TAG_CLASS_LIGHT = 0x6C696768,
        TAG_CLASS_SOUND_LOOPING = 0x6C736E64,
        TAG_CLASS_DEVICE_MACHINE = 0x6D616368,
        TAG_CLASS_GLOBALS = 0x6D617467,
        TAG_CLASS_METER = 0x6D657472,
        TAG_CLASS_LIGHT_VOLUME = 0x6D677332,
        TAG_CLASS_GBXMODEL = 0x6D6F6432,
        TAG_CLASS_MODEL = 0x6D6F6465,
        TAG_CLASS_MULTIPLAYER_SCENARIO_DESCRIPTION = 0x6D706C79,
        TAG_CLASS_NULL = 0xFFFFFFFF,
        TAG_CLASS_PREFERENCES_NETWORK_GAME = 0x6E677072,
        TAG_CLASS_OBJECT = 0x6F626A65,
        TAG_CLASS_PARTICLE = 0x70617274,
        TAG_CLASS_PARTICLE_SYSTEM = 0x7063746C,
        TAG_CLASS_PHYSICS = 0x70687973,
        TAG_CLASS_PLACEHOLDER = 0x706C6163,
        TAG_CLASS_POINT_PHYSICS = 0x70706879,
        TAG_CLASS_PROJECTILE = 0x70726F6A,
        TAG_CLASS_WEATHER_PARTICLE_SYSTEM = 0x7261696E,
        TAG_CLASS_SCENARIO_STRUCTURE_BSP = 0x73627370,
        TAG_CLASS_SCENERY = 0x7363656E,
        TAG_CLASS_SHADER_TRANSPARENT_CHICAGO_EXTENDED = 0x73636578,
        TAG_CLASS_SHADER_TRANSPARENT_CHICAGO = 0x73636869,
        TAG_CLASS_SCENARIO = 0x73636E72,
        TAG_CLASS_SHADER_ENVIRONMENT = 0x73656E76,
        TAG_CLASS_SHADER_TRANSPARENT_GLASS = 0x73676C61,
        TAG_CLASS_SHADER = 0x73686472,
        TAG_CLASS_SKY = 0x736B7920,
        TAG_CLASS_SHADER_TRANSPARENT_METER = 0x736D6574,
        TAG_CLASS_SOUND = 0x736E6421,
        TAG_CLASS_SOUND_ENVIRONMENT = 0x736E6465,
        TAG_CLASS_SHADER_MODEL = 0x736F736F,
        TAG_CLASS_SHADER_TRANSPARENT_GENERIC = 0x736F7472,
        TAG_CLASS_UI_WIDGET_COLLECTION = 0x536F756C,
        TAG_CLASS_SHADER_TRANSPARENT_PLASMA = 0x73706C61,
        TAG_CLASS_SOUND_SCENERY = 0x73736365,
        TAG_CLASS_STRING_LIST = 0x73747223,
        TAG_CLASS_SHADER_TRANSPARENT_WATER = 0x73776174,
        TAG_CLASS_TAG_COLLECTION = 0x74616763,
        TAG_CLASS_CAMERA_TRACK = 0x7472616B,
        TAG_CLASS_DIALOGUE = 0x75646C67,
        TAG_CLASS_UNIT_HUD_INTERFACE = 0x756E6869,
        TAG_CLASS_UNIT = 0x756E6974,
        TAG_CLASS_UNICODE_STRING_LIST = 0x75737472,
        TAG_CLASS_VIRTUAL_KEYBOARD = 0x76636B79,
        TAG_CLASS_VEHICLE = 0x76656869,
        TAG_CLASS_WEAPON = 0x77656170,
        TAG_CLASS_WIND = 0x77696E64,
        TAG_CLASS_WEAPON_HUD_INTERFACE = 0x77706869
    };
}

#endif
