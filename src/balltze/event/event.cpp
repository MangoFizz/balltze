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
        std::optional<EventCallback<T>> ref_callback;
        std::optional<ConstEventCallback<T>> const_callback;

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

    static std::any listeners_iterator_aux;

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
    template class EventHandler<D3D9EndSceneEvent>;
    template class EventHandler<D3D9DeviceResetEvent>;
    template class EventHandler<FrameEvent>;
    template class EventHandler<MapFileDataReadEvent>;
    template class EventHandler<ConsoleCommandEvent>;

    static EventListenerHandle<TickEvent> first_tick_listener;

    void set_up_events() {
        try {
            EventHandler<TickEvent>::init();
            EventHandler<HudHoldForActionMessageEvent>::init();
            EventHandler<GameInputEvent>::init();
            EventHandler<MapFileLoadEvent>::init();
            EventHandler<SoundPlaybackEvent>::init();
            EventHandler<FrameEvent>::init();
            EventHandler<MapFileDataReadEvent>::init();
            EventHandler<ConsoleCommandEvent>::init();
            
            first_tick_listener = TickEvent::subscribe_const(+[](const TickEvent &event) {
                EventHandler<D3D9EndSceneEvent>::init();
                EventHandler<D3D9DeviceResetEvent>::init();
                first_tick_listener.remove();
            });
        }
        catch(std::runtime_error) {
            throw;
        }
    }
}