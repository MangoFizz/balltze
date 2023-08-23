// SPDX-License-Identifier: GPL-3.0-only

#include <filesystem>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <semaphore>
#include <atomic>
#include <d3d9.h>
#include <windows.h>
#include <balltze/command.hpp>
#include <balltze/hook.hpp>
#include <balltze/memory.hpp>
#include <balltze/event.hpp>
#include "../config/config.hpp"
#include "../output/sprite.hpp"
#include "../output/video.hpp"
#include "../logger.hpp"

using namespace std::chrono_literals;

namespace Balltze::Features {
    static constexpr auto loading_screen_fade_out_duration = 2000ms;
    static bool loading_screen_playback = false; 
    static bool loading_screen_demo = false;
    static Bik *bik = nullptr;
    static IDirect3DDevice9 *device = nullptr;
    static IDirect3DTexture9 *texture = nullptr;
    static Sprite sprite;
    static std::uint8_t alpha_channel = 255;
    static D3DLOCKED_RECT locked_rect;
    static std::atomic<bool> map_load_thread_waiting_for_video = false;
    static std::binary_semaphore wait_for_video_end_semaphore{0};
    static void(*draw_loading_screen)() = nullptr;

    static std::filesystem::path get_loading_screen_videos_path() {
        auto path = Config::get_balltze_directory() / "loading_screens";
        std::filesystem::create_directory(path);
        return path;
    }

    static std::filesystem::path choose_random_loading_screen_video() {
        auto path = get_loading_screen_videos_path();
        std::vector<std::filesystem::path> files;
        for(auto &file : std::filesystem::directory_iterator(path)) {
            if(file.is_regular_file()) {
                files.push_back(file.path());
            }
        }

        if(files.size() == 0) {
            return std::filesystem::path();
        }

        std::srand(std::time(nullptr));
        auto index = std::rand() % files.size();
        return files[index];
    }

    static void end_loading_screen_video() {
        if(bik) {
            bik_close(bik);
            bik = nullptr;
        }
        if(texture) {
            texture->Release();
            texture = nullptr;
        }
        loading_screen_playback = false;
        loading_screen_demo = false;
    }

    static void play_loading_screen_video() {
        auto path = choose_random_loading_screen_video();
        if(path.empty()) {
            logger.warning("No loading screens found");
            return;
        }

        if(bik) {
            end_loading_screen_video();
        }

        alpha_channel = 255;

        bik = bik_open(path.string().c_str(), 0);
        if(!bik) {
            logger.error("Failed to open bik file");
            return;
        }

        bik_set_volume(bik, 0, 0);
        map_load_thread_waiting_for_video = false;
        loading_screen_playback = true;
    }

    static void set_texture_alpha_channel(std::uint8_t alpha) {
        for(int y = 0; y < bik->height; ++y) {
            for(int x = 0; x < bik->width; ++x) {
                int pixelIndex = y * locked_rect.Pitch + x * 4;
                reinterpret_cast<std::uint8_t *>(locked_rect.pBits)[pixelIndex + 3] = alpha;
            }
        }
    }

    static void draw_loading_screen_video() {
        if(!device || !loading_screen_playback) {
            return;
        }

        if(!texture) {
            ASSERT_D3D(device->CreateTexture(bik->width, bik->height, 0, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &texture, NULL), "Failed to create texture");
            sprite.update_texture(texture);
        }

        if(bik) {
            auto res = bik_wait(bik);
            if(res == 0) {
                bik_do_frame(bik);
                bik_next_frame(bik);
            }

            // Copy video frame to texture
            ASSERT_D3D(texture->LockRect(0, &locked_rect, NULL, D3DLOCK_DISCARD), "Failed to lock texture");
            bik_copy_to_buffer(bik, locked_rect.pBits, locked_rect.Pitch, bik->height, 0, 0, BINKCOPYALL | BIKSURFACE32);
            set_texture_alpha_channel(alpha_channel);
            ASSERT_D3D(texture->UnlockRect(0), "Failed to unlock texture");

            // Get current render target dimensions
            IDirect3DSurface9 *surface;
            device->GetRenderTarget(0, &surface);
            D3DSURFACE_DESC desc;
            surface->GetDesc(&desc);

            // Draw video frame
            sprite.begin();
            sprite.draw(0, 0, desc.Width, desc.Height);
            sprite.end();

            if(map_load_thread_waiting_for_video) {
                if(bik->time_left() <= loading_screen_fade_out_duration) {
                    map_load_thread_waiting_for_video = false;
                    wait_for_video_end_semaphore.release();
                }
            }

            if(bik->current_frame >= bik->frames_count) {
                if(!loading_screen_demo) {
                    bik_go_to(bik, 1, 0);
                }
                else {
                    end_loading_screen_video();
                }
            }
        }
    }

    static void update_d3d9_device(Event::D3D9EndSceneEvent &event) {
        if(!device) {
            device = event.args.device;
        }
        if(event.time == Event::EVENT_TIME_BEFORE && loading_screen_demo) {
            draw_loading_screen_video();
        }
    }

    static void on_device_reset(Event::D3D9DeviceResetEvent &event) {
        if(texture) {
            texture->Release();
            texture = nullptr;
        }
        device = nullptr;
    }

    static std::string map_to_load;
    static std::atomic<bool> map_load_thread_done = false;

    static void update_map_name(Event::MapLoadEvent &event) {
        if(event.time == Event::EVENT_TIME_BEFORE) {
            map_to_load = event.args.name;
        }
    }

    extern "C" {
        void *load_map_function_address = nullptr;
        std::uint32_t load_map_function_result = false;
        void load_map_override_asm(const char *);
        void set_video_mode_asm();
        void *set_video_mode_return = nullptr;

        void set_map_load_thread_done_flag() {
            if(loading_screen_playback) {
                map_load_thread_waiting_for_video = true;
                wait_for_video_end_semaphore.acquire();
            }
            map_load_thread_done = true;
        }
    }

    static std::uint32_t load_map_override() {
        // Load map in a separate thread so we can render the loading screen
        std::thread map_load_thread(load_map_override_asm, map_to_load.c_str());

        if(loading_screen_playback) {
            map_load_thread.detach();

            // Continue rendering loading screen until load_map thread is done
            while(!map_load_thread_done) {
                MSG message;
                if(PeekMessageA(&message, NULL, 0, 0, PM_REMOVE)) {
                    // Forward window messages so Windows doesn't think the game is frozen 
                    TranslateMessage(&message);
                    DispatchMessageA(&message);
                }
                else {
                    auto hr = device->TestCooperativeLevel();
                    if(hr == D3DERR_DEVICENOTRESET) {
                        texture->Release();
                        texture = nullptr;
                        device = nullptr;
                    }
                    else if(hr == D3D_OK) {
                        device->BeginScene();
                        draw_loading_screen();
                        device->EndScene();
                        device->Present(NULL, NULL, NULL, NULL);
                    }
                }
            }
        }
        else {
            map_load_thread.join();
        }

        // Reset thread exit flag
        map_load_thread_done = false;

        return load_map_function_result;
    }

    std::uint32_t *game_loading_screen_flag = nullptr;

    static void loading_screen_hook() {
        bool are_we_in_loading_screen = *game_loading_screen_flag != 0;
        if(!loading_screen_playback && are_we_in_loading_screen) {
            play_loading_screen_video();
        }
        else if(!loading_screen_demo && loading_screen_playback && !are_we_in_loading_screen) {
            end_loading_screen_video();
        }
        draw_loading_screen_video();
    }

    static bool loading_screen_render_hook(std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t, Engine::ColorARGBInt color_mask) {
        if(*game_loading_screen_flag == 8) {
            alpha_channel = color_mask.alpha;
        }
        return true;
    }

    static Event::EventListenerHandle<Event::TickEvent> tick_event_listener_handle;

    void set_up_loading_screen() {
        Event::D3D9EndSceneEvent::subscribe(update_d3d9_device, Event::EVENT_PRIORITY_HIGHEST);
        Event::D3D9DeviceResetEvent::subscribe(on_device_reset, Event::EVENT_PRIORITY_HIGHEST);
        Event::MapLoadEvent::subscribe(update_map_name);

        load_bik_functions();

        auto set_video_mode_sig = Memory::get_signature("set_video_mode");
        if(!set_video_mode_sig) {
            logger.error("Failed to find signatures for loading screen.");
            return;
        }

        auto *set_video_mode_address = Memory::follow_32bit_jump(set_video_mode_sig->data()) + 12;
        Memory::override_function(set_video_mode_address, set_video_mode_asm, set_video_mode_return);
        set_video_mode_return = set_video_mode_sig->data() + 5;

        tick_event_listener_handle = Event::TickEvent::subscribe([](Event::TickEvent &) {
            auto load_map_function_sig = Memory::get_signature("load_map_function");
            auto loading_screen_handler_sig = Memory::get_signature("loading_screen_handler");
            auto loading_screen_render_function_sig = Memory::get_signature("loading_screen_render_function");
            auto loading_screen_render_function_call_sig = Memory::get_signature("loading_screen_background_render_call");
            if(!load_map_function_sig || !loading_screen_handler_sig || !loading_screen_render_function_sig || !loading_screen_render_function_call_sig) {
                logger.error("Failed to find signatures for loading screen.");
                return;
            }
            game_loading_screen_flag = *reinterpret_cast<std::uint32_t **>(loading_screen_render_function_sig->data() + 1);
            draw_loading_screen = reinterpret_cast<void(*)()>(loading_screen_render_function_sig->data());
            std::function<void()> load_map_hook_callback = reinterpret_cast<void(*)()>(load_map_override);
            Memory::override_function(load_map_function_sig->data(), load_map_hook_callback, load_map_function_address);
            Memory::hook_function(loading_screen_handler_sig->data(), loading_screen_hook);
            Memory::hook_function(loading_screen_render_function_call_sig->data(), std::function<bool()>(reinterpret_cast<bool (*)()>(loading_screen_render_hook)));

            register_command("test_loading_screen", "debug", "Plays a random loading screen", std::nullopt, [](int arg_count, const char **args) -> bool {
                loading_screen_demo = true;
                play_loading_screen_video();
                return true;
            }, false, 0, 0);

            tick_event_listener_handle.remove();
        });
    }
}
