// SPDX-License-Identifier: GPL-3.0-only

/**
 * @file balltze/events/ui_widget.hpp
 * @brief Event classes for UI widget events in Balltze.
 * 
 * @details This header defines all the event classes related to UI widgets.
 */

#ifndef BALLTZE_API__EVENTS__UI_WIDGET_HPP
#define BALLTZE_API__EVENTS__UI_WIDGET_HPP

#include "../events.hpp"

struct Widget;
struct UIWidgetEventRecord;
struct EventHandlerReference;

namespace Balltze::Events {
    class WidgetEventDispatchEvent : public EventData<WidgetEventDispatchEvent> {
    public:
        /**
         * Get the widget associated with this event.
         */
        Widget *widget() const;

        /**
         * Get the event record associated with this event.
         */
        UIWidgetEventRecord *event_record() const;

        /**
         * Get the event handler reference associated with this event.
         */
        EventHandlerReference *event_handler_reference() const;

        /**
         * Constructor for WidgetEventDispatchEvent.
         * 
         * @param widget Pointer to the widget associated with this event.
         * @param event_record Pointer to the UIWidgetEventRecord associated with this event.
         * @param event_handler_reference Pointer to the event handler reference associated with this event.
         */
        WidgetEventDispatchEvent(Widget *widget, UIWidgetEventRecord *event_record, EventHandlerReference *event_handler_reference)
            : EventData<WidgetEventDispatchEvent>(true), m_widget(widget), m_event_record(event_record), m_event_handler_reference(event_handler_reference) {}

    private:
        Widget *m_widget = nullptr;
        UIWidgetEventRecord *m_event_record = nullptr;
        EventHandlerReference *m_event_handler_reference = nullptr;
    };
}

#endif
