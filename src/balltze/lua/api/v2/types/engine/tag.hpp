// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__LUA__API__V2__TYPES__ENGINE__TAG_HPP
#define BALLTZE__LUA__API__V2__TYPES__ENGINE__TAG_HPP

#include <string>
#include <lua.hpp>
#include <impl/tag/tag.h>
#include "types.hpp"

namespace Balltze::Lua::Api::V2 {
    /**
     * Push a TagGroup to the Lua stack.
     * @param state Lua state
     * @param group The TagGroup to push
     */
    void push_tag_group(lua_State *state, TagGroup group) noexcept;

    /**
     * Get a TagGroup from the Lua stack.
     * @param state Lua state
     * @param idx The index of the value on the Lua stack
     * @return An optional TagGroup, empty if the value is not a valid tag group
     */
    std::optional<TagGroup> get_tag_group(lua_State *state, int idx) noexcept;

    /**
     * Push a TagHandle to the Lua stack.
     * @param state Lua state
     * @param handle The TableResourceHandle to push
     * @param readonly If true, the handle will be read-only in Lua
     */
    void push_tag_handle(lua_State *state, const TagHandle &handle, bool readonly = false) noexcept;

    /**
     * Get a TagHandle from the Lua stack.
     * @param state Lua state
     * @param idx The index of the value on the Lua stack
     * @return An optional TagHandle, empty if the value is not a valid handle
     */
    std::optional<TagHandle> get_tag_handle(lua_State *state, int idx) noexcept;

    /**
     * Push a TagEntry to the Lua stack.
     * @param state Lua state
     * @param entry The TagEntry to push
     * @param readonly If true, the entry will be read-only in Lua
     */
    void push_tag_entry(lua_State *state, const TagEntry &entry, bool readonly = false) noexcept;

    /**
     * Push a TagReference to the Lua stack.
     * @param state Lua state
     * @param dependency The TagReference to push
     * @param readonly If true, the reference will be read-only in Lua
     */
    void push_tag_reference(lua_State *state, const TagReference &reference, bool readonly = false) noexcept;

    /**
     * Push a TagRawData to the Lua stack.
     * @param state Lua state
     * @param raw_data The TagRawData to push
     * @param readonly If true, the raw data will be read-only in Lua
     */
    void push_tag_raw_data(lua_State *state, const TagRawData &raw_data, bool readonly = false) noexcept;

    /**
     * Define all tag types in the Lua state.
     */
    void define_engine_tag_types(lua_State *state) noexcept;
}

#endif 
