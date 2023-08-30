// SPDX-License-Identifier: GPL-3.0-only

#include <string>
#include <optional>
#include <cstring>
#include <shlobj.h>
#include <balltze/memory.hpp>
#include <balltze/config.hpp>
#include <balltze/engine/core.hpp>
#include "../config/config.hpp"
#include "../logger.hpp"

namespace Balltze::Engine {
    extern "C" {
        void console_print_asm(const ColorARGB &color, const char *message);
    }

    void console_print(std::string message, ColorARGB color) noexcept {
        console_print_asm(color, message.c_str());
    }

    std::filesystem::path get_path() {
        static std::optional<std::filesystem::path> path;
        if(!path) {
            auto chimera_init = Config::get_chimera_ini();
            auto ini_path = chimera_init.get_value_string("halo.path");
            if(ini_path) {
                path = *ini_path;
            }
            else {
                auto cmd = GetCommandLineW();
                int argc;
                auto argv = CommandLineToArgvW(cmd, &argc);
                bool found = false;
                for(int i = 0; i < argc; i++) {
                    auto arg = argv[i];
                    if(std::wcscmp(arg, L"-path") == 0) {
                        path = argv[i + 1];
                        found = true;
                        break;
                    }
                }

                if(!found) {
                    char documents_folder_path[MAX_PATH];
                    HRESULT res = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, documents_folder_path);
                    if(res == S_OK) {
                        path = std::filesystem::path(documents_folder_path) / "My Games" / "Halo CE";
                        return path.value();
                    }
                    else {
                        logger.debug("Could not get documents folder path");
                        throw std::runtime_error("Could not get documents folder path");
                    }
                }
            }
        }
        return path.value();
    }

    Resolution &get_resolution() noexcept {
        static auto *resolution_sig = Memory::get_signature("resolution");
        static Resolution *resolution = *reinterpret_cast<Resolution **>(resolution_sig->data());
        return *resolution;
    }

    std::size_t get_tick_count() noexcept {
        static std::int32_t *tick_count = nullptr;
        static auto *tick_count_sig = Memory::get_signature("tick_counter");
        if(!tick_count) {
            tick_count = reinterpret_cast<std::int32_t *>(**reinterpret_cast<std::byte ***>(tick_count_sig->data()) + 0xC);
        }
        return *tick_count;
    }

    WindowGlobals *get_window_globals() {
        static std::optional<WindowGlobals *> window_globals;
        if(!window_globals.has_value()) {
            auto *window_globals_sig = Memory::get_signature("window_globals");
            if(!window_globals_sig) {
                throw std::runtime_error("window_globals signature not found");
            }
            window_globals = *reinterpret_cast<WindowGlobals **>(window_globals_sig->data());
        }
        return *window_globals;
    }

    EngineEdition get_engine_edition() {
        static std::optional<EngineEdition> engine_type;
        if(!engine_type.has_value()) {
            auto *engine_edition_sig = Memory::get_signature("engine_type");
            if(!engine_edition_sig) {
                throw std::runtime_error("engine_edition signature not found");
            }
            std::string engine_edition_str = *reinterpret_cast<const char **>(engine_edition_sig->data());
            if(engine_edition_str == "halom") {
                engine_type = ENGINE_TYPE_CUSTOM_EDITION;
            }
            else if(engine_edition_str == "halor") {
                engine_type = ENGINE_TYPE_RETAIL;
            }
            else if(engine_edition_str == "halod") {
                engine_type = ENGINE_TYPE_DEMO;
            }
            else {
                throw std::runtime_error("Unknown engine edition " + engine_edition_str);
            }
        }
        return *engine_type;
    }

    CameraType get_camera_type() noexcept {
        static auto *cta = reinterpret_cast<CameraType *>(*reinterpret_cast<std::byte **>(Memory::get_signature("camera_type")->data()) + 0x56);
        return *cta;
    }

    CameraData &get_camera_data() noexcept {
        static std::optional<CameraData *> camera_coord_addr;
        if(!camera_coord_addr.has_value()) {
            camera_coord_addr = reinterpret_cast<CameraData *>(*reinterpret_cast<std::byte **>(Memory::get_signature("camera_coord")->data()) - 0x8);
        }
        return **camera_coord_addr;
    }

    std::uint8_t get_master_volume() noexcept {
        static auto *master_volume_sig = Memory::get_signature("master_volume");
        static auto *master_volume = *reinterpret_cast<std::uint8_t **>(master_volume_sig->data()) + 0xB78;
        return *master_volume;
    }
}
