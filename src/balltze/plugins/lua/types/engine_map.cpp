// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/engine/map.hpp>
#include "../helpers/bitfield.hpp"
#include "../helpers/luacstruct.hpp"
#include "engine_map.hpp"

namespace Balltze::Plugins::Lua {
    static void define_engine_cache_file_engine_enum(lua_State *state) noexcept {
        luacs_newenum(state, EngineCacheFileEngine);
        luacs_enum_declare_value(state, "XBOX", Engine::CACHE_FILE_XBOX);
        luacs_enum_declare_value(state, "DEMO", Engine::CACHE_FILE_DEMO);
        luacs_enum_declare_value(state, "RETAIL", Engine::CACHE_FILE_RETAIL);
        luacs_enum_declare_value(state, "CUSTOM_EDITION", Engine::CACHE_FILE_CUSTOM_EDITION);
        luacs_enum_declare_value(state, "INVADER", Engine::CACHE_FILE_INVADER);
        luacs_enum_declare_value(state, "DEMO_COMPRESSED", Engine::CACHE_FILE_DEMO_COMPRESSED);
        luacs_enum_declare_value(state, "RETAIL_COMPRESSED", Engine::CACHE_FILE_RETAIL_COMPRESSED);
        luacs_enum_declare_value(state, "CUSTOM_EDITION_COMPRESSED", Engine::CACHE_FILE_CUSTOM_EDITION_COMPRESSED);
        lua_pop(state, 1);
    }

    static void define_engine_map_game_type_enum(lua_State *state) noexcept {
        luacs_newenum(state, EngineMapGameType);
        luacs_enum_declare_value(state, "SINGLE_PLAYER", Engine::MAP_SINGLE_PLAYER);
        luacs_enum_declare_value(state, "MULTIPLAYER", Engine::MAP_MULTIPLAYER);
        luacs_enum_declare_value(state, "USER_INTERFACE", Engine::MAP_USER_INTERFACE);
        lua_pop(state, 1);
    }

    static void define_mapheader_struct(lua_State *state) noexcept {
        luacs_newstruct(state, EngineMapHeader);
        luacs_unsigned_field(state, EngineMapHeader, head, LUACS_FREADONLY);
        luacs_enum_field(state, EngineMapHeader, EngineCacheFileEngine, engine_type, LUACS_FREADONLY);
        luacs_unsigned_field(state, EngineMapHeader, file_size, LUACS_FREADONLY);
        luacs_unsigned_field(state, EngineMapHeader, tag_data_offset, LUACS_FREADONLY);
        luacs_unsigned_field(state, EngineMapHeader, tag_data_size, LUACS_FREADONLY);
        luacs_string_field(state, EngineMapHeader, name, LUACS_FREADONLY);
        luacs_string_field(state, EngineMapHeader, build, LUACS_FREADONLY);
        luacs_enum_field(state, EngineMapHeader, EngineMapGameType, game_type, LUACS_FREADONLY);
        luacs_unsigned_field(state, EngineMapHeader, crc32, LUACS_FREADONLY);
        luacs_unsigned_field(state, EngineMapHeader, foot, LUACS_FREADONLY);
        lua_pop(state, 1); 
    }

    void push_meta_engine_map_header(lua_State *state, Engine::MapHeader *data) noexcept {
        luacs_newobject(state, EngineMapHeader, data);
    }

    void define_engine_map_types(lua_State *state) noexcept {
        define_engine_cache_file_engine_enum(state);
        define_engine_map_game_type_enum(state);
        define_mapheader_struct(state);
    }
}
