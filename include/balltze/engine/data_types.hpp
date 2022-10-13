// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__ENGINE__DATA_TYPES_HPP
#define BALLTZE__ENGINE__DATA_TYPES_HPP

#include <cstdint> 
#include <cstddef> 

namespace Balltze::Engine {
    union ElementID {
        std::uint32_t whole_id;
        struct {
            std::uint16_t index;
            std::uint16_t id;
        } index;

        ElementID(std::uint32_t id) {
            this->whole_id = id;
        }

        ElementID() = default;

        static ElementID null_id() noexcept {
            return { 0xFFFFFFFF };
        }

        bool is_null() const noexcept {
            return *this == null_id();
        }

        bool operator==(const ElementID &other) const noexcept {
            return this->whole_id == other.whole_id;
        }

        bool operator!=(const ElementID &other) const noexcept {
            return this->whole_id != other.whole_id;
        }

        bool operator<(const ElementID& other) const noexcept {
            return index.index < other.index.index; 
        }
    };

    using PlayerID = ElementID;
    using TagID = ElementID;

    template<typename T> struct TagReflexive {
        std::uint32_t count;
        T *offset;
		std::byte pad_3[4];
	};
    
	struct ColorARGBInt {
		std::uint8_t blue;
		std::uint8_t green;
		std::uint8_t red;
		std::uint8_t alpha;
	};
    static_assert(sizeof(ColorARGBInt) == 0x4);

	struct TagDependency {
		std::uint32_t tag_fourcc;
		std::uint32_t path_pointer;
		std::uint32_t path_size;
		std::uint32_t tag_id;
	};
    static_assert(sizeof(TagDependency) == 0x10);

	struct Point2D {
		float x;
		float y;
	};
    static_assert(sizeof(Point2D) == 0x8);

	struct TagDataOffset {
		std::uint32_t size;
		std::uint32_t external;
		std::uint32_t file_offset;
		std::uint32_t pointer;
		std::byte pad_5[4];
	};
    static_assert(sizeof(TagDataOffset) == 0x14);

	struct ColorARGB {
		float alpha;
		float red;
		float green;
		float blue;
	};
    static_assert(sizeof(ColorARGB) == 0x10);

	struct Rectangle2D {
		std::int16_t top;
		std::int16_t left;
		std::int16_t bottom;
		std::int16_t right;
	};
    static_assert(sizeof(Rectangle2D) == 0x8);

	struct Point2DInt {
		std::int16_t x;
		std::int16_t y;
	};
    static_assert(sizeof(Point2DInt) == 0x4);

	struct Euler2D {
		float yaw;
		float pitch;
	};
	static_assert(sizeof(Euler2D) == 0x8);

	struct Vector3D {
		float i;
		float j;
		float k;
	};
	static_assert(sizeof(Vector3D) == 0xC);

	struct ColorRGB {
		float red;
		float green;
		float blue;
	};
	static_assert(sizeof(ColorRGB) == 0xC);
}

#endif
