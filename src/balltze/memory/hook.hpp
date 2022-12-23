// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__MEMORY__HOOK_HPP
#define BALLTZE__MEMORY__HOOK_HPP

#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <cstdint>
#include "codecave.hpp"

namespace Balltze::Memory {
    class Hook {
    public:
        /** Hook exception class */
        class Exception;

        /**
         * Is the hook hooked?
         */
        bool is_hooked() noexcept;

        /**
         * Is the hook initialized?
         */
        bool is_initialized() noexcept;

        /**
         * Hook it!
         */
        void hook() noexcept;

        /**
         * Release hook, restore original code
         */
        void release() noexcept;

        /**
         * Initialize basic hook.
         * @param instruction           Pointer to instruction to hook.
         * @param function_before       Function to be called before original code.
         * @param function_after        Function to be called after original code.
         * @param pushad                Insert pushad and pushfd instructions.
         */
        void initialize(void *instruction, const void *function_before, const void *function_after = nullptr, bool pushad = true);

        /**
         * Basic hook constructor.
         * @param instruction           Pointer to instruction to hook.
         * @param function_before       Function to be called before original code.
         * @param function_after        Function to be called after original code.
         * @param pushad                Insert pushad and pushfd instructions.
         */
        Hook(void *instruction, const void *function_before, const void *function_after = nullptr, bool pushad = true) {
            initialize(instruction, function_before, function_after, pushad);
        }

        /**
         * Empty constructor.
         */
        Hook() {}

        /**
         * Destroy hook.
         */
        ~Hook() noexcept;

        /**
         * Calculate a 32-bit offset.
         * @param jmp           Address of the jmp instruction.
         * @param destination   Jump destination.
         */
        static std::uint32_t calculate_32bit_offset(const void *jmp, const void *destination) noexcept;

        /**
         * Follow a call/jmp instruction.
         * @param jmp   Pointer to the instruction.
         */
        static std::byte *follow_jump(std::byte *jmp) noexcept;

        /**
         * Follow a call/jmp instruction.
         * @param jmp   Pointer to the instruction.
         */
        static inline std::byte *follow_jump(void *jmp) noexcept {
            return follow_jump(reinterpret_cast<std::byte *>(jmp));
        }

        /** Hooked instruction */
        std::byte *instruction;
    protected: 
        /** Hook code */
        Codecave cave;

        /** Original code */
        std::vector<std::byte> original_code;

        /** Is this hooked? */
        bool hooked = false;

        /**
         * Write a function call instruction into cave.
         * @param function  Function to be called.
         * @param pushad    Insert pushad and pushfd instructions.
         */
        void write_function_call(const void *function, bool pushad = true) noexcept;

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
    };

    class Hook::Exception : public std::runtime_error {
    public:
        /**
         * Constructor for Hook exception
         */
        Exception(std::string message) noexcept;
    };

    class SwitchHook : public Hook {
    public:
        /**
         * Set whether or not to execute original code
        */
        void execute_original_code(bool setting) noexcept;

        /**
         * Get execute original code flag
        */
        bool &execute_original_code() noexcept;

        /**
         * Initialize switch hook.
         * @param instruction   Pointer to instruction to hook.
         * @param function      Function to be called in the hook.
         * @param pushad        Insert pushad and pushfd instructions.
         */
        void initialize(void *instruction, const void *function, bool pushad = true);

        /**
         * Switch hook constructor.
         * @param instruction   Pointer to instruction to hook.
         * @param function      Function to be called in the hook.
         * @param pushad        Insert pushad and pushfd instructions.
         */
        SwitchHook(void *instruction, const void *function, bool pushad = true) {
            initialize(instruction, function, pushad);
        }

        /**
         * Empty constructor.
         */
        SwitchHook() {}

        /**
         * Destroy hook.
         */
        ~SwitchHook() noexcept;

        protected:
        /** Skip the original instruction */
        std::unique_ptr<bool> execute_original_code_flag;
    };

    class FunctionOverride : public Hook {
    public:
        /**
         * Initialize function override hook.
         * @param instruction   Pointer to instruction to hook.
         * @param function      Function to be called in the hook.
         * @param cave_return   Return address for function.
         */
        void initialize(void *instruction, const void *function, const void **cave_return);

        /**
         * Function override hook constructor.
         * @param instruction   Pointer to instruction to hook.
         * @param function      Function to be called in the hook.
         * @param cave_return   Return address for function.
         */
        FunctionOverride(void *instruction, const void *function, const void **cave_return) {
            initialize(instruction, function, cave_return);
        }

        /**
         * Empty constructor.
         */
        FunctionOverride() {}

        /**
         * Destroy hook.
         */
        ~FunctionOverride() noexcept;
    };

    class RawHook : public Hook {
    public:
        /**
         * Hook it!
         */
        void hook() noexcept;

        /**
         * Initialize raw hook.
         * @param instruction   Pointer to instruction to hook.
         * @param function      Function to be called in the hook.
         * @param cave_return   Return address for function.
         */
        void initialize(void *instruction, const void *function, const void **cave_return);

        /**
         * Raw hook constructor.
         * @param instruction   Pointer to instruction to hook.
         * @param function      Function to be called in the hook.
         * @param cave_return   Return address for function.
         */
        RawHook(void *instruction, const void *function, const void **cave_return) {
            initialize(instruction, function, cave_return);
        }

        /**
         * Empty constructor.
         */
        RawHook() {}

        /**
         * Destroy hook.
         */
        ~RawHook() noexcept;

    private:
        /** Pointer to function */
        const void *function;
    };

    class ChimeraFunctionOverride : public Hook {
    public:
        /**
         * Initialize Chimera's function-override override
         * @param cave          Pointer to Chimera's cave.
         * @param function      Function to be called in the hook.
         * @param cave_return   Return address for function.
         */
        void initialize(void *cave, const void *function, const void **cave_return);

        /**
         * Constructor for Chimera's function-override override
         * @param cave          Pointer to Chimera's cave.
         * @param function      Function to be called in the hook.
         * @param cave_return   Return address for function.
         */
        ChimeraFunctionOverride(void *cave, const void *function, const void **cave_return) {
            initialize(cave, function, cave_return);
        }

        /**
         * Empty constructor.
         */
        ChimeraFunctionOverride() {}

        /**
         * Destroy hook.
         */
        ~ChimeraFunctionOverride() noexcept;
    };

    void remove_chimera_hook(std::byte *instruction, bool before, bool after, bool pushad) noexcept;
}

#endif
