// SPDX-License-Identifier: GPL-3.0-only

#include "../../../../libraries/luastruct.hpp"
#include "types.hpp"
#include "cache_file.hpp"

namespace Balltze::Lua::Api::V2 {
    static void define_cache_file_type_type(lua_State *state) noexcept {
        LUAS_ENUM(state, CacheFileType);
        LUAS_ENUM_VARIANT(state, CacheFileType, "singlePlayer", CACHE_FILE_TYPE_SINGLE_PLAYER);
        LUAS_ENUM_VARIANT(state, CacheFileType, "multiplayer", CACHE_FILE_TYPE_MULTIPLAYER);
        LUAS_ENUM_VARIANT(state, CacheFileType, "userInterface", CACHE_FILE_TYPE_USER_INTERFACE);
        lua_pop(state, 1);
    }

    static void define_cache_file_header(lua_State *state) noexcept {
        LUAS_STRUCT(state, CacheFileHeader);
        LUAS_PRIMITIVE_FIELD(state, CacheFileHeader, file_size, LUAST_UINT32, LUAS_FIELD_READONLY);
        DEFINE_STRING32_FIELD(state, CacheFileHeader, name);
        DEFINE_STRING32_FIELD(state, CacheFileHeader, build);
        LUAS_ENUM_FIELD(state, CacheFileHeader, game_type, CacheFileType, LUAS_FIELD_READONLY);
        LUAS_PRIMITIVE_FIELD(state, CacheFileHeader, crc32, LUAST_UINT32, LUAS_FIELD_READONLY);
        lua_pop(state, 1);
    }

    void push_cache_file_header(lua_State *state, const CacheFileHeader &header, bool readonly) noexcept {
        LUAS_PUSH_OBJECT(state, CacheFileHeader, &header, readonly);
    }

    void define_engine_cache_file_types(lua_State *state) noexcept {
        define_cache_file_type_type(state);
        define_cache_file_header(state);
    }
}