// SPDX-License-Identifier: GPL-3.0-only

#include <stdexcept>
#include <balltze/output.hpp>
#include <balltze/memory.hpp>
#include <balltze/hook.hpp>
#include <balltze/event.hpp>

namespace Balltze::Event {
    extern "C" {
        bool hold_for_action_hud_message_before_event_button_name();
        void hold_for_action_hud_message_after_event_button_name();
        bool hold_for_action_hud_message_before_event_button_name_left_quote();
        void hold_for_action_hud_message_after_event_button_name_left_quote();
        bool hold_for_action_hud_message_before_event_button_name_right_quote();
        void hold_for_action_hud_message_after_event_button_name_right_quote();
    
        bool dispatch_hold_for_action_hud_message_before_event(const wchar_t *button_name, HudHoldForActionMessageSliceOffset *offset, HudHoldForActionMessageSlice slice, std::size_t button_index) {
            HudHoldForActionMessageArguments args;
            args.slice = slice;
            args.offset = {offset->x, offset->y};
            args.text = button_name;
            if(button_index != -1) {
                args.gamepad_pressed_button_index = button_index;
            }
            HudHoldForActionMessageEvent event(EVENT_TIME_BEFORE, args);
            event.dispatch();
            offset->x = event.args.offset.x;
            offset->y = event.args.offset.y;
            return event.cancelled();
        }

        void dispatch_hold_for_action_hud_message_after_event(const wchar_t *button_name, Engine::Point2DInt *offset, HudHoldForActionMessageSlice slice, std::size_t button_index) {
            HudHoldForActionMessageArguments args;
            args.slice = slice;
            args.offset = *offset;
            args.text = button_name;
            HudHoldForActionMessageEvent event(EVENT_TIME_AFTER, args);
            event.dispatch();
        }
    }

    template <>
    void EventHandler<HudHoldForActionMessageEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        static auto *hold_for_weapon_hud_button_name_draw_sig = Memory::get_signature("hold_for_weapon_hud_button_name_draw");
        if(!hold_for_weapon_hud_button_name_draw_sig) {
            throw std::runtime_error("Could not find signature for hold for action hud message button name draw");
        }
        
        static auto *hold_for_action_message_left_quote_print_sig = Memory::get_signature("hold_for_action_message_left_quote_print");
        if(!hold_for_action_message_left_quote_print_sig) {
            throw std::runtime_error("Could not find signature for hold for action hud message left quote print");
        }
        
        static auto *hold_for_action_message_right_quote_print_sig = Memory::get_signature("hold_for_action_message_right_quote_print");
        if(!hold_for_action_message_right_quote_print_sig) {
            throw std::runtime_error("Could not find signature for hold for action hud message right quote print");
        }

        std::function<bool()> dispatcher = hold_for_action_hud_message_before_event_button_name;
        auto *button_name_hook = Memory::hook_function(hold_for_weapon_hud_button_name_draw_sig->data(), dispatcher, hold_for_action_hud_message_after_event_button_name);

        dispatcher = hold_for_action_hud_message_before_event_button_name_left_quote;
        auto *left_quote_hook = Memory::hook_function(hold_for_action_message_left_quote_print_sig->data(), dispatcher, hold_for_action_hud_message_after_event_button_name_left_quote);

        dispatcher = hold_for_action_hud_message_before_event_button_name_right_quote;
        auto *right_quote_hook = Memory::hook_function(hold_for_action_message_right_quote_print_sig->data(), dispatcher, hold_for_action_hud_message_after_event_button_name_right_quote);
    }
}
