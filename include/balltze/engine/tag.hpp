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
#include "tag_classes.hpp"

namespace Balltze::Engine {
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

    std::string tag_class_to_string(TagClassInt tag_class);
}

#endif
