// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/event.hpp>
#include <balltze/hook.hpp>

namespace Balltze::Event {
    static std::uint32_t render_ui_param;

    extern "C" {
        bool ui_render_event_before_dispatcher_asm();
    
        bool ui_render_event_before_dispatcher(std::uint32_t unknown) {
            render_ui_param = unknown;
            UIRenderEventArgs args = {unknown};
            UIRenderEvent ui_render_event(EVENT_TIME_BEFORE, args);
            ui_render_event.dispatch();
            return ui_render_event.cancelled();
        }
    
        void ui_render_event_after_dispatcher() {
            UIRenderEventArgs args = {render_ui_param};
            UIRenderEvent ui_render_event(EVENT_TIME_AFTER, args);
            ui_render_event.dispatch();
        }
    }

    template<>
    void EventHandler<UIRenderEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        auto *render_ui_sig = Memory::get_signature("render_ui_function_call");
        if(!render_ui_sig) {
            throw std::runtime_error("Could not find signature for UI render event");
        }

        Memory::hook_function(render_ui_sig->data(), std::function<bool()>(ui_render_event_before_dispatcher_asm), ui_render_event_after_dispatcher);
    }

    static bool hud_render_event_before_dispatcher() {
        HUDRenderEvent hud_render_event(EVENT_TIME_BEFORE);
        hud_render_event.dispatch();
        return hud_render_event.cancelled();
    }

    static void hud_render_event_after_dispatcher() {
        HUDRenderEvent hud_render_event(EVENT_TIME_AFTER);
        hud_render_event.dispatch();
    }

    template<>
    void EventHandler<HUDRenderEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        auto *render_hud_sig = Memory::get_signature("render_hud_function_call");
        if(!render_hud_sig) {
            throw std::runtime_error("Could not find signature for HUD render event");
        }

        Memory::hook_function(render_hud_sig->data(), std::function<bool()>(hud_render_event_before_dispatcher), hud_render_event_after_dispatcher);
    }

    static bool post_carnage_report_render_event_before_dispatcher() {
        PostCarnageReportRenderEvent post_carnage_report_render_event(EVENT_TIME_BEFORE);
        post_carnage_report_render_event.dispatch();
        return post_carnage_report_render_event.cancelled();
    }

    static void post_carnage_report_render_event_after_dispatcher() {
        PostCarnageReportRenderEvent post_carnage_report_render_event(EVENT_TIME_AFTER);
        post_carnage_report_render_event.dispatch();
    }

    template<>
    void EventHandler<PostCarnageReportRenderEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        auto *render_post_carnage_report_call_sig = Memory::get_signature("render_post_carnage_report_call");
        if(!render_post_carnage_report_call_sig) {
            throw std::runtime_error("Could not find signature for post carnage report render event");
        }

        Memory::hook_function(render_post_carnage_report_call_sig->data(), std::function<bool()>(post_carnage_report_render_event_before_dispatcher), post_carnage_report_render_event_after_dispatcher);
    }
}
