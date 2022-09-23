// SPDX-License-Identifier: GPL-3.0-only

#ifndef CHIMERA_EVENT_HPP
#define CHIMERA_EVENT_HPP

#include <vector>

namespace Balltze {
    enum EventPriority {
        /** These events are called before the default priority events are called. */
        EVENT_PRIORITY_BEFORE,

        /** This is the default priority where most events are called. */
        EVENT_PRIORITY_DEFAULT,

        /** These events are called after the default priority. This priority gets the last say. */
        EVENT_PRIORITY_AFTER,

        /** These events are called last. This is used for monitoring values. Do not change any of the parameters in this. */
        EVENT_PRIORITY_FINAL
    };

    /** This is a function typename that has no arguments and returns nothing. */
    typedef void (*event_no_args_t)();

    /** This struct is used for events and uses a priority and function pointer. */
    template <class T>
    class Event {
    public:
        /** This is the function pointer used by the event. */
        T function = nullptr;

        /** This is the priority of the event. */
        EventPriority priority = EVENT_PRIORITY_DEFAULT;

        /** 
         * Event default constructor
         */
        Event() {}

        /**
         * Event constructor
         * @param function  Event function
         * @param priority  This is the priority that defines the order in which the event will be called
         */
        Event(T function, EventPriority priority = EVENT_PRIORITY_DEFAULT) : function(function), priority(priority) {}
    };

    /**
     * This function calls events in order.
     * Functions in the same priority are called based on their order in the events vector.
     */
    template<typename T, typename ... Args>
    static inline void call_in_order(std::vector<Event<T>> events, Args&& ... args) {
        auto count = events.size();
        for(typename std::size_t i = 0; i < count; i++) {
            if(events[i].priority == EVENT_PRIORITY_BEFORE) events[i].function(std::forward<Args>(args) ...);
        }
        for(typename std::vector<Event<T>>::size_type i = 0; i < count; i++) {
            if(events[i].priority == EVENT_PRIORITY_DEFAULT) events[i].function(std::forward<Args>(args) ...);
        }
        for(typename std::vector<Event<T>>::size_type i = 0; i < count; i++) {
            if(events[i].priority == EVENT_PRIORITY_AFTER) events[i].function(std::forward<Args>(args) ...);
        }
        for(typename std::vector<Event<T>>::size_type i = 0; i < count; i++) {
            if(events[i].priority == EVENT_PRIORITY_FINAL) events[i].function(std::forward<Args>(args) ...);
        }
    }

    /**
     * This function calls events in order, but the event can be denied by any function. Denying will prevent other events from firing.
     * Functions in the same priority are called based on their order in the events vector.
     */
    template<typename T, typename ... Args>
    static inline void call_in_order_allow(std::vector<Event<T>> events, bool &allow, Args&& ... args) {
        auto count = events.size();
        for(typename std::vector<Event<T>>::size_type i = 0; i < count && allow; i++) {
            if(events[i].priority == EVENT_PRIORITY_BEFORE) allow = events[i].function(std::forward<Args>(args) ...);
        }
        for(typename std::vector<Event<T>>::size_type i = 0; i < count && allow; i++) {
            if(events[i].priority == EVENT_PRIORITY_DEFAULT) allow = events[i].function(std::forward<Args>(args) ...);
        }
        for(typename std::vector<Event<T>>::size_type i = 0; i < count && allow; i++) {
            if(events[i].priority == EVENT_PRIORITY_AFTER) allow = events[i].function(std::forward<Args>(args) ...);
        }
        for(typename std::vector<Event<T>>::size_type i = 0; i < count && allow; i++) {
            if(events[i].priority == EVENT_PRIORITY_FINAL) allow = events[i].function(std::forward<Args>(args) ...);
        }
    }
}

#endif
