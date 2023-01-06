// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/memory.hpp>
#include <balltze/engine/tag.hpp>
#include <balltze/engine/hud.hpp>
#include <balltze/engine/widget.hpp>

namespace Balltze::Engine {
    extern "C" {
        void hud_draw_bitmap_sprite(Point2DInt position, ColorARGBInt color, TagDefinitions::BitmapData *bitmap_data, TagDefinitions::BitmapGroupSprite *sprite);
        void get_string_name_for_button(InputDevice input_device, std::size_t button_index, wchar_t *buffer);
    }

    HudGlobals &get_hud_globals() {
        static auto *sig = Memory::get_signature("hud_icon_messages_tag_id");
        if(!sig) {
            throw std::runtime_error("Could not find signature for hud icon messages tag id");
        }
        static auto **hud_globals = *reinterpret_cast<HudGlobals ***>(sig->data());
        return **hud_globals;
    }

    std::pair<std::size_t, std::size_t> get_bitmap_sequence_size(Engine::TagDefinitions::Bitmap *bitmap, std::size_t sequence_index) {
        if(sequence_index >= bitmap->bitmap_group_sequence.count) {
            throw std::runtime_error("Invalid sequence index");
        }
        auto &sequence = bitmap->bitmap_group_sequence.offset[sequence_index];
        if(sequence.sprites.count == 0) {
            throw std::runtime_error("Sequence has no sprites");
        }
        auto &sprite = sequence.sprites.offset[0];
        auto &bitmap_data = bitmap->bitmap_data.offset[sprite.bitmap_index];

        auto sprite_width = (sprite.right - sprite.left) * bitmap_data.width;
        auto sprite_height = (sprite.bottom - sprite.top) * bitmap_data.height;

        return {sprite_width, sprite_height};
    }

    void draw_hud_message_sprite(Engine::TagDefinitions::Bitmap *bitmap, std::size_t sequence_index, std::size_t sprite_index, Point2DInt position, ColorARGBInt color) {
        if(sequence_index >= bitmap->bitmap_group_sequence.count) {
            throw std::runtime_error("Invalid sequence index");
        }
        auto &sequence = bitmap->bitmap_group_sequence.offset[sequence_index];
        if(sprite_index >= sequence.sprites.count) {
            throw std::runtime_error("Invalid sprite index");
        }
        auto &sprite = sequence.sprites.offset[sprite_index];
        auto &bitmap_data = bitmap->bitmap_data.offset[sprite.bitmap_index];

        auto offset = position;
        auto [sprite_width, sprite_height] = get_bitmap_sequence_size(bitmap, sequence_index);

        // align to text baseline
        offset.y += sprite_height * 0.75f;

        hud_draw_bitmap_sprite(offset, color, &bitmap_data, &sprite);
    }

    std::wstring get_button_name(InputDevice input_device, std::size_t button_index) {
        wchar_t button_name[64];
        get_string_name_for_button(input_device, button_index, button_name);
        return button_name;
    }
}
