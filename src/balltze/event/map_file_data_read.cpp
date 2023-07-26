// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/event.hpp>
#include <balltze/hook.hpp>
#include <balltze/command.hpp>
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

    static bool debug_map_file_data_read_event(int arg_count, const char **args) {
        static std::optional<Event::EventListenerHandle<MapFileDataReadEvent>> handle;
        if(arg_count == 1) {
            bool new_setting = STR_TO_BOOL(args[0]);
            if(new_setting) {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
                handle = Event::MapFileDataReadEvent::subscribe_const([](MapFileDataReadEvent const &event) {
                    auto &arguments = event.args;
                    logger.debug("Map file data read event: file handle: {}, output buffer: {}, size: {}, overlapped: {}", reinterpret_cast<std::uint32_t>(arguments.file_handle), reinterpret_cast<std::uint32_t>(arguments.output_buffer), arguments.size, arguments.overlapped->Offset);
                });
            }
            else {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
            }
        }
        logger.info("debug_map_file_data_read_event: {}", handle.has_value());
        return true;
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

        try {
            Memory::hook_function(read_map_file_data_call_1_sig->data(), map_file_data_read_event_before, map_file_data_read_event_after);
            Memory::hook_function(read_map_file_data_call_2_sig->data(), map_file_data_read_event_before, map_file_data_read_event_after);
        }
        catch(std::runtime_error &e) {
            throw std::runtime_error("Could not hook map file data read event: " + std::string(e.what()));
        }

        // Register debug command
        register_command("debug_map_file_data_read_event", "debug", "Debug map file data read event", debug_map_file_data_read_event, true, 0, 1);
    }
}
