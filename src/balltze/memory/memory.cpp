// SPDX-License-Identifier: GPL-3.0-only

#include <windows.h>
#include <iostream>
#include <cstring>
#include <sstream>
#include <stdexcept>
#include <balltze/memory.hpp>
#include <balltze/engine/version.hpp>
#include "codefinder.hpp"

namespace Balltze::Memory {
    static std::vector<Signature> signatures;

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

    std::int32_t calculate_32bit_offset(const void *origin, const void *destination) noexcept {
        std::int32_t offset = reinterpret_cast<std::uint32_t>(destination) - reinterpret_cast<std::uint32_t>(origin);
        return offset;
    }

    std::byte *follow_32bit_offset(std::uint32_t *offset) noexcept {
        auto offset_end = reinterpret_cast<std::uint32_t>(offset + 1);
        auto *destination = reinterpret_cast<std::byte *>(*offset + offset_end);
        return destination;
    }

    std::string Signature::name() const noexcept {
        return m_name;
    }

    std::byte *Signature::data() const noexcept {
        return m_data;
    }

    void Signature::restore() noexcept {
        overwrite(m_data, m_original_data.data(), m_original_data.size());
    }

    Signature::Signature(std::string name, const short *signature, std::size_t lenght, std::uint16_t offset, std::size_t match_num) {
        auto *address = reinterpret_cast<std::byte *>(FindCode(GetModuleHandle(0), signature, lenght, match_num));
        if(address) {
            m_name = name;
            m_data = address + offset;
            m_original_data.insert(m_original_data.begin(), address + offset, address + lenght);
        }
        else {
            throw std::runtime_error("Could not find signature " + name);
        }
    }

    Signature const *get_signature(std::string name) noexcept {
        for(auto &signature : signatures) {
            if(signature.name() == name) {
                return &signature;
            }
        }
        return nullptr;
    }

    Signature find_signature(const char *name, const short *signature, std::size_t lenght, std::uint16_t offset, std::size_t match_num) {
        try {
            return Signature(name, signature, lenght, offset, match_num);
        }
        catch(std::runtime_error &e) {
            throw;
        }
    }

    Signature find_signature(const char *name, std::string signature, std::uint16_t offset, std::size_t match_num) {
        std::vector<short> data;
        std::string byte;
        if(signature.size() % 2 != 0) {
            throw std::runtime_error("Invalid signature " + signature);
        }
        for(std::size_t i = 0; i < signature.size(); i += 2) {
            byte = signature.substr(i, 2);
            if(byte == "??") {
                data.push_back(-1);
            }
            else {
                data.push_back(std::stoi(byte, nullptr, 16));
            }
        }
        return Signature(name, data.data(), data.size(), offset, match_num);
    }

    #define FIND_SIGNATURE(name, offset, match_num, ...) { \
        const std::int16_t data[] = __VA_ARGS__; \
        signatures.emplace_back(name, data, sizeof(data) / sizeof(data[0]), offset, match_num); \
    }

    static void find_core_signatures() {
        FIND_SIGNATURE("engine_type", 0x4, 0, {0x8D, 0x75, 0xD0, 0xB8, -1, -1, -1, -1, 0xE8, -1, -1, -1, -1, 0x83});
        FIND_SIGNATURE("window_globals", 0x4, 0, {0x8B, 0x45, 0x08, 0xA3, -1, -1, -1, -1, 0x8B, 0x4D, 0x14});
    }

    static void find_engine_signatures() {
        auto engine_type = Engine::ENGINE_TYPE_CUSTOM_EDITION;

        FIND_SIGNATURE("tick_counter", 0x1, 0, {0xA1, -1, -1, -1, -1, 0x8B, 0x50, 0x14, 0x8B, 0x48, 0x0C, 0x83, 0xC4, 0x04, 0x42, 0x41, 0x4E, 0x4F});

        /** Events */
        FIND_SIGNATURE("on_tick", 0x0, 0, {-1, -1, -1, -1, -1, 0xA1, -1, -1, -1, -1, 0x8B, 0x50, 0x14, 0x8B, 0x48, 0x0C});
        FIND_SIGNATURE("on_map_load", 0x0, 0, {0xE8, -1, -1, -1, -1, 0xE8, -1, -1, -1, -1, 0xA1, -1, -1, -1, -1, 0x33, 0xD2, 0x8B, 0xC8, 0x89, 0x11});

        /** Map loading */
        FIND_SIGNATURE("map_header", 0x2, 0, { 0x81, 0x3D, -1, -1, -1, -1, -1, -1, -1, -1, 0x8B, 0x3D });
        FIND_SIGNATURE("map_load_path", 0x0, 0, { /*0xE8*/ -1, -1, -1, -1, -1, 0xA1, -1, -1, -1, -1, 0x83, 0xC4, -1, 0x85, 0xC0, 0xBF, 0x80, 0x00, 0x00, 0x48 });
        FIND_SIGNATURE("read_map_file_data", 0x0, 0, { /*0x57*/ -1, /*0x56*/ -1, /*0x53*/ -1, /*0x55*/ -1, /*0x50*/ -1, 0xFF, 0x54, 0x24, -1, 0x85, 0xC0, 0x75, 0x29 });
        FIND_SIGNATURE("model_data_buffer_alloc", 0x0, 0, {0xFF, 0x15, -1, -1, -1, -1, 0x8B, 0x4B, 0x20, 0x8B, 0x53, 0x14, 0x57, 0x8B, 0xE8});
        
        
        FIND_SIGNATURE("get_tag_id", 0x0, 0, {0xA0, -1, -1, -1, -1, 0x53, 0x83, 0xCB, 0xFF, 0x84, 0xC0, 0x55, 0x8B, 0x6C, 0x24, 0x0C, 0x74, 0x5B, 0xA1, -1, -1, -1, -1, 0x8B, 0x48, 0x0C});
    }

    void find_signatures() {
        find_core_signatures();
        find_engine_signatures();
    }

    extern "C" std::byte *get_address_for_signature(const char *name) noexcept {
        return get_signature(name)->data();
    }

    #undef FIND_SIGNATURE
}
