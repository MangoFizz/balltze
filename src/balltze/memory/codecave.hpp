// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__MEMORY__CODECAVE_HPP
#define BALLTZE__MEMORY__CODECAVE_HPP

#include <memory>
#include <cstdint>

#define DEFAULT_CAVE_SIZE 64

namespace Balltze::Memory {
    class Codecave {
    friend class Hook;
    public:
        /**
         * Get cave top
         */
        std::byte &get_top() const noexcept;

        /**
         * Get data
         */
        std::byte *get_data() const noexcept;

        /**
         * Check if cave is empty.
         */
        bool empty() const noexcept;

        /**
         * Prevents bytes from being inserted
         */
        void lock() noexcept;

        /**
         * Allow bytes from being inserted again
         */
        void unlock() noexcept;

        /**
         * Set cave as executable
         */
        void set_access_protection(bool setting = true) noexcept;

        /**
         * Allocate memory for our cave
         */
        Codecave() noexcept;

        /**
         * Insert a byte into the cave
         * @param byte  Byte to insert
         */
        template<typename T> inline void insert(T byte) noexcept {
            if(locked) {
                return;
            }
            data[top] = static_cast<std::byte>(byte);
            top++;
        }

        /**
         * Insert byte array into cave
         * @param bytes     Pointer to byte array
         * @param lenght    Size of array
         */
        template<typename T> inline void insert(T *bytes, std::size_t lenght) noexcept {
            if(locked) {
                return;
            }
            for(std::size_t i = 0; i < lenght; i++, top++) {
                data[top] = reinterpret_cast<const std::byte *>(bytes)[i];
            }
        }

        /**
         * Insert an address into cave
         * @param address   Address to insert
         */
        inline void insert_address(std::uintptr_t address) noexcept {
            if(locked) {
                return;
            }
            *reinterpret_cast<std::uintptr_t *>(&data[top]) = address;
            top += 4;
        }

    private:
        /** Cave itself */
        std::unique_ptr<std::byte[]> data;

        /** Cave size */
        std::size_t top = 0;

        /** Cave is locked? */
        bool locked = false;

        /** Original data protection */
        unsigned long original_page_protection;
    };
}

#endif