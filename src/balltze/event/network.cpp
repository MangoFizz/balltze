// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/command.hpp>
#include <balltze/memory.hpp>
#include <balltze/hook.hpp>
#include "../logger.hpp"
#include "network.hpp"

namespace Balltze::Event {
    extern "C" {
        bool packet_received_before_event();
        void packet_received_after_event();

        bool packet_received_event_before_dispatcher(SOCKET **socket, std::byte **buffer, sockaddr **from, size_t *from_length) {
            PacketReceivedEventArguments args(*socket, *buffer, 0, *from, *from_length);
            PacketReceivedEvent event(EventTime::EVENT_TIME_BEFORE, args);
            event.dispatch();
            return event.cancelled();
        }

        void packet_received_event_after_dispatcher(SOCKET **socket, std::byte **buffer, size_t bytes_read, sockaddr **from, size_t *from_length) {
            PacketReceivedEventArguments args(*socket, *buffer, bytes_read, *from, *from_length);
            PacketReceivedEvent event(EventTime::EVENT_TIME_AFTER, args);
            event.dispatch();
        }
    }

    static bool debug_packet_received_event_command(int arg_count, const char **args) {
        static std::optional<Event::EventListenerHandle<PacketReceivedEvent>> handle;
        if(arg_count == 1) {
            bool new_setting = STR_TO_BOOL(args[0]);
            if(new_setting) {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
                handle = Event::PacketReceivedEvent::subscribe_const([](PacketReceivedEvent const &event) {
                    auto &arguments = event.args;
                    if(event.time == EVENT_TIME_AFTER) {
                        logger.debug("Packet received event: bytes read: {}", arguments.bytes_read);
                    }                    
                }, EVENT_PRIORITY_HIGHEST);
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

    template<>
    void EventHandler<PacketReceivedEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        auto *socket_recv_from_call_sig = Memory::get_signature("socket_recv_from_call");
        if(!socket_recv_from_call_sig) {
            throw std::runtime_error("Could not find signature for packet received event");
        }

        try {
            std::function<bool ()> before_function = packet_received_before_event;
            Memory::hook_function(socket_recv_from_call_sig->data(), before_function, packet_received_after_event, true);
        }
        catch(std::exception &e) {
            logger.error("Could not hook packet received event: {}", e.what());
        }

        // Register debug command
        register_command("debug_packet_received_event", "debug", "Sets whenever to log packet receives.", "[enable: boolean]", debug_packet_received_event_command, true, 0, 1);
    }
}