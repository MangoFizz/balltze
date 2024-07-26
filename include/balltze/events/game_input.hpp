// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__EVENTS__GAME_INPUT_HPP
#define BALLTZE_API__EVENTS__GAME_INPUT_HPP

#include "../engine/user_interface.hpp"
#include "../event.hpp"

namespace Balltze::Event {
    struct GameInputEventContext {
        const Engine::InputDevice device;
        union {
            std::size_t key_code;
            Engine::GamepadButton gamepad_button;
        } button;
        const bool mapped;

        GameInputEventContext(Engine::InputDevice device, std::size_t key_code, bool mapped) : device(device), mapped(mapped) {
            button.key_code = key_code;
        }
    };

    class GameInputEvent : public EventData<GameInputEvent> {
    public:
        GameInputEventContext context;

        bool cancellable() const {
            return true;
        }

        GameInputEvent(EventTime time, GameInputEventContext context) : EventData(time), context(context) {}
    };
    
    struct KeyboardInputEventContext {
        const Engine::InputGlobals::BufferedKey key;

        KeyboardInputEventContext(Engine::InputGlobals::BufferedKey key) : key(key) { }
    };

    class KeyboardInputEvent : public EventData<KeyboardInputEvent> {
    public:
        KeyboardInputEventContext context;

        bool cancellable() const {
            return false;
        }

        KeyboardInputEvent(EventTime time, KeyboardInputEventContext context) : EventData(time), context(context) {}
    };
}

#endif
