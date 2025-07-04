// SPDX-License-Identifier: GPL-3.0-only

#include <chrono>
#include <balltze/legacy_api/engine/core.hpp>
#include <balltze/legacy_api/event.hpp>
#include <balltze/api.hpp>
#include <balltze/memory.hpp>
#include <balltze/hook.hpp>
#include <balltze/utils.hpp>
#include "../../logger.hpp"

namespace Balltze::LegacyApi::Event {
    extern "C" {
        void d3d9_begin_scene_before_event();
        void d3d9_begin_scene_after_event();
        void d3d9_end_scene_before_event();
        void d3d9_end_scene_after_event();
        void d3d9_device_reset_before_event();
        void d3d9_device_reset_after_event();

        void dispatch_d3d9_begin_scene_before_event(IDirect3DDevice9 *device) {
            D3D9BeginSceneEventContext context(device);
            D3D9BeginSceneEvent event(EVENT_TIME_BEFORE, context);
            event.dispatch();
        }

        void dispatch_d3d9_begin_scene_after_event(IDirect3DDevice9 *device) {
            D3D9BeginSceneEventContext context(device);
            D3D9BeginSceneEvent event(EVENT_TIME_AFTER, context);
            event.dispatch();
        }

        void dispatch_d3d9_end_scene_before_event(IDirect3DDevice9 *device) {
            D3D9EndSceneEventContext context(device);
            D3D9EndSceneEvent event(EVENT_TIME_BEFORE, context);
            event.dispatch();
        }

        void dispatch_d3d9_end_scene_after_event(IDirect3DDevice9 *device) {
            D3D9EndSceneEventContext context(device);
            D3D9EndSceneEvent event(EVENT_TIME_AFTER, context);
            event.dispatch();
        }

        void dispatch_d3d9_device_reset_before_event(IDirect3DDevice9 *device, D3DPRESENT_PARAMETERS *params) {
            D3D9DeviceResetEventContext context(device, params);
            D3D9DeviceResetEvent event(EVENT_TIME_BEFORE, context);
            event.dispatch();
        }

        void dispatch_d3d9_device_reset_after_event(IDirect3DDevice9 *device, D3DPRESENT_PARAMETERS *params) {
            D3D9DeviceResetEventContext context(device, params);
            D3D9DeviceResetEvent event(EVENT_TIME_AFTER, context);
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

        if(get_balltze_side() != BALLTZE_SIDE_CLIENT) {
            return;
        }

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

    template<>
    void EventHandler<D3D9EndSceneEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        if(get_balltze_side() != BALLTZE_SIDE_CLIENT) {
            return;
        }

        auto *d3d9_end_scene_sig = Memory::get_signature("d3d9_call_end_scene");
        if(!d3d9_end_scene_sig) {
            throw std::runtime_error("Could not find signature for D3D9 end scene event");
        }

        try {
            char *d3d9_call_end_scene_sig_ptr = reinterpret_cast<char *>(d3d9_end_scene_sig->data());

            if(d3d9_call_end_scene_sig_ptr[0] == 0xE9) {
                // Workaround for Chimera hook (NEEDS TO BE FIXED)
                std::byte *ptr = Memory::follow_32bit_jump(d3d9_end_scene_sig->data()) + 5;
                Memory::hook_function(ptr, d3d9_end_scene_before_event, d3d9_end_scene_after_event, false);
            }
            else {
                Memory::hook_function(d3d9_end_scene_sig->data(), d3d9_end_scene_before_event, d3d9_end_scene_after_event, false);
            }
        }
        catch(std::runtime_error &e) {
            throw std::runtime_error("Could not hook D3D9 end scene event: " + std::string(e.what()));
        }
    }

    template<>
    void EventHandler<D3D9DeviceResetEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        if(get_balltze_side() != BALLTZE_SIDE_CLIENT) {
            return;
        }

        auto *d3d9_device_reset_sig = Memory::get_signature("d3d9_call_reset");
        if(!d3d9_device_reset_sig) {
            throw std::runtime_error("Could not find signature for D3D9 device reset event");
        }

        try {
            char *d3d9_device_reset_sig_ptr = reinterpret_cast<char *>(d3d9_device_reset_sig->data());

            if(d3d9_device_reset_sig_ptr[0] == 0xE9) {
                // Workaround for Chimera hook (NEEDS TO BE FIXED)
                std::byte *ptr = Memory::follow_32bit_jump(d3d9_device_reset_sig->data()) + 5;
                Memory::hook_function(ptr, d3d9_device_reset_before_event, d3d9_device_reset_after_event, false);
            }
            else {
                Memory::hook_function(d3d9_device_reset_sig->data(), d3d9_device_reset_before_event, d3d9_device_reset_after_event, false);
            }
        }
        catch(std::runtime_error &e) {
            throw std::runtime_error("Could not hook D3D9 device reset event: " + std::string(e.what()));
        }
    }
}
