// SPDX-License-Identifier: GPL-3.0-only

#include <cstring>
#include <balltze/engine.hpp>
#include <luacstruct/luacstruct.h>
#include <lua.hpp>
#include "../helpers.hpp"
#include "../metatables.hpp"

namespace Balltze::Plugins {
    using namespace Engine;
    using EngineTagClass = TagClassInt;

    void lua_push_engine_tag_class(lua_State *state, EngineTagClass tag_class) noexcept {
        if(!luacs_ctype_exists(state, EngineTagClass)) {
            luacs_newenum(state, TagClassInt);
            luacs_enum_declare_value(state, "ACTOR", TAG_CLASS_ACTOR);
            luacs_enum_declare_value(state, "ACTOR_VARIANT", TAG_CLASS_ACTOR_VARIANT);
            luacs_enum_declare_value(state, "ANTENNA", TAG_CLASS_ANTENNA);
            luacs_enum_declare_value(state, "MODEL_ANIMATIONS", TAG_CLASS_MODEL_ANIMATIONS);
            luacs_enum_declare_value(state, "BIPED", TAG_CLASS_BIPED);
            luacs_enum_declare_value(state, "BITMAP", TAG_CLASS_BITMAP);
            luacs_enum_declare_value(state, "SPHEROID", TAG_CLASS_SPHEROID);
            luacs_enum_declare_value(state, "CONTINUOUS_DAMAGE_EFFECT", TAG_CLASS_CONTINUOUS_DAMAGE_EFFECT);
            luacs_enum_declare_value(state, "MODEL_COLLISION_GEOMETRY", TAG_CLASS_MODEL_COLLISION_GEOMETRY);
            luacs_enum_declare_value(state, "COLOR_TABLE", TAG_CLASS_COLOR_TABLE);
            luacs_enum_declare_value(state, "CONTRAIL", TAG_CLASS_CONTRAIL);
            luacs_enum_declare_value(state, "DEVICE_CONTROL", TAG_CLASS_DEVICE_CONTROL);
            luacs_enum_declare_value(state, "DECAL", TAG_CLASS_DECAL);
            luacs_enum_declare_value(state, "UI_WIDGET_DEFINITION", TAG_CLASS_UI_WIDGET_DEFINITION);
            luacs_enum_declare_value(state, "INPUT_DEVICE_DEFAULTS", TAG_CLASS_INPUT_DEVICE_DEFAULTS);
            luacs_enum_declare_value(state, "DEVICE", TAG_CLASS_DEVICE);
            luacs_enum_declare_value(state, "DETAIL_OBJECT_COLLECTION", TAG_CLASS_DETAIL_OBJECT_COLLECTION);
            luacs_enum_declare_value(state, "EFFECT", TAG_CLASS_EFFECT);
            luacs_enum_declare_value(state, "EQUIPMENT", TAG_CLASS_EQUIPMENT);
            luacs_enum_declare_value(state, "FLAG", TAG_CLASS_FLAG);
            luacs_enum_declare_value(state, "FOG", TAG_CLASS_FOG);
            luacs_enum_declare_value(state, "FONT", TAG_CLASS_FONT);
            luacs_enum_declare_value(state, "MATERIAL_EFFECTS", TAG_CLASS_MATERIAL_EFFECTS);
            luacs_enum_declare_value(state, "GARBAGE", TAG_CLASS_GARBAGE);
            luacs_enum_declare_value(state, "GLOW", TAG_CLASS_GLOW);
            luacs_enum_declare_value(state, "GRENADE_HUD_INTERFACE", TAG_CLASS_GRENADE_HUD_INTERFACE);
            luacs_enum_declare_value(state, "HUD_MESSAGE_TEXT", TAG_CLASS_HUD_MESSAGE_TEXT);
            luacs_enum_declare_value(state, "HUD_NUMBER", TAG_CLASS_HUD_NUMBER);
            luacs_enum_declare_value(state, "HUD_GLOBALS", TAG_CLASS_HUD_GLOBALS);
            luacs_enum_declare_value(state, "ITEM", TAG_CLASS_ITEM);
            luacs_enum_declare_value(state, "ITEM_COLLECTION", TAG_CLASS_ITEM_COLLECTION);
            luacs_enum_declare_value(state, "DAMAGE_EFFECT", TAG_CLASS_DAMAGE_EFFECT);
            luacs_enum_declare_value(state, "LENS_FLARE", TAG_CLASS_LENS_FLARE);
            luacs_enum_declare_value(state, "LIGHTNING", TAG_CLASS_LIGHTNING);
            luacs_enum_declare_value(state, "DEVICE_LIGHT_FIXTURE", TAG_CLASS_DEVICE_LIGHT_FIXTURE);
            luacs_enum_declare_value(state, "LIGHT", TAG_CLASS_LIGHT);
            luacs_enum_declare_value(state, "SOUND_LOOPING", TAG_CLASS_SOUND_LOOPING);
            luacs_enum_declare_value(state, "DEVICE_MACHINE", TAG_CLASS_DEVICE_MACHINE);
            luacs_enum_declare_value(state, "GLOBALS", TAG_CLASS_GLOBALS);
            luacs_enum_declare_value(state, "METER", TAG_CLASS_METER);
            luacs_enum_declare_value(state, "LIGHT_VOLUME", TAG_CLASS_LIGHT_VOLUME);
            luacs_enum_declare_value(state, "GBXMODEL", TAG_CLASS_GBXMODEL);
            luacs_enum_declare_value(state, "MODEL", TAG_CLASS_MODEL);
            luacs_enum_declare_value(state, "MULTIPLAYER_SCENARIO_DESCRIPTION", TAG_CLASS_MULTIPLAYER_SCENARIO_DESCRIPTION);
            luacs_enum_declare_value(state, "NULL", TAG_CLASS_NULL);
            luacs_enum_declare_value(state, "PREFERENCES_NETWORK_GAME", TAG_CLASS_PREFERENCES_NETWORK_GAME);
            luacs_enum_declare_value(state, "OBJECT", TAG_CLASS_OBJECT);
            luacs_enum_declare_value(state, "PARTICLE", TAG_CLASS_PARTICLE);
            luacs_enum_declare_value(state, "PARTICLE_SYSTEM", TAG_CLASS_PARTICLE_SYSTEM);
            luacs_enum_declare_value(state, "PHYSICS", TAG_CLASS_PHYSICS);
            luacs_enum_declare_value(state, "PLACEHOLDER", TAG_CLASS_PLACEHOLDER);
            luacs_enum_declare_value(state, "POINT_PHYSICS", TAG_CLASS_POINT_PHYSICS);
            luacs_enum_declare_value(state, "PROJECTILE", TAG_CLASS_PROJECTILE);
            luacs_enum_declare_value(state, "WEATHER_PARTICLE_SYSTEM", TAG_CLASS_WEATHER_PARTICLE_SYSTEM);
            luacs_enum_declare_value(state, "SCENARIO_STRUCTURE_BSP", TAG_CLASS_SCENARIO_STRUCTURE_BSP);
            luacs_enum_declare_value(state, "SCENERY", TAG_CLASS_SCENERY);
            luacs_enum_declare_value(state, "SHADER_TRANSPARENT_CHICAGO_EXTENDED", TAG_CLASS_SHADER_TRANSPARENT_CHICAGO_EXTENDED);
            luacs_enum_declare_value(state, "SHADER_TRANSPARENT_CHICAGO", TAG_CLASS_SHADER_TRANSPARENT_CHICAGO);
            luacs_enum_declare_value(state, "SCENARIO", TAG_CLASS_SCENARIO);
            luacs_enum_declare_value(state, "SHADER_ENVIRONMENT", TAG_CLASS_SHADER_ENVIRONMENT);
            luacs_enum_declare_value(state, "SHADER_TRANSPARENT_GLASS", TAG_CLASS_SHADER_TRANSPARENT_GLASS);
            luacs_enum_declare_value(state, "SHADER", TAG_CLASS_SHADER);
            luacs_enum_declare_value(state, "SKY", TAG_CLASS_SKY);
            luacs_enum_declare_value(state, "SHADER_TRANSPARENT_METER", TAG_CLASS_SHADER_TRANSPARENT_METER);
            luacs_enum_declare_value(state, "SOUND", TAG_CLASS_SOUND);
            luacs_enum_declare_value(state, "SOUND_ENVIRONMENT", TAG_CLASS_SOUND_ENVIRONMENT);
            luacs_enum_declare_value(state, "SHADER_MODEL", TAG_CLASS_SHADER_MODEL);
            luacs_enum_declare_value(state, "SHADER_TRANSPARENT_GENERIC", TAG_CLASS_SHADER_TRANSPARENT_GENERIC);
            luacs_enum_declare_value(state, "UI_WIDGET_COLLECTION", TAG_CLASS_UI_WIDGET_COLLECTION);
            luacs_enum_declare_value(state, "SHADER_TRANSPARENT_PLASMA", TAG_CLASS_SHADER_TRANSPARENT_PLASMA);
            luacs_enum_declare_value(state, "SOUND_SCENERY", TAG_CLASS_SOUND_SCENERY);
            luacs_enum_declare_value(state, "STRING_LIST", TAG_CLASS_STRING_LIST);
            luacs_enum_declare_value(state, "SHADER_TRANSPARENT_WATER", TAG_CLASS_SHADER_TRANSPARENT_WATER);
            luacs_enum_declare_value(state, "TAG_COLLECTION", TAG_CLASS_TAG_COLLECTION);
            luacs_enum_declare_value(state, "CAMERA_TRACK", TAG_CLASS_CAMERA_TRACK);
            luacs_enum_declare_value(state, "DIALOGUE", TAG_CLASS_DIALOGUE);
            luacs_enum_declare_value(state, "UNIT_HUD_INTERFACE", TAG_CLASS_UNIT_HUD_INTERFACE);
            luacs_enum_declare_value(state, "UNIT", TAG_CLASS_UNIT);
            luacs_enum_declare_value(state, "UNICODE_STRING_LIST", TAG_CLASS_UNICODE_STRING_LIST);
            luacs_enum_declare_value(state, "VIRTUAL_KEYBOARD", TAG_CLASS_VIRTUAL_KEYBOARD);
            luacs_enum_declare_value(state, "VEHICLE", TAG_CLASS_VEHICLE);
            luacs_enum_declare_value(state, "WEAPON", TAG_CLASS_WEAPON);
            luacs_enum_declare_value(state, "WIND", TAG_CLASS_WIND);
            luacs_enum_declare_value(state, "WEAPON_HUD_INTERFACE", TAG_CLASS_WEAPON_HUD_INTERFACE);
            lua_getglobal(state, "Engine");
            lua_getfield(state, -1, "tag");
            lua_pushvalue(state, -3);
            lua_setfield(state, -2, "TAG_CLASS");
            lua_pop(state, 3);
        }
    }

    void lua_push_meta_engine_tag(lua_State *state, Engine::Tag &tag, bool read_only) noexcept {
        // lua_push_meta_object(state, &tag, lua_meta_engine_tag__index, lua_meta_engine_tag__newindex, read_only);
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
