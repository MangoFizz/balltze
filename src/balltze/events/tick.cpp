// SPDX-License-Identifier: GPL-3.0-only

#include <iostream>
#include <balltze/memory/hook.hpp>
#include <balltze/balltze.hpp>
#include <balltze/events/tick.hpp>

namespace Balltze {
    static Memory::Hook on_tick_hook;
    static std::vector<Event<event_no_args_t>> tick_events;

    void add_tick_event(event_no_args_t event_function, EventPriority priority) {
        for(std::size_t i = 0; i < tick_events.size(); i++) {
            if(tick_events[i].function == event_function) {
                return;
            }
        }
        tick_events.emplace_back(event_function, priority);
    }

    void remove_tick_event(event_no_args_t event_function) {
        for(std::size_t i = 0; i < tick_events.size(); i++) {
            if(tick_events[i].function == event_function) {
                tick_events.erase(tick_events.begin() + i);
                return;
            }
        }
    }

    static void call_tick_events() noexcept {
        call_in_order(tick_events);
    }

    void set_up_tick_event() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        // Get signature
        static auto &balltze = Balltze::get();
        static auto &sig_manager = balltze.signature_manager();
        static auto *on_tick_sig = sig_manager.get("on_tick");

        if(!on_tick_sig) {
            std::cerr << "Failed to set up tick event" << std::endl;
            return;
        }
        
        // Write hacks
        on_tick_hook.initialize(on_tick_sig->data(), reinterpret_cast<void *>(call_tick_events));
        on_tick_hook.hook();
    }
}
