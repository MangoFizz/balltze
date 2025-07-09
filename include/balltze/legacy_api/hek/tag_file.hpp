// SPDX-License-Identifier: GPL-3.0-only

/**
 * Halo Editing Kit tag file structures.
 * For the most part, these are just wrappers around the engine common data types.
 * Structures from Invader.
 */

#ifndef BALLTZE_LEGACY_API__HEK__TAG_FILE_HPP
#define BALLTZE_LEGACY_API__HEK__TAG_FILE_HPP

#include <optional>
#include "../../api.hpp"
#include "../../memory.hpp"
#include "../engine/data_types.hpp"
#include "../engine/script.hpp"
#include "../engine/tag_classes.hpp"

namespace Balltze::LegacyApi::HEK {
    using Angle = Memory::BigEndian<float>;
	using Fraction = Memory::BigEndian<float>;
	using Index = Memory::BigEndian<std::uint16_t>;
	using TagEnum = Memory::BigEndian<std::uint16_t>;
	using Matrix = Memory::BigEndian<float>[3][3];
    using TagHandle = LegacyApi::Engine::ResourceHandle;
    using TagString = LegacyApi::Engine::TagString;
    using ColorARGBInt = LegacyApi::Engine::ColorARGBInt;
    using ScenarioScriptNodeValue = Memory::BigEndian<LegacyApi::Engine::ScenarioScriptNodeValue>;
	using TagClassInt = LegacyApi::Engine::TagClassInt;

    /**
     * This header takes up the first 64 bytes of a Halo Editing Kit tag file.
     * It is followed by the tag data.
     */
    struct TagFileHeader {
        enum : std::uint32_t {
            BLAM = 0x626C616D
        };

        /** In some of the older point physics tags, this is some sort of tag handle; it's unused */
        Memory::BigEndian<TagHandle> tag_handle_unused;

        /** In some of the older point physics tags, this is set to the tag name; it's unused */
        TagString tag_name_unused;

        /** Tag class of this tag */
        Memory::BigEndian<TagClassInt> tag_class;

        /** CRC32? Unread? */
        Memory::BigEndian<std::uint32_t> crc32;

        /** Equals 0x40. Unread? */
        Memory::BigEndian<std::uint32_t> header_size;

        PADDING(0x8);

        /** Some sort of version value */
        Memory::BigEndian<std::uint16_t> version;

        /** Always 255. Unread? */
        Memory::BigEndian<std::uint16_t> something_255;

        /** Must be equal to 0x626C616D */
        Memory::BigEndian<std::uint32_t> blam;

        /** Get the correct version value for the tag */
        static std::uint16_t version_for_tag(TagClassInt tag_class);

        /** Generate a new tag file header for a tag class */
        TagFileHeader(TagClassInt tag_class);

        /**
         * Validate the header, throwing an exception if not valid
         * @param header             pointer to the header to validate
         * @param tag_file_size      size of the tag file
         * @param expected_tag_class tag class expected
         * @throws                   std::exception if not valid
         */
        static void validate_header(const TagFileHeader *header, std::size_t tag_file_size, std::optional<TagClassInt> expected_tag_class = std::nullopt);

        /** Make a blank tag file header. No values will be initialized. */
        TagFileHeader() = default;

        TagFileHeader(const TagFileHeader &copy) = default;
        ~TagFileHeader() = default;
    };
    static_assert(sizeof(TagFileHeader) == 0x40);

    template<typename T> struct TagBlock {
        Memory::BigEndian<std::uint32_t> count;
        Memory::BigEndian<std::uint32_t> offset;
		void *definition;

		T *element(std::uint32_t index = 0) {
			if(index >= count) {
				return nullptr;
			}
			return reinterpret_cast<T *>(reinterpret_cast<std::byte *>(this) + sizeof(TagBlock<T>) + offset);
		}
	};
	static_assert(sizeof(TagBlock<void>) == 0xC);

	struct TagDependency {
		TagClassInt tag_class;
		Memory::BigEndian<std::uint32_t> path_pointer;
		std::size_t path_size;
		Memory::BigEndian<TagHandle> tag_handle;
	};
    static_assert(sizeof(TagDependency) == 0x10);

	struct Point2D {
		Memory::BigEndian<float> x;
		Memory::BigEndian<float> y;
	};
    static_assert(sizeof(Point2D) == 0x8);

	struct Point3D {
		Memory::BigEndian<float> x;
		Memory::BigEndian<float> y;
		Memory::BigEndian<float> z;
	};
	static_assert(sizeof(Point3D) == 0xC);

	struct TagDataOffset {
		Memory::BigEndian<std::uint32_t> size;
		Memory::BigEndian<std::uint32_t> external;
		Memory::BigEndian<std::uint32_t> file_offset;
		std::byte *pointer;
		std::byte pad_5[4];
	};
    static_assert(sizeof(TagDataOffset) == 0x14);

	struct ColorARGB {
		Memory::BigEndian<float> alpha;
		Memory::BigEndian<float> red;
		Memory::BigEndian<float> green;
		Memory::BigEndian<float> blue;
	};
    static_assert(sizeof(ColorARGB) == 0x10);

	struct Rectangle2D {
		Memory::BigEndian<std::int16_t> top;
		Memory::BigEndian<std::int16_t> left;
		Memory::BigEndian<std::int16_t> bottom;
		Memory::BigEndian<std::int16_t> right;
	};
    static_assert(sizeof(Rectangle2D) == 0x8);

	struct Point2DInt {
		Memory::BigEndian<std::int16_t> x;
		Memory::BigEndian<std::int16_t> y;
	};
    static_assert(sizeof(Point2DInt) == 0x4);

	struct Euler2D {
		Memory::BigEndian<float> yaw;
		Memory::BigEndian<float> pitch;
	};
	static_assert(sizeof(Euler2D) == 0x8);

	struct Euler3D {
		Memory::BigEndian<float> yaw;
		Memory::BigEndian<float> pitch;
		Memory::BigEndian<float> roll;
	};
	static_assert(sizeof(Euler3D) == 0xC);

	struct Vector2D {
		Memory::BigEndian<float> i;
		Memory::BigEndian<float> j;
	};
	static_assert(sizeof(Vector2D) == 0x8);

	struct Vector3D {
		Memory::BigEndian<float> i;
		Memory::BigEndian<float> j;
		Memory::BigEndian<float> k;
	};
	static_assert(sizeof(Vector3D) == 0xC);

	struct ColorRGB {
		Memory::BigEndian<float> red;
		Memory::BigEndian<float> green;
		Memory::BigEndian<float> blue;
	};
	static_assert(sizeof(ColorRGB) == 0xC);

	struct Quaternion {
		Memory::BigEndian<float> i;
		Memory::BigEndian<float> j;
		Memory::BigEndian<float> k;
		Memory::BigEndian<float> w;
	};
	static_assert(sizeof(Quaternion) == 0x10);

	struct Plane3D {
		Vector3D vector;
		Memory::BigEndian<float> w;
	};
	static_assert(sizeof(Plane3D) == 0x10);

	struct Plane2D {
		Vector2D vector;
		Memory::BigEndian<float> w;
	};
	static_assert(sizeof(Plane2D) == 0xC);
}

#endif
