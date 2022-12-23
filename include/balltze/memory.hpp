// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__MEMORY_HPP
#define BALLTZE_API__MEMORY_HPP

#include <windows.h>
#include <string>
#include <vector>
#include <cstddef>
#include "api.hpp"

#define GAP(bytes, line) char gap_##line[bytes]
#define GET_GAP(bytes, line) GAP(bytes, line)
#define PADDING(bytes) GET_GAP(bytes, __LINE__)

namespace Balltze::Memory {
    /**
     * Overwrite the data at the pointer with the given bytes, ignoring any wildcard bytes.
     * @param pointer This is the pointer that points to the data to be overwritten.
     * @param data    This is the pointer that points to the data to be copied, excluding wildcards.
     * @param length  This is the length of the data.
     */
    BALLTZE_API void write_code(void *pointer, const std::uint16_t *data, std::size_t length) noexcept;

    /**
     * Overwrite the data at the pointer with the given bytes. Bytes equal to -1 are ignored.
     * @param  pointer pointer to the data
     * @param  data    bytes to overwrite with
     */
    #define write_code_s(pointer, data) \
        static_assert(sizeof(data[0]) == sizeof(std::byte), "invalid signature");\
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
     * Fill a code chunk with NOPs
     */
    BALLTZE_API void fill_with_nops(void *address, std::size_t length) noexcept;

    /**
     * Class for signatures
     */
    class BALLTZE_API Signature {
    public:
        /**
         * Get the signature name
         */
        std::string name() const noexcept;

        /**
         * Get a pointer to the signature data
         */
        std::byte *data() const noexcept;

        /**
         * Restore the signature original data
         */
        void restore() noexcept;

        /**
         * Constructor for Signature
         * @param name      Name for signature
         * @param signature Signature bytes
         * @param lenght    Number of bytes
         * @param offset    Offset from the signature
         * @param match_num Match number
         */
        Signature(std::string name, const short *signature, std::size_t lenght, std::uint16_t offset, std::size_t match_num);

    private:
        /** Signature name */
        std::string m_name;

        /** Pointer to the signature data */
        std::byte *m_data;

        /** Original signature data */
        std::vector<std::byte> m_original_data;
    };

    /**
     * Get a Balltze's signature by its name
     * @param name Name of the signature
     * @return     Pointer to the Balltze signature
     */
    BALLTZE_API Signature const *get_signature(std::string name) noexcept;

    /**
     * Find a signature
     * @param name      Name of the signature
     * @param signature Signature bytes
     * @param lenght    Number of bytes
     * @param offset    Offset from the signature
     * @param match_num Match number
     * @return          Signature
     */
    BALLTZE_API Signature find_signature(const char *name, const short *signature, std::size_t lenght, std::uint16_t offset = 0, std::size_t match_num = 0);

    /**
     * Find a signature
     * @param name      Name of the signature
     * @param signature Signature bytes
     * @param offset    Offset from the signature
     * @param match_num Match number
     * @return          Signature
     */
    BALLTZE_API Signature find_signature(const char *name, std::string signature, std::uint16_t offset = 0, std::size_t match_num = 0);
}

#endif
