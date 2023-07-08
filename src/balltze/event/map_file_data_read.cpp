// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/event.hpp>
#include <balltze/hook.hpp>
#include "../logger.hpp"

namespace Balltze::Event {
    extern "C" {
        void map_file_data_read_event_before();
        void map_file_data_read_event_after();

        void dispatch_map_file_data_read_event_before(HANDLE file_descriptor, std::byte *output, std::size_t size, LPOVERLAPPED *overlapped) {
            MapFileDataReadEventArgs args;
            args.file_handle = file_descriptor;
            args.output_buffer = output;
            args.size = size;
            args.overlapped = *overlapped;
            MapFileDataReadEvent event(EVENT_TIME_BEFORE, args);
            event.dispatch();
            *overlapped = event.args.overlapped;
        }

        void dispatch_map_file_data_read_event_after(HANDLE file_descriptor, std::byte *output, std::size_t size, LPOVERLAPPED overlapped) {
            MapFileDataReadEventArgs args;
            args.file_handle = file_descriptor;
            args.output_buffer = output;
            args.size = size;
            args.overlapped = overlapped;
            MapFileDataReadEvent event(EVENT_TIME_AFTER, args);
            event.dispatch();
        }
    }

    template <>
    void EventHandler<MapFileDataReadEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        auto *read_map_file_data_call_1_sig = Memory::get_signature("read_map_file_data_call_1");
        auto *read_map_file_data_call_2_sig = Memory::get_signature("read_map_file_data_call_2");
        if(!read_map_file_data_call_1_sig || !read_map_file_data_call_2_sig) {
            throw std::runtime_error("Could not find signature for map file data read event");
        }

        Memory::hook_function(read_map_file_data_call_1_sig->data(), map_file_data_read_event_before, map_file_data_read_event_after);
        Memory::hook_function(read_map_file_data_call_2_sig->data(), map_file_data_read_event_before, map_file_data_read_event_after);
    }
}
