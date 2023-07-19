// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__EVENTS__MAP_FILE_DATA_READ_HPP
#define BALLTZE_API__EVENTS__MAP_FILE_DATA_READ_HPP

#include <windows.h>
#include "../event.hpp"

namespace Balltze::Event {
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
