// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/memory.hpp>
#include <balltze/engine/tag.hpp>
#include <balltze/engine/hud.hpp>

namespace Balltze::Engine {
    extern "C" {
        void hud_draw_bitmap_sprite(Point2DInt position, ColorARGBInt color, TagDefinitions::BitmapData *bitmap_data, TagDefinitions::BitmapGroupSprite *sprite);
    }

    HudGlobals &get_hud_globals() {
        static auto *sig = Memory::get_signature("hud_icon_messages_tag_id");
        if(!sig) {
            throw std::runtime_error("Could not find signature for hud icon messages tag id");
        }
        static auto **hud_globals = *reinterpret_cast<HudGlobals ***>(sig->data());
        return **hud_globals;
    }

    std::pair<std::size_t, std::size_t> get_bitmap_sprite_size(TagHandle bitmap_tag_handle, std::size_t sequence_index, std::size_t sprite_index) {
        auto bitmap_tag = get_tag(bitmap_tag_handle);
        if(!bitmap_tag) {
            throw std::runtime_error("Invalid bitmap tag handle");
        }
        auto *bitmap_tag_data = bitmap_tag->get_data<TagDefinitions::Bitmap>();
        if(sequence_index >= bitmap_tag_data->bitmap_group_sequence.count) {
            throw std::runtime_error("Invalid sequence index");
        }
        auto &sequence = bitmap_tag_data->bitmap_group_sequence.offset[sequence_index];
        if(sprite_index >= sequence.sprites.count) {
            throw std::runtime_error("Invalid sprite index");
        }
        auto &sprite = sequence.sprites.offset[sprite_index];
        auto &bitmap_data = bitmap_tag_data->bitmap_data.offset[sprite.bitmap_index];

        auto sprite_width = (sprite.right - sprite.left) * bitmap_data.width;
        auto sprite_height = (sprite.bottom - sprite.top) * bitmap_data.height;

        return {sprite_width, sprite_height};
    }

    void draw_hud_message_sprite(TagHandle bitmap_tag_handle, std::size_t sequence_index, std::size_t sprite_index, Point2DInt position, ColorARGBInt color) {
        auto bitmap_tag = get_tag(bitmap_tag_handle);
        if(!bitmap_tag) {
            throw std::runtime_error("Invalid bitmap tag handle");
        }
        auto *bitmap_tag_data = bitmap_tag->get_data<TagDefinitions::Bitmap>();
        if(sequence_index >= bitmap_tag_data->bitmap_group_sequence.count) {
            throw std::runtime_error("Invalid sequence index");
        }
        auto &sequence = bitmap_tag_data->bitmap_group_sequence.offset[sequence_index];
        if(sprite_index >= sequence.sprites.count) {
            throw std::runtime_error("Invalid sprite index");
        }
        auto &sprite = sequence.sprites.offset[sprite_index];
        auto &bitmap_data = bitmap_tag_data->bitmap_data.offset[sprite.bitmap_index];

        auto offset = position;
        auto [sprite_width, sprite_height] = get_bitmap_sprite_size(bitmap_tag_handle, sequence_index, sprite_index);

        // align to text baseline
        offset.y += sprite_height * 0.75f;

        hud_draw_bitmap_sprite(offset, color, &bitmap_data, &sprite);
    }
}
