// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__EVENTS__SERVER_CONNECT_HPP
#define BALLTZE_API__EVENTS__SERVER_CONNECT_HPP

#include <cstdint>
#include <string>
#include "../event.hpp"

namespace Balltze::LegacyApi::Event {
    struct ServerConnectEventContext {
        const std::uint32_t address;
        const std::uint16_t port;
        const std::wstring password;

        ServerConnectEventContext(const std::uint32_t &address, const std::uint16_t port, wchar_t *password) : address(address), port(port), password(password) {}
    };

    class ServerConnectEvent : public EventData<ServerConnectEvent> {
    public:
        ServerConnectEventContext context;

        bool cancellable() const {
            return true;
        }

        ServerConnectEvent(EventTime time, const ServerConnectEventContext &context) : EventData(time), context(context) {}
    };
}

#endif
