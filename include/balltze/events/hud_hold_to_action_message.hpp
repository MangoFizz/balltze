// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__EVENTS__HUD_HOLD_TO_ACTION_MESSAGE_HPP
#define BALLTZE_API__EVENTS__HUD_HOLD_TO_ACTION_MESSAGE_HPP

#include <string>
#include <optional>
#include "../engine/data_types.hpp"
#include "../event.hpp"

namespace Balltze::Event {
    enum struct HudHoldForActionMessageSlice {
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

    struct HudHoldForActionMessageArguments {
        HudHoldForActionMessageSlice slice;
        Engine::Point2DInt offset;
        Engine::ColorARGBInt color;
        std::wstring text;
        std::optional<HudHoldToActionMessageButton> button;
    };

    class HudHoldForActionMessageEvent : public EventData<HudHoldForActionMessageEvent> {
    public:
        HudHoldForActionMessageArguments args;

        static bool cancellable() {
            return true;
        }

        HudHoldForActionMessageEvent(EventTime time, HudHoldForActionMessageArguments args) : EventData(time), args(args) {}
    };
}

#endif
