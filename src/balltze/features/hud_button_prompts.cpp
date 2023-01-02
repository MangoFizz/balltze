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

using namespace Balltze::Event;

namespace Balltze::Features {
    struct ButtonIconAnimation {
        Engine::GamepadButton button;
        std::chrono::time_point<std::chrono::steady_clock> last_frame;
        std::size_t frame_index;

        ButtonIconAnimation(Engine::GamepadButton button) : button(button), last_frame(std::chrono::steady_clock::now()), frame_index(0) {}
    };

    static std::vector<ButtonIconAnimation> button_icon_animations;

    static ButtonIconAnimation *get_button_icon_animation(Engine::GamepadButton button) {
        for(auto &button_icon_animation : button_icon_animations) {
            if(button_icon_animation.button == button) {
                return &button_icon_animation;
            }
        }
        return nullptr;
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

    static bool draw_button_icon(Engine::Point2DInt &offset, std::size_t button_index, Engine::ColorARGBInt color) noexcept {
        auto &hud_globals = Engine::get_hud_globals();
        auto message_icons = hud_globals.icon_bitmap.tag_id;

        std::size_t translated_button_index = Config::get_gamepad_mapped_button(button_index);

        // Get button icon
        if(translated_button_index >= hud_globals.button_icons.count) {
            return false;
        }
        auto button = static_cast<Engine::GamepadButton>(translated_button_index);
        auto &button_icon = hud_globals.button_icons.offset[translated_button_index];

        // Get the sprite index
        std::size_t sprite_index = 0;
        try {
            auto icon_sprites_count = get_bitmap_sequence_sprites_count(message_icons, button_icon.sequence_index);
            if(icon_sprites_count > 1) {
                auto icon_anim = get_button_icon_animation(button);
                if(icon_anim) {
                    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - icon_anim->last_frame).count();
                    float frame_duration = 1000.0f / icon_sprites_count;
                    if(elapsed_time > frame_duration) {
                        if(elapsed_time > 1000) {
                            sprite_index = 0;
                        }
                        else {
                            sprite_index = (icon_anim->frame_index + 1) % icon_sprites_count;
                        }
                        icon_anim->last_frame = std::chrono::steady_clock::now();
                        icon_anim->frame_index = sprite_index;
                    }
                    else {
                        sprite_index = icon_anim->frame_index;
                    }
                }
                else {
                    button_icon_animations.emplace_back(button);
                }
            }
        }
        catch(std::runtime_error) {
            return false;
        }

        // Draw the button icon
        auto final_color = color;
        if(button_icon.flags.override_default_color) {
            final_color = button_icon.override_icon_color;
        }
        Engine::draw_hud_message_sprite(message_icons, button_icon.sequence_index, sprite_index, offset, final_color);

        // bump up the offset for the next slice
        auto [sprite_width, sprite_height] = Engine::get_bitmap_sprite_size(message_icons, button_icon.sequence_index, sprite_index);
        offset.x += sprite_width;

        return true;
    }

    static bool on_hud_hold_to_action_event(HudHoldForActionMessageEvent &event) {
        bool allow = true;
        if(event.time == EVENT_TIME_BEFORE) {
            static std::wstring left_quote;
            static std::wstring right_quote;

            auto slice = event.args.slice;
            if(slice == HudHoldForActionMessageSlice::BUTTON_NAME) {
                if(event.args.input_source == Engine::INPUT_DEVICE_GAMEPAD && event.args.gamepad_pressed_button_index) {
                    auto button_index = event.args.gamepad_pressed_button_index.value();
                    auto &offset = event.args.offset;
                    auto &color = event.args.color;
                    bool success = draw_button_icon(offset, button_index, color);
                    allow = !success;
                }
                if(allow) {
                    event.args.text = left_quote + Engine::get_button_name(event.args.input_source.value(), event.args.gamepad_pressed_button_index.value()) + right_quote;
                    allow = true;
                }
            }
            else if(slice == HudHoldForActionMessageSlice::BUTTON_NAME_LEFT_QUOTE) {
                left_quote = event.args.text;
                allow = false;
            }
            else if(slice == HudHoldForActionMessageSlice::BUTTON_NAME_RIGHT_QUOTE) {
                right_quote = event.args.text;
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
