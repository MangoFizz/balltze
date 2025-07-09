// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_LEGACY_API__EVENTS__WIDGET_HPP
#define BALLTZE_LEGACY_API__EVENTS__WIDGET_HPP

#include <balltze/legacy_api/engine/user_interface.hpp>
#include "../event.hpp"

namespace Balltze::LegacyApi::Event {
    struct UIWidgetEventContext {
        LegacyApi::Engine::Widget *const widget;

        UIWidgetEventContext(LegacyApi::Engine::Widget *const widget) : widget(widget) {}
    };

    struct UIWidgetCreateEventContext : UIWidgetEventContext {
        const LegacyApi::Engine::TagHandle definition_tag_handle;
        const bool is_root_widget;
        LegacyApi::Engine::Widget *const parent_widget;

        UIWidgetCreateEventContext(LegacyApi::Engine::Widget *const widget, LegacyApi::Engine::TagHandle definition_tag_handle, bool is_root_widget, LegacyApi::Engine::Widget *const parent_widget) : UIWidgetEventContext(widget), definition_tag_handle(definition_tag_handle), is_root_widget(is_root_widget), parent_widget(parent_widget) {}
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
        const LegacyApi::Engine::WidgetNavigationSound sound;

        UIWidgetSoundEventContext(LegacyApi::Engine::WidgetNavigationSound sound) : sound(sound) {}
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
        LegacyApi::Engine::Widget *const widget_list;
        const UIWidgetListTabType tab;

        UIWidgetListTabEventContext(LegacyApi::Engine::Widget *const widget_list, UIWidgetListTabType tab) : widget_list(widget_list), tab(tab) {}
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
        LegacyApi::Engine::Widget *const widget;
        const LegacyApi::Engine::MouseButton button;

        UIWidgetMouseButtonPressEventContext(LegacyApi::Engine::Widget *const widget, const LegacyApi::Engine::MouseButton button) : widget(widget), button(button) {}
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
