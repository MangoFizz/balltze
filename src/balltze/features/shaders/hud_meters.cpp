// SPDX-License-Identifier: GPL-3.0-only

#include <d3d9.h>
#include <windows.h>
#include <balltze/events/d3d9.hpp>
#include <balltze/memory.hpp>
#include <balltze/hook.hpp>
#include <balltze/command.hpp>
#include <balltze/engine/data_types.hpp>
#include <balltze/engine/rasterizer.hpp>
#include <balltze/engine/tag_definitions/unit_hud_interface.hpp>
#include <balltze/helpers/resources.hpp>
#include "../../resources.hpp"
#include "../../logger.hpp"

namespace Balltze::Features {
    static IDirect3DPixelShader9 *hud_meters_shader = nullptr;
    static Engine::HudInterfaceMeter *current_meter = nullptr;
    static bool force_xbox_shading = false;

    static HRESULT load_pixel_shader_from_resource(IDirect3DDevice9 *device, std::uint32_t res, IDirect3DPixelShader9 **shader) {
        auto shader_data = load_resource_data(get_current_module(), MAKEINTRESOURCEW(res), L"CSO");
        if(!shader_data) {
            return E_FAIL;
        }
        HRESULT hr = device->CreatePixelShader(reinterpret_cast<const DWORD *>(shader_data->data()), shader);
        return hr;
    }

    struct HudMeterColors {
        Engine::ColorARGBInt tint_color;
        Engine::ColorARGBInt max_color; // Not sure what is this, it isn't used 
        Engine::ColorARGBInt empty_color;
        Engine::ColorARGBInt alpha_ref;
    };

    extern "C" {
        void *rasterizer_draw_screen_quad_bitmap_meter_return;
        void save_hud_interface_meter_being_drawn();
        void draw_hud_interface_meter_asm();

        void draw_hud_interface_meter(Engine::HudInterfaceMeter *meter, HudMeterColors *meter_colors, Engine::ScreenQuad *vertex_data) {
            auto *device = Engine::get_d3d9_device();
            if (!device) {
                return;
            }

            if(!meter->flags.use_xbox_shading && !force_xbox_shading) {
                device->SetPixelShader(NULL);
                device->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
                device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
                device->SetRenderState(D3DRS_ALPHAREF, meter_colors->tint_color.alpha);
                device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
                device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
                device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
                device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

                // Draw filled part
                device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_LESSEQUAL);
                device->SetRenderState(D3DRS_TEXTUREFACTOR, *(DWORD *)&meter_colors->tint_color);
                device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
                device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
                device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
                device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
                device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
                device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
                device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
                device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vertex_data, 0x18);

                // Draw empty part
                device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
                device->SetRenderState(D3DRS_TEXTUREFACTOR, *(DWORD *)&meter_colors->empty_color);
                device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE2X);
                device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
                device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
                device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
                device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
                device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
                device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
                device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vertex_data, 0x18);
            }
            else {
                if(!hud_meters_shader) {
                    logger.warning("HUD meters shader not loaded. This is a bug.");
                    return;
                }
                
                auto tint_color = Engine::color_argb_32_to_real(meter_colors->tint_color);
                auto empty_color = Engine::color_argb_32_to_real(meter->empty_color);
                auto alpha_ref = Engine::color_argb_32_to_real(meter_colors->alpha_ref);
                auto flash_color = Engine::color_argb_32_to_real(meter->flash_color);
                float c_tint_color[4] = { tint_color.red, tint_color.green, tint_color.blue, tint_color.alpha };
                float c_empty_color[4] = { empty_color.red, empty_color.green, empty_color.blue, empty_color.alpha };
                float c_background[2] = { meter->opacity, meter->translucency };
                float c_flash_color[4] = { flash_color.red, flash_color.green, flash_color.blue, alpha_ref.alpha };

                device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
                device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_SRCALPHA);
                device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_INVSRCALPHA);
                device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

                device->SetPixelShaderConstantF(0, c_tint_color, 1);
                device->SetPixelShaderConstantF(1, c_empty_color, 1);
                device->SetPixelShaderConstantF(2, c_background, 1);
                device->SetPixelShaderConstantF(3, c_flash_color, 1);
                device->SetPixelShader(hud_meters_shader);
                device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vertex_data, 0x18);
            }
        }
    }

    static Memory::Hook *hud_draw_meter_bitmap_call_hook;
    static Memory::Hook *rasterizer_draw_screen_quad_bitmap_meter_hook;
    static Event::D3D9BeginSceneEvent::ListenerHandle device_begin_scene_listener;

    static void on_device_begin_scene(const Event::D3D9BeginSceneEvent &event) {
        if(hud_meters_shader) {
            return;
        }

        auto *device = event.context.device;

        if(FAILED(load_pixel_shader_from_resource(device, ID_HUD_METERS_SHADER, &hud_meters_shader))) {
            logger.error("Failed to load HUD meters shader");
            hud_draw_meter_bitmap_call_hook->release();
            rasterizer_draw_screen_quad_bitmap_meter_hook->release();
            device_begin_scene_listener.remove();
            return;
        }

        static Event::D3D9DeviceResetEvent::ListenerHandle device_reset_listener = Event::D3D9DeviceResetEvent::subscribe_const([](const auto &event) {
            if(hud_meters_shader) {
                hud_meters_shader->Release();
                hud_meters_shader = nullptr;
            }
            device_reset_listener.remove();
        });
    }

    static bool force_xbox_shading_command(int arg_count, const char **args) {
        if(arg_count == 1) {
            bool new_setting = STR_TO_BOOL(args[0]);
            force_xbox_shading = new_setting;
        }
        logger.info("force_xbox_shading_command is set to: {}", BOOL_TO_STR(force_xbox_shading));
        Engine::console_printf("force_xbox_shading_command: %s", BOOL_TO_STR(force_xbox_shading));
        return true;
    }

    void set_up_hud_meters_shader() {
        auto *hud_draw_meter_bitmap_call_sig = Memory::get_signature("hud_draw_meter_bitmap_call");
        auto *rasterizer_draw_screen_quad_bitmap_meter_sig = Memory::get_signature("rasterizer_draw_screen_quad_bitmap_meter");
        auto *rasterizer_draw_screen_quad_bitmap_meter_return_sig = Memory::get_signature("rasterizer_draw_screen_quad_bitmap_meter_return");

        if(!hud_draw_meter_bitmap_call_sig || !rasterizer_draw_screen_quad_bitmap_meter_sig || !rasterizer_draw_screen_quad_bitmap_meter_return_sig) {
            logger.error("Failed to find signatures for HUD meters shader");
            return;
        }

        try {
            hud_draw_meter_bitmap_call_hook = Memory::hook_function(hud_draw_meter_bitmap_call_sig->data(), save_hud_interface_meter_being_drawn);
            rasterizer_draw_screen_quad_bitmap_meter_hook = Memory::override_function(rasterizer_draw_screen_quad_bitmap_meter_sig->data(), draw_hud_interface_meter_asm, rasterizer_draw_screen_quad_bitmap_meter_return);
            rasterizer_draw_screen_quad_bitmap_meter_return = rasterizer_draw_screen_quad_bitmap_meter_return_sig->data();
            device_begin_scene_listener = Event::D3D9BeginSceneEvent::subscribe_const(on_device_begin_scene);
        }
        catch(const std::runtime_error &e) {
            logger.error("Failed to set up HUD meters shader: {}", e.what());
        }

        register_command("hud_meters_force_xbox_shading", "features", "Set whether to force Xbox shading for HUD meters", "<setting: boolean>", force_xbox_shading_command, true, 0, 1);
    }
}
