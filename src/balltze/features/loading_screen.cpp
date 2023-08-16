// SPDX-License-Identifier: GPL-3.0-only

#include <filesystem>
#include <cstdlib>
#include <thread>
#include <atomic>
#include <d3d9.h>
#include <windows.h>
#include <balltze/command.hpp>
#include <balltze/hook.hpp>
#include <balltze/memory.hpp>
#include <balltze/event.hpp>
#include "../config/config.hpp"
#include "../logger.hpp"

namespace Balltze::Features {
    #define BIKSURFACE32 3
    #define BINKCOPYALL 0x80000000L

    #define ASSERT_D3D(x, msg) if(x != D3D_OK) { logger.error("Assertion failed: {}", std::string(msg)); return; }
    #define ASSERT_D3D_EX(x, msg, ex) if(x != D3D_OK) { logger.error("Assertion failed: {}", std::string(msg)); ex; return; }

    struct Bik {
        std::uint32_t width;
        std::uint32_t height;
        std::uint32_t frames_count;
        std::uint32_t current_frame;
    };

    struct Vertex {
        float x, y, z, rhw;
        D3DCOLOR color;
        float u, v;
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

    static bool loading_screen_playback = false; 
    static Bik *bik = nullptr;
    static IDirect3DDevice9 *device = nullptr;
    static IDirect3DTexture9 *texture = nullptr;
    static D3DLOCKED_RECT locked_rect;
    static std::map<D3DRENDERSTATETYPE, DWORD> old_render_state;

    static std::filesystem::path get_loading_screens_path() {
        auto path = Config::get_balltze_directory() / "loading_screens";
        std::filesystem::create_directory(path);
        return path;
    }

    static std::filesystem::path choose_random_loading_screen() {
        auto path = get_loading_screens_path();
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

    static void end_loading_screen() {
        if(bik) {
            bik_close(bik);
            bik = nullptr;
        }
        if(texture) {
            texture->Release();
            texture = nullptr;
        }
        loading_screen_playback = false;
    }

    static void play_loading_screen() {
        auto path = choose_random_loading_screen();
        if(path.empty()) {
            logger.warning("No loading screens found");
            return;
        }

        if(bik) {
            end_loading_screen();
        }

        bik = bik_open(path.string().c_str(), 0);
        if(!bik) {
            logger.error("Failed to open bik file");
            return;
        }

        loading_screen_playback = true;
    }

    /**
     * Reference: https://learn.microsoft.com/en-us/windows/win32/direct3d9/id3dxsprite--begin#remarks
     */
    void sprite_render_begin() {
        auto set_render_state = [](D3DRENDERSTATETYPE type, DWORD value) {
            DWORD old_value = 0;
            device->GetRenderState(type, &old_value);
            device->SetRenderState(type, value);
            old_render_state.insert_or_assign(type, old_value);
        };

        set_render_state(D3DRS_ALPHABLENDENABLE, true);
        set_render_state(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
        set_render_state(D3DRS_ALPHAREF, 0);
        set_render_state(D3DRS_ALPHATESTENABLE, true);
        set_render_state(D3DRS_BLENDOP, D3DBLENDOP_ADD);
        set_render_state(D3DRS_CLIPPING, true);
        set_render_state(D3DRS_CLIPPLANEENABLE, false);
        set_render_state(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_RED);
        set_render_state(D3DRS_CULLMODE, D3DCULL_NONE);
        set_render_state(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        set_render_state(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
        set_render_state(D3DRS_ENABLEADAPTIVETESSELLATION, FALSE);
        set_render_state(D3DRS_FILLMODE, D3DFILL_SOLID);
        set_render_state(D3DRS_FOGENABLE, false);
        set_render_state(D3DRS_INDEXEDVERTEXBLENDENABLE, false);
        set_render_state(D3DRS_LIGHTING, false);
        set_render_state(D3DRS_RANGEFOGENABLE, false);
        set_render_state(D3DRS_SEPARATEALPHABLENDENABLE, false);
        set_render_state(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
        set_render_state(D3DRS_SPECULARENABLE, false);
        set_render_state(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        set_render_state(D3DRS_SRGBWRITEENABLE, false);
        set_render_state(D3DRS_STENCILENABLE, false);
        set_render_state(D3DRS_VERTEXBLEND, false);
        set_render_state(D3DRS_WRAP0, 0);
    }

    void sprite_render_end() {
        for(auto &[type, value] : old_render_state) {
            device->SetRenderState(type, value);
        }
    }

    void render_sprite(float x, float y, float width, float height, IDirect3DTexture9 *texture) {
        float left = x;
        float right = x + width;
        float top = y;
        float bottom = y + height;

        // Create vertices for our sprite
        Vertex vertices[4] = {
            { left, top, 0.0f, 1.0f, D3DCOLOR_ARGB(255, 255, 255, 255), 0.0f, 0.0f },
            { right, top, 0.0f, 1.0f, D3DCOLOR_ARGB(255, 255, 255, 255), 1.0f, 0.0f },
            { left, bottom, 0.0f, 1.0f, D3DCOLOR_ARGB(255, 255, 255, 255), 0.0f, 1.0f },
            { right, bottom, 0.0f, 1.0f, D3DCOLOR_ARGB(255, 255, 255, 255), 1.0f, 1.0f }
        };

        sprite_render_begin();
        ASSERT_D3D_EX(device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1), "Failed to set FVF", sprite_render_end());
        ASSERT_D3D_EX(device->SetTexture(0, texture), "Failed to set texture", sprite_render_end());
        ASSERT_D3D_EX(device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(Vertex)), "Failed to draw primitive", sprite_render_end());
        sprite_render_end();
    }

    static void set_texture_alpha_channel(std::uint8_t alpha) {
        for(int y = 0; y < bik->height; ++y) {
            for(int x = 0; x < bik->width; ++x) {
                int pixelIndex = y * locked_rect.Pitch + x * 4;
                reinterpret_cast<std::uint8_t *>(locked_rect.pBits)[pixelIndex + 3] = alpha;
            }
        }
    }

    static void draw_loading_screen() {
        if(!device || !loading_screen_playback) {
            return;
        }

        if(!texture) {
            ASSERT_D3D(device->CreateTexture(bik->width, bik->height, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &texture, NULL), "Failed to create texture");
        }

        if(bik) {
            auto res = bik_wait(bik);
            if(res == 0) {
                bik_do_frame(bik);
                bik_next_frame(bik);
            }

            ASSERT_D3D(texture->LockRect(0, &locked_rect, NULL, D3DLOCK_DISCARD), "Failed to lock texture");
            bik_copy_to_buffer(bik, locked_rect.pBits, locked_rect.Pitch, bik->height, 0, 0, BINKCOPYALL | BIKSURFACE32);
            set_texture_alpha_channel(255);
            ASSERT_D3D(texture->UnlockRect(0), "Failed to unlock texture");

            render_sprite(0, 0, bik->width, bik->height, texture);

            if(bik->current_frame >= bik->frames_count) {
                bik_go_to(bik, 1, 0);
                bik_set_volume(bik, 0, 0);
            }
        }
    }

    static void update_d3d9_device(Event::D3D9EndSceneEvent &event) {
        if(!device) {
            device = event.args.device;
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

        void set_map_load_thread_done_flag() {
            std::this_thread::sleep_for(std::chrono::milliseconds(3300)); // ADD A SETTING FOR THIS
            map_load_thread_done = true;
        }
    }

    static std::uint32_t load_map_override() {
        // Load map in a separate thread so we can render the loading screen
        std::thread(load_map_override_asm, map_to_load.c_str()).detach();

        IDirect3DDevice9 *main_device = device;
        IDirect3DTexture9 *old_texture = texture;
        D3DPRESENT_PARAMETERS present_params;
        IDirect3DSwapChain9 *swap_chain;
        IDirect3DDevice9 *aux_device;

        // Get present params from the main device
        main_device->GetSwapChain(0, &swap_chain);
        ZeroMemory(&present_params, sizeof(present_params));
        swap_chain->GetPresentParameters(&present_params);

        // Create an aux device to render the loading screen while the main one is busy setting the map stuff
        static auto d3d9_handle = LoadLibraryA("d3d9.dll");
        static auto PDirect3DCreate9 = reinterpret_cast<decltype(Direct3DCreate9) *>(GetProcAddress(d3d9_handle, "Direct3DCreate9"));
        IDirect3D9 *d3d9 = PDirect3DCreate9(D3D_SDK_VERSION);
        d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, Engine::get_window_globals()->hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &present_params, &aux_device);

        // Set aux device as the main one
        device = aux_device;
        texture = nullptr;

        // Continue rendering loading screen until load_map thread is done
        while(!map_load_thread_done) {
            MSG message;
            if(PeekMessageA(&message, NULL, 0, 0, PM_REMOVE)) {
                // Forward window messages so Windows doesn't think the game is frozen 
                TranslateMessage(&message);
                DispatchMessageA(&message);
            }
            else {
                device->BeginScene();
                draw_loading_screen();
                device->EndScene();
                device->Present(NULL, NULL, NULL, NULL);
            }
        }

        // Release aux device resources
        device = main_device;
        if(texture) {
            texture->Release();
            texture = old_texture;
        }
        aux_device->Release();
        d3d9->Release();

        // Reset thread exit flag
        map_load_thread_done = false;

        return load_map_function_result;
    }

    std::uint32_t *game_loading_screen_flag = nullptr;

    static void loading_screen_hook() {
        bool are_we_in_loading_screen = *game_loading_screen_flag != 0;
        if(!loading_screen_playback && are_we_in_loading_screen) {
            play_loading_screen();
        }
        else if(loading_screen_playback && !are_we_in_loading_screen) {
            end_loading_screen();
        }
        draw_loading_screen();
    }

    static bool loading_screen_render_hook() {
        return true;
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

    static void load_bik_functions() {
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

    static Event::EventListenerHandle<Event::TickEvent> tick_event_listener_handle;

    void set_up_loading_screen() {
        Event::D3D9EndSceneEvent::subscribe(update_d3d9_device, Event::EVENT_PRIORITY_HIGHEST);
        Event::D3D9DeviceResetEvent::subscribe(on_device_reset, Event::EVENT_PRIORITY_HIGHEST);
        Event::MapLoadEvent::subscribe(update_map_name);

        register_command("test_loading_screen", "debug", "Plays a random loading screen", std::nullopt, [](int arg_count, const char **args) -> bool {
            play_loading_screen();
            return true;
        }, false, 0, 0);

        load_bik_functions();

        tick_event_listener_handle = Event::TickEvent::subscribe([](Event::TickEvent &) {
            auto load_map_function_sig = Memory::get_signature("load_map_function");
            auto loading_screen_handler_sig = Memory::get_signature("loading_screen_handler");
            auto loading_screen_flag_address_sig = Memory::get_signature("loading_screen_flag_address");
            auto loading_screen_render_function_call_sig = Memory::get_signature("loading_screen_render_function_call");
            if(!load_map_function_sig || !loading_screen_handler_sig || !loading_screen_flag_address_sig || !loading_screen_render_function_call_sig) {
                logger.error("Failed to find signatures for loading screen.");
                return;
            }
            game_loading_screen_flag = *reinterpret_cast<std::uint32_t **>(loading_screen_flag_address_sig->data());
            std::function<void()> load_map_hook_callback = reinterpret_cast<void(*)()>(load_map_override);
            Memory::override_function(load_map_function_sig->data(), load_map_hook_callback, load_map_function_address);
            Memory::hook_function(loading_screen_handler_sig->data(), loading_screen_hook);
            Memory::hook_function(loading_screen_render_function_call_sig->data(), std::function<bool()>(loading_screen_render_hook));
            tick_event_listener_handle.remove();
        });
    }
}
