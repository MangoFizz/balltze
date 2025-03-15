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
#include <balltze/engine/rasterizer.hpp>
#include <balltze/helpers/d3d9.hpp>
#include <balltze/helpers/resources.hpp>
#include <impl/rasterizer/rasterizer_dx9_render_target.h>
#include <impl/interface/loading_screen.h>
#include "../../config/config.hpp"
#include "../../config/chimera_preferences.hpp"
#include "../../event/console_command.hpp"
#include "../../output/video.hpp"
#include "../../logger.hpp"
#include "../../resources.hpp"

using namespace std::chrono_literals;

namespace Balltze::Features {
    static constexpr auto loading_screen_shader_effect_duration = 4000ms;
    static bool loading_screen_playback = false; 
    static std::optional<std::chrono::steady_clock::time_point> loading_screen_start_time;

    static HBITMAP loading_screen_background;
    static IDirect3DDevice9 *device = nullptr;
    static IDirect3DTexture9 *texture = nullptr;
    static IDirect3DPixelShader9 *pixel_shader = nullptr;
    static Sprite sprite;
    static float last_alpha_value = 0;

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

    static void draw_loading_screen_background(float alpha) {
        if(!device || !texture) {
            return;
        }
        
        // Start playback when alpha changes from 0 to non-zero
        if(last_alpha_value == 0.0f && alpha != 0.0f) {
            loading_screen_playback = true;
            loading_screen_start_time = std::chrono::steady_clock::now();
        }

        if(!loading_screen_playback) {
            return;
        }
        
        // Stop playback when alpha is 0
        if(alpha == 0.0f) {
            loading_screen_playback = false;
            loading_screen_start_time = std::nullopt;
            return;
        }

        auto *render_target = ::rasterizer_dx9_render_target_get(0);

        // Set up constants for pixel shader
        #define FLOAT_CAST(x) static_cast<float>(x)
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - *loading_screen_start_time);
        float c_resolution[2] = {FLOAT_CAST(render_target->width), FLOAT_CAST(render_target->height)};
        float c_duration = FLOAT_CAST(loading_screen_shader_effect_duration.count())  / 1000.0f;
        float c_elapsed = FLOAT_CAST(ms.count()) / 1000.0f;
        float c_opacity = alpha;
        #undef FLOAT_CAST

        sprite.begin();
        device->SetPixelShader(pixel_shader);
        device->SetPixelShaderConstantF(0, c_resolution, 1);
        device->SetPixelShaderConstantF(1, &c_duration, 1);
        device->SetPixelShaderConstantF(2, &c_elapsed, 1);
        device->SetPixelShaderConstantF(3, &c_opacity, 1);
        sprite.draw(0, 0, render_target->width, render_target->height);
        sprite.end();

        last_alpha_value = alpha;
    }

    static void update_d3d9_device(Event::D3D9EndSceneEvent &event) {
        if(!device) {
            device = event.context.device;
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

    extern "C" {
        void *load_map_function_address = nullptr;
        std::uint32_t load_map_function_result = false;
        void load_map_worker_asm(const char *);
        void load_map_override_asm();
        void set_d3d9_device_multithreaded_flag();
        void *set_video_mode_return = nullptr;

        void set_map_load_thread_done_flag() {
            map_load_thread_done = true;
        }
    
        std::uint32_t load_map_override(const char *map_to_load) {
            // Load map in a separate thread so we can render the loading screen
            std::thread map_load_thread(load_map_worker_asm, map_to_load);

            if(device && std::strcmp(map_to_load, "levels\\ui\\ui") != 0) {
                map_load_thread.detach();

                std::chrono::steady_clock::time_point timestamp = std::chrono::steady_clock::now();

                // Continue rendering loading screen until map_load_thread is done
                while(!map_load_thread_done) {
                    if(!Engine::network_game_is_server() && Engine::network_game_is_client() && std::chrono::steady_clock::now() - timestamp > 3s) {
                        logger.debug("Sending chat message to prevent the server from timing out...");
                        const wchar_t *wort = L"wort wort wort";
                        auto message = Engine::NetworkGameMessages::HudChat(Engine::NetworkGameMessages::HudChatType::CUSTOM, Engine::network_game_get_local_rcon_id(), const_cast<wchar_t *>(wort));
                        char buffer[sizeof(message) + std::wcslen(wort) * 2];
                        uint32_t size = Engine::network_game_encode_message(buffer, Engine::NETWORK_GAME_MESSAGE_TYPE_HUD_CHAT, &message);
                        Engine::network_game_client_send_message(buffer, size);
                        timestamp = std::chrono::steady_clock::now();
                    }

                    MSG message;
                    if(PeekMessageA(&message, NULL, 0, 0, PM_REMOVE)) {
                        // Forward window messages so Windows doesn't think the game is frozen 
                        TranslateMessage(&message);
                        DispatchMessageA(&message);
                    }
                    else {
                        auto hr = device->TestCooperativeLevel();
                        if(hr == D3D_OK) {
                            device->BeginScene();
                            ::loading_screen_render();
                            device->EndScene();
                            device->Present(NULL, NULL, NULL, NULL);
                        }
                        else if(hr == D3DERR_DEVICENOTRESET) {
                            if(texture) {
                                texture->Release();
                                texture = nullptr;
                            }
                            if(pixel_shader) {
                                pixel_shader->Release();
                                pixel_shader = nullptr;
                            }
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

    static bool loading_scree_enable_command(int arg_count, const char **args) {
        auto &config = Config::get_config();
        bool mute_ingame = logger.mute_ingame();
        logger.mute_ingame(false);
        if(arg_count == 1) {
            bool enable = STR_TO_BOOL(args[0]);
            config.set("loading_screen.enable", enable);
            config.save();
            logger.warning("You must restart the game for this change to take effect.");
        }
        auto enable = config.get<bool>("loading_screen.enable");
        logger.info("Loading screen is {}", enable.value_or(true) ? "enabled" : "disabled");
        logger.mute_ingame(mute_ingame);
        return true;
    }

    void set_up_loading_screen() {
        register_command("enable_loading_screen", "features", "Set whether to set up loading screen at startup", "<setting: boolean>", loading_scree_enable_command, false, 0, 1);

        auto enable = Config::get_config().get<bool>("loading_screen.enable").value_or(true);
        if(!enable) {
            return;
        }

        Event::D3D9EndSceneEvent::subscribe(update_d3d9_device, Event::EVENT_PRIORITY_HIGHEST);
        Event::D3D9DeviceResetEvent::subscribe(on_device_reset, Event::EVENT_PRIORITY_HIGHEST);

        auto behavior_flags_sig = Memory::get_signature("d3d9_device_behavior_flags");
        auto load_map_function_sig = Memory::get_signature("load_map_function");
        if(!behavior_flags_sig || !load_map_function_sig) {
            logger.error("Failed to find signatures for loading screen.");
            return;
        }
        Memory::hook_function(behavior_flags_sig->data(), set_d3d9_device_multithreaded_flag, std::nullopt, false);
        Memory::override_function(load_map_function_sig->data(), load_map_override_asm, &load_map_function_address);
        Memory::override_function(reinterpret_cast<void *>(::loading_screen_draw_background), reinterpret_cast<void (*)()>(draw_loading_screen_background), nullptr);
    }
}
