// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/api.hpp>
#include <balltze/legacy_api/event.hpp>
#include <balltze/hook.hpp>
#include <balltze/command.hpp>
#include "../../config/config.hpp"
#include "../../logger.hpp"

namespace Balltze::LegacyApi::Event {
    static std::string current_map_name;

    extern "C" {
        void map_load_before_event();
        void map_load_after_event();

        void dispatch_map_load_event_before(const char *map_name) {
            current_map_name = map_name;
            std::size_t pos = current_map_name.find_last_of('\\');
            if(pos != std::string::npos) {
                current_map_name = current_map_name.substr(pos + 1);
            }
            MapLoadEventContext args(current_map_name);
            MapLoadEvent event(EVENT_TIME_BEFORE, args);
            event.dispatch();
        }

        void dispatch_map_load_event_after() {
            MapLoadEventContext args(current_map_name);
            MapLoadEvent event(EVENT_TIME_AFTER, args);
            event.dispatch();
        }
    }

    static bool debug_map_load_event(int arg_count, const char **args) {
        static std::optional<LegacyApi::Event::EventListenerHandle<MapLoadEvent>> handle;
        if(arg_count == 1) {
            bool new_setting = STR_TO_BOOL(args[0]);
            if(new_setting) {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
                handle = LegacyApi::Event::MapLoadEvent::subscribe_const([](MapLoadEvent const &event) {
                    auto &context = event.context;
                    auto time = event_time_to_string(event.time);
                    logger.debug("Map load event ({}): map name: {}", time, context.name);
                });
            }
            else {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
            }
        }
        logger.info("debug_map_load_event: {}", handle.has_value());
        return true;
    }

    template<>
    void EventHandler<MapLoadEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        auto loading_screen_enabled = Config::get_config().get<bool>("loading_screen.enable");
        if(!loading_screen_enabled.value_or(true)) {
            auto *load_map_function_multiplayer_callload_map_path_sig = Memory::get_signature("load_map_function_multiplayer_call");
            auto *load_map_function_singleplayer_call_sig = Memory::get_signature("load_map_function_singleplayer_call");
            if(!load_map_function_multiplayer_callload_map_path_sig || !load_map_function_singleplayer_call_sig) {
                logger.error("Failed to find map load event signatures");
                return;
            }

            try {
                Memory::hook_function(load_map_function_multiplayer_callload_map_path_sig->data(), map_load_before_event, map_load_after_event);
                Memory::hook_function(load_map_function_singleplayer_call_sig->data(), map_load_before_event, map_load_after_event);
            }
            catch(std::runtime_error &e) {
                logger.error("failed to initialize map load event: {}", e.what());
            }
        }

        // Register debug command
        register_command("debug_map_load_event", "debug", "Sets whenever to log map load event.", "[enable: boolean]", debug_map_load_event, true, 0, 1);
    }

    extern "C" {
        void map_file_load_client_hook_asm();
        void map_file_load_server_hook_asm();

        void dispatch_map_file_load_event_before(char *map_path, const char *map_name) {
            if(!map_path || !map_name) {
                logger.debug("dispatch_map_file_load_event_before: map_path or map_name is null");
            }

            std::string name = map_name;
            if(name == "levels\\ui\\ui") {
                name = "ui";
            }

            MapFileLoadEventContext args(name, map_path);
            MapFileLoadEvent event(EVENT_TIME_BEFORE, args);
            event.dispatch();
        }
    }

    static bool debug_map_file_load_event(int arg_count, const char **args) {
        static std::optional<LegacyApi::Event::EventListenerHandle<MapFileLoadEvent>> handle;
        if(arg_count == 1) {
            bool new_setting = STR_TO_BOOL(args[0]);
            if(new_setting) {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
                handle = LegacyApi::Event::MapFileLoadEvent::subscribe_const([](MapFileLoadEvent const &event) {
                    auto &context = event.context;
                    auto time = event_time_to_string(event.time);
                    logger.debug("Map file load event ({}): map name: {}, map path: {}", time, context.map_name, context.map_path);
                });
            }
            else {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
            }
        }
        logger.info("debug_map_file_load_event: {}", handle.has_value());
        return true;
    }

    template<>
    void EventHandler<MapFileLoadEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        try {
            auto *load_map_path_sig = Memory::get_signature("map_load_path");
            std::uint8_t load_map_path_instruction = *reinterpret_cast<std::uint8_t *>(load_map_path_sig->data());
            std::byte *load_map_path_addr = load_map_path_sig->data();

            if(load_map_path_instruction == 0xE9) {
                load_map_path_addr = Memory::follow_32bit_jump(load_map_path_addr) + 14; // let Chimera overwrite the map path
            }

            if(get_balltze_side() == BALLTZE_SIDE_CLIENT) {
                auto *load_map_path_hook = Memory::hook_function(load_map_path_addr, map_file_load_client_hook_asm);
            }
            else {
                auto *load_map_path_hook = Memory::hook_function(load_map_path_addr, map_file_load_server_hook_asm);
            }
        }
        catch(std::runtime_error &e) {
            logger.error("failed to initialize map loading event: {}", e.what());
        }

        // Register debug command
        register_command("debug_map_file_load_event", "debug", "Sets whenever to log map file load event.", "[enable: boolean]", debug_map_file_load_event, true, 0, 1);
    }

    extern "C" {
        void map_file_data_read_event_before();
        void map_file_data_read_event_after();

        void dispatch_map_file_data_read_event_before(HANDLE file_descriptor, std::byte *output, std::size_t *size, LPOVERLAPPED overlapped) {
            MapFileDataReadEventContext args;
            args.file_handle = file_descriptor;
            args.output_buffer = output;
            args.size = *size;
            args.overlapped = overlapped;
            MapFileDataReadEvent event(EVENT_TIME_BEFORE, args);
            event.dispatch();
            *size = event.context.size;
        }

        void dispatch_map_file_data_read_event_after(HANDLE file_descriptor, std::byte *output, std::size_t size, LPOVERLAPPED overlapped) {
            MapFileDataReadEventContext args;
            args.file_handle = file_descriptor;
            args.output_buffer = output;
            args.size = size;
            args.overlapped = overlapped;
            MapFileDataReadEvent event(EVENT_TIME_AFTER, args);
            event.dispatch();
        }
    }

    static bool debug_map_file_data_read_event(int arg_count, const char **args) {
        static std::optional<LegacyApi::Event::EventListenerHandle<MapFileDataReadEvent>> handle;
        if(arg_count == 1) {
            bool new_setting = STR_TO_BOOL(args[0]);
            if(new_setting) {
                if(handle) {
                    handle->remove();
                    handle = std::nullopt;
                }
                handle = LegacyApi::Event::MapFileDataReadEvent::subscribe_const([](MapFileDataReadEvent const &event) {
                    auto &context = event.context;
                    auto time = event_time_to_string(event.time);
                    logger.debug("Map file data read event ({}): file handle: {}, output buffer: {}, size: {}, overlapped: {}", time, reinterpret_cast<std::uint32_t>(context.file_handle), reinterpret_cast<std::uint32_t>(context.output_buffer), context.size, context.overlapped->Offset);
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
    void EventHandler<MapFileDataReadEvent>::init() {
        static bool enabled = false;
        if(enabled) {
            return;
        }
        enabled = true;

        auto *read_map_file_data_call_1_sig = Memory::get_signature("read_map_file_data_call_1");
        auto *read_map_file_data_call_2_sig = Memory::get_signature("read_map_file_data_call_2");
        
        if(!read_map_file_data_call_1_sig || !read_map_file_data_call_2_sig) {
            throw std::runtime_error("Could not find signatures for map file data read event");
        }

        try {
            Memory::hook_function(read_map_file_data_call_1_sig->data(), map_file_data_read_event_before, map_file_data_read_event_after);
            Memory::hook_function(read_map_file_data_call_2_sig->data(), map_file_data_read_event_before, map_file_data_read_event_after);
        }
        catch(std::runtime_error &e) {
            throw std::runtime_error("Could not hook map file data read event: " + std::string(e.what()));
        }

        // Register debug command
        register_command("debug_map_file_data_read_event", "debug", "Sets whenever to log map data read event", "[enable: boolean]", debug_map_file_data_read_event, true, 0, 1);
    }
}
