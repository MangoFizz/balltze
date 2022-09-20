// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/memory/signature.hpp>
#include <balltze/output/message_box.hpp>
#include <balltze/balltze.hpp>

namespace Balltze {
    Balltze *Balltze::m_instance;

    Memory::SignatureManager &Balltze::signature_manager() noexcept {
        return m_signature_manager;
    }

    Balltze::Balltze() noexcept {
        // Set singleton
        m_instance = this;

        // Initialize signature manager
        m_signature_manager.load();

        // Say hi
        message_box("Loaded!");
    }

    Balltze &Balltze::get() noexcept {
        return *m_instance;
    }
}
