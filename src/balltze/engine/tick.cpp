// SPDX-License-Identifier: GPL-3.0-only

#include <cstdint>
#include <balltze/balltze.hpp>
#include <balltze/engine/tick.hpp>

namespace Balltze::Engine {
    std::size_t get_tick_count() noexcept {
        static std::int32_t *tick_count = nullptr;
        static auto &balltze = Balltze::get();
        static auto &sig_manager = balltze.signature_manager();
        static auto *tick_count_sig = sig_manager.get("tick_counter");
        
        if(!tick_count) {
            tick_count = reinterpret_cast<std::int32_t *>(**reinterpret_cast<std::byte ***>(tick_count_sig->data()) + 0xC);
        }
        return *tick_count;
    }
}

