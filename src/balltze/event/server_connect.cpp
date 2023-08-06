// SPDX-License-Identifier: GPL-3.0-only

#include <memory>
#include <balltze/event.hpp>
#include <balltze/hook.hpp>
#include <balltze/command.hpp>
#include "../logger.hpp"

namespace Balltze::Event {
    static std::unique_ptr<ServerConnectEventArgs> server_connect_event_args;

    extern "C" {
        void server_connect_event_before() noexcept;
        void server_connect_event_after() noexcept;
        
        bool dispatch_server_connect_event_before(std::uint32_t &address, std::uint16_t &port, wchar_t *password) {
            server_connect_event_args = std::make_unique<ServerConnectEventArgs>(address, port, password);
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

    static bool debug_server_connect_event(int arg_count, const char **args) {
        static std::optional<Event::EventListenerHandle<ServerConnectEvent>> handle;
        if(arg_count == 1) {
            bool new_setting = STR_TO_BOOL(args[0]);
            if(new_setting) {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
                handle = Event::ServerConnectEvent::subscribe_const([](ServerConnectEvent const &event) {
                    auto &arguments = event.args;
                    auto time = event_time_to_string(event.time);
                    auto address = ip_address_int_to_string(arguments.address);
                    auto password = std::string(arguments.password.begin(), arguments.password.end());
                    logger.debug("Server connect event ({}): address: {}, port: {}, password: {}", time, address, arguments.port, password);
                });
            }
            else {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
            }
        }
        logger.info("debug_server_connect_event: {}", handle.has_value());
        return true;
    }

    template <>
    void EventHandler<ServerConnectEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

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

        // Register debug command
        register_command("debug_server_connect_event", "debug", "Debug server connect event", "[enable]", debug_server_connect_event, true, 0, 1);
    }
}
