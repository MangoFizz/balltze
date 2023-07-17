// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/engine/data_types.hpp>
#include "../../logger.hpp"
#include "../loader.hpp"
#include "helpers.hpp"

namespace Balltze::Plugins {
    void lua_create_functions_table(lua_State *state, const char *name, const luaL_Reg *functions) noexcept {
        lua_pushstring(state, name);
        luaL_newlibtable(state, functions);
        luaL_setfuncs(state, functions, 0);
        lua_settable(state, -3);
    }

    VersionNumber lua_check_version_number(lua_State* state, int index) {
        if(lua_istable(state, index)) {
            VersionNumber version;

            auto get_field = [&state, &index](const char *field, int &value) {
                lua_getfield(state, index, field);
                if(lua_isnumber(state, -1)) {
                    value = lua_tointeger(state, -1);
                }
                else {
                    auto *error = "Expected number for version number field.";
                    throw std::runtime_error(error);
                }
                lua_pop(state, 1);
            };

            try {
                get_field("major", version.major);
                get_field("minor", version.minor);
                get_field("patch", version.patch);
                get_field("build", version.build);
            }
            catch(std::runtime_error &e) {
                throw;
            }

            return version;
        }
        else {
            auto *error = "Expected table for version number.";
            throw std::runtime_error(error);
        }
    }

    Engine::ColorARGB lua_to_color_argb(lua_State *state, int index) {
        Engine::ColorARGB color;
        if(lua_istable(state, index)) {
            auto get_field = [&state, &index](const char *field, float &value) {
                lua_getfield(state, index, field);
                if(lua_isnumber(state, -1)) {
                    value = lua_tonumber(state, -1);
                }
                else {
                    auto *error = "Expected number for color field.";
                    luaL_error(state, error);
                    throw std::runtime_error(error);
                }
                lua_pop(state, 1);
            };

            try {
                get_field("red", color.red);
                get_field("green", color.green);
                get_field("blue", color.blue);
                get_field("alpha", color.alpha);
            }
            catch(std::runtime_error &e) {
                throw;
            }
        }
        else {
            if(lua_gettop(state) >= index + 3) {
                auto get_field = [&state, &index](float &value) {
                    if(lua_isnumber(state, index)) {
                        value = lua_tonumber(state, index);
                        index++;
                    }
                    else {
                        auto *error = "Expected number for color field.";
                        luaL_error(state, error);
                        throw std::runtime_error(error);
                    }
                };

                try {
                    get_field(color.red);
                    get_field(color.green);
                    get_field(color.blue);
                    get_field(color.alpha);
                }
                catch(std::runtime_error &e) {
                    throw;
                }
            }
            else {
                throw std::runtime_error("Could not get color from table or stack.");
            }
        }
        return color;
    }

    std::string engine_edition_to_string(Engine::EngineEdition edition) {
        switch(edition) {
            case Engine::ENGINE_TYPE_RETAIL:
                return "retail";
            case Engine::ENGINE_TYPE_DEMO:
                return "demo";
            case Engine::ENGINE_TYPE_CUSTOM_EDITION:
                return "custom";
            default:
                return "unknown";
        }
    }

    Engine::EngineEdition engine_edition_from_string(const std::string &edition) {
        if(edition == "retail") {
            return Engine::ENGINE_TYPE_RETAIL;
        }
        else if(edition == "demo") {
            return Engine::ENGINE_TYPE_DEMO;
        }
        else if(edition == "custom") {
            return Engine::ENGINE_TYPE_CUSTOM_EDITION;
        }
        else {
            throw std::runtime_error("Invalid engine edition.");
        }
    }

    std::string cache_file_engine_to_string(Engine::CacheFileEngine engine) {
        switch(engine) {
            case Engine::CACHE_FILE_XBOX:
                return "xbox";
            case Engine::CACHE_FILE_DEMO:
                return "demo";
            case Engine::CACHE_FILE_RETAIL:
                return "retail";
            case Engine::CACHE_FILE_CUSTOM_EDITION:
                return "custom";
            case Engine::CACHE_FILE_INVADER:
                return "invader";
            case Engine::CACHE_FILE_DEMO_COMPRESSED:
                return "demo compressed";
            case Engine::CACHE_FILE_RETAIL_COMPRESSED:
                return "retail compressed";
            case Engine::CACHE_FILE_CUSTOM_EDITION_COMPRESSED:
                return "custom compressed";
            default:
                return "unknown";
        }   
    }

    Engine::CacheFileEngine cache_file_engine_from_string(const std::string &engine) {
        if(engine == "xbox") {
            return Engine::CACHE_FILE_XBOX;
        }
        else if(engine == "demo") {
            return Engine::CACHE_FILE_DEMO;
        }
        else if(engine == "retail") {
            return Engine::CACHE_FILE_RETAIL;
        }
        else if(engine == "custom") {
            return Engine::CACHE_FILE_CUSTOM_EDITION;
        }
        else if(engine == "invader") {
            return Engine::CACHE_FILE_INVADER;
        }
        else if(engine == "demo compressed") {
            return Engine::CACHE_FILE_DEMO_COMPRESSED;
        }
        else if(engine == "retail compressed") {
            return Engine::CACHE_FILE_RETAIL_COMPRESSED;
        }
        else if(engine == "custom compressed") {
            return Engine::CACHE_FILE_CUSTOM_EDITION_COMPRESSED;
        }
        else {
            throw std::runtime_error("Invalid cache file engine.");
        }
    }

    std::string map_game_type_to_string(Engine::MapGameType type) {
        switch(type) {
            case Engine::MAP_SINGLE_PLAYER:
                return "single_player";
            case Engine::MAP_MULTIPLAYER:
                return "multiplayer";
            case Engine::MAP_USER_INTERFACE:
                return "user_interface";
            default:
                return "unknown";
        }
    }

    Engine::MapGameType map_game_type_from_string(const std::string &type) {
        if(type == "single_player") {
            return Engine::MAP_SINGLE_PLAYER;
        }
        else if(type == "multiplayer") {
            return Engine::MAP_MULTIPLAYER;
        }
        else if(type == "user_interface") {
            return Engine::MAP_USER_INTERFACE;
        }
        else {
            throw std::runtime_error("Invalid map game type.");
        }
    }

    std::string server_type_to_string(Engine::ServerType type) {
        switch(type) {
            case Engine::SERVER_NONE:
                return "none";
            case Engine::SERVER_DEDICATED:
                return "dedicated";
            case Engine::SERVER_LOCAL:
                return "local";
            default:
                return "unknown";
        }
    }

    Engine::ServerType server_type_from_string(const std::string &type) {
        if(type == "none") {
            return Engine::SERVER_NONE;
        }
        else if(type == "dedicated") {
            return Engine::SERVER_DEDICATED;
        }
        else if(type == "local") {
            return Engine::SERVER_LOCAL;
        }
        else {
            throw std::runtime_error("Invalid server type.");
        }
    }

    std::string server_game_type_to_string(Engine::Gametype gametype) {
        switch(gametype) {
            case Engine::GAMETYPE_CTF:
                return "ctf";
            case Engine::GAMETYPE_SLAYER:
                return "slayer";
            case Engine::GAMETYPE_ODDBALL:
                return "oddball";
            case Engine::GAMETYPE_KING:
                return "king";
            case Engine::GAMETYPE_RACE:
                return "race";
            default:
                return "none";
        }
    }

    Engine::Gametype server_game_type_from_string(const std::string &gametype) {
        if(gametype == "ctf") {
            return Engine::GAMETYPE_CTF;
        }
        else if(gametype == "slayer") {
            return Engine::GAMETYPE_SLAYER;
        }
        else if(gametype == "oddball") {
            return Engine::GAMETYPE_ODDBALL;
        }
        else if(gametype == "king") {
            return Engine::GAMETYPE_KING;
        }
        else if(gametype == "race") {
            return Engine::GAMETYPE_RACE;
        }
        else {
            throw std::runtime_error("Invalid server game type.");
        }
    }

    void lua_push_engine_matrix(lua_State *state, Engine::Matrix &matrix) noexcept {
        lua_newtable(state);
        for(std::size_t i = 0; i < 3; i++) {
            lua_newtable(state);
            for(std::size_t j = 0; j < 3; j++) {
                lua_pushnumber(state, matrix[i][j]);
                lua_rawseti(state, -2, j + 1);
            }
            lua_rawseti(state, -2, i + 1);
        }
    }

    std::shared_ptr<Engine::Matrix> lua_to_engine_matrix(lua_State *state, int index) noexcept {
        auto matrix_ptr = new Engine::Matrix();
        std::shared_ptr<Engine::Matrix> matrix(matrix_ptr);
        for(std::size_t i = 0; i < 3; i++) {
            lua_rawgeti(state, index, i + 1);
            for(std::size_t j = 0; j < 3; j++) {
                lua_rawgeti(state, -1, j + 1);
                matrix[i][j] = luaL_checknumber(state, -1);
                lua_pop(state, 1);
            }
            lua_pop(state, 1);
        }
        return matrix;
    }

    static int lua_engine_color_a_r_g_b_int__index(lua_State *state) {
        lua_getfield(state, 1, "_data"); 
        auto color = static_cast<Engine::ColorARGBInt *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            lua_pushnil(state); 
            return 1; 
        }
        else if(field == "red") { 
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
            return luaL_error(state, "Invalid key"); 
        }
    }

    static int lua_engine_color_a_r_g_b_int__newindex(lua_State *state) {
        lua_getfield(state, 1, "_data"); 
        auto color = static_cast<Engine::ColorARGBInt *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            return luaL_error(state, "Cannot set _data"); 
        }
        else if(field == "red") { 
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
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_color_a_r_g_b_int(lua_State *state, Engine::ColorARGBInt &color) noexcept {
        lua_push_meta_object(state, color, lua_engine_color_a_r_g_b_int__index, lua_engine_color_a_r_g_b_int__newindex); 
    }

    static int lua_engine_tag_dependency__index(lua_State *state) {
        lua_getfield(state, 1, "_data"); 
        auto dependency = static_cast<Engine::TagDependency *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            lua_pushnil(state); 
            return 1; 
        }
        else if(field == "id") { 
            lua_pushinteger(state, dependency->tag_id.whole_id); 
            return 1; 
        }
        else if(field == "class") {
            auto tag_class = Engine::tag_class_to_string(*reinterpret_cast<Engine::TagClassInt *>(dependency->tag_fourcc)); 
            lua_pushstring(state, tag_class.c_str()); 
            return 1; 
        }
        else if(field == "path") { 
            char path[dependency->path_size + 1];
            std::memcpy(path, reinterpret_cast<char *>(dependency->path_pointer), dependency->path_size);
            path[dependency->path_size] = '\0';
            lua_pushstring(state, path);
            return 1; 
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
    }

    static int lua_engine_tag_dependency__newindex(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto dependency = static_cast<Engine::TagDependency *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        }

        std::string field = key;
        if(field == "_data") { 
            return luaL_error(state, "Cannot set _data"); 
        }
        else if(field == "id") { 
            dependency->tag_id.whole_id = luaL_checkinteger(state, 3); 
        }
        else if(field == "class") {
            auto *tag_class = lua_tostring(state, 3);
            if(tag_class == nullptr) {
                return luaL_error(state, "Invalid tag class");
            }
            *reinterpret_cast<Engine::TagClassInt *>(dependency->tag_fourcc) = Engine::tag_class_from_string(tag_class);
        }
        else if(field == "path") { 
            auto *path = lua_tostring(state, 3);
            if(path == nullptr) {
                return luaL_error(state, "Invalid path");
            }
            if(dependency->path_size < std::strlen(path)) {
                return luaL_error(state, "Path is too long");
            }
            std::memcpy(reinterpret_cast<char *>(dependency->path_pointer), path, std::strlen(path));
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_tag_dependency(lua_State *state, Engine::TagDependency &dependency) noexcept {
        lua_push_meta_object(state, dependency, lua_engine_tag_dependency__index, lua_engine_tag_dependency__newindex); 
    }

    static int lua_engine_point2_d__index(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto point = static_cast<Engine::Point2D *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            lua_pushnil(state); 
            return 1; 
        }
        else if(field == "x") { 
            lua_pushnumber(state, point->x); 
            return 1; 
        }
        else if(field == "y") { 
            lua_pushnumber(state, point->y); 
            return 1; 
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
    }

    static int lua_engine_point2_d__newindex(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto point = static_cast<Engine::Point2D *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            return luaL_error(state, "Cannot set _data"); 
        }
        else if(field == "x") { 
            point->x = luaL_checknumber(state, 3); 
        }
        else if(field == "y") { 
            point->y = luaL_checknumber(state, 3); 
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_point2_d(lua_State *state, Engine::Point2D &point) noexcept {
        lua_push_meta_object(state, point, lua_engine_point2_d__index, lua_engine_point2_d__newindex); 
    }

    static int lua_engine_point3_d__index(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto point = static_cast<Engine::Point3D *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            lua_pushnil(state); 
            return 1; 
        }
        else if(field == "x") { 
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
            return luaL_error(state, "Invalid key"); 
        }
    }

    static int lua_engine_point3_d__newindex(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto point = static_cast<Engine::Point3D *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            return luaL_error(state, "Cannot set _data"); 
        }
        else if(field == "x") { 
            point->x = luaL_checkinteger(state, 3); 
        }
        else if(field == "y") { 
            point->y = luaL_checkinteger(state, 3); 
        }
        else if(field == "z") { 
            point->z = luaL_checkinteger(state, 3); 
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_point3_d(lua_State *state, Engine::Point3D &point) noexcept {
        lua_push_meta_object(state, point, lua_engine_point3_d__index, lua_engine_point3_d__newindex); 
    }

    static int lua_engine_tag_data_offset__index(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto offset = static_cast<Engine::TagDataOffset *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            lua_pushnil(state); 
            return 1; 
        }
        else if(field == "size") { 
            lua_pushinteger(state, offset->size); 
            return 1; 
        }
        else if(field == "external") { 
            lua_pushboolean(state, offset->external);
            return 1; 
        }
        else if(field == "file_offset") { 
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
            return luaL_error(state, "Invalid key"); 
        }
    }

    static int lua_engine_tag_data_offset__newindex(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto offset = static_cast<Engine::TagDataOffset *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            return luaL_error(state, "Cannot set _data"); 
        }
        else if(field == "size") { 
            offset->size = luaL_checkinteger(state, 3); 
        }
        else if(field == "external") { 
            offset->external = lua_toboolean(state, 3);
        }
        else if(field == "file_offset") { 
            offset->file_offset = luaL_checkinteger(state, 3);
        }
        else if(field == "pointer") {
            offset->pointer = reinterpret_cast<std::byte *>(luaL_checkinteger(state, 3));
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_tag_data_offset(lua_State *state, Engine::TagDataOffset &offset) noexcept {
        lua_push_meta_object(state, offset, lua_engine_tag_data_offset__index, lua_engine_tag_data_offset__newindex); 
    }

    static int lua_engine_color_a_r_g_b__index(lua_State *state) {
        lua_getfield(state, 1, "_data"); 
        auto color = static_cast<Engine::ColorARGB *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            lua_pushnil(state); 
            return 1; 
        }
        else if(field == "alpha") { 
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
            return luaL_error(state, "Invalid key"); 
        }
    }

    static int lua_engine_color_a_r_g_b__newindex(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto color = static_cast<Engine::ColorARGB *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            return luaL_error(state, "Cannot set _data"); 
        }
        else if(field == "alpha") { 
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
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_color_a_r_g_b(lua_State *state, Engine::ColorARGB &color) noexcept {
        lua_push_meta_object(state, color, lua_engine_color_a_r_g_b__index, lua_engine_color_a_r_g_b__newindex); 
    }

    static int lua_engine_rectangle2_d__index(lua_State *state) {
        lua_getfield(state, 1, "_data"); 
        auto rectangle = static_cast<Engine::Rectangle2D *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            lua_pushnil(state); 
            return 1; 
        }
        else if(field == "top") { 
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
            return luaL_error(state, "Invalid key"); 
        }
    }

    static int lua_engine_rectangle2_d__newindex(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto rectangle = static_cast<Engine::Rectangle2D *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            return luaL_error(state, "Cannot set _data"); 
        }
        else if(field == "top") { 
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
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_rectangle2_d(lua_State *state, Engine::Rectangle2D &rectangle) noexcept {
        lua_push_meta_object(state, rectangle, lua_engine_rectangle2_d__index, lua_engine_rectangle2_d__newindex); 
    }

    static int lua_engine_point2_d_int__index(lua_State *state) {
        lua_getfield(state, 1, "_data"); 
        auto point = static_cast<Engine::Point2DInt *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            lua_pushnil(state); 
            return 1; 
        }
        else if(field == "x") { 
            lua_pushinteger(state, point->x); 
            return 1; 
        }
        else if(field == "y") { 
            lua_pushinteger(state, point->y); 
            return 1; 
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
    }

    static int lua_engine_point2_d_int__newindex(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto point = static_cast<Engine::Point2DInt *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            return luaL_error(state, "Cannot set _data"); 
        }
        else if(field == "x") { 
            point->x = luaL_checkinteger(state, 3); 
        }
        else if(field == "y") { 
            point->y = luaL_checkinteger(state, 3); 
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_point2_d_int(lua_State *state, Engine::Point2DInt &point) noexcept {
        lua_push_meta_object(state, point, lua_engine_point2_d_int__index, lua_engine_point2_d_int__newindex); 
    }

    static int lua_engine_euler2_d__index(lua_State *state) {
        lua_getfield(state, 1, "_data"); 
        auto euler = static_cast<Engine::Euler2D *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            lua_pushnil(state); 
            return 1; 
        }
        else if(field == "yaw") { 
            lua_pushnumber(state, euler->yaw); 
            return 1; 
        }
        else if(field == "pitch") { 
            lua_pushnumber(state, euler->pitch); 
            return 1; 
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
    }

    static int lua_engine_euler2_d__newindex(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto euler = static_cast<Engine::Euler2D *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            return luaL_error(state, "Cannot set _data"); 
        }
        else if(field == "yaw") { 
            euler->yaw = luaL_checknumber(state, 3); 
        }
        else if(field == "pitch") { 
            euler->pitch = luaL_checknumber(state, 3); 
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_euler2_d(lua_State *state, Engine::Euler2D &euler) noexcept {
        lua_push_meta_object(state, euler, lua_engine_euler2_d__index, lua_engine_euler2_d__newindex); 
    }

    static int lua_engine_euler3_d__index(lua_State *state) {
        lua_getfield(state, 1, "_data"); 
        auto euler = static_cast<Engine::Euler3D *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            lua_pushnil(state); 
            return 1; 
        }
        else if(field == "yaw") { 
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
            return luaL_error(state, "Invalid key"); 
        }
    }

    static int lua_engine_euler3_d__newindex(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto euler = static_cast<Engine::Euler3D *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            return luaL_error(state, "Cannot set _data"); 
        }
        else if(field == "yaw") { 
            euler->yaw = luaL_checknumber(state, 3); 
        }
        else if(field == "pitch") { 
            euler->pitch = luaL_checknumber(state, 3); 
        }
        else if(field == "roll") { 
            euler->roll = luaL_checknumber(state, 3); 
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_euler3_d(lua_State *state, Engine::Euler3D &euler) noexcept {
        lua_push_meta_object(state, euler, lua_engine_euler3_d__index, lua_engine_euler3_d__newindex); 
    }

    static int lua_engine_vector2_d__index(lua_State *state) {
        lua_getfield(state, 1, "_data"); 
        auto vector = static_cast<Engine::Vector2D *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            lua_pushnil(state); 
            return 1; 
        }
        else if(field == "i") { 
            lua_pushnumber(state, vector->i); 
            return 1; 
        }
        else if(field == "j") { 
            lua_pushnumber(state, vector->j); 
            return 1; 
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
    }

    static int lua_engine_vector2_d__newindex(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto vector = static_cast<Engine::Vector2D *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            return luaL_error(state, "Cannot set _data"); 
        }
        else if(field == "i") { 
            vector->i = luaL_checknumber(state, 3); 
        }
        else if(field == "j") { 
            vector->j = luaL_checknumber(state, 3); 
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_vector2_d(lua_State *state, Engine::Vector2D &vector) noexcept {
        lua_push_meta_object(state, vector, lua_engine_vector2_d__index, lua_engine_vector2_d__newindex); 
    }

    static int lua_engine_vector3_d__index(lua_State *state) {
        lua_getfield(state, 1, "_data"); 
        auto vector = static_cast<Engine::Vector3D *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            lua_pushnil(state); 
            return 1; 
        }
        else if(field == "i") { 
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
            return luaL_error(state, "Invalid key"); 
        }
    }

    static int lua_engine_vector3_d__newindex(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto vector = static_cast<Engine::Vector3D *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            return luaL_error(state, "Cannot set _data"); 
        }
        else if(field == "i") { 
            vector->i = luaL_checknumber(state, 3); 
        }
        else if(field == "j") { 
            vector->j = luaL_checknumber(state, 3); 
        }
        else if(field == "k") { 
            vector->k = luaL_checknumber(state, 3); 
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_vector3_d(lua_State *state, Engine::Vector3D &vector) noexcept {
        lua_push_meta_object(state, vector, lua_engine_vector3_d__index, lua_engine_vector3_d__newindex); 
    }

    static int lua_engine_color_r_g_b__index(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto color = static_cast<Engine::ColorRGB *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            lua_pushnil(state); 
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
            return luaL_error(state, "Invalid key"); 
        }
    }

    static int lua_engine_color_r_g_b__newindex(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto color = static_cast<Engine::ColorRGB *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            return luaL_error(state, "Cannot set _data"); 
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
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_color_r_g_b(lua_State *state, Engine::ColorRGB &color) noexcept {
        lua_push_meta_object(state, color, lua_engine_color_r_g_b__index, lua_engine_color_r_g_b__newindex); 
    }

    static int lua_engine_quaternion__index(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto quaternion = static_cast<Engine::Quaternion *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            lua_pushnil(state); 
            return 1; 
        }
        else if(field == "i") { 
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
            return luaL_error(state, "Invalid key"); 
        }
    }

    static int lua_engine_quaternion__newindex(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto quaternion = static_cast<Engine::Quaternion *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            return luaL_error(state, "Cannot set _data"); 
        }
        else if(field == "i") { 
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
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_quaternion(lua_State *state, Engine::Quaternion &quaternion) noexcept {
        lua_push_meta_object(state, quaternion, lua_engine_quaternion__index, lua_engine_quaternion__newindex); 
    }

    static int lua_engine_plane3_d__index(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto plane = static_cast<Engine::Plane3D *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            lua_pushnil(state); 
            return 1; 
        }
        else if(field == "vector") { 
            lua_push_meta_engine_vector3_d(state, plane->vector);
            return 1;
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
    }

    static int lua_engine_plane3_d__newindex(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto plane = static_cast<Engine::Plane3D *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            return luaL_error(state, "Cannot set _data"); 
        }
        else if(field == "vector") { 
            return luaL_error(state, "Invalid operation");
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_plane3_d(lua_State *state, Engine::Plane3D &plane) noexcept {
        lua_push_meta_object(state, plane, lua_engine_plane3_d__index, lua_engine_plane3_d__newindex); 
    }

    static int lua_engine_plane2_d__index(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto plane = static_cast<Engine::Plane2D *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            lua_pushnil(state); 
            return 1; 
        }
        else if(field == "vector") { 
            lua_push_meta_engine_vector2_d(state, plane->vector);
            return 1;
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
    }

    static int lua_engine_plane2_d__newindex(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto plane = static_cast<Engine::Plane2D *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "_data") { 
            return luaL_error(state, "Cannot set _data"); 
        }
        else if(field == "vector") { 
            return luaL_error(state, "Invalid operation");
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_plane2_d(lua_State *state, Engine::Plane2D &plane) noexcept {
        lua_push_meta_object(state, plane, lua_engine_plane2_d__index, lua_engine_plane2_d__newindex); 
    }

    extern std::string lua_engine_u_i_widget_type_to_string(Engine::TagDefinitions::UIWidgetType value) noexcept; 
    extern Engine::TagDefinitions::UIWidgetType lua_engine_u_i_widget_type_from_string(std::string str); 

    static int lua_engine_widget__index(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto widget = static_cast<Engine::Widget *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "definition_tag_id") { 
            lua_pushinteger(state, widget->definition_tag_id.whole_id);
            return 1;
        }
        else if(field == "name") {
            lua_pushstring(state, widget->name);
            return 1;
        }
        else if(field == "hidden") {
            if(widget->hidden == 1) {
                lua_pushboolean(state, true);
            }
            else {
                lua_pushboolean(state, false);
            }
            return 1;
        }
        else if(field == "left_bound") {
            lua_pushinteger(state, widget->left_bound);
            return 1;
        }
        else if(field == "top_bound") {
            lua_pushinteger(state, widget->top_bound);
            return 1;
        }
        else if(field == "type") {
            std::string type = lua_engine_u_i_widget_type_to_string(widget->type);
            lua_pushstring(state, type.c_str());
            return 1;
        }
        else if(field == "ms_to_close") {
            lua_pushinteger(state, widget->ms_to_close);
            return 1;
        }
        else if(field == "ms_to_close_fade_time") {
            lua_pushinteger(state, widget->ms_to_close_fade_time);
            return 1;
        }
        else if(field == "opacity") {
            lua_pushnumber(state, widget->opacity);
            return 1;
        }
        else if(field == "previous_widget") {
            if(widget->previous_widget == nullptr) {
                lua_pushnil(state);
            }
            else {
                lua_push_meta_engine_widget(state, *widget->previous_widget);
            }
            return 1;
        }
        else if(field == "next_widget") {
            if(widget->next_widget == nullptr) {
                lua_pushnil(state);
            }
            else {
                lua_push_meta_engine_widget(state, *widget->next_widget);
            }
            return 1;
        }
        else if(field == "parent_widget") {
            if(widget->parent_widget == nullptr) {
                lua_pushnil(state);
            }
            else {
                lua_push_meta_engine_widget(state, *widget->parent_widget);
            }
            return 1;
        }
        else if(field == "child_widget") {
            if(widget->child_widget == nullptr) {
                lua_pushnil(state);
            }
            else {
                lua_push_meta_engine_widget(state, *widget->child_widget);
            }
            return 1;
        }
        else if(field == "focused_child") {
            if(widget->focused_child == nullptr) {
                lua_pushnil(state);
            }
            else {
                lua_push_meta_engine_widget(state, *widget->focused_child);
            }
            return 1;
        }
        else if(field == "text") {
            lua_pushlightuserdata(state, reinterpret_cast<void *>(const_cast<wchar_t *>(widget->text)));
            return 1;
        }
        else if(field == "cursor_index") {
            lua_pushinteger(state, widget->cursor_index);
            return 1;
        }
        else if(field == "bitmap_index") {
            lua_pushinteger(state, widget->bitmap_index);
            return 1;
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
    }

    static int lua_engine_widget__newindex(lua_State *state) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto widget = static_cast<Engine::Widget *>(lua_touserdata(state, -1)); 
        lua_pop(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key; 
        if(field == "definition_tag_id") { 
            widget->definition_tag_id.whole_id = luaL_checkinteger(state, 3);
        }
        else if(field == "name") {
            return luaL_error(state, "Unsupported operation");
        }
        else if(field == "hidden") {
            widget->hidden = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "left_bound") {
            widget->left_bound = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "top_bound") {
            widget->top_bound = static_cast<std::int16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "type") {
            try {
                std::string type = luaL_checkstring(state, 3);
                widget->type = lua_engine_u_i_widget_type_from_string(type);
            }
            catch(...) {
                return luaL_error(state, "Invalid value");
            }
        }
        else if(field == "ms_to_close") {
            widget->ms_to_close = luaL_checkinteger(state, 3);
        }
        else if(field == "ms_to_close_fade_time") {
            widget->ms_to_close_fade_time = luaL_checkinteger(state, 3);
        }
        else if(field == "opacity") {
            float opacity = luaL_checknumber(state, 3);
            if(opacity < 0.0f || opacity > 1.0f) {
                return luaL_error(state, "Invalid value");
            }
            widget->opacity = opacity;
        }
        else if(field == "previous_widget") {
            auto *previous_widget = lua_from_meta_object<Engine::Widget>(state, 3);
            if(previous_widget) {
                widget->previous_widget = previous_widget;
            }
            else {
                return luaL_error(state, "Invalid value");
            }
        }
        else if(field == "next_widget") {
            auto *next_widget = lua_from_meta_object<Engine::Widget>(state, 3);
            if(next_widget) {
                widget->next_widget = next_widget;
            }
            else {
                return luaL_error(state, "Invalid value");
            }
        }
        else if(field == "parent_widget") {
            auto *parent_widget = lua_from_meta_object<Engine::Widget>(state, 3);
            if(parent_widget) {
                widget->parent_widget = parent_widget;
            }
            else {
                return luaL_error(state, "Invalid value");
            }
        }
        else if(field == "child_widget") {
            auto *child_widget = lua_from_meta_object<Engine::Widget>(state, 3);
            if(child_widget) {
                widget->child_widget = child_widget;
            }
            else {
                return luaL_error(state, "Invalid value");
            }
        }
        else if(field == "focused_child") {
            auto *focused_child = lua_from_meta_object<Engine::Widget>(state, 3);
            if(focused_child) {
                widget->focused_child = focused_child;
            }
            else {
                return luaL_error(state, "Invalid value");
            }
        }
        else if(field == "text") {
            return luaL_error(state, "Unsupported operation");
        }
        else if(field == "cursor_index") {
            widget->cursor_index = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else if(field == "bitmap_index") {
            widget->bitmap_index = static_cast<std::uint16_t>(luaL_checkinteger(state, 3));
        }
        else { 
            return luaL_error(state, "Invalid key"); 
        }
        return 0;
    }

    void lua_push_meta_engine_widget(lua_State *state, Engine::Widget &widget) noexcept {
        lua_push_meta_object(state, widget, lua_engine_widget__index, lua_engine_widget__newindex); 
    }
}
