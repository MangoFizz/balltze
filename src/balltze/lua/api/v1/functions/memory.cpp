// SPDX-License-Identifier: GPL-3.0-only

#include <cstring>
#include <cstdint>
#include <windows.h>
#include <lua.hpp>
#include "../../../helpers/function_table.hpp"

namespace Balltze::Lua {
    template <typename T>
    static int read_int(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 1) {
            auto address = luaL_checkinteger(state, 1);
            lua_pushinteger(state, *reinterpret_cast<T *>(address));
            return 1;
        }
        else {
            return luaL_error(state, "invalid number of arguments in Balltze.memory.readInt function");
        }
    }

    template <typename T>
    static int write_int(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 2) {
            auto address = luaL_checkinteger(state, 1);
            *reinterpret_cast<T *>(address) = static_cast<T>(luaL_checkinteger(state, 2));
            return 0;
        }
        else {
            return luaL_error(state, "invalid number of arguments in Balltze.memory.writeInt function");
        }
    }

    template <typename T>
    static int read_float(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 1) {
            auto address = luaL_checkinteger(state, 1);
            lua_pushnumber(state,*reinterpret_cast<T *>(address));
            return 1;
        }
        else {
            return luaL_error(state, "invalid number of arguments in Balltze.memory.readFloat function");
        }
    }

    template <typename T>
    static int write_float(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 2) {
            auto address = luaL_checkinteger(state, 1);
            *reinterpret_cast<T *>(address) = static_cast<T>(luaL_checknumber(state, 2));
            return 0;
        }
        else {
            return luaL_error(state, "invalid number of arguments in Balltze.memory.writeFloat function");
        }
    }

    static int read_string8(lua_State *state) {
        int args = lua_gettop(state);
        if(args == 1) {
            auto address = luaL_checkinteger(state, 1);
            lua_pushstring(state,reinterpret_cast<const char *>(address));
            return 1;
        }
        else{
            return luaL_error(state, "invalid number of arguments in Balltze.memory.readString8 function");
        }
    }

    static int write_string8(lua_State *state) {
        int args = lua_gettop(state);
        if(args == 2) {
            auto address = luaL_checkinteger(state, 1);
            auto *string = luaL_checkstring(state, 2);
            std::strcpy(reinterpret_cast<char *>(address), string);
            return 0;
        }
        else {
            return luaL_error(state, "invalid number of arguments in Balltze.memory.writeString8 function");
        }
    }

    static int read_bit(lua_State *state) {
        int args = lua_gettop(state);
        if(args == 2) {
            auto address = luaL_checkinteger(state, 1);
            std::uint32_t &address_ptr = *reinterpret_cast<std::uint32_t *>(address);
            std::uint32_t bit = luaL_checkinteger(state,2);
            if(bit >= (sizeof(address_ptr) * 8)) {
                return luaL_error(state, "invalid bit index in Balltze.memory.readBit function");
            }
            lua_pushinteger(state, (address_ptr >> bit) & 1);
            return 1;
        }
        else {
            return luaL_error(state, "invalid number of arguments in Balltze.memory.readBit function");
        }
    }

    static int write_bit(lua_State *state) {
        int args = lua_gettop(state);
        if(args == 3) {
            auto address = luaL_checkinteger(state, 1);
            auto &address_ptr = *reinterpret_cast<std::uint32_t *>(address);
            std::uint32_t bit = luaL_checkinteger(state,2);
            if(bit >= (sizeof(address_ptr) * 8)) {
                return luaL_error(state, "invalid bit index in Balltze.memory.writeBit function");
            }
            char bit_to_set;
            if(lua_isboolean(state, 3)) bit_to_set = lua_toboolean(state, 3) == true;
            else {
                auto new_bit = luaL_checkinteger(state, 3);
                if(new_bit > 1) {
                    return luaL_error(state, "invalid bit value in Balltze.memory.writeBit function");
                }
                bit_to_set = new_bit;
            }
            if(bit_to_set != static_cast<char>((address_ptr >> bit) & 1)) {
                switch((address_ptr >> bit) & 1) {
                    case 0:
                        address_ptr += (1 << bit);
                        break;
                    case 1:
                        address_ptr -= (1 << bit);
                        break;
                }
            }
            return 0;
        }
        else {
            return luaL_error(state, "invalid number of arguments in Balltze.memory.writeBit function");
        }
    }

    static const luaL_Reg memory_functions[] = {
        {"readInt8", read_int<std::int8_t>},
        {"readInt16", read_int<std::int16_t>},
        {"readInt32", read_int<std::int32_t>},
        {"readInt64", read_int<std::int64_t>},
        {"writeInt8", write_int<std::int8_t>},
        {"writeInt16", write_int<std::int16_t>},
        {"writeInt32", write_int<std::int32_t>},
        {"writeInt64", write_int<std::int64_t>},
        {"readFloat", read_float<float>},
        {"readDouble", read_float<double>},
        {"writeFloat", write_float<float>},
        {"writeDouble", write_float<double>},
        {"readString8", read_string8},
        {"writeString8", write_string8},
        {"readBit", read_bit},
        {"writeBit", write_bit},
        {nullptr, nullptr}
    };

    void set_memory_function(lua_State *state) noexcept {
        create_functions_table(state, "memory", memory_functions);
    }
}