// SPDX-License-Identifier: GPL-3.0-only

#include <filesystem>
#include <fstream>
#include <balltze/hook.hpp>
#include <balltze/memory.hpp>

namespace Balltze {
    extern "C" {
        void play_bik_video_hook();
        bool override_bik_video_res = false;
        std::uint32_t bik_video_width = 0;
        std::uint32_t bik_video_height = 0;
    }

    static Memory::Hook *play_bik_video_res_override_hook = nullptr;

    static std::pair<std::uint32_t, std::uint32_t> get_bik_video_resolution(std::filesystem::path const &path) {
        try {
            std::ifstream file(path, std::ios::binary);
            if(!file.is_open()) {
                return {0, 0};
            }
            file.seekg(20);
            std::uint32_t width = 0;
            std::uint32_t height = 0;
            file.read(reinterpret_cast<char *>(&width), sizeof(width));
            file.read(reinterpret_cast<char *>(&height), sizeof(height));
            return {width, height};
        }
        catch(...) {
            throw std::runtime_error("Failed to get BIK video resolution");
        }
    }

    void play_bik_video(std::filesystem::path const &path) {
        auto *play_bik_video_sig = Memory::get_signature("play_bik_video_function"); 
        auto *play_bik_video = reinterpret_cast<void (*)(const char *)>(play_bik_video_sig->data());
        std::uint32_t width = 0;
        std::uint32_t height = 0;
        try {
            std::tie(width, height) = get_bik_video_resolution(path);
        }
        catch(...) {
            throw;
        }
        override_bik_video_res = true;
        bik_video_width = width;
        bik_video_height = height;
        if(!play_bik_video_res_override_hook) {
            auto *play_bik_video_resolution_set_sig = Memory::get_signature("play_bik_video_resolution_set");
            play_bik_video_res_override_hook = Memory::hook_function(play_bik_video_resolution_set_sig->data(), play_bik_video_hook, std::nullopt, false);
        }
        play_bik_video(path.string().c_str());
    }

    extern "C" {
        FARPROC bik_open_addr = nullptr;
        FARPROC bik_pause_addr = nullptr;
        FARPROC bik_next_frame_addr = nullptr;
        FARPROC bik_do_frame_addr = nullptr;
        FARPROC bik_copy_to_buffer_addr = nullptr;
        FARPROC bik_wait_addr = nullptr;
        FARPROC bik_close_addr = nullptr;
        FARPROC bik_go_to_addr = nullptr;
        FARPROC bik_set_volume_addr = nullptr;
    }

    void load_bik_functions() {
        auto lib = LoadLibraryA("binkw32.dll");
        bik_open_addr = GetProcAddress(lib, MAKEINTRESOURCE(36));
        bik_pause_addr = GetProcAddress(lib, MAKEINTRESOURCE(41));
        bik_next_frame_addr = GetProcAddress(lib, MAKEINTRESOURCE(35));
        bik_do_frame_addr = GetProcAddress(lib, MAKEINTRESOURCE(22));
        bik_copy_to_buffer_addr = GetProcAddress(lib, MAKEINTRESOURCE(18));
        bik_wait_addr = GetProcAddress(lib, MAKEINTRESOURCE(57));
        bik_close_addr = GetProcAddress(lib, MAKEINTRESOURCE(16));
        bik_go_to_addr = GetProcAddress(lib, MAKEINTRESOURCE(32));
        bik_set_volume_addr = GetProcAddress(lib, MAKEINTRESOURCE(56));
    }
}
