// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/engine/path.hpp>
#include <balltze/memory.hpp>

namespace Balltze::Engine {
    std::filesystem::path get_path() noexcept {
        static auto *halo_path_sig = Memory::get_signature("halo_path");
        static const char *path = nullptr;
        if(!path) {
            path = *reinterpret_cast<const char **>(halo_path_sig->data());
        }
        return std::filesystem::path(path);
    }
}
