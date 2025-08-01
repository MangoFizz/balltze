// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__LUA__API__HELPERS__BITFIELD_HPP
#define BALLTZE__LUA__API__HELPERS__BITFIELD_HPP

#include <lua.hpp>
#include "../libraries/luastruct.hpp"

#define lua_bitfield_struct_method(state, bitfieldType, field) \
    +[](lua_State *state) -> int { \
        auto *self = luacs_to_object(state, 1, bitfieldType); \
        int args = lua_gettop(state); \
        if(args == 2) { \
            if(lua_isboolean(state, 2)) { \
                self->field = lua_toboolean(state, 2) ? 1 : 0; \
            } else { \
                return luaL_error(state, "Expected boolean"); \
            } \
        } \
        else if(args == 1) { \
            lua_pushboolean(state, self->field); \
        } \
        else { \
            return luaL_error(state, "Expected 0 or 1 arguments"); \
        } \
        return 1; \
    }

#define luastruct_bitfield_method(state, bitfield_type, field) \
    +[](lua_State *state) -> int { \
        auto *self = reinterpret_cast<bitfield_type *>(luaL_checkudata(state, 1, OBJECT_METATABLE_NAME)); \
        int args = lua_gettop(state); \
        if(args == 2) { \
            if(lua_isboolean(state, 2)) { \
                self->field = lua_toboolean(state, 2) ? 1 : 0; \
            } else { \
                return luaL_error(state, "Expected boolean"); \
            } \
        } \
        else if(args == 1) { \
            lua_pushboolean(state, self->field); \
        } \
        else { \
            return luaL_error(state, "Expected 0 or 1 arguments"); \
        } \
        return 1; \
    }

#endif
