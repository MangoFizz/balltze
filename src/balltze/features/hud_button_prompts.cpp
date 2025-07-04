// SPDX-License-Identifier: GPL-3.0-only

#include <chrono>
#include <optional>
#include <balltze/config.hpp>
#include <balltze/memory.hpp>
#include <balltze/utils.hpp>
#include <balltze/legacy_api/engine.hpp>
#include <balltze/legacy_api/event.hpp>
#include "../config/config.hpp"
#include "../logger.hpp"

using namespace Balltze::LegacyApi::Event;

namespace Balltze::Features {
    struct ButtonIconCache {
        HudHoldToActionMessageButton button;
        LegacyApi::Engine::GamepadButton icon_index;
        LegacyApi::Engine::TagDefinitions::Bitmap *bitmap;
        std::size_t sequence_index;
        std::size_t sprites_count;
        std::size_t animation_current_sprite_index;
        std::chrono::time_point<std::chrono::steady_clock> last_animation_frame;
        LegacyApi::Engine::Resolution sprite_resolution;
        std::optional<LegacyApi::Engine::ColorARGBInt> color_override;
        bool found = false;
    };

    static Config::Config gamepad_config;
    static std::vector<ButtonIconCache> button_icons;
    static std::string last_map_name;

    static std::optional<LegacyApi::Engine::GamepadButton> get_icon(HudHoldToActionMessageButton &button) {
        static auto gamepad_config = Config::get_gamepad_config();
        if(gamepad_config) {
            if(button.type == HudHoldToActionMessageButton::BUTTON) {
                auto button_icon = gamepad_config->get<int>("icons.button_" + std::to_string(button.index));
                if(button_icon) {
                    return static_cast<LegacyApi::Engine::GamepadButton>(*button_icon);
                }
            }
            else if(button.type == HudHoldToActionMessageButton::AXIS) {
                auto button_icon = gamepad_config->get<int>("icons.axis_" + std::to_string(button.index) + (button.axis_direction == HudHoldToActionMessageButton::POSITIVE ? "+" : "-"));
                if(button_icon) {
                    return static_cast<LegacyApi::Engine::GamepadButton>(*button_icon);
                }
            }
        }
        return std::nullopt;
    }

    static ButtonIconCache *find_button_icon(HudHoldToActionMessageButton &button) {
        ButtonIconCache &button_icon = button_icons.emplace_back();
        auto icon = get_icon(button);
        auto &hud_globals = LegacyApi::Engine::get_hud_globals();
        if(icon && *icon < hud_globals.button_icons.count) {
            auto &hud_icon = hud_globals.button_icons.elements[*icon];
            if(!hud_globals.icon_bitmap.tag_handle.is_null()) {
                auto message_icons_tag_handle = hud_globals.icon_bitmap.tag_handle;
                auto *message_icons_tag = LegacyApi::Engine::get_tag(message_icons_tag_handle);
                if(message_icons_tag) {
                    auto *message_icons = message_icons_tag->get_data<LegacyApi::Engine::TagDefinitions::Bitmap>();
                    auto *bitmap = message_icons;
                    if(hud_icon.sequence_index < bitmap->bitmap_group_sequence.count) {
                        auto &sequence = bitmap->bitmap_group_sequence.elements[hud_icon.sequence_index];
                        if(sequence.sprites.count > 0) {
                            button_icon.button = button;
                            button_icon.icon_index = *icon;
                            button_icon.bitmap = bitmap;
                            button_icon.sequence_index = hud_icon.sequence_index;
                            button_icon.sprites_count = bitmap->bitmap_group_sequence.elements[button_icon.sequence_index].sprites.count;
                            button_icon.animation_current_sprite_index = 0;
                            button_icon.last_animation_frame = std::chrono::steady_clock::now();
                            
                            try {
                                button_icon.sprite_resolution = LegacyApi::Engine::get_bitmap_sprite_resolution(bitmap, hud_icon.sequence_index);
                                button_icon.found = true;
                            }
                            catch(std::exception &e) {
                                button_icon.sprite_resolution = {0, 0};
                                button_icon.found = false;
                                logger.warning("Failed to get sprite size from buttons icons bitmap: {}", e.what());
                            }
                            
                            if(hud_icon.flags.override_default_color) {
                                button_icon.color_override = hud_icon.override_icon_color;
                            }
                        }
                    }
                }
            }
        }
        return &button_icon;
    }
    
    static bool map_changed() {
        auto &map = LegacyApi::Engine::get_map_header();
        if(last_map_name != map.name) {
            last_map_name = map.name;
            return true;
        }
        return false;
    }

    static ButtonIconCache *get_button_icon(HudHoldToActionMessageButton &button) {
        if(map_changed()) {
            button_icons.clear();
        }
        else {
            for(auto &button_icon : button_icons) {
                if(button_icon.button.index == button.index && button_icon.button.type == button.type && button_icon.button.axis_direction == button.axis_direction) {
                    return &button_icon;
                }
            }
        }
        return find_button_icon(button);
    }

    static bool draw_button_icon(LegacyApi::Engine::Point2DInt &offset, HudHoldToActionMessageButton button, LegacyApi::Engine::ColorARGBInt color) noexcept {
        auto *button_icon = get_button_icon(button);
        if(!button_icon || !button_icon->found) {
            return false;
        }

        // Update animation sprite
        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - button_icon->last_animation_frame).count();
        float frame_duration = 1000.0f / button_icon->sprites_count;
        if(elapsed_time > frame_duration) {
            if(elapsed_time > 1000) {
                button_icon->animation_current_sprite_index = 0;
            }
            else {
                button_icon->animation_current_sprite_index = (button_icon->animation_current_sprite_index + 1) % button_icon->sprites_count;
            }
            button_icon->last_animation_frame = std::chrono::steady_clock::now();
        }

        auto final_color = color;
        if(button_icon->color_override) {
            final_color = *button_icon->color_override;
        }

        // align position to text baseline and bump up the offset for the next slice
        auto position = offset;
        position.y += button_icon->sprite_resolution.height * 0.75f;
        offset.x += button_icon->sprite_resolution.width;

        // draw the sprite
        LegacyApi::Engine::draw_hud_message_sprite(button_icon->bitmap, button_icon->sequence_index, button_icon->animation_current_sprite_index, position, final_color);

        return true;
    }

    static void on_hud_hold_to_action_event(HudHoldForActionMessageEvent &event) {
        bool allow = true;
        if(event.time == EVENT_TIME_BEFORE) {
            static std::wstring left_quote;
            static std::wstring right_quote;

            auto slice = event.context.slice;
            if(slice == HudHoldForActionMessageSlice::BUTTON_NAME) {
                if(event.context.button->device == LegacyApi::Engine::INPUT_DEVICE_GAMEPAD) {
                    auto &button = *event.context.button;
                    auto &offset = event.context.offset;
                    auto &color = event.context.color;
                    bool success = draw_button_icon(offset, button, color);
                    allow = !success;
                }
                if(allow) {
                    event.context.text = left_quote + event.context.text + right_quote;
                    allow = true;
                }
            }
            else if(slice == HudHoldForActionMessageSlice::BUTTON_NAME_LEFT_QUOTE) {
                left_quote = event.context.text;
                allow = false;
            }
            else if(slice == HudHoldForActionMessageSlice::BUTTON_NAME_RIGHT_QUOTE) {
                right_quote = event.context.text;
                allow = false;
            }
        }
        if(!allow) {
            event.cancel();
        }
    }

    void set_up_hud_button_prompts() {
        HudHoldForActionMessageEvent::subscribe(on_hud_hold_to_action_event, EVENT_PRIORITY_HIGHEST);
    }
}
