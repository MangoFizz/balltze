// SPDX-License-Identifier: GPL-3.0-only

#include <cstring>
#include <stdexcept>
#include "event.hpp"

using namespace Balltze::Events;

namespace Balltze::Lua::Api::V2 {
    const char *event_priority_to_string(EventPriority priority) {
        switch(priority) {
            case EVENT_PRIORITY_LOWEST:
                return "lowest";
            case EVENT_PRIORITY_DEFAULT:
                return "default";
            case EVENT_PRIORITY_ABOVE_DEFAULT:
                return "above_default";
            case EVENT_PRIORITY_HIGHEST:
                return "highest";
            default:
                return "unknown";
        }
    }

    EventPriority event_priority_from_string(const char *priority) {
        if(strcmp(priority, "lowest") == 0) {
            return EVENT_PRIORITY_LOWEST;
        }
        else if(strcmp(priority, "default") == 0) {
            return EVENT_PRIORITY_DEFAULT;
        }
        else if(strcmp(priority, "above_default") == 0) {
            return EVENT_PRIORITY_ABOVE_DEFAULT;
        }
        else if(strcmp(priority, "highest") == 0) {
            return EVENT_PRIORITY_HIGHEST;
        }
        else {
            throw std::invalid_argument("Invalid event priority.");
        }
    }
}
