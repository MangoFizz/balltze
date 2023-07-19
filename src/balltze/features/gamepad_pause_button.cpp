// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/event.hpp>
#include <balltze/hook.hpp>
#include "../config/config.hpp"

namespace Balltze::Features {
    using namespace Event;

    // workaround for config files not working at startup. FIXME.
    static std::optional<int> get_pause_button() {
        static std::optional<int> pause_button;
        static bool done = false;
        if(!pause_button && !done) {
            auto gamepad_config = Config::get_gamepad_config();
            if(!gamepad_config) {
                return std::nullopt;
            }
            pause_button = gamepad_config->get<int>("pause_button");
            done = true;
        }
        return pause_button;
    }

    static void on_game_input(Event::GameInputEvent &event) {
        if(event.args.device == Engine::INPUT_DEVICE_GAMEPAD) {
            auto pause_button = get_pause_button();
            if(pause_button && pause_button == event.args.button.gamepad_button && !event.args.mapped) {
                Engine::open_pause_menu();
            }
        }
    }

    void set_up_gamepad_pause_button() {
        GameInputEvent::subscribe(on_game_input, EVENT_PRIORITY_HIGHEST);
    }
}
