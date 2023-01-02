// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/event.hpp>
#include <balltze/hook.hpp>
#include <balltze/output.hpp>
#include "../config/config.hpp"

namespace Balltze::Features {
    using namespace Event;

    static bool on_game_input(Event::GameInputEvent &event) {
        if(event.args.device == Engine::INPUT_DEVICE_GAMEPAD) {
            auto button = Config::get_gamepad_mapped_button(event.args.button.gamepad_button);
            if(button == Engine::GAMEPAD_BUTTON_START && !event.args.mapped) {
                Engine::open_pause_menu();
            }
        }
        return true;
    }

    void set_up_gamepad_pause_button() {
        CancellableEventDelegate<GameInputEvent> game_input_event = on_game_input;
        GameInputEvent::subscribe(game_input_event, EVENT_PRIORITY_HIGHEST);
    }
}
