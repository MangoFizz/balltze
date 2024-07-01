// SPDX-License-Identifier: GPL-3.0-only

#include <lua.hpp>
#include <balltze/engine.hpp>
#include "../../../../logger.hpp"
#include "../../../plugin.hpp"
#include "../../../loader.hpp"
#include "../../libraries.hpp"
#include "../../types.hpp"

namespace Balltze::Plugins::Lua {
    static int engine_get_tag_data_header(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 0) {
            auto tag_data_header = Engine::get_tag_data_header();
            
            lua_newtable(state);

            lua_pushinteger(state, reinterpret_cast<std::uint32_t>(tag_data_header.tag_array));
            lua_setfield(state, -2, "tagArrayAddress");

            lua_pushinteger(state, tag_data_header.scenario_tag.value);
            lua_setfield(state, -2, "scenarioTagHandle");

            lua_pushinteger(state, tag_data_header.tag_count);
            lua_setfield(state, -2, "tagCount");

            lua_pushinteger(state, tag_data_header.model_part_count);
            lua_setfield(state, -2, "modelPartCount");

            lua_pushinteger(state, tag_data_header.model_data_file_offset);
            lua_setfield(state, -2, "modelDataFileOffset");

            lua_pushinteger(state, tag_data_header.vertex_size);
            lua_setfield(state, -2, "vertexSize");

            lua_pushinteger(state, tag_data_header.model_data_size);
            lua_setfield(state, -2, "modelDataSize");

            return 1;
        }
        else {
            return luaL_error(state, "Invalid number of arguments in function Engine.tag.getTagDataHeader.");
        }
    }

    static int engine_get_tag(lua_State *state) noexcept {
        int args = lua_gettop(state);
        Engine::Tag *tag_entry = nullptr;
        if(args == 1) {
            auto tag_handle = get_engine_resource_handle(state, 1);
            if(!tag_handle || tag_handle->is_null()) {
                return luaL_error(state, "Invalid tag handle in function Engine.tag.getTag.");
            }
            if(tag_handle->id != 0) {
                tag_entry = Engine::get_tag(*tag_handle);
            }
            else {
                tag_entry = Engine::get_tag(tag_handle->index);
            }
        }
        else if(args == 2) {
            const char *tag_path = luaL_checkstring(state, 1);
            const char *tag_class_str = luaL_checkstring(state, 2);
            auto tag_class = Engine::tag_class_from_string(tag_class_str);
            if(tag_class != Engine::TagClassInt::TAG_CLASS_NULL) {
                tag_entry = Engine::get_tag(tag_path, tag_class);
            }
            else {
                return luaL_error(state, "Invalid tag class.");
            }
        }
        else {
            return luaL_error(state, "Invalid number of arguments in function Engine.tag.getTag.");
        }

        if(tag_entry) {
            push_meta_engine_tag(state, tag_entry);
        }
        else {
            lua_pushnil(state);
        }

        return 1;
    }

    static int engine_find_tags(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 0 || args == 1 || args == 2) {
            std::optional<std::string> path_keyword;
            std::optional<Engine::TagClassInt> tag_class;
            if(args >= 1) {
                if(lua_isstring(state, 1)) {
                    path_keyword = luaL_checkstring(state, 1); 
                }
                else {
                    return luaL_error(state, "Invalid path keyword in function Engine.tag.findTags.");
                }
            }
            if(args == 2) {
                if(lua_isstring(state, 2)) {
                    tag_class = Engine::tag_class_from_string(std::string(lua_tostring(state, 2)));
                    if(tag_class == Engine::TagClassInt::TAG_CLASS_NULL) {
                        return luaL_error(state, "Invalid tag class in function Engine.tag.findTags.");
                    }
                }
                else {
                    return luaL_error(state, "Invalid tag class in function Engine.tag.findTags.");
                }
            }
            auto tags = Engine::find_tags(path_keyword, tag_class);
            lua_newtable(state);
            for(std::size_t i = 0; i < tags.size(); i++) {
                push_meta_engine_tag(state, tags[i]);
                lua_rawseti(state, -2, i + 1);
            }
            return 1;
        }
        else {
            return luaL_error(state, "Invalid number of arguments in function Engine.tag.findTags.");
        }
    }

    static const luaL_Reg engine_tag_functions[] = {
        {"getTagDataHeader", engine_get_tag_data_header},
        {"getTag", engine_get_tag},
        {nullptr, nullptr}
    };

    void set_engine_tag_functions(lua_State *state) noexcept {
        lua_newtable(state);
        luaL_setfuncs(state, engine_tag_functions, 0);
        lua_setfield(state, -2, "tag");
    }
}
