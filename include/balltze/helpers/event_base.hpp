// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__HELPERS__EVENT_BASE_HPP
#define BALLTZE_API__HELPERS__EVENT_BASE_HPP

#include <functional>
#include <stdexcept>

enum EventPriority {
    EVENT_PRIORITY_LOWEST,
    EVENT_PRIORITY_DEFAULT,
    EVENT_PRIORITY_ABOVE_DEFAULT,
    EVENT_PRIORITY_HIGHEST
};

enum EventTime {
    EVENT_TIME_BEFORE,
    EVENT_TIME_AFTER
};

template<typename T>
using EventCallback = std::function<void(T &)>;

template<typename T>
using ConstEventCallback = std::function<void(T const &)>;

template<typename T>
class EventHandler {
public:
    static void init();
    static std::size_t add_listener(EventCallback<T> callback, EventPriority priority = EVENT_PRIORITY_DEFAULT);
    static std::size_t add_listener_const(ConstEventCallback<T> callback, EventPriority priority = EVENT_PRIORITY_DEFAULT);
    static void remove_listener(std::size_t handle);
    static void dispatch(T &event);
};

template<typename T>
class EventListenerHandle {
private:
    std::size_t m_handle;
    bool deleted = false;

public:
    EventListenerHandle() = default;
    EventListenerHandle(std::size_t handle) : m_handle(handle) {}

    void remove() { 
        if(!deleted) {
            deleted = true;
            EventHandler<T>::remove_listener(m_handle);
        }
    }
};

template<typename T>
class EventData {
private:
    bool m_cancelled = false;

public:
    using ListenerHandle = EventListenerHandle<T>;
    
    const EventTime time;

    EventData(EventTime time) : time(time) {}
    EventData(EventData const &) = delete;

    inline void dispatch() {
        EventHandler<T>::dispatch(*(T *)this);
    }

    virtual bool cancellable() const = 0;

    inline void cancel() {
        if(cancellable()) {
            if(this->time == EVENT_TIME_AFTER) {
                throw std::runtime_error("Cannot cancel an event after is has been occurred");
            }
            m_cancelled = true;
        }
        else {
            throw std::runtime_error("Event is not cancellable");
        }
    }

    inline bool cancelled() const {
        return m_cancelled;
    }

    static ListenerHandle subscribe(EventCallback<T> callback, EventPriority priority = EVENT_PRIORITY_DEFAULT) {
        return ListenerHandle(EventHandler<T>::add_listener(callback, priority));
    }

    static ListenerHandle subscribe_const(ConstEventCallback<T> callback, EventPriority priority = EVENT_PRIORITY_DEFAULT) {
        return ListenerHandle(EventHandler<T>::add_listener_const(callback, priority));
    }

    static void unsubscribe(ListenerHandle listener) {
        listener.remove();
    }
};

#endif
