// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__EVENTS__RENDER_HPP
#define BALLTZE_API__EVENTS__RENDER_HPP

#include "../engine/tag_definitions/bitmap.hpp"
#include "../engine/user_interface.hpp"
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

    struct WidgetRenderVertices {
        struct Vertex {
            float x, y, z, rhw;
            float u, v;
        };

        Vertex top_left;
        Vertex top_right;
        Vertex bottom_right;
        Vertex bottom_left;
    };

    struct HUDElementBitmapRenderEventArgs {
        Engine::TagDefinitions::BitmapData *bitmap_data;
        WidgetRenderVertices *vertices;
    };

    class HUDElementBitmapRenderEvent : public EventData<HUDElementBitmapRenderEvent> {
    public:
        HUDElementBitmapRenderEventArgs args;

        bool cancellable() const {
            return true;
        }

        HUDElementBitmapRenderEvent(EventTime time, HUDElementBitmapRenderEventArgs args) : EventData(time), args(args) {}
    };

    struct WidgetBackgroundRenderEventArgs {
        Engine::Widget *widget;
        WidgetRenderVertices *vertices;
    };

    class WidgetBackgroundRenderEvent : public EventData<WidgetBackgroundRenderEvent> {
    public:
        WidgetBackgroundRenderEventArgs args;

        bool cancellable() const {
            return true;
        }

        WidgetBackgroundRenderEvent(EventTime time, WidgetBackgroundRenderEventArgs args) : EventData(time), args(args) {}
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
