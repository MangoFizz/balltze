// SPDX-License-Identifier: GPL-3.0-only

#include <chrono>
#include <balltze/engine/core.hpp>
#include <balltze/event.hpp>
#include <balltze/memory.hpp>
#include <balltze/hook.hpp>
#include <balltze/utils.hpp>

namespace Balltze::Event {
    extern "C" {
        void d3d9_end_scene_before_event();
        void d3d9_end_scene_after_event();

        void dispatch_d3d9_end_scene_before_event(IDirect3DDevice9 *device) {
            D3D9EndSceneEventArguments args(device);
            D3D9EndSceneEvent event(EVENT_TIME_BEFORE, args);
            event.dispatch();
        }

        void dispatch_d3d9_end_scene_after_event(IDirect3DDevice9 *device) {
            D3D9EndSceneEventArguments args(device);
            D3D9EndSceneEvent event(EVENT_TIME_AFTER, args);
            event.dispatch();
        }
    }

    template <>
    void EventHandler<D3D9EndSceneEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        auto *d3d9_end_scene_sig = Memory::get_signature("d3d9_call_end_scene");
        if(!d3d9_end_scene_sig) {
            throw std::runtime_error("Could not find signature for D3D9 end scene event");
        }

        try {
            // Workaround for Chimera hook (NEEDS TO BE FIXED)
            std::byte *ptr = Memory::follow_32bit_jump(d3d9_end_scene_sig->data()) + 5;
            Memory::hook_function(ptr, d3d9_end_scene_before_event, d3d9_end_scene_after_event, false);
        }
        catch(std::runtime_error &e) {
            throw std::runtime_error("Could not hook D3D9 end scene event: " + std::string(e.what()));
        }
    }
}
