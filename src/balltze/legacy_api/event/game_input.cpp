// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/legacy_api/engine/user_interface.hpp>
#include <balltze/legacy_api/event.hpp>
#include <balltze/memory.hpp>
#include <balltze/api.hpp>
#include <balltze/hook.hpp>
#include <balltze/command.hpp>
#include "../../logger.hpp"

namespace Balltze::LegacyApi::Event {
    extern "C" {
        bool keyboard_input_event();
        bool mouse_input_event();
        bool gamepad_input_event();
        void keypress_event();
    
        bool dispatch_input_event_before(LegacyApi::Engine::InputDevice device, std::size_t key_code, bool mapped) {
            GameInputEventContext args(device, key_code, mapped);
            GameInputEvent event(EVENT_TIME_BEFORE, args);
            event.dispatch();
            return event.cancelled();
        }

        void dispatch_input_event_after(LegacyApi::Engine::InputDevice device, std::size_t key_code, bool mapped) {
            GameInputEventContext args(device, key_code, mapped);
            GameInputEvent event(EVENT_TIME_AFTER, args);
            event.dispatch();
        }

        void dispatch_keypress_event(LegacyApi::Engine::InputGlobals::BufferedKey key) {
            KeyboardInputEventContext args(key);
            KeyboardInputEvent event(EVENT_TIME_BEFORE, args);
            event.dispatch();
        }
    }

    template<>
    void EventHandler<GameInputEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        if(get_balltze_side() != BALLTZE_SIDE_CLIENT) {
            return;
        }

        static auto *keyboard_input_sig = Memory::get_signature("keyboard_input");
        if(!keyboard_input_sig) {
            throw std::runtime_error("Could not find signature for keyboard input address");
        }
        static auto *mouse_input_sig = Memory::get_signature("mouse_input");
        if(!mouse_input_sig) {
            throw std::runtime_error("Could not find signature for mouse input address");
        }
        static auto *gamepad_input_sig = Memory::get_signature("gamepad_input");
        if(!gamepad_input_sig) {
            throw std::runtime_error("Could not find signature for gamepad input address");
        }

        try {
            std::function<bool()> dispatcher = keyboard_input_event;
            auto *button_name_hook = Memory::hook_function(keyboard_input_sig->data(), dispatcher, std::nullopt, false);

            dispatcher = mouse_input_event;
            auto *mouse_hook = Memory::hook_function(mouse_input_sig->data(), dispatcher, std::nullopt, false);

            dispatcher = gamepad_input_event;
            auto *gamepad_hook = Memory::hook_function(gamepad_input_sig->data(), dispatcher, std::nullopt, false);
        }
        catch(const std::runtime_error &e) {
            logger.error("Failed to set up input event hooks: {}", e.what());
        }
    }

    template<>
    void EventHandler<KeyboardInputEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        if(get_balltze_side() != BALLTZE_SIDE_CLIENT) {
            return;
        }

        static auto *keyboard_input_sig = Memory::get_signature("keypress_event");
        if(!keyboard_input_sig) {
            throw std::runtime_error("Could not find signature for keyboard input event");
        }

        try {
            auto *button_name_hook = Memory::hook_function(keyboard_input_sig->data(), keypress_event, std::nullopt, false);
        }
        catch(const std::runtime_error &e) {
            logger.error("Failed to set up keyboard input event hook: {}", e.what());
        }
    }
}
