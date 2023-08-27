// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__HELPERS__RESOURCES_HPP
#define BALLTZE_API__HELPERS__RESOURCES_HPP

#include <vector>
#include <memory>
#include <optional>
#include <cstdint>
#include <cstring>
#include <windows.h>
#include <gdiplus.h>
#include "../api.hpp"

namespace Balltze {
    BALLTZE_API std::optional<std::vector<std::byte>> load_resource_data(HMODULE module, const wchar_t *resource_id, const wchar_t *resource_type) noexcept;
    BALLTZE_API std::unique_ptr<Gdiplus::Bitmap> load_image_from_resource(HMODULE module, const wchar_t *resource_id, const wchar_t *resource_type) noexcept;
}

#endif
