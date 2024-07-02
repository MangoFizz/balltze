// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__EVENTS__GAME_INPUT_HPP
#define BALLTZE_API__EVENTS__GAME_INPUT_HPP

#include "../engine/user_interface.hpp"
#include "../event.hpp"

namespace Balltze::Event {
    struct GameInputEventArgs {
        const Engine::InputDevice device;
        union {
            std::size_t key_code;
            Engine::GamepadButton gamepad_button;
        } button;
        const bool mapped;

        GameInputEventArgs(Engine::InputDevice device, std::size_t key_code, bool mapped) : device(device), mapped(mapped) {
            button.key_code = key_code;
        }
    };

    class GameInputEvent : public EventData<GameInputEvent> {
    public:
        GameInputEventArgs args;

        bool cancellable() const {
            return true;
        }

        GameInputEvent(EventTime time, GameInputEventArgs args) : EventData(time), args(args) {}
    };
    
    struct KeyboardInputEventArgs {
        const Engine::InputGlobals::BufferedKey key;

        KeyboardInputEventArgs(Engine::InputGlobals::BufferedKey key) : key(key) { }
    };

    class KeyboardInputEvent : public EventData<KeyboardInputEvent> {
    public:
        KeyboardInputEventArgs args;

        bool cancellable() const {
            return false;
        }

        KeyboardInputEvent(EventTime time, KeyboardInputEventArgs args) : EventData(time), args(args) {}
    };
}

#endif
