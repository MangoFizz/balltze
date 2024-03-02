// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__EVENTS__NETWORK_HPP
#define BALLTZE_API__EVENTS__NETWORK_HPP

#include <string>
#include <balltze/event.hpp>

namespace Balltze::Event {
    struct PacketReceivedEventArguments {
        SOCKET *socket;
        std::byte *buffer;
        size_t bytes_read;
        sockaddr *from;
        size_t from_length;

        PacketReceivedEventArguments(SOCKET *socket, std::byte *buffer, size_t bytes_read, sockaddr *from, size_t from_length) 
            : socket(socket), buffer(buffer), bytes_read(bytes_read), from(from), from_length(from_length) {}
    };

    class PacketReceivedEvent : public EventData<PacketReceivedEvent> {
    public:
        PacketReceivedEventArguments args;

        bool cancellable() const {
            return false;
        }

        PacketReceivedEvent(EventTime time, PacketReceivedEventArguments args) : EventData(time), args(args) {}
    };
}

#endif
