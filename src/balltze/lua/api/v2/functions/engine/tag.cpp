// SPDX-License-Identifier: GPL-3.0-only

#include <string>
#include <lua.hpp>
#include <impl/tag/tag.h>
#include "../../../../helpers/function_table.hpp"
#include "../../../../helpers/enum.hpp"
#include "../../types.hpp"

namespace Balltze::Lua::Api::V2 {
    static int lua_engine_tag_lookup(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args != 2) {
            return luaL_error(state, "Invalid number of arguments in function Engine.tag.lookup.");
        }

        const char *tag_path = luaL_checkstring(state, 1);

        auto tag_group = get_tag_group(state, 2);
        if(!tag_group) {
            return luaL_error(state, "Invalid tag group in function Engine.tag.lookup.");
        }

        TagHandle tag = tag_lookup(tag_path, *tag_group);
        if(HANDLE_IS_NULL(tag)) {
            lua_pushnil(state);
        }
        else {
            push_tag_handle(state, tag, true);
        }

        return 1;
    }

    static int lua_engine_tag_get_data(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args != 2) {
            return luaL_error(state, "Invalid number of arguments in function Engine.tag.getData.");
        }

        auto tag_handle = get_tag_handle(state, 1);
        if(!tag_handle) {
            return luaL_error(state, "Invalid tag handle in function Engine.tag.getData.");
        }

        auto tag_group = get_tag_group(state, 2);
        if(!tag_group) {
            return luaL_error(state, "Invalid tag group in function Engine.tag.getData.");
        }
        
        TagEntry *entry = tag_get_entry(*tag_handle);
        if(entry->primary_group != *tag_group) {
            return luaL_error(state, "Tag group mismatch in function Engine.tag.getData.");
        }

        push_tag_data(state, entry);

        return 1;
    }

    static int lua_engine_tag_get_entry(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args != 1) {
            return luaL_error(state, "Invalid number of arguments in function Engine.tag.getEntry.");
        }

        auto tag_handle = get_tag_handle(state, 1);
        if(!tag_handle) {
            return luaL_error(state, "Invalid tag handle in function Engine.tag.getEntry.");
        }

        TagEntry *tag_entry = tag_get_entry(*tag_handle);
        if(tag_entry) {
            push_tag_entry(state, *tag_entry, true);
        }
        else {
            lua_pushnil(state);
        }

        return 1;
    }

    static int lua_engine_tag_filter(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args != 2) {
            return luaL_error(state, "Invalid number of arguments in function Engine.tag.filter.");
        }

        auto tag_group = get_tag_group(state, 1);
        if(!tag_group) {
            return luaL_error(state, "Invalid tag group in function Engine.tag.filter.");
        }

        std::string filter = luaL_checkstring(state, 2);

        lua_newtable(state);
        auto *tag_data_header = tag_get_data_header();
        auto tag_count = tag_data_header->tag_count;
        for(std::size_t i = 0; i < tag_count; i++) {
            TagEntry *tag_entry = tag_data_header->tags + i;
            std::string path = tag_entry->path;
            if(path.find(filter) != std::string::npos && tag_entry->primary_group == tag_group.value()) {
                push_tag_entry(state, *tag_entry, true);
                lua_rawseti(state, -2, i + 1);
            }
        }

        return 1;
    }

    static const luaL_Reg engine_tag_functions[] = {
        {"lookupTag", lua_engine_tag_lookup},
        {"getTagData", lua_engine_tag_get_data},
        {"getTagEntry", lua_engine_tag_get_entry},
        {"filterTags", lua_engine_tag_filter},
        {nullptr, nullptr}
    };

    void set_engine_tag_functions(lua_State *state) noexcept {
        create_functions_table(state, "tag", engine_tag_functions);
    }
}
