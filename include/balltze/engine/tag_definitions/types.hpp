#ifndef BALLTZE__ENGINE__TAG_DEFINITIONS__TYPES_HPP
#define BALLTZE__ENGINE__TAG_DEFINITIONS__TYPES_HPP

#include <cstddef>

namespace Balltze::Engine { 
	template<typename T> struct TagReflexive
    {
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
	struct TagDependency {
		std::uint32_t tag_fourcc;
		std::uint32_t path_pointer;
		std::uint32_t path_size;
		std::uint32_t tag_id;
	};
	struct Point2D {
		float x;
		float y;
	};
	struct TagDataOffset {
		std::uint32_t size;
		std::uint32_t external;
		std::uint32_t file_offset;
		std::uint32_t pointer;
		std::byte pad_5[4];
	};
	struct ColorARGB {
		float alpha;
		float red;
		float green;
		float blue;
	};
	struct Rectangle2D {
		std::int16_t top;
		std::int16_t left;
		std::int16_t bottom;
		std::int16_t right;
	};
	struct Point2DInt {
		std::int16_t x;
		std::int16_t y;
	};
}

#endif
