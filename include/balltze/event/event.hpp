// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__EVENT__EVENT_HPP
#define BALLTZE__EVENT__EVENT_HPP

#include <functional>

namespace Balltze {
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

    template <typename T>
    using NonCancellableEventDelegate = std::function<void(T &)>;

    template <typename T>
    using NonCancellableConstEventDelegate = std::function<void(T const &)>;

    template <typename T>
    using CancellableEventDelegate = std::function<bool(T &)>;

    template <typename T>
    using CancellableConstEventDelegate = std::function<bool(T const &)>;

    template <typename T>
    class EventHandler {
    public:
        static void init();
        static std::size_t add_listener(NonCancellableEventDelegate<T> callback, EventPriority priority = EVENT_PRIORITY_DEFAULT);
        static std::size_t add_listener_const(NonCancellableConstEventDelegate<T> callback, EventPriority priority = EVENT_PRIORITY_DEFAULT);
        static std::size_t add_listener(CancellableEventDelegate<T> callback, EventPriority priority = EVENT_PRIORITY_DEFAULT);
        static std::size_t add_listener_const(CancellableConstEventDelegate<T> callback, EventPriority priority = EVENT_PRIORITY_DEFAULT);
        static void remove_listener(std::size_t handle);
        static void dispatch(T &event);
    };

    template <typename T>
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

    template <typename T>
    class EventData {
    private:
        bool m_cancelled = false;

    public:
        const EventTime time;

        EventData(EventTime time, bool cancellable) : time(time) {}

        inline void dispatch() {
            EventHandler<T>::dispatch(*(T *)this);
        }

        inline void cancel() {
            m_cancelled = true;
        }

        inline bool cancelled() {
            return m_cancelled;
        }

        static EventListenerHandle<T> subscribe(NonCancellableEventDelegate<T> callback, EventPriority priority = EVENT_PRIORITY_DEFAULT) {
            return EventListenerHandle<T>(EventHandler<T>::add_listener(callback, priority));
        }

        static EventListenerHandle<T> subscribe_const(NonCancellableConstEventDelegate<T> callback, EventPriority priority = EVENT_PRIORITY_DEFAULT) {
            return EventListenerHandle<T>(EventHandler<T>::add_listener_const(callback, priority));
        }

        static EventListenerHandle<T> subscribe(CancellableEventDelegate<T> callback, EventPriority priority = EVENT_PRIORITY_DEFAULT) {
            return EventListenerHandle<T>(EventHandler<T>::add_listener(callback, priority));
        }

        static EventListenerHandle<T> subscribe_const(CancellableConstEventDelegate<T> callback, EventPriority priority = EVENT_PRIORITY_DEFAULT) {
            return EventListenerHandle<T>(EventHandler<T>::add_listener_const(callback, priority));
        }

        static void unsubscribe(EventListenerHandle<T> listener) {
            listener.remove();
        }
    };
}

#endif

#include "tick_event.hpp"
