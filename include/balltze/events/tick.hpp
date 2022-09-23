// SPDX-License-Identifier: GPL-3.0-only

#ifndef HARMONY__EVENTS__TICK_HPP
#define HARMONY__EVENTS__TICK_HPP

#include "event.hpp"

namespace Balltze {
    /**
     * Add a tick event.
     * @param function  Function to be executed on every game tick
     * @param priority  This is the priority that defines the order in which the event will be called
     */
    void add_tick_event(event_no_args_t event_function, EventPriority priority = EVENT_PRIORITY_DEFAULT);

    /**
     * Remove a tick event.
     * @param function  Function of the event to be removed
     */
    void remove_tick_event(event_no_args_t event_function);

    /**
     * Set up tick event
     */
    void set_up_tick_event();
}

#endif
