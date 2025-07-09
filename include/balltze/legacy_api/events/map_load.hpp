// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_LEGACY_API__EVENTS__MAP_LOADING_HPP
#define BALLTZE_LEGACY_API__EVENTS__MAP_LOADING_HPP

#include "../event.hpp"

namespace Balltze::LegacyApi::Event {
    struct MapLoadEventContext {
        const std::string name;

        MapLoadEventContext(const std::string name) : name(name) {}
    };

    class MapLoadEvent : public EventData<MapLoadEvent> {
    public:
        MapLoadEventContext context;

        bool cancellable() const {
            return false;
        }

        MapLoadEvent(EventTime time, MapLoadEventContext context) : EventData(time), context(context) {}
    };

    struct MapFileLoadEventContext {
        const std::string map_name;
        const std::string map_path;

        MapFileLoadEventContext(const std::string map_name, const std::string map_path) : map_name(map_name), map_path(map_path) {}
    };

    class MapFileLoadEvent : public EventData<MapFileLoadEvent> {
    public:
        MapFileLoadEventContext context;

        bool cancellable() const {
            return false;
        }

        MapFileLoadEvent(EventTime time, MapFileLoadEventContext context) : EventData(time), context(context) {}
    };

    struct MapFileDataReadEventContext {
        HANDLE file_handle; 
        std::byte *output_buffer;
        std::size_t size;
        LPOVERLAPPED overlapped;
    };

    class MapFileDataReadEvent : public EventData<MapFileDataReadEvent> {
    public:
        MapFileDataReadEventContext context;

        bool cancellable() const {
            return false;
        }

        MapFileDataReadEvent(EventTime time, MapFileDataReadEventContext context) : EventData(time), context(context) {}
    };
}

#endif
