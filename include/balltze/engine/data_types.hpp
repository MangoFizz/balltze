// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__ENGINE__DATA_TYPES_HPP
#define BALLTZE__ENGINE__DATA_TYPES_HPP

#include <cstdint> 
#include <cstddef>
#include "../memory.hpp"

namespace Balltze::Engine {
	using Angle = float;
	using Fraction = float;
	using Index = std::uint16_t;
	using TagEnum = std::uint16_t;
	using TagString = char[32];
	using TagFourCC = char[4];
	using Matrix = float[3][3];

    union ElementHandle {
        std::uint32_t whole_id;
        struct {
            std::uint16_t index;
            std::uint16_t id;
        } index;

        ElementHandle(std::uint32_t id) {
            this->whole_id = id;
        }

        ElementHandle() = default;

        static ElementHandle null() noexcept {
            return { 0xFFFFFFFF };
        }

        bool is_null() const noexcept {
            return *this == null();
        }

        bool operator==(const ElementHandle &other) const noexcept {
            return this->whole_id == other.whole_id;
        }

        bool operator!=(const ElementHandle &other) const noexcept {
            return this->whole_id != other.whole_id;
        }

        bool operator<(const ElementHandle& other) const noexcept {
            return index.index < other.index.index; 
        }
    };
	static_assert(sizeof(ElementHandle) == sizeof(std::uint32_t));

    using PlayerHandle = ElementHandle;
    using TagHandle = ElementHandle;

    template<typename T> struct TagReflexive {
        std::uint32_t count;
        T *offset;
		std::byte pad_3[4];
	};
	static_assert(sizeof(TagReflexive<void>) == 0xC);
    
	struct ColorARGBInt {
		std::uint8_t blue;
		std::uint8_t green;
		std::uint8_t red;
		std::uint8_t alpha;
	};
    static_assert(sizeof(ColorARGBInt) == 0x4);

	struct TagDependency {
		TagFourCC tag_fourcc;
		std::uint32_t path_pointer;
		std::size_t path_size;
		TagHandle tag_id;
	};
    static_assert(sizeof(TagDependency) == 0x10);

	struct Point2D {
		float x;
		float y;
	};
    static_assert(sizeof(Point2D) == 0x8);

	struct Point3D {
		float x;
		float y;
		float z;
	};
	static_assert(sizeof(Point3D) == 0xC);

	struct TagDataOffset {
		std::uint32_t size;
		std::uint32_t external;
		std::uint32_t file_offset;
		std::byte *pointer;
		std::byte pad_5[4];
	};
    static_assert(sizeof(TagDataOffset) == 0x14);

	struct ColorARGB {
		float alpha;
		float red;
		float green;
		float blue;

		ColorARGBInt to_int() const noexcept {
			return {
				static_cast<std::uint8_t>(this->blue * 255.0F),
				static_cast<std::uint8_t>(this->green * 255.0F),
				static_cast<std::uint8_t>(this->red * 255.0F),
				static_cast<std::uint8_t>(this->alpha * 255.0F)
			};
		}
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

	struct Euler3D {
		float yaw;
		float pitch;
		float roll;
	};
	static_assert(sizeof(Euler3D) == 0xC);

	struct Vector2D {
		float i;
		float j;
	};
	static_assert(sizeof(Vector2D) == 0x8);

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

	struct Quaternion {
		float i;
		float j;
		float k;
		float w;
	};
	static_assert(sizeof(Quaternion) == 0x10);

	struct Plane3D {
		Vector3D vector;
		float w;
	};
	static_assert(sizeof(Plane3D) == 0x10);

	struct Plane2D {
		Vector2D vector;
		float w;
	};
	static_assert(sizeof(Plane2D) == 0xC);

	union ScenarioScriptNodeValue {
        std::int8_t bool_int;
        std::int16_t short_int;
        std::int32_t long_int;
        float real;
        TagHandle tag_handle;

        ScenarioScriptNodeValue() = default;
        ScenarioScriptNodeValue(const ScenarioScriptNodeValue &copy) = default;

        ScenarioScriptNodeValue(std::uint8_t v) {
            this->bool_int = v;
        }

        ScenarioScriptNodeValue(std::uint16_t v) {
            this->short_int = v;
        }

        ScenarioScriptNodeValue(std::uint32_t v) {
            this->long_int = v;
        }

        ScenarioScriptNodeValue(float v) {
            this->real = v;
        }

        ScenarioScriptNodeValue(TagHandle v) {
            this->tag_handle = v;
        }

        void operator=(std::uint8_t v) {
            this->long_int = 0xFFFFFFFF;
            this->bool_int = v;
        }

        void operator=(std::uint16_t v) {
            this->long_int = 0xFFFFFFFF;
            this->short_int = v;
        }

        void operator=(std::uint32_t v) {
            this->long_int = v;
        }

        void operator=(float v) {
            this->real = v;
        }

        void operator=(TagHandle v) {
            this->tag_handle = v;
        }
        
        bool operator==(const ScenarioScriptNodeValue &other) const noexcept {
            return this->long_int == other.long_int;
        }
        
        bool operator!=(const ScenarioScriptNodeValue &other) const noexcept {
            return this->long_int != other.long_int;
        }
    };
	static_assert(sizeof(ScenarioScriptNodeValue) == 0x4);
}

#endif
