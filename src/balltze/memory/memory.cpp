// SPDX-License-Identifier: GPL-3.0-only

#include <windows.h>
#include <balltze/memory/memory.hpp>

namespace Balltze::Memory {
    void write_code(void *pointer, const std::uint16_t *data, std::size_t length) noexcept {
        // Instantiate our new_protection and old_protection variables.
        DWORD new_protection = PAGE_EXECUTE_READWRITE, old_protection;

        // Apply read/write/execute protection
        VirtualProtect(pointer, length, new_protection, &old_protection);

        // Copy
        for(std::size_t i = 0; i < length; i++) {
            if(data[i] != -1) {
                *(reinterpret_cast<std::uint8_t *>(pointer) + i) = static_cast<std::uint8_t>(data[i]);
            }
        }

        // Restore the older protection unless it's the same
        if(new_protection != old_protection) {
            VirtualProtect(pointer, length, old_protection, &new_protection);
        }
    }

    void fill_with_nops(void *address, std::size_t length) noexcept {
        auto *bytes = reinterpret_cast<std::byte *>(address);
        for(std::size_t i = 0; i < length; i++) {
            overwrite(bytes + i, static_cast<std::byte>(0x90));
        }
    }
}
