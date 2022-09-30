#ifndef BALLTZE__ENGINE__TAG_DEFINITIONS__BITMAP_HPP
#define BALLTZE__ENGINE__TAG_DEFINITIONS__BITMAP_HPP

#include <cstdint>
#include "types.hpp"

namespace Balltze::Engine { 
	struct BitmapGroupSprite {
		std::uint16_t bitmap_index;
		std::byte pad_2[2];
		std::byte pad_3[4];
		float left;
		float right;
		float top;
		float bottom;
		Point2D registration_point;
	};
	struct BitmapGroupSequence {
		char name[32];
		std::uint16_t first_bitmap_index;
		std::uint16_t bitmap_count;
		std::byte pad_4[16];
		TagReflexive<BitmapGroupSprite> sprites;
	};
	struct BitmapData {
		char tag_fourcc[4];
		std::uint16_t width;
		std::uint16_t height;
		std::uint16_t depth;
		std::uint16_t type;
		std::uint16_t format;
		std::uint16_t flags;
		Point2DInt registration_point;
		std::uint16_t mipmap_count;
		std::byte pad_10[2];
		std::uint32_t pixel_data_offset;
		std::uint32_t pixel_data_size;
		std::uint32_t bitmap_tag_id;
		std::uint32_t pointer;
		std::byte pad_15[4];
		std::byte pad_16[4];
	};
	struct Bitmap {
		std::uint16_t type;
		std::uint16_t encoding_format;
		std::uint16_t usage;
		std::uint16_t flags;
		float detail_fade_factor;
		float sharpen_amount;
		float bump_height;
		std::uint16_t sprite_budget_size;
		std::uint16_t sprite_budget_count;
		std::uint16_t color_plate_width;
		std::uint16_t color_plate_height;
		TagDataOffset compressed_color_plate_data;
		TagDataOffset processed_pixel_data;
		float blur_filter_size;
		float alpha_bias;
		std::uint16_t mipmap_count;
		std::uint16_t sprite_usage;
		std::uint16_t sprite_spacing;
		std::byte pad_19[2];
		TagReflexive<BitmapGroupSequence> bitmap_group_sequence;
		TagReflexive<BitmapData> bitmap_data;
	};
}

#endif
