// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__EVENTS__MAP_LOAD_HPP
#define BALLTZE_API__EVENTS__MAP_LOAD_HPP

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
}

#endif
