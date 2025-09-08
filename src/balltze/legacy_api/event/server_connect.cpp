// SPDX-License-Identifier: GPL-3.0-only

#include <memory>
#include <balltze/api.hpp>
#include <balltze/legacy_api/event.hpp>
#include <balltze/hook.hpp>
#include <balltze/command.hpp>
#include "../../logger.hpp"

namespace Balltze::LegacyApi::Event {
    static std::unique_ptr<ServerConnectEventContext> server_connect_event_args;

    extern "C" {
        void server_connect_event_before() noexcept;
        void server_connect_event_after() noexcept;
        
        bool dispatch_server_connect_event_before(std::uint32_t &address, std::uint16_t &port, wchar_t *password) {
            server_connect_event_args = std::make_unique<ServerConnectEventContext>(address, port, password);
            ServerConnectEvent event(EVENT_TIME_BEFORE, *server_connect_event_args);
            event.dispatch();
            return event.cancelled();
        }

        void dispatch_server_connect_event_after() {
            ServerConnectEvent event(EVENT_TIME_AFTER, *server_connect_event_args);
            event.dispatch();
            server_connect_event_args = nullptr;
        }
    }

    std::string ip_address_int_to_string(std::uint32_t address_int) noexcept {
        std::string address;
        for(int i = 0; i < 4; i++) {
            address += std::to_string((address_int >> (i * 8)) & 0xFF);
            if(i < 3) {
                address += ".";
            }
        }
        return address;
    }

    template<>
    void EventHandler<ServerConnectEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        if(get_balltze_side() != BALLTZE_SIDE_CLIENT) {
            return;
        }

        auto *server_connect_function_call_sig = Memory::get_signature("server_connect_function_call");
        if(!server_connect_function_call_sig) {
            throw std::runtime_error("Could not find signature for server connnect event");
        }

        try {
            std::function<bool()> before = reinterpret_cast<bool (*)()>(server_connect_event_before);
            std::function<void()> after = reinterpret_cast<void (*)()>(server_connect_event_after);
            Memory::hook_function(server_connect_function_call_sig->data(), before, after);
        }
        catch(const std::runtime_error &e) {
            throw std::runtime_error("Could not hook server connnect event: " + std::string(e.what()));
        }
    }
}
