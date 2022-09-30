#ifndef BALLTZE__ENGINE__TAG_DEFINITIONS__FONT_HPP
#define BALLTZE__ENGINE__TAG_DEFINITIONS__FONT_HPP

#include <cstdint>
#include "types.hpp"

namespace Balltze::Engine { 
	struct FontCharacterIndex {
		std::uint16_t character_index;
	};
	struct FontCharacterTables {
		TagReflexive<FontCharacterIndex> character_table;
	};
	struct FontCharacter {
		std::uint16_t character;
		std::int16_t character_width;
		std::int16_t bitmap_width;
		std::int16_t bitmap_height;
		std::int16_t bitmap_origin_x;
		std::int16_t bitmap_origin_y;
		std::uint16_t hardware_character_index;
		std::byte pad_8[2];
		std::int32_t pixels_offset;
	};
	struct Font {
		std::int32_t flags;
		std::int16_t ascending_height;
		std::int16_t descending_height;
		std::int16_t leading_height;
		std::int16_t leading_width;
		std::byte pad_6[36];
		TagReflexive<FontCharacterTables> character_tables;
		TagDependency bold;
		TagDependency italic;
		TagDependency condense;
		TagDependency underline;
		TagReflexive<FontCharacter> characters;
		TagDataOffset pixels;
	};
}

#endif
