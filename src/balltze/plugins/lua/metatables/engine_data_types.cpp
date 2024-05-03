// SPDX-License-Identifier: GPL-3.0-only

#include <cstring>
#include <balltze/engine.hpp>
#include <lua.hpp>
#include "../helpers.hpp"
#include "../metatables.hpp"

namespace Balltze::Plugins {
    static int lua_meta_engine_tag__index(lua_State *state) noexcept {
        auto *tag = lua_from_meta_object<Engine::Tag>(state, 1);
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {
            return luaL_error(state, "Invalid key in EngineTag metaobject.");
        }

        std::string field = key;
        if(field == "primaryClass") {
            lua_pushstring(state, Engine::tag_class_to_string(tag->primary_class).c_str());
        }
        else if(field == "secondaryClass") {
            lua_pushstring(state, Engine::tag_class_to_string(tag->secondary_class).c_str());
        }
        else if(field == "tertiaryClass") {
            lua_pushstring(state, Engine::tag_class_to_string(tag->tertiary_class).c_str());
        }
        else if(field == "handle") {
            lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&tag->handle));
        }
        else if(field == "path") {
            lua_pushstring(state, tag->path);
        }
        else if(field == "dataAddress") {
            lua_pushinteger(state, reinterpret_cast<std::uint32_t>(tag->data));
        }
        else if(field == "indexed") {
            lua_pushboolean(state, tag->indexed);
        }
        else {
            return luaL_error(state, "Invalid key");
        }
        return 1;
    }

    static int lua_meta_engine_tag__newindex(lua_State *state) noexcept {
        auto *tag = lua_from_meta_object<Engine::Tag>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EngineTag metaobject.");
        }

        std::string field = key;
        if(field == "primaryClass") {
            auto primary_class = Engine::tag_class_from_string(luaL_checkstring(state, 3));
            if(primary_class == Engine::TAG_CLASS_NULL) {
                return luaL_error(state, "Invalid tag class on primaryClass field in EngineTag metaobject.");
            }
            tag->primary_class = primary_class;
        }
        else if(field == "secondaryClass") {
            auto secondary_class = Engine::tag_class_from_string(luaL_checkstring(state, 3));
            if(secondary_class == Engine::TAG_CLASS_NULL) {
                return luaL_error(state, "Invalid tag class on secondaryClass field in EngineTag metaobject.");
            }
            tag->secondary_class = secondary_class;
        }
        else if(field == "tertiaryClass") {
            auto tertiary_class = Engine::tag_class_from_string(luaL_checkstring(state, 3));
            if(tertiary_class == Engine::TAG_CLASS_NULL) {
                return luaL_error(state, "Invalid tag class on tertiaryClass field in EngineTag metaobject.");
            }
            tag->tertiary_class = tertiary_class;
        }
        else if(field == "handle") {
            return luaL_error(state, "Cannot modify tag handle in EngineTag metaobject.");
        }
        else if(field == "path") {
            return luaL_error(state, "Cannot modify tag path in EngineTag metaobject.");
        }
        else if(field == "dataAddress") {
            auto data = luaL_checkinteger(state, 3);
            tag->data = reinterpret_cast<std::byte *>(data);
        }
        else if(field == "indexed") {
            return luaL_error(state, "Cannot modify indexed flag in EngineTag metaobject.");
        }
        else {
            return luaL_error(state, "Invalid field in EngineTag metaobject.");
        }
        return 0;
    }

    void lua_push_meta_engine_tag(lua_State *state, Engine::Tag &tag, bool read_only) noexcept {
        lua_push_meta_object(state, &tag, lua_meta_engine_tag__index, lua_meta_engine_tag__newindex, read_only);
    }

    static int lua_engine_color_a_r_g_b_int__index(lua_State *state) {
        auto *color = lua_from_meta_object<Engine::ColorARGBInt>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EngineColorARGBInt metaobject.");
        } 
        
        std::string field = key; 
        if(field == "red") { 
            lua_pushinteger(state, color->red); 
            return 1; 
        }
        else if(field == "green") { 
            lua_pushinteger(state, color->green); 
            return 1; 
        }
        else if(field == "blue") { 
            lua_pushinteger(state, color->blue); 
            return 1; 
        }
        else if(field == "alpha") { 
            lua_pushinteger(state, color->alpha); 
            return 1; 
        }
        else { 
            return luaL_error(state, "Invalid field in EngineColorARGBInt metaobject.");
        }
    }

    static int lua_engine_color_a_r_g_b_int__newindex(lua_State *state) {
        auto *color = lua_from_meta_object<Engine::ColorARGBInt>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EngineColorARGBInt metaobject.");
        } 
        
        std::string field = key; 
        if(field == "red") { 
            color->red = static_cast<std::uint8_t>(luaL_checkinteger(state, 3)); 
        }
        else if(field == "green") { 
            color->green = static_cast<std::uint8_t>(luaL_checkinteger(state, 3)); 
        }
        else if(field == "blue") { 
            color->blue = static_cast<std::uint8_t>(luaL_checkinteger(state, 3)); 
        }
        else if(field == "alpha") { 
            color->alpha = static_cast<std::uint8_t>(luaL_checkinteger(state, 3)); 
        }
        else { 
            return luaL_error(state, "Invalid field in EngineColorARGBInt metaobject.");
        }
        return 0;
    }

    void lua_push_meta_engine_color_a_r_g_b_int(lua_State *state, Engine::ColorARGBInt &color, bool read_only) noexcept {
        lua_push_meta_object(state, &color, lua_engine_color_a_r_g_b_int__index, lua_engine_color_a_r_g_b_int__newindex, read_only); 
    }

    static int lua_engine_tag_dependency__index(lua_State *state) {
        auto *dependency = lua_from_meta_object<Engine::TagDependency>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EngineTagDependency metaobject.");
        } 
        
        std::string field = key; 
        if(field == "handle") { 
            lua_push_engine_resource_handle(state, reinterpret_cast<Engine::ResourceHandle *>(&dependency->tag_handle));
            return 1; 
        }
        else if(field == "class") {
            auto tag_class = Engine::tag_class_to_string(*reinterpret_cast<Engine::TagClassInt *>(dependency->tag_class)); 
            lua_pushstring(state, tag_class.c_str()); 
            return 1; 
        }
        else if(field == "path") { 
            char path[dependency->path_size + 1];
            std::memcpy(path, dependency->path, dependency->path_size);
            path[dependency->path_size] = '\0';
            lua_pushstring(state, path);
            return 1; 
        }
        else { 
            return luaL_error(state, "Invalid field in EngineTagDependency metaobject.");
        }
    }

    static int lua_engine_tag_dependency__newindex(lua_State *state) noexcept {
        auto *dependency = lua_from_meta_object<Engine::TagDependency>(state, 1); 
        auto *key = luaL_checkstring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EngineTagDependency metaobject.");
        }

        std::string field = key;
        if(field == "handle") { 
            dependency->tag_handle.handle = luaL_checkinteger(state, 3); 
        }
        else if(field == "class") {
            auto *tag_class = lua_tostring(state, 3);
            if(tag_class == nullptr) {
                return luaL_error(state, "Invalid tag class in EngineTagDependency metaobject.");
            }
            *reinterpret_cast<Engine::TagClassInt *>(dependency->tag_class) = Engine::tag_class_from_string(tag_class);
        }
        else if(field == "path") { 
            return luaL_error(state, "Cannot modify path in EngineTagDependency metaobject.");
        }
        else { 
            return luaL_error(state, "Invalid field in EngineTagDependency metaobject.");
        }
        return 0;
    }

    void lua_push_meta_engine_tag_dependency(lua_State *state, Engine::TagDependency &dependency, bool read_only) noexcept {
        lua_push_meta_object(state, &dependency, lua_engine_tag_dependency__index, lua_engine_tag_dependency__newindex, read_only); 
    }

    static int lua_engine_point2_d__index(lua_State *state) noexcept {
        auto *point = lua_from_meta_object<Engine::Point2D>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EnginePoint2D metaobject.");
        } 
        
        std::string field = key; 
        if(field == "x") { 
            lua_pushnumber(state, point->x); 
            return 1; 
        }
        else if(field == "y") { 
            lua_pushnumber(state, point->y); 
            return 1; 
        }
        else { 
            return luaL_error(state, "Invalid field in EnginePoint2D metaobject.");
        }
    }

    static int lua_engine_point2_d__newindex(lua_State *state) noexcept {
        auto *point = lua_from_meta_object<Engine::Point2D>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EnginePoint2D metaobject.");
        } 
        
        std::string field = key; 
        if(field == "x") { 
            point->x = luaL_checknumber(state, 3); 
        }
        else if(field == "y") { 
            point->y = luaL_checknumber(state, 3); 
        }
        else { 
            return luaL_error(state, "Invalid field in EnginePoint2D metaobject.");
        }
        return 0;
    }

    void lua_push_meta_engine_point2_d(lua_State *state, Engine::Point2D &point, bool read_only) noexcept {
        lua_push_meta_object(state, &point, lua_engine_point2_d__index, lua_engine_point2_d__newindex, read_only); 
    }

    static int lua_engine_point3_d__index(lua_State *state) noexcept {
        auto *point = lua_from_meta_object<Engine::Point3D>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EnginePoint3D metaobject.");
        } 
        
        std::string field = key; 
        if(field == "x") { 
            lua_pushinteger(state, point->x); 
            return 1; 
        }
        else if(field == "y") { 
            lua_pushinteger(state, point->y); 
            return 1; 
        }
        else if(field == "z") { 
            lua_pushinteger(state, point->z); 
            return 1; 
        }
        else { 
            return luaL_error(state, "Invalid field in EnginePoint3D metaobject.");
        }
    }

    static int lua_engine_point3_d__newindex(lua_State *state) noexcept {
        auto *point = lua_from_meta_object<Engine::Point3D>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EnginePoint3D metaobject.");
        } 
        
        std::string field = key; 
        if(field == "x") { 
            point->x = luaL_checkinteger(state, 3); 
        }
        else if(field == "y") { 
            point->y = luaL_checkinteger(state, 3); 
        }
        else if(field == "z") { 
            point->z = luaL_checkinteger(state, 3); 
        }
        else { 
            return luaL_error(state, "Invalid field in EnginePoint3D metaobject.");
        }
        return 0;
    }

    void lua_push_meta_engine_point3_d(lua_State *state, Engine::Point3D &point, bool read_only) noexcept {
        lua_push_meta_object(state, &point, lua_engine_point3_d__index, lua_engine_point3_d__newindex, read_only); 
    }

    static int lua_engine_tag_data_offset__index(lua_State *state) noexcept {
        auto *offset = lua_from_meta_object<Engine::TagDataOffset>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EngineTagDataOffset metaobject.");
        } 
        
        std::string field = key; 
        if(field == "size") { 
            lua_pushinteger(state, offset->size); 
            return 1; 
        }
        else if(field == "external") { 
            lua_pushboolean(state, offset->external);
            return 1; 
        }
        else if(field == "fileOffset") { 
            auto file_offset = offset->file_offset;
            if(file_offset == 0) {
                lua_pushnil(state);
            }
            else {
                lua_pushinteger(state, file_offset);
            }
            return 1; 
        }
        else if(field == "pointer") {
            auto pointer = offset->pointer;
            if(pointer) {
                lua_pushinteger(state, reinterpret_cast<std::uintptr_t>(pointer));
            }
            else {
                lua_pushnil(state);
            }
            return 1; 
        }
        else { 
            return luaL_error(state, "Invalid field in EngineTagDataOffset metaobject.");
        }
    }

    static int lua_engine_tag_data_offset__newindex(lua_State *state) noexcept {
        auto *offset = lua_from_meta_object<Engine::TagDataOffset>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EngineTagDataOffset metaobject.");
        } 
        
        std::string field = key; 
        if(field == "size") { 
            offset->size = luaL_checkinteger(state, 3); 
        }
        else if(field == "external") { 
            offset->external = lua_toboolean(state, 3);
        }
        else if(field == "fileOffset") { 
            offset->file_offset = luaL_checkinteger(state, 3);
        }
        else if(field == "pointer") {
            offset->pointer = reinterpret_cast<std::byte *>(luaL_checkinteger(state, 3));
        }
        else { 
            return luaL_error(state, "Invalid field in EngineTagDataOffset metaobject.");
        }
        return 0;
    }

    void lua_push_meta_engine_tag_data_offset(lua_State *state, Engine::TagDataOffset &offset, bool read_only) noexcept {
        lua_push_meta_object(state, &offset, lua_engine_tag_data_offset__index, lua_engine_tag_data_offset__newindex, read_only); 
    }

    static int lua_engine_color_a_r_g_b__index(lua_State *state) {
        auto *color = lua_from_meta_object<Engine::ColorARGB>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EngineColorARGB metaobject.");
        } 
        
        std::string field = key; 
        if(field == "alpha") { 
            lua_pushnumber(state, color->alpha); 
            return 1; 
        }
        else if(field == "red") { 
            lua_pushnumber(state, color->red); 
            return 1; 
        }
        else if(field == "green") { 
            lua_pushnumber(state, color->green); 
            return 1; 
        }
        else if(field == "blue") { 
            lua_pushnumber(state, color->blue); 
            return 1; 
        }
        else { 
            return luaL_error(state, "Invalid field in EngineColorARGB metaobject.");
        }
    }

    static int lua_engine_color_a_r_g_b__newindex(lua_State *state) noexcept {
        auto *color = lua_from_meta_object<Engine::ColorARGB>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EngineColorARGB metaobject.");
        } 
        
        std::string field = key; 
        if(field == "alpha") { 
            color->alpha = luaL_checknumber(state, 3); 
        }
        else if(field == "red") { 
            color->red = luaL_checknumber(state, 3); 
        }
        else if(field == "green") { 
            color->green = luaL_checknumber(state, 3); 
        }
        else if(field == "blue") { 
            color->blue = luaL_checknumber(state, 3); 
        }
        else { 
            return luaL_error(state, "Invalid field in EngineColorARGB metaobject.");
        }
        return 0;
    }

    void lua_push_meta_engine_color_a_r_g_b(lua_State *state, Engine::ColorARGB &color, bool read_only) noexcept {
        lua_push_meta_object(state, &color, lua_engine_color_a_r_g_b__index, lua_engine_color_a_r_g_b__newindex, read_only); 
    }

    static int lua_engine_rectangle2_d__index(lua_State *state) {
        auto *rectangle = lua_from_meta_object<Engine::Rectangle2D>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EngineRectangle2D metaobject.");
        } 
        
        std::string field = key; 
        if(field == "top") { 
            lua_pushinteger(state, rectangle->top); 
            return 1; 
        }
        else if(field == "left") { 
            lua_pushinteger(state, rectangle->left); 
            return 1; 
        }
        else if(field == "bottom") { 
            lua_pushinteger(state, rectangle->bottom); 
            return 1; 
        }
        else if(field == "right") { 
            lua_pushinteger(state, rectangle->right); 
            return 1; 
        }
        else { 
            return luaL_error(state, "Invalid field in EngineRectangle2D metaobject.");
        }
    }

    static int lua_engine_rectangle2_d__newindex(lua_State *state) noexcept {
        auto *rectangle = lua_from_meta_object<Engine::Rectangle2D>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EngineRectangle2D metaobject.");
        } 
        
        std::string field = key; 
        if(field == "top") { 
            rectangle->top = luaL_checkinteger(state, 3); 
        }
        else if(field == "left") { 
            rectangle->left = luaL_checkinteger(state, 3); 
        }
        else if(field == "bottom") { 
            rectangle->bottom = luaL_checkinteger(state, 3); 
        }
        else if(field == "right") { 
            rectangle->right = luaL_checkinteger(state, 3); 
        }
        else { 
            return luaL_error(state, "Invalid field in EngineRectangle2D metaobject.");
        }
        return 0;
    }

    void lua_push_meta_engine_rectangle2_d(lua_State *state, Engine::Rectangle2D &rectangle, bool read_only) noexcept {
        lua_push_meta_object(state, &rectangle, lua_engine_rectangle2_d__index, lua_engine_rectangle2_d__newindex, read_only); 
    }

    static int lua_engine_rectangle2_d_f__index(lua_State *state) {
        auto *rectangle = lua_from_meta_object<Engine::Rectangle2DF>(state, 1); 
        auto *key = lua_tostring(state, 2); 
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EngineRectangle2DF metaobject.");
        } 
        
        std::string field = key;
        if(field == "top") { 
            lua_pushnumber(state, rectangle->top); 
            return 1; 
        }
        else if(field == "left") { 
            lua_pushnumber(state, rectangle->left); 
            return 1; 
        }
        else if(field == "bottom") { 
            lua_pushnumber(state, rectangle->bottom); 
            return 1; 
        }
        else if(field == "right") { 
            lua_pushnumber(state, rectangle->right); 
            return 1; 
        }
        else { 
            return luaL_error(state, "Invalid field in EngineRectangle2DF metaobject.");
        }
    }

    static int lua_engine_rectangle2_d_f__newindex(lua_State *state) noexcept {
        auto *rectangle = lua_from_meta_object<Engine::Rectangle2DF>(state, 1); 
        auto *key = lua_tostring(state, 2); 
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EngineRectangle2DF metaobject.");
        } 
        
        std::string field = key;
        if(field == "top") { 
            rectangle->top = luaL_checknumber(state, 3); 
        }
        else if(field == "left") { 
            rectangle->left = luaL_checknumber(state, 3); 
        }
        else if(field == "bottom") { 
            rectangle->bottom = luaL_checknumber(state, 3); 
        }
        else if(field == "right") { 
            rectangle->right = luaL_checknumber(state, 3); 
        }
        else { 
            return luaL_error(state, "Invalid field in EngineRectangle2DF metaobject.");
        }
        return 0;
    }

    void lua_push_meta_engine_rectangle2_d_f(lua_State *state, Engine::Rectangle2DF &rectangle, bool read_only) noexcept {
        lua_push_meta_object(state, &rectangle, lua_engine_rectangle2_d_f__index, lua_engine_rectangle2_d_f__newindex, read_only); 
    }

    static int lua_engine_point2_d_int__index(lua_State *state) {
        auto *point = lua_from_meta_object<Engine::Point2DInt>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EnginePoint2DInt metaobject.");
        } 
        
        std::string field = key; 
        if(field == "x") { 
            lua_pushinteger(state, point->x); 
            return 1; 
        }
        else if(field == "y") { 
            lua_pushinteger(state, point->y); 
            return 1; 
        }
        else { 
            return luaL_error(state, "Invalid field in EnginePoint2DInt metaobject.");
        }
    }

    static int lua_engine_point2_d_int__newindex(lua_State *state) noexcept {
        auto *point = lua_from_meta_object<Engine::Point2DInt>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EnginePoint2DInt metaobject.");
        } 
        
        std::string field = key; 
        if(field == "x") { 
            point->x = luaL_checkinteger(state, 3); 
        }
        else if(field == "y") { 
            point->y = luaL_checkinteger(state, 3); 
        }
        else { 
            return luaL_error(state, "Invalid field in EnginePoint2DInt metaobject.");
        }
        return 0;
    }

    void lua_push_meta_engine_point2_d_int(lua_State *state, Engine::Point2DInt &point, bool read_only) noexcept {
        lua_push_meta_object(state, &point, lua_engine_point2_d_int__index, lua_engine_point2_d_int__newindex, read_only); 
    }

    static int lua_engine_euler2_d__index(lua_State *state) {
        auto *euler = lua_from_meta_object<Engine::Euler2D>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EngineEuler2D metaobject.");
        } 
        
        std::string field = key; 
        if(field == "yaw") { 
            lua_pushnumber(state, euler->yaw); 
            return 1; 
        }
        else if(field == "pitch") { 
            lua_pushnumber(state, euler->pitch); 
            return 1; 
        }
        else { 
            return luaL_error(state, "Invalid field in EngineEuler2D metaobject.");
        }
    }

    static int lua_engine_euler2_d__newindex(lua_State *state) noexcept {
        auto *euler = lua_from_meta_object<Engine::Euler2D>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EngineEuler2D metaobject.");
        } 
        
        std::string field = key; 
        if(field == "yaw") { 
            euler->yaw = luaL_checknumber(state, 3); 
        }
        else if(field == "pitch") { 
            euler->pitch = luaL_checknumber(state, 3); 
        }
        else { 
            return luaL_error(state, "Invalid field in EngineEuler2D metaobject.");
        }
        return 0;
    }

    void lua_push_meta_engine_euler2_d(lua_State *state, Engine::Euler2D &euler, bool read_only) noexcept {
        lua_push_meta_object(state, &euler, lua_engine_euler2_d__index, lua_engine_euler2_d__newindex, read_only); 
    }

    static int lua_engine_euler3_d__index(lua_State *state) {
        auto *euler = lua_from_meta_object<Engine::Euler3D>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EngineEuler3D metaobject.");
        } 
        
        std::string field = key; 
        if(field == "yaw") { 
            lua_pushnumber(state, euler->yaw); 
            return 1; 
        }
        else if(field == "pitch") { 
            lua_pushnumber(state, euler->pitch); 
            return 1; 
        }
        else if(field == "roll") { 
            lua_pushnumber(state, euler->roll); 
            return 1; 
        }
        else { 
            return luaL_error(state, "Invalid field in EngineEuler3D metaobject.");
        }
    }

    static int lua_engine_euler3_d__newindex(lua_State *state) noexcept {
        auto *euler = lua_from_meta_object<Engine::Euler3D>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EngineEuler3D metaobject.");
        } 
        
        std::string field = key; 
        if(field == "yaw") { 
            euler->yaw = luaL_checknumber(state, 3); 
        }
        else if(field == "pitch") { 
            euler->pitch = luaL_checknumber(state, 3); 
        }
        else if(field == "roll") { 
            euler->roll = luaL_checknumber(state, 3); 
        }
        else { 
            return luaL_error(state, "Invalid field in EngineEuler3D metaobject.");
        }
        return 0;
    }

    void lua_push_meta_engine_euler3_d(lua_State *state, Engine::Euler3D &euler, bool read_only) noexcept {
        lua_push_meta_object(state, &euler, lua_engine_euler3_d__index, lua_engine_euler3_d__newindex, read_only); 
    }

    static int lua_engine_euler3_d_p_y_r__index(lua_State *state) {
        auto *euler = lua_from_meta_object<Engine::Euler3DPYR>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EngineEuler3DPYR metaobject.");
        } 
        
        std::string field = key; 
        if(field == "yaw") { 
            lua_pushnumber(state, euler->yaw); 
            return 1; 
        }
        else if(field == "pitch") { 
            lua_pushnumber(state, euler->pitch); 
            return 1; 
        }
        else if(field == "roll") { 
            lua_pushnumber(state, euler->roll); 
            return 1; 
        }
        else { 
            return luaL_error(state, "Invalid field in EngineEuler3DPYR metaobject.");
        }
    }

    static int lua_engine_euler3_d_p_y_r__newindex(lua_State *state) noexcept {
        auto *euler = lua_from_meta_object<Engine::Euler3DPYR>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EngineEuler3DPYR metaobject.");
        } 
        
        std::string field = key; 
        if(field == "yaw") { 
            euler->yaw = luaL_checknumber(state, 3); 
        }
        else if(field == "pitch") { 
            euler->pitch = luaL_checknumber(state, 3); 
        }
        else if(field == "roll") { 
            euler->roll = luaL_checknumber(state, 3); 
        }
        else { 
            return luaL_error(state, "Invalid field in EngineEuler3DPYR metaobject.");
        }
        return 0;
    }

    void lua_push_meta_engine_euler3_d_p_y_r(lua_State *state, Engine::Euler3DPYR &euler, bool read_only) noexcept {
        lua_push_meta_object(state, &euler, lua_engine_euler3_d_p_y_r__index, lua_engine_euler3_d_p_y_r__newindex, read_only); 
    }

    static int lua_engine_vector2_d__index(lua_State *state) {
        auto *vector = lua_from_meta_object<Engine::Vector2D>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EngineVector2D metaobject.");
        } 
        
        std::string field = key; 
        if(field == "i") { 
            lua_pushnumber(state, vector->i); 
            return 1; 
        }
        else if(field == "j") { 
            lua_pushnumber(state, vector->j); 
            return 1; 
        }
        else { 
            return luaL_error(state, "Invalid field in EngineVector2D metaobject.");
        }
    }

    static int lua_engine_vector2_d__newindex(lua_State *state) noexcept {
        auto *vector = lua_from_meta_object<Engine::Vector2D>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EngineVector2D metaobject.");
        } 
        
        std::string field = key; 
        if(field == "i") { 
            vector->i = luaL_checknumber(state, 3); 
        }
        else if(field == "j") { 
            vector->j = luaL_checknumber(state, 3); 
        }
        else { 
            return luaL_error(state, "Invalid field in EngineVector2D metaobject.");
        }
        return 0;
    }

    void lua_push_meta_engine_vector2_d(lua_State *state, Engine::Vector2D &vector, bool read_only) noexcept {
        lua_push_meta_object(state, &vector, lua_engine_vector2_d__index, lua_engine_vector2_d__newindex, read_only); 
    }

    static int lua_engine_vector3_d__index(lua_State *state) {
        auto *vector = lua_from_meta_object<Engine::Vector3D>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EngineVector3D metaobject.");
        } 
        
        std::string field = key; 
        if(field == "i") { 
            lua_pushnumber(state, vector->i); 
            return 1; 
        }
        else if(field == "j") { 
            lua_pushnumber(state, vector->j); 
            return 1; 
        }
        else if(field == "k") { 
            lua_pushnumber(state, vector->k); 
            return 1; 
        }
        else { 
            return luaL_error(state, "Invalid field in EngineVector3D metaobject.");
        }
    }

    static int lua_engine_vector3_d__newindex(lua_State *state) noexcept {
        auto *vector = lua_from_meta_object<Engine::Vector3D>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EngineVector3D metaobject.");
        } 
        
        std::string field = key; 
        if(field == "i") { 
            vector->i = luaL_checknumber(state, 3); 
        }
        else if(field == "j") { 
            vector->j = luaL_checknumber(state, 3); 
        }
        else if(field == "k") { 
            vector->k = luaL_checknumber(state, 3); 
        }
        else { 
            return luaL_error(state, "Invalid field in EngineVector3D metaobject.");
        }
        return 0;
    }

    void lua_push_meta_engine_vector3_d(lua_State *state, Engine::Vector3D &vector, bool read_only) noexcept {
        lua_push_meta_object(state, &vector, lua_engine_vector3_d__index, lua_engine_vector3_d__newindex, read_only); 
    }

    static int lua_engine_color_r_g_b__index(lua_State *state) noexcept {
        auto *color = lua_from_meta_object<Engine::ColorRGB>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EngineColorRGB metaobject.");
        } 
        
        std::string field = key; 
        if(field == "red") { 
            lua_pushnumber(state, color->red); 
            return 1; 
        }
        else if(field == "green") { 
            lua_pushnumber(state, color->green); 
            return 1; 
        }
        else if(field == "blue") { 
            lua_pushnumber(state, color->blue); 
            return 1; 
        }
        else { 
            return luaL_error(state, "Invalid field in EngineColorRGB metaobject.");
        }
    }

    static int lua_engine_color_r_g_b__newindex(lua_State *state) noexcept {
        auto *color = lua_from_meta_object<Engine::ColorRGB>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EngineColorRGB metaobject.");
        } 
        
        std::string field = key; 
        if(field == "red") { 
            color->red = luaL_checknumber(state, 3); 
        }
        else if(field == "green") { 
            color->green = luaL_checknumber(state, 3); 
        }
        else if(field == "blue") { 
            color->blue = luaL_checknumber(state, 3); 
        }
        else { 
            return luaL_error(state, "Invalid field in EngineColorRGB metaobject.");
        }
        return 0;
    }

    void lua_push_meta_engine_color_r_g_b(lua_State *state, Engine::ColorRGB &color, bool read_only) noexcept {
        lua_push_meta_object(state, &color, lua_engine_color_r_g_b__index, lua_engine_color_r_g_b__newindex, read_only); 
    }

    static int lua_engine_quaternion__index(lua_State *state) noexcept {
        auto *quaternion = lua_from_meta_object<Engine::Quaternion>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EngineQuaternion metaobject.");
        } 
        
        std::string field = key; 
        if(field == "i") { 
            lua_pushnumber(state, quaternion->i); 
            return 1; 
        }
        else if(field == "j") { 
            lua_pushnumber(state, quaternion->j); 
            return 1; 
        }
        else if(field == "k") { 
            lua_pushnumber(state, quaternion->k); 
            return 1; 
        }
        else if(field == "w") { 
            lua_pushnumber(state, quaternion->w); 
            return 1; 
        }
        else { 
            return luaL_error(state, "Invalid field in EngineQuaternion metaobject.");
        }
    }

    static int lua_engine_quaternion__newindex(lua_State *state) noexcept {
        auto *quaternion = lua_from_meta_object<Engine::Quaternion>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EngineQuaternion metaobject.");
        } 
        
        std::string field = key; 
        if(field == "i") { 
            quaternion->i = luaL_checknumber(state, 3); 
        }
        else if(field == "j") { 
            quaternion->j = luaL_checknumber(state, 3); 
        }
        else if(field == "k") { 
            quaternion->k = luaL_checknumber(state, 3); 
        }
        else if(field == "w") { 
            quaternion->w = luaL_checknumber(state, 3); 
        }
        else { 
            return luaL_error(state, "Invalid field in EngineQuaternion metaobject.");
        }
        return 0;
    }

    void lua_push_meta_engine_quaternion(lua_State *state, Engine::Quaternion &quaternion, bool read_only) noexcept {
        lua_push_meta_object(state, &quaternion, lua_engine_quaternion__index, lua_engine_quaternion__newindex, read_only); 
    }

    static int lua_engine_plane3_d__index(lua_State *state) noexcept {
        auto *plane = lua_from_meta_object<Engine::Plane3D>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EnginePlane3D metaobject.");
        } 
        
        std::string field = key; 
        if(field == "vector") { 
            lua_push_meta_engine_vector3_d(state, plane->vector);
            return 1;
        }
        else { 
            return luaL_error(state, "Invalid field in EnginePlane3D metaobject.");
        }
    }

    static int lua_engine_plane3_d__newindex(lua_State *state) noexcept {
        auto *plane = lua_from_meta_object<Engine::Plane3D>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EnginePlane3D metaobject.");
        } 
        
        std::string field = key; 
        if(field == "vector") { 
            return luaL_error(state, "Invalid operation on field vector in EnginePlane3D metaobject.");
        }
        else { 
            return luaL_error(state, "Invalid field in EnginePlane3D metaobject.");
        }
        return 0;
    }

    void lua_push_meta_engine_plane3_d(lua_State *state, Engine::Plane3D &plane, bool read_only) noexcept {
        lua_push_meta_object(state, &plane, lua_engine_plane3_d__index, lua_engine_plane3_d__newindex, read_only); 
    }

    static int lua_engine_plane2_d__index(lua_State *state) noexcept {
        auto *plane = lua_from_meta_object<Engine::Plane2D>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EnginePlane2D metaobject.");
        } 
        
        std::string field = key; 
        if(field == "vector") { 
            lua_push_meta_engine_vector2_d(state, plane->vector);
            return 1;
        }
        else { 
            return luaL_error(state, "Invalid field in EnginePlane2D metaobject.");
        }
    }

    static int lua_engine_plane2_d__newindex(lua_State *state) noexcept {
        auto *plane = lua_from_meta_object<Engine::Plane2D>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EnginePlane2D metaobject.");
        } 
        
        std::string field = key; 
        if(field == "vector") { 
            return luaL_error(state, "Invalid operation on field vector in EnginePlane2D metaobject.");
        }
        else { 
            return luaL_error(state, "Invalid field in EnginePlane2D metaobject.");
        }
        return 0;
    }

    void lua_push_meta_engine_plane2_d(lua_State *state, Engine::Plane2D &plane, bool read_only) noexcept {
        lua_push_meta_object(state, &plane, lua_engine_plane2_d__index, lua_engine_plane2_d__newindex, read_only); 
    }

    static int lua_engine_rotation_matrix__index(lua_State *state) noexcept {
        auto *rotation_matrix = lua_from_meta_object<Engine::RotationMatrix>(state, 1); 
        auto index = luaL_checkinteger(state, 2);

        if(index < 1 || index > 3) {  
            return luaL_error(state, "Index out of bounds in EngineRotationMatrix metaobject.");
        } 
        
        if(index == 1) { 
            lua_push_meta_engine_point3_d(state, rotation_matrix->v[0]);
        }
        else if(index == 2) {
            lua_push_meta_engine_point3_d(state, rotation_matrix->v[1]);
        }
        else if(index == 3) {
            lua_push_meta_engine_point3_d(state, rotation_matrix->v[2]);
        }
        return 1;
    }

    static int lua_engine_rotation_matrix__newindex(lua_State *state) noexcept {
        auto *rotation_matrix = lua_from_meta_object<Engine::RotationMatrix>(state, 1); 
        auto index = luaL_checkinteger(state, 2);

        if(index < 1 || index > 3) {  
            return luaL_error(state, "Index out of bounds in EngineRotationMatrix metaobject.");
        } 
        
        if(index == 1) { 
            return luaL_error(state, "Invalid operation on index #1 in EngineRotationMatrix metaobject.");
        }
        else if(index == 2) {
            return luaL_error(state, "Invalid operation on index #2 in EngineRotationMatrix metaobject.");
        }
        else if(index == 3) {
            return luaL_error(state, "Invalid operation on index #3 in EngineRotationMatrix metaobject.");
        }
        return 0;
    }

    void lua_push_meta_engine_rotation_matrix(lua_State *state, Engine::RotationMatrix &matrix, bool read_only) noexcept {
        lua_push_meta_object(state, &matrix, lua_engine_rotation_matrix__index, lua_engine_rotation_matrix__newindex, read_only); 
    }

    static int lua_engine_model_node__index(lua_State *state) noexcept {
        auto *model_node = lua_from_meta_object<Engine::ModelNode>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EngineModelNode metaobject.");
        } 
        
        std::string field = key;
        if(field == "scale") { 
            lua_pushnumber(state, model_node->scale);
            return 1;
        }
        else if(field == "rotationMatrix") {
            lua_push_meta_engine_rotation_matrix(state, model_node->rotation);
            return 1;
        }
        else if(field == "position") {
            lua_push_meta_engine_point3_d(state, model_node->position);
            return 1;
        }
        else { 
            return luaL_error(state, "Invalid field in EngineModelNode metaobject.");
        }
        return 0;
    }

    static int lua_engine_model_node__newindex(lua_State *state) noexcept {
        auto *model_node = lua_from_meta_object<Engine::ModelNode>(state, 1); 
        auto *key = lua_tostring(state, 2);

        if(key == nullptr) {  
            return luaL_error(state, "Invalid key in EngineModelNode metaobject.");
        } 
        
        std::string field = key;
        if(field == "scale") { 
            model_node->scale = luaL_checknumber(state, 3);
        }
        else if(field == "rotationMatrix") {
            return luaL_error(state, "Invalid operation on field rotationMatrix in EngineModelNode metaobject.");
        }
        else if(field == "position") {
            return luaL_error(state, "Invalid operation on field position in EngineModelNode metaobject.");
        }
        else { 
            return luaL_error(state, "Invalid field in EngineModelNode metaobject.");
        }
        return 0;
    }

    void lua_push_meta_engine_model_node(lua_State *state, Engine::ModelNode &node, bool read_only) noexcept {
        lua_push_meta_object(state, &node, lua_engine_model_node__index, lua_engine_model_node__newindex, read_only); 
    }
}
