// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__EVENTS__MAP_FILE_LOAD_HPP
#define BALLTZE_API__EVENTS__MAP_FILE_LOAD_HPP

#include "../event.hpp"

namespace Balltze::Event {
    struct MapFileLoadEventArgs {
        std::string map_name;
        std::string map_path;
    };

    class MapFileLoadEvent : public EventData<MapFileLoadEvent> {
    public:
        MapFileLoadEventArgs args;

        static bool cancellable() {
            return false;
        }

        MapFileLoadEvent(EventTime time, MapFileLoadEventArgs args) : EventData(time), args(args) {}
    };
}

#endif
