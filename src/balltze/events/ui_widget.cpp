// SPDX-License-Identifier: GPL-3.0-only

#include <impl/interface/ui_widget.h>
#include <balltze/hook.hpp>
#include <balltze/events/ui_widget.hpp>

namespace Balltze::Events {
    extern "C" {
        void ui_widget_event_handler_dispatch_hook();
        
        void dispatch_widget_event(Widget *widget, UIWidgetDefinition *widget_definition, UIWidgetEventRecord *event_record, EventHandlerReference *event_handler, int16_t *controller_index) {
            WidgetEventDispatchEvent event(widget, event_record, event_handler);
            event.dispatch();
        }
    }

    Widget *WidgetEventDispatchEvent::widget() const {
        return m_widget;
    }

    UIWidgetEventRecord *WidgetEventDispatchEvent::event_record() const {
        return m_event_record;
    }

    EventHandlerReference *WidgetEventDispatchEvent::event_handler_reference() const {
        return m_event_handler_reference;
    }

    template<>
    void EventHandler<WidgetEventDispatchEvent>::init() {
        if(m_initialized) {
            return;
        }
        m_initialized = true;

        Memory::hook_function(reinterpret_cast<void *>(0x49d3c0), ui_widget_event_handler_dispatch_hook);
    }
}
