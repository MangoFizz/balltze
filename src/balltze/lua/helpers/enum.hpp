// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__LUA__API__HELPERS__ENUM_HPP
#define BALLTZE__LUA__API__HELPERS__ENUM_HPP

#include <optional>
#include <lua.hpp>
#include "../libraries/luastruct.hpp"

namespace Balltze::Lua {
    inline void publish_luacstruct_enum(lua_State *state, const char *global_table, const char *table, const char *enum_name, int index) {
        lua_pushvalue(state, index);

        lua_getglobal(state, global_table);
        if(lua_isnil(state, -1)) {
            lua_pop(state, 1);
            luaL_error(state, "Global table %s does not exist", global_table);
            return;
        }

        lua_getfield(state, -1, table);
        if(lua_isnil(state, -1)) {
            lua_pop(state, 2);
            luaL_error(state, "Table %s does not exist", table);
            return;
        }

        lua_pushvalue(state, -3);
        lua_setfield(state, -2, enum_name);
        lua_pop(state, 3);
    }

    /**
     * Get an enum variant value from the Lua stack.
     * @param state Lua state
     * @param idx The index of the value on the Lua stack
     * @param enum_name The name of the enum type
     * @return An optional enum value, empty if the value is not a valid enum variant
     */
    template<typename T>
    std::optional<T> get_luastruct_enum_variant_value(lua_State *state, int idx, const char *enum_name) noexcept {
        std::optional<T> enum_value;
        LuastructEnum *enum_type = luastruct_get_enum(state, enum_name);
        if(lua_isinteger(state, idx)) {
            int32_t value = luaL_checkinteger(state, idx);
            luastruct_get_enum_variant_by_value(state, enum_type, value);
            if(!lua_isnil(state, -1)) {
                enum_value = static_cast<T>(value);
            }
        }
        else if(lua_isstring(state, idx)) {
            const char *name = luaL_checkstring(state, idx);
            luastruct_get_enum_variant_by_name(state, enum_type, name);
            if(!lua_isnil(state, -1)) {
                auto *variant = reinterpret_cast<LuastructEnumVariant *>(luaL_checkudata(state, -1, ENUM_VARIANT_METATABLE_NAME));
                if(variant) {
                    enum_value = static_cast<T>(variant->value);
                }
            }
        }
        else if(lua_isuserdata(state, idx)) {
            auto *variant = reinterpret_cast<LuastructEnumVariant *>(luaL_checkudata(state, idx, ENUM_VARIANT_METATABLE_NAME));
            if(variant) {
                enum_value = static_cast<T>(variant->value);
            }
        }
        return enum_value;
    }

    #define GET_ENUM_VARIANT_VALUE(state, idx, type) \
        get_luastruct_enum_variant_value<type>(state, idx, #type)
}

#endif
