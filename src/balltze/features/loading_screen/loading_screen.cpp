// SPDX-License-Identifier: GPL-3.0-only

#include <filesystem>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <semaphore>
#include <atomic>
#include <d3d9.h>
#include <windows.h>
#include <gdiplus.h>
#include <balltze/command.hpp>
#include <balltze/hook.hpp>
#include <balltze/memory.hpp>
#include <balltze/event.hpp>
#include <balltze/engine/renderer.hpp>
#include <balltze/helpers/d3d9.hpp>
#include <balltze/helpers/resources.hpp>
#include "../../config/config.hpp"
#include "../../config/chimera_preferences.hpp"
#include "../../event/console_command.hpp"
#include "../../output/video.hpp"
#include "../../logger.hpp"
#include "../../resources.hpp"

using namespace std::chrono_literals;

namespace Balltze::Features {
    static constexpr auto loading_screen_fade_out_duration = 3000ms;
    static constexpr auto loading_screen_shader_effect_duration = 4000ms;
    static bool loading_screen_playback = false; 
    static bool loading_screen_demo = false;
    static std::optional<std::chrono::steady_clock::time_point> loading_screen_start_time;

    static HBITMAP loading_screen_background;
    static IDirect3DDevice9 *device = nullptr;
    static IDirect3DTexture9 *texture = nullptr;
    static IDirect3DPixelShader9 *pixel_shader = nullptr;
    static Sprite sprite;
    static std::uint8_t alpha_channel = 255;

    static std::atomic<bool> waiting_for_loading_screen_end = false;
    static std::binary_semaphore wait_for_background_end_semaphore{0};

    static bool loading_screen_is_blocked = false;

    static HRESULT load_loading_screen_background_texture(IDirect3DDevice9 *device, IDirect3DTexture9 **texture) {
        return load_texture_from_resource(MAKEINTRESOURCEW(ID_LOADING_SCREEN_BACKGROUND), get_current_module(), device, texture);
    }

    static HRESULT load_loading_screen_shader(IDirect3DDevice9 *device, IDirect3DPixelShader9 **shader) {
        auto shader_data = load_resource_data(get_current_module(), MAKEINTRESOURCEW(ID_LOADING_SCREEN_SHADER), L"CSO");
        if(!shader_data) {
            return E_FAIL;
        }
        HRESULT hr = device->CreatePixelShader(reinterpret_cast<const DWORD *>(shader_data->data()), shader);
        return hr;
    }

    std::chrono::milliseconds loading_screen_background_actual_duration() {
        float duration = static_cast<float>(loading_screen_shader_effect_duration.count());
        return std::chrono::milliseconds(static_cast<std::int64_t>((duration * 1.5f) / (duration - 250) * duration));
    }

    static void end_loading_screen_background() {
        if(!loading_screen_playback) {
            return;
        }
        loading_screen_playback = false;
        loading_screen_demo = false;
        loading_screen_start_time = std::nullopt;
    }

    static void play_loading_screen_background() {
        if(loading_screen_is_blocked) {
            return;
        }
        alpha_channel = 255;
        waiting_for_loading_screen_end = false;
        loading_screen_playback = true;
    }

    static void draw_loading_screen_background() {
        if(!device || !loading_screen_playback) {
            return;
        }

        if(!loading_screen_start_time) {
            loading_screen_start_time = std::chrono::steady_clock::now();
        }

        // Get current render target dimensions
        IDirect3DSurface9 *surface;
        device->GetRenderTarget(0, &surface);
        D3DSURFACE_DESC desc;
        surface->GetDesc(&desc);

        // Set up constants for pixel shader
        #define FLOAT_CAST(x) static_cast<float>(x)
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - *loading_screen_start_time);
        float c_resolution[2] = {FLOAT_CAST(desc.Width), FLOAT_CAST(desc.Height)};
        float c_duration = FLOAT_CAST(loading_screen_shader_effect_duration.count())  / 1000.0f;
        float c_elapsed = FLOAT_CAST(ms.count()) / 1000.0f;
        float c_opacity = FLOAT_CAST(alpha_channel) / 255.0f;
        #undef FLOAT_CAST

        sprite.begin();
        device->SetPixelShader(pixel_shader);
        device->SetPixelShaderConstantF(0, c_resolution, 1);
        device->SetPixelShaderConstantF(1, &c_duration, 1);
        device->SetPixelShaderConstantF(2, &c_elapsed, 1);
        device->SetPixelShaderConstantF(3, &c_opacity, 1);
        sprite.draw(0, 0, desc.Width, desc.Height);
        sprite.end();

        if(waiting_for_loading_screen_end) {
            auto duration = loading_screen_background_actual_duration();
            auto time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - *loading_screen_start_time);
            if(duration - time_elapsed <= loading_screen_fade_out_duration) {
                waiting_for_loading_screen_end = false;
                wait_for_background_end_semaphore.release();
            }
        }

        if(loading_screen_demo) {
            auto time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - *loading_screen_start_time);
            if(time_elapsed > loading_screen_background_actual_duration()) { 
                end_loading_screen_background();
            }
        }
    }

    static void update_d3d9_device(Event::D3D9EndSceneEvent &event) {
        if(!device) {
            device = event.args.device;
        }
        if(event.time == Event::EVENT_TIME_BEFORE && loading_screen_demo) {
            draw_loading_screen_background();
        }

        if(!texture) {
            if(load_loading_screen_background_texture(device, &texture) == S_OK) {
                sprite.update_texture(texture);
            }
            else {
                logger.fatal("Failed to load loading screen background texture.");
                std::exit(EXIT_FAILURE);
            }
        }

        if(!pixel_shader) {
            if(load_loading_screen_shader(device, &pixel_shader) != S_OK) {
                logger.fatal("Failed to load loading screen shader.");
                std::exit(EXIT_FAILURE);
            }
        }
    }

    static void on_device_reset(Event::D3D9DeviceResetEvent &event) {
        if(texture) {
            texture->Release();
            texture = nullptr;
            pixel_shader->Release();
            pixel_shader = nullptr;
        }
        device = nullptr;
    }

    static std::atomic<bool> map_load_thread_done = false;

    static void draw_loading_screen() {
        static auto loading_screen_render_function_sig = Memory::get_signature("loading_screen_render_function");
        static auto draw_func = reinterpret_cast<void(*)()>(loading_screen_render_function_sig->data());
        draw_func();
    }

    static void wait_for_background_end() {
        if(loading_screen_playback) {
            waiting_for_loading_screen_end = true;
            wait_for_background_end_semaphore.acquire();
        }
    }

    extern "C" {
        void *load_map_function_address = nullptr;
        std::uint32_t load_map_function_result = false;
        void load_map_worker_asm(const char *);
        void load_map_override_asm();
        void set_video_mode_asm();
        void *set_video_mode_return = nullptr;

        void set_map_load_thread_done_flag() {
            wait_for_background_end();
            map_load_thread_done = true;
        }
    
        std::uint32_t load_map_override(const char *map_to_load) {
            // Load map in a separate thread so we can render the loading screen
            std::thread map_load_thread(load_map_worker_asm, map_to_load);

            if(device && std::strcmp(map_to_load, "levels\\ui\\ui") != 0) {
                if(!loading_screen_playback) {
                    play_loading_screen_background();
                }

                map_load_thread.detach();

                // Continue rendering loading screen until map_load_thread is done
                while(!map_load_thread_done) {
                    MSG message;
                    if(PeekMessageA(&message, NULL, 0, 0, PM_REMOVE)) {
                        // Forward window messages so Windows doesn't think the game is frozen 
                        TranslateMessage(&message);
                        DispatchMessageA(&message);
                    }
                    else if(!loading_screen_is_blocked) {
                        auto hr = device->TestCooperativeLevel();
                        if(hr == D3D_OK) {
                            device->BeginScene();
                            draw_loading_screen();
                            device->EndScene();
                            device->Present(NULL, NULL, NULL, NULL);
                        }
                        else if(hr == D3DERR_DEVICENOTRESET) {
                            texture->Release();
                            texture = nullptr;
                            pixel_shader->Release();
                            pixel_shader = nullptr;
                            device = nullptr;
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
    }

    std::uint32_t *game_loading_screen_flag = nullptr;

    static void loading_screen_hook() {
        bool are_we_in_loading_screen = *game_loading_screen_flag != 0;
        if(!loading_screen_playback && are_we_in_loading_screen) {
            play_loading_screen_background();
        }
        else if(!loading_screen_demo && loading_screen_playback && !are_we_in_loading_screen) {
            end_loading_screen_background();
        }
        draw_loading_screen_background();
    }

    static bool loading_screen_render_hook(std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t, Engine::ColorARGBInt color_mask) {
        alpha_channel = color_mask.alpha;
        return true;
    }

    static void chimera_block_loading_screen_listener(Event::ConsoleCommandEvent const &event) {
        if(event.time == Event::EVENT_TIME_BEFORE) {
            return;
        }

        auto args = split_arguments(event.args.command);
        auto command = args[0];
        args.erase(args.begin());

        if(command == "chimera_block_loading_screen") {
            if(!args.empty()) {
                loading_screen_is_blocked = STR_TO_BOOL(args.front().c_str());
                logger.debug("chimera_block_loading_screen is now set to {}", loading_screen_is_blocked ? "true" : "false");
                if(loading_screen_is_blocked) {
                    logger.mute_ingame(false);
                    logger.warning("Loading screen is now disabled.");
                    logger.mute_ingame(true);
                }
            }
        }
    }

    void set_up_loading_screen() {
        Event::D3D9EndSceneEvent::subscribe(update_d3d9_device, Event::EVENT_PRIORITY_HIGHEST);
        Event::D3D9DeviceResetEvent::subscribe(on_device_reset, Event::EVENT_PRIORITY_HIGHEST);

        auto set_video_mode_sig = Memory::get_signature("set_video_mode");
        if(!set_video_mode_sig) {
            logger.error("Failed to find signatures for loading screen.");
            return;
        }

        auto *set_video_mode_address = Memory::follow_32bit_jump(set_video_mode_sig->data()) + 12;
        Memory::override_function(set_video_mode_address, set_video_mode_asm, set_video_mode_return);
        set_video_mode_return = set_video_mode_sig->data() + 5;

        auto load_map_function_sig = Memory::get_signature("load_map_function");
        auto loading_screen_handler_sig = Memory::get_signature("loading_screen_handler");
        auto loading_screen_render_function_sig = Memory::get_signature("loading_screen_render_function");
        auto loading_screen_render_function_call_sig = Memory::get_signature("loading_screen_background_render_call");
        if(!load_map_function_sig || !loading_screen_handler_sig || !loading_screen_render_function_sig || !loading_screen_render_function_call_sig) {
            logger.error("Failed to find signatures for loading screen.");
            return;
        }
        game_loading_screen_flag = *reinterpret_cast<std::uint32_t **>(loading_screen_render_function_sig->data() + 1);
        Memory::override_function(load_map_function_sig->data(), load_map_override_asm, load_map_function_address);
        Memory::hook_function(loading_screen_handler_sig->data(), loading_screen_hook);
        Memory::hook_function(loading_screen_render_function_call_sig->data(), std::function<bool()>(reinterpret_cast<bool (*)()>(loading_screen_render_hook)));

        auto &preferences = Config::get_chimera_preferences();
        auto settings = preferences.get_settings_for_command("chimera_block_loading_screen");
        if(settings && !settings->empty()) {
            loading_screen_is_blocked = STR_TO_BOOL(settings->front().c_str());
        }
        Event::ConsoleCommandEvent::subscribe_const(chimera_block_loading_screen_listener);

        register_command("test_loading_screen", "debug", "Plays a random loading screen", std::nullopt, [](int arg_count, const char **args) -> bool {
            loading_screen_demo = true;
            play_loading_screen_background();
            return true;
        }, false, 0, 0);
    }
}
