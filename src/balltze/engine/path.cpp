// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/balltze.hpp>
#include <balltze/engine/path.hpp>

namespace Balltze::Engine {
    std::filesystem::path get_path() noexcept {
        static auto &sig_manager = Balltze::get().signature_manager();
        static auto *halo_path_sig = sig_manager.get("halo_path");
        static const char *path = nullptr;
        if(!path) {
            path = *reinterpret_cast<const char **>(halo_path_sig->data());
        }
        return std::filesystem::path(path);
    }
}
