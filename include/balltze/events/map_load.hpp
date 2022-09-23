// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__EVENTS__MAP_LOAD_HPP
#define BALLTZE__EVENTS__MAP_LOAD_HPP

#include "event.hpp"

namespace Balltze {
    /**
     * Add a map load event.
     * @param function  Function to execute when a map is loaded
     * @param priority  This is the priority that defines the order in which the event will be called
     */
    void add_map_load_event(event_no_args_t event_function, EventPriority priority = EVENT_PRIORITY_DEFAULT) noexcept;

    /**
     * Remove a map load event.
     * @param function  Function of the event to remove
     */
    void remove_map_load_event(event_no_args_t event_function) noexcept;

    /**
     * Set up map load event
     */
    void set_up_map_load_event();
}

#endif
