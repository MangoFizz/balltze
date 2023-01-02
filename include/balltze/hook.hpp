// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__HOOK_HPP
#define BALLTZE_API__HOOK_HPP

#include <memory>
#include <cstdint>
#include <vector>
#include <functional>
#include <optional>
#include <variant>
#include "api.hpp"

namespace Balltze::Memory {
    class BALLTZE_API Codecave {
    private:
        /** Cave itself */
        std::unique_ptr<std::byte[]> m_data;

        /** Cave size */
        std::size_t m_top = 0;

        /** Cave is locked? */
        bool m_locked = false;

        /** Original data protection */
        unsigned long m_original_page_protection;

    public:
        /**
         * Get cave top
         */
        std::byte &top() const;

        /**
         * Get data
         */
        std::byte *data() const noexcept;

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
         * @param enable    Enable or disable execute access
         */
        void enable_execute_access(bool enable = true) noexcept;

        /**
         * Allocate memory for our cave
         */
        Codecave() noexcept;

        /**
         * Allocate memory for our cave
         * @param size  Size of cave
         */
        Codecave(std::size_t size) noexcept;

        /**
         * Insert a byte into the cave
         * @param byte  Byte to insert
         */
        template<typename T> inline void insert(T byte) noexcept {
            if(m_locked) {
                return;
            }
            m_data[m_top] = static_cast<std::byte>(byte);
            m_top++;
        }

        /**
         * Insert byte array into cave
         * @param bytes     Pointer to byte array
         * @param lenght    Size of array
         */
        template<typename T> inline void insert(T *bytes, std::size_t lenght) noexcept {
            if(m_locked) {
                return;
            }
            for(std::size_t i = 0; i < lenght; i++, m_top++) {
                m_data[m_top] = reinterpret_cast<const std::byte *>(bytes)[i];
            }
        }

        /**
         * Insert an address into cave
         * @param address   Address to insert
         */
        inline void insert_address(std::uintptr_t address) noexcept {
            if(m_locked) {
                return;
            }
            *reinterpret_cast<std::uintptr_t *>(&m_data[m_top]) = address;
            m_top += 4;
        }

        /**
         * Insert an address into cave
         * @param pointer   Pointer to insert
         */
        template<typename T> inline void insert_address(T *pointer) noexcept {
            if(m_locked) {
                return;
            }
            *reinterpret_cast<std::uintptr_t *>(&m_data[m_top]) = reinterpret_cast<std::uintptr_t>(pointer);
            m_top += 4;
        }
    };

    class BALLTZE_API Hook {
    private:
        /** Address of the instruction to hook */
        std::byte *m_instruction;

        /** Hook code */
        Codecave m_cave;

        /** Original code */
        std::vector<std::byte> m_original_code;

        /** Is this hooked? */
        bool m_hooked = false;

        /** Skip original code flag storage */
        std::unique_ptr<bool> m_skip_original_code;

        /**
         * Write a function call instruction into cave.
         * @param function  Function to be called.
         * @param pushad    Insert pushad and pushfd instructions.
         */
        void write_function_call(const void *function, bool pushad = true, bool save_result = false) noexcept;

        /**
         * Copy assembly instructions into cave.
         * @param address           Address where the code to copy is.
         * @param instruction_size  Size of the copied instruction(s) in cave.
         * @throw                   Hook exception on unknown instruction.
         */
        void copy_instructions(const void *address, std::uint8_t &instructions_size);

        /**
         * Copy assembly instructions into cave.
         * @param address           Address where the code to copy is.
         * @throw                   Hook exception on unknown instruction.
         */
        void copy_instructions(const void *address);

        /**
         * Write cave return.
         * @param bytes     The amount of bytes to jump
         */
        void write_cave_return_jmp() noexcept;

    public:
        /**
         * Get address of the instruction to hook
         */
        std::byte *address() const noexcept;

        /**
         * Get hook code
         */
        Codecave &cave() noexcept;

        /**
         * Is the hook hooked?
         */
        bool hooked() noexcept;

        /**
         * Hook it!
         */
        void hook() noexcept;

        /**
         * Release hook, restore original code
         */
        void release() noexcept;

        /**
         * Empty constructor.
         */
        Hook() = default;

        // just friends :')
        friend Hook *hook_function(void *, std::optional<std::variant<std::function<void()>, std::function<bool()>>>, std::optional<std::function<void()>>, bool);
        friend Hook *override_function(void *, std::function<void()>, void *&);
    };

    /**
     * Hook a given instruction.
     * Original code can be skipped by returning false in the function_before function.
     * @param address               Address of the instruction to hook
     * @param function_before       Function to be called before original code.
     * @param function_after        Function to be called after original code
     * @param save_registers        Save registers before calling functions
     * @return                      Hook object
     * @throws std::runtime_error   If instruction is not supported or is already hooked
     * @note                        If the instruction to hook can't be skipped, use the proper function type.
     */
    BALLTZE_API Hook *hook_function(void *instruction, std::optional<std::variant<std::function<void()>, std::function<bool()>>> function_before, std::optional<std::function<void()>> function_after = {}, bool save_registers = true);

    /**
     * Override a given function.
     * @param address               Address of the instruction to hook
     * @param function              Function to be called instead of original code
     * @param original_instruction  Pointer to where the original instruction was copied
     * @return                      Hook object
     * @throws std::runtime_error   If instruction is not supported or is already hooked
     */
    BALLTZE_API Hook *override_function(void *instruction, std::function<void()> function, void *&original_instruction);
}

#endif
