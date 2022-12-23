// SPDX-License-Identifier: GPL-3.0-only

#include <iostream>
#include <balltze/config/ini.hpp>
#include <balltze/map_loading/map_loading.hpp>
#include <balltze/memory/signature.hpp>
#include <balltze/output/message_box.hpp>
#include <balltze/balltze.hpp>

namespace Balltze {
    static EventListenerHandle<TickEvent> firstTickListener;
    Balltze *Balltze::m_instance;

    void set_up_events();

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

        set_up_events();

        // Set up first tick
        firstTickListener = TickEvent::subscribe_const(first_tick, EVENT_PRIORITY_HIGHEST);
    }

    void Balltze::first_tick(TickEvent const &context) {
        show_message_box("[%s] Tick #%d in %d milliseconds", context.time == EVENT_TIME_BEFORE ? "Before" : "After", context.args.tick_count, context.args.delta_time_ms);
        if(context.time == EVENT_TIME_AFTER) {
            firstTickListener.remove();
        }
    }

    extern "C" std::byte *get_address_for_signature(const char *name) noexcept {
        return Balltze::get().signature_manager().get(name)->data();
    }

    Balltze &Balltze::get() noexcept {
        return *m_instance;
    }
}
