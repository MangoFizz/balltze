// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__EVENTS__SERVER_CONNECT_HPP
#define BALLTZE_API__EVENTS__SERVER_CONNECT_HPP

#include <cstdint>
#include <string>
#include "../event.hpp"

namespace Balltze::Event {
    struct ServerConnectEventArgs {
        const std::uint32_t address;
        const std::uint16_t port;
        const std::wstring password;

        ServerConnectEventArgs(const std::uint32_t &address, const std::uint16_t port, wchar_t *password) : address(address), port(port), password(password) {}
    };

    class ServerConnectEvent : public EventData<ServerConnectEvent> {
    public:
        ServerConnectEventArgs args;

        bool cancellable() const {
            return true;
        }

        ServerConnectEvent(EventTime time, const ServerConnectEventArgs &args) : EventData(time), args(args) {}
    };
}

#endif
