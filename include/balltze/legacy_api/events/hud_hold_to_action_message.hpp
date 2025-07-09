// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_LEGACY_API__EVENTS__HUD_HOLD_TO_ACTION_MESSAGE_HPP
#define BALLTZE_LEGACY_API__EVENTS__HUD_HOLD_TO_ACTION_MESSAGE_HPP

#include <string>
#include <optional>
#include "../engine/data_types.hpp"
#include "../event.hpp"

namespace Balltze::LegacyApi::Event {
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

        LegacyApi::Engine::InputDevice device;
        PADDING(2);
        Type type;
        std::uint16_t index;
        AxisDirection axis_direction;
        PADDING(2);
    };

    struct HudHoldForActionMessageContext {
        const HudHoldForActionMessageSlice slice;
        LegacyApi::Engine::Point2DInt offset;
        const LegacyApi::Engine::ColorARGBInt color;
        std::wstring text;
        const std::optional<HudHoldToActionMessageButton> button;

        HudHoldForActionMessageContext(HudHoldForActionMessageSlice slice, LegacyApi::Engine::Point2DInt offset, LegacyApi::Engine::ColorARGBInt color, std::wstring text, std::optional<HudHoldToActionMessageButton> button) : slice(slice), offset(offset), color(color), text(text), button(button) {}
    };

    class HudHoldForActionMessageEvent : public EventData<HudHoldForActionMessageEvent> {
    public:
        HudHoldForActionMessageContext context;

        bool cancellable() const {
            return true;
        }

        HudHoldForActionMessageEvent(EventTime time, HudHoldForActionMessageContext context) : EventData(time), context(context) {}
    };
}

#endif
