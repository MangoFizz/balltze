// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/events/map_load.hpp>
#include <balltze/engine/tag.hpp>
#include <balltze/engine/renderer.hpp>
#include "../logger.hpp"

namespace Balltze::Features {
    static void on_map_load(Event::MapLoadEvent &event) {
        if(event.time == Event::EVENT_TIME_AFTER) {
            logger.info("Preloading map textures...");
            auto &tag_data_address = Engine::get_tag_data_header();
            for(std::size_t i = 0; i < tag_data_address.tag_count; i++) {
                auto &tag = tag_data_address.tag_array[i];
                if(tag.indexed) {
                    continue;
                }
                if(tag.primary_class == Engine::TAG_CLASS_BITMAP) {
                    auto *bitmap = reinterpret_cast<Engine::TagDefinitions::Bitmap *>(tag.data);
                    for(std::size_t j = 0; j < bitmap->bitmap_data.count; j++) {
                        auto &bitmap_data = bitmap->bitmap_data.offset[j];
                        Engine::load_bitmap(&bitmap_data, true, true);
                    }
                }
            }
        }
    }

    void set_up_map_textures_preloading() noexcept {
        Event::MapLoadEvent::subscribe(on_map_load, Event::EVENT_PRIORITY_HIGHEST);
    }
}
