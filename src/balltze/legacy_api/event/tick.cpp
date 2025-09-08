// SPDX-License-Identifier: GPL-3.0-only

#include <chrono>
#include <balltze/legacy_api/engine/core.hpp>
#include <balltze/legacy_api/event.hpp>
#include <balltze/command.hpp>
#include <balltze/memory.hpp>
#include <balltze/hook.hpp>
#include "../../logger.hpp"

namespace Balltze::LegacyApi::Event {
    static bool first_tick = true;
    static std::chrono::time_point<std::chrono::steady_clock> last_tick;
    static std::chrono::milliseconds tick_duration = std::chrono::milliseconds(0);

    static void tick_event_before_dispatcher() {
        auto tick_count = LegacyApi::Engine::get_tick_count();
        TickEventContext args(tick_duration.count(), tick_count);
        TickEvent tick_event(EVENT_TIME_BEFORE, args);
        tick_event.dispatch();
    }

    static void tick_event_after_dispatcher() {
        auto tick_count = LegacyApi::Engine::get_tick_count();
        auto current_tick = std::chrono::steady_clock::now();
        if(!first_tick) {
            tick_duration = std::chrono::duration_cast<std::chrono::milliseconds>(current_tick - last_tick);
        }
        else {
            first_tick = false;
        }
        last_tick = current_tick;
        TickEventContext args(tick_duration.count(), tick_count);
        TickEvent tick_event(EVENT_TIME_AFTER, args);
        tick_event.dispatch();
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

        try {
            std::uint8_t instruction_byte = *reinterpret_cast<std::uint8_t *>(tick_event_sig->data());
            if(instruction_byte == 0xE8) {
                Memory::hook_function(tick_event_sig->data(), tick_event_before_dispatcher, tick_event_after_dispatcher);
            }
            else {
                std::byte *ptr = Memory::follow_32bit_jump(tick_event_sig->data()) + 23;
                auto *tick_event_after_chimera_hook = Memory::hook_function(ptr, tick_event_after_dispatcher);
                Memory::hook_function(tick_event_sig->data(), tick_event_before_dispatcher);
            }
        }
        catch(const std::runtime_error &e) {
            throw std::runtime_error("failed to initialize tick event: " + std::string(e.what()));
        }
    }
}