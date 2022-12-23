// SPDX-License-Identifier: GPL-3.0-only

#include <cstdint>
#include <balltze/engine/tick.hpp>
#include <balltze/memory.hpp>

namespace Balltze::Engine {
    std::size_t get_tick_count() noexcept {
        static std::int32_t *tick_count = nullptr;
        static auto *tick_count_sig = Memory::get_signature("tick_counter");
        if(!tick_count) {
            tick_count = reinterpret_cast<std::int32_t *>(**reinterpret_cast<std::byte ***>(tick_count_sig->data()) + 0xC);
        }
        return *tick_count;
    }
}

