// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__OUTPUT__VIDEO_HPP
#define BALLTZE__OUTPUT__VIDEO_HPP

#include <string>
#include <chrono>
#include <memory>

namespace Balltze {
    #define BIKSURFACE32 3
    #define BINKCOPYALL 0x80000000L

    struct Bik {
        std::uint32_t width;
        std::uint32_t height;
        std::uint32_t frames_count;
        std::uint32_t current_frame;
        std::uint32_t last_frame_decompressed;
        std::uint32_t framerate;
        std::uint32_t framerate_div;
        PADDING(0xC0);
        std::int32_t is_paused;

        auto duration() {
            return std::chrono::milliseconds(static_cast<std::size_t>(static_cast<float>(this->frames_count) / (static_cast<float>(this->framerate) / this->framerate_div)) * 1000);
        }

        auto time_left() {
            return this->duration() - std::chrono::milliseconds(static_cast<std::size_t>(static_cast<float>(this->current_frame) / (static_cast<float>(this->framerate) / this->framerate_div)) * 1000);
        }
    };

    extern "C" {
        Bik *bik_open(const char *, std::uint32_t);
        std::int32_t bik_pause(Bik *, std::int32_t);
        void bik_next_frame(Bik *);
        std::int32_t bik_do_frame(Bik *);
        std::uint32_t bik_copy_to_buffer(Bik *, void *, std::int32_t, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t);
        std::int32_t bik_wait(Bik *);
        void bik_close(Bik *);
        void bik_go_to(Bik *, std::uint32_t, std::uint32_t);
        std::int32_t bik_set_volume(Bik *, std::uint32_t, std::uint32_t);
    }

    void load_bik_functions();
}

#endif
