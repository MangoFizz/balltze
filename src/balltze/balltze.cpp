// SPDX-License-Identifier: GPL-3.0-only

#include <iostream>
#include <balltze/event.hpp>
#include <balltze/memory.hpp>
#include <balltze/output.hpp>
#include "config/ini.hpp"
#include "event/event.hpp"
#include "map_loading/map_loading.hpp"
#include "memory/signature.hpp"
#include "balltze.hpp"

namespace Balltze {
    using namespace Event;
    using namespace Memory;

    static void first_tick(TickEvent const &context) noexcept;
    static EventListenerHandle<TickEvent> firstTickListener;
    Balltze *Balltze::m_instance;

    Ini &Balltze::chimera_ini() noexcept {
        return *m_chimera_ini;
    }

    Balltze::Balltze() noexcept {
        m_instance = this;
        find_signatures();
        m_chimera_ini = std::make_unique<Ini>("chimera.ini");
        set_up_map_loading();
        set_up_events();
        firstTickListener = TickEvent::subscribe_const(first_tick, EVENT_PRIORITY_HIGHEST);
    }

    static void first_tick(TickEvent const &context) noexcept {
        show_message_box("[%s] Tick #%d in %d milliseconds", context.time == EVENT_TIME_BEFORE ? "Before" : "After", context.args.tick_count, context.args.delta_time_ms);
        if(context.time == EVENT_TIME_AFTER) {
            firstTickListener.remove();
        }
    }

    Balltze &Balltze::get() noexcept {
        return *m_instance;
    }
}
