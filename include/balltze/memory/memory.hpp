// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__MEMORY__MEMORY_HPP
#define BALLTZE__MEMORY__MEMORY_HPP

#include <memory>
#include <cstdint>
#include <windows.h>

namespace Balltze::Memory {
    /**
     * Overwrite the data at the pointer with the given bytes, ignoring any wildcard bytes.
     * @param pointer This is the pointer that points to the data to be overwritten.
     * @param data    This is the pointer that points to the data to be copied, excluding wildcards.
     * @param length  This is the length of the data.
     */
    void write_code(void *pointer, const std::uint16_t *data, std::size_t length) noexcept;

    /**
     * Overwrite the data at the pointer with the given SigByte bytes. Bytes equal to -1 are ignored.
     * @param  pointer pointer to the data
     * @param  data    bytes to overwrite with
     */
    #define write_code_s(pointer, data) \
        static_assert(sizeof(data[0]) == sizeof(SigByte), "write_code_s requires a SigByte");\
        for(std::size_t i = 0; i < sizeof(data) / sizeof(data[0]); i++) {\
            if(data[i] == -1) continue;\
            overwrite(pointer + i, static_cast<std::uint8_t>(data[i]));\
        }

    /**
     * Overwrite the data at the pointer with the given data even if this pointer is read-only.
     * @param pointer This is the pointer that points to the data to be overwritten.
     * @param data    This is the pointer that points to the data to be copied.
     * @param length  This is the length of the data.
     */
    template<typename T> inline void overwrite(void *pointer, const T *data, std::size_t length) noexcept {
        // Instantiate our new_protection and old_protection variables.
        DWORD new_protection = PAGE_EXECUTE_READWRITE, old_protection;

        // Apply read/write/execute protection
        VirtualProtect(pointer, length, new_protection, &old_protection);

        // Copy
        std::copy(data, data + length, reinterpret_cast<T *>(pointer));

        // Restore the older protection unless it's the same
        if(new_protection != old_protection) {
            VirtualProtect(pointer, length, old_protection, &new_protection);
        }
    }

    /**
     * Overwrite the data at the pointer with the given data even if this pointer is read-only.
     * @param pointer This is the pointer that points to the data to be overwritten.
     * @param data    This is the pointer that points to the data to be copied.
     */
    template<typename T> inline void overwrite(void *pointer, const T &data) noexcept {
        return overwrite(pointer, &data, 1);
    }
    
    /**
     * Make a function do nothing
     * @param function  Pointer to function
     */
    void nuke_function(void *function) noexcept;

    /**
     * Fill bytes with NOPs
     */
    void fill_with_nops(void *address, std::size_t length) noexcept;
}

#endif
