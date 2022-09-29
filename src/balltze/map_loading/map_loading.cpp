// SPDX-License-Identifier: GPL-3.0-only

#include <windows.h>
#include <filesystem>
#include <vector>
#include <deque>
#include <cstring>
#include <memory>
#include <iostream>

#include <balltze/config/ini.hpp>
#include <balltze/engine/map.hpp>
#include <balltze/engine/path.hpp>
#include <balltze/engine/tag.hpp>
#include <balltze/engine/tag_class.hpp>
#include <balltze/engine/version.hpp>
#include <balltze/events/map_load.hpp>
#include <balltze/map_loading/compression.hpp>
#include <balltze/map_loading/crc32.hpp>
#include <balltze/map_loading/laa.hpp>
#include <balltze/memory/hook.hpp>
#include <balltze/memory/memory.hpp>
#include <balltze/output/message_box.hpp>
#include <balltze/balltze.hpp>
#include <balltze/map_loading/map_loading.hpp>

#define HALO_PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679
#define DEGREES_TO_RADIANS(deg) (deg / 180.0 * HALO_PI)

namespace Balltze {
    using namespace Engine;

    static constexpr const char *tmp_format = "tmp_%zu.map";
    static constexpr const char *bitmaps_file = "bitmaps.map";
    static constexpr const char *sounds_file = "sounds.map";
    static constexpr const char *loc_file = "loc.map";
    static constexpr const char *custom_bitmaps_file = "custom_bitmaps.map";
    static constexpr const char *custom_sounds_file = "custom_sounds.map";
    static constexpr const char *custom_loc_file = "custom_loc.map";

    enum ResourceOrigin {
        RESOURCE_ORIGIN_CUSTOM_BIT     = 0b0100,
        
        RESOURCE_ORIGIN_BITMAPS        = 0b0000,
        RESOURCE_ORIGIN_SOUNDS         = 0b0001,
        RESOURCE_ORIGIN_LOC            = 0b0010,
        
        RESOURCE_ORIGIN_CUSTOM_BITMAPS = RESOURCE_ORIGIN_BITMAPS | RESOURCE_ORIGIN_CUSTOM_BIT,
        RESOURCE_ORIGIN_CUSTOM_SOUNDS  = RESOURCE_ORIGIN_SOUNDS  | RESOURCE_ORIGIN_CUSTOM_BIT,
        RESOURCE_ORIGIN_CUSTOM_LOC     = RESOURCE_ORIGIN_LOC     | RESOURCE_ORIGIN_CUSTOM_BIT
    };

    struct ResourceMetadata {
        ResourceOrigin origin;
        std::uint32_t offset;
        std::byte *data;
        std::size_t size;
    };
    
    static std::deque<LoadedMap> loaded_maps;
    static std::vector<ResourceMetadata> metadata;
    static std::byte *map_memory_buffer = nullptr;
    static std::size_t total_buffer_size = 0;
    static std::size_t max_temp_files = 3;
    static bool custom_edition_maps_supported = false;

    // Resource maps' tag data
    static std::vector<std::vector<std::byte>> custom_edition_bitmaps_tag_data;
    static std::vector<std::vector<std::byte>> custom_edition_sounds_tag_data;
    static std::vector<std::string> custom_edition_sounds_tag_data_paths;
    static std::vector<std::vector<std::byte>> custom_edition_loc_tag_data;
    static std::vector<bool> custom_edition_loc_tag_data_fixed; // we don't know what tag is what, so we depend on the map to determine that for us

    // Tag array
    static std::vector<Tag> tag_array;

    extern "C" {
        void data_map_loading_asm() noexcept;
        void map_loading_asm() noexcept;
        void on_read_map_file_data_asm() noexcept;
        void override_ting_volume_set_asm() noexcept;
        void override_ting_volume_write_asm() noexcept;
    }

    static const std::filesystem::path get_map_path() noexcept {
        static auto &balltze = Balltze::get();
        static auto &chimera_ini = balltze.chimera_ini();
        static std::optional<std::filesystem::path> path;

        if(!path.has_value()) {
            const char *ini_path = chimera_ini.get_value("halo.map_path");
            if(ini_path){
                path = std::filesystem::path(ini_path);
                std::filesystem::create_directory(*path);
            }
            else {
                path = std::filesystem::path("maps");
            }
        }
        return *path;
    }

    static const std::filesystem::path get_download_map_path() noexcept {
        static auto &balltze = Balltze::get();
        static auto &chimera_ini = balltze.chimera_ini();
        static std::optional<std::filesystem::path> path;

        if(!path.has_value()) {
            const char *ini_path = chimera_ini.get_value("halo.download_map_path");
            if(ini_path){
                path = std::filesystem::path(ini_path);
                std::filesystem::create_directory(*path);
            }
            else {
                path = std::filesystem::path("maps");
            }
        }
        return *path;
    }

    static std::filesystem::path path_for_tmp(std::size_t tmp) {
        char tmp_name[64];
        std::snprintf(tmp_name, sizeof(tmp_name), tmp_format, tmp);
        return get_path() / "chimera" / "tmp" / tmp_name;
    }

    static bool same_string_case_insensitive(const char *a, const char *b) {
        if(a == b) return true;
        while(std::tolower(*a) == std::tolower(*b)) {
            if(*a == 0) return true;
            a++;
            b++;
        }
        return false;
    }
    
    static std::optional<MapEntry> get_map_entry(const char *map_name) {
        // First, let's lowercase it
        char map_name_lowercase[32];
        std::strncpy(map_name_lowercase, map_name, sizeof(map_name_lowercase) - 1);
        for(auto &i : map_name_lowercase) {
            i = std::tolower(i);
        }
        
        // Add it!
        MapEntry map;
        map.name = map_name_lowercase;
        map.index = std::nullopt;
        map.multiplayer = true;
        
        // Make sure it exists first.
        if(!std::filesystem::exists(map.get_file_path())) {
            return std::nullopt;
        }
        
        // If it's known to not be a multiplayer map, set this
        static const char *NON_MULTIPLAYER_MAPS[] = {
            "a10",
            "a30",
            "a50",
            "b30",
            "b40",
            "c10",
            "c20",
            "c40",
            "d20",
            "d40",
            "ui"
        };
        for(auto &nmp : NON_MULTIPLAYER_MAPS) {
            if(same_string_case_insensitive(nmp, map_name)) {
                map.multiplayer = false;
            }
        }
        
        return std::move(map);
    }

    static std::filesystem::path path_for_map_local(const char *map_name) {
        return get_map_entry(map_name)->get_file_path();
    }

    static std::uint32_t calculate_crc32_of_map_file(const LoadedMap *map) noexcept {
        std::uint32_t crc = 0;
        std::uint32_t tag_data_size;
        std::uint32_t tag_data_offset;
        std::uint32_t current_offset = 0;
        
        auto *maps_in_ram_region = map->memory_location.value_or(nullptr);
        std::FILE *f = (maps_in_ram_region != nullptr) ? nullptr : std::fopen(map->path.string().c_str(), "rb");

        auto seek = [&f, &current_offset](std::size_t offset) {
            if(f) {
                std::fseek(f, offset, SEEK_SET);
            }
            else {
                current_offset = offset;
            }
        };

        auto read = [&f, &current_offset, &maps_in_ram_region](void *where, std::size_t size) {
            if(f) {
                std::fread(where, size, 1, f);
            }
            else {
                std::copy(maps_in_ram_region + current_offset, maps_in_ram_region + size + current_offset, reinterpret_cast<std::byte *>(where));
            }
            current_offset += size;
        };
        
        CacheFileEngine engine;
        union {
            MapHeaderDemo demo_header;
            MapHeader fv_header;
        } header;
        seek(0);
        read(&header, sizeof(header));
        
        if(get_engine_type() == ENGINE_TYPE_DEMO && header.demo_header.is_valid()) {
            engine = header.demo_header.engine_type;
            tag_data_size = header.demo_header.tag_data_size;
            tag_data_offset = header.demo_header.tag_data_offset;
        }
        else {
            engine = header.fv_header.engine_type;
            tag_data_size = header.fv_header.tag_data_size;
            tag_data_offset = header.fv_header.tag_data_offset;
        }
        
        std::uint32_t tag_data_addr;
        switch(engine) {
            case CacheFileEngine::CACHE_FILE_DEMO:
                tag_data_addr = 0x4BF10000;
                break;
            default:
                tag_data_addr = 0x40440000;
                break;
        }

        // Load tag data
        auto tag_data_ptr = std::make_unique<std::byte []>(tag_data_size);
        auto *tag_data = tag_data_ptr.get();
        seek(tag_data_offset);
        read(tag_data, tag_data_size);

        // Get the scenario tag so we can get the BSPs
        auto *scenario_tag = tag_data + (*reinterpret_cast<std::uint32_t *>(tag_data) - tag_data_addr) + (*reinterpret_cast<std::uint32_t *>(tag_data + 4) & 0xFFFF) * 0x20;
        auto *scenario_tag_data = tag_data + (*reinterpret_cast<std::uint32_t *>(scenario_tag + 0x14) - tag_data_addr);

        // CRC32 the BSP(s)
        auto &structure_bsp_count = *reinterpret_cast<std::uint32_t *>(scenario_tag_data + 0x5A4);
        auto *structure_bsps = tag_data + (*reinterpret_cast<std::uint32_t *>(scenario_tag_data + 0x5A4 + 4) - tag_data_addr);
        for(std::size_t b=0;b<structure_bsp_count;b++) {
            auto *bsp = structure_bsps + b * 0x20;
            auto &bsp_offset = *reinterpret_cast<std::uint32_t *>(bsp);
            auto &bsp_size = *reinterpret_cast<std::uint32_t *>(bsp + 4);

            auto bsp_data = std::make_unique<std::byte []>(bsp_size);
            seek(bsp_offset);
            read(bsp_data.get(), bsp_size);
            crc = crc32(crc, bsp_data.get(), bsp_size);
        }

        // Next, CRC32 the model data
        auto &model_vertices_offset = *reinterpret_cast<std::uint32_t *>(tag_data + 0x14);
        auto &vertices_size = *reinterpret_cast<std::uint32_t *>(tag_data + 0x20);

        auto model_vertices = std::make_unique<std::byte []>(vertices_size);
        seek(model_vertices_offset);
        read(model_vertices.get(), vertices_size);
        crc = crc32(crc, model_vertices.get(), vertices_size);

        // Lastly, CRC32 the tag data itself
        crc = crc32(crc, tag_data, tag_data_size);

        return crc;
    }

    static void unload_maps() {
        while(loaded_maps.size() > 1) {
            loaded_maps.pop_back();
        }
    }

    static bool fix_tag(std::vector<std::byte> &tag_data, TagClassInt primary_class) noexcept {
        std::byte *base = tag_data.data();
        auto base_offset = reinterpret_cast<std::uint32_t>(tag_data.data());
        
        // If we're loc, mark as such
        for(auto &i : custom_edition_loc_tag_data) {
            if(&i == &tag_data) {
                std::size_t index = &i - custom_edition_loc_tag_data.data();
                if(custom_edition_loc_tag_data_fixed[index]) {
                    return true;
                }
                else {
                    custom_edition_loc_tag_data_fixed[index] = true;
                }
            }
        }
        
        auto increment_if_necessary = [&base_offset](auto *what) {
            auto &ptr = *reinterpret_cast<std::byte **>(what);
            if(ptr != 0) {
                ptr += base_offset;
            }
        };
        
        switch(primary_class) {
            case TagClassInt::TAG_CLASS_BITMAP: {
                increment_if_necessary(base + 0x54 + 0x4);
                increment_if_necessary(base + 0x60 + 0x4);
                auto sequence_count = *reinterpret_cast<std::uint32_t *>(base + 0x54);
                auto *sequences = *reinterpret_cast<std::byte **>(base + 0x54 + 0x4);
                for(std::uint32_t s = 0; s < sequence_count; s++) {
                    increment_if_necessary(sequences + s * 64 + 0x34 + 0x4);
                }
                break;
            }
            case TagClassInt::TAG_CLASS_SOUND: {
                // Let's begin.
                auto pitch_range_count = *reinterpret_cast<std::uint32_t *>(base + 0x98);
                
                // Add this to account for the header
                base_offset += 0xA4;
                base += 0xA4;

                for(std::uint32_t p = 0; p < pitch_range_count; p++) {
                    auto *pitch_range = base + p * 72;
                    increment_if_necessary(pitch_range + 0x3C + 0x4);
                    
                    auto permutation_count = *reinterpret_cast<std::uint32_t *>(pitch_range + 0x3C);
                    auto permutations = *reinterpret_cast<std::byte **>(pitch_range + 0x3C + 0x4);

                    *reinterpret_cast<std::uint32_t *>(pitch_range + 0x34) = 0xFFFFFFFF;
                    *reinterpret_cast<std::uint32_t *>(pitch_range + 0x38) = 0xFFFFFFFF;

                    for(std::uint32_t pe = 0; pe < permutation_count; pe++) {
                        auto *permutation = permutations + pe * 124;

                        *reinterpret_cast<std::uint32_t *>(permutation + 0x2C) = 0xFFFFFFFF;
                        *reinterpret_cast<std::uint32_t *>(permutation + 0x30) = 0;

                        increment_if_necessary(permutation + 0x54 + 0xC);
                        increment_if_necessary(permutation + 0x68 + 0xC);
                        
                        *reinterpret_cast<std::uint32_t *>(permutation + 0x2C) = 0xFFFFFFFF;
                    }
                }
                break;
            }
            case TagClassInt::TAG_CLASS_FONT: {
                increment_if_necessary(base + 0x7C + 0x4);
                increment_if_necessary(base + 0x30 + 0x4);
                increment_if_necessary(base + 0x88 + 0xC);
                std::uint32_t table_count = *reinterpret_cast<std::uint32_t *>(base + 0x30);
                auto *tables = *reinterpret_cast<std::byte **>(base + 0x30 + 0x4);
                for(std::uint32_t t = 0; t < table_count; t++) {
                    increment_if_necessary(tables + t * 12 + 0x0 + 0x4);
                }
                break;
            }
            case TagClassInt::TAG_CLASS_UNICODE_STRING_LIST: {
                increment_if_necessary(base + 0x0 + 0x4);
                std::uint32_t string_count = *reinterpret_cast<std::uint32_t *>(base + 0x0);
                std::byte *strings = *reinterpret_cast<std::byte **>(base + 0x0 + 0x4);
                for(std::uint32_t s = 0; s < string_count; s++) {
                    increment_if_necessary(strings + s * 20 + 0x0 + 0xC);
                }
                break;
            }
            case TagClassInt::TAG_CLASS_HUD_MESSAGE_TEXT: {
                increment_if_necessary(base + 0x0 + 0xC);
                increment_if_necessary(base + 0x14 + 0x4);
                increment_if_necessary(base + 0x20 + 0x4);
                break;
            }
            default:
                break;
        }
        
        return true;
    }

    template <typename T> static std::vector<std::byte> &translate_index(T index, std::vector<std::vector<std::byte>> &of_what) {
        auto index_val = reinterpret_cast<std::uint32_t>(index);
        if(index_val >= of_what.size()) {
            show_error_box("Map error", "Map could not be loaded due to an invalid index.");
            std::exit(EXIT_FAILURE);
        }
        return of_what[index_val];
    }

    static void resolve_indexed_tags() {
        auto &header = get_map_header();
        
        // Do nothing if not custom edition
        if(header.engine_type != CacheFileEngine::CACHE_FILE_CUSTOM_EDITION) {
            return;
        }
        
        // First, update tags
        auto &tag_data_header = get_tag_data_header();
        auto *tags = tag_data_header.tag_array;
        auto tag_count = tag_data_header.tag_count;
        for(std::uint32_t i = 0; i < tag_count; i++) {
            auto &tag = tags[i];
            if(tag.indexed) {
                // Mark as non-indexed
                tag.indexed = 0;
                auto tag_data = tag.data;
                
                switch(tag.primary_class) {
                    case TagClassInt::TAG_CLASS_BITMAP:
                        tag.data = translate_index(tag_data, custom_edition_bitmaps_tag_data).data();
                        break;
                    case TagClassInt::TAG_CLASS_SOUND: {
                        std::optional<std::size_t> path_index;
                        
                        // Set this stuff
                        for(auto &s : custom_edition_sounds_tag_data_paths) {
                            if(s == tag.path) {
                                path_index = &s - custom_edition_sounds_tag_data_paths.data();
                                break;
                            }
                        }
                        
                        std::uint32_t index = path_index.value_or(0xFFFFFFFF);
                        auto *sound_data = translate_index(index, custom_edition_sounds_tag_data).data();
                        
                        *reinterpret_cast<std::byte **>(tag_data + 0x98 + 0x4) = sound_data + 0xA4;
                        
                        // Copy over channel count and format
                        *reinterpret_cast<std::uint16_t *>(tag_data + 0x6C) = *reinterpret_cast<std::uint16_t *>(sound_data + 0x6C);
                        *reinterpret_cast<std::uint16_t *>(tag_data + 0x6E) = *reinterpret_cast<std::uint16_t *>(sound_data + 0x6E);

                        // Copy over sample rate
                        *reinterpret_cast<std::uint16_t *>(tag_data + 0x6) = *reinterpret_cast<std::uint16_t *>(sound_data + 0x6);

                        // Copy over longest permutation length
                        *reinterpret_cast<std::uint32_t *>(tag_data + 0x84) = *reinterpret_cast<std::uint32_t *>(sound_data + 0x84);
                        
                        // Get the tag ID
                        auto tag_id = tag.id;
                        
                        // Fix those tag IDs
                        std::uint32_t pitch_range_count = *reinterpret_cast<std::uint32_t *>(sound_data + 0x98);
                        auto *pitch_ranges = *reinterpret_cast<std::byte **>(tag_data + 0x98 + 4);
                        
                        // Copy over the pitch range count while we're at it
                        *reinterpret_cast<std::uint32_t *>(tag_data + 0x98) = pitch_range_count;
                        
                        // Okay, now actually fix the tag IDs.
                        for(std::uint32_t pr = 0; pr < pitch_range_count; pr++) {
                            auto *pitch_range = pitch_ranges + pr * 0x48;
                            
                            auto permutation_count = *reinterpret_cast<std::uint32_t *>(pitch_range + 0x3C);
                            auto *permutation_ptr = *reinterpret_cast<std::byte **>(pitch_range + 0x3C + 0x4);
                            
                            for(std::uint32_t pe = 0; pe < permutation_count; pe++) {
                                auto *permutation = permutation_ptr + pe * 0x7C;
                                *reinterpret_cast<TagID *>(permutation + 0x34) = tag_id;
                                *reinterpret_cast<TagID *>(permutation + 0x3C) = tag_id;
                            }
                        }
                        
                        break;
                    }
                    default: {
                        auto &tag_data_vector = translate_index(tag_data, custom_edition_loc_tag_data);
                        tag.data = tag_data_vector.data();
                        fix_tag(tag_data_vector, tag.primary_class);
                        break;
                    }
                }
            }
        }
        
        // Next, fix stock tags
        if(
            get_engine_type() != ENGINE_TYPE_CUSTOM_EDITION &&
            (
                std::strcmp("beavercreek",header.name) == 0 ||
                std::strcmp("bloodgulch",header.name) == 0 ||
                std::strcmp("boardingaction",header.name) == 0 ||
                std::strcmp("carousel",header.name) == 0 ||
                std::strcmp("chillout",header.name) == 0 ||
                std::strcmp("damnation",header.name) == 0 ||
                std::strcmp("dangercanyon",header.name) == 0 ||
                std::strcmp("deathisland",header.name) == 0 ||
                std::strcmp("gephyrophobia",header.name) == 0 ||
                std::strcmp("hangemhigh",header.name) == 0 ||
                std::strcmp("icefields",header.name) == 0 ||
                std::strcmp("infinity",header.name) == 0 ||
                std::strcmp("longest",header.name) == 0 ||
                std::strcmp("prisoner",header.name) == 0 ||
                std::strcmp("putput",header.name) == 0 ||
                std::strcmp("ratrace",header.name) == 0 ||
                std::strcmp("sidewinder",header.name) == 0 ||
                std::strcmp("timberland",header.name) 
            )
        ) {
            bool in_custom_edition_server = false;

            // Fix the stun values
            auto jj_stun = [&in_custom_edition_server](const char *path) {
                auto *tag = get_tag(path, TagClassInt::TAG_CLASS_DAMAGE_EFFECT);
                if(tag) {
                    float new_damage_stun = in_custom_edition_server ? 0.0F : 1.0F;
                    float damage_maximum_stun = in_custom_edition_server ? 0.0F : 1.0F;
                    float damage_stun_time = in_custom_edition_server ? 0.0F : 0.15F;
                    *reinterpret_cast<float *>(tag->data + 0x1E4) = new_damage_stun;
                    *reinterpret_cast<float *>(tag->data + 0x1E8) = damage_maximum_stun;
                    *reinterpret_cast<float *>(tag->data + 0x1EC) = damage_stun_time;
                }
            };
            jj_stun("vehicles\\banshee\\banshee bolt");
            jj_stun("vehicles\\ghost\\ghost bolt");

            // Fix the rwarthog's angles
            auto jj_rwarthog = [&in_custom_edition_server](const char *path) {
                auto *tag = get_tag(path, TagClassInt::TAG_CLASS_WEAPON);
                if(tag) {
                    float new_autoaim_angle = in_custom_edition_server ? DEGREES_TO_RADIANS(6.0F) : DEGREES_TO_RADIANS(1.0F);
                    float new_deviation_angle = in_custom_edition_server ? DEGREES_TO_RADIANS(12.0F) : DEGREES_TO_RADIANS(1.0F);
                    *reinterpret_cast<float *>(tag->data + 0x3E4) = new_autoaim_angle;
                    *reinterpret_cast<float *>(tag->data + 0x3F4) = new_deviation_angle;
                }
            };
            jj_rwarthog("vehicles\\rwarthog\\rwarthog_gun");
        }
    }

    static void set_override_ting(bool should_override) {
        static auto &sig_manager = Balltze::get().signature_manager();

        static Memory::Hook set_flag;
        static auto *ting_sound_call_sig = sig_manager.get("ting_sound_call");
        if(!set_flag.is_initialized()) {
            Memory::remove_chimera_hook(ting_sound_call_sig->data(), false, true, false);
            set_flag.initialize(ting_sound_call_sig->data(), nullptr, reinterpret_cast<void *>(override_ting_volume_set_asm), false);
        }

        static Memory::Hook set_float;
        static auto *game_event_volume_sig = sig_manager.get("game_event_volume");
        if(!set_float.is_initialized()) {
            Memory::remove_chimera_hook(game_event_volume_sig->data(), false, true, false);
            set_float.initialize(game_event_volume_sig->data(), nullptr, reinterpret_cast<void *>(override_ting_volume_write_asm), false);
        }
        
        if(should_override) {
            set_flag.hook();
            set_float.hook();
        }
        else {
            set_flag.release();
            set_float.release();
        }
    }

    static void preload_assets(LoadedMap &map) {
        // If we can't, don't
        if(!map.memory_location.has_value()) {
            return;
        }
        
        // Set this byte stuff
        std::byte *cursor = *map.memory_location + map.loaded_size;
        auto *end = *map.memory_location + map.buffer_size;
        
        std::byte *tag_data = get_tag_data_address();
        auto &tag_data_header = *reinterpret_cast<TagDataHeader *>(tag_data);
        
        std::FILE *bitmaps = nullptr;
        std::FILE *sounds = nullptr;
        
        CacheFileEngine map_engine;
        auto current_engine = get_engine_type();
        
        if(current_engine == ENGINE_TYPE_DEMO) {
            auto &header = *reinterpret_cast<MapHeaderDemo *>(*map.memory_location);
            map_engine = header.engine_type;
        }
        else {
            auto &header = *reinterpret_cast<MapHeader *>(*map.memory_location);
            map_engine = header.engine_type;
        }
        
        bool can_load_indexed_tags = map_engine == CacheFileEngine::CACHE_FILE_CUSTOM_EDITION;
        std::filesystem::path bitmaps_path, sounds_path;

        auto map_path = get_map_path();
        if(map_engine == CacheFileEngine::CACHE_FILE_CUSTOM_EDITION && current_engine != ENGINE_TYPE_CUSTOM_EDITION) {
            bitmaps_path = map_path / custom_bitmaps_file;
            sounds_path = map_path / custom_sounds_file;
        }
        else {
            bitmaps_path = map_path / bitmaps_file;
            sounds_path = map_path / sounds_file;
        }
        
        // If it's a custom edition map, we ought to first figure out what tags go to what
        const std::uint32_t tag_count = tag_data_header.tag_count;
        
        Tag *tag_array = reinterpret_cast<Tag *>(tag_data_header.tag_array);
        
        auto preload_asset_maybe = [&cursor, &end, &can_load_indexed_tags](std::uint32_t offset, std::uint32_t size, std::FILE *from, ResourceOrigin origin) -> bool {
            if(can_load_indexed_tags) {
                origin = static_cast<ResourceOrigin>(origin | ResourceOrigin::RESOURCE_ORIGIN_CUSTOM_BIT);
            }
            
            // Already present? No need then.
            for(auto &i : metadata) {
                if(i.origin == origin && i.offset == offset && i.size >= size) {
                    return true;
                }
            }
            
            // Let's think about this. Make sure it isn't too big
            std::byte *new_cursor = cursor + size;
            if(new_cursor < cursor || new_cursor > end) {
                return false;
            }
            
            // Navigate to this
            std::fseek(from, offset, SEEK_SET);
            std::fread(cursor, size, 1, from);
            
            // Set asset data
            auto &new_asset = metadata.emplace_back();
            new_asset.data = cursor;
            new_asset.origin = origin;
            new_asset.offset = offset;
            new_asset.size = size;
            
            // Increment the cursor
            cursor = new_cursor;
        
            return true;
        };
        
        auto preload_all_tags_of_class = [&preload_asset_maybe, &tag_count, &tag_array, &bitmaps, &sounds](TagClassInt class_int) {
            for(std::uint32_t i = 0; i < tag_count; i++) {
                auto &tag = tag_array[i];
                
                if(tag.primary_class == class_int) {
                    switch(class_int) {
                        case TagClassInt::TAG_CLASS_BITMAP: {
                            auto *td = tag.data;
                            
                            auto *bitmap_data = *reinterpret_cast<std::byte **>(td + 0x60 + 0x4);
                            std::uint32_t bitmap_count = *reinterpret_cast<std::uint32_t *>(td + 0x60);
                            
                            for(std::uint32_t bd = 0; bd < bitmap_count; bd++) {
                                auto *bitmap = bitmap_data + bd * 0x40;
                                
                                bool external = *reinterpret_cast<std::uint8_t *>(bitmap + 0xF) & 1;
                                
                                // Ignore internal tags
                                if(!external) {
                                    continue;
                                }
                                
                                std::uint32_t bitmap_size = *reinterpret_cast<std::uint32_t *>(bitmap + 0x1C);
                                std::uint32_t bitmap_offset = *reinterpret_cast<std::uint32_t *>(bitmap + 0x18);
                                
                                preload_asset_maybe(bitmap_offset, bitmap_size, bitmaps, ResourceOrigin::RESOURCE_ORIGIN_BITMAPS);
                            }
                            
                            break;
                        }
                        case TagClassInt::TAG_CLASS_SOUND: {
                            auto *td = tag.data;
                            
                            auto pitch_range_count = *reinterpret_cast<std::uint32_t *>(td + 0x98);
                            auto *pitch_ranges = *reinterpret_cast<std::byte **>(td + 0x98 + 0x4);
                            
                            for(std::uint32_t pr = 0; pr < pitch_range_count; pr++) {
                                auto *pitch_range = pitch_ranges + pr * 0x48;
                                
                                auto permutation_count = *reinterpret_cast<std::uint32_t *>(pitch_range + 0x3C);
                                auto *permutation_ptr = *reinterpret_cast<std::byte **>(pitch_range + 0x3C + 0x4);
                                
                                for(std::uint32_t pe = 0; pe < permutation_count; pe++) {
                                    auto *permutation = permutation_ptr + pe * 0x7C;
                                    
                                    bool external = *reinterpret_cast<std::uint8_t *>(permutation + 0x44) & 1;
                                    
                                    // Ignore internal tags
                                    if(!external) {
                                        continue;
                                    }
                                    
                                    std::uint32_t sound_offset = *reinterpret_cast<std::uint32_t *>(permutation + 0x48);
                                    std::uint32_t sound_size = *reinterpret_cast<std::uint32_t *>(permutation + 0x40);
                                    
                                    preload_asset_maybe(sound_offset, sound_size, sounds, ResourceOrigin::RESOURCE_ORIGIN_SOUNDS);
                                }
                            }
                            
                            break;
                        }
                        
                        default: break;
                    }
                }
            }
        };
        
        bitmaps = std::fopen(bitmaps_path.string().c_str(), "rb");
        sounds = std::fopen(sounds_path.string().c_str(), "rb");
        
        // If stuff's missing... um... how? Anyway give up.
        if(!bitmaps || !sounds) {
            goto done_preloading_assets;
        }
        
        // Prioritize loading sounds over bitmaps
        preload_all_tags_of_class(TagClassInt::TAG_CLASS_SOUND);
        preload_all_tags_of_class(TagClassInt::TAG_CLASS_BITMAP);
        
        // Cleanup
        done_preloading_assets:
        
        map.loaded_size = (cursor - *map.memory_location);
        map.buffer_size = end - cursor;
        if(bitmaps) {
            std::fclose(bitmaps);
        }
        if(sounds) {
            std::fclose(sounds);
        }
    }

    static void preload_and_resolve() {
        // Resolve indexed tags
        if(custom_edition_maps_supported) {
            resolve_indexed_tags();
            
            // Toggle if the ting is overrided
            if(get_engine_type() == ENGINE_TYPE_RETAIL) {
                set_override_ting(get_map_header().engine_type == CacheFileEngine::CACHE_FILE_CUSTOM_EDITION);
            }
        }
        
        // Preload it all
        preload_assets(*get_loaded_map(get_map_name()));
    }

    static bool set_up_custom_edition_map_support() {
        std::FILE *bitmaps = nullptr;
        std::FILE *sounds = nullptr;
        std::FILE *loc = nullptr;
        auto is_custom_edition = get_engine_type() == ENGINE_TYPE_CUSTOM_EDITION;
        
        auto maps_folder = get_map_path();
        
        bitmaps = std::fopen((maps_folder / custom_bitmaps_file).string().c_str(), "rb");
        sounds = std::fopen((maps_folder / custom_sounds_file).string().c_str(), "rb");
        loc = std::fopen((maps_folder / custom_loc_file).string().c_str(), "rb");
        
        auto try_close = [](auto *&what) {
            if(what) {
                std::fclose(what);
            }
            what = nullptr;
        };
        
        if(is_custom_edition) {
            if(bitmaps && sounds && loc) {
                // TODODILE: make Halo Custom Edition load these files instead
                std::printf("fixme:set_up_custom_edition_map_support:custom_* maps are not yet supported\n");
            }
            
            // Fail on Custom Edition - try opening the normal ones
            try_close(bitmaps);
            try_close(sounds);
            try_close(loc);
            
            bitmaps = std::fopen((maps_folder / bitmaps_file).string().c_str(), "rb");
            sounds = std::fopen((maps_folder / sounds_file).string().c_str(), "rb");
            loc = std::fopen((maps_folder / loc_file).string().c_str(), "rb");
        }
        
        // Fail
        if(!bitmaps || !sounds || !loc) {
            try_close(bitmaps);
            try_close(sounds);
            try_close(loc);
            
            if(is_custom_edition) {
                show_error_box("Map error", "Missing bitmaps.map/sounds.map/loc.map or custom_bitmaps.map/custom_sounds.map/custom_loc.map from your maps folder.");
                std::exit(EXIT_FAILURE);
            }
            return false;
        }
        
        auto read_all_tags = [](std::FILE *from, auto &to_what, std::vector<std::string> *to_what_paths) -> bool {
            struct {
                std::uint32_t type;
                std::uint32_t paths;
                std::uint32_t resources;
                std::uint32_t resource_count;
            } header;
            
            #define read_at(offset, what) \
                std::fseek(from, offset, SEEK_SET); \
                if(std::fread(&what, sizeof(what), 1, from) != 1) { \
                    return false; \
                }
            
            // Read to this address
            read_at(0, header);
            
            // Read every other?
            bool read_every_other = &to_what != &custom_edition_loc_tag_data;
            
            // Reserve
            to_what.reserve((header.resource_count + 1) / (read_every_other ? 2 : 1));
            
            // Read
            for(std::uint32_t i = 0; i < header.resource_count; i++) {
                struct {
                    std::uint32_t path_offset = 0;
                    std::uint32_t size = 0;
                    std::uint32_t data_offset = 0;
                } resource;
                
                // Skip?
                bool skip_this = read_every_other && ((i % 2) == 0);
                
                // Read the resource
                if(!skip_this) {
                    read_at(header.resources + i * sizeof(resource), resource);
                }
                
                // Read the data
                auto &data = to_what.emplace_back(resource.size);
                if(!skip_this) {
                    std::fseek(from, resource.data_offset, SEEK_SET);
                    if(std::fread(data.data(), resource.size, 1, from) != 1) {
                        return false;
                    }
                }
                
                // Read the path?
                if(to_what_paths) {
                    auto &str = to_what_paths->emplace_back();
                    
                    if(!skip_this) {
                        std::uint32_t read_offset = resource.path_offset + header.paths;
                        char c;
                        while(true) {
                            read_at(read_offset++, c);
                            if(c) {
                                str += c;
                            }
                            else {
                                break;
                            }
                        }
                    }
                }
            }
            
            return true;
        };
        
        bool read_success = read_all_tags(bitmaps, custom_edition_bitmaps_tag_data, nullptr) &&
                            read_all_tags(sounds, custom_edition_sounds_tag_data, &custom_edition_sounds_tag_data_paths) &&
                            read_all_tags(loc, custom_edition_loc_tag_data, nullptr);
        
        try_close(bitmaps);
        try_close(sounds);
        try_close(loc);
        
        if(!read_success) {
            show_error_box("Map error", "Failed to read resource maps. Files possibly corrupt or unreadable.");
            std::exit(EXIT_FAILURE);
        }
        
        auto fix_tags = [](TagClassInt primary_class, auto &tags) -> bool {
            for(auto &i : tags) {
                if(i.size()) {
                    if(!fix_tag(i, primary_class)) {
                        return false;
                    }
                }
            }
            return true;
        };
        
        bool fix_success = fix_tags(TagClassInt::TAG_CLASS_BITMAP, custom_edition_bitmaps_tag_data) &&
                           fix_tags(TagClassInt::TAG_CLASS_SOUND, custom_edition_sounds_tag_data);
                           
        if(!fix_success) {
            show_error_box("Map error", "Failed to read resource maps. Files possibly corrupt or unreadable.");
            std::exit(EXIT_FAILURE);
        }
        
        // Hold this
        custom_edition_loc_tag_data_fixed.resize(custom_edition_loc_tag_data.size());
        
        // Set up resolving indices on load
        auto &sig_manager = Balltze::get().signature_manager();
        if(!is_custom_edition) {
            auto *retail_check_version_1 = sig_manager.get("retail_check_version_1");
            auto *retail_check_version_2 = sig_manager.get("retail_check_version_2");
            Memory::overwrite(retail_check_version_1->data(), static_cast<std::uint16_t>(0x9090));
            Memory::overwrite(retail_check_version_2->data(), static_cast<std::uint8_t>(0xEB));
            add_map_load_event(preload_and_resolve, EventPriority::EVENT_PRIORITY_BEFORE);
        }
        else {
            auto *map_load_resolve_indexed_tags_sig = sig_manager.get("map_load_resolve_indexed_tags");
            static Memory::Hook resolve_indexed_tags_hook;
            Memory::remove_chimera_hook(map_load_resolve_indexed_tags_sig->data(), true, false, true);
            try {
                resolve_indexed_tags_hook.initialize(map_load_resolve_indexed_tags_sig->data(), reinterpret_cast<void *>(resolve_indexed_tags));

            }
            catch(std::exception &e) {
                show_error_box("Failed to hook map load resolve indexed tags. %p", map_load_resolve_indexed_tags_sig->data());
                std::exit(EXIT_FAILURE);
            }
            // resolve_indexed_tags_hook.hook();
        }
        
        return true;
    }

    extern "C" bool using_custom_map_on_retail() {
        return get_map_header().engine_type == CacheFileEngine::CACHE_FILE_CUSTOM_EDITION && get_engine_type() == ENGINE_TYPE_RETAIL;
    }

    extern "C" void get_data_map_path(char *map_path, const char *map_name) {
        // Handles loading {bitmaps,sounds,loc}.map - only supports loading from the map dir
        std::strcpy(map_path, (get_map_path() / (std::string(map_name) + ".map")).string().c_str());
    }

    extern "C" void do_map_loading_handling(char *map_path, const char *map_name) {
        if(loaded_maps.size() > 1) {
            if(loaded_maps[1].secondary) {
                if(std::strcmp(map_name, "ui") != 0) {
                    unload_maps();
                }
            }
            else {
                if(loaded_maps[1].name != map_name) {
                    unload_maps();
                }
            }
        }
        std::strcpy(map_path, load_map(map_name)->path.string().c_str());
        load_map("ui", true);
    }

    extern "C" int on_read_map_file_data(HANDLE file_descriptor, std::byte *output, std::size_t size, LPOVERLAPPED overlapped) {
        std::size_t file_offset = overlapped->Offset;
        
        // Get the name
        char file_path_chars[MAX_PATH + 1] = {};
        GetFinalPathNameByHandle(file_descriptor, file_path_chars, sizeof(file_path_chars) - 1, VOLUME_NAME_NONE);
        auto file_path = std::filesystem::path(file_path_chars);
        
        // If it's not a .map file, forget about it
        char file_path_extension[5] = {};
        std::snprintf(file_path_extension, sizeof(file_path_extension), "%s", file_path.extension().string().c_str());
        for(auto &fpe : file_path_extension) {
            fpe = std::tolower(fpe);
        }
        
        // Get the resource file if possible
        auto file_name = file_path.filename();
        std::optional<ResourceOrigin> origin;
        
        if(file_name == custom_bitmaps_file) {
            origin = ResourceOrigin::RESOURCE_ORIGIN_CUSTOM_BITMAPS;
        }
        else if(file_name == custom_sounds_file) {
            origin = ResourceOrigin::RESOURCE_ORIGIN_CUSTOM_SOUNDS;
        }
        else if(file_name == custom_loc_file) {
            origin = ResourceOrigin::RESOURCE_ORIGIN_CUSTOM_LOC;
        }
        else if(file_name == bitmaps_file) {
            origin = ResourceOrigin::RESOURCE_ORIGIN_BITMAPS;
        }
        else if(file_name == sounds_file) {
            origin = ResourceOrigin::RESOURCE_ORIGIN_SOUNDS;
        }
        else if(file_name == loc_file) {
            origin = ResourceOrigin::RESOURCE_ORIGIN_LOC;
        }

        if(origin.has_value()) {
            // load this map now!
            load_map(get_map_name());
            
            bool custom_edition_memes = get_map_header().engine_type == CACHE_FILE_CUSTOM_EDITION;
            
            // If we're on retail and we are loading from a custom edition map's resource map, handle that
            if(custom_edition_memes) {
                origin = static_cast<ResourceOrigin>(*origin | ResourceOrigin::RESOURCE_ORIGIN_CUSTOM_BIT);
            }
            
            // Copy it in?
            for(auto &md : metadata) {
                if(md.origin == origin && file_offset == md.offset && size <= md.size) {
                    std::memcpy(output, md.data, size);
                    return 1;
                }
            }
            
            // If we don't have it precached, read from disk
            if(get_engine_type() == ENGINE_TYPE_RETAIL && (*origin & ResourceOrigin::RESOURCE_ORIGIN_CUSTOM_BIT)) {
                auto maps = get_map_path();
                std::FILE *f = nullptr;
                std::filesystem::path map_path;
                
                switch(*origin) {
                    case ResourceOrigin::RESOURCE_ORIGIN_CUSTOM_BITMAPS:
                        map_path = maps / custom_bitmaps_file;
                        break;
                    case ResourceOrigin::RESOURCE_ORIGIN_CUSTOM_SOUNDS:
                        map_path = maps / custom_sounds_file;
                        break;
                    case ResourceOrigin::RESOURCE_ORIGIN_CUSTOM_LOC:
                        map_path = maps / custom_loc_file;
                        break;
                    default:
                        return 0;
                }
                
                if((f = std::fopen(map_path.string().c_str(), "rb"))) {
                    std::fseek(f, file_offset, SEEK_SET);
                    std::fread(output, size, 1, f);
                    std::fclose(f);
                    return 1;
                }
                else {
                    char error[2048];
                    std::snprintf(error, sizeof(error), "%s could not be opened", map_path.string().c_str());
                    show_error_box("Map error", error);
                    std::exit(EXIT_FAILURE);
                }
            }
            
            return 0;
        }

        else {
            // Check if we're a tmp file
            auto file_name_str = file_name.stem().string();
            auto *file_name_cstr = file_name_str.c_str();
            if(std::strncmp(file_name_cstr, tmp_format, 4) == 0) {
                for(auto &i : loaded_maps) {
                    if(i.tmp_file.has_value()) {
                        // Format it
                        char tmp_name[64];
                        std::snprintf(tmp_name, sizeof(tmp_name), tmp_format, *i.tmp_file);
                        
                        // Truncate the extension
                        for(auto &c : tmp_name) {
                            if(c == '.') {
                                c = 0;
                                break;
                            }
                        }
                        
                        if(file_name_str == tmp_name) {
                            return 0;
                        }
                    }
                }
            }
            
            // Load the map if it's not loaded
            auto *map = load_map(file_name_cstr);
            if(map && map->memory_location.has_value()) {
                if(output == get_tag_data_address()) {
                    std::size_t copied_bytes = 0;
                    
                    // tag data header
                    std::memcpy(output, map->memory_location.value() + file_offset, sizeof(TagDataHeader));
                    copied_bytes += sizeof(TagDataHeader);
                    
                    // tag array
                    auto &tag_data_header = get_tag_data_header();
                    auto *tag_array_raw = reinterpret_cast<Tag *>(map->memory_location.value() + file_offset + copied_bytes);
                    tag_array.clear();
                    tag_array.resize(tag_data_header.tag_count);
                    for(std::size_t i = 0; i < tag_data_header.tag_count; i++) {
                        tag_array[i] = tag_array_raw[i];
                    }
                    tag_data_header.tag_array = tag_array.data();
                    copied_bytes += sizeof(Tag) * tag_data_header.tag_count;

                    // rest of tag data
                    std::memcpy(output + copied_bytes, map->memory_location.value() + file_offset + copied_bytes, size - copied_bytes);

                    // Append secondary maps tag list
                    for(auto &m : loaded_maps) {
                        if(!m.secondary) {
                            continue;
                        }

                        auto secondary_map_header = reinterpret_cast<MapHeader *>(m.memory_location.value());
                        auto *secondary_map_tag_data_header = reinterpret_cast<TagDataHeader *>(m.memory_location.value() + secondary_map_header->tag_data_offset);
                        auto secondary_map_tag_array_raw = reinterpret_cast<Tag *>(secondary_map_tag_data_header + 1);

                        for(std::size_t i = 0; i < secondary_map_tag_data_header->tag_count; i++) {
                            if(secondary_map_tag_array_raw[i].primary_class != TagClassInt::TAG_CLASS_BITMAP || secondary_map_tag_array_raw[i].indexed) {
                                continue;
                            }

                            auto disp = reinterpret_cast<std::uint32_t>(get_tag_data_address()) - reinterpret_cast<std::uint32_t>(secondary_map_tag_data_header);

                            #define TRANSLATE_ADDRESS(x) reinterpret_cast<decltype(x)>(reinterpret_cast<std::byte *>(x) - disp)

                            tag_array.insert(tag_array.end(), secondary_map_tag_array_raw[i]);
                            auto &tag = tag_array.back();
                            auto &previous_tag = tag_array[tag_array.size() - 2];
                            tag.id.index.index = previous_tag.id.index.index + 1;
                            tag.id.index.id = previous_tag.id.index.id + 1;
                            tag.path = TRANSLATE_ADDRESS(tag.path);
                            tag.data = TRANSLATE_ADDRESS(tag.data);
                            tag_data_header.tag_count++;


                            // if tags are already fixed we can continue
                            if(m.fixed_tags) {
                               continue; 
                            }
                            
                            Bitmap *bitmap = reinterpret_cast<Bitmap *>(tag.data);

                            if(bitmap->sequences_count > 0) {
                                bitmap->sequences = TRANSLATE_ADDRESS(bitmap->sequences);
                                for(std::size_t j = 0; j < bitmap->sequences_count; j++) {
                                    auto *sequence = bitmap->sequences + j;
                                    if(sequence->sprites_count > 0) {
                                        sequence->sprites = TRANSLATE_ADDRESS(sequence->sprites);
                                    }
                                }
                            }

                            if(bitmap->bitmaps_count > 0) {
                                bitmap->bitmaps = TRANSLATE_ADDRESS(bitmap->bitmaps);
                                for(std::size_t j = 0; j < bitmap->bitmaps_count; j++) {
                                    bitmap->bitmaps[j].pixel_data += map->loaded_size; 
                                }
                            }
                        }

                        m.fixed_tags = true;
                    }

                    tag_data_header.tag_array = tag_array.data();
                }
                else {
                    std::memcpy(output, *map->memory_location + file_offset, size);
                }
                return 1;
            }
        }

        return 0;
    }

    std::filesystem::path MapEntry::get_file_path() {
        auto p1 = get_map_path() / (this->name + ".map");
        if(std::filesystem::exists(p1)) {
            return p1;
        }
        else {
            return get_download_map_path() / (this->name + ".map");
        }
    }

    LoadedMap *get_loaded_map(const char *name) noexcept {
        // Make a lowercase version
        char map_name_lowercase[32];
        std::strncpy(map_name_lowercase, name, sizeof(map_name_lowercase) - 1);
        for(auto &i : map_name_lowercase) {
            i = std::tolower(i);
        }
        
        // Find it!
        for(auto &i : loaded_maps) {
            if(i.name == map_name_lowercase) {
                return &i;
            }
        }
        
        return nullptr;
    }

    LoadedMap *load_map(const char *map_name, bool secondary) {
        // Lowercase it
        char map_name_lowercase[32] = {};
        std::strncpy(map_name_lowercase, map_name, sizeof(map_name_lowercase) - 1);
        for(auto &i : map_name_lowercase) {
            i = std::tolower(i);
        }
        
        // Get the map path and modified time
        auto map_path = path_for_map_local(map_name_lowercase);
        std::error_code ec;
        auto timestamp = std::filesystem::last_write_time(map_path, ec);
        
        if(ec) {
            char error_message[256];
            std::snprintf(error_message, sizeof(error_message), "Unable to determine the modified time of %s.map.\n\nMake sure the map exists in your maps folder and try again.", map_name);
            MessageBoxA(nullptr, error_message, "Map error", MB_OK | MB_ICONERROR);
            std::exit(EXIT_FAILURE);
        }
        
        // Is the map already loaded?
        auto loaded_map_it = loaded_maps.begin();
        while(loaded_map_it != loaded_maps.end()) {
            auto &map = *loaded_map_it;
            if(map.name == map_name_lowercase && map.secondary == secondary) {
                if(map.timestamp == timestamp) {
                    return &map;
                }
                loaded_maps.erase(loaded_map_it, loaded_maps.end());
                break;
            }
            loaded_map_it++;
        }

        // Add our map to the list
        std::size_t size = std::filesystem::file_size(map_path);
        LoadedMap new_map;
        new_map.name = map_name_lowercase;
        new_map.timestamp = timestamp;
        new_map.file_size = size;
        new_map.decompressed_size = size;
        new_map.path = map_path;
        new_map.secondary = secondary;
        
        // Load it
        std::FILE *f = nullptr;
        auto invalid = [&f, &map_name_lowercase](const char *error) {
            // Close first
            if(f) {
                std::fclose(f);
            }
            
            char error_dialog[256];
            std::snprintf(error_dialog, sizeof(error_dialog), "Failed to load %s due to an error\n\n%s", map_name_lowercase, error);
            show_error_box("Map error", error_dialog);
            std::exit(1);
        };
        
        // Attempt to load directly into memory
        f = std::fopen(map_path.string().c_str(), "rb");
        if(!f) {
            invalid("Map could not be opened");
        }
        
        // Load the thing
        union {
            MapHeaderDemo demo_header;
            MapHeader fv_header;
        } header;
        
        if(std::fread(&header, sizeof(header), 1, f) != 1) {
            invalid("Failed to read map header into memory from the file");
        }
        
        bool needs_decompressed = false;
        
        if(get_engine_type() == ENGINE_TYPE_DEMO && header.demo_header.is_valid()) {
            switch(header.demo_header.engine_type) {
                case CacheFileEngine::CACHE_FILE_DEMO:
                    break;
                case CacheFileEngine::CACHE_FILE_DEMO_COMPRESSED:
                    size = header.demo_header.file_size;
                    needs_decompressed = true;
                    break;
                default:
                    invalid("Invalid map type");
            }
        }
        else if(header.fv_header.is_valid()) {
            switch(header.fv_header.engine_type) {
                case CacheFileEngine::CACHE_FILE_RETAIL:
                case CacheFileEngine::CACHE_FILE_CUSTOM_EDITION:
                    break;
                    
                case CacheFileEngine::CACHE_FILE_RETAIL_COMPRESSED:
                case CacheFileEngine::CACHE_FILE_CUSTOM_EDITION_COMPRESSED:
                case CacheFileEngine::CACHE_FILE_DEMO_COMPRESSED:
                    size = header.fv_header.file_size;
                    needs_decompressed = true;
                    break;
                    
                default:
                    invalid("Invalid map type");
            }
        }
        else {
            invalid("Header is invalid");
        }
        
        std::size_t remaining_buffer_size = total_buffer_size;
        auto *buffer_location = map_memory_buffer;
        for(auto &i : loaded_maps) {
            remaining_buffer_size -= i.loaded_size;
            buffer_location += i.loaded_size;
        }
        
        // Check if we have enough space
        if(remaining_buffer_size < size) {
            invalid("Not enough memory to load map.");
        }

        // We do!
        if(needs_decompressed) {
            std::size_t actual_size;
            try {
                actual_size = decompress_map_file(map_path.string().c_str(), buffer_location, size);
            }
            catch (std::exception &) {
                invalid("Failed to read map");
            }
            if(actual_size != size) {
                invalid("Size in map is incorrect");
            }
            new_map.decompressed_size = actual_size;
        }
        else {
            std::fseek(f, 0, SEEK_SET);
            if(std::fread(buffer_location, size, 1, f) != 1) {
                invalid("Failed to read map");
            }
        }
        
        // We're done with this
        std::fclose(f);
        f = nullptr;
        
        // Find all metadata after the thing we're loading to
        std::size_t metadata_size = metadata.size();
        for(std::size_t m = 0; m < metadata_size; m++) {
            auto &md = metadata[m];
            if(md.data >= buffer_location) {
                metadata.erase(metadata.begin() + m);
                m--;
                metadata_size--;
                continue;
            }
        }

        new_map.loaded_size = size;
        new_map.memory_location = buffer_location;
        new_map.buffer_size = remaining_buffer_size;
        
        // Calculate CRC32
        get_map_entry(new_map.name.c_str())->crc32 = ~calculate_crc32_of_map_file(&new_map);

        // Done!
        return &loaded_maps.emplace_back(new_map);
    }

    void set_up_map_loading() {
        auto &balltze = Balltze::get();
        auto &sig_manager = balltze.signature_manager();
        auto &chimera_ini = balltze.chimera_ini();

        auto is_enabled = [&chimera_ini](const char *what) -> bool {
            return chimera_ini.get_value_bool(what).value_or(false);
        };

        auto read_mib = [&chimera_ini](const char *what, std::size_t default_value) -> std::size_t {
            return chimera_ini.get_value_size(what).value_or(default_value) * 1024 * 1024;
        };

        if(is_enabled("memory.enable_map_memory_buffer") && current_exe_is_laa_patched()) {
            total_buffer_size = read_mib("memory.map_size", 1024);

            MEMORY_BASIC_INFORMATION mbi;

            // Find Chimera's map memory buffer
            for(auto *m = reinterpret_cast<std::byte *>(0x80000000); m < reinterpret_cast<std::byte *>(0xF0000000) && !map_memory_buffer; m += 0x10000000) {
                if(VirtualQuery(m, &mbi, sizeof(mbi))) {
                    if(mbi.RegionSize == total_buffer_size && mbi.State == MEM_COMMIT | MEM_RESERVE && mbi.Protect == PAGE_READWRITE) {
                        std::cout << "[Balltze] Found Chimera's map memory buffer at " << mbi.BaseAddress << std::endl;
                        map_memory_buffer = reinterpret_cast<std::byte *>(mbi.BaseAddress);
                    }
                }
            }

            if(!map_memory_buffer) {
                show_message_box("Failed to find Chimera's map memory buffer. This may be a bug.");
                std::exit(EXIT_FAILURE);
            }
        }

        // Hook loading {bitmaps,sounds,loc}.map
        auto engine = get_engine_type();

        // Hook loading normal maps
        static auto *load_map_path_sig = sig_manager.get("map_load_path");
        static Memory::Hook load_map_path_hook;
        Memory::remove_chimera_hook(load_map_path_sig->data(), false, true, true);
        load_map_path_hook.initialize(load_map_path_sig->data(), nullptr, reinterpret_cast<void *>(map_loading_asm));
        load_map_path_hook.hook();

        // Handle this
        static auto *read_cache_file_data_sig = sig_manager.get("read_map_file_data");
        static Memory::Hook read_cache_file_data_hook;
        Memory::remove_chimera_hook(read_cache_file_data_sig->data(), true, false, true);
        read_cache_file_data_hook.initialize(read_cache_file_data_sig->data(), reinterpret_cast<void *>(on_read_map_file_data_asm));
        read_cache_file_data_hook.hook();

        // Support Cutdown Edition maps
        if(engine == ENGINE_TYPE_RETAIL || engine == ENGINE_TYPE_CUSTOM_EDITION) {
            custom_edition_maps_supported = set_up_custom_edition_map_support();
        }
    }
}
