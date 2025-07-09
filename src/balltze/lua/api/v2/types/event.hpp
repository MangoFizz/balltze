// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__LUA__API__V2__TYPES__EVENT_HPP
#define BALLTZE__LUA__API__V2__TYPES__EVENT_HPP

#include <optional>
#include <lua.hpp>
#include <balltze/events.hpp>

namespace Balltze::Lua::Api::V2 {
    /**
     * Get the string representation of the event priority.
     * 
     * @param priority The event priority to convert.
     * @return The string representation of the event priority.
     */
    const char *event_priority_to_string(Events::EventPriority priority);

    /**
     * Convert a string to an event priority.
     * 
     * @param priority The string representation of the event priority.
     * @return The corresponding event priority.
     * @throws std::invalid_argument if the string does not match any known priority.
     */
    Events::EventPriority event_priority_from_string(const char *priority);

    /**
     * Define all event types in the Lua state.
     * @param state Lua state
     * @note This function should be called to register all types from the functions of this header.
     */
    void define_event_types(lua_State *state) noexcept;
}

#endif
