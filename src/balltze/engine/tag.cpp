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
}
