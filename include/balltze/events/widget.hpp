// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__EVENTS__WIDGET_HPP
#define BALLTZE_API__EVENTS__WIDGET_HPP

#include <balltze/engine/user_interface.hpp>
#include "../event.hpp"

namespace Balltze::Event {
    struct UIWidgetEventArguments {
        Engine::Widget *widget;
    };

    class UIWidgetOpenEvent : public EventData<UIWidgetOpenEvent> {
    public:
        UIWidgetEventArguments args;

        bool cancellable() const {
            return false;
        }

        UIWidgetOpenEvent(EventTime time, UIWidgetEventArguments args) : EventData(time), args(args) {}
    };
}

#endif
