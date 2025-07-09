// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__EVENTS__EVENTS_HPP
#define BALLTZE__EVENTS__EVENTS_HPP

#include <list>
#include <stdexcept>
#include <balltze/events.hpp>

namespace Balltze::Events {
    template<typename T>
    struct EventListener {
        inline static std::size_t next_handle = 0;
        
        std::size_t handle;
        EventPriority priority = EVENT_PRIORITY_DEFAULT;
        EventCallback<T> listener_callback;

        bool remove_on_next_dispatch = false;

        EventListener() {
            handle = next_handle++;
        }

        void operator()(T &event) {
            if(listener_callback) {
                listener_callback(event);
            }
            else {
                throw std::runtime_error("Event listener is not initialized");
            }
        }
    };

    template<typename T>
    static std::list<EventListener<T>> listeners;

    template<typename T>
    std::size_t EventHandler<T>::add_listener(EventCallback<T> callback, EventPriority priority) {
        auto &listener = listeners<T>.emplace_back();
        listener.priority = priority;
        listener.listener_callback = callback;
        return listener.handle;
    }

    template<typename T>
    void EventHandler<T>::remove_listener(std::size_t handle) {
        for(auto it = listeners<T>.begin(); it != listeners<T>.end(); ++it) {
            if(it->handle == handle) {
                it->remove_on_next_dispatch = true;
                break;
            }
        }
    }

    template<typename T>
    void EventHandler<T>::dispatch(T &event) {
        auto call_by_priority = [&](EventPriority priority) {
            auto listener = listeners<T>.begin();
            while(listener != listeners<T>.end()) {
                if(listener->priority == priority) {
                    if(listener->remove_on_next_dispatch) {
                        listener = listeners<T>.erase(listener);
                        continue;
                    }
                    (*listener)(event);
                }
                listener++;
            }
        };

        call_by_priority(EVENT_PRIORITY_HIGHEST);
        call_by_priority(EVENT_PRIORITY_ABOVE_DEFAULT);
        call_by_priority(EVENT_PRIORITY_DEFAULT);
        call_by_priority(EVENT_PRIORITY_LOWEST);
    }

    template class EventHandler<FrameEvent>;
    template class EventHandler<TickEvent>;

    /**
     * Initializes the event handlers for the Balltze events.
     */
    void set_up_events_handlers();
} 

#endif
