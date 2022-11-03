// SPDX-License-Identifier: GPL-3.0-only

#include <iostream>
#include <cstring>
#include <sstream>
#include <stdexcept>
#include <balltze/engine/version.hpp>
#include <balltze/memory/codefinder.hpp>
#include <balltze/memory/memory.hpp>
#include <balltze/memory/signature.hpp>
#include <balltze/output/message_box.hpp>

namespace Balltze::Memory {

    std::string Signature::name() const noexcept {
        return m_name;
    }

    std::byte *Signature::data() const noexcept {
        return m_data;
    }

    void Signature::restore() noexcept {
        overwrite(m_data, m_original_data.data(), m_original_data.size());
    }

    Signature::Signature(const char *name, const short *signature, std::size_t lenght, std::uint16_t offset, std::size_t match_num) {
        // Find it!
        auto *address = reinterpret_cast<std::byte *>(FindCode(GetModuleHandle(0), signature, lenght, match_num));

        if(address) {
            m_name = name;
            m_data = address + offset;
            m_original_data.insert(m_original_data.begin(), address + offset, address + lenght);
        }
        else {
            std::stringstream ss;
            ss << "Could not find signature '" << name << "'";
            std::cerr << "[Balltze] " << ss.str() << std::endl;
            throw std::runtime_error(ss.str());
        }
    }

    Signature *SignatureManager::get(std::string name) noexcept {
        for(auto &signature : m_signatures) {
            if(signature.name() == name) {
                return &signature;
            }
        }

        std::cerr << "[Balltze] Signature '" << name << "' does not exists" << std::endl;
        return nullptr;
    }

    void SignatureManager::load() {
        find_core_signatures();
        find_engine_signatures();
    }

    #define FIND_SIGNATURE(name, offset, match_num, ...) { \
        const std::int16_t data[] = __VA_ARGS__; \
        m_signatures.emplace_back(name, data, sizeof(data) / sizeof(data[0]), offset, match_num); \
    }

    void SignatureManager::find_core_signatures() {
        FIND_SIGNATURE("engine_type", 0x4, 0, {0x8D, 0x75, 0xD0, 0xB8, -1, -1, -1, -1, 0xE8, -1, -1, -1, -1, 0x83});
        FIND_SIGNATURE("window_globals", 0x4, 0, {0x8B, 0x45, 0x08, 0xA3, -1, -1, -1, -1, 0x8B, 0x4D, 0x14});
    }

    void SignatureManager::find_engine_signatures() {
        auto engine_type = Engine::ENGINE_TYPE_CUSTOM_EDITION;

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

    #undef FIND_SIGNATURE
}