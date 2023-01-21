// SPDX-License-Identifier: GPL-3.0-only

#include <string>
#include <optional>
#include <cstring>
#include <balltze/memory.hpp>
#include <balltze/engine/core.hpp>

namespace Balltze::Engine {
    extern "C" {
        void console_print_asm(const ColorARGB &color, const char *message);
    }

    void console_print(std::string message, ColorARGB color) noexcept {
        console_print_asm(color, message.c_str());
    }

    std::filesystem::path get_path() noexcept {
        static auto *halo_path_sig = Memory::get_signature("halo_path");
        static const char *path = nullptr;
        if(!path) {
            path = *reinterpret_cast<const char **>(halo_path_sig->data());
        }
        return std::filesystem::path(path);
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
}
