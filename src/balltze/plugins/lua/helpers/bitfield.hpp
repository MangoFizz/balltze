// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__PLUGINS__LUA__HELPERS__BITFIELD_HPP
#define BALLTZE__PLUGINS__LUA__HELPERS__BITFIELD_HPP

#define lua_bitfield_struct_method(state, bitfieldType, field) \
    [](lua_State *state) -> int { \
        auto *self = reinterpret_cast<bitfieldType *>(lua_touserdata(state, lua_upvalueindex(1))); \
        int args = lua_gettop(state); \
        if(args == 1) { \
            if(lua_isboolean(state, 1)) { \
                self->field = lua_toboolean(state, 1) ? 1 : 0; \
            } else { \
                return luaL_error(state, "Expected boolean"); \
            } \
        } \
        else if(args == 0) { \
            lua_pushboolean(state, self->field); \
        } \
        else { \
            return luaL_error(state, "Expected 0 or 1 arguments"); \
        } \
        return 1; \
    }

#endif
