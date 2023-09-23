// SPDX-License-Identifier: GPL-3.0-only

#include <chrono>
#include <balltze/engine/core.hpp>
#include <balltze/event.hpp>
#include <balltze/command.hpp>
#include <balltze/memory.hpp>
#include <balltze/hook.hpp>
#include "../logger.hpp"

namespace Balltze::Event {
    static bool first_tick = true;
    static std::chrono::time_point<std::chrono::steady_clock> last_tick;
    static std::chrono::milliseconds tick_duration = std::chrono::milliseconds(0);

    static void tick_event_before_dispatcher() {
        auto tick_count = Engine::get_tick_count();
        TickEventArguments args;
        args.delta_time_ms = tick_duration.count();
        args.tick_count = tick_count;
        TickEvent tick_event(EVENT_TIME_BEFORE, args);
        tick_event.dispatch();
    }

    static void tick_event_after_dispatcher() {
        auto tick_count = Engine::get_tick_count();
        auto current_tick = std::chrono::steady_clock::now();
        if(!first_tick) {
            tick_duration = std::chrono::duration_cast<std::chrono::milliseconds>(current_tick - last_tick);
        }
        else {
            first_tick = false;
        }
        last_tick = current_tick;
        TickEventArguments args;
        args.delta_time_ms = tick_duration.count();
        args.tick_count = tick_count;
        TickEvent tick_event(EVENT_TIME_AFTER, args);
        tick_event.dispatch();
    }

    static bool debug_tick_event(int arg_count, const char **args) {
        static std::optional<Event::EventListenerHandle<TickEvent>> handle;
        if(arg_count == 1) {
            bool new_setting = STR_TO_BOOL(args[0]);
            if(new_setting) {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
                handle = Event::TickEvent::subscribe_const([](TickEvent const &event) {
                    auto &arguments = event.args;
                    auto time = event_time_to_string(event.time);
                    logger.debug("Tick event ({}): delta time: {}, tick count: {}", time, arguments.delta_time_ms, arguments.tick_count);
                });
            }
            else {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
            }
        }
        logger.info("debug_tick_event: {}", handle.has_value());
        return true;
    }

    template<>
    void EventHandler<TickEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        static auto *tick_event_sig = Memory::get_signature("on_tick");
        if(!tick_event_sig) {
            throw std::runtime_error("Could not find signature for tick event");
        }

        // Workaround for Chimera hook (NEEDS TO BE FIXED)
        std::byte *ptr = Memory::follow_32bit_jump(tick_event_sig->data()) + 23;
        auto *tick_event_after_chimera_hook = Memory::hook_function(ptr, tick_event_after_dispatcher);
        auto *tick_event_hook = Memory::hook_function(tick_event_sig->data(), tick_event_before_dispatcher);

        // Register debug command
        register_command("debug_tick_event", "debug", "Sets whenever to log tick event.", "[enable: boolean]", debug_tick_event, true, 0, 1);
    }
}