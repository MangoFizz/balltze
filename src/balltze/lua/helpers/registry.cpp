// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/utils.hpp>
#include "../../logger.hpp"
#include "registry.hpp"

namespace Balltze::Lua {
    void get_or_create_registry_table(lua_State *state, const char *name) {
        auto balltze_module = Balltze::get_current_module();
        lua_pushlightuserdata(state, balltze_module);
        lua_gettable(state, LUA_REGISTRYINDEX);
        if(lua_isnil(state, -1)) {
            logger.error("Could not find balltze Lua registry table");
            return;
        }
        lua_getfield(state, -1, name);
        if(lua_isnil(state, -1)) {
            lua_pop(state, 1);
            lua_newtable(state);
            lua_pushvalue(state, -1);
            lua_setfield(state, -3, name);
        }
        lua_remove(state, -2);
    }
}
