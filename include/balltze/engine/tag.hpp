// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__ENGINE__TAG_HPP
#define BALLTZE_API__ENGINE__TAG_HPP

#include <string>
#include <functional>
#include <optional>
#include <cstring>
#include <cstddef>

#include "../memory.hpp"
#include "data_types.hpp"
#include "tag_definitions/definitions.hpp"

namespace Balltze::Engine {
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

    /**
     * Tags are the building block of a map
     */
    struct BALLTZE_API Tag {
        /** Primary tag class; this value is only read for a few tags as well as any indexed tags */
        TagClassInt primary_class;

        /** Secondary class; Unused */
        TagClassInt secondary_class;

        /** Tertiary class; Unused */
        TagClassInt tertiary_class;

        /** Tag ID of the tag */
        TagHandle id;

        /** Tag path; this value *can* be invalid for some tags on protected maps; always check if it's within 0x40440000 - 0x41B40000 before trying to read it!! */
        char *path;

        /**
         * Tag data; like tag paths, this value *can* be invalid for some tags on protected maps if the tag isn't used (a red herring). Be careful!!
         * Some tags also have this as an index value to tell Halo to load tag data, but only when the map is initially loaded
         */
        std::byte *data;

        /**
         * This is set to 1 if indexed. Otherwise unused once the map is fully loaded
         */
        std::uint32_t indexed;

        PADDING(0x4);

        /**
         * Get tag data as a specific type
         * @tparam T Type to get tag data as
         * @return   Tag data as type T
         */
        template<typename T>
        T *get_data() noexcept {
            return reinterpret_cast<T *>(this->data);
        }

        /**
         * Fix tag offsets with a new data address 
         * @param new_data_address              New data address for tag data
         * @param external_data_offset_resolver Function to resolve external data offsets
         */
        void fix_data_offsets(std::byte *new_data_address, std::optional<std::function<std::uint32_t(std::uint32_t)>> external_data_offset_resolver = std::nullopt);

        /**
         * Fix tag dependencies
         * @param dependency_resolver Dependency resolver
         */
        void fix_dependencies(std::function<TagDependency(TagDependency)> dependency_resolver);
    };
    static_assert(sizeof(Tag) == 0x20);

    /**
     * This is the header used for tag data. It's always loaded at 0x40440000 on retail and Custom Edition
     */
    struct TagDataHeader {
        /** Pointer to first tag in tag array */
        Tag *tag_array;

        /** Main scenario tag ID - pretty much every map has this as #0 as tool always puts it first, but don't rely on it (it's a great map protection that breaks most tools) */
        TagHandle scenario_tag;

        /** Unused random number */
        std::uint32_t random_number;

        /** Number of tags in tag array */
        std::uint32_t tag_count;

        /** Model part count (unused?) */
        std::uint32_t model_part_count;

        /** Model data file offset */
        std::uint32_t model_data_file_offset;

        /** Model part count (unused?) */
        std::uint32_t model_part_count_again;

        /** Vertex size and offset of indices from model_data_file_offset */
        std::uint32_t vertex_size;

        /** Size of the model data */
        std::uint32_t model_data_size;

        /** Literally says "tags" */
        std::uint32_t tags_literal;
    };
    static_assert(sizeof(TagDataHeader) == 0x28);

    /**
     * Get the tag data address
     * @return tag data address
     */
    BALLTZE_API std::byte *get_tag_data_address() noexcept;

    inline TagDataHeader &get_tag_data_header() noexcept {
        return *reinterpret_cast<TagDataHeader *>(get_tag_data_address());
    }

    /**
     * Get the tag
     * @param  tag_id id of the tag
     * @return        pointer to the tag if found, nullptr if not
     */
    BALLTZE_API Tag *get_tag(TagHandle tag_id) noexcept;

    /**
     * Get the tag
     * @param  tag_index index of the tag
     * @return           pointer to the tag if found, nullptr if not
     */
    BALLTZE_API Tag *get_tag(std::size_t tag_index) noexcept;

    /**
     * Get the tag
     * @param  path      path of the tag
     * @param  tag_class class of the tag
     * @return           pointer to the tag if found, nullptr if not
     */
    BALLTZE_API Tag *get_tag(std::string path, std::uint32_t tag_class) noexcept;

    /**
     * Get tag class from a given string
     * @return  A tag class int
    */
    TagClassInt tag_class_from_string(std::string tag_class_name) noexcept;
}

#endif
