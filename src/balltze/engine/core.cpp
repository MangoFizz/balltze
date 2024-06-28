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
        return std::filesystem::absolute(path.value());
    }

    std::size_t get_tick_count() noexcept {
        static std::int32_t *tick_count = nullptr;
        static auto *tick_count_sig = Memory::get_signature("tick_counter");
        if(!tick_count) {
            tick_count = reinterpret_cast<std::int32_t *>(**reinterpret_cast<std::byte ***>(tick_count_sig->data()) + 0xC);
        }
        return *tick_count;
    }

    float get_tick_rate() noexcept {
        static float *tick_ptr = nullptr;
        static auto *tick_rate_sig = Memory::get_signature("tick_rate");
        if(tick_rate_sig) {
            tick_ptr = *reinterpret_cast<float **>(tick_rate_sig->data());
        }
        return *tick_ptr;
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
}
