// SPDX-License-Identifier: GPL-3.0-only

#include <chrono>
#include <balltze/events/map_load.hpp>
#include <balltze/engine/tag.hpp>
#include <balltze/engine/renderer.hpp>
#include <balltze/memory.hpp>
#include "../config/config.hpp"
#include "../logger.hpp"

namespace Balltze::Features {
    static bool preload_map_textures = false;
    static std::size_t min_map_size;

    static void on_map_file_load(Event::MapFileLoadEvent &event) {
        if(event.time == Event::EVENT_TIME_BEFORE) {
            auto map_size = std::filesystem::file_size(event.args.map_path);
            preload_map_textures = map_size > MIB_SIZE * 384;
        }
    }

    static void on_map_load(Event::MapLoadEvent &event) {
        if(event.time == Event::EVENT_TIME_AFTER && preload_map_textures) {
            logger.info("Preloading map textures...");
            auto start = std::chrono::steady_clock::now();
            std::size_t count = 0;
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
                        count++;
                    }
                }
            }
            auto end = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            logger.debug("Loaded {} textures in {} ms", count, duration.count());
        }
    }

    void set_up_map_textures_preloading() noexcept {
        Event::TickEvent::subscribe([](Event::TickEvent &event) {
            auto &config = Config::get_config();
            auto enable = config.get<bool>("preload_map_textures.enable").value_or(false);
            if(enable) {
                min_map_size = config.get<std::size_t>("preload_map_textures.min_map_size").value_or(384);
                Event::MapLoadEvent::subscribe(on_map_load, Event::EVENT_PRIORITY_LOWEST);
                Event::MapFileLoadEvent::subscribe(on_map_file_load, Event::EVENT_PRIORITY_LOWEST);
            }
        });
    }
}
