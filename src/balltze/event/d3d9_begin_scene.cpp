// SPDX-License-Identifier: GPL-3.0-only

#include <chrono>
#include <balltze/engine/core.hpp>
#include <balltze/event.hpp>
#include <balltze/memory.hpp>
#include <balltze/hook.hpp>
#include <balltze/utils.hpp>
#include "d3d9_begin_scene.hpp"

namespace Balltze::Event {
    extern "C" {
        void d3d9_begin_scene_before_event();
        void d3d9_begin_scene_after_event();

        void dispatch_d3d9_begin_scene_before_event(IDirect3DDevice9 *device) {
            D3D9BeginSceneEventArguments args(device);
            D3D9BeginSceneEvent event(EVENT_TIME_BEFORE, args);
            event.dispatch();
        }

        void dispatch_d3d9_begin_scene_after_event(IDirect3DDevice9 *device) {
            D3D9BeginSceneEventArguments args(device);
            D3D9BeginSceneEvent event(EVENT_TIME_AFTER, args);
            event.dispatch();
        }
    }

    template<>
    void EventHandler<D3D9BeginSceneEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        auto *d3d9_begin_scene_sig = Memory::get_signature("d3d9_call_begin_scene");
        if(!d3d9_begin_scene_sig) {
            throw std::runtime_error("Could not find signature for D3D9 begin scene event");
        }

        try {
            Memory::hook_function(d3d9_begin_scene_sig->data(), d3d9_begin_scene_before_event, d3d9_begin_scene_after_event, false);
        }
        catch(std::runtime_error &e) {
            throw std::runtime_error("Could not hook D3D9 begin scene event: " + std::string(e.what()));
        }
    }
}
