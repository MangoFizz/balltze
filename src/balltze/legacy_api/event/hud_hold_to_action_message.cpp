// SPDX-License-Identifier: GPL-3.0-only

#include <stdexcept>
#include <balltze/utils.hpp>
#include <balltze/memory.hpp>
#include <balltze/api.hpp>
#include <balltze/hook.hpp>
#include <balltze/legacy_api/event.hpp>
#include "../../logger.hpp"

namespace Balltze::LegacyApi::Event {
    static std::wstring new_text;

    extern "C" {
        bool hold_for_action_hud_message_before_event_button_name();
        void hold_for_action_hud_message_after_event_button_name();
        bool hold_for_action_hud_message_before_event_button_name_left_quote();
        void hold_for_action_hud_message_after_event_button_name_left_quote();
        bool hold_for_action_hud_message_before_event_button_name_right_quote();
        void hold_for_action_hud_message_after_event_button_name_right_quote();
    
        bool dispatch_hold_for_action_hud_message_before_event(const wchar_t **text, HudHoldForActionMessageSliceOffset *offset, LegacyApi::Engine::ColorARGBInt color, HudHoldForActionMessageSlice slice) {
            HudHoldForActionMessageContext args(slice, {offset->x, offset->y}, color, *text, std::nullopt);
            HudHoldForActionMessageEvent event(EVENT_TIME_BEFORE, args);
            event.dispatch();
            offset->x = event.context.offset.x;
            offset->y = event.context.offset.y;
            if(event.context.text != *text) {
                new_text = event.context.text;
                *text = new_text.c_str();
            }
            return event.cancelled();
        }

        void dispatch_hold_for_action_hud_message_after_event(const wchar_t **text, LegacyApi::Engine::Point2DInt *offset, LegacyApi::Engine::ColorARGBInt color, HudHoldForActionMessageSlice slice) {
            HudHoldForActionMessageContext args(slice, {offset->x, offset->y}, color, *text, std::nullopt);
            HudHoldForActionMessageEvent event(EVENT_TIME_AFTER, args);
            event.dispatch();
        }

        bool dispatch_hold_for_action_hud_message_before_event_button_slice(const wchar_t **button_name, HudHoldForActionMessageSliceOffset *offset, LegacyApi::Engine::ColorARGBInt color, HudHoldToActionMessageButton *button) {
            HudHoldForActionMessageContext args(HudHoldForActionMessageSlice::BUTTON_NAME, {offset->x, offset->y}, color, *button_name, *button);
            HudHoldForActionMessageEvent event(EVENT_TIME_BEFORE, args);
            event.dispatch();
            offset->x = event.context.offset.x;
            offset->y = event.context.offset.y;
            if(event.context.text != *button_name) {
                new_text = event.context.text;
                *button_name = new_text.c_str();
            }
            return event.cancelled();
        }

        void dispatch_hold_for_action_hud_message_after_event_button_slice(const wchar_t **button_name, LegacyApi::Engine::Point2DInt *offset, LegacyApi::Engine::ColorARGBInt color, HudHoldToActionMessageButton *button) {
            HudHoldForActionMessageContext args(HudHoldForActionMessageSlice::BUTTON_NAME, {offset->x, offset->y}, color, *button_name, *button);
            HudHoldForActionMessageEvent event(EVENT_TIME_AFTER, args);
            event.dispatch();
        }
    }

    template<>
    void EventHandler<HudHoldForActionMessageEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        if(get_balltze_side() != BALLTZE_SIDE_CLIENT) {
            return;
        }

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

        try {
            std::function<bool()> dispatcher = hold_for_action_hud_message_before_event_button_name;
            auto *button_name_hook = Memory::hook_function(hold_for_weapon_hud_button_name_draw_sig->data(), dispatcher, hold_for_action_hud_message_after_event_button_name);

            dispatcher = hold_for_action_hud_message_before_event_button_name_left_quote;
            auto *left_quote_hook = Memory::hook_function(hold_for_action_message_left_quote_print_sig->data(), dispatcher, hold_for_action_hud_message_after_event_button_name_left_quote);

            dispatcher = hold_for_action_hud_message_before_event_button_name_right_quote;
            auto *right_quote_hook = Memory::hook_function(hold_for_action_message_right_quote_print_sig->data(), dispatcher, hold_for_action_hud_message_after_event_button_name_right_quote);
        }
        catch(std::runtime_error &e) {
            throw std::runtime_error("Could not hook hold for action hud message event: " + std::string(e.what()));
        }
    }
}
