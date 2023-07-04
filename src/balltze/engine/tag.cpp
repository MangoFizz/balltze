// SPDX-License-Identifier: GPL-3.0-only

#include <optional>
#include <balltze/engine/core.hpp>
#include <balltze/engine/tag.hpp>

namespace Balltze::Engine {
    extern "C" TagHandle get_tag_id(const char *path, std::uint32_t tag_class) noexcept;

    std::byte *get_tag_data_address() noexcept {
        static std::optional<std::byte *> address;
        if(!address.has_value()) {
            switch(get_engine_edition()) {
                case ENGINE_TYPE_DEMO:
                    address = reinterpret_cast<std::byte *>(0x4BF10000);
                default:
                    address = reinterpret_cast<std::byte *>(0x40440000);
            }
        }
        return address.value();
    }

    Tag *get_tag(TagHandle tag_id) noexcept {
        if(tag_id.is_null()) {
            return nullptr;
        }

        auto &tag_data_header = get_tag_data_header();
        auto *tag = tag_data_header.tag_array + tag_id.index.index;
        auto tag_count = tag_data_header.tag_count;

        if(tag_id.index.index <= tag_count) {
            return tag;
        }

        return nullptr;
    }

    Tag *get_tag(std::size_t tag_index) noexcept {
        if(tag_index == 0xFFFFFFFF) {
            return nullptr;
        }

        auto &tag_data_header = get_tag_data_header();
        auto *tag = tag_data_header.tag_array + tag_index;
        auto tag_count = tag_data_header.tag_count;

        if(tag_index <= tag_count) {
            return tag;
        }

        return nullptr;
    }

    extern "C" Tag *get_tag_address_from_id(TagHandle tag_id) noexcept {
        return get_tag(tag_id);
    }

    Tag *get_tag(std::string path, std::uint32_t tag_class) noexcept {
        auto tag_id = get_tag_id(path.c_str(), tag_class);
        return get_tag(tag_id);
    }

    TagClassInt tag_class_from_string(std::string tag_class_name) noexcept {
        if(tag_class_name == "actor") 
            return TAG_CLASS_ACTOR;
        else if(tag_class_name == "actor_variant") 
            return TAG_CLASS_ACTOR_VARIANT;
        else if(tag_class_name == "antenna")
            return TAG_CLASS_ANTENNA;
        else if(tag_class_name == "model_animations")
            return TAG_CLASS_MODEL_ANIMATIONS;
        else if(tag_class_name == "biped")
            return TAG_CLASS_BIPED;
        else if(tag_class_name == "bitmap")
            return TAG_CLASS_BITMAP;
        else if(tag_class_name == "spheroid")
            return TAG_CLASS_SPHEROID;
        else if(tag_class_name == "continuous_damage_effect")
            return TAG_CLASS_CONTINUOUS_DAMAGE_EFFECT;
        else if(tag_class_name == "model_collision_geometry")
            return TAG_CLASS_MODEL_COLLISION_GEOMETRY;
        else if(tag_class_name == "color_table")
            return TAG_CLASS_COLOR_TABLE;
        else if(tag_class_name == "contrail")
            return TAG_CLASS_CONTRAIL;
        else if(tag_class_name == "device_control")
            return TAG_CLASS_DEVICE_CONTROL;
        else if(tag_class_name == "decal")
            return TAG_CLASS_DECAL;
        else if(tag_class_name == "ui_widget_definition")
            return TAG_CLASS_UI_WIDGET_DEFINITION;
        else if(tag_class_name == "input_device_defaults")
            return TAG_CLASS_INPUT_DEVICE_DEFAULTS;
        else if(tag_class_name == "device")
            return TAG_CLASS_DEVICE;
        else if(tag_class_name == "detail_object_collection")
            return TAG_CLASS_DETAIL_OBJECT_COLLECTION;
        else if(tag_class_name == "effect")
            return TAG_CLASS_EFFECT;
        else if(tag_class_name == "equipment")
            return TAG_CLASS_EQUIPMENT;
        else if(tag_class_name == "flag")
            return TAG_CLASS_FLAG;
        else if(tag_class_name == "fog")
            return TAG_CLASS_FOG;
        else if(tag_class_name == "font")
            return TAG_CLASS_FONT;
        else if(tag_class_name == "lightning")
            return TAG_CLASS_LIGHTNING;
        else if(tag_class_name == "material_effects")
            return TAG_CLASS_MATERIAL_EFFECTS;
        else if(tag_class_name == "garbage")
            return TAG_CLASS_GARBAGE;
        else if(tag_class_name == "glow")
            return TAG_CLASS_GLOW;
        else if(tag_class_name == "grenade_hud_interface")
            return TAG_CLASS_GRENADE_HUD_INTERFACE;
        else if(tag_class_name == "hud_message_text")
            return TAG_CLASS_HUD_MESSAGE_TEXT;
        else if(tag_class_name == "hud_number")
            return TAG_CLASS_HUD_NUMBER;
        else if(tag_class_name == "hud_globals")
            return TAG_CLASS_HUD_GLOBALS;
        else if(tag_class_name == "item")
            return TAG_CLASS_ITEM;
        else if(tag_class_name == "item_collection")
            return TAG_CLASS_ITEM_COLLECTION;
        else if(tag_class_name == "damage_effect")
            return TAG_CLASS_DAMAGE_EFFECT;
        else if(tag_class_name == "lens_flare")
            return TAG_CLASS_LENS_FLARE;
        else if(tag_class_name == "device_light_fixture")
            return TAG_CLASS_DEVICE_LIGHT_FIXTURE;
        else if(tag_class_name == "light")
            return TAG_CLASS_LIGHT;
        else if(tag_class_name == "sound_looping")
            return TAG_CLASS_SOUND_LOOPING;
        else if(tag_class_name == "device_machine")
            return TAG_CLASS_DEVICE_MACHINE;
        else if(tag_class_name == "globals")
            return TAG_CLASS_GLOBALS;
        else if(tag_class_name == "meter")
            return TAG_CLASS_METER;
        else if(tag_class_name == "light_volume")
            return TAG_CLASS_LIGHT_VOLUME;
        else if(tag_class_name == "gbxmodel")
            return TAG_CLASS_GBXMODEL;
        else if(tag_class_name == "model")
            return TAG_CLASS_MODEL;
        else if(tag_class_name == "multiplayer_scenario_description")
            return TAG_CLASS_MULTIPLAYER_SCENARIO_DESCRIPTION;
        else if(tag_class_name == "preferences_network_game")
            return TAG_CLASS_PREFERENCES_NETWORK_GAME;
        else if(tag_class_name == "object")
            return TAG_CLASS_OBJECT;
        else if(tag_class_name == "particle")
            return TAG_CLASS_PARTICLE;
        else if(tag_class_name == "particle_system")
            return TAG_CLASS_PARTICLE_SYSTEM;
        else if(tag_class_name == "physics")
            return TAG_CLASS_PHYSICS;
        else if(tag_class_name == "placeholder")
            return TAG_CLASS_PLACEHOLDER;
        else if(tag_class_name == "point_physics")
            return TAG_CLASS_POINT_PHYSICS;
        else if(tag_class_name == "projectile")
            return TAG_CLASS_PROJECTILE;
        else if(tag_class_name == "weather")
            return TAG_CLASS_WEATHER_PARTICLE_SYSTEM;
        else if(tag_class_name == "scenario_structure_bsp")
            return TAG_CLASS_SCENARIO_STRUCTURE_BSP;
        else if(tag_class_name == "scenery")
            return TAG_CLASS_SCENERY;
        else if(tag_class_name == "shader_transparent_chicago_extended")
            return TAG_CLASS_SHADER_TRANSPARENT_CHICAGO_EXTENDED;
        else if(tag_class_name == "shader_transparent_chicago")
            return TAG_CLASS_SHADER_TRANSPARENT_CHICAGO;
        else if(tag_class_name == "scenario")
            return TAG_CLASS_SCENARIO;
        else if(tag_class_name == "shader_environment")
            return TAG_CLASS_SHADER_ENVIRONMENT;
        else if(tag_class_name == "transparent_glass")
            return TAG_CLASS_SHADER_TRANSPARENT_GLASS;
        else if(tag_class_name == "shader")
            return TAG_CLASS_SHADER;
        else if(tag_class_name == "sky")
            return TAG_CLASS_SKY;
        else if(tag_class_name == "shader_transparent_meter")
            return TAG_CLASS_SHADER_TRANSPARENT_METER;
        else if(tag_class_name == "sound")
            return TAG_CLASS_SOUND;
        else if(tag_class_name == "sound_environment")
            return TAG_CLASS_SOUND_ENVIRONMENT;
        else if(tag_class_name == "shader_model")
            return TAG_CLASS_SHADER_MODEL;
        else if(tag_class_name == "shader_transparent_generic")
            return TAG_CLASS_SHADER_TRANSPARENT_GENERIC;
        else if(tag_class_name == "ui_widget_collection")
            return TAG_CLASS_UI_WIDGET_COLLECTION;
        else if(tag_class_name == "shader_transparent_plasma")
            return TAG_CLASS_SHADER_TRANSPARENT_PLASMA;
        else if(tag_class_name == "sound_scenery")
            return TAG_CLASS_SOUND_SCENERY;
        else if(tag_class_name == "string_list")
            return TAG_CLASS_STRING_LIST;
        else if(tag_class_name == "shader_transparent_water")
            return TAG_CLASS_SHADER_TRANSPARENT_WATER;
        else if(tag_class_name == "tag_collection")
            return TAG_CLASS_TAG_COLLECTION;
        else if(tag_class_name == "camera_track")
            return TAG_CLASS_CAMERA_TRACK;
        else if(tag_class_name == "unit_dialogue")
            return TAG_CLASS_DIALOGUE;
        else if(tag_class_name == "unit_hud_interface")
            return TAG_CLASS_UNIT_HUD_INTERFACE;
        else if(tag_class_name == "unit")
            return TAG_CLASS_UNIT;
        else if(tag_class_name == "unicode_string_list")
            return TAG_CLASS_UNICODE_STRING_LIST;
        else if(tag_class_name == "virtual_keyboard")
            return TAG_CLASS_VIRTUAL_KEYBOARD;
        else if(tag_class_name == "vehicle")
            return TAG_CLASS_VEHICLE;
        else if(tag_class_name == "weapon")
            return TAG_CLASS_WEAPON;
        else if(tag_class_name == "wind")
            return TAG_CLASS_WIND;
        else if(tag_class_name == "weapon_hud_interface")
            return TAG_CLASS_WEAPON_HUD_INTERFACE;
        else
            return TAG_CLASS_NULL;
    }

    std::string tag_class_to_string(TagClassInt tag_class) {
        switch(tag_class) {
            case TAG_CLASS_ACTOR: 
                return "actor";
            case TAG_CLASS_ACTOR_VARIANT: 
                return "actor_variant";
            case TAG_CLASS_ANTENNA: 
                return "antenna";
            case TAG_CLASS_MODEL_ANIMATIONS: 
                return "model_animations";
            case TAG_CLASS_BIPED: 
                return "biped";
            case TAG_CLASS_BITMAP: 
                return "bitmap";
            case TAG_CLASS_SPHEROID: 
                return "spheroid";
            case TAG_CLASS_CONTINUOUS_DAMAGE_EFFECT: 
                return "continuous_damage_effect";
            case TAG_CLASS_MODEL_COLLISION_GEOMETRY: 
                return "model_collision_geometry";
            case TAG_CLASS_COLOR_TABLE: 
                return "color_table";
            case TAG_CLASS_CONTRAIL: 
                return "contrail";
            case TAG_CLASS_DEVICE_CONTROL: 
                return "device_control";
            case TAG_CLASS_DECAL: 
                return "decal";
            case TAG_CLASS_UI_WIDGET_DEFINITION: 
                return "ui_widget_definition";
            case TAG_CLASS_INPUT_DEVICE_DEFAULTS: 
                return "input_device_defaults";
            case TAG_CLASS_DEVICE: 
                return "device";
            case TAG_CLASS_DETAIL_OBJECT_COLLECTION: 
                return "detail_object_collection";
            case TAG_CLASS_EFFECT: 
                return "effect";
            case TAG_CLASS_EQUIPMENT: 
                return "equipment";
            case TAG_CLASS_FLAG: 
                return "flag";
            case TAG_CLASS_FOG: 
                return "fog";
            case TAG_CLASS_FONT: 
                return "font";
            case TAG_CLASS_LIGHTNING: 
                return "lightning";
            case TAG_CLASS_MATERIAL_EFFECTS: 
                return "material_effects";
            case TAG_CLASS_GARBAGE: 
                return "garbage";
            case TAG_CLASS_GLOW: 
                return "glow";
            case TAG_CLASS_GRENADE_HUD_INTERFACE: 
                return "grenade_hud_interface";
            case TAG_CLASS_HUD_MESSAGE_TEXT: 
                return "hud_message_text";
            case TAG_CLASS_HUD_NUMBER: 
                return "hud_number";
            case TAG_CLASS_HUD_GLOBALS: 
                return "hud_globals";
            case TAG_CLASS_ITEM: 
                return "item";
            case TAG_CLASS_ITEM_COLLECTION: 
                return "item_collection";
            case TAG_CLASS_DAMAGE_EFFECT: 
                return "damage_effect";
            case TAG_CLASS_LENS_FLARE: 
                return "lens_flare";
            case TAG_CLASS_DEVICE_LIGHT_FIXTURE: 
                return "device_light_fixture";
            case TAG_CLASS_LIGHT: 
                return "light";
            case TAG_CLASS_SOUND_LOOPING: 
                return "sound_looping";
            case TAG_CLASS_DEVICE_MACHINE: 
                return "device_machine";
            case TAG_CLASS_GLOBALS: 
                return "globals";
            case TAG_CLASS_METER: 
                return "meter";
            case TAG_CLASS_LIGHT_VOLUME: 
                return "light_volume";
            case TAG_CLASS_GBXMODEL: 
                return "gbxmodel";
            case TAG_CLASS_MODEL: 
                return "model";
            case TAG_CLASS_MULTIPLAYER_SCENARIO_DESCRIPTION: 
                return "multiplayer_scenario_description";
            case TAG_CLASS_PREFERENCES_NETWORK_GAME: 
                return "preferences_network_game";
            case TAG_CLASS_OBJECT: 
                return "object";
            case TAG_CLASS_PARTICLE: 
                return "particle";
            case TAG_CLASS_PARTICLE_SYSTEM: 
                return "particle_system";
            case TAG_CLASS_PHYSICS: 
                return "physics";
            case TAG_CLASS_PLACEHOLDER: 
                return "placeholder";
            case TAG_CLASS_POINT_PHYSICS: 
                return "point_physics";
            case TAG_CLASS_PROJECTILE: 
                return "projectile";
            case TAG_CLASS_WEATHER_PARTICLE_SYSTEM: 
                return "weather";
            case TAG_CLASS_SCENARIO_STRUCTURE_BSP: 
                return "scenario_structure_bsp";
            case TAG_CLASS_SCENERY: 
                return "scenery";
            case TAG_CLASS_SHADER_TRANSPARENT_CHICAGO_EXTENDED: 
                return "shader_transparent_chicago_extended";
            case TAG_CLASS_SHADER_TRANSPARENT_CHICAGO: 
                return "shader_transparent_chicago";
            case TAG_CLASS_SCENARIO: 
                return "scenario";
            case TAG_CLASS_SHADER_ENVIRONMENT: 
                return "shader_environment";
            case TAG_CLASS_SHADER_TRANSPARENT_GLASS: 
                return "transparent_glass";
            case TAG_CLASS_SHADER: 
                return "shader";
            case TAG_CLASS_SKY: 
                return "sky";
            case TAG_CLASS_SHADER_TRANSPARENT_METER: 
                return "shader_transparent_meter";
            case TAG_CLASS_SOUND: 
                return "sound";
            case TAG_CLASS_SOUND_ENVIRONMENT: 
                return "sound_environment";
            case TAG_CLASS_SHADER_MODEL: 
                return "shader_model";
            case TAG_CLASS_SHADER_TRANSPARENT_GENERIC: 
                return "shader_transparent_generic";
            case TAG_CLASS_UI_WIDGET_COLLECTION: 
                return "ui_widget_collection";
            case TAG_CLASS_SHADER_TRANSPARENT_PLASMA: 
                return "shader_transparent_plasma";
            case TAG_CLASS_SOUND_SCENERY: 
                return "sound_scenery";
            case TAG_CLASS_STRING_LIST: 
                return "string_list";
            case TAG_CLASS_SHADER_TRANSPARENT_WATER: 
                return "shader_transparent_water";
            case TAG_CLASS_TAG_COLLECTION: 
                return "tag_collection";
            case TAG_CLASS_CAMERA_TRACK: 
                return "camera_track";
            case TAG_CLASS_DIALOGUE: 
                return "unit_dialogue";
            case TAG_CLASS_UNIT_HUD_INTERFACE: 
                return "unit_hud_interface";
            case TAG_CLASS_UNIT: 
                return "unit";
            case TAG_CLASS_UNICODE_STRING_LIST: 
                return "unicode_string_list";
            case TAG_CLASS_VIRTUAL_KEYBOARD: 
                return "virtual_keyboard";
            case TAG_CLASS_VEHICLE: 
                return "vehicle";
            case TAG_CLASS_WEAPON: 
                return "weapon";
            case TAG_CLASS_WIND: 
                return "wind";
            case TAG_CLASS_WEAPON_HUD_INTERFACE: 
                return "weapon_hud_interface";
            default:
                return "none";
        }
    }
}
