// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__LUA__API__V2__TYPES__ENGINE__CACHE_FILE_HPP
#define BALLTZE__LUA__API__V2__TYPES__ENGINE__CACHE_FILE_HPP

#include <lua.hpp>
#include <impl/cache_file/cache_file.h>

namespace Balltze::Lua::Api::V2 {
    /**
     * Push a CacheFileHeader to the Lua stack.
     * @param state Lua state
     * @param header The CacheFileHeader to push
     * @param readonly If true, the header will be read-only in Lua
     */
    void push_cache_file_header(lua_State *state, const CacheFileHeader &header, bool readonly = false) noexcept;

    /**
     * Define all cache file types in the Lua state.
     */
    void define_engine_cache_file_types(lua_State *state) noexcept;
}

#endif
