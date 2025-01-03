// SPDX-License-Identifier: GPL-3.0-only

#include <chrono>
#include <balltze/engine/core.hpp>
#include <balltze/event.hpp>
#include <balltze/command.hpp>
#include <balltze/memory.hpp>
#include <balltze/hook.hpp>
#include <event/events.h>
#include <impl/interface/ui_widget.h>
#include "../logger.hpp"

namespace Balltze::Event {
    static void dispatch_widget_create_event(Engine::Widget *widget) {
        UIWidgetCreateEventContext args{reinterpret_cast<Engine::Widget *>(widget), widget->definition_tag_handle.value, widget->parent_widget == nullptr, widget->parent_widget};
        UIWidgetCreateEvent widget_create_event_before(EVENT_TIME_BEFORE, args);
        widget_create_event_before.dispatch();
        UIWidgetCreateEvent widget_create_event_after(EVENT_TIME_AFTER, args);
        widget_create_event_after.dispatch();
    }

    static bool debug_widget_create_event(int arg_count, const char **args) {
        static std::optional<Event::EventListenerHandle<UIWidgetCreateEvent>> handle;
        if(arg_count == 1) {
            bool new_setting = STR_TO_BOOL(args[0]);
            if(new_setting) {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
                handle = Event::UIWidgetCreateEvent::subscribe_const([](UIWidgetCreateEvent const &event) {
                    auto &context = event.context;
                    auto time = event_time_to_string(event.time);
                    auto *tag = Engine::get_tag(context.definition_tag_handle);
                    logger.debug("Widget create event ({}): widget: {}; is root: {}", time, tag->path, context.is_root_widget);
                });
            }
            else {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
            }
        }
        logger.info("debug_widget_create_event: {}", handle.has_value());
        return true;
    }

    template<>
    void EventHandler<UIWidgetCreateEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        if(get_balltze_side() != BALLTZE_SIDE_CLIENT) {
            return;
        }

        ringworld_event_subscribe(RW_EVENT_WIDGET_LOADED, reinterpret_cast<RingworldEventCallback>(dispatch_widget_create_event));

        register_command("debug_widget_create_event", "debug", "Sets whenever to log widget open event.", "[enable: boolean]", debug_widget_create_event, true, 0, 1);
    }

    extern "C" {
        void widget_back_event_asm();
        void *widget_back_function_override_return;

        bool dispatch_widget_back_before_event(Engine::Widget *widget) {
            UIWidgetEventContext args{widget};
            UIWidgetBackEvent event(EVENT_TIME_BEFORE, args);
            event.dispatch();
            return event.cancelled();
        }

        void dispatch_widget_back_after_event(Engine::Widget *widget) {
            UIWidgetEventContext args{widget};
            UIWidgetBackEvent event(EVENT_TIME_AFTER, args);
            event.dispatch();
        }
    }

    static bool debug_widget_back_event(int arg_count, const char **args) {
        static std::optional<Event::EventListenerHandle<UIWidgetBackEvent>> handle;
        if(arg_count == 1) {
            bool new_setting = STR_TO_BOOL(args[0]);
            if(new_setting) {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
                handle = Event::UIWidgetBackEvent::subscribe_const([](UIWidgetBackEvent const &event) {
                    auto &context = event.context;
                    auto time = event_time_to_string(event.time);
                    auto *tag = Engine::get_tag(context.widget->definition_tag_handle);
                    logger.debug("Widget close event ({}): widget: {}", time, tag->path);
                });
            }
            else {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
            }
        }
        logger.info("debug_widget_back_event: {}", handle.has_value());
        return true;
    }

    template<>
    void EventHandler<UIWidgetBackEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        if(get_balltze_side() != BALLTZE_SIDE_CLIENT) {
            return;
        }

        auto *widget_back_function_sig = Memory::get_signature("widget_back_function");
        if(!widget_back_function_sig) {
            throw std::runtime_error("Could not find signatures for widget close event.");
        }

        try {
            Memory::override_function(widget_back_function_sig->data(), widget_back_event_asm, widget_back_function_override_return);
        }
        catch(const std::runtime_error &e) {
            throw std::runtime_error("failed to initialize widget close event: " + std::string(e.what()));
        }

        // Register debug command
        register_command("debug_widget_back_event", "debug", "Sets whenever to log widget close event.", "[enable: boolean]", debug_widget_back_event, true, 0, 1);
    }

    extern "C" {
        void widget_focus_event_asm();
        bool widget_mouse_focus_update_before_asm();
        void widget_mouse_focus_update_after_asm();
        void *widget_focus_function_override_return;

        bool dispatch_widget_focus_before_event(Engine::Widget *widget) {
            UIWidgetEventContext args{widget};
            UIWidgetFocusEvent event(EVENT_TIME_BEFORE, args);
            event.dispatch();
            return event.cancelled();
        }

        void dispatch_widget_focus_after_event(Engine::Widget *widget) {
            UIWidgetEventContext args{widget};
            UIWidgetFocusEvent event(EVENT_TIME_AFTER, args);
            event.dispatch();
        }
    }

    static bool debug_widget_focus_event(int arg_count, const char **args) {
        static std::optional<Event::EventListenerHandle<UIWidgetFocusEvent>> handle;
        if(arg_count == 1) {
            bool new_setting = STR_TO_BOOL(args[0]);
            if(new_setting) {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
                handle = Event::UIWidgetFocusEvent::subscribe_const([](UIWidgetFocusEvent const &event) {
                    auto &context = event.context;
                    auto time = event_time_to_string(event.time);
                    auto *tag = Engine::get_tag(context.widget->definition_tag_handle);
                    logger.debug("Widget focus event ({}): widget: {}", time, tag->path);
                });
            }
            else {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
            }
        }
        logger.info("debug_widget_focus_event: {}", handle.has_value());
        return true;
    }

    template<>
    void EventHandler<UIWidgetFocusEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        if(get_balltze_side() != BALLTZE_SIDE_CLIENT) {
            return;
        }

        auto *widget_focus_function_sig = Memory::get_signature("widget_focus_function");
        auto *widget_mouse_focus_update_sig = Memory::get_signature("widget_mouse_focus_update");
        if(!widget_focus_function_sig || !widget_mouse_focus_update_sig) {
            throw std::runtime_error("Could not find signatures for widget focus event.");
        }

        try {
            Memory::override_function(widget_focus_function_sig->data(), widget_focus_event_asm, widget_focus_function_override_return);
            std::function<bool()> mouse_focus_update_before = widget_mouse_focus_update_before_asm;
            Memory::hook_function(widget_mouse_focus_update_sig->data(), mouse_focus_update_before, widget_mouse_focus_update_after_asm, true);
        }
        catch(const std::runtime_error &e) {
            throw std::runtime_error("failed to initialize widget focus event: " + std::string(e.what()));
        }

        // Register debug command
        register_command("debug_widget_focus_event", "debug", "Sets whenever to log widget focus event.", "[enable: boolean]", debug_widget_focus_event, true, 0, 1);
    }

    extern "C" {
        void widget_accept_event_asm();

        bool dispatch_widget_accept_before_event(Engine::Widget *widget) {
            UIWidgetEventContext args{widget};
            UIWidgetAcceptEvent event(EVENT_TIME_BEFORE, args);
            event.dispatch();
            return event.cancelled();
        }

        void dispatch_widget_accept_after_event(Engine::Widget *widget) {
            UIWidgetEventContext args{widget};
            UIWidgetAcceptEvent event(EVENT_TIME_AFTER, args);
            event.dispatch();
        }
    }

    static bool debug_widget_accept_event(int arg_count, const char **args) {
        static std::optional<Event::EventListenerHandle<UIWidgetAcceptEvent>> handle;
        if(arg_count == 1) {
            bool new_setting = STR_TO_BOOL(args[0]);
            if(new_setting) {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
                handle = Event::UIWidgetAcceptEvent::subscribe_const([](UIWidgetAcceptEvent const &event) {
                    auto &context = event.context;
                    auto time = event_time_to_string(event.time);
                    auto *tag = Engine::get_tag(context.widget->definition_tag_handle);
                    logger.debug("Widget accept event ({}): widget: {}", time, tag->path);
                });
            }
            else {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
            }
        }
        logger.info("debug_widget_accept_event: {}", handle.has_value());
        return true;
    }

    template<>
    void EventHandler<UIWidgetAcceptEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        if(get_balltze_side() != BALLTZE_SIDE_CLIENT) {
            return;
        }

        auto *widget_accept_function_sig = Memory::get_signature("widget_accept_event_check");
        if(!widget_accept_function_sig) {
            throw std::runtime_error("Could not find signatures for widget accept event.");
        }

        try {
            Memory::hook_function(widget_accept_function_sig->data(), std::nullopt, widget_accept_event_asm, false);
        }
        catch(const std::runtime_error &e) {
            throw std::runtime_error("failed to initialize widget accept event: " + std::string(e.what()));
        }

        // Register debug command
        register_command("debug_widget_accept_event", "debug", "Sets whenever to log widget accept event.", "[enable: boolean]", debug_widget_accept_event, true, 0, 1);
    }

    extern "C" {
        void widget_sound_event_asm();
        void *widget_sound_function_override_return;

        bool dispatch_widget_sound_before_event(Engine::WidgetNavigationSound sound) {
            UIWidgetSoundEventContext args{sound};
            UIWidgetSoundEvent event(EVENT_TIME_BEFORE, args);
            event.dispatch();
            return event.cancelled();
        }

        void dispatch_widget_sound_after_event(Engine::WidgetNavigationSound sound) {
            UIWidgetSoundEventContext args{sound};
            UIWidgetSoundEvent event(EVENT_TIME_AFTER, args);
            event.dispatch();
        }
    }

    static bool debug_widget_sound_event(int arg_count, const char **args) {
        static std::optional<Event::EventListenerHandle<UIWidgetSoundEvent>> handle;
        if(arg_count == 1) {
            bool new_setting = STR_TO_BOOL(args[0]);
            if(new_setting) {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
                handle = Event::UIWidgetSoundEvent::subscribe_const([](UIWidgetSoundEvent const &event) {
                    auto &context = event.context;
                    auto time = event_time_to_string(event.time);
                    logger.debug("Widget sound event ({}): sound: {}", time, static_cast<std::uint32_t>(context.sound));
                });
            }
            else {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
            }
        }
        logger.info("debug_widget_sound_event: {}", handle.has_value());
        return true;
    }

    template<>
    void EventHandler<UIWidgetSoundEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        if(get_balltze_side() != BALLTZE_SIDE_CLIENT) {
            return;
        }

        auto *widget_sound_function_sig = Memory::get_signature("widget_sound_play_function");
        if(!widget_sound_function_sig) {
            throw std::runtime_error("Could not find signatures for widget sound event.");
        }

        try {
            Memory::override_function(widget_sound_function_sig->data(), widget_sound_event_asm, widget_sound_function_override_return);
        }
        catch(const std::runtime_error &e) {
            throw std::runtime_error("failed to initialize widget sound event: " + std::string(e.what()));
        }

        // Register debug command
        register_command("debug_widget_sound_event", "debug", "Sets whenever to log widget sound event.", "[enable: boolean]", debug_widget_sound_event, true, 0, 1);
    }

    extern "C" {
        void widget_tab_list_items_next_vertical_event_before_asm();
        void widget_tab_list_items_next_vertical_event_after_asm();
        void widget_tab_list_items_next_horizontal_event_before_asm();
        void widget_tab_list_items_next_horizontal_event_after_asm();
        void widget_tab_list_items_previous_vertical_event_before_asm();
        void widget_tab_list_items_previous_vertical_event_after_asm();
        void widget_tab_list_items_previous_horizontal_event_before_asm();
        void widget_tab_list_items_previous_horizontal_event_after_asm();
        void widget_tab_children_next_vertical_event_before_asm();
        void widget_tab_children_next_vertical_event_after_asm();
        void widget_tab_children_next_horizontal_event_before_asm();
        void widget_tab_children_next_horizontal_event_after_asm();
        void widget_tab_children_previous_event_before_asm();
        void widget_tab_children_previous_event_after_asm();

        bool dispatch_widget_list_tab_before_event(Engine::Widget *widget_list, UIWidgetListTabType tab_type) {
            UIWidgetListTabEventContext args{widget_list, tab_type};
            UIWidgetListTabEvent event(EVENT_TIME_BEFORE, args);
            event.dispatch();
            return event.cancelled();
        }

        void dispatch_widget_list_tab_after_event(Engine::Widget *widget_list, UIWidgetListTabType tab_type) {
            UIWidgetListTabEventContext args{widget_list, tab_type};
            UIWidgetListTabEvent event(EVENT_TIME_AFTER, args);
            event.dispatch();
        }
    }

    static bool debug_widget_list_tab_event(int arg_count, const char **args) {
        static std::optional<Event::EventListenerHandle<UIWidgetListTabEvent>> handle;
        if(arg_count == 1) {
            bool new_setting = STR_TO_BOOL(args[0]);
            if(new_setting) {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
                handle = Event::UIWidgetListTabEvent::subscribe_const([](UIWidgetListTabEvent const &event) {
                    auto &context = event.context;
                    auto time = event_time_to_string(event.time);
                    auto *tag = Engine::get_tag(context.widget_list->definition_tag_handle);
                    logger.debug("Widget list tab event ({}): widget: {}. tab type: {}", time, tag->path, static_cast<std::uint32_t>(context.tab));
                });
            }
            else {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
            }
        }
        logger.info("debug_widget_list_tab_event: {}", handle.has_value());
        return true;
    }

    template<>
    void EventHandler<UIWidgetListTabEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        if(get_balltze_side() != BALLTZE_SIDE_CLIENT) {
            return;
        }

        #define HOOK_FUNCTION(sig_name, event_before, event_after) \
            auto *sig_name##_sig = Memory::get_signature(#sig_name); \
            if(!sig_name##_sig) { \
                throw std::runtime_error("Could not find signatures for widget list tab event."); \
            } \
            try { \
                Memory::hook_function(sig_name##_sig->data(), event_before, event_after); \
            } \
            catch(const std::runtime_error &e) { \
                throw std::runtime_error("failed to initialize widget list tab event: " + std::string(e.what())); \
            }

        HOOK_FUNCTION(widget_tab_list_items_next_vertical_call, widget_tab_list_items_next_vertical_event_before_asm, widget_tab_list_items_next_vertical_event_after_asm);
        HOOK_FUNCTION(widget_tab_list_items_next_horizontal_call, widget_tab_list_items_next_horizontal_event_before_asm, widget_tab_list_items_next_horizontal_event_after_asm);
        HOOK_FUNCTION(widget_tab_list_items_previous_vertical_call, widget_tab_list_items_previous_vertical_event_before_asm, widget_tab_list_items_previous_vertical_event_after_asm);
        HOOK_FUNCTION(widget_tab_list_items_previous_horizontal_call, widget_tab_list_items_previous_horizontal_event_before_asm, widget_tab_list_items_previous_horizontal_event_after_asm);
        HOOK_FUNCTION(widget_tab_children_next_vertical_call, widget_tab_children_next_vertical_event_before_asm, widget_tab_children_next_vertical_event_after_asm);
        HOOK_FUNCTION(widget_tab_children_next_horizontal_call, widget_tab_children_next_horizontal_event_before_asm, widget_tab_children_next_horizontal_event_after_asm);
        HOOK_FUNCTION(widget_tab_children_previous_call, widget_tab_children_previous_event_before_asm, widget_tab_children_previous_event_after_asm);

        #undef HOOK_FUNCTION

        // Register debug command
        register_command("debug_widget_list_tab_event", "debug", "Sets whenever to log widget list tab event.", "[enable: boolean]", debug_widget_list_tab_event, true, 0, 1);
    }

    extern "C" {
        void handle_widget_mouse_button_press_asm();

        bool call_widget_mouse_button_press_events(Engine::Widget *pressed_widget, Engine::MouseButton button) {
            UIWidgetMouseButtonPressEventContext args{pressed_widget, button};
            UIWidgetMouseButtonPressEvent event(EVENT_TIME_BEFORE, args);
            event.dispatch();
            return !event.cancelled();
        }
    }

    static bool debug_widget_mouse_button_press_event(int arg_count, const char **args) {
        static std::optional<Event::EventListenerHandle<UIWidgetMouseButtonPressEvent>> handle;
        if(arg_count == 1) {
            bool new_setting = STR_TO_BOOL(args[0]);
            if(new_setting) {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
                handle = Event::UIWidgetMouseButtonPressEvent::subscribe_const([](UIWidgetMouseButtonPressEvent const &event) {
                    auto &context = event.context;
                    auto time = event_time_to_string(event.time);
                    auto *tag = Engine::get_tag(context.widget->definition_tag_handle);
                    logger.debug("Widget mouse button event ({}): widget: {}. button: {}", time, tag->path, static_cast<std::uint32_t>(context.button));
                });
            }
            else {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
            }
        }
        logger.info("debug_widget_mouse_button_click_event: {}", handle.has_value());
        return true;
    }

    template<>
    void EventHandler<UIWidgetMouseButtonPressEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        if(get_balltze_side() != BALLTZE_SIDE_CLIENT) {
            return;
        }

        auto *widget_mbc_function_sig = Memory::get_signature("widget_mouse_pressed_button_check");
        if(!widget_mbc_function_sig) {
            throw std::runtime_error("Could not find signatures for widget mouse button click event.");
        }

        try {
            Memory::fill_with_nops(reinterpret_cast<void *>(widget_mbc_function_sig->data()), 0x1E);
            Memory::replace_function_call(widget_mbc_function_sig->data(), handle_widget_mouse_button_press_asm);
        }
        catch(const std::runtime_error &e) {
            throw std::runtime_error("failed to initialize widget sound event: " + std::string(e.what()));
        }

        // Register debug command
        register_command("debug_widget_mouse_button_click_event", "debug", "Sets whenever to log widget mouse button click event.", "[enable: boolean]", debug_widget_mouse_button_press_event, true, 0, 1);
    }
}