// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__EVENTS__MAP_LOADING_HPP
#define BALLTZE_API__EVENTS__MAP_LOADING_HPP

#include "../event.hpp"

namespace Balltze::Event {
    struct MapLoadEventArgs {
        std::string name;
    };

    class MapLoadEvent : public EventData<MapLoadEvent> {
    public:
        MapLoadEventArgs args;

        bool cancellable() const {
            return false;
        }

        MapLoadEvent(EventTime time, MapLoadEventArgs args) : EventData(time), args(args) {}
    };

    struct MapFileLoadEventArgs {
        std::string map_name;
        std::string map_path;
    };

    class MapFileLoadEvent : public EventData<MapFileLoadEvent> {
    public:
        MapFileLoadEventArgs args;

        bool cancellable() const {
            return false;
        }

        MapFileLoadEvent(EventTime time, MapFileLoadEventArgs args) : EventData(time), args(args) {}
    };

    struct MapFileDataReadEventArgs {
        HANDLE file_handle; 
        std::byte *output_buffer;
        std::size_t size;
        LPOVERLAPPED overlapped;
    };

    class MapFileDataReadEvent : public EventData<MapFileDataReadEvent> {
    public:
        MapFileDataReadEventArgs args;

        bool cancellable() const {
            return false;
        }

        MapFileDataReadEvent(EventTime time, MapFileDataReadEventArgs args) : EventData(time), args(args) {}
    };
}

#endif
