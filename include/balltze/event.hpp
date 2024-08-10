// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__EVENT_HPP
#define BALLTZE_API__EVENT_HPP

#include <functional>
#include <stdexcept>
#include "api.hpp"

namespace Balltze::Event {
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
        BALLTZE_API static void init();
        BALLTZE_API static std::size_t add_listener(EventCallback<T> callback, EventPriority priority = EVENT_PRIORITY_DEFAULT);
        BALLTZE_API static std::size_t add_listener_const(ConstEventCallback<T> callback, EventPriority priority = EVENT_PRIORITY_DEFAULT);
        BALLTZE_API static void remove_listener(std::size_t handle);
        BALLTZE_API static void dispatch(T &event);
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

    std::string event_priority_to_string(EventPriority priority);
    EventPriority event_priority_from_string(const std::string &priority);
    std::string event_time_to_string(EventTime time);
    EventTime event_time_from_string(const std::string &time);
}

#include "events/d3d9.hpp"
#include "events/frame.hpp"
#include "events/tick.hpp"
#include "events/game_input.hpp"
#include "events/hud_hold_to_action_message.hpp"
#include "events/map_load.hpp"
#include "events/netgame.hpp"
#include "events/sound_playback.hpp"
#include "events/camera.hpp"
#include "events/server_connect.hpp"
#include "events/object_damage.hpp"
#include "events/rcon_message.hpp"
#include "events/render.hpp"
#include "events/widget.hpp"

#endif

