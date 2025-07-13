// SPDX-License-Identifier: GPL-3.0-only

#include <chrono>
#include <balltze/events/map.hpp>
#include <balltze/legacy_api/events/map_load.hpp>

namespace Balltze::Events {
    void dispatch_map_load_event(const char *map_name) {
        MapLoadEvent event(map_name);
        event.dispatch();
    }

    const std::string &MapLoadEvent::map_name() const noexcept {
        return m_map_name;
    }

    template<>
    void EventHandler<MapLoadEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        LegacyApi::Event::MapLoadEvent::subscribe([](LegacyApi::Event::MapLoadEvent &event) {
            if(event.time == LegacyApi::Event::EVENT_TIME_BEFORE) {
                dispatch_map_load_event(event.context.name.c_str());
            }
        });
    }

    void dispatch_map_loaded_event(const char *map_name) {
        MapLoadedEvent event(map_name);
        event.dispatch();
    }

    const std::string &MapLoadedEvent::map_name() const noexcept {
        return m_map_name;
    }

    template<>
    void EventHandler<MapLoadedEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        LegacyApi::Event::MapLoadEvent::subscribe([](auto &event) {
            if(event.time == LegacyApi::Event::EVENT_TIME_AFTER) {
                dispatch_map_loaded_event(event.context.name.c_str());
            }
        });
    }
}
