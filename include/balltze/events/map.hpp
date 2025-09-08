// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__EVENTS__MAP_HPP
#define BALLTZE_API__EVENTS__MAP_HPP

#include "../events.hpp"

namespace Balltze::Events {
    class MapLoadEvent : public EventData<MapLoadEvent> {
    public:
        /**
         * Get the name of the map being loaded.
         * 
         * @return The name of the map.
         */
        const std::string &map_name() const noexcept;

        /**
         * Constructor for MapLoadEvent.
         * This event is dispatched before a map is loaded.
         */
        MapLoadEvent(const std::string &map_name) : EventData(false), m_map_name(map_name) {}
    private:
        std::string m_map_name;
    };

    class MapLoadedEvent : public EventData<MapLoadedEvent> {
    public:
        /**
         * Get the name of the map that has been loaded.
         * 
         * @return The name of the loaded map.
         */
        const std::string &map_name() const noexcept;

        /**
         * Constructor for MapLoadedEvent.
         * This event is dispatched after a map has been loaded.
         */
        MapLoadedEvent(const std::string &map_name) : EventData(false), m_map_name(map_name) {}

    private:
        std::string m_map_name;
    };
}

#endif
