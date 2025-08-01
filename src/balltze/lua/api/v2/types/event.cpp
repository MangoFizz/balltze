// SPDX-License-Identifier: GPL-3.0-only

#include <cstring>
#include <stdexcept>
#include <balltze/events.hpp>
#include "../../../libraries/luastruct.hpp"
#include "event.hpp"

using namespace Balltze::Events;

namespace Balltze::Lua::Api::V2 {
    const char *event_priority_to_string(EventPriority priority) {
        switch(priority) {
            case EVENT_PRIORITY_LOWEST:
                return "lowest";
            case EVENT_PRIORITY_DEFAULT:
                return "default";
            case EVENT_PRIORITY_ABOVE_DEFAULT:
                return "above_default";
            case EVENT_PRIORITY_HIGHEST:
                return "highest";
            default:
                return "unknown";
        }
    }

    EventPriority event_priority_from_string(const char *priority) {
        if(strcmp(priority, "lowest") == 0) {
            return EVENT_PRIORITY_LOWEST;
        }
        else if(strcmp(priority, "default") == 0) {
            return EVENT_PRIORITY_DEFAULT;
        }
        else if(strcmp(priority, "above_default") == 0) {
            return EVENT_PRIORITY_ABOVE_DEFAULT;
        }
        else if(strcmp(priority, "highest") == 0) {
            return EVENT_PRIORITY_HIGHEST;
        }
        else {
            throw std::invalid_argument("Invalid event priority.");
        }
    }

    template<typename T>
    static int event_context_cancel_method(lua_State *state) {
        auto *event = LUAS_CHECK_OBJECT(state, 1, T);
        if(!event) {
            return luaL_error(state, "Invalid event context in event.cancel method.");
        }
        try {
            event->cancel();
        }
        catch(const std::runtime_error &e) {
            return luaL_error(state, "%s", e.what());
        }
        return 0;
    }

    static void define_event_map_load_context(lua_State *state) {
        LUAS_STRUCT(state, MapLoadEvent);
        LUAS_METHOD_FIELD(state, MapLoadEvent, "getMapName", [](lua_State *state) -> int {
            auto event = LUAS_CHECK_OBJECT(state, 1, MapLoadEvent);
            lua_pushstring(state, event->map_name().c_str());
            return 1;
        });
    }

    void push_map_load_event_context(lua_State *state, const MapLoadEvent &event, bool readonly) noexcept {
        LUAS_PUSH_OBJECT(state, MapLoadEvent, &event, readonly);
    }

    static void define_event_map_loaded_context(lua_State *state) {
        LUAS_STRUCT(state, MapLoadedEvent);
        LUAS_METHOD_FIELD(state, MapLoadedEvent, "getMapName", [](lua_State *state) -> int {
            auto event = LUAS_CHECK_OBJECT(state, 1, MapLoadedEvent);
            lua_pushstring(state, event->map_name().c_str());
            return 1;
        });
    }

    void push_map_loaded_event_context(lua_State *state, const MapLoadedEvent &event, bool readonly) noexcept {
        LUAS_PUSH_OBJECT(state, MapLoadedEvent, &event, readonly);
    }

    static void define_event_player_input_context(lua_State *state) {
        LUAS_STRUCT(state, PlayerInputEvent);
        LUAS_METHOD_FIELD(state, PlayerInputEvent, "cancel", event_context_cancel_method<PlayerInputEvent>);
        LUAS_METHOD_FIELD(state, PlayerInputEvent, "getDevice", [](lua_State *state) -> int {
            auto *event = LUAS_CHECK_OBJECT(state, 1, PlayerInputEvent);
            if(event->device() == LegacyApi::Engine::INPUT_DEVICE_KEYBOARD) {
                lua_pushstring(state, "keyboard");
            }
            else if(event->device() == LegacyApi::Engine::INPUT_DEVICE_GAMEPAD) {
                lua_pushstring(state, "gamepad");
            }
            else {
                lua_pushstring(state, "unknown");
            }
            return 1;
        });
        LUAS_METHOD_FIELD(state, PlayerInputEvent, "getKeyCode", [](lua_State *state) -> int {
            auto *event = LUAS_CHECK_OBJECT(state, 1, PlayerInputEvent);
            try {
                lua_pushinteger(state, event->key_code());
            }
            catch(const std::runtime_error &e) {
                return luaL_error(state, "%s", e.what());
            }
            return 1;
        });
        LUAS_METHOD_FIELD(state, PlayerInputEvent, "getGamepadButton", [](lua_State *state) -> int {
            auto *event = LUAS_CHECK_OBJECT(state, 1, PlayerInputEvent);
            try {
                lua_pushinteger(state, static_cast<int>(event->gamepad_button()));
            }
            catch(const std::runtime_error &e) {
                return luaL_error(state, "%s", e.what());
            }
            return 1;
        });
        LUAS_METHOD_FIELD(state, PlayerInputEvent, "isMapped", [](lua_State *state) -> int {
            auto *event = LUAS_CHECK_OBJECT(state, 1, PlayerInputEvent);
            lua_pushboolean(state, event->mapped());
            return 1;
        });
    }

    void push_player_input_event_context(lua_State *state, const PlayerInputEvent &event, bool readonly) noexcept {
        LUAS_PUSH_OBJECT(state, PlayerInputEvent, &event, readonly);
    }

    void define_event_types(lua_State *state) noexcept {
        define_event_map_load_context(state);
        define_event_map_loaded_context(state);
        define_event_player_input_context(state);
    }
}
