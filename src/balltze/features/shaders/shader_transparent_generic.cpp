// SPDX-License-Identifier: GPL-3.0-only

#include <d3d9.h>
#include <windows.h>
#include <cstdint>
#include <cmath>
#include <balltze/memory.hpp>
#include <balltze/hook.hpp>
#include <balltze/events/map_load.hpp>
#include <balltze/engine/tag.hpp>
#include <balltze/helpers/resources.hpp>
#include <impl/rasterizer/rasterizer_shader_transparent_generic.h>
#include "../../resources.hpp"
#include "../../logger.hpp"

namespace Balltze::Features {
    extern "C" {
        extern char *shader_transparent_generic_source;
        
        void *switch_jmp = nullptr;
        void *switch_default = nullptr;
        void draw_shader_transparent_generic_hook();
    }

    static std::vector<std::byte> shader_source;

    static bool load_source_from_resources() {
        auto source = load_resource_data(get_current_module(), MAKEINTRESOURCEW(ID_SHADER_TRANSPARENT_GENERIC_SRC), L"HLSL");
        if(!source) {
            return false;
        }
        shader_source = std::move(source.value());
        shader_source.push_back(std::byte{0}); // null-terminate the string
        shader_transparent_generic_source = reinterpret_cast<char *>(shader_source.data());
        return true;
    }
 
    void set_up_shader_transparent_generic_impl() {
        auto *rasterizer_shader_switch_cmp_sig = Memory::get_signature("rasterizer_shader_switch_cmp");
        auto *rasterizer_shader_switch_jmp_sig = Memory::get_signature("rasterizer_shader_switch_entry");
        auto *rasterizer_shader_switch_default_sig = Memory::get_signature("rasterider_shader_switch_default_case");
        if(!rasterizer_shader_switch_cmp_sig || !rasterizer_shader_switch_jmp_sig || !rasterizer_shader_switch_default_sig) {
            logger.error("Could not find rasterizer shader switch signatures");
            return;
        }

        if(!load_source_from_resources()) {
            logger.error("Could not load shader transparent generic source from resources");
            return;
        }

        void *switch_jmp_ret;
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
