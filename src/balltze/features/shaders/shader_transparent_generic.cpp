// SPDX-License-Identifier: GPL-3.0-only

#include <d3d9.h>
#include <windows.h>
#include <cstdint>
#include <cmath>
#include <balltze/memory.hpp>
#include <balltze/hook.hpp>
#include <balltze/events/map_load.hpp>
#include <balltze/engine/tag.hpp>
#include <impl/rasterizer/rasterizer_shader_transparent_generic.h>
#include "../../logger.hpp"

namespace Balltze::Features {
    extern "C" {
        void *switch_jmp_ret;
        void *switch_jmp = nullptr;
        void *switch_default = nullptr;
        void draw_shader_transparent_generic_hook();
    }
 
    void set_up_shader_transparent_generic_impl() {
        auto *rasterizer_shader_switch_cmp_sig = Memory::get_signature("rasterizer_shader_switch_cmp");
        auto *rasterizer_shader_switch_jmp_sig = Memory::get_signature("rasterizer_shader_switch_entry");
        auto *rasterizer_shader_switch_default_sig = Memory::get_signature("rasterider_shader_switch_default_case");
        if(!rasterizer_shader_switch_cmp_sig || !rasterizer_shader_switch_jmp_sig || !rasterizer_shader_switch_default_sig) {
            logger.error("Could not find rasterizer shader switch signatures");
            return;
        }

        Memory::override_function(rasterizer_shader_switch_cmp_sig->data(), draw_shader_transparent_generic_hook, switch_jmp_ret, false);
        switch_jmp = rasterizer_shader_switch_jmp_sig->data();
        switch_default = rasterizer_shader_switch_default_sig->data();

        Event::MapLoadEvent::subscribe([](Event::MapLoadEvent &event) {
            if(event.time == Event::EVENT_TIME_BEFORE) {
                rasterizer_shader_transparent_generic_clear_instances();
            }
            else {
                auto transparent_generic_tags = Engine::find_tags({}, Engine::TAG_CLASS_SHADER_TRANSPARENT_GENERIC);
                for(auto *tag : transparent_generic_tags) {
                    rasterizer_shader_transparent_generic_get_instance(reinterpret_cast<ShaderTransparentGeneric *>(tag->data));
                }
            }
        });
    }
}
