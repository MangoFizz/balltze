// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__EVENTS__WIDGET_HPP
#define BALLTZE_API__EVENTS__WIDGET_HPP

#include <balltze/engine/user_interface.hpp>
#include "../event.hpp"

namespace Balltze::Event {
    struct UIWidgetEventContext {
        Engine::Widget *const widget;

        UIWidgetEventContext(Engine::Widget *const widget) : widget(widget) {}
    };

    struct UIWidgetCreateEventContext : UIWidgetEventContext {
        const Engine::TagHandle definition_tag_handle;
        const bool is_root_widget;
        Engine::Widget *const parent_widget;

        UIWidgetCreateEventContext(Engine::Widget *const widget, Engine::TagHandle definition_tag_handle, bool is_root_widget, Engine::Widget *const parent_widget) : UIWidgetEventContext(widget), definition_tag_handle(definition_tag_handle), is_root_widget(is_root_widget), parent_widget(parent_widget) {}
    };

    class UIWidgetCreateEvent : public EventData<UIWidgetCreateEvent> {
    public:
        UIWidgetCreateEventContext context;

        bool cancellable() const {
            return false;
        }

        UIWidgetCreateEvent(EventTime time, UIWidgetCreateEventContext context) : EventData(time), context(context) {}
    };

    class UIWidgetBackEvent : public EventData<UIWidgetBackEvent> {
    public:
        UIWidgetEventContext context;

        bool cancellable() const {
            return true;
        }

        UIWidgetBackEvent(EventTime time, UIWidgetEventContext context) : EventData(time), context(context) {}
    };

    class UIWidgetFocusEvent : public EventData<UIWidgetFocusEvent> {
    public:
        UIWidgetEventContext context;

        bool cancellable() const {
            return true;
        }

        UIWidgetFocusEvent(EventTime time, UIWidgetEventContext context) : EventData(time), context(context) {}
    };

    class UIWidgetAcceptEvent : public EventData<UIWidgetAcceptEvent> {
    public:
        UIWidgetEventContext context;

        bool cancellable() const {
            return true;
        }

        UIWidgetAcceptEvent(EventTime time, UIWidgetEventContext context) : EventData(time), context(context) {}
    };

    struct UIWidgetSoundEventContext {
        const Engine::WidgetNavigationSound sound;

        UIWidgetSoundEventContext(Engine::WidgetNavigationSound sound) : sound(sound) {}
    };

    class UIWidgetSoundEvent : public EventData<UIWidgetSoundEvent> {
    public:
        UIWidgetSoundEventContext context;

        bool cancellable() const {
            return true;
        }

        UIWidgetSoundEvent(EventTime time, UIWidgetSoundEventContext context) : EventData(time), context(context) {}
    };

    enum UIWidgetListTabType {
        TAB_THRU_ITEM_LIST_ITEMS_NEXT_VERTICAL,
        TAB_THRU_ITEM_LIST_ITEMS_NEXT_HORIZONTAL,
        TAB_THRU_ITEM_LIST_ITEMS_PREV_VERTICAL,
        TAB_THRU_ITEM_LIST_ITEMS_PREV_HORIZONTAL,
        TAB_THRU_CHILDREN_NEXT_VERTICAL,
        TAB_THRU_CHILDREN_NEXT_HORIZONTAL,
        TAB_THRU_CHILDREN_PREV // Find how to differentiate between vertical and horizontal on this one
    };

    struct UIWidgetListTabEventContext {
        Engine::Widget *const widget_list;
        const UIWidgetListTabType tab;

        UIWidgetListTabEventContext(Engine::Widget *const widget_list, UIWidgetListTabType tab) : widget_list(widget_list), tab(tab) {}
    };

    class UIWidgetListTabEvent : public EventData<UIWidgetListTabEvent> {
    public:
        UIWidgetListTabEventContext context;

        bool cancellable() const {
            return true;
        }

        UIWidgetListTabEvent(EventTime time, UIWidgetListTabEventContext context) : EventData(time), context(context) {}
    };

    struct UIWidgetMouseButtonPressEventContext {
        Engine::Widget *const widget;
        const Engine::MouseButton button;

        UIWidgetMouseButtonPressEventContext(Engine::Widget *const widget, const Engine::MouseButton button) : widget(widget), button(button) {}
    };

    class UIWidgetMouseButtonPressEvent : public EventData<UIWidgetMouseButtonPressEvent> {
    public:
        UIWidgetMouseButtonPressEventContext context;

        bool cancellable() const {
            return true;
        }

        UIWidgetMouseButtonPressEvent(EventTime time, UIWidgetMouseButtonPressEventContext context) : EventData(time), context(context) {}
    };
}

#endif
