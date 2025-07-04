// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__ENGINE__DATA_TYPES_HPP
#define BALLTZE__ENGINE__DATA_TYPES_HPP

#include <cstdint> 
#include <cstddef>
#include "../../memory.hpp"

namespace Balltze::LegacyApi::Engine {
	using Angle = float;
	using Fraction = float;
	using Index = std::uint16_t;
	using TagEnum = std::uint16_t;
    using Point = float;
	using TickCount32 = std::uint32_t;
    using TickCount16 = std::uint16_t;

	struct TagString {
        char string[0x20] = {};

        TagString() = default;
    };

    union ResourceHandle {
        std::uint32_t value;
        struct {
            std::uint16_t index;
            std::uint16_t id;
        };

        ResourceHandle(std::uint32_t handle) {
            this->value = handle;
        }

        ResourceHandle() = default;

        static ResourceHandle null() noexcept {
            return { 0xFFFFFFFF };
        }

        bool is_null() const noexcept {
            return *this == null();
        }

        bool operator==(const ResourceHandle &other) const noexcept {
            return this->value == other.value;
        }

        bool operator!=(const ResourceHandle &other) const noexcept {
            return this->value != other.value;
        }

        bool operator<(const ResourceHandle& other) const noexcept {
            return index < other.index; 
        }
    };
	static_assert(sizeof(ResourceHandle) == sizeof(std::uint32_t));

    using PlayerHandle = ResourceHandle;
    using ObjectHandle = ResourceHandle;
    
	struct ColorARGBInt {
		std::uint8_t blue;
		std::uint8_t green;
		std::uint8_t red;
		std::uint8_t alpha;
	};
    static_assert(sizeof(ColorARGBInt) == 0x4);

	struct Point2D {
		Point x;
		Point y;

		inline Point2D operator+(Point2D const &point) {
            return {
                this->x + point.x,
                this->y + point.y
            };
        }

        friend bool operator==(const Point2D &point_a, const Point2D &point_b) {
            return point_a.x == point_b.x && point_a.y == point_b.y;
        }

        friend bool operator!=(const Point2D &point_a, const Point2D &point_b) {
            return point_a.x != point_b.x || point_a.y != point_b.y;
        }
	};
    static_assert(sizeof(Point2D) == 0x8);

	struct Point3D {
		Point x;
		Point y;
		Point z;

		inline Point3D operator+(Point3D const &point) {
            return {
                this->x + point.x,
                this->y + point.y,
                this->z + point.z
            };
        }

        friend bool operator==(const Point3D &point_a, const Point3D &point_b) {
            return point_a.x == point_b.x && point_a.y == point_b.y && point_a.z == point_b.z;
        }

        friend bool operator!=(const Point3D &point_a, const Point3D &point_b) {
            return point_a.x != point_b.x || point_a.y != point_b.y || point_a.z != point_b.z;
        }
	};
	static_assert(sizeof(Point3D) == 0xC);

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

    struct Rectangle2DF {
        float top;
        float left;
        float bottom;
        float right;
    };
    static_assert(sizeof(Rectangle2DF) == 0x10);

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

	struct Euler3DPYR {
        float pitch;
        float yaw;
        float roll;
    };
	static_assert(sizeof(Euler3DPYR) == 0xC);

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

    struct RotationMatrix;

	struct Quaternion {
		float i;
		float j;
		float k;
		float w;

		Quaternion() noexcept = default;
        Quaternion(const RotationMatrix &matrix) noexcept;
        Quaternion(const Quaternion &) noexcept = default;
        Quaternion &operator =(const Quaternion &) noexcept = default;
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

    struct RotationMatrix {
        Point3D v[3];

        RotationMatrix() noexcept;
        RotationMatrix(const Quaternion &quaternion) noexcept;
        RotationMatrix(const RotationMatrix &) noexcept = default;
        RotationMatrix &operator =(const RotationMatrix &) noexcept = default;
    };

	struct Matrix {
		Engine::Vector3D m[3];
	};

	/**
	 * Converts a 32-bit ARGB color to a real ARGB color.
	 * @param color The 32-bit ARGB color.
	 * @return The real ARGB color.
	 */
	ColorARGB color_argb_32_to_real(ColorARGBInt color) noexcept;
}

#endif
