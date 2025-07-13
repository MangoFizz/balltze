// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__EVENTS_HPP
#define BALLTZE_API__EVENTS_HPP

#include <functional>
#include <stdexcept>
#include "api.hpp"

namespace Balltze::Events {
    enum EventPriority {
        EVENT_PRIORITY_LOWEST,
        EVENT_PRIORITY_DEFAULT,
        EVENT_PRIORITY_ABOVE_DEFAULT,
        EVENT_PRIORITY_HIGHEST
    };

    template<typename T>
    using EventCallback = std::function<void(T &)>;

    template<typename T>
    class EventHandler {
    public:
        /**
         * Initialize the event handler.
         * This function should be called once at the start of the execution to set up the event
         */
        BALLTZE_API static void init();

        /**
         * Add a listener for the event.
         * 
         * @param callback The callback function to be called when the event is dispatched.
         * @param priority The priority of the listener. Higher priority listeners are called first.
         * @return A handle to the listener, which can be used to remove it later.
         */
        BALLTZE_API static std::size_t add_listener(EventCallback<T> callback, EventPriority priority = EVENT_PRIORITY_DEFAULT);

        /**
         * Remove a listener for the event.
         * 
         * @param handle The handle of the listener to be removed.
         */
        BALLTZE_API static void remove_listener(std::size_t handle);

        /**
         * Dispatch the event to all listeners.
         * 
         * @param event The event data to be dispatched.
         */
        BALLTZE_API static void dispatch(T &event);
    };

    template<typename T>
    class EventListenerHandle {
    private:
        std::size_t m_handle;
        bool deleted = false;

    public:
        /**
         * Default constructor for EventListenerHandle.
         */
        EventListenerHandle() = default;

        /**
         * Constructor for EventListenerHandle.
         * 
         * @param handle The handle of the listener.
         */
        EventListenerHandle(std::size_t handle) : m_handle(handle) {}

        /**
         * Removes the listener associated with this handle.
         */
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
        
    protected:
        bool m_cancelable = false;

        /**
         * Constructor for EventData.
         */
        EventData(bool cancellable) : m_cancelable(cancellable) {}

    public:
        using ListenerHandle = EventListenerHandle<T>;

        /**
         * Copy constructor for EventData.
         * This constructor is deleted to prevent copying of event data.
         */
        EventData(EventData const &) = delete;

        /**
         * Dispatch the event to all listeners.
         */
        inline void dispatch() {
            EventHandler<T>::dispatch(*(T *)this);
        }

        /**
         * Get whether the event is cancellable.
         * 
         * @return True if the event is cancellable, false otherwise.
         */
        bool cancellable() const;

        /**
         * Cancel the event.
         * 
         * @throws std::runtime_error If the event is not cancellable.
         */
        inline void cancel() {
            if(cancellable()) {
                m_cancelled = true;
            }
            else {
                throw std::runtime_error("Event is not cancellable");
            }
        }

        /**
         * Check if the event has been cancelled.
         * 
         * @return True if the event has been cancelled, false otherwise.
         */
        inline bool cancelled() const {
            return m_cancelled;
        }

        /**
         * Subscribe a listener to the event.
         * 
         * @param callback The callback function to be called when the event is dispatched.
         * @param priority The priority of the listener. Higher priority listeners are called first.
         * @return A handle to the listener, which can be used to remove it later.
         */
        static ListenerHandle subscribe(EventCallback<T> callback, EventPriority priority = EVENT_PRIORITY_DEFAULT) {
            return ListenerHandle(EventHandler<T>::add_listener(callback, priority));
        }

        /**
         * Unsubscribe a listener from the event.
         * 
         * @param listener The handle of the listener to be removed.
         */
        static void unsubscribe(ListenerHandle listener) {
            listener.remove();
        }
    };
}

#endif

#include "events/frame.hpp"
#include "events/tick.hpp"
#include "events/map.hpp"
