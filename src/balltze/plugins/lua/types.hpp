// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__PLUGINS__LUA__TYPES_HPP
#define BALLTZE__PLUGINS__LUA__TYPES_HPP

#include <chrono>
#include "types/balltze_event.hpp"
#include "types/engine_types.hpp"
#include "types/engine_tag.hpp"
#include "types/engine_tag_data.hpp"
#include "types/engine_gamestate_object.hpp"
#include "types/engine_gamestate_device_object.hpp"
#include "types/engine_gamestate_item_object.hpp"
#include "types/engine_gamestate_player.hpp"
#include "types/engine_gamestate_unit_object.hpp"
#include "types/engine_rasterizer.hpp"
#include "types/engine_map.hpp"
#include "types/engine_user_interface.hpp"
#include "../../logger.hpp"

namespace Balltze::Plugins::Lua {
    inline void define_engine_structs(lua_State *state) noexcept {
        define_engine_types(state);
        define_engine_tag_types(state);
        define_engine_tag_data_types(state);
        define_engine_gamestate_object_types(state);
        define_engine_gamestate_item_object_types(state);
        define_engine_gamestate_device_object_types(state);
        define_engine_gamestate_unit_object_types(state);
        declare_engine_gamestate_player_types(state);
        define_engine_rasterizer_types(state);
        define_engine_map_types(state);
        define_engine_user_interface_types(state);
        define_balltze_event_types(state);
    }
}

#endif
