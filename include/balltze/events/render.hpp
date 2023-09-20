// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__EVENTS__RENDER_HPP
#define BALLTZE_API__EVENTS__RENDER_HPP

#include "../event.hpp"

namespace Balltze::Event {
    struct UIRenderEventArgs {
        std::uint32_t unknown_param_1;
    };

    class UIRenderEvent : public EventData<UIRenderEvent> {
    public:
        UIRenderEventArgs args;

        bool cancellable() const {
            return true;
        }

        UIRenderEvent(EventTime time, UIRenderEventArgs args) : EventData(time) {}
    };

    class HUDRenderEvent : public EventData<HUDRenderEvent> {
    public:
        bool cancellable() const {
            return true;
        }

        HUDRenderEvent(EventTime time) : EventData(time) {}
    };

    class PostCarnageReportRenderEvent : public EventData<PostCarnageReportRenderEvent> {
    public:
        bool cancellable() const {
            return true;
        }

        PostCarnageReportRenderEvent(EventTime time) : EventData(time) {}
    };
}

#endif
