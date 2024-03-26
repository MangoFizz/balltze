// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__EVENTS__WIDGET_HPP
#define BALLTZE_API__EVENTS__WIDGET_HPP

#include <balltze/engine/user_interface.hpp>
#include "../event.hpp"

namespace Balltze::Event {
    struct UIWidgetEventArguments {
        Engine::Widget *widget;
    };

    struct UIWidgetOpenEventArguments : UIWidgetEventArguments {
        Engine::TagHandle definition_tag_handle;
        bool is_root_widget;
        Engine::Widget *parent_widget;
    };

    class UIWidgetOpenEvent : public EventData<UIWidgetOpenEvent> {
    public:
        UIWidgetOpenEventArguments args;

        bool cancellable() const {
            return true;
        }
        void cancel();

        UIWidgetOpenEvent(EventTime time, UIWidgetOpenEventArguments args) : EventData(time), args(args) {}
    };

    class UIWidgetBackEvent : public EventData<UIWidgetBackEvent> {
    public:
        UIWidgetEventArguments args;

        bool cancellable() const {
            return true;
        }

        UIWidgetBackEvent(EventTime time, UIWidgetEventArguments args) : EventData(time), args(args) {}
    };

    class UIWidgetFocusEvent : public EventData<UIWidgetFocusEvent> {
    public:
        UIWidgetEventArguments args;

        bool cancellable() const {
            return true;
        }

        UIWidgetFocusEvent(EventTime time, UIWidgetEventArguments args) : EventData(time), args(args) {}
    };

    class UIWidgetAcceptEvent : public EventData<UIWidgetAcceptEvent> {
    public:
        UIWidgetEventArguments args;

        bool cancellable() const {
            return true;
        }

        UIWidgetAcceptEvent(EventTime time, UIWidgetEventArguments args) : EventData(time), args(args) {}
    };

    struct UIWidgetSoundEventArguments {
        Engine::WidgetNavigationSound sound;
    };

    class UIWidgetSoundEvent : public EventData<UIWidgetSoundEvent> {
    public:
        UIWidgetSoundEventArguments args;

        bool cancellable() const {
            return true;
        }

        UIWidgetSoundEvent(EventTime time, UIWidgetSoundEventArguments args) : EventData(time), args(args) {}
    };

    enum struct UIWidgetListTabType {
        TAB_THRU_ITEM_LIST_ITEMS_NEXT_VERTICAL,
        TAB_THRU_ITEM_LIST_ITEMS_NEXT_HORIZONTAL,
        TAB_THRU_ITEM_LIST_ITEMS_PREVIOUS_VERTICAL,
        TAB_THRU_ITEM_LIST_ITEMS_PREVIOUS_HORIZONTAL,
        TAB_THRU_CHILDREN_NEXT_VERTICAL,
        TAB_THRU_CHILDREN_NEXT_HORIZONTAL,
        TAB_THRU_CHILDREN_PREVIOUS // Find how to differentiate between vertical and horizontal on this one
    };

    struct UIWidgetListTabEventArguments {
        Engine::Widget *widget_list;
        UIWidgetListTabType tab;
    };

    class UIWidgetListTabEvent : public EventData<UIWidgetListTabEvent> {
    public:
        UIWidgetListTabEventArguments args;

        bool cancellable() const {
            return true;
        }

        UIWidgetListTabEvent(EventTime time, UIWidgetListTabEventArguments args) : EventData(time), args(args) {}
    };
}

#endif
