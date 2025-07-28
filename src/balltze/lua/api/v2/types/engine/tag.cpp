// SPDX-License-Identifier: GPL-3.0-only

#include "../../../../libraries/luastruct.hpp"
#include "../../../../helpers/enum.hpp"
#include "tag_data.hpp"
#include "tag.hpp"

namespace Balltze::Lua::Api::V2 {
    static void define_tag_group_type(lua_State *state) noexcept {
        LUAS_ENUM(state, TagGroup);
        LUAS_ENUM_VARIANT(state, TagGroup, "actor", TAG_GROUP_ACTOR);
        LUAS_ENUM_VARIANT(state, TagGroup, "actor_variant", TAG_GROUP_ACTOR_VARIANT);
        LUAS_ENUM_VARIANT(state, TagGroup, "antenna", TAG_GROUP_ANTENNA);
        LUAS_ENUM_VARIANT(state, TagGroup, "model_animations", TAG_GROUP_MODEL_ANIMATIONS);
        LUAS_ENUM_VARIANT(state, TagGroup, "biped", TAG_GROUP_BIPED);
        LUAS_ENUM_VARIANT(state, TagGroup, "bitmap", TAG_GROUP_BITMAP);
        LUAS_ENUM_VARIANT(state, TagGroup, "spheroid", TAG_GROUP_SPHEROID);
        LUAS_ENUM_VARIANT(state, TagGroup, "continuous_damage_effect", TAG_GROUP_CONTINUOUS_DAMAGE_EFFECT);
        LUAS_ENUM_VARIANT(state, TagGroup, "model_collision_geometry", TAG_GROUP_MODEL_COLLISION_GEOMETRY);
        LUAS_ENUM_VARIANT(state, TagGroup, "color_table", TAG_GROUP_COLOR_TABLE);
        LUAS_ENUM_VARIANT(state, TagGroup, "contrail", TAG_GROUP_CONTRAIL);
        LUAS_ENUM_VARIANT(state, TagGroup, "device_control", TAG_GROUP_DEVICE_CONTROL);
        LUAS_ENUM_VARIANT(state, TagGroup, "decal", TAG_GROUP_DECAL);
        LUAS_ENUM_VARIANT(state, TagGroup, "ui_widget_definition", TAG_GROUP_UI_WIDGET_DEFINITION);
        LUAS_ENUM_VARIANT(state, TagGroup, "input_device_defaults", TAG_GROUP_INPUT_DEVICE_DEFAULTS);
        LUAS_ENUM_VARIANT(state, TagGroup, "device", TAG_GROUP_DEVICE);
        LUAS_ENUM_VARIANT(state, TagGroup, "detail_object_collection", TAG_GROUP_DETAIL_OBJECT_COLLECTION);
        LUAS_ENUM_VARIANT(state, TagGroup, "effect", TAG_GROUP_EFFECT);
        LUAS_ENUM_VARIANT(state, TagGroup, "equipment", TAG_GROUP_EQUIPMENT);
        LUAS_ENUM_VARIANT(state, TagGroup, "flag", TAG_GROUP_FLAG);
        LUAS_ENUM_VARIANT(state, TagGroup, "fog", TAG_GROUP_FOG);
        LUAS_ENUM_VARIANT(state, TagGroup, "font", TAG_GROUP_FONT);
        LUAS_ENUM_VARIANT(state, TagGroup, "material_effects", TAG_GROUP_MATERIAL_EFFECTS);
        LUAS_ENUM_VARIANT(state, TagGroup, "garbage", TAG_GROUP_GARBAGE);
        LUAS_ENUM_VARIANT(state, TagGroup, "glow", TAG_GROUP_GLOW);
        LUAS_ENUM_VARIANT(state, TagGroup, "grenade_hud_interface", TAG_GROUP_GRENADE_HUD_INTERFACE);
        LUAS_ENUM_VARIANT(state, TagGroup, "hud_message_text", TAG_GROUP_HUD_MESSAGE_TEXT);
        LUAS_ENUM_VARIANT(state, TagGroup, "hud_number", TAG_GROUP_HUD_NUMBER);
        LUAS_ENUM_VARIANT(state, TagGroup, "hud_globals", TAG_GROUP_HUD_GLOBALS);
        LUAS_ENUM_VARIANT(state, TagGroup, "item", TAG_GROUP_ITEM);
        LUAS_ENUM_VARIANT(state, TagGroup, "item_collection", TAG_GROUP_ITEM_COLLECTION);
        LUAS_ENUM_VARIANT(state, TagGroup, "damage_effect", TAG_GROUP_DAMAGE_EFFECT);
        LUAS_ENUM_VARIANT(state, TagGroup, "lens_flare", TAG_GROUP_LENS_FLARE);
        LUAS_ENUM_VARIANT(state, TagGroup, "lightning", TAG_GROUP_LIGHTNING);
        LUAS_ENUM_VARIANT(state, TagGroup, "device_light_fixture", TAG_GROUP_DEVICE_LIGHT_FIXTURE);
        LUAS_ENUM_VARIANT(state, TagGroup, "light", TAG_GROUP_LIGHT);
        LUAS_ENUM_VARIANT(state, TagGroup, "sound_looping", TAG_GROUP_SOUND_LOOPING);
        LUAS_ENUM_VARIANT(state, TagGroup, "device_machine", TAG_GROUP_DEVICE_MACHINE);
        LUAS_ENUM_VARIANT(state, TagGroup, "globals", TAG_GROUP_GLOBALS);
        LUAS_ENUM_VARIANT(state, TagGroup, "meter", TAG_GROUP_METER);
        LUAS_ENUM_VARIANT(state, TagGroup, "light_volume", TAG_GROUP_LIGHT_VOLUME);
        LUAS_ENUM_VARIANT(state, TagGroup, "gbxmodel", TAG_GROUP_GBXMODEL);
        LUAS_ENUM_VARIANT(state, TagGroup, "model", TAG_GROUP_MODEL);
        LUAS_ENUM_VARIANT(state, TagGroup, "multiplayer_scenario_description", TAG_GROUP_MULTIPLAYER_SCENARIO_DESCRIPTION);
        LUAS_ENUM_VARIANT(state, TagGroup, "null", TAG_GROUP_NULL);
        LUAS_ENUM_VARIANT(state, TagGroup, "preferences_network_game", TAG_GROUP_PREFERENCES_NETWORK_GAME);
        LUAS_ENUM_VARIANT(state, TagGroup, "object", TAG_GROUP_OBJECT);
        LUAS_ENUM_VARIANT(state, TagGroup, "particle", TAG_GROUP_PARTICLE);
        LUAS_ENUM_VARIANT(state, TagGroup, "particle_system", TAG_GROUP_PARTICLE_SYSTEM);
        LUAS_ENUM_VARIANT(state, TagGroup, "physics", TAG_GROUP_PHYSICS);
        LUAS_ENUM_VARIANT(state, TagGroup, "placeholder", TAG_GROUP_PLACEHOLDER);
        LUAS_ENUM_VARIANT(state, TagGroup, "point_physics", TAG_GROUP_POINT_PHYSICS);
        LUAS_ENUM_VARIANT(state, TagGroup, "projectile", TAG_GROUP_PROJECTILE);
        LUAS_ENUM_VARIANT(state, TagGroup, "weather_particle_system", TAG_GROUP_WEATHER_PARTICLE_SYSTEM);
        LUAS_ENUM_VARIANT(state, TagGroup, "scenario_structure_bsp", TAG_GROUP_SCENARIO_STRUCTURE_BSP);
        LUAS_ENUM_VARIANT(state, TagGroup, "scenery", TAG_GROUP_SCENERY);
        LUAS_ENUM_VARIANT(state, TagGroup, "shader_transparent_chicago_extended", TAG_GROUP_SHADER_TRANSPARENT_CHICAGO_EXTENDED);
        LUAS_ENUM_VARIANT(state, TagGroup, "shader_transparent_chicago", TAG_GROUP_SHADER_TRANSPARENT_CHICAGO);
        LUAS_ENUM_VARIANT(state, TagGroup, "scenario", TAG_GROUP_SCENARIO);
        LUAS_ENUM_VARIANT(state, TagGroup, "shader_environment", TAG_GROUP_SHADER_ENVIRONMENT);
        LUAS_ENUM_VARIANT(state, TagGroup, "shader_transparent_glass", TAG_GROUP_SHADER_TRANSPARENT_GLASS);
        LUAS_ENUM_VARIANT(state, TagGroup, "shader", TAG_GROUP_SHADER);
        LUAS_ENUM_VARIANT(state, TagGroup, "sky", TAG_GROUP_SKY);
        LUAS_ENUM_VARIANT(state, TagGroup, "shader_transparent_meter", TAG_GROUP_SHADER_TRANSPARENT_METER);
        LUAS_ENUM_VARIANT(state, TagGroup, "sound", TAG_GROUP_SOUND);
        LUAS_ENUM_VARIANT(state, TagGroup, "sound_environment", TAG_GROUP_SOUND_ENVIRONMENT);
        LUAS_ENUM_VARIANT(state, TagGroup, "shader_model", TAG_GROUP_SHADER_MODEL);
        LUAS_ENUM_VARIANT(state, TagGroup, "shader_transparent_generic", TAG_GROUP_SHADER_TRANSPARENT_GENERIC);
        LUAS_ENUM_VARIANT(state, TagGroup, "ui_widget_collection", TAG_GROUP_UI_WIDGET_COLLECTION);
        LUAS_ENUM_VARIANT(state, TagGroup, "shader_transparent_plasma", TAG_GROUP_SHADER_TRANSPARENT_PLASMA);
        LUAS_ENUM_VARIANT(state, TagGroup, "sound_scenery", TAG_GROUP_SOUND_SCENERY);
        LUAS_ENUM_VARIANT(state, TagGroup, "string_list", TAG_GROUP_STRING_LIST);
        LUAS_ENUM_VARIANT(state, TagGroup, "shader_transparent_water", TAG_GROUP_SHADER_TRANSPARENT_WATER);
        LUAS_ENUM_VARIANT(state, TagGroup, "tag_collection", TAG_GROUP_TAG_COLLECTION);
        LUAS_ENUM_VARIANT(state, TagGroup, "camera_track", TAG_GROUP_CAMERA_TRACK);
        LUAS_ENUM_VARIANT(state, TagGroup, "dialogue", TAG_GROUP_DIALOGUE);
        LUAS_ENUM_VARIANT(state, TagGroup, "unit_hud_interface", TAG_GROUP_UNIT_HUD_INTERFACE);
        LUAS_ENUM_VARIANT(state, TagGroup, "unit", TAG_GROUP_UNIT);
        LUAS_ENUM_VARIANT(state, TagGroup, "unicode_string_list", TAG_GROUP_UNICODE_STRING_LIST);
        LUAS_ENUM_VARIANT(state, TagGroup, "virtual_keyboard", TAG_GROUP_VIRTUAL_KEYBOARD);
        LUAS_ENUM_VARIANT(state, TagGroup, "vehicle", TAG_GROUP_VEHICLE);
        LUAS_ENUM_VARIANT(state, TagGroup, "weapon", TAG_GROUP_WEAPON);
        LUAS_ENUM_VARIANT(state, TagGroup, "wind", TAG_GROUP_WIND);
        LUAS_ENUM_VARIANT(state, TagGroup, "weapon_hud_interface", TAG_GROUP_WEAPON_HUD_INTERFACE);
        lua_pop(state, 1);
    }

    void push_tag_group(lua_State *state, TagGroup group) noexcept {
        LUAS_PUSH_ENUM_VARIANT(state, TagGroup, group);
    }

    std::optional<TagGroup> get_tag_group(lua_State *state, int idx) noexcept {
        return GET_ENUM_VARIANT_VALUE(state, idx, TagGroup);
    }

    static void define_tag_handle_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, TagHandle);
        LUAS_PRIMITIVE_FIELD(state, TagHandle, value, LUAST_UINT32, 0);
        LUAS_PRIMITIVE_FIELD(state, TagHandle, id, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, TagHandle, index, LUAST_INT16, 0);
        LUAS_METHOD_FIELD(state, TagHandle, "isNull", [](lua_State *state) -> int {
            auto handle = get_table_resource_handle(state, 1);
            if(!handle.has_value()) {
                return luaL_error(state, "Invalid TableResourceHandle object.");
            }
            lua_pushboolean(state, HANDLE_IS_NULL(handle.value()));
            return 1;
        });
        lua_pop(state, 1);
    }

    void push_tag_handle(lua_State *state, const TagHandle &handle, bool readonly) noexcept {
        LUAS_NEW_OBJECT(state, TagHandle, readonly);
        TagHandle *tag_handle = LUAS_CHECK_OBJECT(state, -1, TagHandle);
        tag_handle->value = handle.value;
    }

    std::optional<TagHandle> get_tag_handle(lua_State *state, int idx) noexcept {
        std::optional<TagHandle> handle;
        if(lua_istable(state, idx) || lua_isuserdata(state, idx)) {
            lua_getfield(state, idx, "value");
            if(lua_isinteger(state, -1)) {
                handle = TagHandle();
                handle->value = luaL_checkinteger(state, -1);
            }
            lua_pop(state, 1);
        } 
        else if(lua_isinteger(state, idx)) {
            handle = TagHandle();
            handle->value = luaL_checkinteger(state, idx);
        }
        return handle;
    }

    static void define_tag_entry_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, TagEntry);
        LUAS_PRIMITIVE_FIELD(state, TagEntry, path, LUAST_STRING_LITERAL, LUAS_FIELD_READONLY);
        LUAS_OBJREF_FIELD(state, TagEntry, handle, TagHandle, LUAS_FIELD_READONLY);
        LUAS_ENUM_FIELD(state, TagEntry, primary_group, TagGroup, LUAS_FIELD_READONLY);
        LUAS_ENUM_FIELD(state, TagEntry, secondary_group, TagGroup, LUAS_FIELD_READONLY);
        LUAS_ENUM_FIELD(state, TagEntry, tertiary_group, TagGroup, LUAS_FIELD_READONLY);
        LUAS_METHOD_FIELD(state, TagEntry, "getData", +[](lua_State *state) -> int {
            if(lua_gettop(state) != 1) {
                return luaL_error(state, "Invalid number of arguments to TagEntry:getData. Expected to be called using the self operator.");
            }
            TagEntry *entry = LUAS_CHECK_OBJECT(state, 1, TagEntry);
            push_tag_data(state, entry);
            return 1;
        });
        lua_pop(state, 1);
    }

    void push_tag_entry(lua_State *state, const TagEntry &entry, bool readonly) noexcept {
        LUAS_PUSH_OBJECT(state, TagEntry, &entry, readonly);
    }

    static void define_tag_reference_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, TagReference);
        LUAS_OBJREF_FIELD(state, TagReference, tag_handle, TagHandle, LUAS_FIELD_READONLY);
        LUAS_ENUM_FIELD(state, TagReference, tag_group, TagGroup, LUAS_FIELD_READONLY);
        lua_pop(state, 1);
    }

    void push_tag_reference(lua_State *state, const TagReference &reference, bool readonly) noexcept {
        LUAS_PUSH_OBJECT(state, TagReference, &reference, readonly);
    }

    static void define_tag_raw_data_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, TagRawData);
        LUAS_PRIMITIVE_FIELD(state, TagRawData, pointer, LUAST_UINT32, 0);
        LUAS_PRIMITIVE_FIELD(state, TagRawData, size, LUAST_INT32, 0);
        lua_pop(state, 1);
    }

    void push_tag_raw_data(lua_State *state, const TagRawData &raw_data, bool readonly) noexcept {
        LUAS_PUSH_OBJECT(state, TagRawData, &raw_data, readonly);
    }

    void define_engine_tag_types(lua_State *state) noexcept {
        define_tag_group_type(state);
        define_tag_handle_type(state);
        define_tag_entry_type(state);
        define_tag_reference_type(state);
        define_tag_raw_data_type(state);
    }
}
