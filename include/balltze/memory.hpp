// SPDX-License-Identifier: GPL-3.0-only

/**
 * Memory-related functions and classes. 
 * This includes functions for reading and writing to memory, as well as functions for finding memory addresses.
 * Code writing functions and endian interfaces are from Chimera and Invader respectively. 
 */

#ifndef BALLTZE_API__MEMORY_HPP
#define BALLTZE_API__MEMORY_HPP

#include <windows.h>
#include <string>
#include <vector>
#include <stdexcept>
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
     * Calculate a 32-bit offset between two pointers.
     * @param origin        Offset origin address.
     * @param destination   Destination address.
     * @return              Offset.
     */
    BALLTZE_API std::int32_t calculate_32bit_offset(const void *origin, const void *destination) noexcept;

    /**
     * Calculate a 32-bit jump to a destination address.
     * @param origin        Jump origin.
     * @param destination   Jump destination.
     * @return              Jump offset
     */
    template<typename T1, typename T2> inline std::int32_t calculate_32bit_jump(T1 *jump, const T2 *destination) noexcept {
        return calculate_32bit_offset(reinterpret_cast<const void *>(reinterpret_cast<std::byte *>(jump) + 5), reinterpret_cast<const void *>(destination));
    }

    /**
     * Follow a 32-bit offset.
     * @param offset    Offset to follow.
     * @return          Pointer to the offset destination.
     */
    BALLTZE_API std::byte *follow_32bit_offset(std::uint32_t *offset) noexcept;

    /**
     * Follow a 32-bit jump instruction.
     * @param jmp   Jump instruction.
     * @return      Pointer to the jump destination.
     */
    template<typename T> inline std::byte *follow_32bit_jump(T *jmp) noexcept {
        return follow_32bit_offset(reinterpret_cast<std::uint32_t *>(reinterpret_cast<std::byte *>(jmp) + 1));
    }

    #define ENDIAN_TEMPLATE(tname) template <template<typename> class tname>
    #define COPY_THIS(what) copy . what = this -> what;
    #define COPY_THIS_ARRAY(what) for(std::size_t copy_this_array_iterator = 0; copy_this_array_iterator < sizeof(this -> what)/sizeof(this -> what[0]); copy_this_array_iterator++) { copy . what [copy_this_array_iterator] = this -> what [copy_this_array_iterator]; }

    /**
     * This is a simple interface for reading/writing swapped endian data
     */
    template <typename T> class SwappedEndian {
    public:
        using struct_type = T;
        
        std::byte value[sizeof(T)] = {};

        /**
         * Convert the value to host endian.
         * @return the value in host endian
         */
        T read() const noexcept {
            std::byte value_copy_byte[sizeof(T)];
            for(std::size_t i = 0; i < sizeof(T); i++) {
                value_copy_byte[i] = value[sizeof(T) - (i + 1)];
            }
            return *reinterpret_cast<const T *>(value_copy_byte);
        }

        /**
         * Convert the value to host endian.
         * @return the value in host endian
         */
        operator T() const noexcept {
            return this->read();
        }

        /**
         * Overwrite the value
         * @param new_value value to overwrite
         */
        void write(const T &new_value) noexcept {
            *reinterpret_cast<T *>(this->value) = reinterpret_cast<const SwappedEndian<T> *>(&new_value)->read();
        }

        /**
         * Overwrite the value
         * @param new_value value to overwrite
         */
        void operator =(const T &new_value) noexcept {
            return this->write(new_value);
        }

        SwappedEndian(const T &new_value) noexcept {
            this->write(new_value);
        }

        SwappedEndian() noexcept {

        }
    };

    /**
     * This is a simple interface for reading/writing native endian data
     */
    template <typename T> class NativeEndian {
    public:
        using struct_type = T;
        
        std::byte value[sizeof(T)] = {};

        /**
         * Convert the value to host endian.
         * @return the value in host endian
         */
        T read() const noexcept {
            return *reinterpret_cast<const T *>(value);
        }

        /**
         * Convert the value to host endian.
         * @return the value in host endian
         */
        operator T() const noexcept {
            return this->read();
        }

        /**
         * Overwrite the value
         * @param new_value value to overwrite
         */
        void write(const T &new_value) noexcept {
            *reinterpret_cast<T *>(value) = new_value;
        }

        /**
         * Overwrite the value
         * @param new_value value to overwrite
         */
        void operator =(const T &new_value) noexcept {
            return this->write(new_value);
        }

        NativeEndian(const T &new_value) noexcept {
            this->write(new_value);
        }

        NativeEndian() noexcept {

        }
    };

    #ifdef __BYTE_ORDER__
        #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
            #define LittleEndian NativeEndian
            #define BigEndian SwappedEndian
        #elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
            #define BigEndian SwappedEndian
            #define LittleEndian NativeEndian
        #endif
    #else
        #define LittleEndian NativeEndian
        #define BigEndian SwappedEndian
    #endif

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
