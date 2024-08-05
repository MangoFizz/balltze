// SPDX-License-Identifier: GPL-3.0-only

#include <optional>
#include <list>
#include <stdexcept>
#include <balltze/event.hpp>
#include "console_command.hpp"

namespace Balltze::Event {
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

    template class EventHandler<TickEvent>;
    template class EventHandler<HudHoldForActionMessageEvent>;
    template class EventHandler<KeyboardInputEvent>;
    template class EventHandler<GameInputEvent>;
    template class EventHandler<MapFileLoadEvent>;
    template class EventHandler<SoundPlaybackEvent>;
    template class EventHandler<D3D9BeginSceneEvent>;
    template class EventHandler<D3D9EndSceneEvent>;
    template class EventHandler<D3D9DeviceResetEvent>;
    template class EventHandler<FrameEvent>;
    template class EventHandler<MapFileDataReadEvent>;
    template class EventHandler<ConsoleCommandEvent>;
    template class EventHandler<CameraEvent>;
    template class EventHandler<ServerConnectEvent>;
    template class EventHandler<ObjectDamageEvent>;
    template class EventHandler<RconMessageEvent>;
    template class EventHandler<MapLoadEvent>;
    template class EventHandler<UIRenderEvent>;
    template class EventHandler<HUDRenderEvent>;
    template class EventHandler<PostCarnageReportRenderEvent>;
    template class EventHandler<HUDElementBitmapRenderEvent>;
    template class EventHandler<UIWidgetBackgroundRenderEvent>;
    template class EventHandler<NavPointsRenderEvent>;
    template class EventHandler<NetworkGameChatMessageEvent>;
    template class EventHandler<UIWidgetCreateEvent>;
    template class EventHandler<UIWidgetBackEvent>;
    template class EventHandler<UIWidgetFocusEvent>;
    template class EventHandler<UIWidgetAcceptEvent>;
    template class EventHandler<UIWidgetSoundEvent>;
    template class EventHandler<UIWidgetListTabEvent>;
    template class EventHandler<UIWidgetMouseButtonPressEvent>;
    template class EventHandler<NetworkGameMultiplayerSoundEvent>;
    template class EventHandler<NetworkGameHudMessageEvent>;

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
            EventHandler<CameraEvent>::init();
            EventHandler<ServerConnectEvent>::init();
            EventHandler<ObjectDamageEvent>::init();
            EventHandler<RconMessageEvent>::init();
            EventHandler<MapLoadEvent>::init();
            EventHandler<UIRenderEvent>::init();
            EventHandler<HUDRenderEvent>::init();
            EventHandler<PostCarnageReportRenderEvent>::init();
            EventHandler<HUDElementBitmapRenderEvent>::init();
            EventHandler<UIWidgetBackgroundRenderEvent>::init();
            EventHandler<NavPointsRenderEvent>::init();
            EventHandler<UIWidgetCreateEvent>::init();
            EventHandler<UIWidgetBackEvent>::init();
            EventHandler<UIWidgetFocusEvent>::init();
            EventHandler<UIWidgetAcceptEvent>::init();
            EventHandler<UIWidgetSoundEvent>::init();
            EventHandler<UIWidgetListTabEvent>::init();
            EventHandler<UIWidgetMouseButtonPressEvent>::init();
            
            first_tick_listener = TickEvent::subscribe_const(+[](const TickEvent &event) {
                EventHandler<D3D9BeginSceneEvent>::init();
                EventHandler<D3D9EndSceneEvent>::init();
                EventHandler<D3D9DeviceResetEvent>::init();
                EventHandler<NetworkGameChatMessageEvent>::init();
                EventHandler<NetworkGameMultiplayerSoundEvent>::init();
                EventHandler<NetworkGameHudMessageEvent>::init();
                EventHandler<KeyboardInputEvent>::init();
                first_tick_listener.remove();
            });
        }
        catch(std::runtime_error) {
            throw;
        }
    }

    std::string event_priority_to_string(EventPriority priority) {
        switch(priority) {
            case EVENT_PRIORITY_LOWEST:
                return "lowest";
            case EVENT_PRIORITY_DEFAULT:
                return "default";
            case EVENT_PRIORITY_ABOVE_DEFAULT:
                return "above_default";
            case EVENT_PRIORITY_HIGHEST:
                return "highest";
            default:
                return "unknown";
        }
    }

    EventPriority event_priority_from_string(const std::string &priority) {
        if(priority == "lowest") {
            return EVENT_PRIORITY_LOWEST;
        }
        else if(priority == "default") {
            return EVENT_PRIORITY_DEFAULT;
        }
        else if(priority == "above_default") {
            return EVENT_PRIORITY_ABOVE_DEFAULT;
        }
        else if(priority == "highest") {
            return EVENT_PRIORITY_HIGHEST;
        }
        else {
            throw std::invalid_argument("Invalid event priority.");
        }
    }

    std::string event_time_to_string(EventTime time) {
        switch(time) {
            case EVENT_TIME_BEFORE:
                return "before";
            case EVENT_TIME_AFTER:
                return "after";
            default:
                return "unknown";
        }
    }

    EventTime event_time_from_string(const std::string &time) {
        if(time == "before") {
            return EVENT_TIME_BEFORE;
        }
        else if(time == "after") {
            return EVENT_TIME_AFTER;
        }
        else {
            throw std::runtime_error("Invalid event time.");
        }
    }
}