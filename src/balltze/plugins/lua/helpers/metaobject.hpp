// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__PLUGINS__LUA__HELPERS__METAOBJECT_HPP
#define BALLTZE__PLUGINS__LUA__HELPERS__METAOBJECT_HPP

#include <functional>
#include <lua.hpp>
#include <balltze/engine/tag.hpp>

namespace Balltze::Plugins {
    int lua_meta_object_read_only__newindex(lua_State *state) noexcept;
    int lua_meta_object__eq(lua_State *state) noexcept;

    template<typename T>
    void lua_push_meta_object(lua_State *state, T *elem, lua_CFunction index, lua_CFunction newindex = nullptr, bool read_only = false) noexcept {
        // Just to be sure
        if(elem == nullptr) {
            lua_pushnil(state);
            return;
        }

        // Create table 
        lua_newtable(state);
        lua_pushlightuserdata(state, reinterpret_cast<void *>(const_cast<T *>(elem)));
        lua_setfield(state, -2, "_data");
        lua_pushinteger(state, typeid(T).hash_code());
        lua_setfield(state, -2, "_type");

        // Create metatable saa
        lua_newtable(state);

        // Set index meta-method
        lua_pushcfunction(state, index);
        lua_setfield(state, -2, "__index");

        // Set new index meta-method
        if(newindex && !read_only) {
            lua_pushcfunction(state, newindex);
        } 
        else {
            lua_pushcfunction(state, lua_meta_object_read_only__newindex);
        }
        lua_setfield(state, -2, "__newindex");

        // Set equals meta-method
        lua_pushcfunction(state, lua_meta_object__eq);
        lua_setfield(state, -2, "__eq");

        // Set metatable for the table
        lua_setmetatable(state, -2);
    }

    template<typename T>
    T *lua_from_meta_object(lua_State *state, int index, bool check_type = false) noexcept {
        if(!lua_istable(state, index)) {
            return nullptr;
        }
        T *elem = nullptr;
        if(check_type) {
            lua_getfield(state, index, "_type");
            if(lua_isinteger(state, -1) && lua_tointeger(state, -1) == typeid(T).hash_code()) {
                lua_getfield(state, index, "_data");
                if(lua_islightuserdata(state, -1)) {
                    elem = reinterpret_cast<T *>(lua_touserdata(state, -1));
                }
            }
            lua_pop(state, 2);
        }
        else {
            lua_getfield(state, index, "_data");
            elem = reinterpret_cast<T *>(lua_touserdata(state, -1));
            lua_pop(state, 1);
        }
        return elem;
    }

    template<typename T>
    int lua_engine_tag_block__index(lua_State *state, std::function<void (lua_State *, T &)> push_function) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto *elem = static_cast<Engine::TagBlock<T> *>(lua_touserdata(state, -1));
        lua_pop(state, 1); 
        auto key = luaL_checkinteger(state, 2);
        if (key < 0 || key >= elem->count) {
            return luaL_error(state, "Index out of bounds");
        }
        push_function(state, elem->offset[key]);
    }
}

#endif
