// SPDX-License-Identifier: GPL-3.0-only

#include <cstring>
#include <sstream>
#include <stdexcept>
#include <balltze/memory/codefinder.hpp>
#include <balltze/memory/memory.hpp>
#include <balltze/memory/signature.hpp>

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

    Signature::Signature(const char *name, const short *signature, std::size_t lenght, std::uint16_t offset) {
        // Find it!
        auto *address = reinterpret_cast<std::byte *>(FindCode(GetModuleHandle(0), signature, lenght));

        if(address) {
            m_name = name;
            m_data = address + offset;
            m_original_data.insert(m_original_data.begin(), m_data, m_data + lenght - offset);
        }
        else {
            std::stringstream ss;
            ss << "Could not find signature '" << name << "'";
            throw std::runtime_error(ss.str());
            std::terminate();
        }
    }

    Signature *SignatureManager::get(std::string name) noexcept {
        for(auto &signature : m_signatures) {
            if(signature.name() == name) {
                return &signature;
            }
        }
        return nullptr;
    }

    void SignatureManager::load() {
        find_core_signatures();
    }

    #define FIND_SIGNATURE(name, offset, ...) { \
        const std::int16_t data[] = __VA_ARGS__; \
        m_signatures.emplace_back(name, data, sizeof(data) / sizeof(data[0]), offset); \
    }

    void SignatureManager::find_core_signatures() {
        FIND_SIGNATURE("engine_type", 0x4, {0x8D, 0x75, 0xD0, 0xB8, -1, -1, -1, -1, 0xE8, -1, -1, -1, -1, 0x83});
        FIND_SIGNATURE("window_globals", 0x4, {0x8B, 0x45, 0x08, 0xA3, -1, -1, -1, -1, 0x8B, 0x4D, 0x14});
    }

    void SignatureManager::find_engine_signatures() {
        /** Events */
        FIND_SIGNATURE("on_tick", 0x0, {-1, -1, -1, -1, -1, 0xA1, -1, -1, -1, -1, 0x8B, 0x50, 0x14, 0x8B, 0x48, 0x0C});
    }

    #undef FIND_SIGNATURE
}