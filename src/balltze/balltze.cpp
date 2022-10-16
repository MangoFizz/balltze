// SPDX-License-Identifier: GPL-3.0-only

#include <iostream>
#include <balltze/config/ini.hpp>
#include <balltze/events/map_load.hpp>
#include <balltze/events/tick.hpp>
#include <balltze/map_loading/map_loading.hpp>
#include <balltze/memory/signature.hpp>
#include <balltze/output/message_box.hpp>
#include <balltze/balltze.hpp>

namespace Balltze {
    Balltze *Balltze::m_instance;

    Memory::SignatureManager &Balltze::signature_manager() noexcept {
        return m_signature_manager;
    }

    Ini &Balltze::chimera_ini() noexcept {
        return *m_chimera_ini;
    }

    Balltze::Balltze() noexcept {
        // Set singleton
        m_instance = this;

        // Initialize signature manager
        m_signature_manager.load();

        // Load Chimera INI
        m_chimera_ini = std::make_unique<Ini>("chimera.ini");

        // Set up map loading
        set_up_map_loading();

        // Set up tick event
        // set_up_tick_event();

        // Set up map load event
        // set_up_map_load_event();

        // Set up first tick
        add_tick_event(first_tick, EventPriority::EVENT_PRIORITY_BEFORE);

        // Siuuu
        std::cout << "Balltze initialized" << std::endl;
    }

    void Balltze::first_tick() noexcept {
        // Remove first tick
        remove_tick_event(first_tick);

        // Show message box
        show_message_box("Balltze initialized");
    }

    extern "C" std::byte *get_address_for_signature(const char *name) noexcept {
        return Balltze::get().signature_manager().get(name)->data();
    }

    Balltze &Balltze::get() noexcept {
        return *m_instance;
    }
}
