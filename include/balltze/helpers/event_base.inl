// SPDX-License-Identifier: GPL-3.0-only

#include <optional>
#include <list>
#include <stdexcept>
#include "event_base.hpp"

template<typename T>
struct EventListener {
    inline static std::size_t next_handle = 0;
    
    std::size_t handle;
    bool constant = false;
    EventPriority priority = EVENT_PRIORITY_DEFAULT;
    std::optional<EventCallback<T>> ref_callback;
    std::optional<ConstEventCallback<T>> const_callback;

    bool remove_on_next_dispatch = false;

    EventListener() {
        handle = next_handle++;
    }

    void operator()(T &event) {
        if(constant && const_callback) {
            const_callback.value()(event);
        }
        else if(ref_callback) {
            ref_callback.value()(event);
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
    listener.constant = false;
    listener.priority = priority;
    listener.ref_callback = callback;
    return listener.handle;
}

template<typename T>
std::size_t EventHandler<T>::add_listener_const(ConstEventCallback<T> callback, EventPriority priority) {
    auto &listener = listeners<T>.emplace_back();
    listener.constant = true;
    listener.priority = priority;
    listener.const_callback = callback;
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
