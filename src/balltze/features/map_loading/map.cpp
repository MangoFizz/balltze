// SPDX-License-Identifier: GPL-3.0-only

#include <cstring>

#include <balltze/config.hpp>
#include <balltze/engine/map.hpp>
#include <balltze/engine/path.hpp>
#include <balltze/engine/version.hpp>
#include "map.hpp"

namespace Balltze::Features {
    using namespace Engine;

    static constexpr const char *tmp_format = "tmp_%zu.map";

    std::filesystem::path MapEntry::get_file_path() {
        auto p1 = get_map_path() / (this->name + ".map");
        if(std::filesystem::exists(p1)) {
            return p1;
        }
        else {
            return get_download_map_path() / (this->name + ".map");
        }
    }

    const std::filesystem::path get_map_path() noexcept {
        static auto chimera_ini = Config::Ini("chimera.ini");
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

    const std::filesystem::path get_download_map_path() noexcept {
        static auto chimera_ini = Config::Ini("chimera.ini");
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

    std::filesystem::path path_for_map_local(const char *map_name) {
        return get_map_entry(map_name)->get_file_path();
    }

    std::filesystem::path path_for_tmp(std::size_t tmp) noexcept {
        char tmp_name[64];
        std::snprintf(tmp_name, sizeof(tmp_name), tmp_format, tmp);
        return Engine::get_path() / "chimera" / "tmp" / tmp_name;
    }

    bool same_string_case_insensitive(const char *a, const char *b) noexcept {
        if(a == b) return true;
        while(std::tolower(*a) == std::tolower(*b)) {
            if(*a == 0) return true;
            a++;
            b++;
        }
        return false;
    }
    
    std::optional<MapEntry> get_map_entry(const char *map_name) {
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
}
