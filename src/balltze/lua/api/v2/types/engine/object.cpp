// SPDX-License-Identifier: GPL-3.0-only

#include "../../../../libraries/luastruct.hpp"
#include "types.hpp"
#include "object.hpp"

namespace Balltze::Lua::Api::V2 {
    static void define_object_handle_type(lua_State *state) noexcept {
        LUAS_STRUCT(state, ObjectHandle);
        LUAS_PRIMITIVE_FIELD(state, ObjectHandle, value, LUAST_UINT32, 0);
        LUAS_PRIMITIVE_FIELD(state, ObjectHandle, id, LUAST_INT16, 0);
        LUAS_PRIMITIVE_FIELD(state, ObjectHandle, index, LUAST_INT16, 0);
        LUAS_METHOD_FIELD(state, ObjectHandle, "isNull", [](lua_State *state) noexcept {
            auto handle = get_table_resource_handle(state, 1);
            if(!handle.has_value()) {
                return luaL_error(state, "Invalid TableResourceHandle object.");
            }
            lua_pushboolean(state, HANDLE_IS_NULL(handle.value()));
            return 1;
        });
        lua_pop(state, 1);
    }

    void push_object_handle(lua_State *state, const ObjectHandle &handle, bool readonly) noexcept {
        LUAS_NEW_OBJECT(state, ObjectHandle, readonly);
        ObjectHandle *object_handle = LUAS_CHECK_OBJECT(state, -1, ObjectHandle);
        object_handle->value = handle.value;
    }

    std::optional<ObjectHandle> get_object_handle(lua_State *state, int idx) noexcept {
        std::optional<ObjectHandle> handle;
        if(lua_istable(state, idx) || lua_isuserdata(state, idx)) {
            lua_getfield(state, idx, "value");
            if(lua_isinteger(state, -1)) {
                handle = ObjectHandle();
                handle->value = luaL_checkinteger(state, -1);
            }
            lua_pop(state, 1);
        } 
        else if(lua_isinteger(state, idx)) {
            handle = ObjectHandle();
            handle->value = luaL_checkinteger(state, idx);
        }
        return handle;
    }

    void define_engine_object_types(lua_State *state) noexcept {
        define_object_handle_type(state);
    }
}
