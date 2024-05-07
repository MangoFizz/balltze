// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__EVENTS__HUD_HOLD_TO_ACTION_MESSAGE_HPP
#define BALLTZE_API__EVENTS__HUD_HOLD_TO_ACTION_MESSAGE_HPP

#include <string>
#include <optional>
#include "../engine/data_types.hpp"
#include "../event.hpp"

namespace Balltze::Event {
    enum HudHoldForActionMessageSlice {
        MESSAGE,
        BUTTON_NAME_LEFT_QUOTE,
        BUTTON_NAME_RIGHT_QUOTE,
        BUTTON_NAME,
        WEAPON_ICON
    };

    struct HudHoldForActionMessageSliceOffset {
        std::int16_t y;
        std::int16_t x;
    };

    struct HudHoldToActionMessageButton {
        enum Type : std::uint16_t {
            BUTTON,
            AXIS
        };

        enum AxisDirection : std::uint16_t {
            POSITIVE = 1,
            NEGATIVE
        };

        Engine::InputDevice device;
        PADDING(2);
        Type type;
        std::uint16_t index;
        AxisDirection axis_direction;
        PADDING(2);
    };

    struct HudHoldForActionMessageArgs {
        const HudHoldForActionMessageSlice slice;
        Engine::Point2DInt offset;
        const Engine::ColorARGBInt color;
        std::wstring text;
        const std::optional<HudHoldToActionMessageButton> button;

        HudHoldForActionMessageArgs(HudHoldForActionMessageSlice slice, Engine::Point2DInt offset, Engine::ColorARGBInt color, std::wstring text, std::optional<HudHoldToActionMessageButton> button) : slice(slice), offset(offset), color(color), text(text), button(button) {}
    };

    class HudHoldForActionMessageEvent : public EventData<HudHoldForActionMessageEvent> {
    public:
        HudHoldForActionMessageArgs args;

        bool cancellable() const {
            return true;
        }

        HudHoldForActionMessageEvent(EventTime time, HudHoldForActionMessageArgs args) : EventData(time), args(args) {}
    };
}

#endif
