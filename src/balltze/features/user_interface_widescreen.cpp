// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/engine.hpp>
#include <balltze/event.hpp>
#include <balltze/hook.hpp>
#include "../logger.hpp"
#include <balltze/features/user_interface.hpp>

namespace Balltze::Features {
    using namespace Event;
    using namespace Engine;

    static bool enabled = false;
    static bool ready = false;
    static float screen_width_480p = 0;
    static float ui_aspect_ratio = 0;
    static float ui_displacement = 0;

    static Memory::Hook *widescreen_ui_text_hook;
    static Memory::Hook *widescreen_ui_text_2_hook;
    static Memory::Hook *widescreen_input_text_hook;
    static Memory::Hook *widescreen_mouse_hook;

    static EventListenerHandle<TickEvent> tick_listener_handler;
    static EventListenerHandle<MapLoadEvent> map_load_listener_handler;
    static EventListenerHandle<UIWidgetBackgroundRenderEvent> ui_widget_background_render_listener_handler;

    static void on_tick(TickEvent const &ev);
    static void on_map_load(MapLoadEvent const &ev);
    static void widescreen_displace_ui_widgets(UIWidgetBackgroundRenderEvent &ev);
    static void jason_jones_widgets() noexcept;

    extern "C" {
        Rectangle2D *widescreen_text_input_element;
        std::int32_t widescreen_mouse_left_bound = 0;
        std::int32_t widescreen_mouse_right_bound = 640;
        std::int32_t *widescreen_mouse_x = nullptr;
        std::int32_t **widescreen_fix_mouse_x_ptr = nullptr;
        std::int32_t *widescreen_mouse_y = nullptr;
        
        void ui_widget_render_text_asm();

        void widescreen_input_text();
        void widescreen_input_text_undo();

        bool widescreen_mouse();
    }

    void enable_widescreen_override(bool setting) noexcept {
        if(ready) {
            if(setting) {
                // Set mouse bounds
                widescreen_mouse_x = *widescreen_fix_mouse_x_ptr;
                widescreen_mouse_y = widescreen_mouse_x + 1;

                // Redirect widescreen fix pointer to a fake variable
                static std::int32_t gotya = 0;
                *widescreen_fix_mouse_x_ptr = &gotya;

                // Hook everything
                widescreen_ui_text_hook->hook();
                widescreen_ui_text_2_hook->hook();
                widescreen_input_text_hook->hook();
                widescreen_mouse_hook->hook();

                // Register events
                tick_listener_handler = TickEvent::subscribe_const(on_tick);
                map_load_listener_handler = MapLoadEvent::subscribe_const(on_map_load);
                ui_widget_background_render_listener_handler = UIWidgetBackgroundRenderEvent::subscribe(widescreen_displace_ui_widgets);

                // Stretch stock widgets
                jason_jones_widgets();

                // Set default aspect ratio
                if(ui_aspect_ratio == 0) {
                    set_ui_aspect_ratio(4, 3);
                }
            }
            else {
                // Remove events
                tick_listener_handler.remove();
                map_load_listener_handler.remove();
                ui_widget_background_render_listener_handler.remove();

                // Restore widescreen fix mouse position pointer
                *widescreen_fix_mouse_x_ptr = widescreen_mouse_x;
                widescreen_mouse_x = nullptr;

                // Release all hooks
                widescreen_ui_text_hook->release();
                widescreen_ui_text_2_hook->release();
                widescreen_input_text_hook->release();
                widescreen_mouse_hook->release();
            }
        }        
        enabled = setting;
    }

    float get_ui_aspect_ratio() noexcept {
        return ui_aspect_ratio;
    }

    void set_ui_aspect_ratio(std::uint16_t x, std::uint16_t y) noexcept {
        ui_aspect_ratio = static_cast<float>(x) / static_cast<float>(y);
        screen_width_480p = 0;
    }

    void reset_ui_aspect_ratio() noexcept {
        ui_aspect_ratio = static_cast<float>(4) / static_cast<float>(3);
        screen_width_480p = 0;
    }

    std::pair<std::uint16_t, std::uint16_t> get_mouse_horizontal_bounds() noexcept {
        return std::make_pair(widescreen_mouse_left_bound, widescreen_mouse_right_bound);
    }

    void set_up_ui_widescreen_override() noexcept {
        if(ready) {
            return;
        }

        static EventListenerHandle<TickEvent> first_tick;

        first_tick = TickEvent::subscribe_const([](TickEvent const &ev) {
            if(ev.time == EVENT_TIME_BEFORE) {
                try {
                    auto *widescreen_ui_text_sig = Memory::get_signature("widescreen_ui_text");
                    widescreen_ui_text_hook = Memory::hook_function(widescreen_ui_text_sig->data(), ui_widget_render_text_asm, std::nullopt, false, true);
                    
                    auto *widescreen_ui_text_2_sig = Memory::get_signature("widescreen_ui_text_2");
                    widescreen_ui_text_2_hook = Memory::hook_function(widescreen_ui_text_2_sig->data(), ui_widget_render_text_asm, std::nullopt, false, true);
                    
                    auto *widescreen_input_text_sig = Memory::get_signature("widescreen_input_text");
                    auto *widescreen_input_text_address = Memory::follow_32bit_jump(widescreen_input_text_sig->data()) + 9;
                    widescreen_input_text_hook = Memory::hook_function(widescreen_input_text_address, widescreen_input_text, widescreen_input_text_undo, true, true);

                    // Get mouse bounds
                    auto *widescreen_mouse_sig = Memory::get_signature("widescreen_mouse");
                    auto *widescreen_mouse_address = widescreen_mouse_sig->data();
                    auto *original_cave = Memory::follow_32bit_jump(widescreen_mouse_address);
                    auto *original_widescreen_mouse_fn = Memory::follow_32bit_jump(original_cave);
                    widescreen_fix_mouse_x_ptr = *reinterpret_cast<std::int32_t ***>(original_widescreen_mouse_fn + 0x28);

                    std::function<bool ()> widescreen_mouse_func = widescreen_mouse;
                    widescreen_mouse_hook = Memory::hook_function(widescreen_mouse_address, widescreen_mouse_func, std::nullopt, false, true);

                    ready = true;

                    // If enabled, enable it again
                    enable_widescreen_override(true);
                }
                catch(std::runtime_error &e) {
                    logger.error("Failed to set up widescreen override: {}", e.what());
                    return;
                }
                first_tick.remove();
            }
        });
    }

    static void jason_jones_widgets() noexcept {
        auto stretch_widget = [](const char *widget_path) noexcept {
            auto *tag = get_tag(widget_path, TAG_CLASS_UI_WIDGET_DEFINITION);
            if(tag) {
                auto *data = reinterpret_cast<TagDefinitions::UiWidgetDefinition *>(tag->data);
                auto &bounds = data->bounds;
                if(bounds.left == 0.0f && bounds.right == 640.0f) {
                    bounds.right = 639.0f;
                }
            }
        };

        stretch_widget("ui\\shell\\main_menu\\halo_logo");
        stretch_widget("ui\\shell\\main_menu\\multiplayer_type_select\\join_game\\join_game_items_list");
        stretch_widget("ui\\shell\\main_menu\\settings_select\\player_setup\\player_profile_edit\\controls_setup\\controls_options_menu");
        stretch_widget("ui\\shell\\main_menu\\settings_select\\player_setup\\player_profile_edit\\gamepad_setup\\gamepad_setup_options");
    }

    static void on_tick(TickEvent const &ev) {
        if(ev.time == EVENT_TIME_BEFORE) {
            static auto &resolution = get_resolution();
            float current_aspect_ratio = static_cast<float>(resolution.width) / static_cast<float>(resolution.height);
            float current_screen_width = current_aspect_ratio * 480.000f;

            if(static_cast<std::uint16_t>(screen_width_480p) != static_cast<std::uint16_t>(current_screen_width)) {
                screen_width_480p = current_screen_width;

                /**
                 * Calculate how much we need to displace back widgets with respect to the 
                 * displacement that will be applied by the Chimera's widescreen fix.
                 */
                float ui_frame_width = ui_aspect_ratio * 480.000f;
                float screen_extra_width = screen_width_480p - 640.000f;
                float ui_frame_offset = (screen_width_480p - ui_frame_width) / 2.0f;
                ui_displacement = ui_frame_offset - screen_extra_width / 2.0f;

                // Calculate mouse cursor bounds
                std::int32_t mouse_increase = static_cast<std::int32_t>(screen_width_480p - ui_frame_width) / 2;
                widescreen_mouse_left_bound = -mouse_increase;
                widescreen_mouse_right_bound = ui_frame_width + mouse_increase;

                if(*widescreen_mouse_x > ui_frame_width + mouse_increase) {
                    *widescreen_mouse_x = widescreen_mouse_right_bound;
                }
                if(*widescreen_mouse_x < -mouse_increase) {
                    *widescreen_mouse_x = -mouse_increase;
                }
            }
        }
    }

    static void on_map_load(MapLoadEvent const &ev) {
        if(ev.time == EVENT_TIME_AFTER) {
            jason_jones_widgets();
        }
    }

    static void widescreen_displace_ui_widgets(UIWidgetBackgroundRenderEvent &ev) {
        if(ev.time == EVENT_TIME_BEFORE) {
            auto &vertices = ev.args.vertices;
            float min_x = vertices->top_left.x;
            float max_x = vertices->top_right.x;
            float min_y = vertices->top_left.y;
            float max_y = vertices->bottom_left.y;

            /**
             * These widgets will be stretched by the widescreen fix
             */
            if(min_x <= 1.0f && max_x >= 639.0f && min_y <= 1.0F && max_y >= 479.0f) {
                return;
            }

            if(min_x == 0.0f && max_x == 640.0f) {
                // Revert widescreen fix displacement
                float screen_extra_width = screen_width_480p - 640.000f;
                vertices->top_right.x -= screen_extra_width / 2.0f;
                vertices->bottom_right.x -= screen_extra_width / 2.0f;
                vertices->top_left.x -= screen_extra_width / 2.0f;
                vertices->bottom_left.x -= screen_extra_width / 2.0f;

                // Stretch it
                vertices->top_right.x += screen_extra_width;
                vertices->bottom_right.x += screen_extra_width;
            }
            else {
                vertices->top_left.x += ui_displacement;
                vertices->top_right.x += ui_displacement;
                vertices->bottom_right.x += ui_displacement;
                vertices->bottom_left.x += ui_displacement;
            }
        }
    }

    extern "C" void widescreen_displace_ui_text(Rectangle2D *text_box_bounds) noexcept {
        text_box_bounds->right += ui_displacement;
        text_box_bounds->left += ui_displacement;
    }

    extern "C" void reposition_ui_text_input() noexcept {
        widescreen_text_input_element->right -= ui_displacement;
        widescreen_text_input_element->left -= ui_displacement;
    }

    extern "C" void unreposition_ui_text_input() noexcept {
        widescreen_text_input_element->right += ui_displacement;
        widescreen_text_input_element->left += ui_displacement;
    }
}
