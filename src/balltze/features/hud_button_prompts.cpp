// SPDX-License-Identifier: GPL-3.0-only

#include <chrono>
#include <optional>
#include <balltze/config.hpp>
#include <balltze/output.hpp>
#include <balltze/memory.hpp>
#include <balltze/engine/hud.hpp>
#include <balltze/engine/widget.hpp>
#include <balltze/event.hpp>
#include "../config/config.hpp"

namespace Balltze::Features {
    using namespace Balltze::Event;

    static std::optional<Engine::GamepadButton> last_gamepad_button_pressed;
    static std::chrono::time_point<std::chrono::steady_clock> start_time;

    static Engine::GamepadButton translate_gamepad_button(std::size_t button_index) {
        try {
            auto balltze_path = Config::get_balltze_directory();
            auto controller_config = Config::Config(balltze_path / "config" / "controller.json");
            auto mapped_button_index = controller_config.get("buttons_map.button_" + std::to_string(button_index));
            if(mapped_button_index) {
                auto translated_button_index = std::atoi(mapped_button_index->c_str());
                return static_cast<Engine::GamepadButton>(translated_button_index);
            }
        }
        catch(std::runtime_error) {
            // Ignore
        }
        return static_cast<Engine::GamepadButton>(button_index);
    }

    static std::size_t get_bitmap_sequence_sprites_count(Engine::TagHandle bitmap_tag_handle, std::size_t sequence_index) {
        auto *bitmap_tag = Engine::get_tag(bitmap_tag_handle);
        auto *bitmap = bitmap_tag->get_data<Engine::TagDefinitions::Bitmap>();
        if(sequence_index >= bitmap->bitmap_group_sequence.count) {
            throw std::runtime_error("Invalid sequence index");
        }
        auto &sequence = bitmap->bitmap_group_sequence.offset[sequence_index];
        return sequence.sprites.count;
    }

    static bool on_hud_hold_to_action_event(Event::HudHoldForActionMessageEvent &event) {
        bool allow = true;
        if(event.time == Event::EVENT_TIME_BEFORE) {
            auto slice = event.args.slice;
            if(slice == Event::HudHoldForActionMessageSlice::BUTTON_NAME) {
                auto &hud_globals = Engine::get_hud_globals();
                auto message_icons = hud_globals.icon_bitmap.tag_id;
                auto offset = event.args.offset;

                if(!event.args.gamepad_pressed_button_index) {
                    return true;
                }
                std::size_t button_index = translate_gamepad_button(event.args.gamepad_pressed_button_index.value());

                // Get button icon
                if(button_index >= hud_globals.button_icons.count) {
                    return true;
                }
                auto &button_icon = hud_globals.button_icons.offset[button_index];

                std::size_t sprite_index = 0;
                auto icon_sprites_count = get_bitmap_sequence_sprites_count(message_icons, button_icon.sequence_index);
                if(icon_sprites_count > 1) {
                    if(last_gamepad_button_pressed && last_gamepad_button_pressed == static_cast<Engine::GamepadButton>(button_index)) {
                        try {
                            auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time).count();
                            float frame_progress = elapsed_time % 1000;
                            // float frame_duration = 1000 / button_icon.frame_rate;
                            float frame_duration = 1000 / icon_sprites_count;
                            int current_frame = frame_progress / frame_duration;
                            sprite_index = current_frame % icon_sprites_count;
                        }
                        catch(std::runtime_error e) {
                            show_message_box("%s", e.what());
                        }
                    }
                }

                // Draw the button icon
                auto color = button_icon.flags.override_default_color ? button_icon.override_icon_color : hud_globals.icon_color.to_int();
                Engine::draw_hud_message_sprite(message_icons, button_icon.sequence_index, sprite_index, offset, color);

                // bump up the offset for the next slice
                auto [sprite_width, sprite_height] = Engine::get_bitmap_sprite_size(message_icons, button_icon.sequence_index, sprite_index);
                event.args.offset.x += sprite_width;

                if(!last_gamepad_button_pressed || last_gamepad_button_pressed.value() != static_cast<Engine::GamepadButton>(button_index)) {
                    last_gamepad_button_pressed = static_cast<Engine::GamepadButton>(button_index);
                    start_time = std::chrono::steady_clock::now();
                }

                allow = false;
            }
            else if(slice == HudHoldForActionMessageSlice::BUTTON_NAME_LEFT_QUOTE || slice == HudHoldForActionMessageSlice::BUTTON_NAME_RIGHT_QUOTE) {
                allow = false;
            }
        }
        return allow;
    }

    void set_up_hud_button_prompts() {
        CancellableEventDelegate<HudHoldForActionMessageEvent> event = on_hud_hold_to_action_event;
        HudHoldForActionMessageEvent::subscribe(event, EVENT_PRIORITY_HIGHEST);
    }
}
