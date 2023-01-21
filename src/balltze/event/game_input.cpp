// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__EVENT__GAME_INPUT_HPP
#define BALLTZE__EVENT__GAME_INPUT_HPP

#include <balltze/engine/user_interface.hpp>
#include <balltze/event.hpp>
#include <balltze/memory.hpp>
#include <balltze/hook.hpp>

namespace Balltze::Event {
    extern "C" {
        bool keyboard_input_before_event();
        bool mouse_input_before_event();
        bool gamepad_input_before_event();
    
        bool dispatch_input_event_before(Engine::InputDevice device, std::size_t key_code, bool mapped) {
            GameInputArguments args;
            args.device = device;
            args.button.key_code = key_code;
            args.mapped = mapped;
            GameInputEvent event(EVENT_TIME_BEFORE, args);
            event.dispatch();
            return event.cancelled();
        }
    }

    template <>
    void EventHandler<GameInputEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        static auto *keyboard_input_address_sig = Memory::get_signature("keyboard_input_address");
        if(!keyboard_input_address_sig) {
            throw std::runtime_error("Could not find signature for keyboard input address");
        }
        static auto *mouse_input_address_sig = Memory::get_signature("mouse_input_address");
        if(!mouse_input_address_sig) {
            throw std::runtime_error("Could not find signature for mouse input address");
        }
        static auto *gamepad_input_sig = Memory::get_signature("gamepad_input");
        if(!gamepad_input_sig) {
            throw std::runtime_error("Could not find signature for gamepad input address");
        }

        std::function<bool()> dispatcher = keyboard_input_before_event;
        // auto *button_name_hook = Memory::hook_function(keyboard_input_address_sig->data(), dispatcher, keyboard_input_after_event, false);

        dispatcher = mouse_input_before_event;
        // auto *mouse_hook = Memory::hook_function(mouse_input_address_sig->data(), dispatcher, mouse_input_after_event, false);

        dispatcher = gamepad_input_before_event;
        auto *gamepad_hook = Memory::hook_function(gamepad_input_sig->data(), dispatcher, std::nullopt, false);
    }
}

#endif
