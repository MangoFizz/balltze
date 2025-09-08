// SPDX-License-Identifier: GPL-3.0-only

#include <event/event.h>
#include <impl/console/console.h>
#include <balltze/memory.hpp>
#include <balltze/events/console.hpp>

namespace Balltze::Events {
    const std::string &ConsoleCommandEvent::console_input() const noexcept {
        return m_console_input;
    }

    template<>
    void EventHandler<ConsoleCommandEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        ringworld_event_subscribe(RW_EVENT_CONSOLE_COMMAND, +[](void *data) -> bool {
            std::string command = reinterpret_cast<char *>(data);
            ConsoleCommandEvent event(command);
            event.dispatch();
            return event.cancelled();
        });
    }
}
