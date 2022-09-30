#ifndef BALLTZE__ENGINE__TAG_DEFINITIONS__UI_WIDGET_DEFINITION_HPP
#define BALLTZE__ENGINE__TAG_DEFINITIONS__UI_WIDGET_DEFINITION_HPP

#include <cstdint>
#include "types.hpp"

namespace Balltze::Engine { 
	struct GameDataInputReference {
		std::uint16_t function;
		std::byte pad_2[2];
		std::byte pad_3[32];
	};
	struct EventHandlerReference {
		std::uint32_t flags;
		std::uint16_t event_type;
		std::uint16_t function;
		TagDependency widget_tag;
		TagDependency sound_effect;
		char script[32];
	};
	struct SearchAndReplaceReference {
		char search_string[32];
		std::uint16_t replace_function;
	};
	struct ConditionalWidgetReference {
		TagDependency widget_tag;
		char name[32];
		std::uint16_t flags;
		std::uint16_t custom_controller_index;
		std::byte pad_5[26];
	};
	struct ChildWidgetReference {
		TagDependency widget_tag;
		char name[32];
		std::uint16_t flags;
		std::uint16_t custom_controller_index;
		std::int16_t vertical_offset;
		std::int16_t horizontal_offset;
		std::byte pad_7[22];
	};
	struct UIWidgetDefinition {
		std::uint16_t widget_type;
		std::uint16_t controller_index;
		char name[32];
		Rectangle2D bounds;
		std::uint32_t flags;
		std::int32_t milliseconds_to_auto_close;
		std::int32_t milliseconds_auto_close_fade_time;
		TagDependency background_bitmap;
		TagReflexive<GameDataInputReference> game_data_inputs;
		TagReflexive<EventHandlerReference> event_handlers;
		TagReflexive<SearchAndReplaceReference> search_and_replace_functions;
		std::byte pad_12[128];
		TagDependency text_label_unicode_strings_list;
		TagDependency text_font;
		ColorARGB text_color;
		std::uint16_t justification;
		std::uint16_t flags_1;
		std::byte pad_18[12];
		std::uint16_t string_list_index;
		std::int16_t horiz_offset;
		std::int16_t vert_offset;
		std::byte pad_22[26];
		std::byte pad_23[2];
		std::uint32_t flags_2;
		TagDependency list_header_bitmap;
		TagDependency list_footer_bitmap;
		Rectangle2D header_bounds;
		Rectangle2D footer_bounds;
		std::byte pad_29[32];
		TagDependency extended_description_widget;
		std::byte pad_31[32];
		std::byte pad_32[256];
		TagReflexive<ConditionalWidgetReference> conditional_widgets;
		std::byte pad_34[128];
		std::byte pad_35[128];
		TagReflexive<ChildWidgetReference> child_widgets;
	};
}

#endif
