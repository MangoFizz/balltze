// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__EVENTS__RENDER_HPP
#define BALLTZE_API__EVENTS__RENDER_HPP

#include "../engine/tag_definitions/bitmap.hpp"
#include "../engine/user_interface.hpp"
#include "../event.hpp"

namespace Balltze::Event {
    struct UIRenderEventContext {
        std::uint32_t unknown_param_1;
    };

    class UIRenderEvent : public EventData<UIRenderEvent> {
    public:
        UIRenderEventContext context;

        bool cancellable() const {
            return true;
        }

        UIRenderEvent(EventTime time, UIRenderEventContext context) : EventData(time) {}
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

    struct HUDElementBitmapRenderEventContext {
        Engine::TagDefinitions::BitmapData *bitmap_data;
        Engine::ScreenQuad *quad;
    };

    class HUDElementBitmapRenderEvent : public EventData<HUDElementBitmapRenderEvent> {
    public:
        HUDElementBitmapRenderEventContext context;

        bool cancellable() const {
            return true;
        }

        HUDElementBitmapRenderEvent(EventTime time, HUDElementBitmapRenderEventContext context) : EventData(time), context(context) {}
    };

    struct UIWidgetBackgroundRenderEventContext {
        Engine::Widget *widget;
        Engine::ScreenQuad *quad;
    };

    class UIWidgetBackgroundRenderEvent : public EventData<UIWidgetBackgroundRenderEvent> {
    public:
        UIWidgetBackgroundRenderEventContext context;

        bool cancellable() const {
            return true;
        }

        UIWidgetBackgroundRenderEvent(EventTime time, UIWidgetBackgroundRenderEventContext context) : EventData(time), context(context) {}
    };

    class NavPointsRenderEvent : public EventData<NavPointsRenderEvent> {
    public:
        bool cancellable() const {
            return true;
        }

        NavPointsRenderEvent(EventTime time) : EventData(time) {}
    };
}

#endif
