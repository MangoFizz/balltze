// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/engine/user_interface.hpp>
#include <balltze/event.hpp>
#include <balltze/memory.hpp>
#include <balltze/api.hpp>
#include <balltze/hook.hpp>
#include <balltze/command.hpp>
#include "../logger.hpp"

namespace Balltze::Event {
    extern "C" {
        bool keyboard_input_event();
        bool mouse_input_event();
        bool gamepad_input_event();
        void keypress_event();
    
        bool dispatch_input_event_before(Engine::InputDevice device, std::size_t key_code, bool mapped) {
            GameInputEventArgs args(device, key_code, mapped);
            GameInputEvent event(EVENT_TIME_BEFORE, args);
            event.dispatch();
            return event.cancelled();
        }

        void dispatch_input_event_after(Engine::InputDevice device, std::size_t key_code, bool mapped) {
            GameInputEventArgs args(device, key_code, mapped);
            GameInputEvent event(EVENT_TIME_AFTER, args);
            event.dispatch();
        }

        void dispatch_keypress_event(Engine::InputGlobals::BufferedKey key) {
            KeyboardInputEventArgs args(key);
            KeyboardInputEvent event(EVENT_TIME_BEFORE, args);
            event.dispatch();
        }
    }

    static bool debug_game_input_event(int arg_count, const char **args) {
        static std::optional<Event::EventListenerHandle<GameInputEvent>> handle;
        if(arg_count == 1) {
            bool new_setting = STR_TO_BOOL(args[0]);
            if(new_setting) {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
                handle = Event::GameInputEvent::subscribe_const([](GameInputEvent const &event) {
                    auto &arguments = event.args;
                    auto time = event_time_to_string(event.time);
                    logger.debug("Game input event ({}): device: {}, key code: {}, mapped: {}", time, static_cast<int>(arguments.device), arguments.button.key_code, arguments.mapped);
                });
            }
            else {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
            }
        }
        logger.info("debug_game_input_event: {}", handle.has_value());
        return true;
    }

    static bool debug_keypress_event(int arg_count, const char **args) {
        static std::optional<Event::EventListenerHandle<KeyboardInputEvent>> handle;
        if(arg_count == 1) {
            bool new_setting = STR_TO_BOOL(args[0]);
            if(new_setting) {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
                handle = Event::KeyboardInputEvent::subscribe_const([](KeyboardInputEvent const &event) {
                    auto &arguments = event.args;
                    auto time = event_time_to_string(event.time);
                    logger.debug("Keypress event ({}): key: {}", time, arguments.key.keycode);
                });
            }
            else {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
            }
        }
        logger.info("debug_keypress_event: {}", handle.has_value());
        return true;
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

        // Register debug command
        register_command("debug_game_input_event", "debug", "Sets whenever to log game input event.", "[enable: boolean]", debug_game_input_event, true, 0, 1);
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

        // Register debug command
        register_command("debug_keypress_event", "debug", "Sets whenever to log keyboard input event.", "[enable: boolean]", debug_keypress_event, true, 0, 1);
    }
}
