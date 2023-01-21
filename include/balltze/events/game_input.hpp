// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__EVENTS__GAME_INPUT_HPP
#define BALLTZE_API__EVENTS__GAME_INPUT_HPP

#include "../engine/user_interface.hpp"
#include "../event.hpp"

namespace Balltze::Event {
    struct GameInputArguments {
        Engine::InputDevice device;
        union {
            std::size_t key_code;
            Engine::GamepadButton gamepad_button;
        } button;
        bool mapped;
    };

    class GameInputEvent : public EventData<GameInputEvent> {
    public:
        GameInputArguments args;

        static bool cancellable() {
            return true;
        }

        GameInputEvent(EventTime time, GameInputArguments args) : EventData(time), args(args) {}
    };
}

#endif
