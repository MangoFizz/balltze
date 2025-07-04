// SPDX-License-Identifier: GPL-3.0-only

#include <chrono>
#include <balltze/legacy_api/events/map_load.hpp>
#include <balltze/legacy_api/engine/tag.hpp>
#include <balltze/legacy_api/engine/rasterizer.hpp>
#include <balltze/memory.hpp>
#include "../config/config.hpp"
#include "../logger.hpp"

namespace Balltze::Features {
    using namespace LegacyApi::Engine;

    static bool preload_map_textures = false;
    static std::size_t min_map_size;

    static void on_map_file_load(LegacyApi::Event::MapFileLoadEvent &event) {
        if(event.time == LegacyApi::Event::EVENT_TIME_BEFORE) {
            if(event.context.map_name != "ui") {
                auto map_size = std::filesystem::file_size(event.context.map_path);
                preload_map_textures = map_size > MIB_SIZE * 384;
            }
            else {
                preload_map_textures = false;
            }
        }
    }

    static void on_map_load(LegacyApi::Event::MapLoadEvent &event) {
        if(event.time == LegacyApi::Event::EVENT_TIME_AFTER && preload_map_textures) {
            logger.info("Preloading map textures...");
            auto start = std::chrono::steady_clock::now();
            std::size_t count = 0;
            auto &tag_data_address = get_tag_data_header();
            for(std::size_t i = 0; i < tag_data_address.tag_count; i++) {
                auto &tag = tag_data_address.tag_array[i];
                if(tag.indexed) {
                    continue;
                }
                if(tag.primary_class == TAG_CLASS_BITMAP) {
                    auto *bitmap = reinterpret_cast<TagDefinitions::Bitmap *>(tag.data);
                    for(std::size_t j = 0; j < bitmap->bitmap_data.count; j++) {
                        auto &bitmap_data = bitmap->bitmap_data.elements[j];
                        load_bitmap_data_texture(&bitmap_data, true, true);
                        count++;
                    }
                }
            }
            auto end = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            logger.debug("Loaded {} textures in {} ms", count, duration.count());
        }
    }

    LegacyApi::Event::EventListenerHandle<LegacyApi::Event::TickEvent> map_textures_preloading_listener;

    void set_up_map_textures_preloading() noexcept {
        map_textures_preloading_listener = LegacyApi::Event::TickEvent::subscribe([](LegacyApi::Event::TickEvent &event) {
            auto &config = Config::get_config();
            auto enable = config.get<bool>("preload_map_textures.enable").value_or(false);
            if(enable) {
                min_map_size = config.get<std::size_t>("preload_map_textures.min_map_size").value_or(384);
                LegacyApi::Event::MapLoadEvent::subscribe(on_map_load, LegacyApi::Event::EVENT_PRIORITY_LOWEST);
                LegacyApi::Event::MapFileLoadEvent::subscribe(on_map_file_load, LegacyApi::Event::EVENT_PRIORITY_LOWEST);
            }
            map_textures_preloading_listener.remove();
        });
    }
}
