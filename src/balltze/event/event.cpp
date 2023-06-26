// SPDX-License-Identifier: GPL-3.0-only

#include <optional>
#include <list>
#include <any>
#include <stdexcept>
#include <balltze/event.hpp>

namespace Balltze::Event {
    template<typename T>
    struct EventListener {
        inline static std::size_t next_handle = 0;
        
        std::size_t handle;
        bool constant = false;
        EventPriority priority = EVENT_PRIORITY_DEFAULT;
        std::optional<NonCancellableEventDelegate<T>> non_cancellable_callback;
        std::optional<CancellableEventDelegate<T>> cancellable_callback;
        std::optional<NonCancellableConstEventDelegate<T>> non_cancellable_const_callback;
        std::optional<CancellableConstEventDelegate<T>> cancellable_const_callback;

        EventListener() {
            handle = next_handle++;
        }

        void operator()(T &event) {
            if(T::cancellable()) {
                if(constant) {
                    if(cancellable_const_callback.has_value()) {
                        if(!cancellable_const_callback.value()(event)) {
                            event.cancel();
                        }
                    }
                } 
                else {
                    if(cancellable_callback.has_value()) {
                        if(!cancellable_callback.value()(event)) {
                            event.cancel();
                        }
                    }
                }
            } 
            else {
                if(constant) {
                    if(non_cancellable_const_callback.has_value()) {
                        non_cancellable_const_callback.value()(event);
                    }
                } 
                else {
                    if(non_cancellable_callback.has_value()) {
                        non_cancellable_callback.value()(event);
                    }
                }
            }
        }
    };

    template<typename T>
    static std::list<EventListener<T>> listeners;

    static std::any listeners_iterator_aux;

    template<typename T>
    std::size_t EventHandler<T>::add_listener(NonCancellableEventDelegate<T> callback, EventPriority priority) {
        if(T::cancellable()) {
            throw std::invalid_argument("Event is cancellable, use CancellableEventDelegate instead");
        }
        auto &listener = listeners<T>.emplace_back();
        listener.constant = false;
        listener.priority = priority;
        listener.non_cancellable_callback = callback;
        return listener.handle;
    }

    template<typename T>
    std::size_t EventHandler<T>::add_listener_const(NonCancellableConstEventDelegate<T> callback, EventPriority priority) {
        if(T::cancellable()) {
            throw std::invalid_argument("Event is cancellable, use CancellableEventDelegate instead");
        }
        auto &listener = listeners<T>.emplace_back();
        listener.constant = true;
        listener.priority = priority;
        listener.non_cancellable_const_callback = callback;
        return listener.handle;
    }
    
    template<typename T>
    std::size_t EventHandler<T>::add_listener(CancellableEventDelegate<T> callback, EventPriority priority) {
        if(!T::cancellable()) {
            throw std::invalid_argument("Event is not cancellable, use NonCancellableEventDelegate instead");
        }
        auto &listener = listeners<T>.emplace_back();
        listener.constant = false;
        listener.priority = priority;
        listener.cancellable_callback = callback;
        return listener.handle;
    }

    template<typename T>
    std::size_t EventHandler<T>::add_listener_const(CancellableConstEventDelegate<T> callback, EventPriority priority) {
        if(!T::cancellable()) {
            throw std::invalid_argument("Event is not cancellable, use NonCancellableEventDelegate instead");
        }
        auto &listener = listeners<T>.emplace_back();
        listener.constant = true;
        listener.priority = priority;
        listener.cancellable_const_callback = callback;
        return listener.handle;
    }

    template<typename T>
    void EventHandler<T>::remove_listener(std::size_t handle) {
        for(auto it = listeners<T>.begin(); it != listeners<T>.end(); ++it) {
            if(it->handle == handle) {
                listeners_iterator_aux = listeners<T>.erase(it);
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
                    (*listener)(event);
                }
                if(listeners_iterator_aux.has_value()) {
                    try {
                        listener = std::any_cast<decltype(listener)>(listeners_iterator_aux);
                        listeners_iterator_aux.reset();
                    }
                    catch(std::bad_any_cast) {
                        throw std::runtime_error("Event listener iterator was modified");
                    }
                } 
                else {
                    listener++;
                }
            }
        };

        call_by_priority(EVENT_PRIORITY_HIGHEST);
        call_by_priority(EVENT_PRIORITY_ABOVE_DEFAULT);
        call_by_priority(EVENT_PRIORITY_DEFAULT);
        call_by_priority(EVENT_PRIORITY_LOWEST);
    }

    template class EventHandler<TickEvent>;
    template class EventHandler<HudHoldForActionMessageEvent>;
    template class EventHandler<GameInputEvent>;
    template class EventHandler<MapFileLoadEvent>;
    template class EventHandler<SoundPlaybackEvent>;

    void set_up_events() {
        try {
            EventHandler<TickEvent>::init();
            EventHandler<HudHoldForActionMessageEvent>::init();
            EventHandler<GameInputEvent>::init();
            EventHandler<MapFileLoadEvent>::init();
            EventHandler<SoundPlaybackEvent>::init();
        }
        catch(std::runtime_error) {
            throw;
        }
    }
}