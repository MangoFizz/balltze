// SPDX-License-Identifier: GPL-3.0-only

#include <windows.h>
#include <vector>
#include <memory>
#include <balltze/memory.hpp>
#include <balltze/hook.hpp>

#define DEFAULT_CAVE_SIZE 64

namespace Balltze::Memory {
    std::vector<std::unique_ptr<Hook>> hooks;

    std::byte &Codecave::top() const {
        if(m_top == 0) {
            throw std::runtime_error("Codecave is empty");
        }
        return m_data[m_top - 1];
    }

    std::byte *Codecave::data() const noexcept {
        return m_data.get();
    }

    bool Codecave::empty() const noexcept {
        return m_top == 0;
    }

    void Codecave::lock() noexcept {
        m_locked = true;
    }

    void Codecave::unlock() noexcept {
        m_locked = false;
    }

    void Codecave::enable_execute_access(bool enable) noexcept {
        if(enable) {
            VirtualProtect(m_data.get(), m_top, PAGE_EXECUTE_READWRITE, &m_original_page_protection);
        }
        else {
            VirtualProtect(m_data.get(), m_top, m_original_page_protection, NULL);
        }
    }

    Codecave::Codecave() noexcept {
        m_data = std::make_unique<std::byte[]>(DEFAULT_CAVE_SIZE);
    }

    Codecave::Codecave(std::size_t size) noexcept {
        m_data = std::make_unique<std::byte[]>(size);
    }

    std::byte *Hook::address() const noexcept {
        return m_instruction;
    }

    Codecave &Hook::cave() noexcept {
        return m_cave;
    }

    bool Hook::hooked() noexcept {
        return m_hooked;
    }

    void Hook::hook() noexcept {
        if(m_cave.empty() || m_hooked) {
            return;
        }
        m_hooked = true;
        m_cave.enable_execute_access();

        // Overwrite original code with jmp to cave
        fill_with_nops(m_instruction, m_original_code.size());
        overwrite(m_instruction, static_cast<std::byte>(0xE9));
        overwrite(m_instruction + 1, calculate_32bit_jump(m_instruction, m_cave.data()));
    }

    void Hook::release() noexcept {
        if(!m_hooked) {
            return;
        }
        m_hooked = false;
        m_cave.enable_execute_access(false);

        // Restore original code
        overwrite(m_instruction, m_original_code.data(), m_original_code.size());
    }

    void Hook::write_function_call(const void *function, bool pushad, bool save_result) noexcept {
        if(pushad) {
            m_cave.insert(0x9C); // pushfd
            m_cave.insert(0x60); // pushad
        }

        m_cave.insert(0xE8); // call
        auto fn_offset = calculate_32bit_jump(&m_cave.top(), function);
        m_cave.insert_address(fn_offset);

        if(save_result) {
            // mov [m32], al
            m_cave.insert(0xA2);
            m_cave.insert_address(m_skip_original_code.get());
        }

        if(pushad) {
            m_cave.insert(0x61); // popad
            m_cave.insert(0x9D); // popfd
        }
    }

    void Hook::copy_instructions(const void *address, std::uint8_t &copied_bytes) {
        auto *instruction = reinterpret_cast<const std::uint8_t *>(address);
        copied_bytes = 0;
        std::size_t instruction_size = 0;

        while(copied_bytes < 5) {
            instruction += instruction_size;

            switch(instruction[0]) {
                // call rel32
                case 0xE8: 
                // jmp rel32
                case 0xE9: {
                    m_cave.insert(instruction[0]); // call or jmp

                    // offset
                    auto original_offset = *reinterpret_cast<const std::uint32_t *>(&instruction[1]);
                    auto offset = original_offset + calculate_32bit_jump(&m_cave.top(), &instruction[5]);
                    m_cave.insert_address(offset);
                    
                    instruction_size = 5;
                    break;
                }

                // mov r/m8, imm8
                case 0xC6: {
                    m_cave.insert(0xC6);

                    // mov [esp + disp8], imm8
                    if(instruction[1] == 0x44 && instruction[2] == 0x24) {
                        m_cave.insert(&instruction[1], 4);
                        instruction_size = 5;
                    }
                    else {
                        throw std::runtime_error("Unsupported mov instruction.");
                    }
                    break;
                }

                // mov r8, byte ptr [r32 + disp8]
                case 0x8A: {
                    switch(instruction[1]) {
                        case 0x47: { // al, edi
                            m_cave.insert(&instruction[0], 3);
                            instruction_size = 3;
                            break;
                        }

                        case 0x07: {
                            m_cave.insert(&instruction[0], 2);
                            instruction_size = 2;
                            break;
                        }

                        default: {
                            throw std::runtime_error("Unsupported mov instruction.");
                        }
                    }
                    break;
                }

                // test r/m8,r8
                case 0x84: {
                    m_cave.insert(&instruction[0], 2);
                    instruction_size = 2;
                    break;
                }

                // far call / jump
                case 0xFF: {
                    m_cave.insert(0xFF);

                    // call dword ptr [edx + disp32]
                    if(instruction[1] == 0x92 || instruction[1] == 0x91) {
                        m_cave.insert(&instruction[1], 5);

                        instruction_size = 6;
                        break;
                    }
                    // call dword ptr [edx + disp8]
                    else if(instruction[1] == 0x52) {
                        m_cave.insert(0x52);
                        m_cave.insert(instruction[2]);

                        instruction_size = 3;
                        break;
                    }
                    // jmp dword ptr [eax ^ 4 + m32]
                    else if(instruction[1] == 0x24 && instruction[2] == 0x85) {
                        m_cave.insert(0x24);
                        m_cave.insert(0x85);
                        m_cave.insert(&instruction[3], 4);

                        instruction_size = 7;
                        break;
                    }
                    // call dword ptr [imm32]
                    else if(instruction[1] == 0x15) {
                        m_cave.insert(0x15);
                        m_cave.insert(&instruction[2], 4);
                        instruction_size = 6;
                        break;
                    }
                    else {
                        throw std::runtime_error("Unsupported call instruction.");
                    }
                }

                // test
                case 0x85: {
                    // test eax, eax (?)
                    if(instruction[1] == 0xC0) {
                        m_cave.insert(0xC0);
                        instruction_size = 2;
                    }
                    else {
                        throw std::runtime_error("Unsupported test instruction.");
                    }
                    break;
                }

                // movsx / sete
                case 0x0F: {
                    if(instruction[1] == 0xB6 && instruction[2] == 0x47) { // eax, byte ptr [edi + disp8]
                        m_cave.insert(&instruction[0], 4);
                        instruction_size = 4;
                    }
                    else if(instruction[1] == 0xBF && (instruction[2] == 0xC0 || instruction[2] == 0xC1)) { // eax, ax/cx
                        m_cave.insert(&instruction[0], 3);
                        instruction_size = 3;
                    }
                    else if(instruction[1] == 0x94 && instruction[2] == 0xC1) { // sete cl
                        m_cave.insert(&instruction[0], 3);
                        instruction_size = 3;
                    }
                    else {
                        throw std::runtime_error("Unsupported movzx instruction.");
                    }
                    break;
                }

                // cmp / inc
                case 0x66: {
                    if(instruction[1] == 0x39 && instruction[2] == 0x46) { // cmp word ptr [esi + disp8], ax
                        m_cave.insert(&instruction[0], 4);
                        instruction_size = 4;
                    }
                    else if(instruction[1] == 0xFF && instruction[2] == 0x05) { // inc word ptr [imm32]
                        m_cave.insert(&instruction[0], 7);
                        instruction_size = 7;
                    }
                    else if(instruction[1] == 0xC7 && instruction[2] == 0x00) { // cmp word ptr [r32], imm16
                        m_cave.insert(&instruction[0], 5);
                        instruction_size = 5;
                    }
                    else {
                        throw std::runtime_error("Unsupported cmp instruction.");
                    }
                    break;
                }

                // cmp al, imm8
                case 0x3C: {
                    m_cave.insert(&instruction[0], 2);
                    instruction_size = 2;
                    break;
                }

                // dec eax
                case 0x48: {
                    m_cave.insert(&instruction[0], 1);
                    instruction_size = 1;
                    break;
                }

                // cmp / add / sub
                case 0x83: {
                    if(instruction[1] == 0xF8) { // eax, imm8
                        m_cave.insert(&instruction[0], 3);
                        instruction_size = 3;
                    }
                    else if(instruction[1] == 0xC4) {
                        m_cave.insert(&instruction[0], 3);
                        instruction_size = 3;
                    }
                    else if(instruction[1] == 0xEC) { // sub esp, imm8
                        m_cave.insert(&instruction[0], 3);
                        instruction_size = 3;
                    }
                    else {
                        throw std::runtime_error("Unsupported cmp/add/sub instruction.");
                    }
                    break;
                }

                // nop
                case 0x90: {
                    m_cave.insert(0x90);
                    instruction_size = 1;
                    break;
                }

                // lea / mov
                case 0x8D:
                case 0x8B: {
                    if(instruction[1] == 0x4C && instruction[2] == 0x24) {
                        m_cave.insert(&instruction[0], 4);
                        instruction_size = 4;
                    }
                    else if(instruction[1] == 0x84 && instruction[2] == 0x24) {
                        m_cave.insert(&instruction[0], 7);
                        instruction_size = 7;
                    }
                    else if(instruction[1] == 0xD8) {
                        m_cave.insert(&instruction[0], 2);
                        instruction_size = 2;
                    }
                    else if(instruction[1] == 0x56) { // mov edx, [esi + imm8]
                        m_cave.insert(&instruction[0], 3);
                        instruction_size = 3;
                    }
                    else if(instruction[1] == 0xC9) {
                        m_cave.insert(&instruction[0], 2);
                        instruction_size = 2;
                    }
                    else if(instruction[1] == 0x0D) {
                        m_cave.insert(&instruction[0], 6);
                        instruction_size = 6;
                    }
                    else if(instruction[1] == 0x34 && instruction[2] == 0x40) {
                        m_cave.insert(&instruction[0], 3);
                        instruction_size = 3;
                    }
                    else {
                        throw std::runtime_error("Unsupported lea / mov instruction.");
                    }
                    break;
                }

                // mov
                case 0x89: {
                    if(instruction[1] == 0x72) { // mov [edx + imm8], esi
                        m_cave.insert(&instruction[0], 3);
                        instruction_size = 3;
                    }
                    else if(instruction[1] == 0x3D) {
                        m_cave.insert(&instruction[0], 6);
                        instruction_size = 6;
                    }
                    else {
                        throw std::runtime_error("Unsupported mov instruction.");
                    }
                    break;
                }

                // push r32 / pop r32 
                case 0x50 ... 0x5F: {
                    m_cave.insert(instruction[0]);
                    instruction_size = 1;
                    break;
                }

                // sub / cmp
                case 0x81: {
                    if(instruction[1] == 0xEC) { // sub esp, imm32
                        m_cave.insert(&instruction[0], 6);
                        instruction_size = 6;
                    }
                    else if(instruction[1] == 0xFD || instruction[1] == 0xFE) { // cmp ebp/esi, imm32
                        m_cave.insert(&instruction[0], 6);
                        instruction_size = 6;
                    }
                    else {
                        throw std::runtime_error("Unsupported sub instruction.");
                    }
                    break;
                }

                // mov
                case 0xA0 ... 0xA1: {
                    m_cave.insert(&instruction[0], 5);
                    instruction_size = 5;
                    break;
                }

                // xor bl, bl
                case 0x32: {
                    m_cave.insert(&instruction[0], 2);
                    instruction_size = 2;
                    break;
                }

                default: {
                    char message[256];
                    snprintf(message, sizeof(message), "Unable to build cave: unsupported instruction. \nOpcode: 0x%.2X at 0x%p", instruction[0], instruction);
                    throw std::runtime_error(message);
                }
            }

            // Backup original instruction
            auto *instruction_bytes = reinterpret_cast<const std::byte *>(instruction);
            m_original_code.insert(m_original_code.end(), instruction_bytes, instruction_bytes + instruction_size);

            copied_bytes += instruction_size;
        }
    }

    void Hook::copy_instructions(const void *address) {
        try {
            std::uint8_t no = 0;
            copy_instructions(address, no);
        }
        catch(std::runtime_error &e) {
            throw;
        }
    }

    void Hook::write_cave_return_jmp() noexcept {
        m_cave.insert(0xE9);
        auto offset = calculate_32bit_jump(&m_cave.top(), m_instruction + m_original_code.size());
        m_cave.insert_address(offset);
    }

    Hook *hook_function(void *instruction, std::optional<std::variant<std::function<void()>, std::function<bool()>>> function_before, std::optional<std::function<void()>> function_after, bool save_registers) {
        for(auto &hook : hooks) {
            if(hook->m_instruction == instruction) {
                throw std::runtime_error("address already hooked");
            }
        }

        Hook *hook = hooks.emplace_back(std::make_unique<Hook>()).get();
        hook->m_instruction = reinterpret_cast<std::byte *>(instruction);
        hook->m_skip_original_code = std::make_unique<bool>(false);

        if(function_before) {
            auto function_variant = *function_before;
            bool skipable_instruction = std::holds_alternative<std::function<bool()>>(function_variant);
            if(skipable_instruction) {
                auto function = std::get<std::function<bool()>>(function_variant);
                if(!function) {
                    throw std::invalid_argument("function_before must be a valid function");
                }
                hook->write_function_call(*reinterpret_cast<void **>(function.target<bool(*)()>()), save_registers, true);
            }
            else {
                auto function = std::get<std::function<void()>>(function_variant);
                if(!function) {
                    throw std::invalid_argument("function_before must be a valid function");
                }
                hook->write_function_call(*reinterpret_cast<void **>(function.target<void(*)()>()), save_registers, false);
            }
        }

        // cmp byte ptr [flag], 1
        hook->m_cave.insert(0x80);
        hook->m_cave.insert(0x3D);
        auto flag_address = reinterpret_cast<std::uint32_t>(hook->m_skip_original_code.get());
        hook->m_cave.insert_address(flag_address);
        hook->m_cave.insert(1);

        // je instruction_size
        hook->m_cave.insert(0x74);
        hook->m_cave.insert(0x0);
        std::uint8_t &jmp_offset = *reinterpret_cast<std::uint8_t *>(&hook->m_cave.top());

        std::uint8_t instruction_size;
        try {
            hook->copy_instructions(instruction, instruction_size);
        }
        catch(std::runtime_error) {
            throw;
        }

        jmp_offset = instruction_size;

        if(function_after) {
            if(!*function_after) {
                throw std::invalid_argument("function_after must be a valid function");
            }
            hook->write_function_call(*reinterpret_cast<void **>(function_after.value().target<void(*)()>()), save_registers);
        }

        hook->write_cave_return_jmp();
        hook->hook();
        return hook;
    }

    Hook *override_function(void *instruction, std::function<void()> function, void *&original_instruction) {
        for(auto &hook : hooks) {
            if(hook->m_instruction == instruction) {
                throw std::runtime_error("address already hooked");
            }
        }

        if(!function) {
            throw std::invalid_argument("function must be a valid function");
        }

        Hook *hook = hooks.emplace_back(std::make_unique<Hook>()).get();
        hook->m_instruction = reinterpret_cast<std::byte *>(instruction);

        hook->m_cave.insert(0xE9);
        hook->m_cave.insert_address(calculate_32bit_jump(&hook->m_cave.top(), *reinterpret_cast<void **>(function.target<void(*)()>())));
        original_instruction = &hook->m_cave.top() + 1;

        std::uint8_t instruction_size;
        try {
            hook->copy_instructions(instruction, instruction_size);
        }
        catch(std::runtime_error) {
            throw;
        }

        hook->write_cave_return_jmp();
        hook->hook();
        return hook;
    }

    Hook *replace_function_call(void *instruction, std::function<void()> function) {
        for(auto &hook : hooks) {
            if(hook->m_instruction == instruction) {
                throw std::runtime_error("address already hooked");
            }
        }

        Hook *hook = hooks.emplace_back(std::make_unique<Hook>()).get();
        hook->m_instruction = reinterpret_cast<std::byte *>(instruction);

        hook->m_cave.insert(0xE8);
        hook->m_cave.insert_address(calculate_32bit_jump(&hook->m_cave.top(), *reinterpret_cast<void **>(function.target<void(*)()>())));

        hook->m_cave.lock();
        std::uint8_t instruction_size;
        try {
            hook->copy_instructions(instruction, instruction_size);
        }
        catch(std::runtime_error) {
            throw;
        }
        hook->m_cave.unlock();

        hook->write_cave_return_jmp();
        hook->hook();
        return hook;
    }
}
