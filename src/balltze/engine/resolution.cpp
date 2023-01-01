// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/engine/resolution.hpp>
#include <balltze/memory.hpp>

namespace Balltze::Engine {
    Resolution &get_resolution() noexcept {
        static auto *resolution_sig = Memory::get_signature("resolution");
        static Resolution *resolution = *reinterpret_cast<Resolution **>(resolution_sig->data());
        return *resolution;
    }
}
