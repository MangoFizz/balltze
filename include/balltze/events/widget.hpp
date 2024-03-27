// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__EVENTS__WIDGET_HPP
#define BALLTZE_API__EVENTS__WIDGET_HPP

#include <balltze/engine/user_interface.hpp>
#include "../event.hpp"

namespace Balltze::Event {
    struct UIWidgetEventArgs {
        Engine::Widget const *widget;

        UIWidgetEventArgs(Engine::Widget const *widget) : widget(widget) {}
    };

    struct UIWidgetCreateEventArgs : UIWidgetEventArgs {
        const Engine::TagHandle definition_tag_handle;
        const bool is_root_widget;
        Engine::Widget const *parent_widget;

        UIWidgetCreateEventArgs(Engine::Widget const *widget, Engine::TagHandle definition_tag_handle, bool is_root_widget, Engine::Widget const *parent_widget) : UIWidgetEventArgs(widget), definition_tag_handle(definition_tag_handle), is_root_widget(is_root_widget), parent_widget(parent_widget) {}
    };

    class UIWidgetCreateEvent : public EventData<UIWidgetCreateEvent> {
    public:
        UIWidgetCreateEventArgs args;

        bool cancellable() const {
            return true;
        }
        void cancel();

        UIWidgetCreateEvent(EventTime time, UIWidgetCreateEventArgs args) : EventData(time), args(args) {}
    };

    class UIWidgetBackEvent : public EventData<UIWidgetBackEvent> {
    public:
        UIWidgetEventArgs args;

        bool cancellable() const {
            return true;
        }

        UIWidgetBackEvent(EventTime time, UIWidgetEventArgs args) : EventData(time), args(args) {}
    };

    class UIWidgetFocusEvent : public EventData<UIWidgetFocusEvent> {
    public:
        UIWidgetEventArgs args;

        bool cancellable() const {
            return true;
        }

        UIWidgetFocusEvent(EventTime time, UIWidgetEventArgs args) : EventData(time), args(args) {}
    };

    class UIWidgetAcceptEvent : public EventData<UIWidgetAcceptEvent> {
    public:
        UIWidgetEventArgs args;

        bool cancellable() const {
            return true;
        }

        UIWidgetAcceptEvent(EventTime time, UIWidgetEventArgs args) : EventData(time), args(args) {}
    };

    struct UIWidgetSoundEventArgs {
        const Engine::WidgetNavigationSound sound;

        UIWidgetSoundEventArgs(Engine::WidgetNavigationSound sound) : sound(sound) {}
    };

    class UIWidgetSoundEvent : public EventData<UIWidgetSoundEvent> {
    public:
        UIWidgetSoundEventArgs args;

        bool cancellable() const {
            return true;
        }

        UIWidgetSoundEvent(EventTime time, UIWidgetSoundEventArgs args) : EventData(time), args(args) {}
    };

    enum struct UIWidgetListTabType {
        TAB_THRU_ITEM_LIST_ITEMS_NEXT_VERTICAL,
        TAB_THRU_ITEM_LIST_ITEMS_NEXT_HORIZONTAL,
        TAB_THRU_ITEM_LIST_ITEMS_PREV_VERTICAL,
        TAB_THRU_ITEM_LIST_ITEMS_PREV_HORIZONTAL,
        TAB_THRU_CHILDREN_NEXT_VERTICAL,
        TAB_THRU_CHILDREN_NEXT_HORIZONTAL,
        TAB_THRU_CHILDREN_PREV // Find how to differentiate between vertical and horizontal on this one
    };

    struct UIWidgetListTabEventArgs {
        Engine::Widget const *widget_list;
        const UIWidgetListTabType tab;

        UIWidgetListTabEventArgs(Engine::Widget const *widget_list, UIWidgetListTabType tab) : widget_list(widget_list), tab(tab) {}
    };

    class UIWidgetListTabEvent : public EventData<UIWidgetListTabEvent> {
    public:
        UIWidgetListTabEventArgs args;

        bool cancellable() const {
            return true;
        }

        UIWidgetListTabEvent(EventTime time, UIWidgetListTabEventArgs args) : EventData(time), args(args) {}
    };
}

#endif
